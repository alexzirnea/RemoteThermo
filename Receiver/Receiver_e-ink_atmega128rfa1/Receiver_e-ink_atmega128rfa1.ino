/* E-ink includes */
#include "DEV_Config.h"
#include "EPD.h"
#include "GUI_Paint.h"
#include <stdlib.h>

/* NRF24L01 Includes */
#include <nRF24L01.h>
#include <RF24.h>

#define Imagesize (((EPD_2IN13D_WIDTH % 8 == 0) ? (EPD_2IN13D_WIDTH / 8) : (EPD_2IN13D_WIDTH / 8 + 1)) * EPD_2IN13D_HEIGHT)

/* E-ink pins */
#define CS_1 4
#define CS_2 7
#define BUSY_1 3
#define BUSY_2 2

/* NRF24L01 pins */
#define NRF_CE PF4
#define NRF_CSN PF5
#define NRF_IRQ PF6

RF24 radio(NRF_CE, NRF_CSN); // CE, CSN
const byte address[5] = {0x01, 0x01, 0x01, 0x01, 0x01};

UBYTE BlackImage[Imagesize];
char disp_buffer[10];
int8_t temp1 = -1, temp2 = -20;

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

void setup()
{

  pinMode(CS_1, OUTPUT);
  pinMode(CS_2, OUTPUT);
  pinMode(BUSY_1, OUTPUT);
  pinMode(BUSY_2, OUTPUT);

  pinMode(NRF_CE, OUTPUT);
  pinMode(NRF_CSN, OUTPUT);

  //printf("EPD_2IN13D_test Demo\r\n");

  radio.openReadingPipe(0, address); //Setting the address at which we will receive the data
  //radio.setPALevel(RF24_PA_MIN);       //You can set this as minimum or maximum depending on the distance between the transmitter and receiver.
  radio.setDataRate(RF24_250KBPS);

  radio.setChannel(100);
  radio.printDetails();
  radio.startListening(); //This sets the module as receiver

  digitalWrite(CS_1, LOW);
  digitalWrite(CS_2, HIGH);
  DEV_Module_Init();

  digitalWrite(CS_1, HIGH);
  digitalWrite(CS_2, LOW);
  DEV_Module_Init();

  //printf("e-Paper Init and Clear...\r\n");
  EPD_2IN13D_Init();
  EPD_2IN13D_Clear();
  DEV_Delay_ms(500);

  Paint_NewImage(BlackImage, EPD_2IN13D_WIDTH, EPD_2IN13D_HEIGHT, 270, WHITE);
  Paint_SelectImage(BlackImage);
}

void loop()
{
  bool validData = false;
  char payload[32]; //Saving the incoming data
  float poolTemp, ambientTemp;
    
  if (radio.available()) //Looking for the data.
  {
    radio.read(&payload, 32); //Reading the data
    poolTemp = byteArrayToFloat((uint8_t *)payload);
    ambientTemp = byteArrayToFloat((uint8_t *)(payload + 4));
    validData = true;
    /*
    Serial.print("Pool Temp: ");
    Serial.print(poolTemp);
    Serial.println("C");
    Serial.print("Ambient Temp: ");
    Serial.print(ambientTemp);
    Serial.println("C");
    */
  }
  if (validData)
  {
    digitalWrite(CS_1, LOW);
    digitalWrite(CS_2, HIGH);
    DEV_Delay_ms(2000);
    Paint_DrawString_EN(5, 5, "POOL TEMP:", &Font16, BLACK, WHITE);
    itoa(poolTemp, disp_buffer, 10);
    Paint_DrawString_EN(20, 24, disp_buffer, &Font24, BLACK, WHITE);
    Paint_DrawString_EN(5, 60, "OUTSIDE TEMP:", &Font16, BLACK, WHITE);
    itoa(ambientTemp, disp_buffer, 10);
    Paint_DrawString_EN(20, 79, disp_buffer, &Font24, BLACK, WHITE);
    EPD_2IN13D_Display(BlackImage);

    digitalWrite(CS_1, HIGH);
    digitalWrite(CS_2, LOW);
    DEV_Delay_ms(2000);
    Paint_Clear(WHITE);
    Paint_DrawString_EN(5, 5, "POOL TEMP:", &Font16, BLACK, WHITE);
    itoa(poolTemp, disp_buffer, 10);
    Paint_DrawString_EN(20, 24, disp_buffer, &Font24, BLACK, WHITE);
    Paint_DrawString_EN(5, 60, "OUTSIDE TEMP:", &Font16, BLACK, WHITE);
    itoa(ambientTemp, disp_buffer, 10);
    Paint_DrawString_EN(20, 79, disp_buffer, &Font24, BLACK, WHITE);
    EPD_2IN13D_Display(BlackImage);
    temp1++;
    temp2++;
  }
}
