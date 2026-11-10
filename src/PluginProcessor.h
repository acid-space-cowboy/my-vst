#pragma once
#include "MySynth.h"
#include <juce_audio_processors/juce_audio_processors.h>
#include <memory>

class PluginProcessor : public juce::AudioProcessor {
public:
    PluginProcessor();
    void prepareToPlay(double sampleRate, int expectedMaxFramesPerBlock) override;
    void processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages) override;
    void releaseResources() override;
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;
    const juce::String getName() const override;
    bool isBusesLayoutSupported(const BusesLayout& layout) const override;
    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram(int index) override;
    const juce::String getProgramName(int index) override;
    void changeProgramName(int index, const juce::String& newName) override;
    void getStateInformation(juce::MemoryBlock& destData) override;
    void setStateInformation(const void* data, int sizeInBytes) override;
private:
    std::unique_ptr<MySynth> mySynth;
    PluginProcessor(const PluginProcessor& pP) = delete;
    PluginProcessor& operator=(const PluginProcessor& p) = delete;
};