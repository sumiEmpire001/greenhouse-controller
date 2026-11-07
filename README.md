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
