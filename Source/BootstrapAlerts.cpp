/*
  ==============================================================================

    BootstrapAlert.cpp
    Created: 6 Dec 2015 1:26:15pm
    Author:  Jim

  ==============================================================================
*/

#include "BootstrapAlerts.h"
#include <functional>

Colour BootstrapAlert::getDarkColour() const
{
	return colour.withBrightness(0.4f); 
}

void BootstrapAlert::paint(Graphics& g)
{
	auto area = getLocalBounds().reduced(3).toFloat();

	g.setColour(colour.withBrightness(0.9f).withSaturation(0.3f));
	g.fillRoundedRectangle(area, 5.0f); 

	message.setColour(getDarkColour());
	g.drawRoundedRectangle(area, 5.0f, 1.0f); 
	message.draw(g, textArea); 
}

BootstrapAlert::BootstrapAlert()
{
	initalize(Colours::green, String::empty, String::empty);
}

BootstrapAlert::BootstrapAlert(Colour colour_, const String& title_, const String& details_)
{
	initalize(colour_, title_, details_);
}

void BootstrapAlert::addButton(const String& buttonText)
{
	auto button = new TextButton(buttonText);
	button->setComponentID(buttonText);
	buttons.add(button);
	button->addListener(this);
	addAndMakeVisible(button);
}

void BootstrapAlert::setActionOnClose(std::function<void(int)> callback)
{
	actionOnClose = callback;
}

float BootstrapAlert::getMinimumDialogHeight(int dialogWidth) const
{
	TextLayout layout;
	layout.createLayout(message, getTextArea(dialogWidth).getWidth()); 
	return jmax(layout.getHeight() + 10.0f, 30.0f);
}

Rectangle<float> BootstrapAlert::getTextArea(int componentWidth) const
{
	auto xPadding = 10;
	auto buttonWidth = 40;
	auto area = getLocalBounds()
		.withWidth(componentWidth)
		.reduced(xPadding, 1)
		.withTrimmedRight((buttonWidth + xPadding) * buttons.size())
		.toFloat()
		.withTrimmedRight(20.0f); /* for the close button. */
	return area;
}

void BootstrapAlert::resized()
{
	auto xPadding = 10;
	auto buttonWidth = 40;
	auto bounds = getLocalBounds();

	textArea = getTextArea(bounds.getWidth());

	auto x = int(textArea.getRight()) + xPadding;

	for (auto b: buttons)
	{
		b->setBounds(x, 2, int(buttonWidth), bounds.getHeight() - 4);
		x += buttonWidth + xPadding;
	}

	closeButton.setBounds(x, 2, 10, bounds.getHeight() - 4);
}

void BootstrapAlert::initalize(Colour colour_, const String& title_, const String& details_)
{
	setColour(colour_);

	AttributedString s;
	s.setJustification(Justification::centredLeft);
	s.setLineSpacing(0.5f);
	s.append(title_ + " ", Font(14.0f).boldened());
	s.append(details_, Font(14.0f));
	setMessage(s);

	addAndMakeVisible(closeButton); 
	closeButton.addListener(this); 
	closeButton.setColour(getDarkColour());

	setComponentID("dialog");
}

void BootstrapAlert::buttonClicked(Button* buttonThatWasClicked)
{
	if (buttonThatWasClicked == &closeButton)
	{
		closeDialogWithResultCode(-1); 
		return;
	}

	for (int i = 0; i < buttons.size(); ++i)
	{
		if (buttons[i] == buttonThatWasClicked)
		{
			closeDialogWithResultCode(i); 
			return;
		}
	}
}

void BootstrapAlert::closeDialogWithResultCode(int result)
{
	if (actionOnClose) 
		actionOnClose(result);

	owner->removeDialog(this); /* This will delete me! */
}

void BootstrapAlert::setOwner(BootstrapAlertContainer* owner_)
{
	owner = owner_;
}

BootstrapAlertContainer::BootstrapAlertContainer()
{
	setComponentID("BootstrapAlertContainer");
	setInterceptsMouseClicks(false, true);
}

void BootstrapAlertContainer::addAndOwnDialog(BootstrapAlert* dialog)
{
	dialogs.add(dialog);
	addAndMakeVisible(dialog);
	dialog->setOwner(this);
	resized();
}

void BootstrapAlertContainer::removeDialog(BootstrapAlert* dialog)
{
	auto heightRemoved = dialog->getHeight();
	removeChildComponent(dialog);

	auto index = dialogs.indexOf(dialog); 

	dialogs.removeObject(dialog);

	/* Move remaining dialogs, if any, up. */
	for (int i = index; i < dialogs.size(); ++i)
	{
		auto comp = dialogs[i];
		auto destination = comp->getBounds().translated(0, - heightRemoved); 
		animator.animateComponent(comp, destination, 1.0f, 500, true, 0.2, 0.2);
	}
}

void BootstrapAlertContainer::resized()
{
	auto bounds = getLocalBounds();

	for (auto d : dialogs)
	{
		auto dialogHeight = int(d->getMinimumDialogHeight(bounds.getWidth()));
		d->setBounds(bounds.withHeight(dialogHeight));
		bounds.translate(0, dialogHeight);
	}
}