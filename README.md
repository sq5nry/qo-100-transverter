# qo-100-transverter

[![Hardware: CC BY-NC-SA 4.0](https://img.shields.io/badge/Hardware-CC%20BY--NC--SA%204.0-lightgrey.svg)](LICENSE-HARDWARE)
[![Firmware: GPL v2](https://img.shields.io/badge/Firmware-GPL%20v2%20%2B%20commercial%20exception-blue.svg)](LICENSE-CODE)
[![GitHub stars](https://img.shields.io/github/stars/sq5nry/qo-100-transverter?style=social)](https://github.com/sq5nry/qo-100-transverter/stargazers)

## What it's about
- source-available, hardware and software project of a SAT transverter. The outdoor version takes 1.5 days to assemble starting from bare PCB, bag of parts, and patience.
- complete up and down-converter below 199,99 € : [see the BOM](hw-trv/BOM.md)
- in-fab assembly automation in progress (*v0.3-snapshot*)

## Equipment and skills required
- knack for SMD soldering with stencil and solder paste
- patience for by-hand component placement
- 10x10 cm heat plate and ability to control the temperature profile
- multimeter
- set of fixed attenuators, a variable one and patch cables
- VNA+SA and a signal generator up to 3GHz minimum
- Pluto-like SDR to receive, measure, and generate signals during tests and adjustments
- RF power meter
- RF personal protective equipment rated to attenuate 2.4 GHz frequencies
- 30V 5A lab power supply with over-current protection
- ST-link programmer
- patience for drilling holes in aluminum enclosure
- patience for tapping 3 mm holes
- set of sockets (N, F, DC+PTT, UFL)

## Features
- QO-100 narrow-band transponder SSB, CW, and Data operation with an HF-VHF transceiver
- ±0.25 ppm reference oscillator stability
- 6W of 2.4GHz output power
- 10m, 6m, 4m, 2m, 70cm bands supported, 5W input power maximum
- 738MHz receive output for signal supervision
- 25MHz reference oscillator for LNB: injected into the LNB feeder or a shared output
- PTT-over-coax supported
- Splash-proof outdoor version

## Block diagram

![block-diagram](media/block_diagram.png)

## Operation

### Prerequisites

- 28V DC power supply offering 1.1A (31W), or
- 13.8V DC at 3.5A and using the Companion Box for voltage boost
- Minimum 60cm dish antenna suggested with 5W ERP according to AMSAT guidelines
- Standard universal LNB (13/18V), 500mA fused
- Standard PTT ground-to-transmit, or
- PTT-over-coax by transceiver or using the Companion Box to mix PTT and RF
- Operation from 13.8V (1.1A) directly is possible with lower output power (2W max). Adequate linearity is  not guaranteed.

### Basic procedures

1. Configure IF (Intermediate Frequency) corresponding to your transceiver setup using the three-position slide switch to select one of eight possible IF base frequencies.
2. IF can be updated on-the-fly, the change is indicated by a flashing green LED.
3. Connect DC power and PTT signal.
4. Switch on RX pre-amp in your transceiver.
5. Check position of the received QO-100 mid beacon, adjust it to fall into the middle of the band using REF ADJ control with a small flat-head screwdriver.
6. Set your transceiver output power to 3–5W and adjust TX ATT control so that your signal is at the mid-beacon level. Lower power with a higher TX ATT setting is advised to minimize heat dissipation inside the transverter.

## Schematics

- [v0.3 schematics (current)](hw-trv/sch_v0.3-sh.pdf)
- [v0.2 schematics](https://github.com/sq5nry/qo-100-transverter/blob/77cc88346982ab3cb6913bec3531f18df363a01a/hw-trv/sch_v0.2.pdf) — release presented at [Ham Radio Friedrichshafen 2026](https://www.hamradio-friedrichshafen.com/trade-show-program/exhibitors/exhibitor?id=107515806) in  [PZK](https://www.pzk.org.pl/) (Polish Amateur Radio Union) tent

## Circuit description

### Transmit path

The transceiver signal (up to 5W / +37dBm) passes through a variable 35–42dB attenuator,
reducing it to approximately −8dBm at the mixer input. A diode protection limiter with a
clipping level of +4dBm guards the mixer. The signal enters a balanced
Gilbert-cell mixer (RFFC2071), whose output is filtered by a 2.4GHz bandpass filter to
suppress LO leakage and the image frequency. A driver stage amplifies the signal to +12dBm,
followed by a 29dB LDMOS power amplifier and a low-pass filter, delivering +38dBm (6W) to
the TX output socket. Interstage 1dB and 3dB fixed attenuation pads improve impedance
matching, distribute gain, and improve amplifier stability.

### Receive path

LNB power is supplied via a bias-T with sufficient RF isolation at 738MHz and also the 25MHz reference signal (and its harmonics up to 125MHz). The reference signal is low-pass filtered to 125MHz before injection. The
received 738MHz IF from the LNB passes through a high-pass filter and two cascaded SAW
filters centred around 738MHz, then reaches an optional signal splitter. The signal can be
routed to the receive mixer or directed to the external AUX socket. From the mixer output
the signal passes through a PIN diode switch to the shared TRX socket. A fixed input
attenuator is always present on the RX path — enable the RX preamplifier in your
transceiver to compensate.

### Control

The STM32L0 microcontroller configures the RFFC2071 PLL for the RX and TX local oscillator
frequencies corresponding to the IF band selected by the DIP switch. On PTT transitions,
the firmware switches between the TX and RX PLL register banks in the RFFC2071. The PTT
signal can originate from a physical PTT line or be detected from the coax (PTT-over-coax).

## Jumper configuration

### Jumper reference

| Jumper | Name | Default | Function |
|--------|------|---------|----------|
| JP1 | AUX SEL | Reference | Selects what the RX/REF (AUX) socket outputs: **Reference** (25MHz) or **RX** (738MHz) |
| JP2 | RX SPLIT | no split | Part of RX split — change together with JP4 |
| JP4 | RX SPLIT | no split | Part of RX split — change together with JP2 |
| JP5 | OVEN | disconnected | Reserved for OCXO option; disabled in v0.2 |
| JP6 | REF ROUTE | AUX path | Routes the 25MHz reference toward the AUX socket (default) or toward the LNB feeder (change when using JP7) |
| JP7 | REF TO LNB | Open | Solder to mix the 25MHz reference signal into the LNB feeder coax |

### Typical use cases

**1. RX/REF (AUX) socket outputs 25MHz reference** *(default — no changes needed)*

Use this to supply 25MHz reference to an external device via the AUX socket.

| JP1 | JP2 | JP4 | JP6 | JP7 |
|-----|-----|-----|-----|-----|
| default (Reference) | default | default | default (AUX path) | open |

---

**2. 25MHz reference injected into LNB feeder cable**

Use this when the LNB is fed its reference via the coax (no separate reference cable needed).

| JP1 | JP2 | JP4 | JP6 | JP7 |
|-----|-----|-----|-----|-----|
| default | default | default | **change (LNB path)** | **solder** |

---

**3. RX signal (738MHz) on AUX socket**

Use this to route the received downconverted signal to an external SDR or receiver via the AUX socket.

| JP1 | JP2 | JP4 | JP6 | JP7 |
|-----|-----|-----|-----|-----|
| **change (RX)** | **change** | **change** | default | open |

---

**4. Reference injected into LNB feeder + RX signal on AUX socket**

Combines use cases 2 and 3: LNB gets its reference via coax, AUX socket carries the RX signal.

| JP1 | JP2 | JP4 | JP6 | JP7 |
|-----|-----|-----|-----|-----|
| **change (RX)** | **change** | **change** | **change (LNB path)** | **solder** |

---

## Gallery

[▶ Video](https://drive.google.com/file/d/1cbVlMBV8zS9o48hG52KBiPKmD1HPmWjP/view?usp=drive_link)

### Finished units

<table>
<tr>
<td><img src="media/enclosures.JPG" alt="various realizations"></td>
<td><img src="media/DSC_1662.JPG" alt="shack version"></td>
</tr>
<tr>
<td><img src="media/DSC_1681.JPG" alt="outdoor deluxe"></td>
<td><img src="media/companion_box.JPG" alt="companion box"></td>
</tr>
<tr>
<td><img src="media/DSC_1663.JPG" alt="front panel — IF switch and DC+PTT connector"></td>
<td></td>
</tr>
</table>

### Enclosure internals

<table>
<tr>
<td><img src="media/DSC_1679.JPG" alt="shack version inside"></td>
<td><img src="media/DSC_1674.JPG" alt="outdoor version inside"></td>
</tr>
<tr>
<td><img src="media/P_20260605_232504.jpg" alt="shack unit open during integration"></td>
<td><img src="media/P_20260608_010719.jpg" alt="shack unit half-open"></td>
</tr>
<tr>
<td><img src="media/P_20260512_150707.jpg" alt="companion box wiring"></td>
<td><img src="media/DSC_1675.JPG" alt="outdoor deluxe open"></td>
</tr>
</table>

### PCB assembly

<table>
<tr>
<td><img src="media/P_20260518_201419.jpg" alt="component delivery"></td>
<td><img src="media/P_20260518_201309.jpg" alt="component kits unpacked"></td>
</tr>
<tr>
<td><img src="media/P_20260519_123407.jpg" alt="SMD component reels and bags"></td>
<td><img src="media/P_20260519_134542.jpg" alt="MAX20457 ICs in ESD packaging"></td>
</tr>
<tr>
<td><img src="media/P_20260519_010348.jpg" alt="solder stencil"></td>
<td><img src="media/P_20260601_175435.jpg" alt="stencil clamped over PCB"></td>
</tr>
<tr>
<td><img src="media/P_20260520_113411.jpg" alt="solder paste preparation"></td>
<td><img src="media/P_20260520_114559.jpg" alt="solder paste applied"></td>
</tr>
<tr>
<td><img src="media/P_20260518_185056.jpg" alt="reflow on heating platform"></td>
<td><img src="media/P_20260601_181620.jpg" alt="reflow in progress"></td>
</tr>
<tr>
<td><img src="media/P_20260609_193324.jpg" alt="hot plate component rework"></td>
<td><img src="media/P_20260519_185443.jpg" alt="first assembled v0.2 board in frame"></td>
</tr>
<tr>
<td><img src="media/P_20260520_123340.jpg" alt="populated PCB panel"></td>
<td><img src="media/P_20260601_181708.jpg" alt="assembled batch"></td>
</tr>
</table>

### PCB details

<table>
<tr>
<td><img src="media/P_20250405_205523.jpg" alt="power supply IC close-up"><br><em>v0.1</em></td>
<td><img src="media/P_20260527_001843_1.jpg" alt="2.4GHz TX section"></td>
</tr>
<tr>
<td><img src="media/P_20260609_180522.jpg" alt="PCB bottom side"></td>
<td><img src="media/P_20260602_144805.jpg" alt="v0.2 board populated"></td>
</tr>
<tr>
<td><img src="media/DSC_1671.JPG" alt="bare populated v0.2 PCB"></td>
<td><img src="media/P_20260522_161048.jpg" alt="IC after rework"></td>
</tr>
</table>

### Development and testing

<table>
<tr>
<td><img src="media/P_20250405_231727.jpg" alt="early prototype debugging"><br><em>v0.1</em></td>
<td><img src="media/P_20250601_133825.jpg" alt="v0.1 bench test"><br><em>v0.1</em></td>
</tr>
<tr>
<td><img src="media/P_20250615_222837.jpg" alt="v0.1 test setup with instruments"><br><em>v0.1</em></td>
<td><img src="media/P_20250731_152340.jpg" alt="PCB in helping hands during assembly"><br><em>v0.1</em></td>
</tr>
<tr>
<td><img src="media/P_20260209_001514.jpg" alt="RF section prototype"></td>
<td><img src="media/P_20260209_001531.jpg" alt="multi-board bench integration"></td>
</tr>
<tr>
<td><img src="media/P_20260301_190344.jpg" alt="submodule PCB probing"></td>
<td><img src="media/P_20260301_214121.jpg" alt="complex integration wiring"></td>
</tr>
</table>

### Measurements

<table>
<tr>
<td><img src="media/P_20260422_134604.jpg" alt="test attenuator controller"></td>
<td><img src="media/P_20260208_160030.jpg" alt="bench integration"></td>
</tr>
</table>

## Measurements - TX & RX paths

### TX mixer-to-driver-out, LDMOS gain

<table>
<tr>
<td><img src="hw-trv/measurements/tx_if_chain_s21.png" alt="TX IF chain S21"></td>
<td><img src="hw-trv/measurements/pa_amp_s21.png" alt="PA S21"></td>
</tr>
</table>

### LO, upper sideband, image, and 2nd harmonic levels

<table>
<tr>
<td><img src="hw-trv/measurements/29M_output_spectrum_LO_leak.png" alt="Output spectrum — LO leakage"></td>
<td><img src="hw-trv/measurements/29M_output_spectrum_IMG.png" alt="Output spectrum — image frequency"></td>
</tr>
<tr>
<td><img src="hw-trv/measurements/29M_output_spectrum_2nd_harmonic.png" alt="Output spectrum — 2nd harmonic"></td>
<td></td>
</tr>
</table>

### TRX port

<table>
<tr>
<td align="center"><img src="hw-trv/measurements/trv_s11_tx.png" alt="TX port, s11"><br><em>TRX port on TX, s11</em></td>
<td align="center"><img src="hw-trv/measurements/trv_s11_rx.png" alt="RX port, s11"><br><em>TRX port on RX, s11</em></td>
</tr>
</table>

### RX filter

![739MHz dual SAW filter S21](hw-trv/measurements/739M_2xSAW_s21.png)

### TCVCXO output spectrum

![TCXO output spectrum](hw-trv/measurements/tcxo_output_spectrum.png)

## License

All source code, hardware design files, and documentation are publicly available — schematics, PCB layout, and firmware can be freely studied, built, and modified for personal and amateur radio use. This project is dual-licensed by component, and **commercial use of any part of it is not permitted**:

| Component | License |
|---|---|
| Firmware (`src/`, excluding bundled vendor drivers) | [GPL v2 + commercial exception](LICENSE-CODE) |
| Hardware design (`hw-trv/`, `hw-companion-box/`), manual, docs/media | [CC BY-NC-SA 4.0](LICENSE-HARDWARE) |

See [LICENSE](LICENSE) for the full overview. Third-party code bundled
under `src/Drivers/` (ST's STM32 HAL drivers and ARM's CMSIS headers)
keeps its own original license terms.

`src/Core/Src/rffc5071.c`, `src/Core/Src/rffc5071_spi.c`, and associated
headers are derived from [HackRF](https://github.com/greatscottgadgets/hackrf)
by [Great Scott Gadgets](https://greatscottgadgets.com/) and Jared Boone,
licensed under [GPL v2](https://www.gnu.org/licenses/old-licenses/gpl-2.0.html).
