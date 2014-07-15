
| ID | Part  | Notes |
| ------------ |---------------| -----|
| C1,C3,C4 | 0.1uf ceramic 1206 ||
| C2 | 22uf tantalum
| LED1 | DC-LED11119 5050 RGB LED | Battery status light |
| LED2 | Red 1206 LED | Temp. warning light|
| LED3 | Green 1206 LED |Power light|
| R1 | 1.2k Ohm 1206 | Ballast for LED2 |
| R2 | 470 Ohm 1206 | Ballast for LED3 |
| R3+ | 1.2k Ohm 1206 | Ballast for LED1, Green |
| R4+ | 470 Ohm 1206 | Ballast for LED1, Red |
| R5+ | 1.2k Ohm 1206 | Ballast for LED1, Blue |
| R6 | 10K Ohm | Vbat divider |
| R7 | 1k Ohm | Vbat divider |
| R8* | 1K Ohm | Pulldown for temp sensor |
| R9 | 10k Ohm | Pulldown for MOSFET |
|JP1| 2 pin KK-100 header | 12v in |
|JP2| 7 pin KK-100 header | controls |
|JP3| JP4: Screw terminals | LED |
|JP4| 3 pin KK-100 header | Temp |
|JP5| 4 pin KK-100 header** | Fan |
|JP6| 2x3 pin 0.1" header | AVR ISP |
| Q1 | AO4438 ||
| U1  | ATINY84A-SU-ND ||
| U2  | TLV1117-50CDCYR | 5v LDO regulator


** Tab on header must be shaved down in order to fit the PWM fan connector, which expects the keying tab to be the size that it is on a standard 3 pin KK-100 connector. 
