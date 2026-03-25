#include "oscillator.h"
#include <emscripten/bind.h>

EMSCRIPTEN_BINDINGS(dsp_module)
{
  emscripten::class_<Oscillator>("Oscillator")
    .constructor()
    .function("prepare", &Oscillator::prepare)
    .function("setIsPlaying", &Oscillator::setIsPlaying)
    .function("process", &Oscillator::process)
    .function("setCutoffFreq", &Oscillator::setCutoffFreq)
    .function("setQ", &Oscillator::setQ);
}
