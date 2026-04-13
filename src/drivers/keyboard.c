#include <stddef.h>
#include "bool.h"
#include "keyboard.h"
#include "idt.h"
#include "ps2.h"

#define KEYBOARD_EXTENDED_SCAN_CODE 0xE0

void (*keyboard_handler_user)(struct KeyboardEvent event);

char to_ascii(uint16_t code) {
    switch (code) {
        case KEY_CODE_1: return '1';
        case KEY_CODE_2: return '2';
        case KEY_CODE_3: return '3';
        case KEY_CODE_4: return '4';
        case KEY_CODE_5: return '5';
        case KEY_CODE_6: return '6';
        case KEY_CODE_7: return '7';
        case KEY_CODE_8: return '8';
        case KEY_CODE_9: return '9';
        case KEY_CODE_0: return '0';

        case KEY_CODE_A: return 'A';
        case KEY_CODE_B: return 'B';
        case KEY_CODE_C: return 'C';
        case KEY_CODE_D: return 'D';
        case KEY_CODE_E: return 'E';
        case KEY_CODE_F: return 'F';
        case KEY_CODE_G: return 'G';
        case KEY_CODE_H: return 'H';
        case KEY_CODE_I: return 'I';
        case KEY_CODE_J: return 'J';
        case KEY_CODE_K: return 'K';
        case KEY_CODE_L: return 'L';
        case KEY_CODE_M: return 'M';
        case KEY_CODE_N: return 'N';
        case KEY_CODE_O: return 'O';
        case KEY_CODE_P: return 'P';
        case KEY_CODE_Q: return 'Q';
        case KEY_CODE_R: return 'R';
        case KEY_CODE_S: return 'S';
        case KEY_CODE_T: return 'T';
        case KEY_CODE_U: return 'U';
        case KEY_CODE_V: return 'V';
        case KEY_CODE_W: return 'W';
        case KEY_CODE_X: return 'X';
        case KEY_CODE_Y: return 'Y';
        case KEY_CODE_Z: return 'Z';
        case KEY_CODE_SPACE: return ' ';
        case KEY_CODE_ENTER: return '\n';
        case KEY_CODE_BACKSPACE: return '\b';
        default: return '?';
    }
}

void keyboard_handler() {
    static bool is_extended = 0;
    
    uint8_t scan_code = ps2_read_scan_code();
    
    if (scan_code == KEYBOARD_EXTENDED_SCAN_CODE) {
        is_extended = true;
        return;
    }
    
    if (keyboard_handler_user == NULL) {
        return;
    }
    
    uint16_t fat_code = scan_code & 0x7F;
    
    if (is_extended) {
        is_extended = false;
        fat_code |= KEYBOARD_EXTENDED_SCAN_CODE << 8;
    }
    
    struct KeyboardEvent event;
    if ((scan_code & 0x80) == 0) {
        event.type = KEYBOARD_EVENT_TYPE_MAKE;
    } else {
        event.type = KEYBOARD_EVENT_TYPE_BREAK;
    }
    
    event.code = fat_code;
    
    keyboard_handler_user(event);
}

void keyboard_init() {
    idt_init();
    idt_set_handler_keyboard(keyboard_handler);
}

void keyboard_set_handler(void (*handler)(struct KeyboardEvent event)) {
    keyboard_handler_user = handler;    
}