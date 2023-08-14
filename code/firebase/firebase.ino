#include <WiFi.h>
#include <FirebaseESP32.h>

// Provide the token generation process info.
#include <addons/TokenHelper.h>

// Provide the RTDB payload printing info and other helper functions.
#include <addons/RTDBHelper.h>

FirebaseData fbdo;

FirebaseAuth auth;
FirebaseConfig config;

const char* WIFI_SSID = "HomeNet2";
const char* WIFI_PASSWORD = "AjP160#1218";
const char* API_KEY = "AIzaSyAxP8GeimLKZwvs7xAu0psmbB3bMY2afY4";
const char* DATABASE_URL = "https://deliverysafe-92d96-default-rtdb.firebaseio.com/";

unsigned long checkDataPrevMillis = 0;

unsigned long count = 0;

bool signupOK = false;

String otp_password = "xxx";

void setup()
{

  Serial.begin(115200);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

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

void loop() {
  // put your main code here, to run repeatedly:
  if (Firebase.ready() && signupOK && (millis() - checkDataPrevMillis > 15000 || checkDataPrevMillis == 0)) {
    checkDataPrevMillis = millis();
    if (Firebase.getString(fbdo, "/DeliverySafe/user1/pwhash")) {
      otp_password = fbdo.to<String>();
      otp_password = otp_password.substring(2, otp_password.length()-2);
      Serial.println("Set OTP password: " + otp_password);
    } else {
      otp_password = "xxx";
      Serial.println("Password not set");
    }
    count ++;
  }
}
