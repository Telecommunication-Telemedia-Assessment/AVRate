#pragma once

#include "FormRating.h"


namespace SliderTest {
	/// <summary>
	/// Summary for Form1
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	//public ref class FormHardwareConst : public System::Windows::Forms::Form {
	public ref class FormHardwareConst : public FormRating {

	public:
		FormHardwareConst(SliderTest::GeneralSettings^ gs):FormRating(gs)
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
			samplingInterval = gs->samplingInterval;

			//#ifdef TESTMODE
			//			// check out if this is useful here at all (not only for softwareslider)
			//			sw = gcnew System::Diagnostics::Stopwatch();
			//#endif

			timer1->Start();

			if(controller->generalSettings->rmtype == RatingMethodType::Standard)
				labelPosition->Text = System::String::Format("Item: {0}/{1}", controller->GetVideoPos() + 1, controller->GetTotalVideoNumber());
			else
				labelPosition->Text = System::String::Format("Item: {0}", controller->GetItemsPlayed()+1);

			labelMessage->Text = resman->GetString("HWconst_proceed",userculture);
			labelMessage->Visible = true;

			stopNext = gcnew System::Diagnostics::Stopwatch();
			stopNext->Reset();
			stopNext->Start();
		}

	protected:
	/// <summary>
	/// Clean up any resources being used.
	/// </summary>
	~FormHardwareConst()
	{
		if (components)
		{
			delete components;
		}
	}

	private: System::Windows::Forms::Timer^  timer1;
	private: System::Windows::Forms::TrackBar^  trackBar1;
	private: System::ComponentModel::IContainer^  components;
	private: System::Windows::Forms::RadioButton^  radioButton1;
	private: System::Windows::Forms::Button^  btnQuit;
	private: System::Windows::Forms::Label^  labelPosition;
	private: System::Windows::Forms::TextBox^  textBoxFormat;
	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::TextBox^  textBox2;
	private: System::Windows::Forms::CheckBox^  checkIntScale;
	private: System::Windows::Forms::GroupBox^  boxSlider;
	System::Diagnostics::Stopwatch^ stopNext;
	private: System::Windows::Forms::Label^  labelMessage;

	int waitTimeRating;
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
			this->timer1 = (gcnew System::Windows::Forms::Timer(this->components));
			this->trackBar1 = (gcnew System::Windows::Forms::TrackBar());
			this->radioButton1 = (gcnew System::Windows::Forms::RadioButton());
			this->btnQuit = (gcnew System::Windows::Forms::Button());
			this->labelPosition = (gcnew System::Windows::Forms::Label());
			this->textBoxFormat = (gcnew System::Windows::Forms::TextBox());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->textBox2 = (gcnew System::Windows::Forms::TextBox());
			this->checkIntScale = (gcnew System::Windows::Forms::CheckBox());
			this->boxSlider = (gcnew System::Windows::Forms::GroupBox());
			this->labelMessage = (gcnew System::Windows::Forms::Label());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->trackBar1))->BeginInit();
			this->boxSlider->SuspendLayout();
			this->SuspendLayout();
			// 
			// timer1
			// 
			this->timer1->Interval = 10;
			this->timer1->Tick += gcnew System::EventHandler(this, &FormHardwareConst::timer1_Tick);
			// 
			// trackBar1
			// 
			this->trackBar1->Location = System::Drawing::Point(21, 25);
			this->trackBar1->Maximum = 100;
			this->trackBar1->Name = L"trackBar1";
			this->trackBar1->Size = System::Drawing::Size(578, 42);
			this->trackBar1->TabIndex = 1;
			this->trackBar1->TickFrequency = 10;
			// 
			// radioButton1
			// 
			this->radioButton1->AutoSize = true;
			this->radioButton1->Location = System::Drawing::Point(185, 23);
			this->radioButton1->Name = L"radioButton1";
			this->radioButton1->Size = System::Drawing::Size(84, 17);
			this->radioButton1->TabIndex = 2;
			this->radioButton1->TabStop = true;
			this->radioButton1->Text = L"Slider button";
			this->radioButton1->UseVisualStyleBackColor = true;
			this->radioButton1->Visible = false;
			// 
			// btnQuit
			// 
			this->btnQuit->Location = System::Drawing::Point(185, 258);
			this->btnQuit->Name = L"btnQuit";
			this->btnQuit->Size = System::Drawing::Size(75, 26);
			this->btnQuit->TabIndex = 3;
			this->btnQuit->Text = L"Quit";
			this->btnQuit->UseVisualStyleBackColor = true;
			this->btnQuit->Visible = false;
			this->btnQuit->Click += gcnew System::EventHandler(this, &FormHardwareConst::btnQuit_Click);
			// 
			// labelPosition
			// 
			this->labelPosition->AutoSize = true;
			this->labelPosition->Location = System::Drawing::Point(23, 24);
			this->labelPosition->Name = L"labelPosition";
			this->labelPosition->Size = System::Drawing::Size(43, 13);
			this->labelPosition->TabIndex = 9;
			this->labelPosition->Text = L"Item:";
			this->labelPosition->TextAlign = System::Drawing::ContentAlignment::MiddleRight;
			// 
			// textBoxFormat
			// 
			this->textBoxFormat->Location = System::Drawing::Point(21, 258);
			this->textBoxFormat->Name = L"textBoxFormat";
			this->textBoxFormat->Size = System::Drawing::Size(100, 20);
			this->textBoxFormat->TabIndex = 15;
			this->textBoxFormat->Text = L"%d %m %r";
			this->textBoxFormat->Visible = false;
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(20, 240);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(63, 13);
			this->label2->TabIndex = 16;
			this->label2->Text = L"Log format: ";
			this->label2->Visible = false;
			// 
			// textBox2
			// 
			this->textBox2->BackColor = System::Drawing::SystemColors::ActiveBorder;
			this->textBox2->BorderStyle = System::Windows::Forms::BorderStyle::None;
			this->textBox2->Enabled = false;
			this->textBox2->Location = System::Drawing::Point(21, 284);
			this->textBox2->Multiline = true;
			this->textBox2->Name = L"textBox2";
			this->textBox2->Size = System::Drawing::Size(119, 40);
			this->textBox2->TabIndex = 18;
			this->textBox2->Text = L"%d date\r\n%m seq name\r\n%r rating";
			this->textBox2->Visible = false;
			// 
			// checkIntScale
			// 
			this->checkIntScale->AutoSize = true;
			this->checkIntScale->Location = System::Drawing::Point(185, 387);
			this->checkIntScale->Name = L"checkIntScale";
			this->checkIntScale->Size = System::Drawing::Size(87, 17);
			this->checkIntScale->TabIndex = 24;
			this->checkIntScale->Text = L"Integer scale";
			this->checkIntScale->UseVisualStyleBackColor = true;
			this->checkIntScale->Visible = false;
			// 
			// boxSlider
			// 
			this->boxSlider->Controls->Add(this->trackBar1);
			this->boxSlider->Location = System::Drawing::Point(12, 90);
			this->boxSlider->Name = L"boxSlider";
			this->boxSlider->Size = System::Drawing::Size(626, 77);
			this->boxSlider->TabIndex = 25;
			this->boxSlider->TabStop = false;
			this->boxSlider->Visible = false;
			// 
			// labelMessage
			// 
			this->labelMessage->AutoSize = true;
			this->labelMessage->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->labelMessage->Location = System::Drawing::Point(26, 65);
			this->labelMessage->Name = L"labelMessage";
			this->labelMessage->Size = System::Drawing::Size(35, 13);
			this->labelMessage->TabIndex = 26;
			this->labelMessage->Text = L"";
			//this->labelMessage->TextAlign = System::Drawing::ContentAlignment::MiddleRight;
			// 
			// FormHardwareConst
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			//this->ClientSize = System::Drawing::Size(193, 70);
			this->ClientSize = System::Drawing::Size(292, 273);
			this->ControlBox = false;
			this->Controls->Add(this->labelMessage);
			this->Controls->Add(this->boxSlider);
			this->Controls->Add(this->checkIntScale);
			this->Controls->Add(this->textBox2);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->textBoxFormat);
			this->Controls->Add(this->labelPosition);
			this->Controls->Add(this->btnQuit);
			this->Controls->Add(this->radioButton1);
			this->KeyPreview = true;
			this->Name = L"FormHardwareConst";
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterScreen;
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->trackBar1))->EndInit();
			this->boxSlider->ResumeLayout(false);
			this->boxSlider->PerformLayout();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion

private: System::Void timer1_Tick(System::Object^  sender, System::EventArgs^  e) {
	if(controller->endplay == true)
	{
		controller->endplay = false;	// set this! (because timer is still running)
		PlayingDoneCallback();
	}

	// Store current running values
	controller->storeSliderValues(); // ok, because capturing==false

	// More than 1 slider ? Get the button state
	bool next = false;
	if (controller->generalSettings->listID->Count > 1)
		next = (stopNext->ElapsedMilliseconds > waitTimeRating);// || controller->isNextItemAuto() ; ?check so in HWend.
	else
		next = controller->GetSliderButton();

	// Playback stopped and Button Pressed ?
	if (next && (controller->getPlayStatus()==false)) 
	{
		// wait for unpressing of button
		while(controller->GetSliderButton() == true)
			System::Threading::Thread::Sleep(50);

		controller->logStoredValues();
		timer1->Interval = samplingInterval;
		controller->PlayNext();
	}
}

private: System::Void btnQuit_Click(System::Object^  sender, System::EventArgs^  e) {
	quitAppFromUser();
}
// backgroundworker delegate, called after playing
//void PlayingDone(Object^ /*sender*/, System::ComponentModel::RunWorkerCompletedEventArgs^ e) {
void PlayingDoneCallback(void)
{
	controller->StopCapture();
	controller->StopLogging(controller->getLastItemName());//Check! is this necessary? only for endrating?
	timer1->Stop();//safety
	if(controller->generalSettings->rmtype == RatingMethodType::Standard)
	{
		// TODO: incremented too early/late if not threaded...or so....the usual problem
		int cPos = controller->GetVideoPos() + 1;
		int tPos = controller->GetTotalVideoNumber();
		if (cPos > tPos) cPos = tPos;// videonumber after final playback should stay within bounds
		labelPosition->Text = System::String::Format("Item: {0}/{1}", cPos, tPos);
	}
	else
		labelPosition->Text = System::String::Format("Item: {0}", controller->GetItemsPlayed());
	stopNext->Reset();
	stopNext->Start();
	//labelMessage->Visible = true;
	timer1->Interval = 10;
	timer1->Start();
}
};
}