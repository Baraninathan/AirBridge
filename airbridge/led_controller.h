#ifndef LED_CONTROLLER_H
#define LED_CONTROLLER_H
extern "C"{ 
  #include "gpio.h"
  #include "user_interface.h"
}
/*
 * gpio_12 = yellow
 * gpio_14 = green
 */
class LEDController{
  private:
    uint8_t pin, cycle;
    int16_t period;
    uint32_t led_tstamp;
  public:
    LEDController(uint8_t pinAttach){
      pin = pinAttach;
    };  
    void setup();
    void init(uint16_t period, uint16_t cycle);
    void run();
    void hi();
    void lo();
    void stop();
    int16_t getCycle();
    uint32_t getTimeLeft();
};


inline void LEDController::setup(){
  pinMode(pin, OUTPUT);
}

inline void LEDController::init(uint16_t periodAttach, uint16_t cycleAttach){
  period = periodAttach;
  cycle = cycleAttach;
  digitalWrite(pin, HIGH);
  led_tstamp = system_get_rtc_time() + period*174;
}

inline void LEDController::run(){
  if (cycle > 0){
    if (system_get_rtc_time() > led_tstamp){
      if (digitalRead(pin) == HIGH){
        lo();
        led_tstamp = system_get_rtc_time() + period*174;
        cycle--;
      }
      else{
        hi();
        led_tstamp = system_get_rtc_time() + period*174;
      }
    }
  }
}

inline void LEDController::hi(){
  digitalWrite(pin, HIGH);
}

inline void LEDController::lo(){
  digitalWrite(pin, LOW);
}

inline void LEDController::stop(){
  lo();
  period = 0;
  cycle = 0;
}

inline int16_t LEDController::getCycle(){
  return cycle;
}

inline uint32_t LEDController::getTimeLeft(){
  return period*360*cycle;
}

#endif
