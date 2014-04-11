#include "main.h"

void main() {
    UINT8 address[] = {01, 02, 03};
    UINT8 rx_message_length = 0;
    message_t rx_message;

    init_pic();
    init_protocol(get_address());
    init_spi();
    init_radio(address);

    test_leds();

    CE = 1;

    while (1)
    {
        if (INTCONbits.INT0IF)
        {
            INTCONbits.INT0IF = 0;
            if (hal_nrf_get_clear_irq_flags() & (1 << HAL_NRF_RX_DR))
            {
                rx_message_length = hal_nrf_read_rx_pl_w();
                if (rx_message_length > 32)
                {
                    hal_nrf_flush_rx();
                }
                else
                {
                    hal_nrf_read_rx_pload((UINT8 *)(&rx_message));
                    hal_nrf_flush_rx();

                    rx_handler(&rx_message, rx_message_length);
                }
            }
        }

        if (INTCON3bits.INT1IF)
        {
            LED1 = 1;
            tx_message(get_address(), (UINT8 *)"Test", 4, FALSE);
            delay_ms(200);
            LED1 = 0;
            INTCON3bits.INT1IF = 0;
        }

        if (INTCON3bits.INT2IF)
        {
            LED1 = 1;
            tx_message(get_address(), (UINT8 *)"Test", 4, TRUE);
            delay_ms(200);
            LED1 = 0;
            INTCON3bits.INT2IF = 0;
        }

        handle_tx_queue();
    }
}

//void interrupt isr() {}