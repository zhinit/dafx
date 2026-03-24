import { useState, useRef } from "react";
import "./App.css";

function App() {
  const [isPlaying, setIsPlaying] = useState<boolean>(false);
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

  return (
    <div className="container">
      <button className="test-button" onPointerDown={handlePlay}>
        Test
      </button>
    </div>
  );
}

export default App;
