#include <WiFi.h>
#include <BluetoothSerial.h>
#include <ESP32Servo.h>

BluetoothSerial SerialBT;

const char* ssid = "YOUR_WIFI";
const char* password = "YOUR_PASS";

Servo L1H, L1K, L1A;
Servo L2H, L2K, L2A;
Servo L3H, L3K, L3A;
Servo L4H, L4K, L4A;

#define trigPin 32
#define echoPin 33

long duration;
int distance;

void setup() {
  Serial.begin(115200);

  SerialBT.begin("BostonDog");

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }

  L1H.attach(13); L1K.attach(12); L1A.attach(14);
  L2H.attach(27); L2K.attach(26); L2A.attach(25);
  L3H.attach(33); L3K.attach(32); L3A.attach(15);
  L4H.attach(4);  L4K.attach(16); L4A.attach(17);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  stand();
}

void loop() {
  bluetoothControl();
  autoMode();
}

void bluetoothControl() {
  if (SerialBT.available()) {
    char cmd = SerialBT.read();

    if(cmd == 'F') moveForward();
    if(cmd == 'B') moveBackward();
    if(cmd == 'L') moveLeft();
    if(cmd == 'R') moveRight();
    if(cmd == 'D') dance();
    if(cmd == 'H') friendly();
  }
}

void autoMode() {
  distance = getDistance();
  if(distance < 20) {
    avoidObstacle();
  }
}

void stand() {
  setAll(90);
}

void setAll(int angle) {
  L1H.write(angle); L1K.write(angle); L1A.write(angle);
  L2H.write(angle); L2K.write(angle); L2A.write(angle);
  L3H.write(angle); L3K.write(angle); L3A.write(angle);
  L4H.write(angle); L4K.write(angle); L4A.write(angle);
}

void moveForward() {
  L1H.write(70); L3H.write(70);
  delay(200);
  L2H.write(110); L4H.write(110);
  delay(200);
  stand();
}

void moveBackward() {
  L1H.write(110); L3H.write(110);
  delay(200);
  L2H.write(70); L4H.write(70);
  delay(200);
  stand();
}

void moveLeft() {
  L1H.write(70);
  L4H.write(110);
  delay(300);
  stand();
}

void moveRight() {
  L2H.write(70);
  L3H.write(110);
  delay(300);
  stand();
}

void dance() {
  for(int i=0;i<3;i++){
    setAll(60);
    delay(200);
    setAll(120);
    delay(200);
  }
  stand();
}

void friendly() {
  for(int i=0;i<2;i++){
    L1A.write(60);
    L4A.write(120);
    delay(200);
    L1A.write(120);
    L4A.write(60);
    delay(200);
  }
}

void avoidObstacle() {
  moveBackward();
  delay(300);
  moveLeft();
}

int getDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  return duration * 0.034 / 2;
}
