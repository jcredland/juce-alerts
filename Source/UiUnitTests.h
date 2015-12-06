
#ifndef UIUNITTESTS_H_INCLUDED
#define UIUNITTESTS_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

/** Adds some experimental UI specific utility functions to UnitTest. */
class UiUnitTest : public UnitTest
{
public:
	UiUnitTest(const String & name) : UnitTest (name) {}

	/** Returns false if the button click could not be simulated. Otherwise simulates a button click using the triggerClick method of the button */
	static bool clickButton(Component * rootComponent, const String & buttonComponentId)
	{
		if (!rootComponent)
			return false;

		auto c = recursivelyFindComponentWithID(rootComponent, buttonComponentId);

		auto button = dynamic_cast<Button *>(c);

		if (!button)
			return false; /* Also covers c == nullptr, i.e. no component found */

		button->triggerClick();
		return true;
	}

	static Component * recursivelyFindComponentWithID(Component * currentBase, const String & componentID) 
	{
		if (currentBase->getComponentID() == componentID)
			return currentBase;

		int numChildren = currentBase->getNumChildComponents();

		for (int componentIndex = 0; componentIndex < numChildren; ++componentIndex)
		{
			auto c = currentBase->getChildComponent(componentIndex);
			auto result = recursivelyFindComponentWithID(c, componentID);

			if (result)
				return result;
		}

		return nullptr;
	}

	static int countChildrenWithID(Component * base, const String & componentID)
	{
		std::function<int(Component *)> counter;

		counter = [componentID, &counter](Component * currentBase)
		{
			if (currentBase->getComponentID() == componentID)
				return 1;

			int numChildren = currentBase->getNumChildComponents();
			int count = 0;

			for (int componentIndex = 0; componentIndex < numChildren; ++componentIndex)
			{
				auto c = currentBase->getChildComponent(componentIndex);
				count += counter(c);
			}
			return count;
		};

		return counter(base); 
	}

	/** Return true if a child of rootComponent with ID componentID is visible.
	Searches recursively.  Does not check that the bounds are on screen.
	Assumes that rootComponent is visible. */
	static bool isVisible(Component * rootComponent, const String & componentID)
	{
		if (!rootComponent)
			return false;

		auto c = recursivelyFindComponentWithID(rootComponent, componentID);

		if (!c)
			return false;

		while (
			c
			&&
			c->isVisible()
			&&
			c->getWidth() != 0
			&&
			c->getHeight() != 0
			&&
			c != rootComponent
			)
		{
			c = c->getParentComponent();
		}

		/* We have navigated to the root component */
		if (c == rootComponent)
			return true;

		return false;
	}

};


#endif  // UIUNITTESTS_H_INCLUDED
