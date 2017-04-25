#ifndef step_motor_h
#define step_motor_h

class StepMotor {
  public:
    StepMotor(int motor_pin_1, int motor_pin_2,
              int motor_pin_3, int motor_pin_4);

    void make_steps(int number_of_steps, unsigned long delay_msc, 
                    unsigned long delay_between_steps_mcs);
  private:
    void next_step(int this_step);
    void reset_pins();
    void loop_delay_mcs(unsigned long mcs);

    int motor_pin_1;
    int motor_pin_2;
    int motor_pin_3;
    int motor_pin_4;
};

#endif

