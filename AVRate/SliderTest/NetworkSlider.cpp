#include "StdAfx.h"
#include "NetworkSlider.h"


using namespace System::Collections::Generic;

NetworkSlider::NetworkSlider() : AbstractSlider() {

}


bool NetworkSlider::getButton() {
	return true;
}

void NetworkSlider::readFromSlider() {

}

void NetworkSlider::setValue(float v) {
	analogueValf = v;
	if (capturing)
		valueList.Add(analogueValf);
}

