# qo-100-transverter

[![Hardware: CC BY-NC-SA 4.0](https://img.shields.io/badge/Hardware-CC%20BY--NC--SA%204.0-lightgrey.svg)](LICENSE-HARDWARE)
[![Firmware: GPL v2](https://img.shields.io/badge/Firmware-GPL%20v2%20%2B%20commercial%20exception-blue.svg)](LICENSE-CODE)
[![GitHub stars](https://img.shields.io/github/stars/sq5nry/qo-100-transverter?style=social)](https://github.com/sq5nry/qo-100-transverter/stargazers)

*[English version / Wersja angielska](README.md)*

## O projekcie
- sprzętowo-programowy projekt transvertera do pracy przez satelitę [QO-100](https://amsat-uk.org/satellites/geo/eshail-2/). Czas montażu to ok. 1,5 dnia zaczynając od lutowania elementów aż do strojenia i regulacji
- kompletny up i downconverter w jednym poniżej 199,99€ — [patrz BOM](hw-trv/BOM.md)

## Potrzebne wyposażenie i umiejętności
- umiejętność lutowania SMD z szablonem i pastą lutowniczą
- cierpliwość do ręcznego rozmieszczania wielu elementów
- płyta grzewcza 10×10 cm z termostatem
- multimetr
- zestaw tłumików i kabli RF
- VNA+SA oraz generator sygnałów do ok. 3GHz
- SDR a'la Pluto — do odbioru, pomiaru i generowania sygnałów podczas testów
- miernik mocy RF
- zasilacz laboratoryjny 30V/5A z zabezpieczeniem prądowym
- programator ST-Link
- cierpliwość do wiercenia otworów w obudowie aluminiowej
- cierpliwość do nacinania gwintów M3
- osobiste środki ochrony RF na pasmo 2,4 GHz

## Funkcje
- praca na wąskopasmowym transponederze QO-100 w trybach SSB, CW i DATA z transceiverem HF/VHF
- stabilność oscylatora odniesienia ±0,25 ppm
- moc wyjściowa m.in. 6W na 2,4GHz
- obsługiwane pasma: 10m, 6m, 4m, 2m, 70cm; maksymalna moc wejściowa 5W
- filtrowane wyjście RX 738MHz do monitorowania sygnału
- oscylator 25MHz dla LNB: na osobnym wyjściu lub sumowany do kabla LNB
- obsługa PTT przez kabel koncentryczny
- możliwe wersje obudowy: wodoodporna outdoor oraz mini 

## Schemat blokowy

![schemat-blokowy](media/block_diagram.png)

## Obsługa

### Wymagania

- zasilanie 28V DC 1,1A (31W), lub
- 13,8V DC / 3,5A z użyciem Companion Box do podwyższenia napięcia
- zalecana antena 60cm przy 5W ERP, zgodnie z wytycznymi AMSAT
- standardowy uniwersalny LNB (13/18V), max. 500mA
- standardowe PTT zwarciem do masy, lub
- PTT przez kabel koncentryczny — z transceivera lub przez Companion Box
- praca bezpośrednio z 13,8V (1,1A) możliwa przy niższej mocy wyjściowej (max 2W); liniowość nie jest jednak gwarantowana

### Jak używać

1. Ustaw częstotliwość pośrednią (IF) odpowiadającą Twojemu transceiverowi za pomocą trójpozycyjnego przełącznika suwakowego — jedną z ośmiu możliwych.
2. IF można zmieniać na bieżąco; zmiana jest sygnalizowana miganiem diody INFO.
3. Podłącz zasilanie DC i sygnał PTT.
4. Włącz przedwzmacniacz RX w transceiverze dla pasm 29-70MHz. Tor odbiorczy transwertera nie ma wzmocnienia przemiany i sygnały odbierane muszą być dodatkowo wzmocnione.
5. Sprawdź pozycję środkowego beaconu QO-100; wyreguluj go na środek pasma za pomocą pokrętła REF ADJ małym wkrętakiem płaskim.
6. Ustaw moc wyjściową transceivera na 1–5W i tłumik TX ATT tak, aby sygnał był na poziomie środkowego beaconu. Zalecane jest używanie mniejszej mocy z wyższym ustawieniem TX ATT w celu minimalizacji wydzielania ciepła wewnątrz transvertera.

## Schematy

- [schematy v0.3 (aktualne)](hw-trv/sch_v0.3-sh.pdf)
- [schematy v0.2](https://github.com/sq5nry/qo-100-transverter/blob/77cc88346982ab3cb6913bec3531f18df363a01a/hw-trv/sch_v0.2.pdf) — wersja przedstawiona na [Ham Radio Friedrichshafen 2026](https://www.hamradio-friedrichshafen.com/trade-show-program/exhibitors/exhibitor?id=107515806) w namiocie [PZK](https://www.pzk.org.pl/)

> **⚠️ Uwaga:** nie montuj podzespołów obwodu OCXO (wzmacniacz błędu i elementy grzejnika, podłączone przez zworę JP5). Ten obwód jest niefunkcjonalny w bieżącej wersji i zostanie usunięty w następnej. Zwolnione miejsce na płytce można wykorzystać do dodatkowej izolacji termicznej wokół TCXO.

## Montaż

### Przygotowanie

Zgromadź wszystkie elementy z [BOM](hw-trv/BOM.md) i rozłóż je na stanowisku pracy przed rozpoczęciem.
Sprawdź oznaczenia orientacji układów scalonych i trzymaj elementy wrażliwe na ESD (układy scalone, tranzystory MOSFET) w opakowaniach aż do momentu umieszczania na płytce.

### Nakładanie pasty lutowniczej

Nanieś pastę lutowniczą przez szablon PCB. Przetestowałem dwa rodzaje pasty:

- **Sn63Pb37** (eutektyczna 183°C, np. Mechanic XG-50) — zalecana; standardowy profil SnPb, łatwo dostępna.
- **Sn42Bi58** (~138°C eutektyczna, np. ChipQuik SMD291 lub Mechanic UV50) — alternatywa niskotemperaturowa; przydatna gdy elementy są wrażliwe na ciepło, wymaga jednak ostrożnego postępowania ze względu na kruchość stopów bizmutu i ograniczoną kompatybilność z elementami zawierającymi ołów.

Nałóż szablon stabilnie i równo na płytce, nanieś pastę równym kawałkiem plastiku, a następnie unieś szablon pionowo, aby uniknąć rozmazania. Przed rozmieszczaniem elementów skontroluj nałożoną pastę (czy nie zabrakło pasty na którychś polach).

### Rozmieszczanie elementów

Umieść wszystkie elementy SMD na polach z pastą wg schematu montażowego. Zacznij od mniejszych, najpierw elementy bierne, potem układy scalone. Zwróć szczególną uwagę na orientację układów scalonych i elementów spolaryzowanych.
Obudowy QFN wymagają w miarę starannego wycentrowania na padach; napięcie powierzchniowe pasty samo nieco wyrówna je podczas lutowania.

### Lutowanie przepływowe (reflow)

Umieść zaludnioną płytkę PCB na płycie grzewczej. Zastosuj standardowy **profil lutowania SnPb**:

| Strefa | Temperatura | Czas |
|--------|-------------|------|
| Podgrzewanie | 100–150°C | ~60 s |
| Wyrównywanie temperatury | 150–180°C | 60–90 s |
| Reflow (powyżej liquidus) | szczyt 183–215°C | 30–45 s |
| Chłodzenie | wolne | naturalne |

Użyj gorącego powietrza do wspomagania lutowania wokół obudów QFN i elementów o dużej masie termicznej (PA, przetwornica DC-DC). Obserwuj tworzenie się niepożądanych zworek między wyprowadzeniami układów scalonych.

### Inspekcja

Po lutowaniu i schłodzeniu sprawdź połączenia pod powiększeniem:

- Usuń ewentualne mostki lutownicze plecionką lub cienkim grotem lutownicy.
- Dogrzej spody obudów z padami termicznymi QFN/PQFN — użyj lutownicy z topnikiem przez otwory termiczne od spodu płytki, tak aby wypełnić przelotki spoiwem dla mniejszego oporu cieplenego.
- Przelotki termiczne od spodu nie muszą być wypełnione lutowiem idealnie na płasko; zostaną przykryte elastycznymi podkładkami termicznymi łączącymi z radiatorem lub dnem obudowy.

### Elementy THT

Wlutuj rezystor 5W (THT) ręcznie po zakończeniu lutowania przepływowego SMD. W wersji v0.3 rezystory mocy będzie w wersji SMD, dodatkowo pozostawiony będzie zestaw otworów pod wersje THT.\
Przygotuj TCVCXO zawijając go w piankę termoizolacyjną za pomocą taśmy poliestrowej. Wcześniej wlutuj cztery cienkie przewody o długości 3cm. Tak przygotowane zawiniątko przylutuj na płytce.

### Ekrany EMI

Płytka posiada niepokryte maską pola do montażu metalowych ekranów SMD nad bardziej wrażliwymi sekcjami. Montaż ekranu nad modułem IF jest zalecany dla stabilności i ochrony przed urazami mechanicznymi np. przy regulacji. Ekranowanie sekcji zasilacza impulsowego i TCXO jest zalecane, lecz nie krytyczne.

### Pochłaniacze EMC

Większość typów obudów wymaga zastosowania pianek (mat) pochłaniających w obszarze wzmacniacza mocy, aby tłumić samowzbudzenia i minimalizować pasożytnicze sprzężenia między sekcjami. Bez pochłaniacza wzmacniacz może wzbudzać się samoistnie na częstotliwości w zakresie 2,2–2,5GHz wytwarzając nawet 13W mocy.
Dobrze sprawdziły mi się pochłaniacze z węglowo-plastikowych pianek do przechowywania elementów wrażliwych na ładunki elektrostatyczne. Powierzchnie należy naciąć w kształt liter V dla skuteczniejszego pochłaniania. Miejsca zamocowania mat dla wybranych obudów i sposobu montażu płytki transvertera pokazane jest w sekcji galerii.

### Czyszczenie płytki

Dokładnie przemyj zmontowaną płytkę izopropanolem, aby usunąć wszystkie pozostałości topnika przed dalszymi pracami. Zanieczyszczenie topnikiem na powierzchni płytki może znacząco wpływać na działanie obwodów 2,4GHz — zmieniając strojenie, zwiększając straty i utrudniając regulacje RF.
Przed włączeniem zasilania poczekaj, aż płytka całkowicie wyschnie.

### Pierwsze uruchomienie i kontrola wstępna

Podłącz kable zasilania i PTT. Ustaw napięcie 28V z ograniczeniem prądowym 50mA. Zmierz następujące punkty testowe i zweryfikuj napięcia przed kontynuowaniem:

1. Sprawdź punkty testowe 13V i 5V.
2. Jeśli są prawidłowe, zweryfikuj: 3V (analog), 3V (cyfr.), 3,3V (tcxo), 3V (stm32), 9V (bias) i 1,8V (25M ref).

Jeśli którekolwiek napięcie zasilania jest nieobecne lub poza zakresem, nie kontynuuj — zbadaj przyczynę rozbieżności.

### Firmware

Wgraj firmware przy użyciu **STM32CubeProgrammer** przez interfejs ST-LINK i złącze SWD ISP.
Dostępny jest gotowy plik binarny firmware; do kompilacji ze źródeł użyj **STM32CubeIDE**.

Po zaprogramowaniu wskaźnik **PLL LOCK** powinien się zaświecić. Jeśli nie, sprawdź sygnał oscylatora 25MHz.

### Regulacja prądów polaryzacji PA

Ustaw spoczynkowy prąd drenu (Idq) dla stopni PA 2,4GHz:

- 1. stopień: **15mA**
- 2. stopień: **75mA**

Przed regulacją zwiększ zabezpieczenie prądowe zasilacza laboratoryjnego do około **250mA**, aby nie zadziałało ono podczas tej regulacji.

Aby zmierzyć prąd, przetnij odpowiednią ścieżkę zasilania drenu na płytce i włącz amperomierz szeregowo, lub oblicz różnicę prądu na zasilaczu laboratoryjnym.
Reguluj potencjometrem polaryzacji każdego stopnia do osiągnięcia docelowego Idq, a następnie zalutuj przerwaną ścieżkę.
Dostosuj pozycję C87 w obwodzie wyjściowym PA dla uzyskania maksymalnego wzmocnienia małosygnałowego.

### Strojenie stopnia mocy

Zmierz wzmocnienie PA i dostosuj pozycję C88, aby uzyskać wzmocnienie **28–30dB** — przesuń C88 wzdłuż linii wyjściowej w kierunku grubszej sekcji. Jeśli samo to nie maksymalizuje wzmocnienia, dodaj pojemność 0,8pF równolegle do C88. Nie montuj jeszcze elementów filtra dolnoprzepustowego (LPF) — zmostkuj cewkę pi-filtra paskiem miedzianej folii, aby móc zmierzyć wyjście PA bez potencjalnie źle dopasowanego filtru.

Po ustawieniu wzmocnienia zamontuj cewkę pi-filtra i wyreguluj go dla maksymalnych strat wprowadzenia **1dB przy 2,4GHz**.

Ustaw próg zabezpieczenia nadprądowego na **1,1A**.

Podłącz miernik mocy RF do gniazda TX 2,4GHz i wyreguluj tor TX dla uzyskania **6W** wyjścia w oczekiwanym zakresie mocy wejściowej (1–5W).

> **⚠️ Ostrzeżenie:** nie zwiększaj mocy wejściowej w celu osiągnięcia 6W, dopóki wzmocnienie PA nie wynosi co najmniej 28dB. PA przegrzeje się i będzie pracował nieliniowo.

> **⚠️ Ostrzeżenie:** nie steruj PA powyżej **9..12W** mocy wejściowej — przekroczenie tej granicy powoduje nieliniową pracę stopnia i zakłócenia dla innych użytkowników.

> **⚠️ Uwaga:** układ PA dobrze toleruje niedopasowanie anteny — WFS do 10:1 na porcie TX 2,4GHz nie uszkodzi go. Jednak praca bez podłączonego obciążenia może powodować samowzbudzenia w zakresie 2,2–2,5GHz; montowany na płytce bezpiecznik 1,5A ograniczy wynikający z tego prąd.

## Opis układu

### Tor nadawczy

Sygnał z transceivera (max 5W / +37dBm) tłumiony jest w zakresie 35–42dB, redukując się do około −8dBm na wejściu mieszacza. Ogranicznik diodowy z poziomem obcinania od +4dBm chroni mieszacz — ma za zadanie ograniczyć uszkodzenia w przypadku podłączenia 100W na dłuższy czas. Należy pamiętać, że długotrwałe przeciążenia mogą mimo to uszkodzić PA przez przekroczenie jego maksymalnego poziomu wejściowego, nawet jeśli sam mieszacz to przeżyje.
Sygnał wchodzi do zrównoważonego mieszacza na bazie komórki Gilberta (RFFC2071). Szerokopasmowy transformator wyjściowy 4:1 pomaga minimalizować wyciek LO; jest on poniżej −32dBc co jest szczególnie krytycznie na paśmie 10m. Mieszacz TX na tym paśmie ma ustawiony mniejszy prąd polaryzacji, co bezpośrednio zmniejsza poziom LO na jego wyjściu.
Sygnał jest dalej filtrowany przez filtr pasmowy 2,4GHz, odfiltrowywana jest heterodyna oraz jeszcze bardziej, sygnał lustrzany. Dwa filtry SAW 2441.8MHz dopasowane między sobą zapewniają małe tłumienie pasmowe i zadawalającą stromość zbocza dla tłumienia sygnałów niepożądanych. Driver mocy wzmacnia sygnał do +12dBm i przez tłumik podaje do wzmacniacza mocy LDMOS. Po filtrze dolnoprzepustowym 2.6GHz otrzymujemy +38dBm (6W) na gnieździe wyjściowym N.
Tłumiki międzystopniowe poprawiają dopasowanie impedancji, rozkładają wzmocnienie i zwiększają stabilność całego toru. Tłumienie sygnału lustrzanego wynosi 40dB na paśmie 10m.
Wzmocnienie przemiany to 2–9dB; około 0,8–4W z transceivera da 6W mocy na wyjściu.

### Tor odbiorczy

Sygnał p.cz. 738MHz z LNB przechodzi wpierw przez filtr górnoprzepustowy (aby nie przeciążać filtrów SAW sygnałem oscylatora odniesienia) i dwa kaskadowo połączone filtry SAW 738MHz. Następnie opcjonalny rozdzielacz umożliwia wyprowadzenie sygnału odbieranego do zewnętrznego gniazda RX AUX. Wersja v0.2 używa rozdzielacza rezystancyjnego, w następnej będzie już użyty dzielnik Wilkinsona. Jeśli wyjście RX AUX nie jest potrzebne, nie montuje się elementów rozdzielacza. \
Napięcie zasilania LNB i sygnał odniesienia 25MHz są wstrzykiwane przez bias-T, który zapewnia wystarczającą izolację RF w zakresie 25-125MHz oraz 738MHz, gdzie sygnały te dzielą to samo złącze. Harmoniczne sygnału odniesienia filtrowane są do ok. 125MHz. \
Od wyjścia mieszacza sygnał przechodzi przez przełącznik na diodzie PIN do wspólnego gniazda TRX. Bias-T przełącznika jest zbudowany z trzech indukcyjności tak, aby każda z nich przedstawiała co najmniej 1kΩ reaktancji w każdym obsługiwanym paśmie IF.
Projekt zakłada że tłumik wejściowy zawsze jest obecny na ścieżce RX — należy włączyć przedwzmacniacz RX w transceiverze, aby skompensować straty. Dla obniżenia kosztu, tor odbiorczy nie dodaje wzmocnienia i opiera się na typowych poziomach sygnału LNB.
Na IF 432MHz najsilniejsze sygnały są odbierane na poziomie S9 przy włączonym przedwzmacniaczy RX, środkowy beacon jest na S7.

### Sterowanie

Mikrokontroler STM32L0 konfiguruje rejestry PLL RFFC2071 pod kątem częstotliwości RX i TX odpowiadających wybranemu pasmu. Także konfigurowane są prądy mieszczacza (pasmo 10m). Przy zmianach PTT kontroler przełącza tylko banki rejestrów syntezy.

### Wskaźniki

Niebieska dioda LED (PLL LOCK) informuje o poprawnym stanie pracy syntezera, oznaczającym że mikrokontroler skonfigurował jego rejestry, działa oscylator 25MHz i powinna być generowana częstotliwość heterodyny.

Zielona dioda LED (INFO) miga w trybie RX i świeci ciągłym światłem w trybie TX. Przy starcie i zmianie pasma IF miga wg. numeru pozycji IF (1–8).

## Konfiguracja zworek

### Tabela zworek

| Zworka | Nazwa | Domyślnie | Funkcja |
|--------|-------|-----------|---------|
| JP1 | AUX SEL | Reference | Wybiera, co wyprowadza gniazdo RX/REF (AUX): **Reference** (25MHz) lub **RX** (738MHz) |
| JP2 | RX SPLIT | no-split | Element toru RX split — zmienić razem z JP4. Zmień pozycję tylko jeśli chcesz RX na gnieździe AUX. |
| JP4 | RX SPLIT | no-split | Element toru RX split — zmienić razem z JP2. Zmień pozycję tylko jeśli chcesz RX na gnieździe AUX. |
| JP5 | OVEN | rozłączone | Zarezerwowane dla opcji OCXO; wyłączone w v0.2 |
| JP6 | REF ROUTE | AUX path | Kieruje odniesienie 25MHz do gniazda AUX (domyślnie) lub do kabla LNB (sprawdź też JP7) |
| JP7 | REF TO LNB | open | Przylutuj, aby wmieszać sygnał odniesienia 25MHz do kabla LNB. Otwarte = nie wstrzykuj REF do LNB; Przylutowane = wstrzykuj REF do LNB. |

### Typowe scenariusze

**1. Gniazdo RX/REF (AUX) wyprowadza odniesienie 25MHz** *(domyślnie — brak zmian)*

Użyj, aby dostarczyć odniesienie 25MHz do zewnętrznego urządzenia przez gniazdo AUX.

| JP1 | JP2 | JP4 | JP6 | JP7 |
|-----|-----|-----|-----|-----|
| domyślnie (Reference) | domyślnie | domyślnie | domyślnie (AUX path) | open |

---

**2. Odniesienie 25MHz wstrzyknięte w kabel LNB**

Użyj, gdy LNB otrzymuje odniesienie przez koncentryk.

| JP1 | JP2 | JP4 | JP6 | JP7 |
|-----|-----|-----|-----|-----|
| domyślnie | domyślnie | domyślnie | **zmień (LNB path)** | **przylutuj** |

---

**3. Sygnał RX (738MHz) na gnieździe AUX**

Użyj, aby wyprowadzić odebrany sygnał do zewnętrznego odbiernika SDR przez gniazdo AUX.

| JP1 | JP2 | JP4 | JP6 | JP7 |
|-----|-----|-----|-----|-----|
| **zmień (RX)** | **zmień** | **zmień** | domyślnie | open |

---

**4. Odniesienie wstrzykiwane do LNB + sygnał RX na gnieździe AUX**

Łączy scenariusze 2 i 3: LNB dostaje odniesienie przez koncentryk, gniazdo AUX niesie sygnał RX.

| JP1 | JP2 | JP4 | JP6 | JP7 |
|-----|-----|-----|-----|-----|
| **zmień (RX)** | **zmień** | **zmień** | **zmień (LNB path)** | **przylutuj** |

---

## Galeria

[![Miniatura wideo](https://drive.google.com/thumbnail?id=1cbVlMBV8zS9o48hG52KBiPKmD1HPmWjP&sz=w640)](https://drive.google.com/file/d/1cbVlMBV8zS9o48hG52KBiPKmD1HPmWjP/view?usp=drive_link)

*Kliknij obraz, aby obejrzeć wideo.*

[![Miniatura wideo — moc wyjściowa](https://drive.google.com/thumbnail?id=1jGGn86MhkM-9hL6HgY638JuyBKbzmVd1&sz=w640)](https://drive.google.com/file/d/1jGGn86MhkM-9hL6HgY638JuyBKbzmVd1/view?usp=drive_link)

*Moc wyjściowa — kliknij, aby obejrzeć. Att. 40dB/50W + att. SMA 20dB (39,5+19,5=59dB przy 2,4GHz)*

[![Miniatura wideo — podczerwień](https://drive.google.com/thumbnail?id=16R6eq9bAkn6X3CiZJTOIlDRJh-qXqAGV&sz=w640)](https://drive.google.com/file/d/16R6eq9bAkn6X3CiZJTOIlDRJh-qXqAGV/view?usp=drive_link)

*Wideo w podczerwieni PCB podczas testu PA — chłodzenie pasywne przez obudowę aluminiową. Kliknij, aby obejrzeć.*

[![Miniatura wideo — pomiary mocy](https://drive.google.com/thumbnail?id=1BobpsWnM3RhQmgtJEgMQ4XA69L-facsb&sz=w640)](https://drive.google.com/file/d/1BobpsWnM3RhQmgtJEgMQ4XA69L-facsb/view?usp=drive_link)

*Pomiary mocy — kliknij, aby obejrzeć. Heat-pipe bezpośrednio przy PA.*

### Gotowe urządzenia

<table>
<tr>
<td><img src="media/enclosures.JPG" alt="różne realizacje"><br><em>shack, outdoor classic, outdoor mini, bare bone</em></td>
<td><img src="media/DSC_1662.JPG" alt="wersja szafkowa"><br><em>wersja shack</em></td>
</tr>
<tr>
<td><img src="media/DSC_1681.JPG" alt="zewnętrzny mały"><br><em>outdoor mini</em></td>
<td><img src="media/companion_box.JPG" alt="companion box"><br><em>Companion Box</em></td>
</tr>
<tr>
<td><img src="media/DSC_1663.JPG" alt="wersja szafkowa, tylna płyta"><br><em>wersja shack, tylna ścianka</em></td>
<td><img src="media/P_20260707_175807.jpg" alt="szafkowy mini"><br><em>shack mini</em></td>
</tr>
</table>

### Wnętrze obudów

<table>
<tr>
<td><img src="media/DSC_1679.JPG" alt="wersja szafkowa w środku"><br><em>shack</em></td>
<td><img src="media/DSC_1674.JPG" alt="zewnętrzny klasyczny w środku"><br><em>outdoor classic</em></td>
</tr>
<tr>
<td><img src="media/P_20260605_232504.jpg" alt="wersja szafkowa podczas integracji"><br><em>shack</em></td>
<td><img src="media/P_20260608_010719.jpg" alt="wersja szafkowa otwarta"><br><em>shack</em></td>
</tr>
<tr>
<td><img src="media/P_20260512_150707.jpg" alt="Companion Box proof-of-concept"><br><em>Companion Box PoC</em></td>
<td><img src="media/DSC_1675.JPG" alt="zewnętrzny mini otwarty"><br><em>outdoor mini</em></td>
</tr>
</table>

### Montaż PCB

<table>
<tr>
<td><img src="media/P_20260518_201419.jpg" alt="elementy"></td>
<td><img src="media/P_20260518_201309.jpg" alt="zestawy elementów rozpakowane"></td>
</tr>
<tr>
<td><img src="media/P_20260519_123407.jpg" alt="szpule i torebki SMD"></td>
<td><img src="media/P_20260519_134542.jpg" alt="układy MAX20457 w opakowaniach ESD"></td>
</tr>
<tr>
<td><img src="media/P_20260519_010348.jpg" alt="szablon lutowniczy"></td>
<td><img src="media/P_20260601_175435.jpg" alt="szablon zaciśnięty na PCB"></td>
</tr>
<tr>
<td><img src="media/P_20260520_113411.jpg" alt="przygotowanie pasty lutowniczej"></td>
<td><img src="media/P_20260520_114559.jpg" alt="nałożona pasta lutownicza"></td>
</tr>
<tr>
<td><img src="media/P_20260518_185056.jpg" alt="reflow na płycie grzewczej"></td>
<td><img src="media/P_20260601_181620.jpg" alt="reflow w toku"></td>
</tr>
<tr>
<td><img src="media/P_20260609_193324.jpg" alt="lutowanie elementów na płycie grzewczej"></td>
<td><img src="media/P_20260519_185443.jpg" alt="pierwsza złożona płyta v0.2 w ramce"></td>
</tr>
<tr>
<td><img src="media/P_20260520_123340.jpg" alt="spopulowana PCB"></td>
<td><img src="media/P_20260601_181708.jpg" alt="zmontowana partia"></td>
</tr>
</table>

### Szczegóły PCB

<table>
<tr>
<td><img src="media/P_20260527_001843_1.jpg" alt="filtr dolnoprzepustowy 2,8GHz"><br><em>filtr dolnoprzepustowy 2,8GHz</em></td>
<td><img src="media/P_20260609_180522.jpg" alt="spód PCB"></td>
</tr>
<tr>
<td><img src="media/P_20260602_144805.jpg" alt="spopulowana płyta v0.2"></td>
<td><img src="media/DSC_1671.JPG" alt="goła PCB v0.2"></td>
</tr>
<tr>
<td><img src="media/P_20260522_161048.jpg" alt="wymiana układu IF"><br><em>wymiana układu IF</em></td>
<td><img src="hw-trv/pcb_all_layers.png" alt="PCB wszystkie warstwy"></td>
</tr>
</table>

### Pochłaniacze EMC

<table>
<tr>
<td><img src="media/P_20260705_020045.jpg" alt="montaż pochłaniacza EMC"></td>
<td><img src="media/P_20260705_020213.jpg" alt="szczegół pochłaniacza EMC"></td>
</tr>
<tr>
<td><img src="media/P_20260705_140738.jpg" alt="zainstalowany pochłaniacz EMC"></td>
<td><img src="media/P_20260705_203015.jpg" alt="pochłaniacz EMC gotowy"></td>
</tr>
</table>

### Prace rozwojowe i testy

<table>
<tr>
<td><img src="media/P_20260209_001514.jpg" alt="prototyp sekcji RF"></td>
<td><img src="media/P_20260209_001531.jpg"></td>
</tr>
<tr>
<td><img src="media/P_20260301_190344.jpg"></td>
<td><img src="media/P_20260301_214121.jpg"></td>
</tr>
<tr>
<td><img src="media/P_20250405_231727.jpg" alt="debugowanie wczesnego prototypu"></td>
<td><img src="media/P_20250601_133825.jpg" alt="test v0.1 na stole"></td>
</tr>
<tr>
<td><img src="media/P_20250615_222837.jpg" alt="stanowisko testowe v0.1 z przyrządami"></td>
<td><img src="media/P_20250731_152340.jpg" alt="PCB podczas montażu"></td>
</tr>
<tr>
<td><img src="media/P_20250405_205523.jpg" alt="układ zasilania"></td>
<td><img src="media/2026-05-30-01-12-42-c3a2c.jpg" alt="C88"><br><em>ceramiczny C88 zamiast porcelanowego ATC)</em></td>
</tr>
</table>

### Pomiary

<table>
<tr>
<td><img src="media/P_20260422_134604.jpg" alt="sterownik tłumika testowego"></td>
<td><img src="media/P_20260208_160030.jpg" alt="integracja"></td>
</tr>
</table>

## Pomiary — tory TX i RX

### TX mikser–wyjście sterownika, wzmocnienie LDMOS

<table>
<tr>
<td><img src="hw-trv/measurements/tx_if_chain_s21.png" alt="TX IF chain S21" width="480"></td>
<td><img src="hw-trv/measurements/pa_amp_s21.png" alt="PA S21" width="480"></td>
</tr>
</table>

### Heterodyna, sygnał wyjściowy, lustrzany oraz 2 harmoniczna

<table>
<tr>
<td><img src="hw-trv/measurements/29M_output_spectrum_LO_leak.png" alt="Widmo wyjściowe — wyciek LO"></td>
<td><img src="hw-trv/measurements/29M_output_spectrum_IMG.png" alt="Widmo wyjściowe — częstotliwość lustrzana"></td>
</tr>
<tr>
<td><img src="hw-trv/measurements/29M_output_spectrum_2nd_harmonic.png" alt="Widmo wyjściowe — 2. harmoniczna"></td>
<td></td>
</tr>
</table>

### Port TRX

<table>
<tr>
<td align="center"><img src="hw-trv/measurements/trv_s11_tx.png" alt="port TX, s11"><br><em>port TRX w TX, s11</em></td>
<td align="center"><img src="hw-trv/measurements/trv_s11_rx.png" alt="port RX, s11"><br><em>port TRX w RX, s11</em></td>
</tr>
</table>

> **⚠️ Uwaga:** WFS nadawczy na 23cm jest wysoki. Na tym paśmie przetestowałem jedynie RX. Pełne testy TX na 23cm są w toku — może ono działać z mocą ograniczoną przez ALC transceivera.

### Filtrowanie RF

![dwa filtry 739MHz kaskadowo, uwzględniając straty baluna](hw-trv/measurements/739M_2xSAW_s21.png)
*dwa filtry 739MHz kaskadowo — uwzględniając straty baluna*

![szerokopasmowe charakterystyki RX od gniazda F do wejścia miksera, z filtrem górnoprzepustowym](media/rx_if_filters_lnb_socket_to_mix_in_no_shielding_with_HPF.png)
*szerokopasmowe charakterystyki RX od gniazda F do miksera, z filtrem górnoprzepustowym. Bez ekranowania modułu IF.*

### Widmo wyjściowe TCVCXO

![Widmo wyjściowe TCXO](hw-trv/measurements/tcxo_output_spectrum.png)

## Podziękowania

- **SP5E Krzysztof** — za pomysł i inne dyskusje
- **SQ6QV Tomasz** — za konsultacje
- **Koleżanki i Koledzy z Radioklubu w DOK Ursynów** — za dyskusje projektowe

## Licencja

Cały kod źródłowy, pliki projektu sprzętowego i dokumentacja są publicznie dostępne — schematy, layout PCB i firmware mogą być swobodnie studiowane, montowane i modyfikowane do użytku osobistego i krótkofalarskiego. Projekt jest podwójnie licencjonowany według komponentów, a **użycie komercyjne jakiejkolwiek jego części jest niedozwolone**:

| Komponent | Licencja |
|---|---|
| Firmware (`src/`, z wyłączeniem dołączonych sterowników zewnętrznych) | [GPL v2 + wyjątek komercyjny](LICENSE-CODE) |
| Projekt sprzętu (`hw-trv/`, `hw-companion-box/`), instrukcja, dokumentacja/media | [CC BY-NC-SA 4.0](LICENSE-HARDWARE) |

Pełne informacje znajdziesz w pliku [LICENSE](LICENSE). Zewnętrzny kod dołączony w katalogu `src/Drivers/` (sterowniki STM32 HAL firmy ST i nagłówki CMSIS firmy ARM) zachowuje oryginalne warunki licencyjne.

`src/Core/Src/rffc5071.c`, `src/Core/Src/rffc5071_spi.c` i powiązane pliki nagłówkowe są pochodną projektu [HackRF](https://github.com/greatscottgadgets/hackrf) autorstwa [Great Scott Gadgets](https://greatscottgadgets.com/) i Jareda Boone'a, licencjonowanego na warunkach [GPL v2](https://www.gnu.org/licenses/old-licenses/gpl-2.0.html).
