#include <cmath>
#include <iostream>
#include <limits>
#include "GreenhouseController.h"

// Never use assert() for CI tests: NDEBUG disables assertions in Release builds.
static int failures = 0;
static void check(bool condition, const char* scenario) {
  if (!condition) {
    std::cerr << "FAIL: " << scenario << '\n';
    ++failures;
  }
}

int main() {
  using namespace greenhouse;
  const double nan = std::numeric_limits<double>::quiet_NaN();
  const double inf = std::numeric_limits<double>::infinity();
  Decision start = decide(31, 20);
  check(start.valid && start.state.fan && start.state.pump, "start both outputs");
  Decision hold = decide(28, 40, start.state);
  check(hold.valid && hold.state.fan && hold.state.pump, "hysteresis holds");
  Decision stop = decide(27, 45, hold.state);
  check(stop.valid && !stop.state.fan && !stop.state.pump, "stop at off thresholds");
  check(decide(30, 30).state.fan && decide(30, 30).state.pump, "on thresholds inclusive");
  check(!decide(29.99, 30.01).state.fan && !decide(29.99, 30.01).state.pump,
        "below on thresholds stays off");
  check(!decide(20, -1).valid && !decide(20, 101).valid, "moisture range");
  check(!decide(nan, 50).valid && !decide(inf, 50).valid, "temperature nonfinite");
  check(!decide(20, nan).valid && !decide(20, -inf).valid, "moisture nonfinite");
  const Settings bad[] = {
    Settings(20, 25), Settings(nan, 27), Settings(30, inf),
    Settings(30, 27, -1, 45), Settings(30, 27, 30, 101),
    Settings(30, 27, 50, 50), Settings(30, 27, nan, 45)
  };
  for (unsigned i = 0; i < sizeof(bad)/sizeof(bad[0]); ++i) {
    Decision d = decide(25, 25, State(true, true), bad[i]);
    check(!d.valid && !d.state.fan && !d.state.pump,
          "invalid configuration must fail both outputs OFF");
  }
  const double invalid[][2] = {{nan, 50}, {inf, 50}, {-inf, 50},
                                {25, nan}, {25, inf}, {25, -1}, {25, 101}};
  for (unsigned i = 0; i < sizeof(invalid)/sizeof(invalid[0]); ++i) {
    Decision d = decide(invalid[i][0], invalid[i][1], State(true, true));
    check(!d.valid && !d.state.fan && !d.state.pump,
          "invalid sensor reading must fail both outputs OFF");
  }
  if (failures) std::cerr << failures << " checks failed\n";
  return failures ? 1 : 0;
}
