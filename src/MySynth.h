#pragma once
#include <map>
#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_audio_basics/juce_audio_basics.h>
#include "MidiNote.h"

class MySynth {
public:
    MySynth();
    void prepare(double sampleRate, int expectedMaxFramesPerBlock);
    void process(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages);
private:
    std::array<MidiNote, 10> midiNotes;
    void playMidiNote(juce::MidiMessage& msg, int startPos);
    double sampleRate;
    int expectedMaxFramesPerBlock;
    float lastP;
    float amp = 0.5;
};