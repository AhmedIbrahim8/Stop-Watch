# Stop-Watch
# Project Description
- ATmega16 Microcontroller with frequency 1Mhz.
- Timer1 in ATmega16 with CTC mode to count the Stop Watch time.
- Six Common Anode 7-segments are used.
- The six 7-segments in the project are connected using the multiplexed technique.
- Only one 7447 decoder is used for all 7-segments and control the enable/disable for each 7-segement using a NPN BJT transistor connect to one of the MCU pins like the picture below:<br/>
![image](https://user-images.githubusercontent.com/91912492/139963473-ed851557-deb0-4d35-bae3-f0f071419abf.png) <br/>
- We can connect more than one 7-segment display by using the Multiplexing method. In this method, at a time one 7-segment display is driven by the Microcontroller and the rest are OFF. It keeps switching the displays using transistors. Due to the persistence of vision, it appears as a normal display.
- Stop Watch counting should start once the power is connected to the MCU.
- External Interrupt INT0 is configured with falling edge. Connect a push button with the internal pull-up resistor. If a falling edge detected the Stop Watch time should be reset.
- External Interrupt INT1 is configured with raising edge. Connect a push button with the external pull-down resistor. If a raising edge detected the Stop Watch time should be paused.
- External Interrupt INT2 is configured with falling edge. Connect a push button with the internal pull-up resistor. If a falling edge detected the Stop Watch time should be resumed.
