#include <Keypad.h>
#include <Preferences.h>  // for permanent storage
#include <WiFi.h>
#include <FirebaseESP32.h>
#include "wifi.h"

#define ROW_NUM 4
#define COLUMN_NUM 3
#define RESET '*'
#define ENTER '#'

#define K1 4
#define K2 23
#define K3 32
#define K4 33
#define K5 25
#define K6 26
#define K7 27

#define NORMAL 0
#define CHANGE 1

#define RESET_PIN 22
#define RED_LED_PIN 18
#define GREEN_LED_PIN 14
#define BUZZER_PIN 21
#define LOCK_PIN 15

#define KEY_PRESS 440
#define CORRECT 880
#define WRONG 220

unsigned long checkDataPrevMillis = 0;

unsigned long last_unlock_time = 0;
String last_unlock_type = "";
bool is_last_unlock_master = false;

char keys[ROW_NUM][COLUMN_NUM] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};

byte pin_row[ROW_NUM] = {K2, K7, K6, K4}; //connect to the row pinouts of the keypad{K1, K2, K3, K4};//
byte pin_column[COLUMN_NUM] = {K3, K1, K5}; //connect to the column pinouts of the keypad{K5, K6, K7};//

Keypad keypad = Keypad(makeKeymap(keys), pin_row, pin_column, ROW_NUM, COLUMN_NUM);
Preferences preferences;

String entered = "";
String master_password; 
String otp_password = "xxx";

int mode = NORMAL;

void light_LED(int state) {
  digitalWrite(RED_LED_PIN, !state);
  digitalWrite(GREEN_LED_PIN, state);
}

void lock_state_outputs(int state) {
  digitalWrite(LOCK_PIN, state);  // to confirm that it is not floating
  light_LED(state);
}

void update_otp() {
  if (Firebase.getString(fbdo, "/DeliverySafe/user1/pwhash")) {
    otp_password = fbdo.to<String>();
    otp_password = otp_password.substring(2, otp_password.length()-2);
    Serial.println("Set OTP password: " + otp_password);
  } else {
    otp_password = "xxx";
    Serial.println("Password not set");
  }
}

void enter_action() {
  if (entered == "0") { // wifi config mode
    if (WiFi.status() != WL_CONNECTED) {
      Serial.println("Entering WiFi config mode");
      Serial.println("Go to 192.168.4.1");
      autoconnect_wifi();
    }

  } else if (mode == NORMAL) {
    if (Firebase.ready()) update_otp();
    if (entered == master_password || entered == otp_password) {
      Serial.println("Correct password");
      lock_state_outputs(HIGH);

      tone(BUZZER_PIN, CORRECT);
      delay(400);
      noTone(BUZZER_PIN);

      delay(2000);
      lock_state_outputs(LOW);

      last_unlock_time = get_time();
      
      if (entered == otp_password) {
        last_unlock_type = "otp";
        otp_password = "xxx"; 
        is_last_unlock_master = false;
      } else {
        last_unlock_type = "master";
        is_last_unlock_master = true;
      }

    } else {
      Serial.println("Wrong password");
      tone(BUZZER_PIN, WRONG);
      delay(400);
      noTone(BUZZER_PIN);
    }
  } else if (mode == CHANGE) {
    master_password = entered;
    preferences.begin("password", false); 
    preferences.putString("pass",master_password);
    preferences.end();

    Serial.println("Changed master password to: " + master_password);
    mode = NORMAL;
    light_LED(LOW);
  }
}

void key_action(char key) {
  if (key == RESET) {
    entered = "";
  } else if (key == ENTER) {
    enter_action();
    entered = "";
  } else {
    entered = entered + String(key);
  }
  Serial.println(entered);
}

void setup() {
  // put your setup code here, to run once:
  pinMode(RESET_PIN, INPUT_PULLDOWN);
  pinMode(RED_LED_PIN, OUTPUT);
  pinMode(GREEN_LED_PIN, OUTPUT);
  pinMode(WIFI_LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LOCK_PIN, OUTPUT);

  Serial.begin(115200);
  lock_state_outputs(LOW);

  preferences.begin("password", true); 
  master_password = preferences.getString("pass", "!!");
  preferences.end();

  if (master_password == "!!") {  // No password set
    master_password = "1234";
    preferences.begin("password", false); 
    preferences.putString("pass",master_password);
    preferences.end();
  }

  Serial.println("Password is: "+master_password);

  setup_wifi();

}

void loop() {
  // put your main code here, to run repeatedly:
  char key = keypad.getKey();
  if (key) {
    tone(BUZZER_PIN, KEY_PRESS);
    delay(200);
    noTone(BUZZER_PIN);
    key_action(key);
  }

  if (digitalRead(RESET_PIN) && is_last_unlock_master) {
    Serial.println("In password changing mode");
    mode = CHANGE;
    light_LED(HIGH);
    tone(BUZZER_PIN, CORRECT);
    delay(200);
    noTone(BUZZER_PIN);
  }

  if ((millis() - checkDataPrevMillis > 15000 || checkDataPrevMillis == 0) && Firebase.ready() && signupOK) {
    checkDataPrevMillis = millis();
    update_otp();
  }

  if (last_unlock_type != "" && Firebase.ready()) {
    update_unlock_info(last_unlock_type, last_unlock_time);
    last_unlock_type = "";
  }

  if (WiFi.status() == WL_CONNECTED) {
    digitalWrite(WIFI_LED_PIN, HIGH);
  } else if (millis() - blinkMillis > 200) {
    blink_status = !blink_status;
    digitalWrite(WIFI_LED_PIN, blink_status);
    blinkMillis = millis();
  }
}
