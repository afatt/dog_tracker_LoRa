/*********
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/ttgo-lora32-sx1276-arduino-ide/
*********/

//Libraries for LoRa
#include <SPI.h>
#include <LoRa.h>

//Libraries for OLED Display
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

//Libraries for GPS
#include <TinyGPS++.h>
#include <SoftwareSerial.h>

//define the pins used by the LoRa transceiver module
#define SCK 5
#define MISO 19
#define MOSI 27
#define SS 18
#define RST 14
#define DIO0 26

//433E6 for Asia
//866E6 for Europe
//915E6 for North America
#define BAND 915E6

//OLED pins
#define OLED_SDA 4
#define OLED_SCL 15 
#define OLED_RST 16
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

//packet counter
int counter = 0;

//GPS variables
float lat = 0.0; //latitude
float lon = 0.0; //longitude
static const int RXPin = 16, TXPin = 17;

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RST);

TinyGPSPlus gps;
SoftwareSerial ss(RXPin, TXPin);

void setup() {

  //reset OLED display via software
  pinMode(OLED_RST, OUTPUT);
  digitalWrite(OLED_RST, LOW);
  delay(20);
  digitalWrite(OLED_RST, HIGH);

  //initialize OLED
  Wire.begin(OLED_SDA, OLED_SCL);
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3c, false, false)) { // Address 0x3C for 128x32
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(0,0);
  display.print("LORA SENDER ");
  display.display();
  
  //initialize Serial Monitor
  //Serial.begin(115200);
  Serial.begin(9600);
  
  Serial.println("LoRa Sender Test");

  //SPI LoRa pins
  SPI.begin(SCK, MISO, MOSI, SS);
  //setup LoRa transceiver module
  LoRa.setPins(SS, RST, DIO0);
  
  if (!LoRa.begin(BAND)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
  Serial.println("LoRa Initializing OK!");
  display.setCursor(0,10);
  display.print("LoRa Initializing OK!");
  display.display();

  ss.begin(9600);
    
  delay(2000);
}

void loop() {
  
  while (ss.available() > 0){
    gps.encode(ss.read());
    if (gps.location.isUpdated()){
      Serial.print("Latitude= "); 
      Serial.print(gps.location.lat(), 6);
      Serial.print(" Longitude= "); 
      Serial.println(gps.location.lng(), 6);

      // add counter so it isnt transmitting so much
      
      LoRa.beginPacket();
      LoRa.print(gps.location.lat(), 6);
      LoRa.print(",");
      LoRa.print(gps.location.lng(), 6);
      LoRa.endPacket();
    }
  }
  
  //Serial.print("Sending packet: ");
  //Serial.println(counter);

  //Send LoRa packet to receiver
  /*LoRa.beginPacket();
  LoRa.print("Rx # ");
  LoRa.print(counter);
  LoRa.print(", ");
  LoRa.print("lat ");
  LoRa.print(lat);
  LoRa.print(", ");
  LoRa.print("lon ");
  LoRa.print(lon);
  LoRa.endPacket();
  */
 /*
  display.clearDisplay();
  display.setCursor(0,0);
  display.println("LORA SENDER");
  display.setCursor(0,20);
  display.setTextSize(1);
  display.print("LoRa packet sent.");
  display.setCursor(0,30);
  display.print("Counter:");
  display.setCursor(50,30);
  display.print(counter);
  // GPS DISPLAY
  display.setCursor(0,40);
  display.print("lat:");
  display.setCursor(30,40);
  display.print(lat);
  display.setCursor(0,50);
  display.print("lon:");
  display.setCursor(30,50);
  display.print(lon);
  display.display();

  counter++;
  lat++;
  lon++;
  
  delay(10000);*/
}
