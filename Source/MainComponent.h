/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#ifndef MAINCOMPONENT_H_INCLUDED
#define MAINCOMPONENT_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "BootstrapAlerts.h"
#include "ErrorGenerator.h"


class MainContentComponent : public Component
{
public:
    //==============================================================================
    MainContentComponent();
    ~MainContentComponent();

    void paint (Graphics&) override;
    void resized() override;

private:
	LookAndFeel_V3 laf;
	BootstrapAlertContainer integratedDialogContainer;
	ErrorGenerator generator;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)
};




#endif  // MAINCOMPONENT_H_INCLUDED
