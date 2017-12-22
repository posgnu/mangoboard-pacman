#include "s3c_gpio.h"
#include "mango_button.h"


#define MANGO_GPIO_BUTTON_PORT	S3C_GPIO_PORT_N


void mango_btn_init(void)
{
    mango_gpio_set_type(MANGO_GPIO_BUTTON_PORT, 0, S3C_GPIO_TYPE_INPUT);
    mango_gpio_set_type(MANGO_GPIO_BUTTON_PORT, 1, S3C_GPIO_TYPE_INPUT);
    mango_gpio_set_type(MANGO_GPIO_BUTTON_PORT, 2, S3C_GPIO_TYPE_INPUT);
    mango_gpio_set_type(MANGO_GPIO_BUTTON_PORT, 3, S3C_GPIO_TYPE_INPUT);

    int i;
    for (i=0; i<3; i++)
	    prev_state[i] = 0;

}

MANGO_BTN_STATE mango_btn_scan(MANGO_BTN_KEYCODE code)
{
    int val;

    mango_gpio_get_val(MANGO_GPIO_BUTTON_PORT, code, &val);


    if (!val && prev_state[code]==0) 	/* pressed */
	{
		prev_state[code]=1;
	        return MANGO_BTN_STATE_DOWN;
	}
    
    if (!val) return MANGO_BTN_STATE_STABLE;

    if (val && prev_state[code]==1)
	{
		prev_state[code]=0;
		return MANGO_BTN_STATE_UP;
	}
 
    return MANGO_BTN_STATE_STABLE;

}



