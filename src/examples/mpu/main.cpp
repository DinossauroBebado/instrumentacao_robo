//Import all required libraries
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>



#include "mpu.h"


bool _imu_connect; 
bool _connect = false;
const int MPU6050_addr = 0x68;
int16_t AccX, AccY, AccZ, Temp, GyroX, GyroY, GyroZ;
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
void setup() {
  Serial.begin(9600);
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { //Ive changed the address //already chill
    Serial.println(F("SSD1306 allocation failed"));
    for (;;); // Don't proceed, loop forever
  }
  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
  display.display();
  delay(2000); // Pause for 2 seconds
  // Clear the buffer
  // Show the display buffer on the screen. You MUST call display() after
  // drawing commands to make them visible on screen!

  _imu_connect = imu_setup();


  Wire.endTransmission(true);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setRotation(0);
}

void loop() {
  float* imu_ypr = imu_get_ypr();  
  //retunr from + pi to -pi 
  Serial.print("Orientation ");
  Serial.print(imu_ypr[0], 5);
  Serial.print(", ");
  Serial.print(imu_ypr[1],5);
  Serial.print(", ");
  Serial.print(imu_ypr[2],5);
  Serial.println("");
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("   MICRODIGISOFT");
  display.println("");
  display.println("   MPU WITH OLED");
  display.println("Gyroscope - rps");
  display.print(imu_ypr[0], 2);
  display.print(", ");
  display.print(imu_ypr[1],2);
  display.print(", ");
  display.print(imu_ypr[2],2);
  display.println("");
  display.display();
  delay(200); // Pause for 2 seconds
}