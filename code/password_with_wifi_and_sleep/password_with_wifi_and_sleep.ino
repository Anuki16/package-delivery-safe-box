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
#define RED_LED_PIN 16
#define GREEN_LED_PIN 17
#define BUZZER_PIN 21
#define LOCK_PIN 15
#define WIFI_RED_LED 18//LED_BUILTIN  //18
#define WIFI_GREEN_LED 19

#define KEY_PRESS 440
#define CORRECT 880
#define WRONG 220

const unsigned long idle_time = 60 * 1000;
const unsigned long sleep_time = 30;

RTC_DATA_ATTR unsigned long checkDataPrevMillis = 0;
RTC_DATA_ATTR unsigned long last_action_time = 0;
RTC_DATA_ATTR unsigned long offset_time = 0;
RTC_DATA_ATTR unsigned long last_unlock_time = 0;

RTC_DATA_ATTR String last_unlock_type = "";
RTC_DATA_ATTR bool is_last_unlock_master = false;

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

RTC_DATA_ATTR String entered = "";
RTC_DATA_ATTR String master_password; 
RTC_DATA_ATTR String otp_password = "xxx";

RTC_DATA_ATTR int mode = NORMAL;

unsigned long wifi_blink_millis = 0;
int wifi_blink_status = HIGH;
unsigned long lock_blink_millis = 0;
int lock_blink_status = HIGH;

void light_LED(int state) {
  //digitalWrite(RED_LED_PIN, !state);
  digitalWrite(GREEN_LED_PIN, state);
}

void light_WIFI_LED(int state) {
  digitalWrite(WIFI_RED_LED, !state);
  digitalWrite(WIFI_GREEN_LED, state);
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
    Serial.println("Password not updated");
  }
}

void enter_action() {
  if (entered == "0") { // wifi config mode
    if (WiFi.status() != WL_CONNECTED) {
      light_WIFI_LED(LOW);
      Serial.println("Entering WiFi config mode");
      Serial.println("Go to 192.168.4.1");
      autoconnect_wifi();
    }

  } else if (mode == NORMAL) {
    if (entered != master_password && WiFi.status() == WL_CONNECTED) update_otp();
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
  pinMode(WIFI_RED_LED, OUTPUT);
  pinMode(WIFI_GREEN_LED, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LOCK_PIN, OUTPUT);
  esp_sleep_enable_ext0_wakeup(GPIO_NUM_13,1);
  esp_sleep_enable_timer_wakeup(sleep_time * 1000000);

  Serial.begin(115200);
  lock_state_outputs(LOW);
  light_WIFI_LED(LOW);

  if (esp_sleep_get_wakeup_cause() == ESP_SLEEP_WAKEUP_UNDEFINED) {
    offset_time = millis();
    last_action_time = millis();
  } else if (esp_sleep_get_wakeup_cause() == ESP_SLEEP_WAKEUP_EXT0) {
    last_action_time = offset_time + millis();
  }

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

  if (key || digitalRead(RESET_PIN)) {
    last_action_time = offset_time + millis();
  }

  if (digitalRead(RESET_PIN) && is_last_unlock_master) {
    Serial.println("In password changing mode");
    mode = CHANGE;
    light_LED(HIGH);
    tone(BUZZER_PIN, CORRECT);
    delay(200);
    noTone(BUZZER_PIN);
  }

  if ((millis() - checkDataPrevMillis > 15000 || checkDataPrevMillis == 0) && WiFi.status() == WL_CONNECTED) {
    checkDataPrevMillis = millis();
    Serial.println("Checking for updates...");
    update_otp();
  }

  if (last_unlock_type != "" && WiFi.status() == WL_CONNECTED) {
    update_unlock_info(last_unlock_type, last_unlock_time);
    last_unlock_type = "";
  }

  if (WiFi.status() == WL_CONNECTED) {
    light_WIFI_LED(HIGH);
  } else if (millis() - wifi_blink_millis > 200) {
    digitalWrite(WIFI_GREEN_LED, LOW);
    wifi_blink_status = !wifi_blink_status;
    digitalWrite(WIFI_RED_LED, wifi_blink_status);
    wifi_blink_millis = millis();
  }

  if (mode == CHANGE && (millis() - lock_blink_millis > 200)) {
    lock_blink_status = !lock_blink_status;
    digitalWrite(GREEN_LED_PIN, lock_blink_status);
    lock_blink_millis = millis();
  }

  if (offset_time + millis() - last_action_time > idle_time) {
    Serial.println(offset_time + millis() - last_action_time);
    offset_time = offset_time + millis();
    Serial.println("Going to sleep now");
    esp_deep_sleep_start();
  }
}
