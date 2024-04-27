
#include <cloudrunner.h>
CloudRat Cloudrat;
void setup() {
  //Initialize board and motors
  Cloudrat.begin();

  //Beep motors twice
  Cloudrat.motor_quartersec_beep(2);
  //Manually Calibrate for 10s
  for(int i = 0; i<10; i++){
    Cloudrat.calibrate_sensors();
    Cloudrat.calibrate_turn_sensors();
    delay(1000);
  }

  //Beep motors thrice
  Cloudrat.motor_quartersec_beep(3);

  //Wait 5s and beep every second
  for(int i=0; i<5; i++){
    Cloudrat.beep_motor();
    delay(1000);
  }
}

void loop() {
  Cloudrat.follow_line();
}
