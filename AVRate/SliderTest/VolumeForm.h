#pragma once	

// form to set the audio volume during the training phase, either with a slider or 3 buttons
ref class VolumeForm : System::Windows::Forms::Form {
public:
	static VolumeForm^ buttonVolumeFactory(System::Drawing::Rectangle^ parentBounds, System::Globalization::CultureInfo^ cinfo) 
	{
		VolumeForm^ volumeForm = gcnew VolumeForm(cinfo);
		
		volumeForm->StartPosition = System::Windows::Forms::FormStartPosition::Manual;
		volumeForm->ControlBox = false;
		volumeForm->Width = parentBounds->Width;
		volumeForm->Height = 120;
		volumeForm->Left = parentBounds->Left;
		volumeForm->Top = parentBounds->Top - volumeForm->Height;

		System::Windows::Forms::RadioButton^ lNormal;
		System::Windows::Forms::RadioButton^ lLouder;
		System::Windows::Forms::RadioButton^ lLower;

		lNormal = gcnew System::Windows::Forms::RadioButton();
		lLouder = gcnew System::Windows::Forms::RadioButton();
		lLower = gcnew System::Windows::Forms::RadioButton();

		lNormal->Appearance = System::Windows::Forms::Appearance::Button;
		lLouder->Appearance = System::Windows::Forms::Appearance::Button;
		lLower->Appearance = System::Windows::Forms::Appearance::Button;
			
		lNormal->FlatStyle = System::Windows::Forms::FlatStyle::System;
		lLouder->FlatStyle = System::Windows::Forms::FlatStyle::System;
		lLower->FlatStyle = System::Windows::Forms::FlatStyle::System;
		

		lNormal->Click += gcnew System::EventHandler(volumeForm, &VolumeForm::btnNormalClick);
		lLouder->Click += gcnew System::EventHandler(volumeForm, &VolumeForm::btnNormalLouder);
		lLower->Click += gcnew System::EventHandler(volumeForm, &VolumeForm::btnNormalLower);
		
		int btnWidth = volumeForm->Width / 3 - 30;
		lLouder->Width = btnWidth;
		lLower->Width = btnWidth;
		lNormal->Width = btnWidth;
		lLouder->Top = 20;
		lLower->Top = 20;
		lNormal->Top = 20;
		int btnHeight = volumeForm->Height - 40;
		lLouder->Height = btnHeight;
		lLower->Height = btnHeight;
		lNormal->Height = btnHeight;

		lNormal->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
		lLouder->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
		lLower->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;


		System::Resources::ResourceManager^ resman = gcnew System::Resources::ResourceManager("SliderTest.UImsg",System::Reflection::Assembly::GetExecutingAssembly());
		lNormal->Text = resman->GetString("VOL_normal",cinfo);
		lLouder->Text = resman->GetString("VOL_louder",cinfo);
		lLower->Text = resman->GetString("VOL_softer",cinfo);

		lLower->Left = 20;
		lNormal->Left = volumeForm->Width / 2 - lNormal->Width / 2;
		lLouder->Left = volumeForm->Width - 20 - lLouder->Width;

		lNormal->Checked = true;

		volumeForm->Controls->Add(lNormal);
		volumeForm->Controls->Add(lLouder);
		volumeForm->Controls->Add(lLower);
		return volumeForm;
	}

	static VolumeForm^ sliderVolumeFactory(System::Drawing::Rectangle^ parentBounds, System::Globalization::CultureInfo^ cinfo) {
		VolumeForm^ volumeForm = gcnew VolumeForm(cinfo);
	
		volumeForm->StartPosition = System::Windows::Forms::FormStartPosition::Manual;
		volumeForm->ControlBox = false;
		volumeForm->Width = parentBounds->Width;
		volumeForm->Height = 120;
		volumeForm->Left = parentBounds->Left;
		volumeForm->Top = parentBounds->Top - volumeForm->Height;

		MySlider^ volSlider = gcnew MySlider();
		volSlider->setBounds(35, 20, 40, volumeForm->Width - 40);
		volSlider->setMinMax(0, 100);
		volSlider->valueChanged += gcnew System::EventHandler(volumeForm, &VolumeForm::sliderChange);
		volSlider->enablePointer(true);
		volSlider->setValue(0.5f);
			
		System::Windows::Forms::Label^ lVolume = gcnew System::Windows::Forms::Label();

		System::Resources::ResourceManager^ resman = gcnew System::Resources::ResourceManager("SliderTest.UImsg",System::Reflection::Assembly::GetExecutingAssembly());
		lVolume->Text = resman->GetString("VOL_set",cinfo);
		lVolume->Left = 20;
		lVolume->Top = 10;

		System::Windows::Forms::Label^ lNormal = gcnew System::Windows::Forms::Label();
		System::Windows::Forms::Label^ lLouder = gcnew System::Windows::Forms::Label();
		System::Windows::Forms::Label^ lLower = gcnew System::Windows::Forms::Label();
		
		lLouder->Width = 50;
		lLower->Width = 50;
		lNormal->Width = 50;
		lLouder->Top = 80;
		lLower->Top = 80;
		lNormal->Top = 80;

		lNormal->Text = resman->GetString("VOL_normal",cinfo);
		lLouder->Text = resman->GetString("VOL_louder",cinfo);
		lLower->Text = resman->GetString("VOL_softer",cinfo);

		lLouder->TextAlign = System::Drawing::ContentAlignment::MiddleLeft;
		lLouder->Left = 20;

		lNormal->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
		lNormal->Left = volumeForm->Width / 2 - lNormal->Width / 2;
		
		lLower->TextAlign = System::Drawing::ContentAlignment::MiddleRight;
		lLower->Left = volumeForm->Width - 20 - lLouder->Width;
		
		volumeForm->Controls->Add(volSlider);
		volumeForm->Controls->Add(lNormal);
		volumeForm->Controls->Add(lLouder);
		volumeForm->Controls->Add(lLower);
		volumeForm->Controls->Add(lVolume);
		return volumeForm;

	}

	float getVolume() { return volume; }
	void setVolume(float v) { volume = v; }
private:
	VolumeForm(System::Globalization::CultureInfo^ cinfo) {}
	
	void btnNormalClick(System::Object^  sender, System::EventArgs^  e) {
		setVolume(1.0f);
	}
	void btnNormalLouder(System::Object^  sender, System::EventArgs^  e) {
		setVolume(3.2f);
	}
	void btnNormalLower(System::Object^  sender, System::EventArgs^  e) {
		setVolume(.2f);
	}
	void sliderChange(System::Object^  sender, System::EventArgs^  e) {
		MySlider^ s = (MySlider^)sender;
		float norm = s->getValue() / 100.0f - 0.5f; // => norm = [-.5, 0.5]
		setVolume(1 + norm / 2.f);
	}


	float volume;
};
