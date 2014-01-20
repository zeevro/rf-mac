#pragma config OSC = INTIO67
#pragma config WDT = OFF
#pragma config PBADEN = OFF
#pragma config MCLRE = OFF
#pragma config LVP = OFF

#include "my_lib.h"

void delay_us(const unsigned long delay) {
    // 8MHz clock
    unsigned long x = delay / 6;
    while(--x > 0);
}

void delay_ms(const unsigned long delay) {
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

    TRISB = 0x01;

    delay_ms(500);

    LED = LED_OFF;
    CE_LOW();
}

void init_spi() {
    CSN = 1;
    OpenSPI(SPI_FOSC_64, MODE_00, SMPEND);
}

void test_led() {
    LED = LED_ON;
    delay_ms(800);
    LED = LED_OFF;
}

uint8_t hal_nrf_rw(uint8_t value) {
    WriteSPI(value);
    while (!DataRdySPI());
    return SSPBUF;
}

void xfer(uint8_t recv[], const uint8_t send[], const uint8_t length) {
    int i;

    CSN = 0;

    for (i = 0; i < length; i++)
    {
        WriteSPI(send[i]);
        while (!DataRdySPI());
        recv[i] = SSPBUF;
    }

    CSN = 1;
}

void radio_esb_init(const uint8_t * address) {
  delay_ms(100);
  hal_nrf_close_pipe(HAL_NRF_ALL);                        // First close all radio pipes (0 and 1 are open by default)
  hal_nrf_open_pipe(HAL_NRF_PIPE0, false);                // Then open pipe0, w/o autoack
  hal_nrf_set_crc_mode(HAL_NRF_CRC_16BIT);                // Operate in 16-bit CRC mode
  hal_nrf_set_auto_retr(0, 250);                          // 0 retrans with 250ms delay
  hal_nrf_set_address_width(HAL_NRF_AW_3BYTES);           // 3-byte address width
  hal_nrf_set_address(HAL_NRF_TX, (uint8_t *)address);    // Set device's address
  hal_nrf_set_address(HAL_NRF_PIPE0, (uint8_t *)address); // Set recieving address on pipe0
  hal_nrf_set_operation_mode(HAL_NRF_PRX);                // Enter RX mode
  hal_nrf_enable_dynamic_pl();                            // Enable dynamic payload size
  hal_nrf_setup_dyn_pl(1);                                // Enable dynamic payload size on pipe 0
  hal_nrf_set_rf_channel(2);                              // Operating on static channel. Frequency = (2400 + RF_CHANNEL) KHz
  hal_nrf_set_power_mode(HAL_NRF_PWR_UP);                 // Power up device
  delay_us(130);                                          // Wait for the radio to power up
}

uint8_t wait_for_irq() {
    while (IRQ == 1);

    return hal_nrf_get_clear_irq_flags();
}

bool radio_tx(const message_t * message, const uint8_t length) {
    uint8_t tx_ret;
    bool success;

    if (length > 32) return false;

    hal_nrf_write_tx_pload((uint8_t *)message, length);

    CE_LOW();
    hal_nrf_set_operation_mode(HAL_NRF_PTX);

    CE_HIGH();
    delay_us(10);
    CE_LOW();

    tx_ret = wait_for_irq();
    success = (tx_ret & (1<<HAL_NRF_TX_DS)) >> HAL_NRF_TX_DS;

    hal_nrf_set_operation_mode(HAL_NRF_PRX);
    CE_HIGH();

    return success;
}