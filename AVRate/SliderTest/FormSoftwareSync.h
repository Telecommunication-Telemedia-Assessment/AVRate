#pragma once

#include "FormRating.h"


namespace SliderTest {
	/// <summary>
	/// Summary for FormSoftwareSlider
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
public ref class FormSoftwareSync : public FormRating
{

public:
	FormSoftwareSync(GeneralSettings ^gs, System::Windows::Forms::Orientation orient):FormRating(gs)
	{
		this->systemculture = gs->systemculture;
		this->userculture = gs->userculture;

		// Set scaling factors from screen size
		scaleX = float(System::Windows::Forms::Screen::PrimaryScreen->Bounds.Right) / kSTDSCREENWIDTH;
		scaleY = float(System::Windows::Forms::Screen::PrimaryScreen->Bounds.Bottom) / kSTDSCREENHEIGHT;

		InitializeComponent();
		this->SetStyle(System::Windows::Forms::ControlStyles::SupportsTransparentBackColor, true);// no effect on transparency of lbels
		//this->TransparencyKey = System::Drawing::Color::Red;
		InitializeSelf(orient);

		timer1->Start();

		stopwatch = gcnew System::Diagnostics::Stopwatch();

		for each (RatingGroup^ s in listSliders) {
			AbstractSlider^ slider = nullptr;
			if (s->GetType() == SliderGroup::typeid)
				slider = gcnew SoftwareSlider(((SliderGroup^)s)->getSlider());
			else if (s->GetType() == ButtonGroup::typeid)
				slider = gcnew ButtonSlider(((ButtonGroup^)s)->getButtons());
			
			controller->setSlider(slider);

		}

		if (!gs->repeat)// not checked.
			btnRepeat->Visible = false;

		volumeForm = nullptr;
	}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~FormSoftwareSync()
		{
			if (components)
			{
				delete components;
			}
		}
	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::Windows::Forms::Button^  btnPlay;
		System::ComponentModel::IContainer^  components;
		VolumeForm^ volumeForm;
		System::Windows::Forms::Timer^  timer1;
		System::Collections::Generic::List<RatingGroup^>^ listSliders;
		System::Windows::Forms::Label^ labelPosition;
		System::Windows::Forms::Button^  btnRepeat;
		System::Diagnostics::Stopwatch^ stopwatch;
		float scaleX;
		float scaleY;
		#ifdef USE_QUITBUTTON
		System::Windows::Forms::Button^  quitbutton;
		#endif
#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			System::Drawing::Font^ stdFont = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", scaleX*8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->Font = stdFont;
			this->components = (gcnew System::ComponentModel::Container());
			this->timer1 = (gcnew System::Windows::Forms::Timer(this->components));
			this->btnPlay = (gcnew System::Windows::Forms::Button());
			this->labelPosition = (gcnew System::Windows::Forms::Label());
			this->btnRepeat = (gcnew System::Windows::Forms::Button());
			#ifdef USE_QUITBUTTON
			this->quitbutton = (gcnew System::Windows::Forms::Button());
			#endif
			this->SuspendLayout();
			// 
			// btnPlay
			// 14.25 orig
			this->btnPlay->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", scaleX*12.F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->btnPlay->Location = System::Drawing::Point(147, 19);
			this->btnPlay->Name = L"btnPlay";
			this->btnPlay->Size = System::Drawing::Size(int(scaleX*kPLAYBUTTONW), int(scaleY*kPLAYBUTTONH));
			this->btnPlay->TabIndex = 10;
			this->TabStop = false;
			this->btnPlay->Text = L"Play";
			this->btnPlay->UseVisualStyleBackColor = true;
			this->btnPlay->Click += gcnew System::EventHandler(this, &FormSoftwareSync::btnPlay_Click);
			// 
			// labelPosition
			// 
			this->labelPosition->AutoSize = true;
			this->labelPosition->Location = System::Drawing::Point(int(scaleX*80), int(scaleY*55));
			this->labelPosition->Name = L"labelPosition";
			this->labelPosition->Size = System::Drawing::Size(int(scaleX*43), int(scaleY*13));
			this->labelPosition->TabIndex = 11;
			this->labelPosition->Text = L"Item:";
			this->labelPosition->TextAlign = System::Drawing::ContentAlignment::MiddleRight;
			// 
			// timer1
			// 
			this->timer1->Interval = 10;
			this->timer1->Tick += gcnew System::EventHandler(this, &FormSoftwareSync::timer1_Tick);
			// 
			// btnRepeat
			// 
			this->btnRepeat->Enabled = false;
			this->btnRepeat->Location = System::Drawing::Point(int(scaleY*355), int(scaleY*19));
			this->btnRepeat->Name = L"btnRepeat";
			this->btnRepeat->Size = System::Drawing::Size(int(scaleX*75), int(scaleY*23));
			this->btnRepeat->TabIndex = 13;
			this->btnRepeat->Text = L"Nochmal";
			this->btnRepeat->UseVisualStyleBackColor = true;
			this->btnRepeat->Click += gcnew System::EventHandler(this, &FormSoftwareSync::btnRepeat_Click);
			#ifdef USE_QUITBUTTON
			// 
			// quitbutton
			// 
			this->quitbutton->Location = System::Drawing::Point(0, 1);
			this->quitbutton->Name = L"quitbutton";
			this->quitbutton->Size = System::Drawing::Size(29, 23);
			this->quitbutton->TabIndex = 14;
			this->quitbutton->Text = L"quit";
			this->quitbutton->UseVisualStyleBackColor = true;
			this->quitbutton->Click += gcnew System::EventHandler(this, &FormSoftwareSync::quitbutton_Click);
			#endif
			// 
			// FormSoftwareSync
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::None;// prevents autoscaling of Play button
			this->ClientSize = System::Drawing::Size(467, 520);
			//this->MinimumSize = this->ClientSize;
			//this->MaximumSize = this->ClientSize;
			this->ControlBox = false;
			#ifdef USE_QUITBUTTON
			this->Controls->Add(this->quitbutton);
			#endif
			this->Controls->Add(this->btnRepeat);
			this->Controls->Add(this->labelPosition);
			this->Controls->Add(this->btnPlay);
			this->KeyPreview = true;
			this->Name = L"FormSoftwareSync";
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterScreen;
			this->Load += gcnew System::EventHandler(this, &FormSoftwareSync::FormSoftwareSync_Load);
			this->Shown += gcnew System::EventHandler(this, &FormSoftwareSync::FormSoftwareSync_Shown);
			this->FormClosed += gcnew System::Windows::Forms::FormClosedEventHandler(this, &FormSoftwareSync::FormSoftwareSync_FormClosed);
			//this->FormClosing += gcnew System::Windows::Forms::FormClosingEventHandler(this, &FormSoftwareSync::FormSoftwareSync_FormClosing);
			this->btnRepeat->Focus();// This takes the focus from btnPlay, so theres no accidental keypressing
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
// init sliders and set the positions, resize the form afterwards
private:
void InitializeSelf(System::Windows::Forms::Orientation orient) {
	SettingsReader^ reader = gcnew SettingsReader(controller->generalSettings->settingsFile);

	listSliders = gcnew System::Collections::Generic::List<RatingGroup^>;

	// function to call, when a slider on the form is being changed
	sliderChangedDelegate^ sDelegate = gcnew sliderChangedDelegate(this, &FormSoftwareSync::sliderChanged);

	// generate the list of sliders/buttons from the XML and add them to the form	
	System::Collections::Generic::List<System::Collections::Generic::List<KeyValueSetting^>^>^ 
		raterList = reader->ReadSliderList();

	int count = 0;
	for each (System::Collections::Generic::List<KeyValueSetting^>^ rater in raterList) {
		RatingGroup^ rg = nullptr;
		if (rater[0]->key == "type" && rater[0]->value == "slider") {
			rg = gcnew SliderGroup(rater, count, sDelegate, orient, userculture);
		}			
		else if (rater[0]->key == "type" && rater[0]->value == "buttons") {
			rg = gcnew ButtonGroup(rater, count, sDelegate, orient, userculture);
		}
		
		listSliders->Add(rg);
		this->Controls->Add(rg);
		count++;
	}

	// fit the form to the amount of rating groups
	this->Width = 20 + listSliders[listSliders->Count - 1]->Left + listSliders[listSliders->Count - 1]->Width;
	this->Height = 70 + listSliders[listSliders->Count - 1]->Top + listSliders[listSliders->Count - 1]->Height;

	labelPosition->Top = this->Height - 40 - labelPosition->Height;
	labelPosition->Left = 10;
	btnPlay->Left = this->Width / 2 - btnPlay->Width / 2;
	btnRepeat->Top = this->Height - 35 - btnRepeat->Height;
	btnRepeat->Left = this->Width - 20 - btnRepeat->Width;
}
private: 
	System::Void timer1_Tick(System::Object^  sender, System::EventArgs^  e) {
		if(controller->endplay == true)
		{
			controller->endplay = false;	// set this! (because timer is still running)
			PlayingDoneCallback();
		}
	}
private:
	void PlayingDoneCallback(void) {
		//controller->StopCapture();
		controller->StopLogging(controller->getLastItemName());//Check! is this necessary? only for endrating?
		timer1->Interval = 10;//check

		if(controller->generalSettings->logKeypress)
		{
			controller->logStoredKeyValues();
		}

		enableMouse();
		if(!controller->disableClipping)
			clipMouse();

		if (!controller->isLastItemAuto()) 
		{
			btnRepeat->Enabled = true;
			for each (RatingGroup^ s in listSliders)
			{
				s->makeRateable();
				s->Focus();// gives focus, so that keypressing for exit typing works
			}
		}
		else 
		{
			btnPlay->Enabled = true;
		}

		//btnPlay->Focus();

		// volume form for audio training phase hide/show
		if (controller->needVolumeWindow() && volumeForm != nullptr) {
			volumeForm->Visible = true;
		}
		else if (volumeForm != nullptr) {
			volumeForm->Visible = false;
		}
	}

private: System::Void btnPlay_Click(System::Object^ sender, System::EventArgs^ e) 
{
	if (!controller->isLastItemAuto()) {
		controller->logLastValues();
		if (volumeForm != nullptr)
			controller->storeVolume(volumeForm->getVolume());
	}

	timer1->Stop();
	for each (RatingGroup^ rg in listSliders) {
		rg->Reset();
		rg->Enabled = false;
	}

	// volume form for audio training phase
	if (volumeForm != nullptr) {
		if (volumeForm->Visible)
			controller->setVolume(volumeForm->getVolume());
	}

	btnPlay->Enabled = false;
	btnRepeat->Enabled = false;

	bool nextAuto = controller->isNextItemAuto();
	if (nextAuto)
		unclipMouse();	// For auto item accessibility
	else
	{
		controller->endplay = false;
		timer1->Start();	// mouse gets clipped when video is over

		unclipMouse();		// was clipMouse() before. But together with mHook, it will be clipped-visible in VLC10
		disableMouse();		// and disable while video plays
	}

	controller->PlayNext();

	// if(nextAuto) clipMouse();// do NOT enable this. Still clipped when regularly quitted (eg nomoreitems). // reclip again, only for auto items
	if(controller->generalSettings->rmtype == RatingMethodType::Standard)
		labelPosition->Text = System::String::Format("Item: {0}/{1}", controller->GetVideoPos() + 1, controller->GetTotalVideoNumber());
	else
		labelPosition->Text = System::String::Format("Item: {0}", controller->GetItemsPlayed() );

}

// callback, called by every slider that gets changed
// check if all sliders have been rated and if so, enable the playbutton
void sliderChanged() {
	for each (RatingGroup^ s in listSliders)
		if (!s->hasChanged()) return;
	btnPlay->Enabled = true;
}
private: System::Void FormSoftwareSync_FormClosed(System::Object^  sender, FormClosedEventArgs^  e) { 
			unclipMouse();
		 }	
//private: System::Void FormSoftwareSync_FormClosing(System::Object^  sender, FormClosingEventArgs^  e) { 
//			unclipMouse();
//		 }	
private: System::Void FormSoftwareSync_Load(System::Object^  sender, System::EventArgs^  e) { 
	clipRect = System::Drawing::Rectangle(this->Bounds.Left + 4,this->Bounds.Top + 4,this->Bounds.Width - 8,this->Bounds.Height - 8);
		 }
private: System::Void FormSoftwareSync_Shown(System::Object^  sender, System::EventArgs^  e) { 
	if (controller->generalSettings->volumeFormType == VolumeFormType::Slider )
		volumeForm = VolumeForm::sliderVolumeFactory(this->Bounds, userculture);
	else if (controller->generalSettings->volumeFormType == VolumeFormType::Button )
		volumeForm = VolumeForm::buttonVolumeFactory(this->Bounds,userculture);

	if(controller->generalSettings->rmtype == RatingMethodType::Standard)
		//labelPosition->Text = System::String::Format("Item: 1 /{0}",  controller->GetTotalVideoNumber());
		labelPosition->Text = System::String::Format("Item: {0}/{1}", controller->GetVideoPos() + 1, controller->GetTotalVideoNumber());
	else
		//labelPosition->Text = System::String::Format("Item: 0");
		labelPosition->Text = System::String::Format("Item: {0}", controller->GetItemsPlayed()+1);

	// Clip mouse in general for the evaluation. Has to be before SW_shown because Unclip gets called too early in FRating when Plisterror
	// NO other way...has to be in SW_SHOWN or wont work.
	//clipRect = System::Drawing::Rectangle(this->Bounds.Left + 4,this->Bounds.Top + 4,this->Bounds.Width - 8,this->Bounds.Height - 8);
	//clipMouse();

	if(kplayinitfile)
		controller->PlayInit();
}

// NOT TESTED FOR FORMSOFTWARESYNC : (TODO)
private: System::Void btnRepeat_Click(System::Object^  sender, System::EventArgs^  e) {
	for each (RatingGroup^ s in listSliders) {
		s->Reset();
		s->Enabled = false;
	}
	
	btnRepeat->Enabled = false;

	// volume form for audio training phase
	if (volumeForm != nullptr) {
		if (volumeForm->Visible)
			controller->setVolume(volumeForm->getVolume());
	}
	
	bool nextAuto = controller->isNextItemAuto();
	if (!nextAuto)
		disableMouse();

	controller->PlayCurrent();

	if (!nextAuto)
		enableMouse();

	btnRepeat->Enabled = true;
	for each (RatingGroup^ s in listSliders)
	{
		s->makeRateable();
		s->Focus();// gives focus, so that keypressing for exit typing works
	}
}

#ifdef USE_QUITBUTTON
private: System::Void quitbutton_Click(System::Object^  sender, System::EventArgs^  e) {
			quitAppFromUser();
		 }
#endif 
};
}
