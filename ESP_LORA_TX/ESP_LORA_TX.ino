/*********
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/ttgo-lora32-sx1276-arduino-ide/

  LORA TX 
*********/

//Libraries for LoRa
#include <SPI.h>
#include <LoRa.h>

#define Button_1 34
#define Button_2 35
#define Button_3 36
#define Button_4 39
#define Button_5 13
#define Button_6 15

// Define button pins and their corresponding messages
const int buttonPins[] = { Button_1, Button_2, Button_3, Button_4, Button_5, Button_6 };
const char* buttonMessages[] = { "B1", "B2", "B3", "B4", "B5", "B6" };

volatile bool dataAvailable = false;
uint8_t receivedBuffer[18];





//-- for islx bitmap
#define imageWidth 128
#define imageHeight 32




//---islx bitmap
const unsigned char islxLogo[] PROGMEM = {
  // 'undefined, 128x32px
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x0f, 0x80, 0x1f, 0xff, 0xff, 0x81, 0xf0, 0x00, 0x00, 0x1f, 0xe0, 0x00, 0x0f, 0xf0, 0x00,
  0x00, 0x0f, 0x80, 0x7f, 0xff, 0xff, 0x81, 0xf0, 0x00, 0x00, 0x07, 0xf0, 0x00, 0x1f, 0xc0, 0x00,
  0x00, 0x0f, 0x80, 0xff, 0xff, 0xff, 0x81, 0xf0, 0x00, 0x00, 0x03, 0xf8, 0x00, 0x3f, 0x80, 0x00,
  0x00, 0x0f, 0x81, 0xff, 0xff, 0xff, 0x81, 0xf0, 0x00, 0x00, 0x01, 0xfc, 0x00, 0x7f, 0x00, 0x00,
  0x00, 0x0f, 0x81, 0xff, 0xff, 0xff, 0x81, 0xf0, 0x00, 0x00, 0x00, 0xfe, 0x00, 0xff, 0x00, 0x00,
  0x00, 0x0f, 0x83, 0xf8, 0x00, 0x00, 0x01, 0xf0, 0x00, 0x00, 0x00, 0xff, 0x01, 0xfe, 0x00, 0x00,
  0x00, 0x0f, 0x83, 0xf0, 0x00, 0x00, 0x01, 0xf0, 0x00, 0x00, 0x00, 0x7f, 0x01, 0xfc, 0x00, 0x00,
  0x00, 0x0f, 0x83, 0xe0, 0x00, 0x00, 0x01, 0xf0, 0x00, 0x00, 0x00, 0x3f, 0x83, 0xf8, 0x00, 0x00,
  0x00, 0x0f, 0x83, 0xe0, 0x00, 0x00, 0x01, 0xf0, 0x00, 0x00, 0x00, 0x1f, 0xc7, 0xf0, 0x00, 0x00,
  0x00, 0x0f, 0x83, 0xe0, 0x00, 0x00, 0x01, 0xf0, 0x00, 0x00, 0x00, 0x0f, 0xef, 0xe0, 0x00, 0x00,
  0x00, 0x0f, 0x83, 0xf0, 0x00, 0x00, 0x01, 0xf0, 0x00, 0x00, 0x00, 0x07, 0xff, 0xc0, 0x00, 0x00,
  0x00, 0x0f, 0x83, 0xfc, 0x00, 0x00, 0x01, 0xf0, 0x00, 0x00, 0x00, 0x07, 0xff, 0xc0, 0x00, 0x00,
  0x00, 0x0f, 0x81, 0xff, 0xff, 0xfe, 0x01, 0xf0, 0x00, 0x00, 0x00, 0x03, 0xff, 0x80, 0x00, 0x00,
  0x00, 0x0f, 0x80, 0xff, 0xff, 0xff, 0x01, 0xf0, 0x00, 0x00, 0x00, 0x01, 0xff, 0x00, 0x00, 0x00,
  0x00, 0x0f, 0x80, 0xff, 0xff, 0xff, 0x81, 0xf0, 0x00, 0x00, 0x00, 0x00, 0xfe, 0x00, 0x00, 0x00,
  0x00, 0x0f, 0x80, 0x3f, 0xff, 0xff, 0xc1, 0xf0, 0x00, 0x00, 0x00, 0x01, 0xff, 0x00, 0x00, 0x00,
  0x00, 0x0f, 0x80, 0x0f, 0xff, 0xff, 0xe1, 0xf0, 0x00, 0x00, 0x00, 0x03, 0xff, 0x80, 0x00, 0x00,
  0x00, 0x0f, 0x80, 0x00, 0x00, 0x07, 0xe1, 0xf0, 0x00, 0x00, 0x00, 0x07, 0xff, 0xc0, 0x00, 0x00,
  0x00, 0x0f, 0x80, 0x00, 0x00, 0x03, 0xe1, 0xf0, 0x00, 0x00, 0x00, 0x0f, 0xff, 0xe0, 0x00, 0x00,
  0x00, 0x0f, 0x80, 0x00, 0x00, 0x03, 0xe1, 0xf0, 0x00, 0x00, 0x00, 0x0f, 0xef, 0xf0, 0x00, 0x00,
  0x00, 0x0f, 0x80, 0x00, 0x00, 0x03, 0xe1, 0xf0, 0x00, 0x00, 0x00, 0x1f, 0xc7, 0xf0, 0x00, 0x00,
  0x00, 0x0f, 0x80, 0x00, 0x00, 0x03, 0xe1, 0xf0, 0x00, 0x00, 0x00, 0x3f, 0x83, 0xf8, 0x00, 0x00,
  0x00, 0x0f, 0x80, 0x00, 0x00, 0x07, 0xe1, 0xf0, 0x00, 0x00, 0x00, 0x7f, 0x01, 0xfc, 0x00, 0x00,
  0x00, 0x0f, 0x80, 0x00, 0x00, 0x0f, 0xe1, 0xf0, 0x00, 0x00, 0x00, 0xfe, 0x00, 0xfe, 0x00, 0x00,
  0x00, 0x0f, 0x83, 0xff, 0xff, 0xff, 0xc1, 0xff, 0xff, 0xff, 0xc1, 0xfc, 0x00, 0x7f, 0x00, 0x00,
  0x00, 0x0f, 0x83, 0xff, 0xff, 0xff, 0xc1, 0xff, 0xff, 0xff, 0xc3, 0xfc, 0x00, 0x7f, 0x80, 0x00,
  0x00, 0x0f, 0x83, 0xff, 0xff, 0xff, 0x81, 0xff, 0xff, 0xff, 0xc3, 0xf8, 0x00, 0x3f, 0xc0, 0x00,
  0x00, 0x0f, 0x83, 0xff, 0xff, 0xff, 0x01, 0xff, 0xff, 0xff, 0xc7, 0xf0, 0x00, 0x1f, 0xc0, 0x00,
  0x00, 0x0f, 0x83, 0xff, 0xff, 0xfc, 0x00, 0xff, 0xff, 0xff, 0xdf, 0xe0, 0x00, 0x0f, 0xf0, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};






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
  display.clearDisplay();  // stop the diaply of adafruit logo
  display.drawBitmap(0, 0, islxLogo, 128, 32, 1);

  display.display();

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
  //display.setCursor(0, 10);
  //display.print("LoRa Initializing OK!");
  //display.display();
  delay(2000);


  LoRa.onReceive(onReceive);
  LoRa.receive();  // Put LoRa in receive mode
  Serial.println("LoRa initialized. Waiting for data...");

  pinMode(Button_1, INPUT);
  pinMode(Button_2, INPUT);  // Set the new button pins as INPUT_PULLUP
  pinMode(Button_3, INPUT);
  pinMode(Button_4, INPUT);
  pinMode(Button_5, INPUT);
  pinMode(Button_6, INPUT);

  strip.begin();
  strip.setBrightness(255);
  strip.show();


  // Initialize all pixels to 'off'
  colorWipe(strip.Color(255, 0, 0), 100);  // Red
  colorWipe(strip.Color(0, 255, 0), 100);  // Green

  colorWipe(strip.Color(0, 0, 255), 100);  // Blue

  rainbowCycle(2);

  strip.setBrightness(127);
  strip.setPixelColor(0, 255, 0, 0);  //RED
  strip.setPixelColor(1, 0, 255, 0);  //Yellow
  strip.setPixelColor(2, 0, 0, 255);  //Green
  strip.setPixelColor(3, 255, 0, 0);  //RED
  strip.setPixelColor(4, 0, 255, 0);  //Yellow
  strip.setPixelColor(5, 0, 0, 255);  //Green
                                      //


  strip.setPixelColor(0, 10, 10, 10);  //RED
  strip.setPixelColor(1, 10, 10, 10);  //Yellow
  strip.setPixelColor(2, 10, 10, 10);  //Green
  strip.setPixelColor(3, 10, 10, 10);  //RED
  strip.setPixelColor(4, 10, 10, 10);  //Yellow
  strip.setPixelColor(5, 10, 10, 10);  //Green


  strip.show();

  // attachInterrupt(34, isrButton1, FALLING);
  // attachInterrupt(35, isrButton2, FALLING);  // Attach interrupts to the new button pins
  // attachInterrupt(36, isrButton3, FALLING);
}


void loop() {



  if (dataAvailable) {
    //  esp_task_wdt_reset(); // Reset watchdog
    dataAvailable = false;
    handlePacket();
  }

  checkButtons();




  display.clearDisplay();
  display.setCursor(0, 0);
  display.println(String(digitalRead(Button_1)) + " " + String(digitalRead(Button_2)) + " " + String(digitalRead(Button_3)) + " " + String(digitalRead(Button_4)) + " " + String(digitalRead(Button_5)) + " " + String(digitalRead(Button_6)));
  //display.setCursor(0, 20);
  //display.setTextSize(1);
  //display.print(String(digitalRead(35)));
  //display.setCursor(0, 30);
  ///display.print(String(digitalRead(36)));
  display.setCursor(50, 30);
  display.print(counter);
  display.setCursor(0, 40);
  display.print(LoRaData);
  display.setCursor(0, 40);
  int rssi = LoRa.packetRssi();
  display.print("RSSI ");
  display.println(String(map(rssi, -120, -30, 0, 100)) + "%");
  display.display();

  counter++;
}



void checkButtons() {
  for (int i = 0; i < sizeof(buttonPins) / sizeof(buttonPins[0]); i++) {
    if (digitalRead(buttonPins[i]) == LOW) {
      LoRa.beginPacket();
      LoRa.print(buttonMessages[i]);
      LoRa.endPacket();
    }
  }
}




void onReceive(int packetSize) {
  if (packetSize != 18) {
    return;  // Ignore invalid packets
  }

  for (int i = 0; i < packetSize; i++) {
    if (LoRa.available()) {
      receivedBuffer[i] = LoRa.read();
    }
  }
  dataAvailable = true;  // Flag for processing in the main loop
}

void handlePacket() {
  uint8_t receivedColors[6][3];
  for (int i = 0; i < 18; i++) {
    receivedColors[i / 3][i % 3] = receivedBuffer[i];
  }

  for (int i = 0; i < 6; i++) {
    strip.setPixelColor(i, strip.Color(receivedColors[i][0], receivedColors[i][1], receivedColors[i][2]));
  }
  strip.show();
}


void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for (j = 0; j < 256 * 5; j++) {  // 5 cycles of all colors on wheel
    for (i = 0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) - j + 256) & 255));
    }
    strip.show();
    //setRGBLED();
    delay(wait);
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if (WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if (WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for (uint16_t i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    //setRGBLED();
    delay(wait);
  }
}
