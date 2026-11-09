# greenhouse-controller

A deterministic greenhouse control **decision engine**, not a physical actuator driver. Given temperature, soil moisture percentage, previous fan/pump state and configurable thresholds, it returns new actuator commands. Separate on/off thresholds provide hysteresis to avoid rapid toggling. It rejects invalid or nonfinite readings. **Safety:** This software does not control real pumps, heaters or fans, and has no sensor fault recovery, emergency stop, watchdog or hardware interlock. Do not connect it to unattended physical equipment without independent safety controls.

## Run tests

Python 3.10+ with no third-party packages:

```sh
python -m unittest discover -s tests -v
```

## Example

```python
from greenhouse_controller import decide, State
print(decide(31, 20, State()))
```

GitHub Actions is configured to run tests for Python 3.10, 3.12 and 3.13. CI results must be checked separately.

## Arduino / ESP32 C++ implementation

The existing Python decision engine is preserved. A separate C++11 core lives in
`arduino/GreenhouseController/src/GreenhouseController.h` and applies the same
temperature and moisture hysteresis logic. Invalid inputs fail both outputs OFF.
The Arduino library includes an `IndicatorDemo` example. Two potentiometers
simulate sensor values; GPIOs drive **LED indicators only**, never a fan or pump.

Install `arduino/GreenhouseController` as an Arduino library, select an Arduino Uno
or ESP32 board, and open `File > Examples > GreenhouseController > IndicatorDemo`.
On the Uno the two analog inputs are A0/A1 and LEDs use D8/D9. On a classic
ESP32 development board, analog inputs use GPIO34/35 and LEDs GPIO18/19.
Use current-limiting resistors and adjust pin mapping for other boards.

### C++ host tests
```sh
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
```

GitHub Actions builds the C++ core with both GCC and Clang. **C++ host tests do not
verify Arduino/ESP32 firmware compilation or real hardware.** Never connect an
unattended fan or irrigation pump without independent hardware interlocks,
sensor calibration and a watchdog. This is a teaching prototype.
