/* Taskcam v2 software for IRS Taskcam v2 Camera Shield + IRS Taskcam Camera Module */
/* Written by Andy Sheen 2017 */

/*~~~~~~~~~~TO DO~~~~~~~~~~*/
/* - Add Button and switch names
 * - Add power down function
 * - Add animation to text 
 * - Add question display
 * - Add photo capture animation
 * - Remove Adafruit splash (sorry Lady Ada...) 
 * - Add Photo hash tally 
 * - Add Contrast/Light/FX menu (bonus)
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

  //PWR Pin
  pinMode(10, OUTPUT);
  digitalWrite(10, 1);

  pinMode(5, OUTPUT);
  digitalWrite(5, 1);


  //Buttons
  pinMode(7, INPUT_PULLUP);
  pinMode(8, INPUT_PULLUP);
  pinMode(9, INPUT_PULLUP);
  pinMode(6, INPUT_PULLUP);

  Serial.begin(57600);
  while (!Serial) {
  }
  //Camera Module Interface
  mySerial.begin(38400);

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3D (for the 128x64)
  display.display();
  delay(2000);
  display.clearDisplay();

  //Init CAM
  mySerial.write('~');
  mySerial.write('i');
  mySerial.write(0x0D);
  mySerial.write(0x0A);
  while (mySerial.available() < 2) {
    //WAIT
    delay(1);
  }
  //Wait for 'INI'
  for (int i = 0 ; i < 3; i++) {
    Serial.print((char)mySerial.read());
  }
  Serial.println();

  //Index Questions
  mySerial.write('~');
  mySerial.write('+');
  mySerial.write(0x0D);
  mySerial.write(0x0A);
  while (mySerial.available() < 2) {
    delay(1);
  }
  //Returns number of Qs + new line
  for (int i = 0 ; i < 3; i++) {
    Serial.print(mySerial.read());
  }
  Serial.println();
  delay(100);

  // q + question num return Qs
  mySerial.write('q');
  mySerial.write(0x04);
  mySerial.write(0x0D);
  mySerial.write(0x0A);

  //prints out the Question.... TO FIX
  while (mySerial.available() < 63) {
  }
  inputBuffer = new char[64];
  for (int i = 0; i < 64; i++) {
    inputBuffer[i] = (char)mySerial.read();
  }
  Serial.println(inputBuffer);
  for (int i = 0 ; i < 100; i++) {
    mySerial.read();
  }
}

void loop() { // run over and over

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
    //Take picture
    mySerial.write('~');
    mySerial.write('!');
    mySerial.write(0x0D);
    mySerial.write(0x0A);
    while (mySerial.available() < 0) {
      delay(1);
    }
    //Ack for when picture finished saving... needs fixing as hang over from oversized Q in buffer
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
