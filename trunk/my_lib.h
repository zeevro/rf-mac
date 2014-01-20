#ifndef _MY_LIB_H
#define _MY_LIB_H

typedef unsigned char bool;
typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
#define true (1)
#define false (0)

// #define __18F4520

#include <p18f4520.h>
#include <plib/spi.h>
#include <string.h>

#include "hal_nrf.h"
#include "protocol.h"

#define CSN PORTBbits.RB2  // CSN PIN for BFB
#define CE  PORTBbits.RB1  // CE PIN
#define IRQ PORTBbits.RB0  // IRQ PIN

#define GPIO1 PORTBbits.RB3
#define GPIO2 PORTBbits.RB4

#define LED PORTBbits.RB5

#define LED_ON  1
#define LED_OFF 0

#define MAX_PAYLOAD_LENGTH 5

void delay_us(const unsigned long delay);

void delay_ms(const unsigned long delay);

void init_pic();

void init_spi();

void test_led();

uint8_t hal_nrf_rw(uint8_t value);

void xfer(uint8_t recv[], const uint8_t send[], const uint8_t length);

void radio_esb_init(const uint8_t * address);

uint8_t wait_for_irq();

bool radio_tx(const message_t * message, const uint8_t length);

#endif // _MY_LIB_H