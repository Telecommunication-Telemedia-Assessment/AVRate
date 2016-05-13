#pragma once

#include "SettingsReader.h"
#include "MySlider.h"
#include "RatingGroup.h"

// groupbox containing a MySlider slider and labels along the slider 
// (settable from the settings.xml)

ref class SliderGroup : public RatingGroup
{
public:
	SliderGroup() {}
	// hack, used for hardware slider
	SliderGroup(System::Collections::Generic::List<KeyValueSetting^>^ dict, System::Globalization::CultureInfo^ cinfo);
	// used for normal slidergroups on SoftwareEnd, local shownumbers
	SliderGroup(System::Collections::Generic::List<KeyValueSetting^>^ dict, int count, sliderChangedDelegate^ s, System::Windows::Forms::Orientation orient, System::Globalization::CultureInfo^ cinfo);

	//void SliderGroup_KeyPress(System::Object^  sender, System::Windows::Forms::KeyPressEventArgs^  e) ;
	virtual void Reset() override;
	virtual bool hasChanged() override;
	virtual void makeRateable() override;
	MySlider^ getSlider();
	inline System::String^ getLabel()							{if(labelRate != nullptr) return labelRate->Text; else return "";}

private:
	// called, when the slider is being changed
	void sliderScroll(System::Object^ sender, System::EventArgs^ e);
	
	MySlider^ mySlider;
	System::Windows::Forms::Label^ labelRate;
	bool valueChanged;
	sliderChangedDelegate^ sliderChanged;
	float scaleX;
	float scaleY;
};
