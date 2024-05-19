#include <string.h>
#define IBUS_BUFFSIZE 32
#define IBUS_MAXCHANNELS 6
#include <Servo.h>
static uint8_t ibusIndex = 0;
static uint8_t ibus[IBUS_BUFFSIZE] = { 0 };
static uint16_t rcValue[IBUS_MAXCHANNELS];

static boolean rxFrameDone;

int ch_width_1;
int ch_width_2;
int ch_width_3;
int st_mtr1;
int st_mtr2;



void setup() {
  

  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(9, OUTPUT);

  pinMode(6, OUTPUT);
  pinMode(8, OUTPUT);
  Serial.begin(115200);
}
void loop() {

  readRx();
  if (ch_width_2 > 10) {
    digitalWrite(4, HIGH);
    digitalWrite(5, LOW);
    analogWrite(3, ch_width_2);
  } else if (ch_width_2 < (-10)) {
    digitalWrite(4, LOW);
    digitalWrite(5, HIGH);
    analogWrite(3, -(ch_width_2));
  } else {
    digitalWrite(4, LOW);
    digitalWrite(5, LOW);
  }
  if (ch_width_1 > 10) {
    digitalWrite(8, HIGH);
    digitalWrite(6, LOW);
    analogWrite(9, ch_width_1);
  } else if (ch_width_1 < (-10)) {
    digitalWrite(8, LOW);
    digitalWrite(6, HIGH);
    analogWrite(9, -(ch_width_1));
  } else {
    digitalWrite(8, LOW);
    digitalWrite(6, LOW);
  }
  //analogWrite(A5, ch_width_3);
  // Serial.print(ch_width_1);
  // Serial.print('  ');
  // Serial.print(ch_width_2);
  // Serial.println();
}

void readRx() {
  rxFrameDone = false;

  if (Serial.available()) {
    uint8_t val = Serial.read();
    // Look for 0x2040 as start of packet
    if (ibusIndex == 0 && val != 0x20) {
      ibusIndex = 0;
      return;
    }
    if (ibusIndex == 1 && val != 0x40) {
      ibusIndex = 0;
      return;
    }

    if (ibusIndex == IBUS_BUFFSIZE) {
      ibusIndex = 0;
      int high = 3;
      int low = 2;
      for (int i = 0; i < IBUS_MAXCHANNELS; i++) {
        rcValue[i] = (ibus[high] << 8) + ibus[low];
        high += 2;
        low += 2;
      }
      ch_width_1 = map(rcValue[0], 1000, 2000, -255, 255);
      

      ch_width_2 = map(rcValue[1], 1000, 2000, -255, 255);

      ch_width_3 = map(rcValue[2], 1000, 2000, 0, 255);

      Serial.print(ch_width_1);
      Serial.print(ch_width_2);
      Serial.println(ch_width_3);

      rxFrameDone = true;
      return;
    } else {
      ibus[ibusIndex] = val;
      ibusIndex++;
    }
  }
}