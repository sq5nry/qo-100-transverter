/*
 * Copyright 2012-2022 Great Scott Gadgets <info@greatscottgadgets.com>
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

/*
 * 'gcc -DTEST -DDEBUG -O2 -o test rffc5071.c' prints out what test
 * program would do if it had a real spi library
 */

/*
 * The actual part on Jawbreaker is the RFFC5072, not the RFFC5071, but the
 * RFFC5071 may be installed instead.  The only difference between the parts is
 * that the RFFC5071 includes a second mixer.
 */

#include <stdint.h>
#include <string.h>
#include "rffc5071.h"
#include "rffc5071_regs.def" // private register def macros

/* Second pass over rffc5071_regs.def: for every field NAME generate
 * NAME_R (register), NAME_O (bit offset), NAME_L (bit length). This lets
 * the init code use the field definitions as compact data instead of
 * calling one set_ accessor per field (which costs ~70 B each at -O0). */
#undef __RFFC5071_REGS_DEF
#undef __MREG__
#define __MREG__(n,r,o,l) enum { n##_R = r, n##_O = o, n##_L = l };
#include "rffc5071_regs.def"

typedef struct {
	uint16_t r : 5; /* register 0..30 */
	uint16_t o : 4; /* bit offset */
	uint16_t l : 5; /* bit length 1..16 */
	uint16_t v;     /* field value */
} rffc5071_field_t;

#define RFFC_FIELD(n, val) { n##_R, n##_O, n##_L, (val) }
#define RFFC_SET(drv, n, val) rffc5071_field_set((drv), n##_R, n##_O, n##_L, (val))

static void rffc5071_field_set(rffc5071_driver_t *const drv,
		uint8_t r, uint8_t o, uint8_t l, uint16_t v) {
	const uint16_t mask = (uint16_t)(((1UL << l) - 1) << o);
	drv->regs[r] = (uint16_t)((drv->regs[r] & ~mask) | (((uint32_t)v << o) & mask));
	RFFC5071_REG_SET_DIRTY(drv, r);
}
#include "stm32l0xx_hal.h"

/* Common register defaults (same for all bands), set field by field
 * through the accessors from rffc5071_regs.def. Band-dependent registers
 * (MIX_CONT, P1_FREQx, P2_FREQx) are set in rffc5071_set_band(). */
static const rffc5071_field_t rffc5071_defaults[] = {
	/* 00 LF = 0xBEFA */
	RFFC_FIELD(RFFC5071_PLLCPL, 0x2),
	RFFC_FIELD(RFFC5071_P1CPDEF, 0x1f),
	RFFC_FIELD(RFFC5071_P2CPDEF, 0x1f),
	RFFC_FIELD(RFFC5071_IFACT, 1),

	/* 01 XO = 0x4064 */
	RFFC_FIELD(RFFC5071_SUWAIT, 0x64),
	RFFC_FIELD(RFFC5071_XOCF, 0),
	RFFC_FIELD(RFFC5071_XOC, 0x8),
	RFFC_FIELD(RFFC5071_XOCH, 0),

	/* 02 CAL_TIME = 0x9055 */
	RFFC_FIELD(RFFC5071_TKV2, 0x5),
	RFFC_FIELD(RFFC5071_TKV1, 0x5),
	RFFC_FIELD(RFFC5071_TCT, 0x4),
	RFFC_FIELD(RFFC5071_WAIT, 1),

	/* 03 VCO_CTRL = 0x2D02 */
	RFFC_FIELD(RFFC5071_ICPUP, 0x1),
	RFFC_FIELD(RFFC5071_REFST, 0),
	RFFC_FIELD(RFFC5071_XOI3, 0),
	RFFC_FIELD(RFFC5071_XOI2, 0),
	RFFC_FIELD(RFFC5071_XOI1, 0),
	RFFC_FIELD(RFFC5071_KVPOL, 0),
	RFFC_FIELD(RFFC5071_KVRNG, 1),
	RFFC_FIELD(RFFC5071_KVAVG, 0x2),
	RFFC_FIELD(RFFC5071_CLKPL, 1),
	RFFC_FIELD(RFFC5071_CTPOL, 0),
	RFFC_FIELD(RFFC5071_CTAVG, 0x1),
	RFFC_FIELD(RFFC5071_XTVCO, 0),

	/* 04 CT_CAL1 = 0xB0BF */
	RFFC_FIELD(RFFC5071_P1CTDEF, 0x3f),
	RFFC_FIELD(RFFC5071_P1CT, 1),
	RFFC_FIELD(RFFC5071_P1CTV, 0x10),
	RFFC_FIELD(RFFC5071_P1CTGAIN, 0x5),

	/* 05 CT_CAL2 = 0xB0BF */
	RFFC_FIELD(RFFC5071_P2CTDEF, 0x3f),
	RFFC_FIELD(RFFC5071_P2CT, 1),
	RFFC_FIELD(RFFC5071_P2CTV, 0x10),
	RFFC_FIELD(RFFC5071_P2CTGAIN, 0x5),

	/* 06 PLL_CAL1 = 0x0028 */
	RFFC_FIELD(RFFC5071_P1SGN, 0),
	RFFC_FIELD(RFFC5071_P1KVGAIN, 0x5),
	RFFC_FIELD(RFFC5071_P1DN, 0x0),
	RFFC_FIELD(RFFC5071_P1KV, 0),

	/* 07 PLL_CAL2 = 0x0028 */
	RFFC_FIELD(RFFC5071_P2SGN, 0),
	RFFC_FIELD(RFFC5071_P2KVGAIN, 0x5),
	RFFC_FIELD(RFFC5071_P2DB, 0x0),
	RFFC_FIELD(RFFC5071_P2KV, 0),

	/* 08 VCO_AUTO = 0xFC06 */
	RFFC_FIELD(RFFC5071_CTMIN, 0x3),
	RFFC_FIELD(RFFC5071_CTMAX, 0x7c),
	RFFC_FIELD(RFFC5071_AUTO, 1),

	/* 09 PLL_CTRL = 0x8220 */
	RFFC_FIELD(RFFC5071_PLLDY, 0x0),
	RFFC_FIELD(RFFC5071_ALOI, 0),
	RFFC_FIELD(RFFC5071_RELOK, 0),
	RFFC_FIELD(RFFC5071_LDLEV, 0),
	RFFC_FIELD(RFFC5071_LDEN, 1),
	RFFC_FIELD(RFFC5071_TVCO, 0x8),
	RFFC_FIELD(RFFC5071_PLLST, 0),
	RFFC_FIELD(RFFC5071_CLKDIV, 0x0),
	RFFC_FIELD(RFFC5071_DIVBY, 1),

	/* 0A PLL_BIAS = 0x0202 */
	RFFC_FIELD(RFFC5071_P2VCOI, 0x2),
	RFFC_FIELD(RFFC5071_P2LOI, 0x0),
	RFFC_FIELD(RFFC5071_P1VCOI, 0x2),
	RFFC_FIELD(RFFC5071_P1LOI, 0x0),

	/* 12 FN_CTRL = 0x2A80 */
	RFFC_FIELD(RFFC5071_TZPS, 0),
	RFFC_FIELD(RFFC5071_DMODE, 0),
	RFFC_FIELD(RFFC5071_FM, 0),
	RFFC_FIELD(RFFC5071_DITH, 0),
	RFFC_FIELD(RFFC5071_DSM_MODE, 0),
	RFFC_FIELD(RFFC5071_PHSALNDLY, 0x2),
	RFFC_FIELD(RFFC5071_PHSALNGAIN, 0x2),
	RFFC_FIELD(RFFC5071_PHALN, 1),
	RFFC_FIELD(RFFC5071_SDM, 0x2),
	RFFC_FIELD(RFFC5071_DITHR, 0),
	RFFC_FIELD(RFFC5071_FNZ, 0),

	/* 13 EXT_MOD = 0x0000 */
	RFFC_FIELD(RFFC5071_MODSTEP, 0x0),
	RFFC_FIELD(RFFC5071_MODSETUP, 0x0),

	/* 14 FMOD = 0x0000 */
	RFFC_FIELD(RFFC5071_MODULATION, 0x0),

	/* 15 SDI_CTRL = 0x0000 */
	RFFC_FIELD(RFFC5071_RESET, 0),
	RFFC_FIELD(RFFC5071_ADDR, 0),
	RFFC_FIELD(RFFC5071_4WIRE, 0),
	RFFC_FIELD(RFFC5071_MODE, 0),
	RFFC_FIELD(RFFC5071_ENBL, 0),
	RFFC_FIELD(RFFC5071_SIPIN, 0),

	/* 16 GPO = 0x0000 */
	RFFC_FIELD(RFFC5071_LOCK, 0),
	RFFC_FIELD(RFFC5071_GATE, 0),
	RFFC_FIELD(RFFC5071_P1GPO, 0x0),
	RFFC_FIELD(RFFC5071_P2GPO, 0x0),

	/* 17 T_VCO = 0x4900 */
	RFFC_FIELD(RFFC5071_CURVE_VCO3, 0x2),
	RFFC_FIELD(RFFC5071_CURVE_VCO2, 0x2),
	RFFC_FIELD(RFFC5071_CURVE_VCO1, 0x2),

	/* 18 IQMOD1 = 0x0281 */
	RFFC_FIELD(RFFC5071_BUFDC, 0x1),
	RFFC_FIELD(RFFC5071_DIVBIAS, 0),
	RFFC_FIELD(RFFC5071_CALBLK, 0),
	RFFC_FIELD(RFFC5071_CALNUL, 0),
	RFFC_FIELD(RFFC5071_CALON, 0),
	RFFC_FIELD(RFFC5071_LOBIAS, 0x2),
	RFFC_FIELD(RFFC5071_MODBIAS, 0x2),
	RFFC_FIELD(RFFC5071_CTRL, 0x0),

	/* 19 IQMOD2 = 0xF00F */
	RFFC_FIELD(RFFC5071_MODBUF, 0x3),
	RFFC_FIELD(RFFC5071_MOD, 0x3),
	RFFC_FIELD(RFFC5071_CALATTEN, 0x0),
	RFFC_FIELD(RFFC5071_RCTUNE, 0x0),
	RFFC_FIELD(RFFC5071_BBATTEN, 0xf),

	/* 1A IQMOD3 = 0x0000 */
	RFFC_FIELD(RFFC5071_DACEN, 0),
	RFFC_FIELD(RFFC5071_BUFDACQ, 0x0),
	RFFC_FIELD(RFFC5071_BUFDACI, 0x0),

	/* 1B IQMOD4 = 0x0005 */
	RFFC_FIELD(RFFC5071_IQMOD4_B10, 0x1),
	RFFC_FIELD(RFFC5071_BUFBIAS2, 0x1),
	RFFC_FIELD(RFFC5071_BUFBIAS1, 0x0),
	RFFC_FIELD(RFFC5071_MODDACQ, 0x0),
	RFFC_FIELD(RFFC5071_MODDACI, 0x0),

	/* 1C T_CTRL = 0xC840 */
	RFFC_FIELD(RFFC5071_V_TEST, 0),
	RFFC_FIELD(RFFC5071_LDO_BY, 1),
	RFFC_FIELD(RFFC5071_EXT_FILT, 0),
	RFFC_FIELD(RFFC5071_REF_SEL, 0),
	RFFC_FIELD(RFFC5071_FILT_CTRL, 0x0),
	RFFC_FIELD(RFFC5071_FC_EN, 1),
	RFFC_FIELD(RFFC5071_TBL_SEL, 0x0),
	RFFC_FIELD(RFFC5071_TC_EN, 0x3),

	/* 1D DEV_CTRL = 0x1000 */
	RFFC_FIELD(RFFC5071_BYPAS, 0),
	RFFC_FIELD(RFFC5071_CTCLK, 0),
	RFFC_FIELD(RFFC5071_DAC, 0),
	RFFC_FIELD(RFFC5071_CPD, 0),
	RFFC_FIELD(RFFC5071_CPU, 0),
	RFFC_FIELD(RFFC5071_RSMSTOPST, 0x0),
	RFFC_FIELD(RFFC5071_RSMST, 0),
	RFFC_FIELD(RFFC5071_READSEL, 0x1),

	/* 1E TEST = 0x0005 */
	RFFC_FIELD(RFFC5071_LFSRD, 1),
	RFFC_FIELD(RFFC5071_RCBYP, 0),
	RFFC_FIELD(RFFC5071_RGBYP, 1),
	RFFC_FIELD(RFFC5071_LFSRT, 0),
	RFFC_FIELD(RFFC5071_LFSRGATET, 0x0),
	RFFC_FIELD(RFFC5071_LFSRP, 0),
	RFFC_FIELD(RFFC5071_LFSR, 0),
	RFFC_FIELD(RFFC5071_TSEL, 0x0),
	RFFC_FIELD(RFFC5071_TMUX, 0x0),
	RFFC_FIELD(RFFC5071_TEN, 0),
};

static void rffc5071_set_defaults(rffc5071_driver_t *const drv) {
	memset(drv->regs, 0, sizeof(drv->regs));
	for (size_t i = 0; i < sizeof(rffc5071_defaults) / sizeof(rffc5071_defaults[0]); i++) {
		const rffc5071_field_t *const f = &rffc5071_defaults[i];
		rffc5071_field_set(drv, f->r, f->o, f->l, f->v);
	}
}

typedef struct {
	uint8_t p1mixidd, p2mixidd;       /* MIX_CONT mixer currents */
	uint16_t p1n, p2n;                /* integer N */
	uint32_t p1frac, p2frac;          /* 24-bit fractional N (NMSB:NLSB) */
	uint8_t p1lodiv, p1presc, p1vcosel;
	uint8_t p2lodiv, p2presc, p2vcosel;
} rffc5071_band_cfg_t;

/* Per-band synthesizer settings. LO frequencies assume fREF = 25 MHz, R = 1:
 *   fVCO = (n + frac/2^24) * presc * fREF   (presc: 1 = /2, 2 = /4)
 *   fLO  = fVCO / 2^lodiv
 * Programming guide: fVCO > 3.2 GHz requires presc /4 (all entries comply). */
static const rffc5071_band_cfg_t rffc5071_band_cfg[] = {
	[BAND_29M] = { /* LO1(RX) 710.5000 MHz, LO2(TX) 2371.0000 MHz */
		.p1mixidd = 4, .p2mixidd = 1,
		.p1n = 56, .p1frac = 0xD70A3D, .p1lodiv = 2, .p1presc = 1, .p1vcosel = 0,
		.p2n = 47, .p2frac = 0x6B851E, .p2lodiv = 1, .p2presc = 2, .p2vcosel = 0,
	},
	[BAND_50M] = { /* LO1(RX) 689.5000 MHz, LO2(TX) 2350.0000 MHz */
		.p1mixidd = 4, .p2mixidd = 7,
		.p1n = 55, .p1frac = 0x28F5C2, .p1lodiv = 2, .p1presc = 1, .p1vcosel = 0,
		.p2n = 47, .p2frac = 0x000000, .p2lodiv = 1, .p2presc = 2, .p2vcosel = 0,
	},
	[BAND_51M] = { /* LO1(RX) 688.5000 MHz, LO2(TX) 2349.0000 MHz */
		.p1mixidd = 4, .p2mixidd = 7,
		.p1n = 55, .p1frac = 0x147AE1, .p1lodiv = 2, .p1presc = 1, .p1vcosel = 0,
		.p2n = 46, .p2frac = 0xFAE147, .p2lodiv = 1, .p2presc = 2, .p2vcosel = 0,
	},
	[BAND_70M] = { /* LO1(RX) 669.5000 MHz, LO2(TX) 2330.0000 MHz */
		.p1mixidd = 4, .p2mixidd = 7,
		.p1n = 53, .p1frac = 0x8F5C28, .p1lodiv = 3, .p1presc = 2, .p1vcosel = 0,
		.p2n = 46, .p2frac = 0x999999, .p2lodiv = 1, .p2presc = 2, .p2vcosel = 0,
	},
	[BAND_144M] = { /* LO1(RX) 595.5000 MHz, LO2(TX) 2256.0000 MHz */
		.p1mixidd = 4, .p2mixidd = 7,
		.p1n = 47, .p1frac = 0xA3D70A, .p1lodiv = 3, .p1presc = 2, .p1vcosel = 0,
		.p2n = 45, .p2frac = 0x1EB851, .p2lodiv = 1, .p2presc = 2, .p2vcosel = 0,
	},
	[BAND_145M] = { /* LO1(RX) 594.5000 MHz, LO2(TX) 2255.0000 MHz */
		.p1mixidd = 4, .p2mixidd = 7,
		.p1n = 47, .p1frac = 0x8F5C28, .p1lodiv = 3, .p1presc = 2, .p1vcosel = 0,
		.p2n = 45, .p2frac = 0x199999, .p2lodiv = 1, .p2presc = 2, .p2vcosel = 0,
	},
	[BAND_432M] = { /* LO1(RX) 307.5000 MHz, LO2(TX) 1968.0000 MHz */
		.p1mixidd = 4, .p2mixidd = 7,
		.p1n = 49, .p1frac = 0x333333, .p1lodiv = 4, .p1presc = 2, .p1vcosel = 0,
		.p2n = 39, .p2frac = 0x5C28F5, .p2lodiv = 1, .p2presc = 2, .p2vcosel = 0,
	},
	[BAND_1296M] = { /* LO1(RX) 556.5000 MHz, LO2(TX) 1104.0000 MHz */
		.p1mixidd = 4, .p2mixidd = 7,
		.p1n = 44, .p1frac = 0x851EB8, .p1lodiv = 3, .p1presc = 2, .p1vcosel = 0,
		.p2n = 44, .p2frac = 0x28F5C2, .p2lodiv = 2, .p2presc = 2, .p2vcosel = 0,
	},
};

static void rffc5071_set_band(rffc5071_driver_t *const drv, BAND_IF band) {
	if ((unsigned)band >= sizeof(rffc5071_band_cfg) / sizeof(rffc5071_band_cfg[0])) {
		band = BAND_29M;
	}
	const rffc5071_band_cfg_t *const c = &rffc5071_band_cfg[band];

	/* 0B MIX_CONT (FULLD is set in rffc5071_setup) */
	RFFC_SET(drv, RFFC5071_P1MIXIDD, c->p1mixidd);
	RFFC_SET(drv, RFFC5071_P2MIXIDD, c->p2mixidd);

	/* 0C..0E P1_FREQ1..3 */
	RFFC_SET(drv, RFFC5071_P1N, c->p1n);
	RFFC_SET(drv, RFFC5071_P1LODIV, c->p1lodiv);
	RFFC_SET(drv, RFFC5071_P1PRESC, c->p1presc);
	RFFC_SET(drv, RFFC5071_P1VCOSEL, c->p1vcosel);
	RFFC_SET(drv, RFFC5071_P1NMSB, (uint16_t)(c->p1frac >> 8));
	RFFC_SET(drv, RFFC5071_P1NLSB, (uint16_t)(c->p1frac & 0xff));

	/* 0F..11 P2_FREQ1..3 */
	RFFC_SET(drv, RFFC5071_P2N, c->p2n);
	RFFC_SET(drv, RFFC5071_P2LODIV, c->p2lodiv);
	RFFC_SET(drv, RFFC5071_P2PRESC, c->p2presc);
	RFFC_SET(drv, RFFC5071_P2VCOSEL, c->p2vcosel);
	RFFC_SET(drv, RFFC5071_P2NMSB, (uint16_t)(c->p2frac >> 8));
	RFFC_SET(drv, RFFC5071_P2NLSB, (uint16_t)(c->p2frac & 0xff));
}

/* Set up all registers: common defaults + band-specific settings. */
void rffc5071_init(rffc5071_driver_t *const drv, BAND_IF band) {
	rffc5071_set_defaults(drv);
	rffc5071_set_band(drv, band);

	drv->regs_dirty = 0x7fffffff;

	/* Write default register values to chip. */
	rffc5071_regs_commit(drv);
}


static inline void rffc5071_reg_commit(rffc5071_driver_t *const drv, uint8_t r);
static uint16_t rffc5071_spi_read(rffc5071_driver_t *const drv, uint8_t r);

/*
 * Set up pins for GPIO and SPI control, configure SSP peripheral for SPI, and
 * set our own default register configuration.
 */
void rffc5071_setup(rffc5071_driver_t *const drv, BAND_IF band) {
	rffc5071_init(drv, band);

	/* lock detect flag to gpo4 */
	RFFC_SET(drv, RFFC5071_LOCK, 1);

	/* set ENBL and MODE to be configured via 3-wire interface,
	 * not control pins. */
	RFFC_SET(drv, RFFC5071_SIPIN, 1);

	/* GPOs are active at all times */
	RFFC_SET(drv, RFFC5071_GATE, 1);

	RFFC_SET(drv, RFFC5071_FULLD, 0);

	/* readback reg (0x1F) mux: READSEL=0001 -> bit 15 = PLL lock detect */
	RFFC_SET(drv, RFFC5071_READSEL, 0x1);

	rffc5071_regs_commit(drv);
}

static uint16_t rffc5071_spi_read(rffc5071_driver_t *const drv, uint8_t r) {
	uint16_t data[] = {0x80 | (r & 0x7f), 0xffff};
	spi_bus_transfer(drv->bus, data, 2);
	return data[1];
}

/* Trigger VCO auto-select / coarse-tune recalibration and PLL relock
 * without reprogramming (PLL_CTRL:relok, self-clearing). Keeps current
 * MODE (RX/TX) and frequency. */
void rffc5071_relock(rffc5071_driver_t *const drv) {
	RFFC_SET(drv, RFFC5071_RELOK, 1);
	rffc5071_regs_commit(drv);
	/* chip clears the bit itself; mirror that in the cache, no write */
	RFFC_SET(drv, RFFC5071_RELOK, 0);
	RFFC5071_REG_SET_CLEAN(drv, 9);
}

/* Uncached read straight from the chip (debug / bit-alignment check). */
uint16_t rffc5071_reg_read_raw(rffc5071_driver_t *const drv, uint8_t r) {
	return rffc5071_spi_read(drv, r);
}

void rffc5071_spi_write(rffc5071_driver_t *const drv, uint8_t r, uint16_t v) {
	uint16_t data[] = {0x00 | (r & 0x7f), v};
	spi_bus_transfer(drv->bus, data, 2);
}

uint16_t rffc5071_reg_read(rffc5071_driver_t *const drv, uint8_t r) {
	/* Readback register is not cached. */
	if (r == RFFC5071_READBACK_REG) {
		return rffc5071_spi_read(drv, r);
	}

	/* Discard uncommited write when reading. This shouldn't
	 * happen, and probably has not been tested. */
	if ((drv->regs_dirty >> r) & 0x1) {
		drv->regs[r] = rffc5071_spi_read(drv, r);
	};
	return drv->regs[r];
}

bool rffc5071_lock_detect(rffc5071_driver_t *const drv) {
	/* assumes READSEL=0001 (set in rffc5071_setup) */
	const uint16_t rb = rffc5071_reg_read(drv, RFFC5071_READBACK_REG);
	return (rb >> 15) & 0x1;
}

void rffc5071_reg_write(rffc5071_driver_t *const drv, uint8_t r, uint16_t v) {
	drv->regs[r] = v;
	rffc5071_spi_write(drv, r, v);
	RFFC5071_REG_SET_CLEAN(drv, r);
}

static inline void rffc5071_reg_commit(rffc5071_driver_t *const drv, uint8_t r) {
	rffc5071_reg_write(drv, r, drv->regs[r]);
}

void rffc5071_regs_commit(rffc5071_driver_t *const drv) {
	int r;
	for (r = 0; r < RFFC5071_NUM_REGS; r++) {
		if ((drv->regs_dirty >> r) & 0x1) {
			rffc5071_reg_commit(drv, r);
		}
	}
}

/* Programming guide: when fVCO > 3.2 GHz (prescaler /4) the charge-pump
 * leakage LF:pllcpl must be 3 for CT calibration to work correctly,
 * otherwise 2. LF is shared by both paths, so set it for the active one. */
static void rffc5071_set_pllcpl(rffc5071_driver_t *const drv, uint16_t presc) {
	RFFC_SET(drv, RFFC5071_PLLCPL, (presc == 2) ? 3 : 2);
}

void rffc5071_tx(rffc5071_driver_t *const drv) {
	RFFC_SET(drv, RFFC5071_ENBL, 0);
	RFFC_SET(drv, RFFC5071_MODE, 1);
	rffc5071_set_pllcpl(drv, get_RFFC5071_P2PRESC(drv));
	rffc5071_regs_commit(drv);

	RFFC_SET(drv, RFFC5071_ENBL, 1);
	rffc5071_regs_commit(drv);
}

void rffc5071_rx(rffc5071_driver_t *const drv) {
	RFFC_SET(drv, RFFC5071_ENBL, 0);
	RFFC_SET(drv, RFFC5071_MODE, 0);
	rffc5071_set_pllcpl(drv, get_RFFC5071_P1PRESC(drv));
	rffc5071_regs_commit(drv);

	RFFC_SET(drv, RFFC5071_ENBL, 1);
	rffc5071_regs_commit(drv);
}
