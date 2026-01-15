#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent() : synthAudioSource  (keyboardState),
          keyboardComponent (keyboardState, juce::MidiKeyboardComponent::horizontalKeyboard)
{
    setSize (800, 600);
    setAudioChannels(0,2);

    frequencySlider.setRange (50.0, 5000.0);
    frequencySlider.setSkewFactorFromMidPoint (500.0);
    frequencySlider.setValue (currentFrequency, juce::dontSendNotification);
    frequencySlider.setTextBoxStyle (juce::Slider::TextBoxRight, false, 100, 20);
    frequencySlider.onValueChange = [this] { targetFrequency = frequencySlider.getValue(); };
    frequencyLabel.setText ("Frenquence", juce::dontSendNotification);

    levelSlider.setRange (0.0, 0.25);
    levelSlider.setValue ((double) currentLevel, juce::dontSendNotification);
    levelSlider.setTextBoxStyle (juce::Slider::TextBoxRight, false, 100, 20);
    levelSlider.onValueChange = [this] { targetLevel = (float) levelSlider.getValue(); };
    levelLabel.setText ("Level", juce::dontSendNotification);

    scope.setBufferSize (64);
    scope.setSamplesPerBlock (16);
    scope.setColours (juce::Colours::black, juce::Colours::white);

    //Midi
    addAndMakeVisible (midiInputListLabel);
    midiInputListLabel.setText ("MIDI Input:", juce::dontSendNotification);
    midiInputListLabel.attachToComponent (&midiInputList, true);

    auto midiInputs = juce::MidiInput::getAvailableDevices();
    addAndMakeVisible (midiInputList);
    midiInputList.setTextWhenNoChoicesAvailable ("No MIDI Inputs Enabled");

    juce::StringArray midiInputNames;
    for (auto input : midiInputs)
        midiInputNames.add (input.name);

    midiInputList.addItemList (midiInputNames, 1);
    midiInputList.onChange = [this] { setMidiInput (midiInputList.getSelectedItemIndex()); };

    for (auto input : midiInputs)
    {
        if (deviceManager.isMidiInputDeviceEnabled (input.identifier))
        {
            setMidiInput (midiInputs.indexOf (input));
            break;
        }
    }

    if (midiInputList.getSelectedId() == 0)
        setMidiInput (0);

    //Visible
    addAndMakeVisible (&frequencySlider);
    addAndMakeVisible (&frequencyLabel);
    addAndMakeVisible (&levelSlider);
    addAndMakeVisible (&levelLabel);
    addAndMakeVisible (&scope);
    addAndMakeVisible (keyboardComponent);

    startTimer (400);

}

MainComponent::~MainComponent()
{
    shutdownAudio();
}

//==============================================================================
void MainComponent::paint (juce::Graphics& g)
{
}

void MainComponent::resized()
{
    frequencyLabel.setBounds (10, 10, 90, 20);
    frequencySlider.setBounds (100, 10, getWidth() - 110, 20);
    levelLabel.setBounds (10, 30, 90, 20);
    levelSlider.setBounds (100, 30, getWidth() - 110, 20);
    midiInputList    .setBounds (200, 50, getWidth() - 210, 20);
    keyboardComponent.setBounds (10, getHeight()-400, getWidth() - 20, 200);
    scope.setBounds (10, getHeight()-200, getWidth() - 20, 200);

}

void MainComponent::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    synthAudioSource.prepareToPlay (samplesPerBlockExpected, sampleRate);
    /*
    currentSampleRate = sampleRate;
    updateAngleDelta();
    */
}

void MainComponent::releaseResources()
{
    synthAudioSource.releaseResources();
}

void MainComponent::getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill)
{
    /*
    auto* leftBuffer = bufferToFill.buffer->getWritePointer(0, bufferToFill.startSample);
    auto* rightBuffer = bufferToFill.buffer->getWritePointer(1, bufferToFill.startSample);

    auto localTargetFrequency = targetFrequency;

    if (! juce::approximatelyEqual (targetFrequency, currentFrequency))
    {
        auto frequencyIncrement = (targetFrequency - currentFrequency) / bufferToFill.numSamples;

        for (auto sample = 0; sample < bufferToFill.numSamples; ++sample)
        {
            auto currentSample = (float) std::sin (currentAngle);
            currentFrequency += frequencyIncrement;
            updateAngleDelta();
            currentAngle += angleDelta;
            leftBuffer[sample]  = currentSample;
            rightBuffer[sample] = currentSample;
        }

        currentFrequency = localTargetFrequency;
    }
    else
    {
        for (auto sample = 0; sample < bufferToFill.numSamples; ++sample)
        {
            auto currentSample = (float) std::sin (currentAngle);
            currentAngle += angleDelta;
            leftBuffer[sample]  = currentSample;
            rightBuffer[sample] = currentSample;
        }
    }

    auto localTargetLevel = targetLevel;
    bufferToFill.buffer->applyGainRamp (bufferToFill.startSample, bufferToFill.numSamples, currentLevel, localTargetLevel);
    currentLevel = localTargetLevel;
    */
    synthAudioSource.getNextAudioBlock (bufferToFill);
    scope.pushBuffer(bufferToFill);
}