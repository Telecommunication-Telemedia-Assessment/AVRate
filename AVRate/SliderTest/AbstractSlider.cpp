#include "StdAfx.h"
#include "AbstractSlider.h"

using namespace System::Collections::Generic;

AbstractSlider::AbstractSlider(void) {
	capturing = false;
}

float AbstractSlider::getSliderValue() {
	return analogueValf;
}

System::Collections::Generic::List<float>^ AbstractSlider::getValueList() {
	return %valueList;
}
#ifdef TESTMODE
System::Collections::Generic::List<System::String^>^ AbstractSlider::getXinfoList() {
	return %xinfoList;
}
#endif
void AbstractSlider::StartCapture() {
	capturing = true;
	valueList.Clear(); 
}

void AbstractSlider::StopCapture() {
	capturing = false;
}