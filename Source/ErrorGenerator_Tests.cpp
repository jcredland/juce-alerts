/*
  ==============================================================================

    ErrorGenerator_Tests.cpp
    Created: 6 Dec 2015 2:34:31pm
    Author:  Jim

  ==============================================================================
*/

#include "ErrorGenerator.h"
#include "UiUnitTests.h"

class ErrorGenerator_Tests : public UiUnitTest
{
public:
	ErrorGenerator_Tests() : UiUnitTest("ErrorGenerator") {}

	void runTest() override
	{
		Component base; 
		base.setSize(500, 500); 

		BootstrapAlertContainer container; 
		base.addAndMakeVisible(container); 
		container.setBounds(base.getLocalBounds()); 

		ErrorGenerator errorGenerator(container); 
		base.addAndMakeVisible(errorGenerator); 
		errorGenerator.setBounds(base.getLocalBounds().withSizeKeepingCentre(200, 200));

		beginTest("It should show a menu when the button is clicked");
		/* 
		No idea how to test this, as the menu appears as a new heavyweight 
		window.  How do we even find it?
		
		Our testing class will need to be a friend of the ErrorGenerator
		class?  Or failing that we can dive into find the component and then
		do a dynamic class to click the button.   This has the advantage that
		the button will need to be visible! 
		*/
		{
			beginTest("It should show errors when Add Errors is selected on the menu");
			auto countBefore = countChildrenWithID(&container, "dialog");
			errorGenerator.handleMenuOption(ErrorGenerator::menu_AddErrors);
			auto countAfter = countChildrenWithID(&container, "dialog");
			expect(countAfter > countBefore);
		}
		
		{
			beginTest("It should show errors when Add Notices is selected on the menu");
			auto countBefore = countChildrenWithID(&container, "dialog");
			errorGenerator.handleMenuOption(ErrorGenerator::menu_AddNotices);
			auto countAfter = countChildrenWithID(&container, "dialog");
			expect(countAfter > countBefore);
		}
	}
};


static ErrorGenerator_Tests errorGeneratorTests;