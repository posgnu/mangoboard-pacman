#ifndef __MANGO_BUTTON_H
#define __MANGO_BUTTON_H

typedef enum {
    MANGO_BTN_STATE_UP,
    MANGO_BTN_STATE_DOWN,
    MANGO_BTN_STATE_STABLE
} MANGO_BTN_STATE;

typedef enum {
    MANGO_BTN_KEYCODE_UP,
    MANGO_BTN_KEYCODE_DOWN,
    MANGO_BTN_KEYCODE_LEFT,
    MANGO_BTN_KEYCODE_RIGHT,
} MANGO_BTN_KEYCODE;

int prev_state[4];

void mango_btn_init(void);

MANGO_BTN_STATE mango_btn_scan(MANGO_BTN_KEYCODE code);


#endif	/* __MANGO_BUTTON_H */

