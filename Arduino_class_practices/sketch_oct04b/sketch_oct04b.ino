#include <Stepper.h>

#define Motor_steps_per_Revolution 32
#define GSR 32*64
//GSR = Geared shaft per one revolution

Stepper stepper(Motor_steps_per_Revolution, 8,10,9,11);
void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:
  stepper.setSpeed(350);
  stepper.step(GSR/2);
  delay(2000);
  
  stepper.setSpeed(350);
  stepper.step(-GSR/2);
  delay(2000);
  
  stepper.setSpeed(700);
  stepper.step(GSR);
  delay(2000);
  
  stepper.setSpeed(700);
  stepper.step(-GSR);
  delay(2000);
}
