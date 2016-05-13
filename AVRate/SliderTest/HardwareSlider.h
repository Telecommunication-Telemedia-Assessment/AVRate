#pragma once;
#include "AbstractSlider.h"

#include "K8055D_C.h"

ref class HardwareSlider : AbstractSlider {
public:
	HardwareSlider(int min, int max);
	HardwareSlider(int number, int min, int max);

	virtual bool getButton() override;
	bool isConnected();
	int getRawValue();
	void setStabilized(bool s) { stabilized = s; } // removes slight value jitter

	virtual void readFromSlider() override;
#ifdef TESTMODE
	virtual void readFromSlider(System::String^) override;
#endif
#ifdef DEBUG
	virtual void readFromSliderLost() override;
#endif
protected:
	int deviceNumber;
	int scaleMin;
	int scaleMax;
	int analogueChannel;
	int digitalChannel;
	int digitalVal;
	bool stabilized; // to compensate the slight jitter of the slider, reduces precision a bit
	int lastAnalogueVal;
};
