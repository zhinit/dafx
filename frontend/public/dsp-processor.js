class DSPProcessor extends AudioWorkletProcessor {
  constructor() {
    super();
    this.engine = null;
    this.module = null;
    this.heapBufferLeft = null;
    this.heapBufferRight = null;
    this.port.onmessage = (e) => this.handleMessage(e.data);
  }

  async handleMessage(data) {
    if (data.type === "init") {
      const fn = new Function(
        data.emscriptenGlue + "; return createDSPModule;",
      );
      const createDSPModule = fn();
      const module = await createDSPModule();
      this.module = module;
      this.engine = new module.Oscillator();

      this.engine.prepare(sampleRate, 2);

      this.heapBufferLeft = this.module._malloc(128 * 4);
      this.heapBufferRight = this.module._malloc(128 * 4);

      this.channelPtrsPtr = this.module._malloc(2 * 4);
      const HEAPU32 = new Uint32Array(this.module.HEAPF32.buffer);
      HEAPU32[this.channelPtrsPtr / 4] = this.heapBufferLeft;
      HEAPU32[this.channelPtrsPtr / 4 + 1] = this.heapBufferRight;

      this.port.postMessage({ type: "ready" });
      return;
    }

    if (!this.engine || !this.module) return;

    switch (data.type) {
      case "setIsPlaying":
        this.engine.resetFilter();
        this.engine.setIsPlaying(data.value);
        break;
      case "setFreq":
        this.engine.setCutoffFreq(data.value);
        break;
      case "setQ":
        this.engine.setQ(data.value);
        break;
      case "setFilterType":
        this.engine.setFilterType(this.module.FilterType[data.value]);
        break;
    }
  }

  process(inputs, outputs) {
    if (!this.engine || !this.module) return true;

    const leftOutput = outputs[0][0];
    const rightOutput = outputs[0][1];
    const numSamples = leftOutput.length;

    this.engine.process(this.channelPtrsPtr, 128);

    const wasmLeft = new Float32Array(
      this.module.HEAPF32.buffer,
      this.heapBufferLeft,
      numSamples,
    );
    const wasmRight = new Float32Array(
      this.module.HEAPF32.buffer,
      this.heapBufferRight,
      numSamples,
    );

    leftOutput.set(wasmLeft);
    rightOutput.set(wasmRight);

    return true;
  }
}

registerProcessor("dsp-processor", DSPProcessor);
