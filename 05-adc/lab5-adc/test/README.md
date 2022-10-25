# Lab 5: Jakub Drápal

### Analog-to-Digital Conversion

1. Complete table with voltage divider, calculated, and measured ADC values for all five push buttons.
  ![your figure](pictures/left.jpg)
    ![your figure](pictures/select.jpg)
   | **Push button** | **PC0[A0] voltage** | **ADC value (calculated)** | **ADC value (measured)** | **ADC value (measured, hex)** |
   | :-: | :-: | :-: | :-: | :-: |
   | Right  | 0&nbsp;V | 0   | 0 | 0 |
   | Up     | 0.495&nbsp;V | 101 |  |  |
   | Down   | 1.203&nbsp;V | 246 |  |  |
   | Left   | 1.969&nbsp;V | 403 |  |  |
   | Select | 3.182&nbsp;V | 651 |  |  |
   | none   | 5.000&nbsp;V | 1023 |  |  |

### Temperature meter

Consider an application for temperature measurement. Use analog temperature sensor [TC1046](http://ww1.microchip.com/downloads/en/DeviceDoc/21496C.pdf), LCD, and a LED. Every 30 seconds, the temperature is measured and the value is displayed on LCD screen. When the temperature is too high, the LED will turn on.

2. Draw a schematic of temperature meter. The image can be drawn on a computer or by hand. Always name all components and their values.

   ![your figure]()

3. Draw two flowcharts for `main` function and `TIMER1_OVF_vect` interrupt service routine which overflows every 1&nbsp;sec. The image can be drawn on a computer or by hand. Use clear descriptions of the individual steps of the algorithms.

   ![your figure]()
