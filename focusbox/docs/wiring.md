# FocusBox wiring guide

Disconnect USB before changing wires. The GD32 GPIO pins are 3.3 V pins. Do not connect the 5 V rail to PB0, PB1, PA5, or PB10.

## Connections

| From | To | Notes |
|---|---|---|
| GD32 GND | breadboard ground rail | Bridge the rail gap if the rail is split. |
| GD32 PA5 | one side of pushbutton | Other side of button goes to GND; firmware enables internal pull-up. |
| GD32 PB0 | 1 kΩ resistor, then green LED anode | LED cathode goes to GND. |
| GD32 PB1 | 1 kΩ resistor, then blue LED anode | LED cathode goes to GND. |
| GD32 PB10 | 1 kΩ resistor, then transistor base | Base is the transistor's center lead only for the tested part/orientation; verify your specific part. |
| Transistor emitter | GND | NPN low-side switch. Confirm lead identity for your exact transistor. |
| Transistor collector | buzzer negative lead | Confirm lead identity for your exact transistor. |
| Buzzer positive lead | 5 V | Use the board's 5 V/VBUS rail only if it is clearly identified. |
| 1N4007 diode | across buzzer | Cathode (banded end) to buzzer +5 V; anode to buzzer negative/collector. |

The buzzer's current must flow through the transistor: **+5 V → buzzer → transistor collector/emitter → GND**. PB10 drives only the transistor base through the 1 kΩ resistor. Do not power the buzzer from PB10.

## Breadboard reminders

- Five holes in a numbered terminal strip are connected together; rows across the center trench are separate.
- Power rails may be split in the middle. A wire bridge is needed if both halves are used.
- The LED's longer leg is normally the anode (+); its shorter leg/flat side is normally the cathode (−).
- The four legs of a tactile switch are internally paired. Place it across the center trench and use opposite sides of the switch.
- Transistor pinouts differ between manufacturers and variants. The mapping above describes the tested NPN switching arrangement, not a universal physical left-to-right pin order.
