#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
    setSize (600, 400);
    setAudioChannels(0,2);

    levelSlider.setRange (0.0, 0.25);
    levelSlider.setTextBoxStyle (juce::Slider::TextBoxRight, false, 100, 20);
    levelLabel.setText ("Noise Level", juce::dontSendNotification);

    scope.setBufferSize (128);
    scope.setSamplesPerBlock (16);
    scope.setColours (juce::Colours::black, juce::Colours::white);

    addAndMakeVisible (&levelSlider);
    addAndMakeVisible (&levelLabel);
    addAndMakeVisible (&scope);

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
    scope.setBounds (10, 30, getWidth() - 20, 200);
}

void MainComponent::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{

}

void MainComponent::releaseResources()
{

}

void MainComponent::getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill)
{
    auto* buffer = bufferToFill.buffer;
    auto level = (float) levelSlider.getValue();

    for (auto channel = 0; channel < buffer->getNumChannels(); ++channel)
    {
        auto* data = buffer->getWritePointer (channel, bufferToFill.startSample);

        for (auto sample = 0; sample < bufferToFill.numSamples; ++sample)
            data[sample] = random.nextFloat() * level;
    }

    scope.pushBuffer (*buffer);
}