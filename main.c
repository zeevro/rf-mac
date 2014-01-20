#include "my_lib.h"

void main() {
    uint8_t address[] = {01, 02, 03};
    uint8_t rx_message_length = 0;
    uint8_t irq_source = 0;
    message_t rx_message, tx_message;

    init_pic();

    test_led();

    init_route_table();

    init_spi();

    radio_esb_init(address);

    CE_HIGH();

    /*
    tx_message.signature = MESSAGE_SIGNATURE;
    tx_message.id = 1;
    tx_message.u_src = NODE_ADDRESS;
    tx_message.u_dst = 3;
    tx_message.src = NODE_ADDRESS;
    tx_message.dst = 0;
    tx_message.hops = 0;
    *tx_message.payload = "Hello";
    if (radio_tx(&tx_message, HEADER_SIZE + 5)) test_led();
    // */

    while (1)
    {
        irq_source = wait_for_irq();

        if (!(irq_source & (1<<HAL_NRF_RX_DR)))
        {
            continue;
        }

        rx_message_length = hal_nrf_read_rx_pl_w();
        if (rx_message_length > 32)
        {
            hal_nrf_flush_rx();
            continue;
        }

        hal_nrf_read_rx_pload((uint8_t *)&rx_message);
        hal_nrf_flush_rx();

        rx_handler(&rx_message, rx_message_length);
    }
}

