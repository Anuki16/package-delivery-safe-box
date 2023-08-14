
#include <WiFi.h>
#include <FirebaseESP32.h>

// Provide the token generation process info.
#include <addons/TokenHelper.h>

// Provide the RTDB payload printing info and other helper functions.
#include <addons/RTDBHelper.h>

#define WIFI_LED_PIN 19

FirebaseData fbdo;

FirebaseAuth auth;
FirebaseConfig config;

const char* WIFI_SSID = "";
const char* WIFI_PASSWORD = "";
const char* API_KEY = "";
const char* DATABASE_URL = "";

// NTP server to request epoch time
const char* ntpServer = "pool.ntp.org";

bool signupOK = false;

int blink_status = HIGH;

void setup_wifi() {
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    blink_status = !blink_status;
    digitalWrite(WIFI_LED_PIN, blink_status);
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  configTime(0, 0, ntpServer);
}

void setup_firebase() {
  Serial.printf("Firebase Client v%s\n\n", FIREBASE_CLIENT_VERSION);

  /* Assign the api key (required) */
  config.api_key = API_KEY;

  /* Assign the user sign in credentials 
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;*/

  /* Assign the RTDB URL (required) */
  config.database_url = DATABASE_URL;

    /* Sign up */
  if (Firebase.signUp(&config, &auth, "", "")){
    Serial.println("Signup successful");
    signupOK = true;
  }
  else{
    Serial.printf("%s\n", config.signer.signupError.message.c_str());
  }

  /* Assign the callback function for the long running token generation task */
  config.token_status_callback = tokenStatusCallback; // see addons/TokenHelper.h

  Firebase.begin(&config, &auth);

  // Comment or pass false value when WiFi reconnection will control by your code or third party library
  Firebase.reconnectWiFi(true);

  Firebase.setDoubleDigits(5);
}

unsigned long get_time() {
  time_t now;
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    //Serial.println("Failed to obtain time");
    return(0);
  }
  time(&now);
  return now;
}

void update_unlock_info(String type, unsigned long timestamp) {
  Firebase.setStringAsync(fbdo, "DeliverySafe/user1/history/" + String(timestamp), type);
  if (type == "otp") {
    Firebase.setStringAsync(fbdo, "DeliverySafe/user1/pwhash/", "xxx");
  }
}
