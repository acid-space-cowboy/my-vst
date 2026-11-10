#include "MySynth.h"
#include <cmath>
#include <numbers>

MySynth::MySynth() {
    lastP = 0.0;
    for (int i = 0; i < 10; i++) {
        midiNotes[i] = MidiNote();
    }
}

void MySynth::prepare(double sr, int emfpb) {
    this->sampleRate = sr;
    this->expectedMaxFramesPerBlock = emfpb;
}

// TODO: make this map from note number to MidiNote with an accompanying pool of inactive MidiNotes.
void MySynth::playMidiNote(juce::MidiMessage& msg, int pos) {
    int note = msg.getNoteNumber();
    // If note is off we find the corresponding MidiNote in our data structure and set an end position.
    // The note is set to inactive later.
    if (msg.isNoteOff()) {
        for (int i = 0; i < 10; i++) {
            int midiNoteNum = midiNotes[i].note;
            if (note == midiNoteNum && pos == 0) {
                midiNotes[i].reset();
            } else if (note == midiNoteNum) {
                midiNotes[i].endPos = pos;
                midiNotes[i].deferredInactive = true;
                return;
            }
        }
    } else if (msg.isNoteOn()) {
        for (int i = 0; i < 10; i++) {
            int midiNoteNum = midiNotes[i].note;
            if (note == midiNoteNum) {
                midiNotes[i].note = note;
                midiNotes[i].freq = juce::MidiMessage::getMidiNoteInHertz(note);
                midiNotes[i].startPos = pos;
                midiNotes[i].active = true;
                return;
            }
        }
        for (int i = 0; i < 10; i++) {
            if (!midiNotes[i].active) {
                midiNotes[i].note = note;
                midiNotes[i].freq = juce::MidiMessage::getMidiNoteInHertz(note);
                midiNotes[i].startPos = pos;
                midiNotes[i].active = true;
                return;
            }
        }
    }
}

void MySynth::process(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages) {
    int numFrames = buffer.getNumSamples();
    int numChannels = buffer.getNumChannels();
    float pi = std::numbers::pi_v<float>;

    for (const juce::MidiMessageMetadata metadata : midiMessages) {
        auto msg = metadata.getMessage();
        playMidiNote(msg, metadata.samplePosition);
    }

    for (auto& note : midiNotes) {
        if (!note.active) continue;
        float p;
        for (int c = 0; c < numChannels; c++) {
            p = note.phase;
            int endPos = note.endPos > note.startPos && note.endPos < numFrames ? note.endPos : numFrames;
            for (int f = note.startPos; f < endPos; f++) {
                float freq = note.freq;
                float val = 0.5 * std::sin(p);
                p = p + float(2 * pi * freq / (sampleRate));
                if (p >= 2 * pi) p = p - (2 * pi);
                float prevSample = buffer.getSample(c, f);
                buffer.setSample(c, f, prevSample + val);
            }
        }
        note.phase = p;
        note.startPos = 0.0;
        note.endPos = 0.0;
        if (note.deferredInactive) {
            note.deferredInactive = false;
            note.reset();
        }
    }
}