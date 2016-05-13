#include "StdAfx.h"
#include "ButtonSlider.h"

using namespace System::Windows::Forms;

ButtonSlider::ButtonSlider(System::Collections::Generic::List<System::Windows::Forms::RadioButton^>^ rblist) {
	radioButtonList = rblist;
}

void ButtonSlider::readFromSlider() {	
	// FIXME: get active radio button and put its value into here
	analogueValf = -1;
	int count = 0;
	for each (RadioButton^ rb in radioButtonList) {
		if (rb->Checked) {
			analogueValf = (float)count;
		}
		count++;
	}

	if (capturing)
		valueList.Add(analogueValf);
}