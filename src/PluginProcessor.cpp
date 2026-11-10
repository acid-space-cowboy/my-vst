#pragma once
#include "PluginProcessor.h"
#include <ranges>

PluginProcessor::PluginProcessor()
    : AudioProcessor(
        BusesProperties()
            .withInput("Input", juce::AudioChannelSet::stereo(), true)
            .withOutput("Output", juce::AudioChannelSet::stereo(), true)) {
    mySynth = std::make_unique<MySynth>();
}

void PluginProcessor::prepareToPlay(double sampleRate, int expectedMaxFramesPerBlock) {
    mySynth->prepare(sampleRate, expectedMaxFramesPerBlock);
}

void PluginProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages) {
    const int totalNumInputChannels = getTotalNumInputChannels();
    const int totalNumOutputChannels = getTotalNumOutputChannels();
    for (const auto channelToClear : std::views::iota(0, totalNumOutputChannels)) {
        buffer.clear(channelToClear, 0, buffer.getNumSamples());
    }
    mySynth->process(buffer, midiMessages);
}

void PluginProcessor::releaseResources() {
    // mySynth.reset();
}

juce::AudioProcessorEditor* PluginProcessor::createEditor() {
    return nullptr;
}

bool PluginProcessor::hasEditor() const {
    return false;
}

const juce::String PluginProcessor::getName() const {
    return PLUGIN_NAME;
}

bool PluginProcessor::isBusesLayoutSupported(const BusesLayout& layout) const {
    if (layout.getMainOutputChannelSet() != juce::AudioChannelSet::mono() &&
        layout.getMainOutputChannelSet() != juce::AudioChannelSet::stereo()) {
        return false;
    }

    if (layout.getMainOutputChannelSet() != layout.getMainInputChannelSet()) {
        return false;
    }

    return true;
}

bool PluginProcessor::acceptsMidi() const {
    return true;
}

bool PluginProcessor::producesMidi() const {
    return false;
}

bool PluginProcessor::isMidiEffect() const {
    return false;
}

double PluginProcessor::getTailLengthSeconds() const {
    return 0.0;
}

int PluginProcessor::getNumPrograms() {
    return 1;
}

int PluginProcessor::getCurrentProgram() {
    return 0;
}

void PluginProcessor::setCurrentProgram(int index) {
    juce::ignoreUnused(index);
}

const juce::String PluginProcessor::getProgramName(int index) {
    juce::ignoreUnused(index);
    return "None";
}

void PluginProcessor::changeProgramName(int index, const juce::String& newName) {
    juce::ignoreUnused(index, newName);
}


void PluginProcessor::getStateInformation(juce::MemoryBlock& destData) {
    juce::ignoreUnused(destData);
}

void PluginProcessor::setStateInformation(const void* data, int sizeInBytes) {
    juce::ignoreUnused(data, sizeInBytes);
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter() {
    return new PluginProcessor();
}