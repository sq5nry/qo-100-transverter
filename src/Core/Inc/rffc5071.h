/*
 * Copyright 2012-2014 Great Scott Gadgets <info@greatscottgadgets.com>
 * Copyright 2014 Jared Boone <jared@sharebrained.com>
 *
 * This file is part of HackRF.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */

#ifndef __RFFC5071_H
#define __RFFC5071_H

#include <stdint.h>

#include "spi_bus.h"
#include <stdbool.h>
#include "stm32l011xx.h"

/* 31 registers, each containing 16 bits of data. */
#define RFFC5071_NUM_REGS 31

typedef struct {
	spi_bus_t *const bus;
	GPIO_TypeDef *gpio_reset_port;
	uint32_t gpio_reset_pin;
	uint16_t regs[RFFC5071_NUM_REGS];
	uint32_t regs_dirty;
} rffc5071_driver_t;

typedef enum {
	BAND_29M = 0U,
	BAND_50M,
	BAND_51M,
	BAND_70M,
	BAND_144M,
	BAND_145M,
	BAND_432M,
	BAND_1296M
} BAND_IF;

/* Initialize chip. Call _setup() externally, as it calls _init(). */
extern void rffc5071_init(rffc5071_driver_t *const drv, BAND_IF band);
extern void rffc5071_setup(rffc5071_driver_t *const drv, BAND_IF band);

/* Read a register via SPI. Save a copy to memory and return
 * value. Discard any uncommited changes and mark CLEAN. */
extern uint16_t rffc5071_reg_read(rffc5071_driver_t *const drv, uint8_t r);

/* Write value to register via SPI and save a copy to memory. Mark
 * CLEAN. */
extern void rffc5071_reg_write(rffc5071_driver_t *const drv, uint8_t r,
		uint16_t v);

extern void rffc5071_spi_write(rffc5071_driver_t *const drv, uint8_t r, uint16_t v);

/* Write all dirty registers via SPI from memory. Mark all clean. Some
 * operations require registers to be written in a certain order. Use
 * provided routines for those operations. */
extern void rffc5071_regs_commit(rffc5071_driver_t *const drv);

/* Set up rx only, tx only, or full duplex. Chip should be disabled
 * before _tx, _rx, or _rxtx are called. */
extern void rffc5071_tx(rffc5071_driver_t *const drv);
extern void rffc5071_rx(rffc5071_driver_t *const drv);

#endif // __RFFC5071_H
