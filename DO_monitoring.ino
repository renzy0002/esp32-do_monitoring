#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

// WiFi credentials
const char* ssid = "FRFR";
const char* password = "Ronniedeluvio00/";

// Local server details (replace with your PC's IP)
const char* serverUrl = "http://192.168.1.9/do_logger.php"; 

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected to WiFi");
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    // Generate random DO value (2.0 - 12.0 mg/L)
    float randomDO = random(20, 121) / 10.0;  // Range: 2.0 to 12.0
    
    // Create JSON payload
    StaticJsonDocument<200> doc;
    doc["sensor_id"] = 1;  // Match your sensor_id in database
    doc["do_level"] = randomDO;
    doc["alert"] = (randomDO < 4.0) ? 1 : 0;  // Alert if DO < 4.0 mg/L

    String payload;
    serializeJson(doc, payload);
    
    // Send to server
    HTTPClient http;
    http.begin(serverUrl);
    http.addHeader("Content-Type", "application/json");
    
    int httpCode = http.POST(payload);
    
    if (httpCode == HTTP_CODE_OK) {
      Serial.print("DO Value: ");
      Serial.print(randomDO);
      Serial.println(" mg/L - Sent to server!");
    } else {
      Serial.print("Error code: ");
      Serial.println(httpCode);
    }
    http.end();
  }
  delay(5000);  // Send every 5 seconds
}