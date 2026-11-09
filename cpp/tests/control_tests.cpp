#include <cassert>
#include <limits>
#include "GreenhouseController.h"
int main() {
  using namespace greenhouse;
  Decision start=decide(31,20);
  assert(start.valid && start.state.fan && start.state.pump);
  Decision hold=decide(28,40,start.state);
  assert(hold.valid && hold.state.fan && hold.state.pump);
  Decision stop=decide(27,45,hold.state);
  assert(stop.valid && !stop.state.fan && !stop.state.pump);
  assert(decide(30,30).state.fan && decide(30,30).state.pump);
  assert(!decide(20,-1).valid);
  assert(!decide(20,101).valid);
  assert(!decide(std::numeric_limits<double>::quiet_NaN(),50).valid);
  assert(!decide(20,50,State(true,true),Settings(20,25)).valid);
  Decision unsafe=decide(20,std::numeric_limits<double>::infinity(),State(true,true));
  assert(!unsafe.valid && !unsafe.state.fan && !unsafe.state.pump);
}
