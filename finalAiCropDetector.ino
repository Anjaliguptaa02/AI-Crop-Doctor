#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>
#include <ESP32Servo.h>

// ================= OLED =================
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define OLED_ADDR 0x3C

#define SDA_PIN 21
#define SCL_PIN 22

Adafruit_SSD1306 display(
  SCREEN_WIDTH,
  SCREEN_HEIGHT,
  &Wire,
  OLED_RESET
);

// ================= SENSORS =================
#define SOIL_PIN 34
#define RAIN_PIN 35

#define DHT_PIN 4
#define DHT_TYPE DHT11

DHT dht(DHT_PIN, DHT_TYPE);

// ================= SERVO =================
#define SERVO_PIN 18
Servo servo;

// ================= LED =================
#define LED_PIN 2


void setup() {

  Serial.begin(115200);

  // ---------- I2C ----------
  Wire.begin(SDA_PIN, SCL_PIN);

  // ---------- OLED ----------
  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
    Serial.println("OLED NOT FOUND!");
    while (true);
  }

  // ---------- DHT ----------
  dht.begin();

  // ---------- LED ----------
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  // ---------- SERVO ----------
  servo.attach(SERVO_PIN);
  servo.write(0);

  // ---------- Starting screen ----------
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);

  display.setTextSize(1);
  display.setCursor(18, 5);
  display.println("AI CROP DETECTOR");

  display.setCursor(25, 25);
  display.println("Starting...");

  display.display();

  delay(2000);
}


void loop() {

  // =================================================
  // READ SENSORS
  // =================================================

  int soilRaw = analogRead(SOIL_PIN);
  int rainRaw = analogRead(RAIN_PIN);

  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();


  // =================================================
  // CONVERT TO PERCENTAGE
  // =================================================

  // Soil:
  // Dry -> high ADC
  // Wet -> low ADC
  int soilPercent = map(soilRaw, 4095, 0, 0, 100);
  soilPercent = constrain(soilPercent, 0, 100);


  // Rain sensor:
  // Usually wet/rain -> lower ADC
  int rainPercent = map(rainRaw, 4095, 0, 0, 100);
  rainPercent = constrain(rainPercent, 0, 100);


  // =================================================
  // DETERMINE CONDITIONS
  // =================================================

  String soilStatus;

  if (soilPercent < 30) {
    soilStatus = "DRY";
  }
  else if (soilPercent < 70) {
    soilStatus = "NORMAL";
  }
  else {
    soilStatus = "WET";
  }


  String rainStatus;

  if (rainPercent > 30) {
    rainStatus = "RAIN";
  }
  else {
    rainStatus = "NO RAIN";
  }


  // =================================================
  // SERVO + LED LOGIC
  // =================================================

  if (soilPercent < 30 && rainPercent < 30) {

    // Soil dry + no rain
    // Servo ON
    servo.write(90);

    // LED ON
    digitalWrite(LED_PIN, HIGH);

  }
  else {

    // No watering required
    servo.write(0);

    digitalWrite(LED_PIN, LOW);
  }


  // =================================================
  // SERIAL MONITOR
  // =================================================

  Serial.println();
  Serial.println("========== CROP STATUS ==========");

  Serial.print("Soil Raw: ");
  Serial.println(soilRaw);

  Serial.print("Soil Moisture: ");
  Serial.print(soilPercent);
  Serial.println("%");

  Serial.print("Rain Raw: ");
  Serial.println(rainRaw);

  Serial.print("Rain: ");
  Serial.print(rainPercent);
  Serial.println("%");

  Serial.print("Temperature: ");

  if (isnan(temperature))
    Serial.println("ERROR");
  else {
    Serial.print(temperature);
    Serial.println(" C");
  }

  Serial.print("Humidity: ");

  if (isnan(humidity))
    Serial.println("ERROR");
  else {
    Serial.print(humidity);
    Serial.println("%");
  }

  Serial.print("Soil Status: ");
  Serial.println(soilStatus);

  Serial.print("Rain Status: ");
  Serial.println(rainStatus);


  // =================================================
  // OLED DISPLAY
  // =================================================

  display.clearDisplay();

  display.setTextColor(SSD1306_WHITE);

  display.setTextSize(1);

  display.setCursor(12, 0);
  display.println("AI CROP DETECTOR");

  display.drawLine(0, 11, 127, 11, SSD1306_WHITE);


  display.setCursor(0, 15);
  display.print("Soil: ");
  display.print(soilPercent);
  display.print("% ");
  display.println(soilStatus);


  display.setCursor(0, 27);
  display.print("Rain: ");
  display.print(rainPercent);
  display.print("% ");
  display.println(rainStatus);


  display.setCursor(0, 39);
  display.print("Temp: ");

  if (isnan(temperature))
    display.print("--");
  else
    display.print(temperature, 1);

  display.println(" C");


  display.setCursor(0, 51);
  display.print("Humidity: ");

  if (isnan(humidity))
    display.print("--");
  else
    display.print(humidity, 0);

  display.println("%");


  display.display();


  delay(2000);
}