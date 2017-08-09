/*
  Software serial multple serial test
  Receives from the hardware serial, sends to software serial.
  Receives from software serial, sends to hardware serial.
  The circuit:
   RX is digital pin 10 (connect to TX of other device)
   TX is digital pin 11 (connect to RX of other device)
  Note:
  Not all pins on the Mega and Mega 2560 support change interrupts,
  so only the following can be used for RX:
  10, 11, 12, 13, 50, 51, 52, 53, 62, 63, 64, 65, 66, 67, 68, 69
  Not all pins on the Leonardo support change interrupts,
  so only the following can be used for RX:
  8, 9, 10, 11, 14 (MISO), 15 (SCK), 16 (MOSI).
  created back in the mists of time
  modified 25 May 2012
  by Tom Igoe
  based on Mikal Hart's example
  This example code is in the public domain.
*/
#include <SoftwareSerial.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

char* inputBuffer;

bool flag = false;

SoftwareSerial mySerial(3, 2); // RX, TX

void setup() {

  pinMode(10, OUTPUT);
  digitalWrite(10, 1);
  pinMode(5, OUTPUT);
  digitalWrite(5, 1);
  //Serial.begin(9600);
  pinMode(7, INPUT_PULLUP);
  pinMode(8, INPUT_PULLUP);
  pinMode(9, INPUT_PULLUP);
  pinMode(6, INPUT_PULLUP);

  // Open serial communications and wait for port to open:
  pinMode(5, OUTPUT);
  digitalWrite(5, 1);;
  Serial.begin(57600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only



  }



  // set the data rate for the SoftwareSerial port
  mySerial.begin(38400);

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3D (for the 128x64)
  // init done

  // Show image buffer on the display hardware.
  // Since the buffer is intialized with an Adafruit splashscreen
  // internally, this will display the splashscreen.
  display.display();
  delay(2000);

  // Clear the buffer.
  display.clearDisplay();

  mySerial.write('~');
  mySerial.write('i');
  mySerial.write(0x0D);
  mySerial.write(0x0A);
  while (mySerial.available() < 2) {
    //WAIT
    delay(1);
  }
  for (int i = 0 ; i < 3; i++) {
    Serial.print((char)mySerial.read());
  }
  Serial.println();

  mySerial.write('~');
  mySerial.write('+');
  mySerial.write(0x0D);
  mySerial.write(0x0A);
  while (mySerial.available() < 2) {
    //WAIT
    delay(1000);
  }
  for (int i = 0 ; i < 3; i++) {
    Serial.print(mySerial.read());
  }
  Serial.println();
  delay(100);
  mySerial.write('q');
  mySerial.write(0x04);
  mySerial.write(0x0D);
  mySerial.write(0x0A);


  while (mySerial.available() < 63) {
  }
  inputBuffer = new char[64];
  for(int i = 0; i < 64; i++){
    inputBuffer[i] = (char)mySerial.read();
  }
  Serial.println(inputBuffer);
 for(int i = 0 ; i < 100; i++){
  mySerial.read();
 }
}

void loop() { // run over and over


  /*
    if (mySerial.available()) {
      Serial.write(mySerial.read());
    }
    if (Serial.available()) {
      mySerial.write(Serial.read());
    }
  */
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(10, 0);
  display.clearDisplay();
  display.println(inputBuffer);
  display.display();
  delay(1);
  display.display();

  if (digitalRead(6) == 1 && flag == true) {
    flag = false;
  }
  if (digitalRead(6) == 0 && flag == false) {
    display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(10, 0);
    display.clearDisplay();
    display.println("taking photo");
    display.display();
    delay(1);
    display.display();

    flag = true;
    mySerial.write('~');
    mySerial.write('!');
    mySerial.write(0x0D);
    mySerial.write(0x0A);
    while (mySerial.available() < 0) {
      delay(1);
    }
   
    if (mySerial.read() == 0x06) {
      display.clearDisplay();
      display.setTextSize(2);
      display.setTextColor(WHITE);
      display.setCursor(10, 0);
      display.clearDisplay();
      display.println("saved!");
      display.display();
      delay(1000);
      display.display();
    } else {
      display.clearDisplay();
      display.setTextSize(2);
      display.setTextColor(WHITE);
      display.setCursor(10, 0);
      display.clearDisplay();
      display.println("ERROR!");
      display.display();
      delay(1000);
      display.display();
    }
  }


}
