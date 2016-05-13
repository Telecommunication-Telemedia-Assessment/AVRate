#include "StdAfx.h"
#include "SoftwareSlider.h"

SoftwareSlider::SoftwareSlider(MySlider^ m) {
	trackSlider = m;
}
#ifdef TESTMODE
void SoftwareSlider::readFromSlider(String^ time) {
	analogueValf = trackSlider->getValue();
	
	if (capturing)
	{
		valueList.Add(analogueValf);
		xinfoList.Add(time);
	}
}
#endif
#ifdef DEBUG
void SoftwareSlider::readFromSliderLost() {
	// reads values from slider which are normally missed out ( eg not in a certain time frame of playing etc)
	// marks them with a negative sign
	analogueValf = -trackSlider->getValue();
	
	if (capturing)
		valueList.Add(analogueValf);
}
#endif
void SoftwareSlider::readFromSlider() {
	analogueValf = trackSlider->getValue();
	
	if (capturing)
		valueList.Add(analogueValf);
}