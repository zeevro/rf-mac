#ifndef _HAL_H
#define _HAL_H

#define CSN PORTDbits.RD0 // Out
#define CE  PORTDbits.RD1 // Out
#define IRQ PORTBbits.RB0 // In

#define BUTTON1 PORTBbits.RB1 // In
#define BUTTON2 PORTBbits.RB2 // In

#define LED1 PORTCbits.RC0 // Out
#define LED2 PORTCbits.RC1 // Out

#define TRIS 0xFF

#define SYS_FREQ 8000

void delay_us(UINT32 delay);
void delay_ms(UINT32 delay);
void init_pic();
void init_spi();
node_address_t get_address();
void test_leds();
UINT8 spi_rw(UINT8 value);
void init_radio(UINT8 * address);
UINT8 wait_for_irq();
BOOL radio_tx(UINT8 * message, UINT8 length);

#endif /* _HAL_H */