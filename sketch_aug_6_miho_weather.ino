#include <LiquidCrystal.h>

LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

#define CH_CLOUD_LEFT 0
byte cloudyLeft[8] = {
  B00011,
  B00100,
  B00100,
  B01000,
  B01000,
  B00111,
  B00000,
  B00000,
};

#define CH_CLOUD_RIGHT 1
byte cloudyRight[8] = {
  B10000,
  B01000,
  B00110,
  B01001,
  B00001,
  B11110,
  B00000,
  B00000,
};

#define CH_RAIN_LEFT 2
byte rainyLeft[8] = {
  B00011,
  B00100,
  B00100,
  B01000,
  B01000,
  B00111,
  B00010,
  B00001,
};

#define CH_RAIN_RIGHT 3
byte rainyRight[8] = {
  B10000,
  B01000,
  B00110,
  B01001,
  B00001,
  B11110,
  B10100,
  B01010,
};

#define CH_SUN_LEFT 4
byte sunnyLeft[8] = {
  B00010,
  B00001,
  B01010,
  B00100,
  B00100,
  B01010,
  B00001,
  B00010,
};

#define CH_SUN_RIGHT 5
byte sunnyRight[8] = {
  B01000,
  B10000,
  B01010,
  B00100,
  B00100,
  B01010,
  B10000,
  B01000,
};

void setup() {
  Serial.begin(9600);

  lcd.createChar(CH_CLOUD_LEFT, cloudyLeft);
  lcd.createChar(CH_CLOUD_RIGHT, cloudyRight);

  lcd.createChar(CH_RAIN_LEFT, rainyLeft);
  lcd.createChar(CH_RAIN_RIGHT, rainyRight);

  lcd.createChar(CH_SUN_LEFT, sunnyLeft);
  lcd.createChar(CH_SUN_RIGHT, sunnyRight);

  lcd.begin(16, 2);
}

int i = 0;

int incomingType;
int incomingValue;
int key_in;
bool isPressed;
int buttonCooldown;

void loop() {
  key_in = analogRead(0);

  if (key_in >= 1000) {
    isPressed = false;
  }

  if (isPressed) {
    if (buttonCooldown == 0) {
      isPressed = false;
    } else {
      buttonCooldown--;
    }
  }

  if (key_in >= 82 && key_in <= 182 && !isPressed) {
    isPressed = true;
    buttonCooldown = 4000;
    Serial.println("up");
  }

  if (key_in >= 257 && key_in <= 357 && !isPressed) {
    isPressed = true;
    buttonCooldown = 4000;
    Serial.println("down");
  }


  if (Serial.available() > 0) {

    incomingType = Serial.read();

    switch (incomingType) {
      case 'p':
        incomingValue = Serial.parseInt();
        lcd.setCursor(0, 0);

        switch (incomingValue) {
          case 0:
            lcd.print("Shinjuku");
            break;

          case 1:
            lcd.print("Osaka   ");
            break;

          case 2:
            lcd.print("Sapporo ");
            break;

          case 3:
            lcd.print("Okinawa ");
            break;
        }
        break;

      case 't':
        incomingValue = Serial.parseInt();

        // say what you got:
        Serial.print("I received temp: ");
        Serial.println(incomingValue, DEC);

        if (incomingValue < 0) {
          lcd.setCursor(11, 0);
        } else {
          lcd.setCursor(12, 0);
        }

        lcd.print(incomingValue, DEC);
        lcd.write(B11011111);
        lcd.write("C   ");
        break;

      case 'h':
        incomingValue = Serial.parseInt();

        // say what you got:
        Serial.print("I received humid: ");
        Serial.println(incomingValue, DEC);

        lcd.setCursor(12, 1);
        lcd.print(incomingValue, DEC);
        lcd.print("%   ");
        break;

      case 's':
        incomingValue = Serial.parseInt();

        // say what you got:
        Serial.print("I received symbol: ");
        Serial.println(incomingValue, DEC);

        switch (incomingValue) {
          case 0:
            lcd.setCursor(3, 1);
            lcd.write(byte(CH_CLOUD_LEFT));
            lcd.write(byte(CH_CLOUD_RIGHT));
            break;
          
          case 1:
            lcd.setCursor(3, 1);
            lcd.write(byte(CH_RAIN_LEFT));
            lcd.write(byte(CH_RAIN_RIGHT));
            break;

          case 2: 
            lcd.setCursor(3, 1);
            lcd.write(byte(CH_SUN_LEFT));
            lcd.write(byte(CH_SUN_RIGHT));
            break;
        }
        break; 
    }
  }
}