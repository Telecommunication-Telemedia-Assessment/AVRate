#pragma once

#include "FormRating.h"

namespace SliderTest {

	public ref class FormHardwareEnd : public FormRating
	{
	public:
		FormHardwareEnd(SliderTest::GeneralSettings^ gs):FormRating(gs)
		{
			this->systemculture = gs->systemculture;
			this->userculture = gs->userculture;

			InitializeComponent();
			
			SettingsReader^ reader = gcnew SettingsReader(gs->settingsFile);
			System::Collections::Generic::List<	System::Collections::Generic::List<KeyValueSetting^>^>^ 
				sliders = reader->ReadSliderList();

			int min = 0;
			int max = 1;

			for each (KeyValueSetting^ kv in sliders[0]) {
				if (kv->key == "min") {
					min = System::Convert::ToInt16(kv->value);
				}
				else if (kv->key == "max") {
					max = System::Convert::ToInt16(kv->value);
				}
			}

			for (int i = 0; i < gs->listID->Count; i++) {
				HardwareSlider^ hardSlider = gcnew HardwareSlider(i, min, max);
				controller->setSlider(hardSlider);
			}
			
			waitTimeRating = gs->waitTimeRating;

			if(controller->generalSettings->rmtype == RatingMethodType::Standard)
				labelPosition->Text = System::String::Format("Item: {0}/{1}", controller->GetVideoPos() + 1, controller->GetTotalVideoNumber());
			else
				labelPosition->Text = System::String::Format("Item: {0}", controller->GetItemsPlayed());

			timer1->Start();
			stopNext = gcnew System::Diagnostics::Stopwatch();
			stopNext->Reset();
			stopNext->Start();
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~FormHardwareEnd()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Timer^  timer1;
	private: System::ComponentModel::IContainer^  components;
	private: System::Windows::Forms::Label^  labelPosition;
	private: System::Windows::Forms::Label^  labelMessage;

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::Diagnostics::Stopwatch^ stopNext;
		int waitTimeRating;
		
#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			this->timer1 = (gcnew System::Windows::Forms::Timer(this->components));
			this->labelPosition = (gcnew System::Windows::Forms::Label());
			this->labelMessage = (gcnew System::Windows::Forms::Label());
			this->SuspendLayout();
			// 
			// timer1
			// 
			this->timer1->Tick += gcnew System::EventHandler(this, &FormHardwareEnd::timer1_Tick);
			// 
			// labelPosition
			// 
			this->labelPosition->AutoSize = true;
			this->labelPosition->Location = System::Drawing::Point(23, 24);
			this->labelPosition->Name = L"labelPosition";
			this->labelPosition->Size = System::Drawing::Size(43, 13);
			this->labelPosition->TabIndex = 10;
			this->labelPosition->Text = L"Item:";
			this->labelPosition->TextAlign = System::Drawing::ContentAlignment::MiddleRight;
			// 
			// labelMessage
			// 
			this->labelMessage->AutoSize = true;
			this->labelMessage->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->labelMessage->Location = System::Drawing::Point(26, 65);
			this->labelMessage->Name = L"labelMessage";
			this->labelMessage->Size = System::Drawing::Size(35, 13);
			this->labelMessage->TabIndex = 11;
			this->labelMessage->Text = L"";
			// 
			// FormHardwareEnd
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(292, 273);
			//this->ClientSize = System::Drawing::Size(467, 520);
			this->Controls->Add(this->labelMessage);
			this->Controls->Add(this->labelPosition);
			this->ControlBox = false;
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterScreen;
			this->KeyPreview = true;
			this->Name = L"FormHardwareEnd";
			this->Text = L"";
			this->Shown += gcnew System::EventHandler(this, &FormHardwareEnd::FormHardwareEnd_Shown);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion

	private: System::Void timer1_Tick(System::Object^  sender, System::EventArgs^  e) {
		bool next = false;

		// More than 1 slider ? Get the button state
		if (controller->generalSettings->listID->Count > 1)
			next = (stopNext->ElapsedMilliseconds > waitTimeRating) || controller->isNextItemAuto() ;
		else
			next = controller->GetSliderButton();

		// Playback stopped and Button Pressed ?
		if (next && !controller->getPlayStatus()) 
		{
			// wait for unpressing of button
			while(controller->GetSliderButton() == true)
				System::Threading::Thread::Sleep(50);

			timer1->Stop();//prevent double calls..necessary?

			if (!controller->isLastItemAuto())
				controller->logLastValues();

			bool isAuto = controller->isNextItemAuto();

			labelMessage->Text = "";

			controller->PlayNext();

			if(controller->generalSettings->rmtype == RatingMethodType::Standard)
			{
				// pre-playback incrementing display doesnt work for server
				int cPos = controller->GetVideoPos() + 1;
				int tPos = controller->GetTotalVideoNumber();
				if (cPos > tPos) cPos = tPos;// videonumber after final playback should stay within bounds
				labelPosition->Text = System::String::Format("Item: {0}/{1}", cPos, tPos);
			}
			else
				labelPosition->Text = System::String::Format("Item: {0}", controller->GetItemsPlayed());

			if(isAuto)
				labelMessage->Text = resman->GetString("HW_rateAuto",userculture); 
			else
				labelMessage->Text = resman->GetString("HW_rate",userculture);
			stopNext->Reset();
			stopNext->Start();
			timer1->Start();//restart
		}
	}
private: System::Void FormHardwareEnd_Shown(System::Object^  sender, System::EventArgs^  e) { 
			if(controller->generalSettings->rmtype == RatingMethodType::Standard)
				labelPosition->Text = System::String::Format("Item: {0}/{1}", controller->GetVideoPos() + 1, controller->GetTotalVideoNumber());
			else
				labelPosition->Text = System::String::Format("Item: {0}", controller->GetItemsPlayed()+1);
			labelMessage->Text = resman->GetString("HW_start",userculture); 
			if(kplayinitfile)
				controller->PlayInit();
		 }
};
}
