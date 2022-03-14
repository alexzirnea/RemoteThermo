#include <nRF24L01.h>
#include <RF24.h>

#define NRF_CE 30 //PF4
#define NRF_CSN 31 //PF5
#define NRF_IRQ 32 //PF6

//#define NRF_CE D4
//#define NRF_CSN D8

RF24 radio(NRF_CE, NRF_CSN, 1000000); // CE, CSN

const byte address[5] = { 0x01, 0x01, 0x01, 0x01, 0x01 };
void setup() {
Serial.begin(115200);
radio.begin();
radio.openReadingPipe(0, address);   //Setting the address at which we will receive the data
//radio.setPALevel(RF24_PA_MIN);       //You can set this as minimum or maximum depending on the distance between the transmitter and receiver.
radio.setDataRate(RF24_250KBPS);

radio.setChannel(100);
radio.printDetails();
radio.startListening();              //This sets the module as receiver
radio.printDetails();

Serial.println("Listening...");
}
void loop()
{
  float x;
delay(500);
if (radio.available())              //Looking for the data.
{
  
char payload[32];                 //Saving the incoming data
float poolTemp, ambientTemp;
radio.read(&payload, 32);    //Reading the data
poolTemp = byteArrayToFloat((uint8_t *)payload);
ambientTemp = byteArrayToFloat((uint8_t *)(payload+4));
Serial.print("Pool Temp: ");
Serial.print(poolTemp);
Serial.println("C");
Serial.print("Ambient Temp: ");
Serial.print(ambientTemp);
Serial.println("C");

}
}

float byteArrayToFloat(uint8_t* temp_p ) {
    uint8_t i;
    float x=0;
    for (i = 0; i < 4; i++) {
        *(((uint8_t*)&x)+i)=temp_p[i];
    }
    return x;
}
