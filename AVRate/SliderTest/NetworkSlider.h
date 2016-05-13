#pragma once;
#include "AbstractSlider.h"


ref class NetworkSlider : AbstractSlider {
public:
	NetworkSlider();

	virtual bool getButton() override;
	virtual void readFromSlider() override;

	void setValue(float v);

#ifdef DEBUG
	public:	virtual void readFromSliderLost() override {};	// internally read a value from the slider and //add it to the list/update the current value
#endif

};
