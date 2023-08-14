#include <Keypad.h>

#define K1 4
#define K2 23
#define K3 32
#define K4 33
#define K5 25
#define K6 26
#define K7 27
#define BUZZER

const int ROW_NUM = 4;
const int COLUMN_NUM = 3;

char keys[ROW_NUM][COLUMN_NUM] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};

byte pin_row[ROW_NUM] = {K1, K2, K3, K4};//{K2, K7, K6, K4}; //connect to the row pinouts of the keypad
byte pin_column[COLUMN_NUM] = {K5, K6, K7};//{K3, K1, K5}; //connect to the column pinouts of the keypad

Keypad keypad = Keypad(makeKeymap(keys), pin_row, pin_column, ROW_NUM, COLUMN_NUM);


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
  char key = keypad.getKey();
  if (key) {
    Serial.print(key);
  }
}
