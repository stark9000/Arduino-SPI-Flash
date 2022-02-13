#include "DF.h"
#include  <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x3f, 16, 2);

//** WARNING **
//use the circuit or the pcb  provided ,to shift the 5v to 3.3v.
//connecting the chip directly  to 5v ,will burn the chip.
//check DF.h file to see the pin connections.

boolean START_WRITE = false;
boolean START_READ = false;

int WRITE_BTN = 2;
int READ_BTN = 3;

String ADDR = "0x00";

void setup() {
  Serial.begin(9600);
  lcd.begin();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print(" SPI Flash ROM");
  while (!Serial);
  if (!DataFlash.Setup()) {
    Serial.println("DataFlash not found");
    for (;;);
  }
  pinMode(WRITE_BTN, INPUT);
  pinMode(READ_BTN, INPUT);
}


void loop() {
  if (!digitalRead(WRITE_BTN)) {
    clearLCDrow();
    delay(50);
    if (!START_WRITE) {
      START_WRITE = true;
      Serial.println("start writing !");
      Serial.println("erasing chip ...");
      DataFlash.BeginWrite();
      Serial.println("writing data ...");
      for (unsigned long i = 0; i < 25; i++) {
        if (i > 15) {
          ADDR = String("0x") + String(i, HEX);
          Serial.print(String("writing data to address ") + ADDR);
          Serial.println();
        } else {
          ADDR = String("0x0") + String(i, HEX);
          Serial.print(String("writing data to address ") + ADDR);
          Serial.println();
        }
        lcd.setCursor(0, 1);
        lcd.print(String("WR to ADD :") + ADDR);
        DataFlash.WriteByte(i);
        delay(300);
      }
      DataFlash.EndWrite();
      Serial.println("writing done !");
      START_WRITE = false;
    }
  }
  if (!digitalRead(READ_BTN)) {
    clearLCDrow();
    delay(50);
    if (!START_READ) {
      START_READ = true;
      Serial.println("start reading !");
      Serial.println("reading ...");
      DataFlash.BeginRead(0);
      for (unsigned long i = 0; i < 25; i++) {
        if (i > 15) {
          ADDR = String("0x") + String(i, HEX);
          Serial.print(String("reading from address ") + ADDR + String(":"));
          Serial.print(DataFlash.ReadByte());
          Serial.println();
        } else {
          ADDR = String("0x0") + String(i, HEX);
          Serial.print(String("reading from address ") + ADDR + String(":"));
          Serial.print(DataFlash.ReadByte());
          Serial.println();
        }
        lcd.setCursor(0, 1);
        lcd.print(String("R from ADD :") + ADDR);
        delay(300);
      }
      DataFlash.EndRead();
      Serial.println("reading done !");
      START_READ = false;
    }
  }
}
void clearLCDrow() {
  for (int i = 0; i < 17; i++) {
    lcd.setCursor(i, 1);
    lcd.print(" ");
  }
}
