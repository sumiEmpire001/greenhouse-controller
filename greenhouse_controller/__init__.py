"""Pure decision engine for greenhouse ventilation and irrigation."""
from dataclasses import dataclass
import math

@dataclass(frozen=True)
class Settings:
    temperature_on: float = 30.0
    temperature_off: float = 27.0
    moisture_on: float = 30.0
    moisture_off: float = 45.0
    def __post_init__(self):
        values = (self.temperature_on, self.temperature_off, self.moisture_on, self.moisture_off)
        if any(isinstance(v, bool) or not isinstance(v, (int, float)) or not math.isfinite(v) for v in values):
            raise ValueError("thresholds must be finite numbers")
        if self.temperature_off >= self.temperature_on:
            raise ValueError("temperature hysteresis invalid")
        if not 0 <= self.moisture_on < self.moisture_off <= 100:
            raise ValueError("moisture hysteresis invalid")

@dataclass(frozen=True)
class State:
    fan: bool = False
    pump: bool = False

def decide(temperature, moisture, previous=State(), settings=Settings()):
    if not isinstance(previous, State) or not isinstance(settings, Settings):
        raise ValueError("invalid state or settings")
    if any(isinstance(v, bool) or not isinstance(v, (int, float)) or not math.isfinite(v)
           for v in (temperature, moisture)):
        raise ValueError("sensor values must be finite numbers")
    if not 0 <= moisture <= 100:
        raise ValueError("moisture must be percentage 0..100")
    fan = temperature > settings.temperature_off if previous.fan else temperature >= settings.temperature_on
    pump = moisture < settings.moisture_off if previous.pump else moisture <= settings.moisture_on
    return State(fan=fan, pump=pump)
