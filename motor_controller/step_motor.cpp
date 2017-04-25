#include "Arduino.h"
#include "step_motor.h"

StepMotor::StepMotor(int motor_pin_1, int motor_pin_2,
                     int motor_pin_3, int motor_pin_4)
{
  this->motor_pin_1 = motor_pin_1;
  this->motor_pin_2 = motor_pin_2;
  this->motor_pin_3 = motor_pin_3;
  this->motor_pin_4 = motor_pin_4;

  pinMode(this->motor_pin_1, OUTPUT);
  pinMode(this->motor_pin_2, OUTPUT);
  pinMode(this->motor_pin_3, OUTPUT);
  pinMode(this->motor_pin_4, OUTPUT);

  this->reset_pins();
}

void StepMotor::make_steps(int number_of_steps, unsigned long delay_mcs, 
                           unsigned long delay_between_steps_mcs)
{
  int steps_left = abs(number_of_steps);
  int direction  = number_of_steps > 0 ? 1 : -1;
  int curr_step  = number_of_steps > 0 ? 0 : steps_left;

  while (steps_left > 0) {
      curr_step  += direction;
      steps_left -= 1;

      this->next_step(curr_step % 4);
      
      if (delay_between_steps_mcs > 0) {
        this->loop_delay_mcs(delay_between_steps_mcs);
        this->reset_pins();
      }
      
      this->loop_delay_mcs(delay_mcs - delay_between_steps_mcs);
  }
  this->reset_pins();
}

void StepMotor::next_step(int thisStep)
{
  switch (thisStep) {
    case 0:  // 1010
      digitalWrite(this->motor_pin_1, HIGH);
      digitalWrite(this->motor_pin_2, LOW);
      digitalWrite(this->motor_pin_3, HIGH);
      digitalWrite(this->motor_pin_4, LOW);
    break;
    case 1:  // 0110
      digitalWrite(this->motor_pin_1, LOW);
      digitalWrite(this->motor_pin_2, HIGH);
      digitalWrite(this->motor_pin_3, HIGH);
      digitalWrite(this->motor_pin_4, LOW);
    break;
    case 2:  //0101
      digitalWrite(this->motor_pin_1, LOW);
      digitalWrite(this->motor_pin_2, HIGH);
      digitalWrite(this->motor_pin_3, LOW);
      digitalWrite(this->motor_pin_4, HIGH);
    break;
    case 3:  //1001
      digitalWrite(this->motor_pin_1, HIGH);
      digitalWrite(this->motor_pin_2, LOW);
      digitalWrite(this->motor_pin_3, LOW);
      digitalWrite(this->motor_pin_4, HIGH);
    break;
  }
}

void StepMotor::reset_pins()
{
  digitalWrite(this->motor_pin_1, LOW);
  digitalWrite(this->motor_pin_2, LOW);
  digitalWrite(this->motor_pin_3, LOW);
  digitalWrite(this->motor_pin_4, LOW);
}

void StepMotor::loop_delay_mcs(unsigned long mcs)
{
  unsigned long then = micros();
  unsigned long delta = 0;
  do {
    delta = micros() - then;
  } while (delta < mcs);
}

