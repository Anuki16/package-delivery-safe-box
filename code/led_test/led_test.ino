#define RESET_PIN 22
#define RED_LED_PIN 16
#define GREEN_LED_PIN 17
#define BUZZER_PIN 21
#define LOCK_PIN 15
#define WIFI_RED_LED 18//LED_BUILTIN  //18
#define WIFI_GREEN_LED 19

void light_LED(int state) {
  digitalWrite(RED_LED_PIN, !state);
  digitalWrite(GREEN_LED_PIN, state);
}

void light_WIFI_LED(int state) {
  digitalWrite(WIFI_RED_LED, !state);
  digitalWrite(WIFI_GREEN_LED, state);
}

void setup() {
  // put your setup code here, to run once:
    pinMode(RED_LED_PIN, OUTPUT);
  pinMode(GREEN_LED_PIN, OUTPUT);
  pinMode(WIFI_RED_LED, OUTPUT);
  pinMode(WIFI_GREEN_LED, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  light_LED(HIGH);
  delay(1000);
  //light_LED(LOW);
  //delay(1000);
  light_WIFI_LED(LOW);
  delay(1000);
  //light_WIFI_LED(LOW);
  //delay(1000);
  delay(1000);
}
