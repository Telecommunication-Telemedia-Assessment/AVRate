#pragma once
#include "abstractslider.h"

ref class ButtonSlider :
public AbstractSlider
{
public:
	ButtonSlider(System::Collections::Generic::List<System::Windows::Forms::RadioButton^>^ rblist);
	
	virtual void readFromSlider() override;
#ifdef TESTMODE
	virtual void readFromSlider(System::String^) override
	{};
#endif
#ifdef DEBUG
	virtual void readFromSliderLost() override
	{};
#endif
	
private:
	System::Collections::Generic::List<System::Windows::Forms::RadioButton^>^ radioButtonList;
};
