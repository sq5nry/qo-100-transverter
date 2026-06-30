/*
 * Copyright 2017-2022 Great Scott Gadgets <info@greatscottgadgets.com>
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

#include "mixer.h"
#include "rffc5071.h"
#include "rffc5071_spi.h"
#include "stm32l0xx_hal.h"
#include "main.h"

/* RFFC5071 GPIO serial interface PinMux */
// clang-format off

const rffc5071_spi_config_t rffc5071_spi_config = {
		.gpio_select_port = RFFC_Port, .gpio_select_pin = RFFC_ENX_Pin,
		.gpio_clock_port = RFFC_Port, .gpio_clock_pin = RFFC_SCLK_Pin,
		.gpio_data_port = RFFC_Port, .gpio_data_pin = RFFC_SDATA_Pin,
};

spi_bus_t spi_bus_rffc5071 = {
		.config = &rffc5071_spi_config,
		.start = rffc5071_spi_start,
		.transfer = rffc5071_spi_transfer,
		.transfer_gather = rffc5071_spi_transfer_gather,
};

rffc5071_driver_t mixer = {
		.bus = &spi_bus_rffc5071,
};


rffc5071_driver_t* mixer_bus_setup() {
	spi_bus_start(&spi_bus_rffc5071, &rffc5071_spi_config);
	return &mixer;
}
