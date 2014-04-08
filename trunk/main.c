#include "main.h"

void main() {
    UINT8 address[] = {01, 02, 03};
    UINT8 rx_message_length = 0;
    message_t rx_message;

    init_pic();
    init_protocol(get_address());
    init_spi();
    init_radio(address);

    test_led();

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

        if (INTCONbits.RBIF)
        {
            if (PORTBbits.RB4 == 0)
            {
                LED = 1;
                tx_message(2, (UINT8 *)"Hello", 5, TRUE);
                delay_ms(100);
                LED = 0;
            }
            else
            {
                delay_ms(100);
            }
            INTCONbits.RBIF = 0;
        }

        handle_tx_queue();
    }
}

//void interrupt isr() {}