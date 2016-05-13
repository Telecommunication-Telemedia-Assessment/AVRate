#pragma once

#include "FormRating.h"

namespace SliderTest {

	/// <summary>
	/// Summary for FormSoftwareEnd
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class FormSoftwareConst : public FormRating
	{
	public:
		FormSoftwareConst(SliderTest::GeneralSettings^ gs):FormRating(gs)
		{
			this->systemculture = gs->systemculture;
			this->userculture = gs->userculture;

			InitializeComponent();
			this->ControlBox = false;
		
			SettingsReader^ reader = gcnew SettingsReader(gs->settingsFile);
			System::Collections::Generic::List<	System::Collections::Generic::List<KeyValueSetting^>^>^ 
				sliders = reader->ReadSliderList();

			slGroup = gcnew SliderGroup(sliders[0], userculture);
			this->Controls->Add(slGroup);

			SoftwareSlider^ softSlider = gcnew SoftwareSlider(slGroup->getSlider());
			controller->setSlider(softSlider);
			btnRepeat->Visible = false;
			#ifdef TESTMODE			
			sw = gcnew System::Diagnostics::Stopwatch();
			#endif
			samplingInterval = gs->samplingInterval;

			timer1->Start();
			timerM->Start();

			// move rating window to the bottom
			this->StartPosition = System::Windows::Forms::FormStartPosition::Manual;
			this->Width = System::Windows::Forms::Screen::PrimaryScreen->Bounds.Right;
			this->Height = 100;
			this->Top = System::Windows::Forms::Screen::PrimaryScreen->Bounds.Bottom - this->Height;
			this->Left = 0;

			// Init Position is 0
			if(gs->rmtype == RatingMethodType::Standard)
				labelPosition->Text = System::String::Format("Item: {0}/{1}", controller->GetVideoPos()+1, controller->GetTotalVideoNumber());
			else
				labelPosition->Text = System::String::Format("Item: {0}", controller->GetItemsPlayed());
			labelPosition->TextAlign = System::Drawing::ContentAlignment::BottomLeft;
			labelPosition->Left = 10;
			labelPosition->Top = this->Height - 27;

			btnPlay->Width = 50;
			btnPlay->Height = 50;
			btnPlay->Left = 10;
			btnPlay->Top = 10;

			clipRect = System::Drawing::Rectangle(this->Bounds.Left + 4,this->Bounds.Top + 4,this->Bounds.Width - 8,this->Bounds.Height - 8);

		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~FormSoftwareConst()
		{
			if (components)
			{
				delete components;
			}
		}
	private:
		System::Windows::Forms::Label^  labelPosition;
		System::Windows::Forms::Timer^  timer1;
		System::Windows::Forms::Timer^  timerM;
		System::ComponentModel::IContainer^  components;
		SliderGroup^ slGroup;
		System::Windows::Forms::Button^ btnPlay;
		System::Windows::Forms::Button^  btnRepeat;
		System::Globalization::CultureInfo^ userculture;
		System::Globalization::CultureInfo^ systemculture;
		int samplingInterval;

		private: System::Windows::Forms::Label^  labelcs;
		#ifdef TESTMODE
		System::Diagnostics::Stopwatch^ sw;
		#endif

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			this->labelPosition = (gcnew System::Windows::Forms::Label());
			this->timer1 = (gcnew System::Windows::Forms::Timer(this->components));
			this->timerM = (gcnew System::Windows::Forms::Timer(this->components));
			this->btnPlay = (gcnew System::Windows::Forms::Button());
			this->btnRepeat = (gcnew System::Windows::Forms::Button());

			this->labelcs = (gcnew System::Windows::Forms::Label());
			this->SuspendLayout();
			// 
			// labelPosition
			// 
			this->labelPosition->AutoSize = true;
			this->labelPosition->Location = System::Drawing::Point(41, 31);
			this->labelPosition->Name = L"labelPosition";
			this->labelPosition->Size = System::Drawing::Size(43, 13);
			this->labelPosition->TabIndex = 11;
			this->labelPosition->Text = L"Item:";
			this->labelPosition->TextAlign = System::Drawing::ContentAlignment::MiddleRight;
			// 
			// timer1
			// 
			this->timer1->Interval = 10;
			this->timer1->Tick += gcnew System::EventHandler(this, &FormSoftwareConst::timer1_Tick);
			// 
			// timerM
			// 
			this->timerM->Interval = 10;// fixed mouseclip Interval.
			this->timerM->Tick += gcnew System::EventHandler(this, &FormSoftwareConst::timerM_Tick);
			// 
			// btnPlay
			// 
			this->btnPlay->Location = System::Drawing::Point(0, 12);
			this->btnPlay->Name = L"btnPlay-g";
			this->btnPlay->Size = System::Drawing::Size(116, 56);
			this->btnPlay->TabIndex = 12;
			this->btnPlay->Text = L"Play";
			this->btnPlay->UseVisualStyleBackColor = true;
			this->btnPlay->Click += gcnew System::EventHandler(this, &FormSoftwareConst::btnPlay_Click);
			// 
			// btnRepeat
			// 
			this->btnRepeat->Enabled = false;
			this->btnRepeat->Location = System::Drawing::Point(78, 12);
			this->btnRepeat->Name = L"btnRepeat";
			this->btnRepeat->Size = System::Drawing::Size(75, 23);
			this->btnRepeat->TabIndex = 13;
			this->btnRepeat->Text = L"Nochmal";
			this->btnRepeat->UseVisualStyleBackColor = true;
			this->btnRepeat->Click += gcnew System::EventHandler(this, &FormSoftwareConst::btnNochmal_Click);
			// 
			// labelcs
			// 
			this->labelcs->AutoSize = true;
			this->labelcs->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 48, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->labelcs->Location = System::Drawing::Point(58, 13);
			this->labelcs->Name = L"labelcs";
			this->labelcs->Size = System::Drawing::Size(232, 73);
			this->labelcs->TabIndex = 14;
			// 
			// FormSoftwareConst
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(320, 98);
			this->Controls->Add(this->labelcs);
			this->Controls->Add(this->btnRepeat);
			this->Controls->Add(this->btnPlay);
			this->Controls->Add(this->labelPosition);
			this->KeyPreview = true;
			this->Name = L"FormSoftwareConst";
			this->TopMost = true;
			this->btnRepeat->Focus();// This takes the focus from btnPlay, so theres no accidental keypressing
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion

private:

	void PlayingDoneCallback(void) {
		controller->StopCapture();
		controller->StopLogging(controller->getLastItemName());//Check! is this necessary? only for endrating?
		timer1->Interval = 10;//check

		if(controller->generalSettings->logKeypress)
		{
			controller->logStoredKeyValues();
		}

		btnPlay->Enabled = true;
		btnPlay->Focus();

		if (!controller->isLastItemAuto())
			btnRepeat->Enabled = true;

		slGroup->Enabled = false;
		slGroup->Reset();
	}

private: 
	System::Void btnPlay_Click(System::Object^  sender, System::EventArgs^  e) 
	{
		#ifdef TESTMODE
		if (sw->IsRunning) 		 sw->Stop();
		sw->Reset();
		sw->Start();
		#endif
		// This has to be BEFORE isNextItemAuto or other calls to getnextitem!
		if (!controller->isLastItemAuto())
			controller->logStoredValues();

		timer1->Stop();
		timer1->Interval = samplingInterval;
		timerM->Stop();	// stop clipMouse to make autoitems accessible 
		btnPlay->Enabled = false;
		btnRepeat->Enabled = false;

		bool nextAuto = controller->isNextItemAuto();
		if (nextAuto)
		{
			unclipMouse();
		}
		else
		{
			slGroup->makeRateable();
			slGroup->Focus();// gives focus, so that keypressing for exit typing works
			timer1->Start();// hast to be called for autoitems as well! else no endcallback
			timerM->Start();
			//controller->endplay = false;
		}

		controller->PlayNext();	// WMP cant run as BackgroundWorker, needs STA attribute in Thread
		if (nextAuto)
		{
			//clipMouse();// do NOT enable this. Still clipped when regularly quitted (eg nomoreitems).// reclip again, only for auto items
			btnPlay->Enabled = true;
		}

		if(controller->generalSettings->rmtype == RatingMethodType::Standard)
		{
			int cPos = controller->GetVideoPos() + 1;
			int tPos = controller->GetTotalVideoNumber();
			if (cPos > tPos) cPos = tPos;// videonumber after final playback should stay within bounds
			labelPosition->Text = System::String::Format("Item: {0}/{1}", cPos, tPos);
		}
		else
			labelPosition->Text = System::String::Format("Item: {0}", controller->GetItemsPlayed());
	}
private: 
	System::Void timerM_Tick(System::Object^  sender, System::EventArgs^  e) {
		// Permanently clipping the mouse to the rating area

#ifndef NOCLIPMOUSE
		if(controller->disableClipping)// the only solution to keep Socket Exception Messageboxes accessible
		{
			// but doesnt work.half-frozen. enableMouse();// this is in addition, because mHook is involved again since Rev42
			unclipMouse();
		}
		else
			clipMouse();
		#endif
	}

private: 
	System::Void timer1_Tick(System::Object^  sender, System::EventArgs^  e) {

		#ifdef TESTMODE
			int secs = (int)(sw->ElapsedMilliseconds / 1000);
			int millis = (int)(sw->ElapsedMilliseconds - (secs * 1000));
			labelcs->Text = secs + ":" + millis;
			controller->storeSliderValues(secs + ":" + millis);		
		#else
			controller->storeSliderValues();
		#endif

		if(controller->endplay == true)
		{
			controller->endplay = false;	// set this! (because timer is still running)
			PlayingDoneCallback();
		}
	}
private: 
	System::Void btnNochmal_Click(System::Object^  sender, System::EventArgs^  e) {
		timer1->Interval = samplingInterval;
		btnRepeat->Enabled = false;
		btnPlay->Enabled = false;
		slGroup->makeRateable();
		slGroup->Focus();// gives focus, so that keypressing for exit typing works
	}

};

}
