#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
    setSize (600, 400);
    setAudioChannels(0,2);

    levelSlider.setRange (0.0, 0.25);
    levelSlider.setTextBoxStyle (juce::Slider::TextBoxRight, false, 100, 20);
    levelLabel.setText ("Noise Level", juce::dontSendNotification);

    addAndMakeVisible (&levelSlider);
    addAndMakeVisible (&levelLabel);

}

MainComponent::~MainComponent()
{
    shutdownAudio();
}

//==============================================================================
void MainComponent::paint (juce::Graphics& g)
{
    /*
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setFont (juce::FontOptions (16.0f));
    g.setColour (juce::Colours::white);
    g.drawText ("Soon", getLocalBounds(), juce::Justification::centred, true);
    g.fillRoundedRectangle (10, 10, 200, 200, 5.0);
    */
}

void MainComponent::resized()
{
    levelLabel .setBounds (10, 10, 90, 20);
    levelSlider.setBounds (100, 10, getWidth() - 110, 20);
}

void MainComponent::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{

}

void MainComponent::releaseResources()
{

}

void MainComponent::getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill)
{
    auto level = (float) levelSlider.getValue();

    for (auto channel = 0; channel < bufferToFill.buffer->getNumChannels(); ++channel)
    {
        // Get a pointer to the start sample in the buffer for this audio output channel
        auto* buffer = bufferToFill.buffer->getWritePointer (channel, bufferToFill.startSample);
        // Fill the required number of samples with noise between -0.125 and +0.125
        for (auto sample = 0; sample < bufferToFill.numSamples; ++sample)
            buffer[sample] = random.nextFloat() * level;
    }
}
