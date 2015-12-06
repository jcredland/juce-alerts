/*
  ==============================================================================

    BootstrapAlert.h
    Created: 6 Dec 2015 1:26:15pm
    Author:  Jim

  ==============================================================================
*/

#ifndef INTEGRATEDDIALOG_H_INCLUDED
#define INTEGRATEDDIALOG_H_INCLUDED


#include "../JuceLibraryCode/JuceHeader.h"

#include "FontAwesome.h"

class BootstrapAlertContainer;

/**
 * A single alert, based on the Bootstrap CSS alert styling.   Use with
 * BootstrapAlertContainer.
 *
 * @todo - integrate this with my global logging and error handling class.
 * Add in progress bars and configurable buttons.
 */
class BootstrapAlert
	:
	public Component,
	public Button::Listener
{
public:

	BootstrapAlert();

    /**
     * Create an alert. The title will be shown in bold, the details in a
     * standard font. 
     */
	BootstrapAlert(Colour colour_, const String& title_, const String& details_ = String::empty);

    /** 
     * The function passed as 'callback' is run when the user dismisses the
     * alert.  The int passed will be -1 if the user clicks on the close
     * button, or another value when the addButton method is introduced in a
     * future version.  
     *
     * Typical use: 
     * @code 
     * setActionOnClose([this](int result){
     *   if (result == -1) cout << "user pressed close" << endl; 
     * }); 
     * @endcode
	*/
	void setActionOnClose(std::function<void(int)> callback);

    /** Returns the message. */
	AttributedString getMessage() const { return message; }

    /** 
     * Sets the message displayed to the user. The AttributedString allows you to 
     * apply bold and other text effects to the message. You don't need to use 
	 * this function - normally the constructor will do everything you need. 
     */
	void setMessage(const AttributedString& message_) { message = message_; }

    /**
     * Sets the colour of the alert.  It will use a light version of this colour
     * for the background and a darker version for the text.  Override the draw
     * method and getDarkColour() if this isn't your desired behaviour. 
     */
	void setColour(const Colour& colour_) { colour = colour_; }


    /** 
     * Used for the text colour in the paint method and the colour of the border
     * and the close icon.  Override to change the colour if setColour() isn't 
     * giving you what you need. 
     */
	virtual Colour getDarkColour() const;

    /**
     * Override paint if you really need to. 
     */
	void paint(Graphics& g) override;
protected:
    /** 
     * Returns the colour set for this alert. This may be useful if you
     * override the paint method. 
     */
    Colour getColour() const { return colour; }
    /**
     * Returns the area where the message should be drawn, allowing for 
     * progress bars, buttons and so forth.  Useful if you override the paint
     * method.
     */
	Rectangle<float> getTextArea(int componentWidth) const;
private:
    class CloseButton : public Button
    {
    public:
        CloseButton() : Button("Close Button") { setComponentID("Close Button");  setColour(Colours::black); }
        void setColour(Colour colour_) { colour = colour_; }
        void paintButton(Graphics & g, bool isMouseOver, bool /* isButtonDown */) override
        {
            g.setColour(colour.withMultipliedBrightness(isMouseOver ? 0.5f : 1.0f)); 
            FontAwesomeIcons::drawIcon(g, FontAwesomeIcons::faClose, getLocalBounds().toFloat());
        }
    private:
        Colour colour;
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CloseButton)
    };

    /** At some point in the future I'll add custom buttons and progress bars
     * to this. Until then. This is private. */
    void addButton(const String& buttonText);

    /** Return the minimum height for a given component width for this dialog,
     * allowing for the quantity of text. */
    float getMinimumDialogHeight(int dialogWidth) const;


	void resized() override;

    /** @internal - used by the constructors */
	void initalize(Colour colour_, const String& title_, const String& details_);
    /** @internal */
	void buttonClicked(Button* buttonThatWasClicked) override;
    /** @internal */
	void closeDialogWithResultCode(int result);

	friend class BootstrapAlertContainer;
	void setOwner(BootstrapAlertContainer* owner);

	BootstrapAlertContainer * owner {nullptr};

	AttributedString message;
	Colour colour;

	Rectangle<float> textArea;
	OwnedArray<TextButton> buttons;
	CloseButton closeButton;
	std::function<void(int)> actionOnClose;
	
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BootstrapAlert)
};

/** 
 * Displays error messages and dialogs.  Add to your MainComponent.  Set
 * the size to the full size of your window with setBounds().
 *
 * This displays the BootstrapAlerts.  You can create BootstrapAlerts manually
 * and then add them to the BootstrapAlertContainer by calling
 * addAndOwnDialog(BootstrapAlert *)
 *
 * @todo - allow shrinking of MainComponent so alerts are not shown over the
 * top of other content. 
 */
class BootstrapAlertContainer
	:
	public Component
{
public:
	BootstrapAlertContainer();

	/** Adds and owns a BootstrapAlert.  It will be deleted automatically 
	when the user clicks the close button. */
	void addAndOwnDialog(BootstrapAlert* dialog);

	void resized() override;

private:
	void removeDialog(BootstrapAlert* dialog);

	friend class BootstrapAlert;

	ComponentAnimator animator;

	OwnedArray<BootstrapAlert> dialogs;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BootstrapAlertContainer)
};



#endif  // INTEGRATEDDIALOG_H_INCLUDED
