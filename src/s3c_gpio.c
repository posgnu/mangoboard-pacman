#include <stdio.h>
#include "io.h"
#include "s3c_gpio.h"

static __inline unsigned long mango_gpio_get_baseaddr(S3C_GPIO_PORT port)
{
    unsigned long baseAddress = 0;
    switch(port)
    {

    case S3C_GPIO_PORT_A: baseAddress = 0x7F008000; break;
    case S3C_GPIO_PORT_B: baseAddress = 0x7F008020; break;
    case S3C_GPIO_PORT_C: baseAddress = 0x7F008040; break;
    case S3C_GPIO_PORT_D: baseAddress = 0x7F008060; break;
    case S3C_GPIO_PORT_E: baseAddress = 0x7F008080; break;
    case S3C_GPIO_PORT_F: baseAddress = 0x7F0080A0; break;
    case S3C_GPIO_PORT_G: baseAddress = 0x7F0080C0; break;
    case S3C_GPIO_PORT_H: baseAddress = 0x7F0080E0; break;
    case S3C_GPIO_PORT_I: baseAddress = 0x7F008100; break;
    case S3C_GPIO_PORT_J: baseAddress = 0x7F008120; break;
    case S3C_GPIO_PORT_K: baseAddress = 0x7F008800; break;
    case S3C_GPIO_PORT_L: baseAddress = 0x7F008810; break;
    case S3C_GPIO_PORT_M: baseAddress = 0x7F008820; break;
    case S3C_GPIO_PORT_N: baseAddress = 0x7F008830; break;
    case S3C_GPIO_PORT_O: baseAddress = 0x7F008140; break;
    case S3C_GPIO_PORT_P: baseAddress = 0x7F008160; break;
    case S3C_GPIO_PORT_Q: baseAddress = 0x7F008180; break;

    default:
        break;
    }
    return baseAddress;
}

static __inline unsigned long mango_gpio_get_reg_offset(S3C_GPIO_PORT port, unsigned long org_offset)
{
    if (port == S3C_GPIO_PORT_H || port == S3C_GPIO_PORT_K || port == S3C_GPIO_PORT_L)
        org_offset += 0x4;

    return org_offset;
}



static __inline int mango64_gpio_port_has_large_config(S3C_GPIO_PORT port)
{
    return ((port == S3C_GPIO_PORT_H) || (port == S3C_GPIO_PORT_K) || (port == S3C_GPIO_PORT_L));
}

static __inline int mango64_gpio_get_conf_bitsnum(S3C_GPIO_PORT port)
{
    if ((port == S3C_GPIO_PORT_F) || (port == S3C_GPIO_PORT_I) || \
        (port == S3C_GPIO_PORT_J) || (port == S3C_GPIO_PORT_N) || \
        (port == S3C_GPIO_PORT_O) || (port == S3C_GPIO_PORT_P) || \
        (port == S3C_GPIO_PORT_Q)) {
        return 2;
    } else {
        return 4;
    }
}

static __inline int mango64_gpio_set_type(S3C_GPIO_PORT port, int pinnum, S3C_GPIO_TYPE type, unsigned long base)
{
    unsigned long reg;    
    unsigned long offset;
    int bitsnum, seccfg_exist;
    unsigned int mask;

    offset = 0;
    bitsnum = mango64_gpio_get_conf_bitsnum(port);
    if (bitsnum == 4)
        mask = 0xf;
    else
        mask = 0x3;
    seccfg_exist = mango64_gpio_port_has_large_config(port);
    if (seccfg_exist) {
        if (pinnum >= 8) {
            offset += 4;
            pinnum -= 8;
        }
    }
    
    reg = readl(base + offset);
    reg &= ~(mask << (pinnum * bitsnum));
    reg |= (type << (pinnum * bitsnum));
    writel(reg, base + offset);

    return 0;
}


int mango_gpio_set_type(S3C_GPIO_PORT port, int pinnum, S3C_GPIO_TYPE type)
{
    unsigned long base;

    if (port >= S3C_GPIO_PORT_MAX || type >= S3C_GPIO_TYPE_MAX) {
        printf("%s: Invalid argument\n", __func__);
        return -1;
    }

    base = mango_gpio_get_baseaddr(port);

    mango64_gpio_set_type(port, pinnum, type, base);

    return 0;
}

int mango_gpio_set_val(S3C_GPIO_PORT port, int pinnum, int val)
{
    unsigned long reg;
    unsigned long base;

    if (port >= S3C_GPIO_PORT_MAX) {
        printf("%s: Invalid argument\n", __func__);
        return -1;
    }

    base = mango_gpio_get_baseaddr(port);

    {
        reg = readl(base + mango_gpio_get_reg_offset(port, GPDAT));
        reg &= ~(1 << pinnum);
        if (val)
            reg |= (1 << pinnum);
        writel(reg, base + mango_gpio_get_reg_offset(port, GPDAT));
    }

    return 0;
}

int mango_gpio_get_val(S3C_GPIO_PORT port, int pinnum, int *val)
{
    unsigned long reg;
    unsigned long base;

    if (port >= S3C_GPIO_PORT_MAX) {
        printf("%s: Invalid argument\n", __func__);
        return -1;
    }

    base = mango_gpio_get_baseaddr(port);

    {
        reg = readl(base + mango_gpio_get_reg_offset(port, GPDAT));
        *val = (reg >> pinnum) & 0x1;
    }

    return 0;
}

int mango_gpio_set_pullupdown(S3C_GPIO_PORT port, int pinnum, S3C_GPIO_PUD pud)
{
    unsigned long reg;
    unsigned long base;

    if (port >= S3C_GPIO_PORT_MAX || pud >= S3C_GPIO_PUD_MAX) {
        printf("%s: Invalid argument\n", __func__);
        return -1;
    }

    base = mango_gpio_get_baseaddr(port);


    reg = readl(base + mango_gpio_get_reg_offset(port, GPUDP));
    reg &= ~(0x3 << (pinnum * 2));
    reg |= (pud << (pinnum * 2));
    writel(reg, base + mango_gpio_get_reg_offset(port, GPUDP));

    return 0;
}
