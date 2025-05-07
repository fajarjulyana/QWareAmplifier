#include "PluginProcessor.h"
#include "PluginEditor.h"

GuitarAmpAudioProcessorEditor::GuitarAmpAudioProcessorEditor(GuitarAmpAudioProcessor& p)
    : AudioProcessorEditor(&p), audioProcessor(p)
{
    // Set QWare custom look and feel globally (or set per component)
    setLookAndFeel(&qwareLookAndFeel);

    addAndMakeVisible(background);
    
    // Gain slider
    gainSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    gainSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 90, 30);
    gainSlider.setRange(0.0, 1.0, 0.01);
    gainSlider.setName("GAIN");
    gainSlider.setTextValueSuffix("");
    addAndMakeVisible(gainSlider);
    
    // Drive slider
    driveSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    driveSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 90, 30);
    driveSlider.setRange(0.0, 1.0, 0.01);
    driveSlider.setName("DRIVE");
    driveSlider.setTextValueSuffix("");
    addAndMakeVisible(driveSlider);
    
    // Range slider
    rangeSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    rangeSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 90, 30);
    rangeSlider.setRange(0.0, 1.0, 0.01);
    rangeSlider.setName("RANGE");
    rangeSlider.setTextValueSuffix("");
    addAndMakeVisible(rangeSlider);
    
    // Blend slider
    blendSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    blendSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 90, 30);
    blendSlider.setRange(0.0, 1.0, 0.01);
    blendSlider.setName("BLEND");
    blendSlider.setTextValueSuffix("");
    addAndMakeVisible(blendSlider);
    
    // Volume slider
    volumeSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    volumeSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 90, 30);
    volumeSlider.setRange(0.0, 1.0, 0.01);
    volumeSlider.setName("VOLUME");
    volumeSlider.setTextValueSuffix("");
    addAndMakeVisible(volumeSlider);
    
    // Distortion type combo box
    distortionTypeBox.addItem("Soft Clip", 1);
    distortionTypeBox.addItem("Hard Clip", 2);
    distortionTypeBox.addItem("Sine Fold", 3);
    distortionTypeBox.addItem("Bit Crush", 4);
    distortionTypeBox.setJustificationType(juce::Justification::centred);
    distortionTypeLabel.setText("DISTORTION TYPE", juce::dontSendNotification);
    distortionTypeLabel.setJustificationType(juce::Justification::centred);
    distortionTypeLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    addAndMakeVisible(distortionTypeBox);
    addAndMakeVisible(distortionTypeLabel);
    
    // Parameter attachments
    gainAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.parameters, "gain", gainSlider);
    driveAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.parameters, "drive", driveSlider);
    rangeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.parameters, "range", rangeSlider);
    blendAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.parameters, "blend", blendSlider);
    volumeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.parameters, "volume", volumeSlider);
    distortionTypeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(
        audioProcessor.parameters, "distType", distortionTypeBox);
    
    // Set component size
    setSize(500, 400);
}

GuitarAmpAudioProcessorEditor::~GuitarAmpAudioProcessorEditor()
{
    setLookAndFeel(nullptr);
}

void GuitarAmpAudioProcessorEditor::paint(juce::Graphics& g)
{
    // Main painting is done in the AmpBackground component
}

void GuitarAmpAudioProcessorEditor::resized()
{
    // Set background to fill entire component
    background.setBounds(getLocalBounds());
    
    auto area = getLocalBounds().reduced(20);
    area.removeFromTop(50); // Make room for title
    
    const int sliderWidth = 80;
    const int sliderHeight = 100;
    const int comboBoxHeight = 30;
    
    // Layout knobs with spacing
    auto knobArea = area.removeFromTop(sliderHeight + 20);
    int knobSpacing = (knobArea.getWidth() - (5 * sliderWidth)) / 6; // Calculate even spacing
    
    gainSlider.setBounds(knobArea.removeFromLeft(sliderWidth + knobSpacing).withWidth(sliderWidth).withY(knobArea.getY()));
    driveSlider.setBounds(knobArea.removeFromLeft(sliderWidth + knobSpacing).withWidth(sliderWidth).withY(knobArea.getY()));
    rangeSlider.setBounds(knobArea.removeFromLeft(sliderWidth + knobSpacing).withWidth(sliderWidth).withY(knobArea.getY()));
    blendSlider.setBounds(knobArea.removeFromLeft(sliderWidth + knobSpacing).withWidth(sliderWidth).withY(knobArea.getY()));
    volumeSlider.setBounds(knobArea.removeFromLeft(sliderWidth).withY(knobArea.getY()));
    
    // Layout combo box
    auto comboArea = area.removeFromTop(80);
    distortionTypeLabel.setBounds(comboArea.removeFromTop(20));
    distortionTypeBox.setBounds(comboArea.reduced(comboArea.getWidth() * 0.3f, 5));
}