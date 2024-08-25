/*********
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/ttgo-lora32-sx1276-arduino-ide/

  LORA TX 
*********/

//Libraries for LoRa
#include <SPI.h>
#include <LoRa.h>

//Libraries for OLED Display
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>  // Required for 16 MHz Adafruit Trinket
#endif
//define the pins used by the LoRa transceiver module
#define SCK 5
#define MISO 19
#define MOSI 27
#define SS 18
#define RST 23
#define DIO0 26

//433E6 for Asia
//866E6 for Europe
//915E6 for North America
#define BAND 915E6

//OLED pins
#define OLED_SDA 21
#define OLED_SCL 22
#define OLED_RST -1
#define SCREEN_WIDTH 128  // OLED display width, in pixels
#define SCREEN_HEIGHT 64  // OLED display height, in pixels



//packet counter
int counter = 0;
String LoRaData;
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RST);


#define PIN 14

Adafruit_NeoPixel strip = Adafruit_NeoPixel(7, PIN, NEO_RGB + NEO_KHZ800);

uint32_t red = strip.Color(255, 0, 0);
uint32_t green = strip.Color(0, 255, 0);
uint32_t blue = strip.Color(0, 0, 255);
uint32_t white = strip.Color(255, 255, 255);
uint32_t yellow = strip.Color(255, 242, 3);
uint32_t pink = strip.Color(255, 0, 100);
uint32_t cyan = strip.Color(0, 255, 255);
uint32_t orange = strip.Color(230, 80, 0);

volatile bool button1Pressed = false;
volatile bool button2Pressed = false;
volatile bool button3Pressed = false;
/*
void IRAM_ATTR isrButton1() {
  button1Pressed = true;
  // LoRa.beginPacket();
   // LoRa.print("B3");
    //LoRa.endPacket();
}

void IRAM_ATTR isrButton2() {
  button2Pressed = true;
}

void IRAM_ATTR isrButton3() {
  button3Pressed = true;
   //LoRa.beginPacket();
    //LoRa.print("B3");
    //LoRa.endPacket();
}
*/



void setup() {
  //initialize Serial Monitor
  Serial.begin(115200);

  //reset OLED display via software
  pinMode(OLED_RST, OUTPUT);
  digitalWrite(OLED_RST, LOW);
  delay(20);
  digitalWrite(OLED_RST, HIGH);

  //initialize OLED
  Wire.begin(OLED_SDA, OLED_SCL);
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3c, false, false)) {  // Address 0x3C for 128x32
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ;  // Don't proceed, loop forever
  }

  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.print("LORA SENDER ");
  display.display();

  Serial.println("LoRa Sender Test");

  //SPI LoRa pins
  SPI.begin(SCK, MISO, MOSI, SS);
  //setup LoRa transceiver module
  LoRa.setPins(SS, RST, DIO0);

  if (!LoRa.begin(BAND)) {
    Serial.println("Starting LoRa failed!");
    while (1)
      ;
  }
  Serial.println("LoRa Initializing OK!");
  display.setCursor(0, 10);
  display.print("LoRa Initializing OK!");
  display.display();
  delay(2000);




  pinMode(34, INPUT);
  pinMode(35, INPUT);  // Set the new button pins as INPUT_PULLUP
  pinMode(36, INPUT);
  pinMode(39, INPUT);
  pinMode(13, INPUT);
  pinMode(15, INPUT);

  strip.begin();
  strip.setBrightness(255);
  strip.show();

  strip.setBrightness(127);
  strip.setPixelColor(0, 255, 255, 255);  //RED
  strip.setPixelColor(1, 255, 255, 255);  //Yellow
  strip.setPixelColor(2, 255, 255, 255);  //Green
  strip.setPixelColor(3, 255, 255, 255);  //RED
  strip.setPixelColor(4, 255, 255, 255);  //Yellow
  strip.setPixelColor(5, 255, 255, 255);  //Green
    strip.setPixelColor(6, 127, 127, 127);  //Green
  strip.show();

  // attachInterrupt(34, isrButton1, FALLING);
  // attachInterrupt(35, isrButton2, FALLING);  // Attach interrupts to the new button pins
  // attachInterrupt(36, isrButton3, FALLING);
}


void loop() {

  if (digitalRead(34) == LOW) {

    LoRa.beginPacket();
    LoRa.print("B1");
    LoRa.endPacket();
    // button1Pressed = false;
  }

  if (digitalRead(35) == LOW) {

    LoRa.beginPacket();
    LoRa.print("B2");
    LoRa.endPacket();
    // button2Pressed = false;
  }

  if (digitalRead(36) == LOW) {

    LoRa.beginPacket();
    LoRa.print("B3");
    LoRa.endPacket();
    // button3Pressed = false;
  }
  /*
LoRa.beginPacket();
  LoRa.print("hello ");
  LoRa.print(counter);
  LoRa.endPacket();
*/

  //delay(1000);







  //try to parse packet Recieved from companion
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    //received a packet
    //   Serial.print("Received packet ");

    //read packet
    while (LoRa.available()) {
      LoRaData = LoRa.readString();
      Serial.println(LoRaData);
    }

    if (LoRaData == "L1") {

      strip.setPixelColor(0, 255, 0, 0);  //RED

      strip.show();
    } else if (LoRaData == "L2") {
      strip.setPixelColor(1, 255, 0, 0);  //RED

      strip.show();

    } else if (LoRaData == "L3") {
      strip.setPixelColor(2, 255, 0, 0);  //RED

      strip.show();
    } else if (LoRaData == "P1") {
      strip.setPixelColor(0, 0, 255, 0);  //GREEN

      strip.show();
    } else if (LoRaData == "P2") {
      strip.setPixelColor(1, 0, 255, 0);  //GREEN

      strip.show();

    } else if (LoRaData == "P3") {
      strip.setPixelColor(2, 0, 255, 0);  //GREEN

      strip.show();
    } else if (LoRaData == "W1") {
      strip.setPixelColor(0, 255, 255, 255);  //GREEN

      strip.show();
    } else if (LoRaData == "W2") {
      strip.setPixelColor(1, 255, 255, 255);  //GREEN

      strip.show();

    } else if (LoRaData == "W3") {
      strip.setPixelColor(2, 255, 255, 255);  //GREEN

      strip.show();
    }



    //print RSSI of packet
    int rssi = LoRa.packetRssi();
    //  Serial.print(" with RSSI ");
    // Serial.println(rssi);
  }

  display.clearDisplay();
  display.setCursor(0, 0);
  display.println(String(digitalRead(34)));
  display.setCursor(0, 20);
  display.setTextSize(1);
  display.print(String(digitalRead(35)));
  display.setCursor(0, 30);
  display.print(String(digitalRead(36)));
  display.setCursor(50, 30);
  display.print(counter);
  display.setCursor(0, 40);
  display.print(LoRaData);


  display.display();

  counter++;
}