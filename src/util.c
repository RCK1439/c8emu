#include "util.h"

#include <raylib.h>

uint8_t util_kp(void)
{
    switch (GetKeyPressed()) {
        case KEY_ZERO:
        case KEY_KP_0:
            return 0x00;
        case KEY_ONE:
        case KEY_KP_1:
            return 0x01;
        case KEY_TWO:
        case KEY_KP_2:
            return 0x02;
        case KEY_THREE:
        case KEY_KP_3:
            return 0x03;
        case KEY_FOUR:
        case KEY_KP_4:
            return 0x04;
        case KEY_FIVE:
        case KEY_KP_5:
            return 0x05;
        case KEY_SIX:
        case KEY_KP_6:
            return 0x06;
        case KEY_SEVEN:
        case KEY_KP_7:
            return 0x07;
        case KEY_EIGHT:
        case KEY_KP_8:
            return 0x08;
        case KEY_NINE:
        case KEY_KP_9:
            return 0x09;
        case KEY_A:
            return 0x0A;
        case KEY_B:
            return 0x0B;
        case KEY_C:
            return 0x0C;
        case KEY_D:
            return 0x0D;
        case KEY_E:
            return 0x0E;
        case 0X0F:
            return 0x0F;
    }

    return 0xFF;
}