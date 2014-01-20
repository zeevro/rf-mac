#include "my_lib.h"

void main() {
    uint8_t address[] = {01, 02, 03};
    uint8_t rx_message_length = 0;
    message_t rx_message;

    init_pic();

    test_led();

    init_protocol();

    init_spi();

    radio_esb_init(address);

    CE_HIGH();

    while (1)
    {
        if (IRQ == 0 && (hal_nrf_get_clear_irq_flags() & (1<<HAL_NRF_RX_DR)))
        {
            rx_message_length = hal_nrf_read_rx_pl_w();
            if (rx_message_length > 32)
            {
                hal_nrf_flush_rx();
            }
            else
            {
                hal_nrf_read_rx_pload((uint8_t *)&rx_message);
                hal_nrf_flush_rx();

                rx_handler(&rx_message, rx_message_length);
            }
        }

        if (GPIO1 == 1)
        {
            tx_message(3, "Hello", 5);
        }

        if (GPIO2 == 1)
        {
            tx_message(3, "Bye", 3);
        }
    }
}

