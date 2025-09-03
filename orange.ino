#include <SoftwareSerial.h>

// 모터 핀 (L298N 등 가정)
const int ENA1 = 6;   // 모터A 속도(PWM)
const int IN1  = 7;   // 모터A 방향
const int IN2  = 8;   // 모터A 방향
const int ENA2 = 9;   // 모터B 속도(PWM)
const int IN3  = 10;  // 모터B 방향
const int IN4  = 11;  // 모터B 방향

SoftwareSerial BTSerial(4, 5);  // RX=4, TX=5 (BLE 모듈과 연결)

const uint8_t SPEED = 200; // 0~255 원하는 기본 속도

void setup() {
  Serial.begin(9600);
  BTSerial.begin(9600);

  pinMode(ENA1, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENA2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  stopMotors();
}

void loop() {
  // PC → BLE 브리지 (시리얼 모니터에서 입력한 걸 BLE로 전송)
  if (Serial.available()) {
    char c = Serial.read();
    BTSerial.write(c);
  }

  // BLE → PC 브리지 + 명령 파싱 (BLE에서 온 걸 PC에도 보여주고, 명령 처리)
  if (BTSerial.available()) {
    char command = BTSerial.read();
    Serial.write(command);  // 모니터에 에코

    handleCommand(command);
  }
}

// ======== 동작 명령 처리 ========
void handleCommand(char command) {
  switch (command) {
    case 'D':  // Forward
      forward();
      delay(800);
      stopMotors();
      break;

    case 'B':  // Backward
      backward();
      delay(800);
      stopMotors();
      break;

    case 'L':  // Turn Left (탱크처럼 제자리 좌회전)
      turnLeft();
      delay(800);
      stopMotors();
      break;

    case 'R':  // Turn Right (탱크처럼 제자리 우회전)
      turnRight();
      delay(800);
      stopMotors();
      break;

    case 'S':  // Stop
      stopMotors();
      break;

    default:
      // 필요 시: 미지의 명령 무시
      break;
  }
}

// ======== 모터 제어 헬퍼 ========
void forward() {
  // 왼/오 바퀴 모두 정방향
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENA1, SPEED);
  analogWrite(ENA2, SPEED);
}

void backward() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  analogWrite(ENA1, SPEED);
  analogWrite(ENA2, SPEED);
}

void turnLeft() {
  // 왼쪽 바퀴 뒤로, 오른쪽 바퀴 앞으로 → 제자리 좌회전
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENA1, SPEED);
  analogWrite(ENA2, SPEED);
}

void turnRight() {
  // 왼쪽 바퀴 앞으로, 오른쪽 바퀴 뒤로 → 제자리 우회전
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  analogWrite(ENA1, SPEED);
  analogWrite(ENA2, SPEED);
}

void stopMotors() {
  // 완전 정지 (방향핀 LOW + PWM 0)
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  analogWrite(ENA1, 0);
  analogWrite(ENA2, 0);
}
