// SMART LIGHTING USING LDR WITH ESP32, LCD AND BLYNK IOT

#define BLYNK_TEMPLATE_ID "YOUR_TEMPLATE_ID"
#define BLYNK_TEMPLATE_NAME "YOUR_TEMPLATE_NAME"
#define BLYNK_AUTH_TOKEN "YOUR_AUTH_TOKEN"

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

char ssid[] = "YOUR WIFI NAME";
char pass[] = "YOUR WIFI PASSWORD";

#define LDR_PIN 34      // LDR sensor connected to Analog pin 34
#define LED_PIN 2       // LED connected to GPIO pin 2

int threshold = 1000;  // Light threshold (adjust based on your environment)

LiquidCrystal_I2C lcd(0x27, 16, 2);  // LCD I2C address

BlynkTimer timer;

// Function to read sensor data and update LCD & Blynk
void sendSensorData()
{
  int ldrValue = analogRead(LDR_PIN); // Read ADC value from LDR (0 to 4095)

  // Send ADC value to Blynk Virtual Pin V1
  Blynk.virtualWrite(V1, ldrValue);

  // Display ADC value on LCD
  lcd.setCursor(0, 0);
  lcd.print("LDR VALUE:");
  lcd.print(ldrValue);
  lcd.print("    ");

  // Print ADC value in Serial Monitor
  Serial.print("LDR VALUE: ");
  Serial.println(ldrValue);

  // if-else block
  // If it is dark (sensor value drops below threshold)
  if (ldrValue < threshold)
  {
    digitalWrite(LED_PIN, HIGH); // Turn ON light

    lcd.setCursor(0, 1);
    lcd.print("LIGHT ON     "); // Display status on LCD

    // Send light status to Blynk Virtual Pin V2
    Blynk.virtualWrite(V2, 1);

    Serial.println("Light ON");
  }

  // If it is bright
  else
  {
    digitalWrite(LED_PIN, LOW); // Turn OFF light

    lcd.setCursor(0, 1);
    lcd.print("LIGHT OFF    "); // Display status on LCD

    // Send light status to Blynk Virtual Pin V2
    Blynk.virtualWrite(V2, 0);

    Serial.println("Light OFF");
  }
}

void setup()
{
  pinMode(LED_PIN, OUTPUT);
  pinMode(LDR_PIN, INPUT);

  Serial.begin(115200); // Start serial monitor

  lcd.init(); // Initiating I2C LCD screen
  lcd.backlight();

  lcd.setCursor(0, 0);
  lcd.print("SMART LIGHTING");

  // Connect ESP32 to Wi-Fi and Blynk Cloud
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  // Execute sendSensorData() every 1 second
  timer.setInterval(1000L, sendSensorData);

  delay(2000);
  lcd.clear();
}

void loop()
{
  Blynk.run(); // Maintain connection with Blynk server

  timer.run(); // Run timer events
}
