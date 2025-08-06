/*********
RX

  Complete project details at https://RandomNerdTutorials.com/ttgo-lora32-sx1276-arduino-ide/
*********/

//Libraries for LoRa
#include <SPI.h>
#include <LoRa.h>

//Libraries for OLED Display
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

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


Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RST);

// Buffer for incoming commands
char commandBuffer[512]; // Assuming commands won't exceed 32 characters
int bufferIndex = 0;

String LoRaData;
String teststr;

// Example RGB colors
uint8_t colors[6][3] = {
  { 255, 0, 0 },    // Red
  { 0, 255, 0 },    // Green
  { 0, 0, 255 },    // Blue
  { 255, 255, 0 },  // Yellow
  { 0, 255, 255 },  // Cyan
  { 255, 0, 255 }   // Magenta
};



void processCommand(char* command) {
  int led, r, g, b;
  if (sscanf(command, "%d,%d,%d,%d", &led, &r, &g, &b) == 4) { // Parse command
    if (led >= 0 && led < 6) {
      colors[led][0] = r;
      colors[led][1] = g;
      colors[led][2] = b;
      Serial.print("Updated LED ");
      Serial.print(led);
      Serial.print(" to RGB(");
      Serial.print(r);
      Serial.print(", ");
      Serial.print(g);
      Serial.print(", ");
      Serial.print(b);
      Serial.println(")");
   
      sendColors(); // Send the new color configuration via LoRa
    } else {
    //  Serial.println("Invalid LED number");
    }
  } else {
   // Serial.println("Invalid command format");
  }
}



void sendColors() {
  LoRa.beginPacket();
  for (int i = 0; i < 6; i++) {
    for (int j = 0; j < 3; j++) {
      LoRa.write(colors[i][j]);  // Send each color component
    /*  Serial.print("Sending color[");
      Serial.print(i);
      Serial.print("][");
      Serial.print(j);
      Serial.print("]: ");
      Serial.println(colors[i][j]);
    */
    }
  }
  LoRa.endPacket();
  Serial.println("Colors sent via LoRa");
}

void setup() {
  //initialize Serial Monitor
  Serial.setTimeout(5);
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
  display.print("LORA RECEIVER ");
  display.display();

  Serial.println("LoRa Receiver Test");

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
  display.println("LoRa Initializing OK!");
  display.display();





  //sendColors();
}

void loop() {
  // LoRa Packet Reception
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    LoRaData = LoRa.readString();
   // Serial.println("Received LoRa Packet: " + LoRaData);
    Serial.println(LoRaData);

    // Display LoRa Data
    display.clearDisplay();
    display.setCursor(0, 0);
    display.print("LORA RECEIVER");
    display.setCursor(0, 20);
    display.print("Received:");
    display.setCursor(0, 30);
    display.print(LoRaData);
    display.display();
  }

  // Serial Command Input (Non-blocking)
  while (Serial.available() > 0) {
    char inChar = Serial.read();

    if (inChar == '\n' || inChar == '\r') {
      if (bufferIndex > 0) {
        commandBuffer[bufferIndex] = '\0'; // Terminate string
        Serial.print("Command Received: ");
        Serial.println(commandBuffer);

        // Process Command
        processCommand(commandBuffer);

        // Send command string over LoRa for testing
        LoRa.beginPacket();
        LoRa.print(commandBuffer);
        LoRa.endPacket();

        bufferIndex = 0;
      }
    } else if (bufferIndex < sizeof(commandBuffer) - 1) {
      commandBuffer[bufferIndex++] = inChar;
    }
  }
}


