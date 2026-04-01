#include "audio_processor.h"
#include <emscripten/bind.h>

EMSCRIPTEN_BINDINGS(dsp_module)
{
  emscripten::enum_<FilterType>("FilterType")
    .value("LP", LP)
    .value("HP", HP)
    .value("BP", BP)
    .value("BR", BR)
    .value("AP", AP);
  emscripten::class_<AudioProcessor>("AudioProcessor")
    .constructor()
    .function("prepare", &AudioProcessor::prepare)
    .function("setIsPlaying", &AudioProcessor::setIsPlaying)
    .function("process", &AudioProcessor::process)
    .function("setCutoffFreq", &AudioProcessor::setCutoffFreq)
    .function("setQ", &AudioProcessor::setQ)
    .function("setFilterType", &AudioProcessor::setFilterType)
    .function("resetFilter", &AudioProcessor::resetFilter);
}
