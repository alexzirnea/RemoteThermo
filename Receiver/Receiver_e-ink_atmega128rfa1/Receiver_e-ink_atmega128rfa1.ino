/* E-ink includes */
#include "DEV_Config.h"
#include "EPD.h"
#include "GUI_Paint.h"
#include <stdlib.h>

/* NRF24L01 Includes */
#include <nRF24L01.h>
#include <RF24.h>
#include "printf.h"

#define Imagesize (((EPD_2IN13D_WIDTH % 8 == 0) ? (EPD_2IN13D_WIDTH / 8) : (EPD_2IN13D_WIDTH / 8 + 1)) * EPD_2IN13D_HEIGHT)
#define STALE_DATA_TIMEOUT 30000
/* E-ink pins */
#define CS_1 4
#define CS_2 7
#define BUSY_1 3
#define BUSY_2 2

/* NRF24L01 pins */
#define NRF_CE 30 //PF4
#define NRF_CSN 31 //PF5
#define NRF_IRQ 32 //PF6

#define MAX_READINGS_NUMBER 8

RF24 radio(NRF_CE, NRF_CSN, 1000000); // CE, CSN
const byte address[5] = {0x01, 0x01, 0x01, 0x01, 0x01};
uint32_t last_recv_millis;


UBYTE BlackImage[Imagesize];
char disp_buffer[10];
uint8_t readings = 0;
double ambientTempSum = 0, poolTempSum = 0;

void setup()
{

  Serial.begin(115200);
  printf_begin();
  Serial.println("test");
  radio.begin();
  radio.openReadingPipe(0, address); //Setting the address at which we will receive the data
  radio.setDataRate(RF24_250KBPS);

  radio.setChannel(100);
  radio.startListening(); //This sets the module as receiver

  pinMode(CS_1, OUTPUT);
  pinMode(CS_2, OUTPUT);
  //pinMode(BUSY_1, OUTPUT);
  //pinMode(BUSY_2, OUTPUT);

  digitalWrite(CS_1, LOW);
  digitalWrite(CS_2, LOW);
  DEV_Module_Init();
  digitalWrite(CS_1, HIGH);
  digitalWrite(CS_2, HIGH);

  digitalWrite(CS_1, LOW);
  digitalWrite(CS_2, LOW);
  EPD_2IN13D_Init();
  digitalWrite(CS_1, HIGH);
  digitalWrite(CS_2, HIGH);

  digitalWrite(CS_1, LOW);
  digitalWrite(CS_2, LOW);
  EPD_2IN13D_Clear();
  digitalWrite(CS_1, HIGH);
  digitalWrite(CS_2, HIGH);

  Paint_NewImage(BlackImage, EPD_2IN13D_WIDTH, EPD_2IN13D_HEIGHT, 270, WHITE);
  Paint_SelectImage(BlackImage);

  Paint_DrawString_EN(5, 5, "Initializing RF...", &Font16, BLACK, WHITE);
  digitalWrite(CS_1, LOW);
  digitalWrite(CS_2, LOW);
  EPD_2IN13D_Display(BlackImage);
  digitalWrite(CS_1, HIGH);
  digitalWrite(CS_2, HIGH);

  if (1)
  {
    Paint_DrawString_EN(5, 20, "RF Initialized!", &Font16, BLACK, WHITE);
  }
  else
  {
    Paint_DrawString_EN(5, 20, "RF init fail!", &Font16, BLACK, WHITE);
  }

  if (radio.isChipConnected())
  {
    Paint_DrawString_EN(5, 35, "nRF24 Connected", &Font16, BLACK, WHITE);
  }
  else
  {
    Paint_DrawString_EN(5, 35, "nRF24 Not available!", &Font16, BLACK, WHITE);
  }
  Paint_DrawString_EN(5, 50, "Waiting for data..", &Font16, BLACK, WHITE);
  digitalWrite(CS_1, LOW);
  digitalWrite(CS_2, LOW);
  EPD_2IN13D_Display(BlackImage);
  digitalWrite(CS_1, HIGH);
  digitalWrite(CS_2, HIGH);

  radio.printDetails();

  last_recv_millis = millis();
}

void loop()
{
  bool validData = false;
  char payload[32]; //Saving the incoming data
  float poolTemp, ambientTemp;

  if (radio.available()) //Looking for the data.
  {
    radio.read(payload, 32); //Reading the data
    poolTemp = byteArrayToFloat((uint8_t *)payload);
    ambientTemp = byteArrayToFloat((uint8_t *)(payload + 4));
    validData = true;
    last_recv_millis = millis();
  }

  if (validData)
  {
    poolTempSum += poolTemp;
    ambientTempSum += ambientTemp;
    readings++;
  }

  if (readings >= MAX_READINGS_NUMBER)
  {
    Paint_Clear(WHITE);
    Paint_DrawString_EN(5, 5, "POOL TEMP:", &Font16, BLACK, WHITE);
    tempFloatToChar((float)(poolTempSum / MAX_READINGS_NUMBER), disp_buffer);
    Serial.println(disp_buffer);
    Paint_DrawString_EN(20, 24, disp_buffer, &Font24, BLACK, WHITE);
    Paint_DrawString_EN(5, 60, "OUTSIDE TEMP:", &Font16, BLACK, WHITE);
    tempFloatToChar((float)(ambientTempSum / MAX_READINGS_NUMBER), disp_buffer);
    Paint_DrawString_EN(20, 79, disp_buffer, &Font24, BLACK, WHITE);
    digitalWrite(CS_1, LOW);
    digitalWrite(CS_2, LOW);
    EPD_2IN13D_Display(BlackImage);
    digitalWrite(CS_1, HIGH);
    digitalWrite(CS_2, HIGH);
    DEV_Delay_ms(2000);
    readings = 0;
    poolTempSum = 0;
    ambientTempSum = 0;
  }

  if ((millis() - last_recv_millis) >= STALE_DATA_TIMEOUT)
  {
    Paint_DrawString_EN(150, 80, "STALE", &Font16, BLACK, WHITE);
    digitalWrite(CS_1, LOW);
    digitalWrite(CS_2, LOW);
    EPD_2IN13D_Display(BlackImage);
    digitalWrite(CS_1, HIGH);
    digitalWrite(CS_2, HIGH);
    last_recv_millis = millis();
  }
}

void tempFloatToChar(float temp, char* buf_p)
{
  if (temp >= 10.00)
  {
    dtostrf(temp, 5, 2, buf_p);
  }
  else if (temp >= 0.00)
  {
    dtostrf(temp, 4, 2, buf_p);
  }
  else if (temp > -10.00)
  {
    dtostrf(temp, 5, 2, buf_p);
  }
  else
  {
    dtostrf(temp, 6, 2, buf_p);
  }
}

float byteArrayToFloat(uint8_t *temp_p)
{
  uint8_t i;
  float x = 0;
  for (i = 0; i < 4; i++)
  {
    *(((uint8_t *)&x) + i) = temp_p[i];
  }
  return x;
}
