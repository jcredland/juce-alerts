
#include "BootstrapAlerts.h"
#include "UiUnitTests.h"

/* 
Experimenting with putting tests of the GUI code in place.  I have tests for
some of more engine-room code, but these are the first GUI tests I've tried
with JUCE.

The helper functions may need a bit of refinement, and I have some further 
ideas about how to do these quickly and easily.
*/
class BootstrapAlert_Tests : public UiUnitTest
{
public:
	BootstrapAlert_Tests() 
		:
		UiUnitTest("Integrated Dialog")
	{}

	void runTest() override
	{
		Component mainComponent;
		BootstrapAlertContainer container;

		/* Setup :- */

		mainComponent.addAndMakeVisible(container);
		mainComponent.setSize(500, 500);
		container.setBounds(mainComponent.getLocalBounds());

		bool callbackWorked = false;
		auto dialog = new BootstrapAlert(Colours::red, "Test Dialog");

		dialog->setActionOnClose([&callbackWorked](int result)
		{
			if (result == -1)
				callbackWorked = true;
		});

		container.addAndOwnDialog(dialog);

		/* Tests:- */
		{
			beginTest("Dialog should display");
			expect(isVisible(&mainComponent, "dialog"));
		}

		{
			beginTest("Dialog should be removed when okay button is pushed");
			expect(clickButton(recursivelyFindComponentWithID(&mainComponent, "dialog"), "Close Button"));

			MessageManager::getInstance()->runDispatchLoopUntil(100);  /* Required to deal with the async
																	   callback after the button is clicked.
																	   But is there a better way of doing this? */
			expect(!isVisible(&mainComponent, "dialog"));
		}

		{
			beginTest("Dialog should call callback when button is clicked.");
			expect(callbackWorked);
		}

	}

private:
	/* 
	Having anything remotely complicated as a member variable here causes a crash on exit.  
	Presuambly because of the static initialization of the UnitTest below.  Exiting the application with 
	the BootstrapAlertContainer here for example causes a crash in the Timer callback of the 
	ComponentAnimator!  I've not bothered investigating further ... 
	*/
};

static BootstrapAlert_Tests integratedDialogTests;

