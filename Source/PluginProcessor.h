#pragma once

#include <JuceHeader.h>

class GuitarAmpAudioProcessor : public juce::AudioProcessor
{
public:
    GuitarAmpAudioProcessor();
    ~GuitarAmpAudioProcessor() override;

    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;
    void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    const juce::String getName() const override;

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

    // Audio processor value tree state untuk parameter
    juce::AudioProcessorValueTreeState parameters;

    // Parameter pointers
std::atomic<float>* gainParam = nullptr;
std::atomic<float>* driveParam = nullptr;
std::atomic<float>* rangeParam = nullptr;
std::atomic<float>* blendParam = nullptr;
std::atomic<float>* volumeParam = nullptr;
std::atomic<float>* distortionType = nullptr; // nanti ini bisa kita bahas, karena Choice harusnya int


private:
    // Parameters
    float prevGain = 0.5f;
    float prevDrive = 0.5f;
    float prevRange = 0.5f;
    float prevBlend = 0.5f;
    float prevVolume = 0.5f;
    int prevDistType = 0;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GuitarAmpAudioProcessor)
};