#include "StdAfx.h"
#include "HardwareSlider.h"

#include "K8055D_C.h"

using namespace System::Collections::Generic;

HardwareSlider::HardwareSlider(int min, int max) : AbstractSlider() {
	scaleMin = min;
	scaleMax = max;

	analogueChannel = 1;
	digitalChannel = 1;
	
	stabilized = true;

	deviceNumber = 0; // device address, set by jumpers SK5/SK6
}

HardwareSlider::HardwareSlider(int number, int min, int max) : AbstractSlider() {
	scaleMin = min;
	scaleMax = max;

	analogueChannel = 1;
	digitalChannel = 1;
	
	stabilized = true;

	deviceNumber = number; // device address, set by jumpers SK5/SK6
}

bool HardwareSlider::isConnected() {
	bool connect = false;
	if (OpenDevice(deviceNumber) == deviceNumber)
		connect = true;
	CloseDevice();
	return (connect);
}
bool HardwareSlider::getButton() {
	if (OpenDevice(deviceNumber) == deviceNumber) { 
		bool val = ReadDigitalChannel(digitalChannel);
		CloseDevice();
		return val;
	}

	return false;
}
int HardwareSlider::getRawValue() {
	int val=0;
	if (OpenDevice(deviceNumber) == deviceNumber) 
		val = ReadAnalogChannel(analogueChannel);
	CloseDevice();
	return val;
}

#ifdef TESTMODE
void HardwareSlider::readFromSlider(System::String^) 
{}
#endif
#ifdef DEBUG
void HardwareSlider::readFromSliderLost() 
{}
#endif

void HardwareSlider::readFromSlider() {
	OpenDevice(deviceNumber);
	int val = ReadAnalogChannel(analogueChannel);
	CloseDevice();
	
	if (stabilized)	{
		if (System::Math::Abs(val - lastAnalogueVal) > 3) {
			val -= 4;
			if (val < 0) val = 0;
			lastAnalogueVal = val;
			analogueValf = val / 250.0f;
			int delta = scaleMax - scaleMin;
			analogueValf = analogueValf * delta - scaleMin;
		}
	}
	else {
		analogueValf = val / 255.0f;
		int delta = scaleMax - scaleMin;
		analogueValf = analogueValf * delta - scaleMin;
	}

	if (analogueValf > scaleMax) analogueValf = (float)scaleMax;
	else if (analogueValf < scaleMin) analogueValf = (float)scaleMin;
	
	if (capturing)
		valueList.Add(analogueValf);
}