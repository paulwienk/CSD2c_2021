/*
  ==============================================================================

   This file is part of the JUCE tutorials.
   Copyright (c) 2020 - Raw Material Software Limited

   The code included in this file is provided under the terms of the ISC license
   http://www.isc.org/downloads/software-support-policy/isc-license. Permission
   To use, copy, modify, and/or distribute this software for any purpose with or
   without fee is hereby granted provided that the above copyright notice and
   this permission notice appear in all copies.

   THE SOFTWARE IS PROVIDED "AS IS" WITHOUT ANY WARRANTY, AND ALL WARRANTIES,
   WHETHER EXPRESSED OR IMPLIED, INCLUDING MERCHANTABILITY AND FITNESS FOR
   PURPOSE, ARE DISCLAIMED.

  ==============================================================================
*/

/*******************************************************************************
 The block below describes the properties of this PIP. A PIP is a short snippet
 of code that can be read by the Projucer and used to generate a JUCE project.

 BEGIN_JUCE_PIP_METADATA

 name:             ProcessingAudioInputTutorial
 version:          1.0.0
 vendor:           JUCE
 website:          http://juce.com
 description:      Performs processing on an input signal.

 dependencies:     juce_audio_basics, juce_audio_devices, juce_audio_formats,
                   juce_audio_processors, juce_audio_utils, juce_core,
                   juce_data_structures, juce_events, juce_graphics,
                   juce_gui_basics, juce_gui_extra
 exporters:        xcode_mac, vs2019, linux_make

 type:             Component
 mainClass:        MainContentComponent

 useLocalCopy:     1

 END_JUCE_PIP_METADATA

*******************************************************************************/


#pragma once

//==============================================================================
class MainContentComponent   : public juce::AudioAppComponent
{
public:
    //==============================================================================
    MainContentComponent()
    {
        levelSlider.setTextBoxStyle (juce::Slider::TextBoxRight, false, 100, 20);
        levelLabel.setText ("Noise Level", juce::dontSendNotification);

        addAndMakeVisible (levelSlider);
        addAndMakeVisible (levelLabel);

        setSize (600, 100);
        setAudioChannels (2, 2);
    }

    ~MainContentComponent() override
    {
        shutdownAudio();
    }

    void prepareToPlay (int, double) override {}

    void getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill) override
    {
        auto* device = deviceManager.getCurrentAudioDevice();
        auto activeInputChannels  = device->getActiveInputChannels();
        auto activeOutputChannels = device->getActiveOutputChannels();
        auto maxInputChannels  = activeInputChannels .getHighestBit() + 1;
        auto maxOutputChannels = activeOutputChannels.getHighestBit() + 1;

        auto level = (float) levelSlider.getValue();

        
        for (auto channel = 0; channel < maxOutputChannels; ++channel)
        {
            if ((! activeOutputChannels[channel]) || maxInputChannels == 0)
            {
                bufferToFill.buffer->clear (channel, bufferToFill.startSample, bufferToFill.numSamples);
            }
            else
            {
                auto actualInputChannel = channel % maxInputChannels; // [1]

                if (! activeInputChannels[channel]) // [2]
                {
                    bufferToFill.buffer->clear (channel, bufferToFill.startSample, bufferToFill.numSamples);
                }
                else // [3]
                {
                    auto* inBuffer = bufferToFill.buffer->getReadPointer (actualInputChannel,
                                                                          bufferToFill.startSample);
                    auto* outBuffer = bufferToFill.buffer->getWritePointer (channel, bufferToFill.startSample);

                    for (auto sample = 0; sample < bufferToFill.numSamples; ++sample)
                        outBuffer[sample] = inBuffer[sample] * level;
                }
            }
        }
    }

    

    void releaseResources() override {}

    void resized() override
    {
        levelLabel .setBounds (10, 10, 90, 20);
        levelSlider.setBounds (100, 10, getWidth() - 110, 20);
    }

private:
    juce::Random random;
    juce::Slider levelSlider;
    juce::Label levelLabel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)
};
