# qo-100-transverter

[![Hardware: CC BY-NC-SA 4.0](https://img.shields.io/badge/Hardware-CC%20BY--NC--SA%204.0-lightgrey.svg)](LICENSE-HARDWARE)
[![Firmware: GPL v2](https://img.shields.io/badge/Firmware-GPL%20v2%20%2B%20commercial%20exception-blue.svg)](LICENSE-CODE)
[![GitHub stars](https://img.shields.io/github/stars/sq5nry/qo-100-transverter?style=social)](https://github.com/sq5nry/qo-100-transverter/stargazers)

*[Wersja polska / Polish version](README_PL.md)*

## What it's about
- source-available, hardware and software project of a transverter for the [QO-100](https://amsat-uk.org/satellites/geo/eshail-2/) satellite. The outdoor version takes 1.5 days to assemble starting from bare PCB, bag of parts, and patience.
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
- 30V 5A lab power supply with over-current protection
- ST-link programmer
- patience for drilling holes in aluminum enclosure
- patience for tapping 3 mm holes
- RF personal protective equipment rated to attenuate 2.4 GHz frequencies

## Features
- QO-100 narrow-band transponder SSB, CW, and Data operation with an HF-VHF transceiver
- ±0.25 ppm reference oscillator stability
- 6W of 2.4GHz output power
- 10m, 6m, 4m, 2m, 70cm bands supported, 5W input power maximum
- 738MHz filtered, receive output for signal supervision
- 25MHz reference oscillator for LNB: injected into the LNB feeder or the shared output
- PTT-over-coax supported
- splash-proof outdoor version

## Block diagram

![block-diagram](media/block_diagram.png)

## Operation

### Prerequisites

- 28V DC power supply offering 1.1A (31W), or
- 13.8V DC at 3.5A and using the Companion Box for voltage boost
- 60cm dish antenna suggested with 5W ERP according to AMSAT guidelines
- standard universal LNB (13/18V), 500mA fused
- standard PTT ground-to-transmit, or
- PTT-over-coax by transceiver or using the Companion Box to mix PTT and RF
- operation from 13.8V (1.1A) directly is possible with lower output power (2W max). Adequate linearity is not guaranteed.

### Basic procedures

1. Configure IF (Intermediate Frequency) corresponding to your transceiver setup using the three-position slide switch to select one of eight possible IF base frequencies.
2. IF can be updated on-the-fly, the change is indicated by a flashing green LED.
3. Connect DC power and PTT signal.
4. Switch on RX pre-amp in your transceiver.
5. Check the position of the received QO-100 mid beacon, adjust it to fall into the middle of the band using REF ADJ control with a small flat-head screwdriver.
6. Set your transceiver output power to 3–5W and adjust TX ATT control so that your signal is at the mid-beacon level. Lower power with a higher TX ATT setting is advised to minimize heat dissipation inside the transverter.

## Schematics

- [v0.3 schematics (current)](hw-trv/sch_v0.3-sh.pdf)
- [v0.2 schematics](https://github.com/sq5nry/qo-100-transverter/blob/77cc88346982ab3cb6913bec3531f18df363a01a/hw-trv/sch_v0.2.pdf) — release presented at [Ham Radio Friedrichshafen 2026](https://www.hamradio-friedrichshafen.com/trade-show-program/exhibitors/exhibitor?id=107515806) in [PZK](https://www.pzk.org.pl/) (Polish Amateur Radio Union) tent

> **⚠️ Note:** do not mount the OCXO subcircuit (error amplifier and heater components, connected via JP5). This circuitry is non-functional in the current release and will be removed in a future revision. The freed board space can instead be used for additional thermal insulation around the TCXO.

## Assembly

### Preparation

Gather all parts from the [BOM](hw-trv/BOM.md) and arrange them on your workbench before starting.
Verify IC orientation markings and keep ESD-sensitive parts (ICs, MOSFETs) in their packaging
until placement.

### Solder paste application

Apply solder paste through the PCB stencil. Two paste options have been tested:

- **Sn63Pb37** (183°C eutectic, e.g. Mechanic XG-50) — recommended; standard SnPb profile,
  widely available, easy to work with.
- **Sn42Bi58** (~138°C eutectic, e.g. ChipQuik SMD291 or Mechanic UV50) — low-melting-point
  alternative; useful if heat-sensitive components are a concern, but requires careful handling
  as bismuth alloys are more brittle and less compatible with lead-containing component finishes.

Clamp the stencil firmly over the PCB, apply paste with a squeegee in a single smooth stroke,
and lift the stencil straight up to avoid smearing. Inspect paste deposits under magnification
before placing components.

### Component placement

Place all SMD components onto the paste deposits. Work from smallest to largest — passives first,
then ICs. Pay close attention to pin-1 orientation on all ICs and polarised components.
QFN packages require careful centring over their pads; the paste surface tension will self-align
them slightly during reflow.

### Reflow

Place the populated PCB on the heat plate. Follow a standard **SnPb reflow profile**:

| Zone | Temperature | Duration |
|------|------------|---------|
| Preheat | 100–150°C | ~60 s |
| Soak | 150–180°C | 60–90 s |
| Reflow (above liquidus) | 183–215°C peak | 30–45 s |
| Cooldown | ramp down | natural |

Use hot air to assist reflow around QFN packages and larger thermal-mass parts (PA, DC-DC
converter). Watch for bridging on fine-pitch ICs.

### Inspection and rework

After reflow and cooldown, inspect all joints under magnification:

- Remove solder bridges with braid or a fine soldering iron tip.
- Add solder to any under-filled joints, particularly on exposed thermal pads of QFN/PQFN
  packages — use a soldering iron with flux through the heat-via holes on the bottom side.
- The bottom-side thermal vias do not need to be flush; they will be covered by thermal pads
  connecting to the heat spreader or enclosure floor.

### Through-hole parts

Solder the 5W power resistor by hand after SMD reflow. Mount the TCVCXO with polyester
tape and foam fitting as described in the enclosure assembly notes.

### EMI shields

The PCB has footprints for SMD metal EMI shields over sensitive sections. Mounting the shield over the IF Unit is important for stability. Shielding the switched-mode power supply section and TCXO is recommended but not critical.

### EMI absorber

Most of enclosure types require absorption foams in PA area to help suppress self-oscillations and minimize parasitic coupling between stages. The amplifier may start self-oscillations at a frequency between 2.2-2.5GHz with full power without the absorber. \
See gallery section for a poor's-man approach.

### PCB cleaning

Wash the assembled PCB thoroughly with isopropanol to remove all flux residues before
proceeding. Flux contamination on the board surface can significantly affect 2.4GHz circuit
behaviour — altering tuning, increasing losses, and making RF adjustments unreliable.
Allow the board to dry completely before applying power.

### Power-up and initial checks

Connect power and PTT cables. Supply 28V with the current limit set to 50mA. Measure the
following test points and verify voltages before proceeding:

1. Check 13V and 5V test points first.
2. If those are correct, verify: 3V (analog), 3V (digital), 3.3V (reference oscillator),
   3V (MCU), 9V, and 1.8V.

If any rail is missing or out of range, do not proceed — investigate before applying full power.

### Firmware

Flash the firmware using **STM32CubeProgrammer** via the ST-LINK interface and the SWD ISP
header. A firmware release binary is provided; to build from source use **STM32CubeIDE**.

After programming, the **PLL LOCK** indicator should illuminate. If it does not, check the
25MHz reference oscillator signal.

### PA bias adjustment

Set the quiescent drain current (Idq) for the 2.4GHz PA stages:

- 1st stage: **15mA**
- 2nd stage: **75mA**

Before adjusting, raise the lab supply's overload protection (current limit) to about
**250mA** so it does not trip during bias trimming.

To measure Idq, cut the respective drain supply PCB trace and insert an ammeter in series or just calculate current delta on the lab supply.
Adjust the bias trimmer for each stage until the target Idq is reached, then bridge the cut
with solder.
Adjust the position of C87 in the PA output circuit for highest stage gain.

### PA gain and output matching

Measure the PA gain and adjust the position of C88 to obtain **28–30dB** gain — slide C88 along the output line toward the thicker section. If that alone does not maximise gain, add 0.8pF in parallel with C88. Do not mount
the low-pass filter (LPF) elements yet — bridge the LPF inductor footprint with a strip of
copper foil instead, so the PA output can be measured without the potentially mismatched filter.

Once the gain is set, mount the LPF inductor and adjust it for a maximum insertion loss of
**1dB at 2.4GHz**.

Set the overload (overcurrent) protection threshold to **1.1A**.

Connect an RF power meter to the 2.4GHz TX output socket and adjust the TX path for **6W**
output across your expected input power range (1–5W).

> **⚠️ Warning:** do not increase input power in an attempt to reach 6W until PA gain is at least 28dB. PA chip will overheat and operate non-linearly. I bricked one PA after overloading its input with just 17dBm.

> **⚠️ Warning:** do not drive the PA past **9..12W** output — exceeding this limit pushes the
> stage into non-linear operation, causing spectral splatter and interference to other users.

> **⚠️ Note:** the PA chip tolerates antenna mismatch well — SWR up to 10:1 on the 2.4GHz
> TX port will not damage it. However, operating with no load connected may cause
> self-oscillation in the 2.2–2.5GHz range; the on-board 1.5A resettable fuse will limit
> the resulting current in that event.

## Circuit description

### Transmit path

The transceiver signal (up to 5W / +37dBm) passes through a variable 35–42dB attenuator,
reducing it to approximately −8dBm at the mixer input. A diode protection limiter with a
clipping level starting softly at +4dBm guards the mixer — this is to limit damage in case you connect 100W for a longer time. Note that sustained high-power overshoots may still damage the PA by exceeding its maximum input level even if the mixer itself survives. \
The signal enters a balanced Gilbert-cell mixer (RFFC2071). The mixer's wideband 4:1 output transformer provides differential symmetry that helps cancel LO leakage, minimising it before the signal
even reaches the downstream filter. Although slightly more expensive than a narrowband balun
chip, the wideband transformer's superior symmetry keeps LO leakage below −32dBc relative
to the upper sideband with only two SAW filters — most critically on the 29MHz band, where
LO leakage lands closest to occupied spectrum. The TX mixer on 10m is intentionally operated at a reduced bias current, which directly reduces LO leakage at its output. \
The output is filtered by a 2.4GHz bandpass filter to further suppress any residual LO leakage and the image frequency. The two TX bandpass filters are inter-stage matched using a parallel 0.8pF capacitor; this matching network optimises power transfer between the filter sections. A driver stage amplifies the signal to +12dBm, followed by a 29dB LDMOS power amplifier — whose input is matched to 50Ω using a short high-impedance transmission line — and a low-pass filter, delivering +38dBm (6W) to
the TX output socket. \
1dB and 3dB fixed attenuation pads improve impedance matching, distribute gain, and improve amplifier stability. Their 50Ω terminations also
provide convenient probe attachment points when measuring gain between individual stages. \
Image attenuation is 40dB on 10m.
Total conversion gain is 2–9dB; about 0.8–4W from the transceiver will produce 6W output.

### Receive path

Both the LNB supply voltage and the 25MHz reference signal (along with its harmonics up to 125MHz) are injected via a bias-T, which must provide sufficient RF isolation across the full 25–738MHz range where these signals share the same junction. The reference signal is low-pass filtered to 125MHz before injection. \
The received 738MHz IF from the LNB passes through a high-pass filter (so as not to hammer the input SAW filters with the reference oscillator) and two cascaded SAW filters centred around 738MHz. Then follows optional splitter to tap IF to the external RX AUX socket. Release v0.2 uses resistive splitter but it's advised to use minimum-loss option as in v0.3-sh. If you don't need the RX AUX output, do not mount the splitter parts.\
From the mixer output the signal passes through a PIN diode switch to the shared TRX socket. The switch's bias-T is built from three inductors in series, chosen to present at least 1kΩ of reactance at
every supported IF band. \
The input attenuator is always present on the RX path — enable the RX preamplifier in your
transceiver to compensate. The receive path does not add amplification (to keep the cost low) and relies on typical LNB signal levels so as not to degrade the SNR with noise from the RX chain. \
On 70cm IF the strongest signals come at S9 level with RX preamp on, mid-beacon is S7.

### Control

The STM32L0 microcontroller configures the RFFC2071 PLL for the RX and TX frequencies corresponding to the IF band selected by the DIP switch. In addition, mixer bias currents ate also configurd per band. On PTT transitions, the logic switches between the TX and RX PLL register banks in the RFFC2071. The PTT signal is a logical OR from the physical PTT line and the coax (PTT-over-coax).

### Indicators

The blue LED indicates that the PLL synthesizer is locked — the MCU has configured the PLL, the 25MHz reference is running, and the LO frequency is being generated. Mixer bias currents are configured according to IF band.

The green LED blinks in RX mode and is lit continuously in TX mode. On startup and on IF band change, it blinks once per IF position number (1–8).

## Jumper configuration

### Jumper reference

| Jumper | Name | Default | Function |
|--------|------|---------|----------|
| JP1 | AUX SEL | Reference | Selects what the RX/REF (AUX) socket outputs: **Reference** (25MHz) or **RX** (738MHz) |
| JP2 | RX SPLIT | no-split | Part of RX split — change together with JP4. Change position only if you want RX in AUX socket. |
| JP4 | RX SPLIT | no-split | Part of RX split — change together with JP2. Change position only if you want RX in AUX socket. |
| JP5 | OVEN | disconnected | Reserved for OCXO option; disabled in v0.2 |
| JP6 | REF ROUTE | AUX path | Routes the 25MHz reference toward the AUX socket (default) or toward the LNB feeder (also check JP7) |
| JP7 | REF TO LNB | open | Solder to mix the 25MHz reference signal into the LNB feeder coax. This jumper enhances signal integrity when the REF signal isn't routed to the LNB feeder. "Open" when you don't want to mix the REF with the LNB feeder, "Connected" to inject the REF into the LNB feeder. |

### Typical usage scenarios

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

[![Video thumbnail](https://drive.google.com/thumbnail?id=1cbVlMBV8zS9o48hG52KBiPKmD1HPmWjP&sz=w640)](https://drive.google.com/file/d/1cbVlMBV8zS9o48hG52KBiPKmD1HPmWjP/view?usp=drive_link)

*Click the image to watch the video.*

[![Output power video thumbnail](https://drive.google.com/thumbnail?id=1jGGn86MhkM-9hL6HgY638JuyBKbzmVd1&sz=w640)](https://drive.google.com/file/d/1jGGn86MhkM-9hL6HgY638JuyBKbzmVd1/view?usp=drive_link)

*Output power — click to watch. 40dB/50W att. + 20dB SMA att (39.5+19,5=59dB at 2.4GHz)*

[![Infrared video thumbnail](https://drive.google.com/thumbnail?id=16R6eq9bAkn6X3CiZJTOIlDRJh-qXqAGV&sz=w640)](https://drive.google.com/file/d/16R6eq9bAkn6X3CiZJTOIlDRJh-qXqAGV/view?usp=drive_link)

*Infrared video of the PCB during PA test — passive cooling via aluminum enclosure. Click to watch.*

[![Power measurements video thumbnail](https://drive.google.com/thumbnail?id=1BobpsWnM3RhQmgtJEgMQ4XA69L-facsb&sz=w640)](https://drive.google.com/file/d/1BobpsWnM3RhQmgtJEgMQ4XA69L-facsb/view?usp=drive_link)

*Power measurements — click to watch. Heat-pipe directly at PA.*

### Finished units

<table>
<tr>
<td><img src="media/enclosures.JPG" alt="various realizations"><br><em>cabinet, outdoor classic, outdoor mini, bare bone</em></td>
<td><img src="media/DSC_1662.JPG" alt="shack version"><br><em>cabinet version</em></td>
</tr>
<tr>
<td><img src="media/DSC_1681.JPG" alt="outdoor small"><br><em>outdoor mini</em></td>
<td><img src="media/companion_box.JPG" alt="companion box"><br><em>Companion Box</em></td>
</tr>
<tr>
<td><img src="media/DSC_1663.JPG" alt="cabinet version, back plate"><br><em>cabinet version, back plate</em></td>
<td><img src="media/P_20260707_175807.jpg" alt="cabinet tiny"><br><em>cabinet mini</em></td>
</tr>
</table>

### Enclosure internals

<table>
<tr>
<td><img src="media/DSC_1679.JPG" alt="cabinet version inside"><br><em>cabinet version</em></td>
<td><img src="media/DSC_1674.JPG" alt="outdoor classic inside"><br><em>outdoor classic</em></td>
</tr>
<tr>
<td><img src="media/P_20260605_232504.jpg" alt="cabinet version during integration"><br><em>cabinet version</em></td>
<td><img src="media/P_20260608_010719.jpg" alt="cabinet version open"><br><em>cabinet version</em></td>
</tr>
<tr>
<td><img src="media/P_20260512_150707.jpg" alt="Companion Box proof-of-concept"><br><em>Companion Box proof-of-concept</em></td>
<td><img src="media/DSC_1675.JPG" alt="outdoor mini open"><br><em>outdoor mini</em></td>
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
<td><img src="media/P_20260527_001843_1.jpg" alt="2.8GHz low-pass filter"><br><em>2.8GHz low-pass filter</em></td>
<td><img src="media/P_20260609_180522.jpg" alt="PCB bottom side"></td>
</tr>
<tr>
<td><img src="media/P_20260602_144805.jpg" alt="v0.2 board populated"></td>
<td><img src="media/DSC_1671.JPG" alt="bare populated v0.2 PCB"></td>
</tr>
<tr>
<td><img src="media/P_20260522_161048.jpg" alt="replacing IF chip"><br><em>replacing IF chip</em></td>
<td><img src="hw-trv/pcb_all_layers.png" alt="PCB all layers"></td>
</tr>
</table>

### EMI absorbers

<table>
<tr>
<td><img src="media/P_20260705_020045.jpg" alt="EMC absorber fitting"></td>
<td><img src="media/P_20260705_020213.jpg" alt="EMC absorber detail"></td>
</tr>
<tr>
<td><img src="media/P_20260705_140738.jpg" alt="EMC absorber installed"></td>
<td><img src="media/P_20260705_203015.jpg" alt="EMC absorber completed"></td>
</tr>
</table>

### Development and testing

<table>
<tr>
<td><img src="media/P_20260209_001514.jpg" alt="RF section prototype"></td>
<td><img src="media/P_20260209_001531.jpg" alt="multi-board bench integration"></td>
</tr>
<tr>
<td><img src="media/P_20260301_190344.jpg" alt="submodule PCB probing"></td>
<td><img src="media/P_20260301_214121.jpg" alt="complex integration wiring"></td>
</tr>
<tr>
<td><img src="media/P_20250405_231727.jpg" alt="early prototype debugging"></td>
<td><img src="media/P_20250601_133825.jpg" alt="v0.1 bench test"></td>
</tr>
<tr>
<td><img src="media/P_20250615_222837.jpg" alt="v0.1 test setup with instruments"></td>
<td><img src="media/P_20250731_152340.jpg" alt="PCB in helping hands during assembly"></td>
</tr>
<tr>
<td><img src="media/P_20250405_205523.jpg" alt="power supply IC close-up"></td>
<td><img src="media/2026-05-30-01-12-42-c3a2c.jpg" alt="C88"><br><em>a cheap C88 (ordinary ceramic instead of ATC porcelain one)</em></td>
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
<td><img src="hw-trv/measurements/tx_if_chain_s21.png" alt="TX IF chain S21" width="480"></td>
<td><img src="hw-trv/measurements/pa_amp_s21.png" alt="PA S21" width="480"></td>
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

> **⚠️ Note:** TX SWR on 23cm is high. Only RX has been tested and confirmed on this band. Proper TX 23cm testing is in progress — it may work with ALC-limited power from your transceiver.

### RF filtering

![two 739MHz filters cascaded incl. balun loss](hw-trv/measurements/739M_2xSAW_s21.png)
*two 739MHz filters cascaded incl. balun loss*

![wideband RX characteristics from F socket to mixer incl. the high-pass filter input](media/rx_if_filters_lnb_socket_to_mix_in_no_shielding_with_HPF.png)
*wideband RX characteristics from F socket to mixer incl. the high-pass filter. No shielding of IF Unit.*

### TCVCXO output spectrum

![TCXO output spectrum](hw-trv/measurements/tcxo_output_spectrum.png)

## Acknowledgements

- **SP5E Krzysztof** — for the original idea and other discussions
- **SQ6QV Tomasz** — for consultation
- **Colleagues from Radio Club in DOK Ursynów** — for countless discussions

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

