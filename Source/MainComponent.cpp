/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "MainComponent.h"



//==============================================================================
MainContentComponent::MainContentComponent()
	:
	generator(integratedDialogContainer)
{
    setSize (600, 400);
	addAndMakeVisible(integratedDialogContainer);

	addAndMakeVisible(generator);

	auto dialog1 = new BootstrapAlert(Colours::green, "We succeeded in doing something!", "Aren't you great");
	integratedDialogContainer.addAndOwnDialog(dialog1);

	auto dialog2 = new BootstrapAlert(Colours::orange, "Some warning");
	integratedDialogContainer.addAndOwnDialog(dialog2);

	auto dialog3 = new BootstrapAlert(Colours::red, "Some serious error!", "With lots of additional detailed information that will probably spill over into the next line or two and may even required the dialog to be resized and or spill into a third line");
	integratedDialogContainer.addAndOwnDialog(dialog3);

	auto dialog4 = new BootstrapAlert(Colours::blue, "Information", "Is important");
	integratedDialogContainer.addAndOwnDialog(dialog4);


	getLookAndFeel().setDefaultLookAndFeel(&laf); 

	UnitTestRunner testRunner;
	testRunner.runAllTests();

}

MainContentComponent::~MainContentComponent()
{
}

void MainContentComponent::paint (Graphics& g)
{
    g.fillAll (Colour (0xff001F36));
    g.setFont (Font (16.0f));
}

void MainContentComponent::resized()
{
	integratedDialogContainer.setBounds(getLocalBounds());
	generator.setBounds(getLocalBounds().withSizeKeepingCentre(200, 200));
}

