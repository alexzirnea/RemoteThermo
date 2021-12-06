/* Includes ------------------------------------------------------------------*/
#include "DEV_Config.h"
#include "EPD.h"
#include "GUI_Paint.h"
#include <stdlib.h>

#define Imagesize (((EPD_2IN13D_WIDTH % 8 == 0) ? (EPD_2IN13D_WIDTH / 8 ) : (EPD_2IN13D_WIDTH / 8 + 1)) * EPD_2IN13D_HEIGHT)

#define CS_1 4
#define CS_2 7
#define BUSY_1 3
#define BUSY_2 2
/* Entry point ----------------------------------------------------------------*/
UBYTE BlackImage[Imagesize];
char disp_buffer[10];
int8_t temp1 = -1, temp2 = -20;
void setup()
{

  pinMode(CS_1, OUTPUT);
  pinMode(CS_2, OUTPUT);
  pinMode(BUSY_1, OUTPUT);
  pinMode(BUSY_2, OUTPUT);
  printf("EPD_2IN13D_test Demo\r\n");

  digitalWrite(CS_1, LOW);
  digitalWrite(CS_2, HIGH);
  DEV_Module_Init();

  digitalWrite(CS_1, HIGH);
  digitalWrite(CS_2, LOW);
  DEV_Module_Init();
  
  printf("e-Paper Init and Clear...\r\n");
  EPD_2IN13D_Init();
  EPD_2IN13D_Clear();
  DEV_Delay_ms(500);

  Paint_NewImage(BlackImage, EPD_2IN13D_WIDTH, EPD_2IN13D_HEIGHT, 270, WHITE);
  Paint_SelectImage(BlackImage);
}

/* The main loop -------------------------------------------------------------*/
void loop()
{
  digitalWrite(CS_1, LOW);
  digitalWrite(CS_2, HIGH);
  DEV_Delay_ms(2000);
  Paint_DrawString_EN(5, 5, "POOL TEMP:", &Font16, BLACK, WHITE);
  itoa(temp1, disp_buffer, 10);
  Paint_DrawString_EN(20, 24, disp_buffer, &Font24, BLACK, WHITE);
  Paint_DrawString_EN(5, 60, "OUTSIDE TEMP:", &Font16, BLACK, WHITE);
  itoa(temp2, disp_buffer, 10);
  Paint_DrawString_EN(20, 79, disp_buffer, &Font24, BLACK, WHITE);
  EPD_2IN13D_Display(BlackImage);

  digitalWrite(CS_1, HIGH);
  digitalWrite(CS_2, LOW);
  DEV_Delay_ms(2000);
  Paint_Clear(WHITE);
  Paint_DrawString_EN(5, 5, "POOL TEMP:", &Font16, BLACK, WHITE);
  itoa(temp1, disp_buffer, 10);
  Paint_DrawString_EN(20, 24, disp_buffer, &Font24, BLACK, WHITE);
  Paint_DrawString_EN(5, 60, "OUTSIDE TEMP:", &Font16, BLACK, WHITE);
  itoa(temp2, disp_buffer, 10);
  Paint_DrawString_EN(20, 79, disp_buffer, &Font24, BLACK, WHITE);
  EPD_2IN13D_Display(BlackImage);
  temp1++;
  temp2++;
}
