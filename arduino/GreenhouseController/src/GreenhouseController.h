#ifndef LORD_SUMI_GREENHOUSE_CONTROLLER_H
#define LORD_SUMI_GREENHOUSE_CONTROLLER_H
#include <math.h>
namespace greenhouse {
struct Settings {
    double temperature_on, temperature_off, moisture_on, moisture_off;
    Settings(double ton=30.0,double toff=27.0,double mon=30.0,double moff=45.0)
      : temperature_on(ton),temperature_off(toff),moisture_on(mon),moisture_off(moff) {}
};
struct State {
    bool fan, pump;
    State(bool f=false,bool p=false):fan(f),pump(p){}
};
struct Decision {
    bool valid;
    State state;
    Decision(bool v,State s):valid(v),state(s){}
};
inline bool valid_settings(const Settings& s) {
    return isfinite(s.temperature_on) && isfinite(s.temperature_off) &&
        isfinite(s.moisture_on) && isfinite(s.moisture_off) &&
        s.temperature_off < s.temperature_on &&
        s.moisture_on >= 0.0 && s.moisture_on < s.moisture_off && s.moisture_off <= 100.0;
}
inline Decision decide(double temperature_c,double moisture_percent,
                       const State& previous=State(),const Settings& settings=Settings()) {
    if (!valid_settings(settings) || !isfinite(temperature_c) || !isfinite(moisture_percent) ||
        moisture_percent < 0.0 || moisture_percent > 100.0) {
        return Decision(false,State());
    }
    bool fan = previous.fan ? temperature_c > settings.temperature_off :
                              temperature_c >= settings.temperature_on;
    bool pump = previous.pump ? moisture_percent < settings.moisture_off :
                                moisture_percent <= settings.moisture_on;
    return Decision(true,State(fan,pump));
}
} // namespace greenhouse
#endif
