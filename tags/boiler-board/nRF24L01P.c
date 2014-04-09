#include "main.h"

#define SET_BIT(pos) ((UINT8) (1<<( (UINT8) (pos) )))

void hal_nrf_set_irq_mode(hal_nrf_irq_source_t int_source, BOOL irq_state) {
    if (irq_state)
    {
        hal_nrf_write_reg(CONFIG, hal_nrf_read_reg(CONFIG) & ~SET_BIT(int_source));
    }
    else
    {
        hal_nrf_write_reg(CONFIG, hal_nrf_read_reg(CONFIG) | SET_BIT(int_source));
    }
}

UINT8 hal_nrf_get_clear_irq_flags() {
    return hal_nrf_write_reg(NRF_STATUS, (BIT_6 | BIT_5 | BIT_4)) & (BIT_6 | BIT_5 | BIT_4);
}

void hal_nrf_clear_irq_flag(hal_nrf_irq_source_t int_source) {
    hal_nrf_write_reg(NRF_STATUS, SET_BIT(int_source));
}

BOOL hal_nrf_get_irq_mode(UINT8 int_type) {
    if (hal_nrf_read_reg(CONFIG) & SET_BIT(int_type))
        return FALSE;
    else
        return TRUE;
}

UINT8 hal_nrf_get_irq_flags() {
    return hal_nrf_nop() & (BIT_6 | BIT_5 | BIT_4);
}

void hal_nrf_set_crc_mode(hal_nrf_crc_mode_t crc_mode) {
    hal_nrf_write_reg(CONFIG, (hal_nrf_read_reg(CONFIG) & ~(BIT_3 | BIT_2)) | ((UINT8)crc_mode << 2));
}

void hal_nrf_open_pipe(hal_nrf_address_t pipe_num, BOOL auto_ack) {
    switch (pipe_num)
    {
        case HAL_NRF_PIPE0:
        case HAL_NRF_PIPE1:
        case HAL_NRF_PIPE2:
        case HAL_NRF_PIPE3:
        case HAL_NRF_PIPE4:
        case HAL_NRF_PIPE5:
            hal_nrf_write_reg(EN_RXADDR, hal_nrf_read_reg(EN_RXADDR) | SET_BIT(pipe_num));

            if (auto_ack)
                hal_nrf_write_reg(EN_AA, hal_nrf_read_reg(EN_AA) | SET_BIT(pipe_num));
            else
                hal_nrf_write_reg(EN_AA, hal_nrf_read_reg(EN_AA) & ~SET_BIT(pipe_num));
            break;

        case HAL_NRF_ALL:
            hal_nrf_write_reg(EN_RXADDR, (UINT8)(~(BIT_7 | BIT_6)));

            if (auto_ack)
                hal_nrf_write_reg(EN_AA, (UINT8)(~(BIT_7 | BIT_6)));
            else
                hal_nrf_write_reg(EN_AA, 0);
            break;

        default:
            break;
    }
}

void hal_nrf_close_pipe(hal_nrf_address_t pipe_num) {
    switch (pipe_num)
    {
        case HAL_NRF_PIPE0:
        case HAL_NRF_PIPE1:
        case HAL_NRF_PIPE2:
        case HAL_NRF_PIPE3:
        case HAL_NRF_PIPE4:
        case HAL_NRF_PIPE5:
            hal_nrf_write_reg(EN_RXADDR, hal_nrf_read_reg(EN_RXADDR) & ~SET_BIT(pipe_num));
            hal_nrf_write_reg(EN_AA, hal_nrf_read_reg(EN_AA) & ~SET_BIT(pipe_num));
            break;

        case HAL_NRF_ALL:
            hal_nrf_write_reg(EN_RXADDR, 0);
            hal_nrf_write_reg(EN_AA, 0);
            break;

        default:
            break;
    }
}

void hal_nrf_set_address(hal_nrf_address_t address, UINT8 * addr) {
    switch (address)
    {
        case HAL_NRF_TX:
        case HAL_NRF_PIPE0:
        case HAL_NRF_PIPE1:
            hal_nrf_write_multibyte_reg((UINT8)address, addr, 0);
            break;

        case HAL_NRF_PIPE2:
        case HAL_NRF_PIPE3:
        case HAL_NRF_PIPE4:
        case HAL_NRF_PIPE5:
            hal_nrf_write_reg(RX_ADDR_P0 + (UINT8)address, *addr);
            break;

        default:
            break;
    }
}

void hal_nrf_set_auto_retr(UINT8 retr, UINT16 delay) {
    hal_nrf_write_reg(SETUP_RETR, (((delay / 250) - 1) << 4) | retr);
}

void hal_nrf_set_address_width(hal_nrf_address_width_t address_width) {
    hal_nrf_write_reg(SETUP_AW, ((UINT8)address_width - 2));
}

void hal_nrf_set_rx_pload_width(UINT8 pipe_num, UINT8 pload_width) {
    hal_nrf_write_reg(RX_PW_P0 + pipe_num, pload_width);
}

UINT8 hal_nrf_get_crc_mode() {
    return (hal_nrf_read_reg(CONFIG) & (BIT_3 | BIT_2)) >> CRCO;
}

UINT8 hal_nrf_get_pipe_status(UINT8 pipe_num) {
    UINT8 en_rx, en_aa;

    en_rx = hal_nrf_read_reg(EN_RXADDR) & (1 << pipe_num);
    en_aa = hal_nrf_read_reg(EN_AA) & (1 << pipe_num);

    en_rx >>= pipe_num;
    en_aa >>= pipe_num;

    return (en_aa << 1) +en_rx;
}

UINT8 hal_nrf_get_address(UINT8 address, UINT8 * addr) {
    switch (address)
    {
        case HAL_NRF_PIPE0:
        case HAL_NRF_PIPE1:
        case HAL_NRF_TX:
            return hal_nrf_read_multibyte_reg(address, addr);

        default:
            *addr = hal_nrf_read_reg(RX_ADDR_P0 + address);
            return hal_nrf_get_address_width();
    }
}

UINT8 hal_nrf_get_auto_retr_status() {
    return hal_nrf_read_reg(OBSERVE_TX);
}

UINT8 hal_nrf_get_packet_lost_ctr() {
    return (hal_nrf_read_reg(OBSERVE_TX) & (BIT_7 | BIT_6 | BIT_5 | BIT_4)) >> 4;
}

UINT8 hal_nrf_get_address_width() {
    return (hal_nrf_read_reg(SETUP_AW) + 2);
}

UINT8 hal_nrf_get_rx_pload_width(UINT8 pipe_num) {
    return hal_nrf_read_reg(RX_PW_P0 + pipe_num);
}

void hal_nrf_set_operation_mode(hal_nrf_operation_mode_t op_mode) {
    if (op_mode == HAL_NRF_PRX)
    {
        hal_nrf_write_reg(CONFIG, (hal_nrf_read_reg(CONFIG) | (1 << PRIM_RX)));
    }
    else
    {
        hal_nrf_write_reg(CONFIG, (hal_nrf_read_reg(CONFIG) & ~(1 << PRIM_RX)));
    }
}

void hal_nrf_set_power_mode(hal_nrf_pwr_mode_t pwr_mode) {
    if (pwr_mode == HAL_NRF_PWR_UP)
    {
        hal_nrf_write_reg(CONFIG, (hal_nrf_read_reg(CONFIG) | (1 << PWR_UP)));
    }
    else
    {
        hal_nrf_write_reg(CONFIG, (hal_nrf_read_reg(CONFIG) & ~(1 << PWR_UP)));
    }
}

void hal_nrf_set_rf_channel(UINT8 channel) {
    hal_nrf_write_reg(RF_CH, channel);
}

void hal_nrf_set_output_power(hal_nrf_output_power_t power) {
    hal_nrf_write_reg(RF_SETUP, (hal_nrf_read_reg(RF_SETUP) & ~((1 << RF_PWR1) | (1 << RF_PWR0))) | ((UINT8)power << 1));
}

void hal_nrf_set_datarate(hal_nrf_datarate_t datarate) {
    if (datarate == HAL_NRF_1MBPS)
    {
        hal_nrf_write_reg(RF_SETUP, (hal_nrf_read_reg(RF_SETUP) & ~(1 << RF_DR)));
    }
    else
    {
        hal_nrf_write_reg(RF_SETUP, (hal_nrf_read_reg(RF_SETUP) | (1 << RF_DR)));
    }
}

UINT8 hal_nrf_get_operation_mode() {
    return (hal_nrf_read_reg(CONFIG) & (1 << PRIM_RX)) >> PRIM_RX;
}

UINT8 hal_nrf_get_power_mode() {
    return (hal_nrf_read_reg(CONFIG) & (1 << PWR_UP)) >> PWR_UP;
}

UINT8 hal_nrf_get_rf_channel() {
    return hal_nrf_read_reg(RF_CH);
}

UINT8 hal_nrf_get_output_power() {
    return (hal_nrf_read_reg(RF_SETUP) & ((1 << RF_PWR1) | (1 << RF_PWR0))) >> RF_PWR0;
}

UINT8 hal_nrf_get_datarate() {
    return (hal_nrf_read_reg(RF_SETUP) & (1 << RF_DR)) >> RF_DR;
}

BOOL hal_nrf_rx_fifo_empty() {
    if (hal_nrf_get_rx_data_source() == 7)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

BOOL hal_nrf_rx_fifo_full() {
    return (BOOL)((hal_nrf_read_reg(FIFO_STATUS) >> RX_EMPTY) & 1);
}

BOOL hal_nrf_tx_fifo_empty() {
    return (BOOL)((hal_nrf_read_reg(FIFO_STATUS) >> TX_EMPTY) & 1);
}

BOOL hal_nrf_tx_fifo_full() {
    return (BOOL)((hal_nrf_read_reg(FIFO_STATUS) >> TX_FIFO_FULL) & 1);
}

UINT8 hal_nrf_get_tx_fifo_status() {
    return ((hal_nrf_read_reg(FIFO_STATUS) & ((1 << TX_FIFO_FULL) | (1 << TX_EMPTY))) >> 4);
}

UINT8 hal_nrf_get_rx_fifo_status() {
    return (hal_nrf_read_reg(FIFO_STATUS) & ((1 << RX_FULL) | (1 << RX_EMPTY)));
}

UINT8 hal_nrf_get_fifo_status() {
    return hal_nrf_read_reg(FIFO_STATUS);
}

UINT8 hal_nrf_get_transmit_attempts() {
    return hal_nrf_read_reg(OBSERVE_TX) & (BIT_3 | BIT_2 | BIT_1 | BIT_0);
}

BOOL hal_nrf_get_carrier_detect() {
    return hal_nrf_read_reg(CD) & 1;
}

void hal_nrf_write_tx_pload(UINT8 *tx_pload, UINT8 length) {
    hal_nrf_write_multibyte_reg((UINT8)HAL_NRF_TX_PLOAD, tx_pload, length);
}

void hal_nrf_setup_dyn_pl(UINT8 setup) {
    hal_nrf_write_reg(DYNPD, setup & ~0xC0);
}

void hal_nrf_enable_dynamic_pl() {
    hal_nrf_write_reg(FEATURE, (hal_nrf_read_reg(FEATURE) | 0x04));
}

void hal_nrf_disable_dynamic_pl() {
    hal_nrf_write_reg(FEATURE, (hal_nrf_read_reg(FEATURE) & ~0x04));
}

void hal_nrf_enable_ack_pl() {
    hal_nrf_write_reg(FEATURE, (hal_nrf_read_reg(FEATURE) | 0x02));
}

void hal_nrf_disable_ack_pl() {
    hal_nrf_write_reg(FEATURE, (hal_nrf_read_reg(FEATURE) & ~0x02));
}

void hal_nrf_enable_dynamic_ack() {
    hal_nrf_write_reg(FEATURE, (hal_nrf_read_reg(FEATURE) | 0x01));
}

void hal_nrf_disable_dynamic_ack() {
    hal_nrf_write_reg(FEATURE, (hal_nrf_read_reg(FEATURE) & ~0x01));
}

void hal_nrf_write_ack_pload(UINT8 pipe, UINT8 *tx_pload, UINT8 length) {
    CSN = 0;

    spi_rw(WR_ACK_PLOAD | pipe);
    while (length--)
    {
        spi_rw(*tx_pload++);
    }

    CSN = 1;
}

UINT8 hal_nrf_read_rx_pl_w() {
    UINT8 temp;

    CSN = 0;

    spi_rw(RD_RX_PLOAD_W);
    temp = spi_rw(0);
    CSN = 1;

    return temp;
}

void hal_nrf_lock_unlock() {
    CSN = 0;

    spi_rw(LOCK_UNLOCK);
    spi_rw(0x73);

    CSN = 1;
}

UINT8 hal_nrf_get_rx_data_source() {
    return ((hal_nrf_nop() & (BIT_3 | BIT_2 | BIT_1)) >> 1);
}

// Fixed: returns length==0 and pipe==7 means FIFO empty

UINT16 hal_nrf_read_rx_pload(UINT8 *rx_pload) {
    return hal_nrf_read_multibyte_reg((UINT8)HAL_NRF_RX_PLOAD, rx_pload);
}

void hal_nrf_reuse_tx() {
    hal_nrf_write_reg(REUSE_TX_PL, 0);
}

BOOL hal_nrf_get_reuse_tx_status() {
    return (BOOL)((hal_nrf_get_fifo_status() & (1 << TX_REUSE)) >> TX_REUSE);
}

void hal_nrf_flush_rx() {
    hal_nrf_write_reg(FLUSH_RX, 0);
}

void hal_nrf_flush_tx() {
    hal_nrf_write_reg(FLUSH_TX, 0);
}

UINT8 hal_nrf_nop() {
    return hal_nrf_write_reg(RFNOP, 0);
}

void hal_nrf_set_pll_mode(hal_nrf_pll_mode_t pll_mode) {
    if (pll_mode == HAL_NRF_PLL_LOCK)
    {
        hal_nrf_write_reg(RF_SETUP, (hal_nrf_read_reg(RF_SETUP) | (1 << PLL_LOCK)));
    }
    else
    {
        hal_nrf_write_reg(RF_SETUP, (hal_nrf_read_reg(RF_SETUP) & ~(1 << PLL_LOCK)));
    }
}

hal_nrf_pll_mode_t hal_nrf_get_pll_mode() {
    return (hal_nrf_pll_mode_t)((hal_nrf_read_reg(RF_SETUP) & (1 << PLL_LOCK)) >> PLL_LOCK);
}

void hal_nrf_set_lna_gain(hal_nrf_lna_mode_t lna_gain) {
    if (lna_gain == HAL_NRF_LNA_HCURR)
    {
        hal_nrf_write_reg(RF_SETUP, (hal_nrf_read_reg(RF_SETUP) | (1 << LNA_HCURR)));
    }
    else
    {
        hal_nrf_write_reg(RF_SETUP, (hal_nrf_read_reg(RF_SETUP) & ~(1 << LNA_HCURR)));
    }
}

hal_nrf_lna_mode_t hal_nrf_get_lna_gain() {
    return (hal_nrf_lna_mode_t)((hal_nrf_read_reg(RF_SETUP) & (1 << LNA_HCURR)) >> LNA_HCURR);
}

UINT8 hal_nrf_read_reg(UINT8 reg) {
    UINT8 temp;
    CSN = 0;
    spi_rw(reg);
    temp = spi_rw(0);
    CSN = 1;

    return temp;
}

UINT8 hal_nrf_write_reg(UINT8 reg, UINT8 value) {
    UINT8 retval;
    CSN = 0;
    if (reg < WRITE_REG) // i.e. this is a register access
    {
        retval = spi_rw(WRITE_REG + reg);
        spi_rw(value);
    }
    else // single byte cmd OR future command/register access
    {
        if (!(reg == FLUSH_TX) && !(reg == FLUSH_RX) && !(reg == REUSE_TX_PL) && !(reg == RFNOP))
        {
            retval = spi_rw(reg);
            spi_rw(value);
        }
        else // single byte L01 command
        {
            retval = spi_rw(reg);
        }
    }
    CSN = 1;

    return retval;
}

UINT16 hal_nrf_read_multibyte_reg(UINT8 reg, UINT8 pbuf[]) {
    UINT8 ctr, length;
    switch (reg)
    {
        case HAL_NRF_PIPE0:
        case HAL_NRF_PIPE1:
        case HAL_NRF_TX:
            length = ctr = hal_nrf_get_address_width();
            CSN = 0;
            ;
            spi_rw(RX_ADDR_P0 + reg);
            break;

        case HAL_NRF_RX_PLOAD:
            if ((reg = hal_nrf_get_rx_data_source()) < 7)
            {
                length = ctr = hal_nrf_read_rx_pl_w();

                CSN = 0;
                spi_rw(RD_RX_PLOAD);
            }
            else
            {
                ctr = length = 0;
            }
            break;

        default:
            ctr = length = 0;
            break;
    }

    do
    {
        pbuf[--ctr] = spi_rw(0xFF);
    } while (ctr);

    CSN = 1;

    return (((UINT16)reg << 8) | length);
}

void hal_nrf_write_multibyte_reg(UINT8 reg, UINT8 pbuf[], UINT8 length) {
    switch (reg)
    {
        case HAL_NRF_PIPE0:
        case HAL_NRF_PIPE1:
        case HAL_NRF_TX:
            length = hal_nrf_get_address_width();
            CSN = 0;
            spi_rw(WRITE_REG + RX_ADDR_P0 + reg);
            break;

        case HAL_NRF_TX_PLOAD:
            CSN = 0;
            spi_rw(WR_TX_PLOAD);
            break;
        default:
            break;
    }

    do
    {
        spi_rw(pbuf[--length]);
    } while (length);

    CSN = 1;
}