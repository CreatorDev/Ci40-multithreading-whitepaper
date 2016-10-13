#include "letmecreate/core/led.h"

int main(void)
{
  int x=0;
  led_init();
  //toggle leds on-off 10 times
  for (x=0; x<10; x++)
  {
    led_switch_on(ALL_LEDS);
    usleep(100000);
    led_switch_off(ALL_LEDS);
    usleep(100000);
  }
}
