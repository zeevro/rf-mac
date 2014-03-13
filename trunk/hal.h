#ifndef _HAL_H
#define _HAL_H

#define CSN PORTBbits.RB2
#define CE  PORTBbits.RB1
#define IRQ PORTBbits.RB0

#define GPIO1 PORTBbits.RB3
#define GPIO2 PORTBbits.RB4

#define LED PORTBbits.RB5

#define SYS_FREQ 8000

void delay_us(UINT32 delay);
void delay_ms(UINT32 delay);
void init_pic();
void init_spi();
node_address_t get_address();
void test_led();
UINT8 spi_rw(UINT8 value);
void init_radio(UINT8 * address);
UINT8 wait_for_irq();
BOOL radio_tx(UINT8 * message, UINT8 length);

#endif /* _HAL_H */