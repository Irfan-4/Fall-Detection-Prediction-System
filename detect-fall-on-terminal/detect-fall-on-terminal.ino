// #include <Wire.h>
// #include <ESP8266WiFi.h>

// const int MPU_addr = 0x68;  // I2C address of the MPU-6050
// int16_t AcX, AcY, AcZ, Tmp, GyX, GyY, GyZ;
// float ax = 0, ay = 0, az = 0, gx = 0, gy = 0, gz = 0;
// boolean fall = false; //stores if a fall has occurred
// boolean trigger1 = false; //stores if first trigger (lower threshold) has occurred
// boolean trigger2 = false; //stores if second trigger (upper threshold) has occurred
// boolean trigger3 = false; //stores if third trigger (orientation change) has occurred
// byte trigger1count = 0; //stores the counts past since trigger 1 was set true
// byte trigger2count = 0; //stores the counts past since trigger 2 was set true
// byte trigger3count = 0; //stores the counts past since trigger 3 was set true
// int angleChange = 0;

// // WiFi network info.
// const char *ssid = "Naazriyaz-2.4G"; // Enter your Wi-Fi Name
// const char *pass = "abdu0312"; // Enter your Wi-Fi Password

// void setup() {
//   Serial.begin(9600);
//   Wire.begin();
//   Wire.beginTransmission(MPU_addr);
//   Wire.write(0x6B);  // PWR_MGMT_1 register
//   Wire.write(0);     // set to zero (wakes up the MPU-6050)
//   Wire.endTransmission(true);
//   Serial.println("Wrote to IMU");

//   Serial.println("Connecting to WiFi...");
//   WiFi.begin(ssid, pass);
//   while (WiFi.status() != WL_CONNECTED) {
//     delay(500);
//     Serial.print(".");
//   }
//   Serial.println("");
//   Serial.println("WiFi connected");

//   // Delay to allow WiFi connection to stabilize
//   delay(2000);
// }

// void loop() {
//   mpu_read();
//   ax = (AcX - 2050) / 16384.00;
//   ay = (AcY - 77) / 16384.00;
//   az = (AcZ - 1947) / 16384.00;
//   gx = (GyX + 270) / 131.07;
//   gy = (GyY - 351) / 131.07;
//   gz = (GyZ + 136) / 131.07;

//   // calculating Amplitude vector for 3 axes
//   float Raw_Amp = pow(pow(ax, 2) + pow(ay, 2) + pow(az, 2), 0.5);
//   int Amp = Raw_Amp * 10;  // Multiplied by 10 because values are between 0 to 1
//   Serial.println(Amp);

//   if (Amp <= 2 && trigger2 == false && trigger1 == false) { // If Amplitude breaks lower threshold (0.4g)
//     trigger1 = true;
//     Serial.println("TRIGGER 1 ACTIVATED");
//   }
//   if (trigger1 == true) {
//     trigger1count++;
//     if (Amp >= 12) { // If Amplitude breaks upper threshold (3g)
//       trigger2 = true;
//       Serial.println("TRIGGER 2 ACTIVATED");
//       Serial.println("FALL DETECTED");
//       trigger1 = false; trigger1count = 0;
//     }
//   }
//   if (trigger2 == true) {
//     trigger2count++;
//     angleChange = pow(pow(gx, 2) + pow(gy, 2) + pow(gz, 2), 0.5);
//     Serial.println(angleChange);
//     if (angleChange >= 80 && angleChange <= 100) { // If orientation changes by between 80-100 degrees
//       trigger3 = true; trigger2 = false; trigger2count = 0;
//       Serial.println(angleChange);
//       Serial.println("TRIGGER 3 ACTIVATED");
//     }
//   }
//   if (trigger3 == true) {
//     trigger3count++;
//     if (trigger3count >= 10) {
//       angleChange = pow(pow(gx, 2) + pow(gy, 2) + pow(gz, 2), 0.5);
//       Serial.println(angleChange);
//       if ((angleChange >= 0) && (angleChange <= 10)) { // If orientation changes remains between 0-10 degrees
//         fall = true; trigger3 = false; trigger3count = 0;
//         Serial.println(angleChange);
//       }
//       else { // User regained normal orientation
//         trigger3 = false; trigger3count = 0;
//         Serial.println("TRIGGER 3 DEACTIVATED");
//       }
//     }
//   }
//   if (fall == true) { // In event of a fall detection
//     Serial.println("FALL DETECTED");
//     // Add any additional actions you want to perform when a fall is detected
//     fall = false; // Reset fall flag
//   }

//   if (trigger2count >= 6) { // Allow 0.5s for orientation change
//     trigger2 = false; trigger2count = 0;
//     Serial.println("TRIGGER 2 DEACTIVATED");
//   }
//   if (trigger1count >= 6) { // Allow 0.5s for Amplitude to break upper threshold
//     trigger1 = false; trigger1count = 0;
//     Serial.println("TRIGGER 1 DEACTIVATED");
//   }

//   delay(100);
// }

// void mpu_read() {
//   Wire.beginTransmission(MPU_addr);
//   Wire.write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H)
//   Wire.endTransmission(false);
//   Wire.requestFrom((uint8_t)MPU_addr, 14, true);
//  // request a total of 14 registers
//   AcX = Wire.read() << 8 | Wire.read();  // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)    
//   AcY = Wire.read() << 8 | Wire.read();  // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
//   AcZ = Wire.read() << 8 | Wire.read();  // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
//   Tmp = Wire.read() << 8 | Wire.read();  // 0x41 (TEMP_OUT_H) & 0x42 (TEMP_OUT_L)
//   GyX = Wire.read() << 8 | Wire.read();  // 0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
//   GyY = Wire.read() << 8 | Wire.read();  // 0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
//   GyZ = Wire.read() << 8 | Wire.read();  // 0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)
// }


#define BLYNK_TEMPLATE_ID "TMPL3Rl-6KqnR"
#define BLYNK_TEMPLATE_NAME "Smart Fall Detection"
#include <Wire.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

char auth[] = "9oXwgB8I5W7NplyOborr6512F_bR_eBl";
const char *ssid = "Ir"; // Enter your Wi-Fi Name
const char *pass = "12345678"; // Enter your Wi-Fi Password
//  const char *ssid = "Naazriyaz-2.4G"; // Enter your Wi-Fi Name
//  const char *pass = "abdu0312"; 
// const char *ssid = "tp09"; // Enter your Wi-Fi Name
// const char *pass = "vasavicc"; // Enter your Wi-Fi Password

const int MPU_addr = 0x68;
int16_t AcX, AcY, AcZ, Tmp, GyX, GyY, GyZ;
float ax = 0, ay = 0, az = 0, gx = 0, gy = 0, gz = 0;
boolean fall = false;
boolean trigger1 = false;
boolean trigger2 = false;
boolean trigger3 = false;
byte trigger1count = 0;
byte trigger2count = 0;
byte trigger3count = 0;
int angleChange = 0;

BlynkTimer timer;

void setup() {
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass);
  Wire.begin();
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x6B);
  Wire.write(0);
  Wire.endTransmission(true);
  Serial.println("Wrote to IMU");

  Serial.println("Connecting to WiFi...");
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  delay(2000);

  // Setup timer to send data every second
  timer.setInterval(1000L, sendData);
}

void loop() {
  Blynk.run();
  timer.run();
}

void sendData() {
  mpu_read();
  ax = (AcX - 2050) / 16384.00;
  ay = (AcY - 77) / 16384.00;
  az = (AcZ - 1947) / 16384.00;
  gx = (GyX + 270) / 131.07;
  gy = (GyY - 351) / 131.07;
  gz = (GyZ + 136) / 131.07;

  float Raw_Amp = pow(pow(ax, 2) + pow(ay, 2) + pow(az, 2), 0.5);
  int Amp = Raw_Amp * 10;
  Serial.println(Amp);

  if (Amp <= 7 && trigger2 == false && trigger1 == false) {
    trigger1 = true;
    Serial.println("TRIGGER 1 ACTIVATED");
    Blynk.virtualWrite(V1, 1); // Trigger automation rule
  }

  if (trigger1 == true) {
    trigger1count++;
    if (Amp >= 10) {
      trigger2 = true;
      Serial.println("TRIGGER 2 ACTIVATED");
      Serial.println("FALL DETECTED");
      trigger1 = false; trigger1count = 0;
      Blynk.virtualWrite(V1, 2); // Trigger automation rule
    }
  }

  if (trigger2 == true) {
    trigger2count++;
    angleChange = pow(pow(gx, 2) + pow(gy, 2) + pow(gz, 2), 0.5);
    Serial.println(angleChange);
    if (angleChange >= 80 && angleChange <= 100) {
      trigger3 = true; trigger2 = false; trigger2count = 0;
      Serial.println(angleChange);
      Serial.println("TRIGGER 3 ACTIVATED");
      Blynk.virtualWrite(V1, 2); // Trigger automation rule
    }
  }

  if (trigger3 == true) {
    trigger3count++;
    if (trigger3count >= 8) {
      angleChange = pow(pow(gx, 2) + pow(gy, 2) + pow(gz, 2), 0.5);
      Serial.println(angleChange);
      if ((angleChange >= 0) && (angleChange <= 10)) {
        fall = true; trigger3 = false; trigger3count = 0;
        Serial.println(angleChange);
        Blynk.virtualWrite(V1, 2); // Trigger automation rule
      }
      else {
        trigger3 = false; trigger3count = 0;
        Serial.println("TRIGGER 3 DEACTIVATED");
      }
    }
  }

  if (fall == true) {
    Serial.println("FALL DETECTED");
    Blynk.virtualWrite(V1, 0); // Trigger automation rule
    fall = false;
  }

  if (trigger2count >= 6) {
    trigger2 = false; trigger2count = 0;
    Serial.println("TRIGGER 2 DEACTIVATED");
  }

  if (trigger1count >= 6) {
    trigger1 = false; trigger1count = 0;
    Serial.println("TRIGGER 1 DEACTIVATED");
  }
}

void mpu_read() {
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x3B);
  Wire.endTransmission(false);
  Wire.requestFrom((uint8_t)MPU_addr, 14, true);
  AcX = Wire.read() << 8 | Wire.read();    
  AcY = Wire.read() << 8 | Wire.read();
  AcZ = Wire.read() << 8 | Wire.read();
  Tmp = Wire.read() << 8 | Wire.read();
  GyX = Wire.read() << 8 | Wire.read();
  GyY = Wire.read() << 8 | Wire.read();
  GyZ = Wire.read() << 8 | Wire.read();
}