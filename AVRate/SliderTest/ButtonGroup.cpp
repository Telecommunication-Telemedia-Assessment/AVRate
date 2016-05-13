#include "StdAfx.h"
#include "ButtonGroup.h"
#include "RatingGroup.h"
#include "SettingsReader.h"

using namespace System::Windows::Forms;
using namespace System::Collections::Generic;

ref class CounterObject
{
public:	int count;
};
//CounterObject::CounterObject(void)
//{}
ButtonGroup::ButtonGroup(List<KeyValueSetting^>^ dict, int count, sliderChangedDelegate^ s, Orientation orient,System::Globalization::CultureInfo^ cinfo) {
	System::Resources::ResourceManager^ resman = gcnew System::Resources::ResourceManager("SliderTest.UImsg",System::Reflection::Assembly::GetExecutingAssembly());//typeof(FormSettings).Assembly);

	// Set scaling factors from screen size
	scaleX = float(System::Windows::Forms::Screen::PrimaryScreen->Bounds.Right) / kSTDSCREENWIDTH;
	scaleY = float(System::Windows::Forms::Screen::PrimaryScreen->Bounds.Bottom) / kSTDSCREENHEIGHT;

	sliderChanged = s;

	// read the slidervalues from the settings
	List<String^>^ buttonLabels = gcnew List<String^>;
	String^ groupCaption = "";

	for each (KeyValueSetting^ kv in dict) {
		if (kv->key == "label") { 
			buttonLabels->Add(kv->value);
		}
		else if (kv->key == "name") {
			groupCaption = kv->value;
		}
	}

	this->Text = groupCaption;
	int sliderHeight, buttonHeight, buttonWidth;
	int buttonDistanceY = 25;
	int buttonDistanceX = 10;
	int boxY = int(scaleY * kBOXTOPMARGIN);//ok
	if (orient == Orientation::Horizontal) {
		sliderHeight = int(scaleX * kSLIDERLENGTH);
		int boxHeight = int(scaleY * kBOXHEIGHT);
		this->SetBounds(kBOXLEFTMARGIN, boxY + (count * (boxHeight + kBOXTOBOXMARGIN)), sliderHeight + kSLIDERMARGINX*2, boxHeight);
		buttonHeight = int(scaleY * kBUTTONH);
		buttonWidth = (int)((this->Width - (buttonLabels->Count + 1) * buttonDistanceX) / (float)buttonLabels->Count);
	}//ok
	else {
		sliderHeight = int(scaleY * kSLIDERLENGTH);
		int boxWidth = int(scaleX * kBOXWIDTH);
		this->SetBounds(kBOXLEFTMARGIN + (count * (boxWidth + kBOXTOBOXMARGIN)), boxY, boxWidth, sliderHeight + kSLIDERMARGINY*2);
		// 20 is additional space for Rate text
		buttonHeight = (int)((this->Height - (buttonLabels->Count + 1) * buttonDistanceY -20) / (float)buttonLabels->Count);
		buttonWidth = int(scaleX * kBUTTONW);
	}//ok

	radioButtonList = gcnew List<RadioButton^>();
	int counter = 0;
	int maxcounter = buttonLabels->Count - 1;
	for each (String^ s in buttonLabels) {
		RadioButton^ rb = gcnew RadioButton();
		CounterObject^ countobj=gcnew CounterObject();
		if (orient == Orientation::Horizontal)
			countobj->count = counter;
		else
			countobj->count = maxcounter - counter;
		rb->Text = s;
		rb->Tag = countobj;
		rb->Appearance = System::Windows::Forms::Appearance::Button;
		rb->FlatStyle = System::Windows::Forms::FlatStyle::System;
		rb->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
		rb->CheckedChanged += gcnew EventHandler(this, &ButtonGroup::buttonChanged);
		if (orient == Orientation::Horizontal)
			rb->SetBounds((counter * (buttonWidth + buttonDistanceX)) + buttonDistanceX,
						 this->Height / 2 - buttonHeight / 2, buttonWidth, buttonHeight);
		else
			rb->SetBounds(this->Width / 2 - buttonWidth / 2, 
						((maxcounter-counter) * (buttonHeight + buttonDistanceY)) + buttonDistanceY, 
						buttonWidth, buttonHeight);
		radioButtonList->Add(rb);
		Controls->Add(rb);
		++counter;
	}

	// label to tell the subject to do a rating on this slider
	labelRate = gcnew Label();
	labelRate->Width = int(scaleX * 100);
	labelRate->Height = int(scaleY * 23);
	labelRate->Text = resman->GetString("GRP_rate",cinfo); 
	labelRate->TextAlign = ContentAlignment::MiddleCenter;
	labelRate->Left = Width / 2 - labelRate->Width / 2;
	labelRate->Top = Height - labelRate->Height - 2;
	labelRate->Visible = false;
	Controls->Add(labelRate);

	valueChanged = true;
	Enabled = false;
}

	
void ButtonGroup::Reset() {
	labelRate->Visible = false;
	for each (RadioButton^ rb in radioButtonList)
		rb->Checked = false;
	valueChanged = false;
}

bool ButtonGroup::hasChanged() {
	return valueChanged;
}

void ButtonGroup::makeRateable() {
	Enabled = true;
	labelRate->Visible = true;
}

List<RadioButton^>^ ButtonGroup::getButtons() {
	return radioButtonList;
}

void ButtonGroup::buttonChanged(System::Object^ sender, System::EventArgs^ e) {
	labelRate->Visible = false;
	valueChanged = true;
	sliderChanged();
}