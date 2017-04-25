#include "step_motor.h"

#define SERIAL_RATE 9600
#define HANDSHAKE_STRING "Arduino step motor controller"

#define MOTOR1_NAME "A"
#define MOTOR1_PINS 50, 51, 52, 53
#define MOTOR2_NAME "B"
#define MOTOR2_PINS 46, 47, 48, 49
#define MOTOR3_NAME "C"
#define MOTOR3_PINS 42, 43, 44, 45

#define COMMAND_SPIN "SPIN"

#define COMMAND_SEPARATOR " "
#define END_OF_COMMAND '\n'

#define RESPONSE_PREFIX_OK "OK"
#define RESPONSE_PREFIX_FAIL "FAIL"

String data_buf;
bool data_buf_available;

StepMotor motor1(MOTOR1_PINS);
StepMotor motor2(MOTOR2_PINS);
StepMotor motor3(MOTOR3_PINS);

void reset_data_buffer() {
  data_buf = "";
  data_buf_available = false;
}

char* tokenize(char* text_buff) {
  return strtok(data_buf.c_str(), COMMAND_SEPARATOR);
}

char* tokenize_next() {
  return strtok(NULL, COMMAND_SEPARATOR);
}

bool command_spin() {
  String motor = tokenize_next();
  unsigned long steps = atol(tokenize_next());
  unsigned long step_interval = atol(tokenize_next());
  unsigned long step_delay = atol(tokenize_next());

  if ((steps != 0) && (step_interval > 0) && (step_delay >= 0)) {
    if (motor.equals(MOTOR1_NAME))
      motor1.make_steps(steps, step_interval, step_delay);
    else if (motor.equals(MOTOR2_NAME))
      motor2.make_steps(steps, step_interval, step_delay);
    else if (motor.equals(MOTOR3_NAME))
      motor3.make_steps(steps, step_interval, step_delay);
    else
      return false;

    return true;
  } else
    return false;
}

void setup() {
  reset_data_buffer();
  data_buf.reserve(256);

  Serial.begin(SERIAL_RATE);
  while (!Serial);
  Serial.println(HANDSHAKE_STRING);
}

void loop() {
  if (data_buf_available) {
    unsigned long time_stamp = micros();

    bool command_result = false;

    String command = tokenize(data_buf.c_str());
    if (command.equals(COMMAND_SPIN))
      command_result = command_spin();

    time_stamp = micros() - time_stamp;

    Serial.println(
      String(command_result ? RESPONSE_PREFIX_OK : RESPONSE_PREFIX_FAIL) + COMMAND_SEPARATOR +
      command + COMMAND_SEPARATOR +
      String(time_stamp, DEC)
    );

    reset_data_buffer();
  }
}

void serialEvent() {
  while (Serial.available()) {
    char inChar = Serial.read();

    if (inChar == END_OF_COMMAND)
      data_buf_available = true;
    else
      data_buf += inChar;
  }
}
