#pragma once

#include "MySlider.h"

delegate void sliderChangedDelegate();

ref class RatingGroup abstract : public System::Windows::Forms::GroupBox
{
public:
	RatingGroup(void);
	virtual void Reset() abstract;
	virtual void makeRateable() abstract;
	virtual bool hasChanged() abstract;
};
