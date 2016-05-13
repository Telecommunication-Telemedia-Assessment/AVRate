#pragma once
#include "AbstractSlider.h"
#include "MySlider.h"

ref class SoftwareSlider : public AbstractSlider
{
public:
	SoftwareSlider(MySlider^ m);

	virtual void readFromSlider() override;
#ifdef TESTMODE
	virtual void readFromSlider(System::String^ time) override ;
#endif
#ifdef DEBUG
	virtual void readFromSliderLost() override ;
#endif

private:
	MySlider^ trackSlider;
};
