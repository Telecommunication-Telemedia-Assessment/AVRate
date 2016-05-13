#include "StdAfx.h"
#include "sliderGroup.h"

using namespace System;
using namespace System::Collections::Generic;
using namespace System::Windows::Forms;

// sliders can be either horizontal or vertical, this constructor takes both ways into account, local shownumbers
SliderGroup::SliderGroup(List<KeyValueSetting^>^ dict, int count, sliderChangedDelegate^ s, System::Windows::Forms::Orientation orient, System::Globalization::CultureInfo^ cinfo)
{
	System::Resources::ResourceManager^ resman = gcnew System::Resources::ResourceManager("SliderTest.UImsg",System::Reflection::Assembly::GetExecutingAssembly());//typeof(FormSettings).Assembly);
	
	// Set scaling factors from screen size
	scaleX = float(System::Windows::Forms::Screen::PrimaryScreen->Bounds.Right) / kSTDSCREENWIDTH;
	scaleY = float(System::Windows::Forms::Screen::PrimaryScreen->Bounds.Bottom) / kSTDSCREENHEIGHT;

	sliderChanged = s;

	// read the slidervalues from the settings
	List<String^>^ sliderLabels = gcnew List<String^>;
	String^ sliderCaption = "";
	int min = 0;
	int max = 100;
	int ticks = 10;
	bool showNumbers = false;
	bool fixedticks = false;
	
	for each (KeyValueSetting^ kv in dict) {
		if (kv->key == "label") {
			sliderLabels->Add(kv->value);
		}
		else if (kv->key == "name") {
			sliderCaption = kv->value;
		}
		else if (kv->key == "ticks") {
			ticks = Convert::ToInt16(kv->value);
		}
		else if (kv->key == "fixed") {
			fixedticks = Convert::ToBoolean(kv->value);
		}
		else if (kv->key == "min") {
			min = Convert::ToInt16(kv->value);
		}
		else if (kv->key == "max") {
			max = Convert::ToInt16(kv->value);
		}
		else if (kv->key == "shownumbers") {
			showNumbers = Convert::ToBoolean(kv->value);
		}
	}

	this->Text = sliderCaption;

	int sliderHeight;
	int boxY = int(scaleY * kBOXTOPMARGIN);//ok
	if (orient == Orientation::Horizontal) {
		sliderHeight = int(scaleX * kSLIDERLENGTH);
		int boxHeight = int(scaleY * kBOXHEIGHT);
		this->SetBounds(kBOXLEFTMARGIN, boxY + (count * (boxHeight + kBOXTOBOXMARGIN)), sliderHeight + kSLIDERMARGINX*2, boxHeight);
	}//ok
	else {
		sliderHeight = int(scaleY * kSLIDERLENGTH);
		int boxWidth = int(scaleX * kBOXWIDTH  ); 
		this->SetBounds(kBOXLEFTMARGIN + (count * (boxWidth + kBOXTOBOXMARGIN)), boxY, boxWidth, sliderHeight + kSLIDERMARGINY*2);
	}//ok

	mySlider = gcnew MySlider();
	mySlider->setOrientation(orient);
	//mySlider->KeyPreview = true;

	// Set Position of slider here (except: leave 2nd argument fixed. See sliderY handling in MySlider.h)
	if (orient == Orientation::Horizontal)
		mySlider->setBounds((this->Height-kSLIDERH/2)/3, kSLIDERMARGINX, int(kSLIDERH*scaleY), sliderHeight);// y,x,h,w
		//mySlider->setBounds( 40, 50, 40, sliderHeight);
	else
		mySlider->setBounds((this->Width-kSLIDERH/2)/3, kSLIDERMARGINY, int(kSLIDERH*scaleX), sliderHeight);// x,y,w,h
		//mySlider->setBounds(50, 40, 40, sliderHeight);//distance to labels...
	//ok
	mySlider->valueChanged += gcnew EventHandler(this, &SliderGroup::sliderScroll);
	mySlider->enablePointer(false);
	mySlider->setMinMax(min, max);
	mySlider->setTicks(ticks);
	mySlider->setFixedTicks(fixedticks);
	Controls->Add(mySlider);

	int labelCount = 0;
	int positionSwitch = 0;
	//bool switchPositions = true;
	int labeltextwidth = int(scaleX * 90); 
	if ((orient == Orientation::Horizontal) && (switchPositions==false))
		labeltextwidth = (sliderHeight - 0*kSLIDERMARGINX) / sliderLabels->Count;
	int labeltextheight = int(scaleY * 26);//check 28

	// Add description to ticks on the slider
	for each (String^ s in sliderLabels) {
		Label^ l = gcnew Label();
		l->Text = s;
		l->Height = int(scaleY*23);
		// align the labels equally besides/below the slider
		if (orient == Orientation::Horizontal) 
		{
			l->Width = labeltextwidth;
			l->TextAlign = ContentAlignment::MiddleCenter;
			l->Top = mySlider->Bottom + positionSwitch*labeltextheight;
			l->Left = 2*kSLIDERMARGINX - labeltextwidth/2 + (int)(labelCount * (sliderHeight - 2*kSLIDERMARGINX) /(float)(sliderLabels->Count - 1));
		}//ok
		else 
		{
			float lmax = (float)(sliderLabels->Count - 1);
			l->TextAlign = ContentAlignment::MiddleLeft;
			l->Left = mySlider->Right;
			l->Width = labeltextwidth;
			l->Top = 2*kSLIDERMARGINY - labeltextheight/2 + (int)((lmax-labelCount) * (sliderHeight - 2*kSLIDERMARGINY) / lmax);
		}//ok
		if(s != "")
		{
			if(switchPositions)
				positionSwitch ^= 1;
			Controls->Add(l);
		}//ok
		labelCount++;
	}

	// add a number corresponding to its value besides each tick
	if (ticks > 1 && showNumbers) {
		float delta = ((max - min) / (float)(ticks - 1));
		
		for (int i = 0; i < ticks; i++) {
			Label^ l = gcnew Label();
			int num = (int)Math::Round(i * delta);
			l->Width = int(scaleX*25);
			l->Height = int(scaleY*23);

			if (orient == Orientation::Horizontal) {
				num += min;	// add the offset, only for horizontal
				l->Text = Convert::ToString(num);
				l->TextAlign = ContentAlignment::MiddleCenter;
				l->Left = 2*kSLIDERMARGINX - l->Width/2 + (int)(i * (sliderHeight - 2*kSLIDERMARGINX) / (float)(ticks - 1));
				l->Top = mySlider->Top - l->Height;
				//l->Top = (this->Height - kSLIDERH)/2 - int(scaleY * 20);// Check! scaleX?
			}//ok
			else {
				num = max - num; // reverse the order of the numbers, only for vertical
				l->Text = Convert::ToString(num);
				l->TextAlign = ContentAlignment::MiddleRight;
				l->Top = 2*kSLIDERMARGINY - labeltextheight/2 + (int)(i * (sliderHeight - 2*kSLIDERMARGINY) / (float)(ticks - 1));
				l->Left = mySlider->Left - l->Width;
				//l->Left = (this->Width - kSLIDERH)/2 - int(scaleX * 20);
			}//ok

			//this->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &SliderGroup::SliderGroup_KeyPress);
			Controls->Add(l);
		}
	}

	// label to tell the subject to do a rating on this slider
	labelRate = gcnew Label();
	labelRate->Width = int(scaleX * 100);
	labelRate->Height = int(scaleY * 23);
	labelRate->Text = resman->GetString("GRP_rate",cinfo); 
	labelRate->TextAlign = ContentAlignment::MiddleCenter;
	//labelRate->BackColor = System::Drawing::Color::Red;
	labelRate->Left = Width / 2 - labelRate->Width / 2;
	labelRate->Top = Height - labelRate->Height - 7;
	labelRate->Visible = false;
	Controls->Add(labelRate);

	valueChanged = true;
	Enabled = false;
}


// ugly doubling of the above constructor for continuous rating purpose
// slider goes horizontally over the screen
SliderGroup::SliderGroup(List<KeyValueSetting^>^ dict, System::Globalization::CultureInfo^ cinfo) {
	System::Resources::ResourceManager^ resman = gcnew System::Resources::ResourceManager("SliderTest.UImsg",System::Reflection::Assembly::GetExecutingAssembly());//typeof(FormSettings).Assembly);

	// Set scaling factors from screen size
	scaleX = float(System::Windows::Forms::Screen::PrimaryScreen->Bounds.Right) / kSTDSCREENWIDTH;
	scaleY = float(System::Windows::Forms::Screen::PrimaryScreen->Bounds.Bottom) / kSTDSCREENHEIGHT;

	int boxWidth = System::Windows::Forms::Screen::PrimaryScreen->WorkingArea.Width - 80;
	int sliderHeight = boxWidth - 60;

	// defaults, override from file
	List<String^>^ sliderLabels = gcnew List<String^>;
	String^ sliderCaption = "";
	int min = 0;
	int max = 100;
	int ticks = 11;

	for each (KeyValueSetting^ kv in dict) {
		if (kv->key == "label") {
			sliderLabels->Add(kv->value);
		}
		else if (kv->key == "name") {
			sliderCaption = kv->value;
		}
		else if (kv->key == "ticks") {
			ticks = Convert::ToInt16(kv->value);
		}
		else if (kv->key == "min") {
			min = Convert::ToInt16(kv->value);
		}
		else if (kv->key == "max") {
			max = Convert::ToInt16(kv->value);
		}
	}

	Text = sliderCaption;

	int boxHeight = 80;
	//int boxHeight = int(scaleY*80);
	SetBounds(70, 7, boxWidth, boxHeight);

	mySlider = gcnew MySlider();
	mySlider->setOrientation(Windows::Forms::Orientation::Horizontal);
	mySlider->setBounds(15, 30, 30, sliderHeight);
	mySlider->enablePointer(false);
	mySlider->setMinMax(min, max);
	mySlider->setTicks(ticks);
	Controls->Add(mySlider);

	int labelCount = 0;

	for each (String^ s in sliderLabels) {
		Label^ l = gcnew Label();
		l->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
		l->Width = 70;
		l->Text = s;
		l->Left = 30 + 10 + (int)(labelCount * (sliderHeight - 20) /(sliderLabels->Count - 1)) - l->Width / 2;
		l->Top = 45;

		Controls->Add(l);
		labelCount++;
	}

	labelRate = gcnew Label();
	labelRate->Visible = false;

	valueChanged = true;
	Enabled = false;
}

void SliderGroup::sliderScroll(System::Object^ sender, System::EventArgs^ e) {
	labelRate->Visible = false;
	valueChanged = true;
	sliderChanged();
}

void SliderGroup::Reset() {
	labelRate->Visible = false;
	mySlider->enablePointer(false);
	valueChanged = false;
}

bool SliderGroup::hasChanged() {
	return valueChanged;
}

void SliderGroup::makeRateable() {
	Enabled = true;
	labelRate->Visible = true;
}

MySlider^ SliderGroup::getSlider() {
	return mySlider;
}