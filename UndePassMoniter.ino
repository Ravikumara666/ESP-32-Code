#include <Arduino.h>
#include <WiFi.h>
#include <FirebaseESP32.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <addons/TokenHelper.h>

// 1. WiFi and Firebase setup
#define WIFI_SSID "YourWiFiNetwork"
#define WIFI_PASSWORD "YourWiFiPassword"
#define API_KEY "YourAPIKey123456789"
#define DATABASE_URL "https://example-database.firebaseio.com/"
#define USER_EMAIL "example@example.com"
#define USER_PASSWORD "ExamplePassword123"

// Firebase objects
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

// Time interval setup
unsigned long sendDataPrevMillis = 0;
unsigned long interval = 10000; // 10 seconds

// Pin definitions
const int waterLevelPin = 34;  // Analog pin for water level sensor
const int buzzerPin = 26;      // Digital pin for buzzer
const int relayChannel1 = 27;  // Relay pin for water pump
const int relayChannel2 = 14;  // Relay pin for DC motor

// Variables for water level tracking
float currentWaterLevel = 0.0;
float lowestWaterLevel = 100.0; // Initialized high to track the lowest level correctly
float highestWaterLevel = 0.0;
bool zeroLevelSent = false;

// NTP Client for time synchronization
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 19800, 60000); // Offset is for IST (UTC +5:30)

// Additional variables for pump control
bool pumpActive = false;
unsigned long pumpStartTime = 0;
const unsigned long pumpDuration = 13000; // 13 seconds

// Function to read stable water levels
float getStableWaterLevel() {
  int readings = 10;
  float total = 0;

  for (int i = 0; i < readings; i++) {
    total += analogRead(waterLevelPin);
    delay(10); // Small delay between readings
  }

  float averageReading = total / readings;

  // Adjust the mapping to increase the water level by 15 cm
  averageReading = map(averageReading, 0, 4095, 0, 100); // Original mapping
  averageReading += 15; // Add 15 cm to the result
  
  return constrain(averageReading, 0, 100); // Constrain to 100 cm max
}

// Function to track water levels
void updateWaterLevelExtremes(float waterLevel) {
  static float lastReading = -1;

  if (abs(waterLevel - lastReading) > 2) { // Ignore fluctuations < 2 cm
    lowestWaterLevel = min(lowestWaterLevel, waterLevel);
    highestWaterLevel = max(highestWaterLevel, waterLevel);
    lastReading = waterLevel;
  }
}

// Setup function
void setup() {
  Serial.begin(115200);

  // Initialize pins
  pinMode(buzzerPin, OUTPUT);
  digitalWrite(buzzerPin, LOW);

  pinMode(relayChannel1, OUTPUT);
  pinMode(relayChannel2, OUTPUT);

  // Set relay pins HIGH initially to keep the relays off (active-low logic)
  digitalWrite(relayChannel1, HIGH);
  digitalWrite(relayChannel2, HIGH);

  // Connect to Wi-Fi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());

  // Firebase setup
  config.api_key = API_KEY;
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;
  config.database_url = DATABASE_URL;
  config.token_status_callback = tokenStatusCallback; // For token generation

  Firebase.begin(&config, &auth);
  Firebase.reconnectNetwork(true);
  fbdo.setBSSLBufferSize(4096, 1024);

  // Initialize NTP
  timeClient.begin();
}

// Loop function
void loop() {
  timeClient.update();

  if (Firebase.ready() && (millis() - sendDataPrevMillis > interval || sendDataPrevMillis == 0)) {
    sendDataPrevMillis = millis();

    // Get a stable water level reading
    currentWaterLevel = getStableWaterLevel();

    if (currentWaterLevel > 0) {
      updateWaterLevelExtremes(currentWaterLevel);
      zeroLevelSent = false;

      // Buzzer control
      if (currentWaterLevel >= 60) {
        digitalWrite(buzzerPin, HIGH);
      } else {
        digitalWrite(buzzerPin, LOW);
      }

      // Pump and motor control
      if (currentWaterLevel >= 60 && !pumpActive) {
        digitalWrite(relayChannel1, LOW);
        pumpStartTime = millis();
        pumpActive = true;
      }

      if (pumpActive && (millis() - pumpStartTime >= pumpDuration)) {
        digitalWrite(relayChannel1, HIGH);
        pumpActive = false;
      }

      if (currentWaterLevel >= 60) {
        digitalWrite(relayChannel2, LOW);
        delay(2000);
        digitalWrite(relayChannel2, HIGH);
      } else {
        digitalWrite(relayChannel2, HIGH);
      }

      // Upload data to Firebase
      String currentDate = getFormattedDate();
      String currentTime = getFormattedTime();
      uploadDataToFirebase(currentDate, currentTime);
    } else {
      if (!zeroLevelSent) {
        String currentDate = getFormattedDate();
        String currentTime = getFormattedTime();
        uploadZeroWaterLevel(currentDate, currentTime);
        zeroLevelSent = true;
      }
      digitalWrite(buzzerPin, LOW);
      digitalWrite(relayChannel1, HIGH);
      digitalWrite(relayChannel2, HIGH);
    }
  }
}

// Function to upload water level data to Firebase
void uploadDataToFirebase(String currentDate, String currentTime) {
  FirebaseJson json;
  json.set("current", currentWaterLevel);
  json.set("highest", highestWaterLevel);
  json.set("lowest", lowestWaterLevel);

  String path = "/waterLevel/dates/" + currentDate + "/" + currentTime;
  if (Firebase.set(fbdo, path, json)) {
    Serial.println("Data uploaded successfully");
  } else {
    Serial.print("Error uploading data: ");
    Serial.println(fbdo.errorReason());
  }
}

// Function to upload zero water level data to Firebase
void uploadZeroWaterLevel(String currentDate, String currentTime) {
  FirebaseJson json;
  json.set("current", 0);
  json.set("highest", highestWaterLevel);
  json.set("lowest", lowestWaterLevel);

  String path = "/waterLevel/dates/" + currentDate + "/" + currentTime;
  if (Firebase.updateNode(fbdo, path, json)) {
    Serial.println("Zero water level uploaded successfully");
  } else {
    Serial.print("Error uploading zero data: ");
    Serial.println(fbdo.errorReason());
  }
}

// Helper function to get formatted date
String getFormattedDate() {
  unsigned long epochTime = timeClient.getEpochTime();
  unsigned long currentDateTime = epochTime + 19800;
  time_t rawtime = currentDateTime;
  struct tm *timeinfo = localtime(&rawtime);
  char buffer[80];
  strftime(buffer, sizeof(buffer), "%Y-%m-%d", timeinfo);
  return String(buffer);
}

// Helper function to get formatted time
String getFormattedTime() {
  return timeClient.getFormattedTime();
}
