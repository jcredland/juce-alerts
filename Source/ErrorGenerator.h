
#ifndef ERRORGENERATOR_H_INCLUDED
#define ERRORGENERATOR_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "BootstrapAlerts.h"


/** @internal - helper class for showMenuAsync free functions below. Fractionally
less ugly than the standard JUCE methods with a callback to a static member, but 
still pretty unpleasant! */
template <typename WeakRefType>
class JModalCallback : public ModalComponentManager::Callback
{
public:
	JModalCallback(WeakRefType * criticalObject_, std::function<void(int)> action_)
		:
		criticalObject(criticalObject_), action(action_)
	{}
	void modalStateFinished(int result) override
	{
		if (criticalObject)
			action(result);
	}
private:
	WeakReference<WeakRefType> criticalObject;
	std::function<void(int)> action;
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(JModalCallback)
};


/** Shows a menu for a weak referencable type and calls the callback only if
 * the object hasn't been deleted whilst the menu was displayed.  Safe.
 */
template <typename T>
void showMenuAsync(
	PopupMenu & menu, 
	T * weakReferenceable, 
	std::function<void(int)> action, 
	PopupMenu::Options options = PopupMenu::Options()
	)
{
	auto callback = new JModalCallback<T>(weakReferenceable, action);
	menu.showMenuAsync(options, callback);
}

/** Shows a menu for a component and calls the callback only if the component
 * hasn't been deleted whilst the menu was displayed.  Safe.
 *
 * @note:There's a prize if you know the generic solution for finding a base
 * class containing a WeakReference<>.  Here we rely on the overloaded function
 * being given priority over the template version above ... */
inline void showMenuAsyncOnComponent(
	PopupMenu & menu, 
	Component * weakReferenceable, 
	std::function<void(int)> action,
	PopupMenu::Options options = PopupMenu::Options()
	)
{
	auto callback = new JModalCallback<Component>(weakReferenceable, action);
	menu.showMenuAsync(options, callback);
}

/** 
 * Error Generator is a component used for demonstrating the Integrated
 * Dialog.  It presents a bunch of options for creating new errors and the user
 * can then see how the BootstrapAlert responds. 
 *
 * It has a few more user input options than normal so I can also use it for
 * testing the UiTestUnit functionality.
 * 
 * If you are reading this code you aren't reading the important bits !
 * Have a look at the FontAwesome class, BootstrapAlert classes and
 * maybe some of the tests instead...
 */ 
class ErrorGenerator 
	: 
	public Component,
	public Button::Listener
{
public:
	enum
	{
		menu_AddErrors = 1,
		menu_AddWarnings,
		menu_AddNotices
	};

	ErrorGenerator(BootstrapAlertContainer & dialogContainer_)
		:
		dialogContainer(dialogContainer_)
	{
		menu.addItem(menu_AddErrors, "Add errors");
		menu.addItem(menu_AddWarnings, "Add warnings");
		menu.addItem(menu_AddNotices, "Add notices");

		showMenuButton.setButtonText("SHOW MENU"); 
		showMenuButton.setTriggeredOnMouseDown(true);
		showMenuButton.addListener(this);
		addAndMakeVisible(showMenuButton);
    }

	void resized() override
	{
		showMenuButton.setBounds(getLocalBounds().withSizeKeepingCentre(100, 20));
	}

	/** Let's expose this function so we can simulate menu selections for testing. */
	void handleMenuOption(int menuOption)
	{
		switch (menuOption)
		{
		case menu_AddErrors:
			addItem("Some new error", Colours::red); 
			break;
		case menu_AddWarnings:
			addItem("Some new warning", Colours::orange); 
			break;
		case menu_AddNotices:
			addItem("Some new notice", Colours::blue); 
			break;
		default:
			break;
		}
	}

	void buttonClicked(Button *) override
	{
		showMenuAsyncOnComponent(menu, this, [this](int result) { handleMenuOption(result); });  
	}

private:
	void addItem(const String & text, Colour colour)
	{
		auto dialog = new BootstrapAlert(colour, text); 
		dialogContainer.addAndOwnDialog(dialog); 
	}

	TextButton showMenuButton;
	
    /** We will introduce a pop-up menu just because these are awkward to test.  */
    PopupMenu menu; 
	
	BootstrapAlertContainer & dialogContainer;
};


#endif  // ERRORGENERATOR_H_INCLUDED
