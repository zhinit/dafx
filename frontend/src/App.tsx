import { useState, useRef } from "react";
import "./App.css";

type FilterType = "LP" | "HP" | "BP" | "BR" | "AP";

function App() {
  const [isPlaying, setIsPlaying] = useState<boolean>(false);
  const [freq, setFreq] = useState<number>(350.0);
  const [q, setQ] = useState<number>(0.707);
  const [filterType, setFilterType] = useState<FilterType>("LP");
  const audioContextRef = useRef<AudioContext | null>(null);
  const workletNodeRef = useRef<AudioWorkletNode | null>(null);

  const initAudio = async () => {
    if (audioContextRef.current) return;

    const audioContext = new AudioContext();

    const response = await fetch("/dsp-emscripten-glue.js");
    const emscriptenGlue = await response.text();

    await audioContext.audioWorklet.addModule("dsp-processor.js");

    const audioWorklet = new AudioWorkletNode(audioContext, "dsp-processor", {
      outputChannelCount: [2],
    });
    audioWorklet.connect(audioContext.destination);

    audioWorklet.port.postMessage({ type: "init", emscriptenGlue });
    await new Promise<void>((resolve) => {
      audioWorklet.port.onmessage = (e) => {
        if (e.data.type === "ready") {
          resolve();
        }
      };
    });

    audioContextRef.current = audioContext;
    workletNodeRef.current = audioWorklet;
  };

  const handlePlay = async () => {
    if (!audioContextRef.current) {
      await initAudio();
    }

    if (audioContextRef.current?.state === "suspended") {
      await audioContextRef.current.resume();
    }

    const newIsPlaying = !isPlaying;
    setIsPlaying(newIsPlaying);

    workletNodeRef.current?.port.postMessage({
      type: "setIsPlaying",
      value: newIsPlaying,
    });
  };

  const handleFreqSlider = (e) => {
    const newFreq = e.target?.value;
    setFreq(newFreq);
    workletNodeRef.current?.port.postMessage({
      type: "setFreq",
      value: newFreq,
    });
  };

  return (
    <div className="container">
      <button className="test-button" onPointerDown={handlePlay}>
        play/pause
      </button>
      <input
        type="range"
        className="filter-slider"
        min="0"
        max="10000"
        onInput={handleFreqSlider}
      />
      <label htmlFor="filter-slider">frequency</label>
      <input type="range" className="filter-slider" min="0" max="10000" />
      <label htmlFor="filter-slider">Q</label>
      <select className="filter-select">
        <option value="LP">Low Pass</option>
        <option value="HP">High Pass</option>
        <option value="BP">Band Pass</option>
        <option value="BR">Band Reject</option>
        <option value="AP">All Pass</option>
      </select>
    </div>
  );
}

export default App;
