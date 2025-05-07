#include "PluginProcessor.h"
#include "PluginEditor.h"

GuitarAmpAudioProcessor::GuitarAmpAudioProcessor()
    : AudioProcessor(BusesProperties()
        .withInput("Input", juce::AudioChannelSet::stereo(), true)
        .withOutput("Output", juce::AudioChannelSet::stereo(), true)),
    parameters(*this, nullptr, "PARAMETERS",
        { 
            std::make_unique<juce::AudioParameterFloat>("gain", "Gain", 0.0f, 1.0f, 0.5f),
            std::make_unique<juce::AudioParameterFloat>("drive", "Drive", 0.0f, 1.0f, 0.5f),
            std::make_unique<juce::AudioParameterFloat>("range", "Range", 0.0f, 1.0f, 0.5f),
            std::make_unique<juce::AudioParameterFloat>("blend", "Blend", 0.0f, 1.0f, 0.5f),
            std::make_unique<juce::AudioParameterFloat>("volume", "Volume", 0.0f, 1.0f, 0.5f),
            std::make_unique<juce::AudioParameterChoice>("distType", "Distortion Type", 
                                 juce::StringArray("Soft Clip", "Hard Clip", "Sine Fold", "Bit Crush"), 0)
        })
{
    gainParam = parameters.getRawParameterValue("gain");
    driveParam = parameters.getRawParameterValue("drive");
    rangeParam = parameters.getRawParameterValue("range");
    blendParam = parameters.getRawParameterValue("blend");
    volumeParam = parameters.getRawParameterValue("volume");
    distortionType = parameters.getRawParameterValue("distType"); // distType sekarang atomic<float>*
}

GuitarAmpAudioProcessor::~GuitarAmpAudioProcessor()
{
}

const juce::String GuitarAmpAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool GuitarAmpAudioProcessor::acceptsMidi() const
{
    return false;
}

bool GuitarAmpAudioProcessor::producesMidi() const
{
    return false;
}

bool GuitarAmpAudioProcessor::isMidiEffect() const
{
    return false;
}

double GuitarAmpAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int GuitarAmpAudioProcessor::getNumPrograms()
{
    return 1;
}

int GuitarAmpAudioProcessor::getCurrentProgram()
{
    return 0;
}

void GuitarAmpAudioProcessor::setCurrentProgram(int index)
{
}

const juce::String GuitarAmpAudioProcessor::getProgramName(int index)
{
    return {};
}

void GuitarAmpAudioProcessor::changeProgramName(int index, const juce::String& newName)
{
}

void GuitarAmpAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    // Initialization
}

void GuitarAmpAudioProcessor::releaseResources()
{
    // When playback stops, released resources
}

// Distortion Function
float applyDistortion(float sample, int distType, float drive, float range)
{
    // Scale drive from 1 to 100
    float scaledDrive = 1.0f + drive * 99.0f;
    // Add pre-gain
    sample *= scaledDrive;
    
    // Different distortion algorithms
    switch (distType)
    {
        case 0: // Soft clip (tanh)
            sample = std::tanh(sample * range * 3.0f);
            break;
            
        case 1: // Hard clip
            sample = juce::jlimit(-0.8f, 0.8f, sample * range * 2.0f);
            break;
            
        case 2: // Sine fold (wave shaper)
            sample = std::sin(sample * range * 3.14159f);
            break;
            
        case 3: // Bit Crush
        {
            float bits = 1.0f + (1.0f - range) * 14.0f; // 1 to 15 bits
            float stepping = std::pow(2.0f, bits);
            sample = std::round(sample * stepping) / stepping;
            break;
        }
        default:
            break;
    }
    
    return sample;
}

void GuitarAmpAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    
    // Get current parameter values
    float gain = *gainParam;
    float drive = *driveParam;
    float range = *rangeParam;
    float blend = *blendParam;
    float volume = *volumeParam;
    int distType = static_cast<int>(*distortionType);
    
    // Process audio
    for (int channel = 0; channel < buffer.getNumChannels(); ++channel)
    {
        float* channelData = buffer.getWritePointer(channel);
        
        for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
        {
            // Apply input gain
            float inputSample = channelData[sample] * (1.0f + gain * 9.0f); // Gain from 1x to 10x
            
            // Store clean signal for blending
            float cleanSignal = inputSample;
            
            // Apply distortion
            float distortedSignal = applyDistortion(inputSample, distType, drive, range);
            
            // Blend between clean and distorted
            float outputSample = cleanSignal * (1.0f - blend) + distortedSignal * blend;
            
            // Apply output volume
            channelData[sample] = outputSample * volume;
        }
    }
}

bool GuitarAmpAudioProcessor::hasEditor() const
{
    return true;
}

juce::AudioProcessorEditor* GuitarAmpAudioProcessor::createEditor()
{
    return new GuitarAmpAudioProcessorEditor(*this);
}

void GuitarAmpAudioProcessor::getStateInformation(juce::MemoryBlock& destData)
{
    auto state = parameters.copyState();
    std::unique_ptr<juce::XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);
}

void GuitarAmpAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));
    
    if (xmlState.get() != nullptr)
        if (xmlState->hasTagName(parameters.state.getType()))
            parameters.replaceState(juce::ValueTree::fromXml(*xmlState));
}

// This creates new instances of the plugin
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new GuitarAmpAudioProcessor();
}
