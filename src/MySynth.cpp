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
void MySynth::processMidiNote(juce::MidiMessage& msg) {
    int note = msg.getNoteNumber();
    // If note is off we find the corresponding MidiNote in our data structure and set it to inactive.
    if (msg.isNoteOff()) {
        for (int i = 0; i < 10; i++) {
            int midiNoteNum = midiNotes[i].note;
            if (note == midiNoteNum && midiNotes[i].active && midiNotes[i].state != NoteState::Release) {
                midiNotes[i].state = NoteState::Release;
                return;
            }
        }
    // If note is on we check for a double count or set an inactive note to active (and configure it)
    } else if (msg.isNoteOn()) {
        for (int i = 0; i < 10; i++) {
            int midiNoteNum = midiNotes[i].note;
            if (note == midiNoteNum) {
                midiNotes[i].setAll(
                    note,
                    juce::MidiMessage::getMidiNoteInHertz(note),
                    midiNotes[i].phase,
                    true,
                    midiNotes[i].amp,
                    NoteState::Attack
                );
                return;
            }
        }
        for (int i = 0; i < 10; i++) {
            if (!midiNotes[i].active) {
                midiNotes[i].setAll(
                    note,
                    juce::MidiMessage::getMidiNoteInHertz(note),
                    0.0,
                    true,
                    0.0,
                    NoteState::Attack
                );
                return;
            }
        }
    }
}

void MySynth::playNotes(juce::AudioBuffer<float>& buffer, int startPos, int endPos) {
    int numChannels = buffer.getNumChannels();
    float pi = std::numbers::pi_v<float>;
    for (auto& note : midiNotes) {
        if (!note.active) continue;
        float p;
        float a;
        // TODO: reorder loops...
        for (int c = 0; c < numChannels; c++) {
            p = note.phase;
            a = note.amp;
            // TODO: separate out envelope into it's own function
            for (int i = startPos; i < endPos; i++) {
                if (note.state == NoteState::Attack) {
                    a += .00001; // TODO: parameterize this
                    if (a >= 0.25) {
                        a = 0.25; // TODO: parameterize this
                        note.state = NoteState::Decay;
                    }
                } else if (note.state == NoteState::Decay) {
                    a -= .00001; // TODO: parameterize this
                    if (a <= 0.125) {
                        a = 0.125; // TODO: parameterize this
                        note.state = NoteState::Sustain;
                    }
                } else if (note.state == NoteState::Sustain) {
                    a = 0.125; // TODO: parameterize this
                } else if (note.state == NoteState::Release) {
                    a -= .00001; // TODO: parameterize this
                    if (a <= 0.0) note.reset();
                }
                float freq = note.freq;
                float val = note.amp * std::sin(p); // This sin function can be replaced with any wave.
                p = p + float(2 * pi * freq / (sampleRate));
                if (p >= 2 * pi) p = p - (2 * pi);
                float prevSample = buffer.getSample(c, i);
                buffer.setSample(c, i, prevSample + val);
            }
        }
        note.phase = p;
        note.amp = a;
        if (note.state == NoteState::Release) {
            if (note.amp <= 0.0) note.reset();
        }
    }
}

void MySynth::process(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages) {
    int numFrames = buffer.getNumSamples();
    float pi = std::numbers::pi_v<float>;

    // Iterate over sorted midi messages and increment between consecutive message positions
    int prevPos = 0;
    for (const juce::MidiMessageMetadata metadata : midiMessages) {
        playNotes(buffer, prevPos, metadata.samplePosition);
        prevPos = metadata.samplePosition;
        auto msg = metadata.getMessage();
        processMidiNote(msg);
    }
    playNotes(buffer, prevPos, numFrames);
}