#pragma once

// CMake builds don't use an AppConfig.h, so it's safe to include juce module headers
// directly. If you need to remain compatible with Projucer-generated builds, and
// have called `juce_generate_juce_header(<thisTarget>)` in your CMakeLists.txt,
// you could `#include <JuceHeader.h>` here instead, to make all your module headers visible.
#include <JuceHeader.h>

#include "SynthAudioSource.h"

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent : public    juce::AudioAppComponent,
                                private juce::Timer
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent() override;
    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void releaseResources() override;
    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override;

    inline void updateAngleDelta()
    {
        auto cyclesPerSample = currentFrequency / currentSampleRate;
        angleDelta = cyclesPerSample * 2.0 * juce::MathConstants<double>::pi;
    }

    void setMidiInput (int index)
    {
        auto list = juce::MidiInput::getAvailableDevices();

        deviceManager.removeMidiInputDeviceCallback (list[lastInputIndex].identifier,
            synthAudioSource.getMidiCollector());

        auto newInput = list[index];

        if (!deviceManager.isMidiInputDeviceEnabled (newInput.identifier))
            deviceManager.setMidiInputDeviceEnabled (newInput.identifier, true);

        deviceManager.addMidiInputDeviceCallback (newInput.identifier, synthAudioSource.getMidiCollector());
        midiInputList.setSelectedId (index + 1, juce::dontSendNotification);

        lastInputIndex = index;
    }

private:
    juce::Random random;
    juce::Slider frequencySlider;
    juce::Label frequencyLabel;
    juce::Slider levelSlider;
    juce::Label levelLabel;
    juce::AudioVisualiserComponent scope { 1 };

    juce::MidiKeyboardState keyboardState;
    SynthAudioSource synthAudioSource;
    juce::MidiKeyboardComponent keyboardComponent;

    juce::ComboBox midiInputList;
    juce::Label midiInputListLabel;
    int lastInputIndex = 0;

    double currentSampleRate = 0.0, currentAngle = 0.0, angleDelta = 0.0;
    double currentFrequency = 440.0, targetFrequency = 440.0;
    float currentLevel = 0.1f, targetLevel = 0.1f;


    void timerCallback() override
    {
        keyboardComponent.grabKeyboardFocus();
        stopTimer();
    }

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
