#ifndef _MAIN_H
#define _MAIN_H

#include <GenericTypeDefs.h>
#include <xc.h>
#include <plib/spi.h>
#include <plib/portb.h>
#include <plib/usart.h>
#include <string.h>

#include "message_ids_queue.h"
#include "protocol.h"
#include "tx_queue.h"
#include "hal.h"
#include "nRF24L01P.h"

#define BIT_0 (1 << 0)
#define BIT_1 (1 << 1)
#define BIT_2 (1 << 2)
#define BIT_3 (1 << 3)
#define BIT_4 (1 << 4)
#define BIT_5 (1 << 5)
#define BIT_6 (1 << 6)
#define BIT_7 (1 << 7)

#endif /* _MAIN_H */