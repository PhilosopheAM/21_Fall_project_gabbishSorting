#ifndef LED_HPP
#define LED_HPP

#include "bsp_led.h"

class Led
{
public:
    enum LedList
    {
        Red = 0,
        Green,
        DebugH7,
        DebugG6,
        DebugF5,
        DebugE4,
        DebugD3,
        DebugC2,
        DebugB1,
        DebugA0
    };

    static void Init(){ bsp_led_init(); }

    static void On(LedList _led){ bsp_led_on((bsp_led_list_t)_led); }

    static void Off(LedList _led){ bsp_led_off((bsp_led_list_t)_led); }

    static void Toggle(LedList _led){ bsp_led_toggle((bsp_led_list_t)_led);
    }
};

#endif
