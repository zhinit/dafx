#include "oscillator.h"
#include <emscripten/bind.h>

EMSCRIPTEN_BINDINGS(dsp_module)
{
  emscripten::enum_<FilterType>("FilterType")
    .value("LP", LP)
    .value("HP", HP)
    .value("BP", BP)
    .value("BR", BR)
    .value("AP", AP);
  emscripten::class_<Oscillator>("Oscillator")
    .constructor()
    .function("prepare", &Oscillator::prepare)
    .function("setIsPlaying", &Oscillator::setIsPlaying)
    .function("process", &Oscillator::process)
    .function("setCutoffFreq", &Oscillator::setCutoffFreq)
    .function("setQ", &Oscillator::setQ)
    .function("setFilterType", &Oscillator::setFilterType)
    .function("resetFilter", &Oscillator::resetFilter);
}
