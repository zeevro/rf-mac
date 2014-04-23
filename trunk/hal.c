#pragma config OSC = INTIO67
#pragma config WDT = OFF
#pragma config PBADEN = OFF
//#pragma config MCLRE = OFF
#pragma config LVP = OFF

#include "main.h"

void delay_us(UINT32 delay) {
    // 8MHz clock
    unsigned long x = delay / 6;
    while(--x > 0);
}

void delay_ms(UINT32 delay) {
    // 8MHz clock
    unsigned long i = 150, j = delay;
    while (--i > 0)
    {
        while (--j > 0);
        j = delay;
    }
}

void init_pic() {
    OSCCON = 0x73; // 8 MHz
    OSCTUNEbits.PLLEN = 0; // No PLL

    TRISA = 0;
    TRISB = 0;
    TRISC = 0;
    TRISD = 0;
    TRISE = 0;

    TRISB = TRIS;

    LED1 = 0;
    LED2 = 0;
    CE = 0;

    OpenRB0INT(PORTB_CHANGE_INT_ON & FALLING_EDGE_INT & PORTB_PULLUPS_ON);
    OpenRB1INT(PORTB_CHANGE_INT_ON & FALLING_EDGE_INT & PORTB_PULLUPS_ON);
    OpenRB2INT(PORTB_CHANGE_INT_ON & FALLING_EDGE_INT & PORTB_PULLUPS_ON);

    INTCONbits.GIE = 1;
    INTCONbits.GIEL = 1;
}

void init_spi() {
    CSN = 1;
    OpenSPI(SPI_FOSC_64, MODE_00, SMPEND);
}

node_address_t get_address() {
    return (((~PORTB) >> 3) & 7) + 1;
}

void test_leds() {
    LED1 = 1;
    LED2 = 1;
    delay_ms(800);
    LED1 = 0;
    LED2 = 0;
}

UINT8 spi_rw(UINT8 value) {
    WriteSPI(value);
    while (!DataRdySPI());
    return SSPBUF;
}

void init_radio(UINT8 * address) {
    delay_ms(100);
    hal_nrf_close_pipe(HAL_NRF_ALL);                        // First close all radio pipes (0 and 1 are open by default)
    hal_nrf_open_pipe(HAL_NRF_PIPE0, FALSE);                // Then open pipe0, w/o autoack
    hal_nrf_set_crc_mode(HAL_NRF_CRC_16BIT);                // Operate in 16-bit CRC mode
    hal_nrf_set_auto_retr(0, 250);                          // 0 retrans with 250ms delay
    hal_nrf_set_address_width(HAL_NRF_AW_3BYTES);           // 3-byte address width
    hal_nrf_set_address(HAL_NRF_TX, (UINT8 *)address);      // Set device's address
    hal_nrf_set_address(HAL_NRF_PIPE0, (UINT8 *)address);   // Set recieving address on pipe0
    hal_nrf_set_operation_mode(HAL_NRF_PRX);                // Enter RX mode
    hal_nrf_enable_dynamic_pl();                            // Enable dynamic payload size
    hal_nrf_setup_dyn_pl(1);                                // Enable dynamic payload size on pipe 0
    hal_nrf_set_rf_channel(2);                              // Operating on static channel. Frequency = (2400 + RF_CHANNEL) KHz
    hal_nrf_set_datarate(HAL_NRF_2MBPS);                    // 2Mbps data rate
    hal_nrf_set_power_mode(HAL_NRF_PWR_UP);                 // Power up device
    delay_us(130);                                          // Wait for the radio to power up
}

UINT8 wait_for_irq() {
    while (IRQ == 1);

    return hal_nrf_get_clear_irq_flags();
}

BOOL radio_tx(UINT8 * message, UINT8 length) {
    UINT8 tx_ret;
    BOOL success;

    if (length > 32) return FALSE;

    hal_nrf_write_tx_pload(message, length);

    CE = 0;
    hal_nrf_set_operation_mode(HAL_NRF_PTX);

    INTCONbits.INT0IE = 0;
    CE = 1;
    delay_us(10);
    CE = 0;
    INTCONbits.INT0IF = 0;
    INTCONbits.INT0IE = 1;

    tx_ret = wait_for_irq();
    success = (tx_ret & (1 << HAL_NRF_TX_DS)) >> HAL_NRF_TX_DS;

    hal_nrf_set_operation_mode(HAL_NRF_PRX);
    CE = 1;

    return success;
}