
#include <FirebaseESP32.h>
#include <SPI.h>
#include <MFRC522.h>
#include <LiquidCrystal_I2C.h>
#include <WiFi.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);

#define FIREBASE_HOST "attendance-ed3eb-default-rtdb.firebaseio.com" 
#define SS_PIN 15  
#define RST_PIN 2  

#define WIFI_SSID "xxxxxxxxxxxxx"     // Replace with your WiFi SSID
#define WIFI_PASSWORD "xxxxxxxxxxx"  // Replace with your WiFi password

// Update Firebase config variables
const char* FIREBASE_PROJECT_ID = "attendance-ed3eb";
const char* FIREBASE_CLIENT_EMAIL = "firebase-adminsdk-fbsvc@attendance-ed3eb.iam.gserviceaccount.com";
const char* FIREBASE_PRIVATE_KEY =   "-----BEGIN PRIVATE KEY-----\nMIIEvgIBADANBgkqhkiG9w0BAQEFAASCBKgwggSkAgEAAoIBAQCn1Xz/OP7N796s\nLZYX3L/Z7A6aLhSzOcPIbRuYdRryPVrqlxY5xL4JPAZYR0jDFH4aaLvXWsYbaVV6\nMp+UIzuumqJ/kWcSXvQLEOhVT1eiNxPa5h7dfnVQi12lnwh+AZOYBqG4rs09KsNZ\nIrFNknKWc0S2fW6w4Q7oeio1nRZcQTNw4H3t1SvzbBcl9D3X4UYXjKSZ7SKkernU\nImRArtlqRTgjxZug4o66FwXk58vfiwEtmGj0bpiSuT8LPeS2ipCqGSaU4ic9EATE\nR4jCUWU8r1iqK4zRFPesfmtgpZO3utj+LILJW4p2Oj5am/YTxEnpdG8KFVyTERXZ\ncPdho4J5AgMBAAECggEAHd/25NBurNY8nw3rSHqbLO2jyMaYmTGYW9ouHhSEVCff\nQeBcjLJseGTWpUqlFVKVwtovS52FI5Z7qGmP9dTSuBkfAVGu1PqRetHK5Xqe2Qlw\nT5gEWP18YPa51/Qn1RQvQ1G04lS9RFzTEpAGGw6NNqI0kj5gIHyjRHcQgeQhOZD4\nUwDLzyD1F+GcQ7ywnmQaOAVcZL+r18oBj6+NYjCie/vab0x3SWJ89h8WZpXWs5Is\nCzW88qWTeUvh6NrzfixNp7JzAn/nU6UPKWenIu32KvwyG7NfqwIv3TbAruyLNWwf\nWPjQ5CN+E4rS3Zh8nAKqnF3KPhc1hcmPjKYRP+eOzQKBgQDoRuRl1QPlnEB/Dj6i\nWzqHvnd++8robAxMn9G+dvkplxadJvd+N04HrmD+kbgRv06DBvmTbRH6BOCIToxO\nhG07ZvdRsM3d4xXXPjRGjolAVu1kWUENDTAyjvcViYLx7e0XFoqokbZ4nr7E6Cyo\noHjIXlLzoOCCDwqu5GUVQ7SwLQKBgQC4+avNkyWuAp/TbFK+pVZUqQDF9XjCJWMD\nYL1FknPueV825oL5irMSgEvJ9s+afqqm7S4Hrh2K4MoLefJjC7MXlkRUVdB+GB5b\nk0HL8lR3dETMACvbwQWmuAKtVcRZg7o4iP+FN00+JaavcBykzmtUGHTA/o6OGAMP\nKGe31JC+/QKBgQDXUzDUPf6Y0prvrfLea5f80tLQAvd9MvzL47R6zqv4pDmwgbkZ\nZoeNk1gTZl6vBRvxV3vx8UCi9/Z2JernttfUI4XOUwrlUWij2uR3LBeMb4kuZ5do\nU/uEQwhHDO04/Ym2x4zO7BWuB+ttepY71Fdj2B0qAM9fgjYzH0OdzOAqGQKBgQCJ\nE0xDnYXQu9Ug4ic3bGLbfho6mLwSc446VJguVGdhE8SBdLUeo7AsR7LiSepyvOdN\nqHnx6w+teQOBGa+LwoHHOu4L6UQGDW2Bw1CBCGWb2CebXY0i3pmHgNlLBkHpa/q2\nJp/kYqT1JwWtANHZN7bFrRHoazvNBPJPa0vhKL7TKQKBgHkvv23sLda7UeXFdavS\nzw0WdHD5XutnYnlBjoCr/Q/OOYJ7uB93SEVHtuwmX4xKs+2M+6N1VB29F3uyEfw7\n1DxSf0Ho9MNSJ9qwaWaln7AjrEIIkXIzZ+EuaerxNKVEAubjhQM9cyw3QzbcnZvt\nPhCUoeSRTWMctNDu5purTZDz\n-----END PRIVATE KEY-----\n";

// Add database URL
const char* FIREBASE_DATABASE_URL = "https://attendance-ed3eb-default-rtdb.firebaseio.com/";

FirebaseData fbdo;
FirebaseConfig config;
FirebaseAuth auth;
MFRC522 rfid(SS_PIN, RST_PIN);


void setup() {
  Serial.begin(115200);
  Serial.println("Starting setup...");
  
  SPI.begin();
  rfid.PCD_Init();
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Initializing...");
  Serial.println("Initialized SIM800L, RFID, and LCD");

  // Initialize WiFi with detailed logging
  Serial.println("Attempting WiFi connection...");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to WiFi: ");
  Serial.println(WIFI_SSID);
  
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Connecting WiFi..");
  
  int wifiAttempts = 0;
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
    wifiAttempts++;
    if (wifiAttempts > 20) { // 10 second timeout
      Serial.println("\nWiFi connection failed!");
      lcd.clear();
      lcd.print("WiFi Failed!");
      delay(2000);
      ESP.restart();
    }
  }
  Serial.println("\nWiFi connected successfully!");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  // Configure Firebase with detailed logging
  Serial.println("Configuring Firebase...");
  config.service_account.data.client_email = FIREBASE_CLIENT_EMAIL;
  config.service_account.data.project_id = FIREBASE_PROJECT_ID;
  config.service_account.data.private_key = FIREBASE_PRIVATE_KEY;
  config.database_url = FIREBASE_DATABASE_URL;

  Serial.println("Attempting Firebase connection...");
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);

  if (Firebase.ready()) {
    Serial.println("Firebase connected successfully!");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("System Ready");
  } else {
    Serial.println("Firebase connection failed!");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Firebase Failed!");
  }
}

void loop() {
  if (!Firebase.ready()) {
    Serial.println("Firebase not ready");
    Serial.print("WiFi status: ");
    Serial.println(WiFi.status() == WL_CONNECTED ? "Connected" : "Disconnected");
    Serial.print("Last Firebase Error: ");
    Serial.println(fbdo.errorReason());
    
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Firebase Error");
    lcd.setCursor(0, 1);
    lcd.print(fbdo.errorReason().substring(0, 16)); // Show first 16 chars of error
    delay(2000);
    return;
  }

  if (rfid.PICC_IsNewCardPresent() && rfid.PICC_ReadCardSerial()) {
    String tagID = "";
    for (byte i = 0; i < rfid.uid.size; i++) {
      tagID += String(rfid.uid.uidByte[i], HEX);
    }
    rfid.PICC_HaltA();

    Serial.println("\nNew RFID tag detected!");
    Serial.println("RFID Tag ID: " + tagID);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("RFID: " + tagID);

    // Get current timestamp
    unsigned long currentTime = millis();
    String timeStampStr = String(currentTime);

    // Update path structure to: attendance/RfidTag/TimeStamp/{data}
    String path = "/attendance/" + tagID + "/" + timeStampStr;
    FirebaseJson json;
    json.set("RFID", tagID);
    json.set("Timestamp", currentTime);
    json.set("Date", "2024-03-21"); // You might want to add real date functionality
    json.set("Status", "Present");

    Serial.println("Attempting to send data to Firebase...");
    Serial.println("Path: " + path);
  Serial.println(String("Data: ") + json.raw());

    if (Firebase.setJSON(fbdo, path, json)) {
      Serial.println("Data sent successfully!");
      lcd.setCursor(0, 1);
      lcd.print("Sent to Firebase");
    } else {
      Serial.println("Firebase push failed!");
      Serial.print("Error reason: ");
      Serial.println(fbdo.errorReason());
      Serial.print("HTTP response code: ");
      Serial.println(fbdo.httpCode());
      lcd.setCursor(0, 1);
      lcd.print("Send Failed!");
    }
  }
  delay(1000);  
}