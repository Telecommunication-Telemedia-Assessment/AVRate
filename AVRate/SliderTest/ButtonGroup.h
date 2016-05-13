#pragma once
#include "ratinggroup.h"
#include "SettingsReader.h"

ref class ButtonGroup :
public RatingGroup
{
public:
	//ButtonGroup(void);
	ButtonGroup(System::Collections::Generic::List<KeyValueSetting^>^ dict, int count, sliderChangedDelegate^ s, System::Windows::Forms::Orientation orient, System::Globalization::CultureInfo^ cinfo);
	virtual void Reset() override;
	virtual bool hasChanged() override;
	virtual void makeRateable() override;
	System::Collections::Generic::List<System::Windows::Forms::RadioButton^>^ 	getButtons();
	
private:
	System::Collections::Generic::List<System::Windows::Forms::RadioButton^>^ radioButtonList;
	void buttonChanged(System::Object^ sender, System::EventArgs^ e);	

	System::Windows::Forms::Label^ labelRate;
	bool valueChanged;
	sliderChangedDelegate^ sliderChanged;
	float scaleX;
	float scaleY;
};
