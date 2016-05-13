#pragma once

#include "FormRating.h"
#include "PlayButtonPannel.h"


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
public ref class FormSoftwareEnd : public FormRating
{

public:
	FormSoftwareEnd(GeneralSettings ^gs, System::Windows::Forms::Orientation orient):FormRating(gs)
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
		~FormSoftwareEnd()
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
		System::Windows::Forms::Button^ btnPlay;
		System::Windows::Forms::Timer^  timerWhileVideoPlaying;
		System::ComponentModel::IContainer^  components;
		VolumeForm^ volumeForm;
		System::Collections::Generic::List<RatingGroup^>^ listSliders;
		System::Windows::Forms::Label^ labelPosition;
		System::Windows::Forms::Label^ labelNameSeqUnderEval;
		System::Windows::Forms::Button^  btnRepeat;
		System::Windows::Forms::Button ^ btnNextSerie;
		System::Diagnostics::Stopwatch^ stopwatch;
		float scaleX;
		float scaleY;
		int lastPlayButtonID;
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
			lastPlayButtonID = 0;
			btnNextSerie = nullptr;
			
			System::Drawing::Font^ stdFont = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", scaleX*8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->Font = stdFont;
			this->components = (gcnew System::ComponentModel::Container());
			this->btnPlay = (gcnew PlayButtonPannel());
			this->labelPosition = (gcnew System::Windows::Forms::Label());
			this->timerWhileVideoPlaying = (gcnew System::Windows::Forms::Timer(this->components));
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
			this->btnPlay->Click += gcnew System::EventHandler(this, &FormSoftwareEnd::btnPlay_Click);
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
			// label name sequence
			// 
			if(controller->generalSettings->showSequenceName != 0) {
				this->labelNameSeqUnderEval = gcnew System::Windows::Forms::Label();
				this->labelNameSeqUnderEval->Location = System::Drawing::Point(int(10), int(10));
				this->Controls->Add(this->labelNameSeqUnderEval);
			}

			// 
			// timerWhileVideoPlaying
			// 
			this->timerWhileVideoPlaying->Interval = 10;
			this->timerWhileVideoPlaying->Tick += gcnew System::EventHandler(this, &FormSoftwareEnd::timerWhileVideoPlaying_Tick);
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
			this->btnRepeat->Click += gcnew System::EventHandler(this, &FormSoftwareEnd::btnRepeat_Click);
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
			this->quitbutton->Click += gcnew System::EventHandler(this, &FormSoftwareEnd::quitbutton_Click);
			#endif
			// 
			// FormSoftwareEnd
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
			this->Controls->Add(this->btnPlay);
			this->Controls->Add(this->labelPosition);
			this->KeyPreview = true;
			this->Name = L"FormSoftwareEnd";
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterScreen;
			this->Load += gcnew System::EventHandler(this, &FormSoftwareEnd::FormSoftwareEnd_Load);
			this->Shown += gcnew System::EventHandler(this, &FormSoftwareEnd::FormSoftwareEnd_Shown);
			this->FormClosed += gcnew System::Windows::Forms::FormClosedEventHandler(this, &FormSoftwareEnd::FormSoftwareEnd_FormClosed);
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
	sliderChangedDelegate^ sDelegate = gcnew sliderChangedDelegate(this, &FormSoftwareEnd::sliderChanged);

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
	if(listSliders->Count != 0)
	{
		this->Width = 20 + listSliders[listSliders->Count - 1]->Left + listSliders[listSliders->Count - 1]->Width;
		this->Height = 70 + listSliders[listSliders->Count - 1]->Top + listSliders[listSliders->Count - 1]->Height;
	}
	else // if there are not rating groups defined in the xml settings file
	{
		this->Width = 600;
		this->Height = 250;

	}


	PlayButtonPannel^ buttonPannel = (PlayButtonPannel^) this->btnPlay;
	buttonPannel->setParentSize(this->Size);
	
	SuspendLayout();
	buttonPannel->Left = (this->Width - buttonPannel->Width) / 2;
	for(int i = 0 ; i < controller->GetNumberOfcomparedSequences() - 1 && (controller->generalSettings->rmtype == RatingMethodType::SAMVIQ) ; ++i) 
	{
		System::Windows::Forms::Button^ button = buttonPannel->AddButtonInPannel();
		button->Click += gcnew System::EventHandler(this, &FormSoftwareEnd::btnPlay_Click);
		this->Controls->Add(button);

		System::Windows::Forms::Label ^label = buttonPannel->getLabel(i+1);
		this->Controls->Add(label);

		if(button->Size.Width < 100)
		{
			this->Width += i * (100 - button->Size.Width);
			buttonPannel->setParentSize(this->Size);
			buttonPannel->updateButtonSize();

			int sizeSliders = 0;
			for(int k = 0 ; k < listSliders->Count ; ++k)
			{
				sizeSliders += listSliders[k]->Width;
			}
			int off = (this->Width - sizeSliders) / (listSliders->Count+1);
			
			sizeSliders = 0;
			for(int k = 0 ; k < listSliders->Count ; ++k)
			{
				int sz = listSliders[k]->Width;
				listSliders[k]->Location = System::Drawing::Point(off + sizeSliders, listSliders[k]->Top);
				sizeSliders += off + sz;
			}

		}
	}
	buttonPannel->disableAllButRef();

	if((controller->GetNumberOfcomparedSequences() > 1) && (controller->generalSettings->rmtype == RatingMethodType::SAMVIQ)) {
		btnNextSerie = gcnew System::Windows::Forms::Button();

		System::Windows::Forms::Label ^label = buttonPannel->getLabel(0);
		this->Controls->Add(label);

		btnNextSerie->Enabled = false;
		btnNextSerie->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", scaleX*12.F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
		btnNextSerie->Location = System::Drawing::Point(this->Width-int(scaleX*kNEXTBUTTONW)-5, this->Height-int(scaleY*kNEXTBUTTONH)-5);
		btnNextSerie->Name = L"btnPlay";
		btnNextSerie->Size = System::Drawing::Size(int(scaleX*kNEXTBUTTONW), int(scaleY*kNEXTBUTTONH));
		btnNextSerie->TabIndex = 10;
		btnNextSerie->UseVisualStyleBackColor = false;
		btnNextSerie->Text = L">>";
		btnNextSerie->UseVisualStyleBackColor = true;
		btnNextSerie->Click += gcnew System::EventHandler(this, &FormSoftwareEnd::btnNext_Serie);

		this->Controls->Add(btnNextSerie);
		controller->PreparePoolSequences();
	}

	ResumeLayout(false);
	PerformLayout();


	labelPosition->Top = this->Height - 90 - labelPosition->Height;
	labelPosition->Left = 10;
	
	btnRepeat->Top = this->Height - 35 - btnRepeat->Height;
	btnRepeat->Left = this->Width - 20 - btnRepeat->Width;

	if(labelNameSeqUnderEval != nullptr) {
		labelNameSeqUnderEval->Top = this->Height - 40 - labelPosition->Height;
		labelNameSeqUnderEval->Left = labelPosition->Right + 50;
	}
}




private: System::Void btnNext_Serie(System::Object^ sender, System::EventArgs^ e) 
{
	if (!controller->isLastItemAuto()) {
		controller->logLastValues();
		if (volumeForm != nullptr)
			controller->storeVolume(volumeForm->getVolume());
	}

	for each (RatingGroup^ rg in listSliders) {
		rg->Reset();
		rg->Enabled = false;
	}
 
	// volume form for audio training phase
	if (volumeForm != nullptr) {
		if (volumeForm->Visible)
			controller->setVolume(volumeForm->getVolume());
	}

	controller->PreparePoolSequences();
	btnNextSerie->Enabled = false;

	PlayButtonPannel ^buttonPannel = (PlayButtonPannel^)(btnPlay);
	buttonPannel->disableAllButRef();
	buttonPannel->clearComments();
	lastPlayButtonID = 0;

}


private: System::Void timerWhileVideoPlaying_Tick(System::Object^  sender, System::EventArgs^  e) {

	if(controller->endplay == true)
	{
		controller->endplay = false;	// set this! (because timer is still running)
		if(listSliders->Count == 0)
		{
			((PlayButtonPannel^) btnPlay)->setEnable(true);
			if(btnNextSerie != nullptr)
				btnNextSerie->Enabled = true;
		}
#ifndef _DEBUG
		enableMouse();
		if(!controller->disableClipping)
			clipMouse();
#endif
		if(controller->generalSettings->logKeypress)
		{
			controller->logStoredKeyValues();
		}
		timerWhileVideoPlaying->Stop();
	}
}

private: System::Void btnPlay_Click(System::Object^ sender, System::EventArgs^ e) 
{
	PlayVideoButton^ button = dynamic_cast<PlayVideoButton^>(sender);
	PlayButtonPannel ^buttonPannel = (PlayButtonPannel^)(btnPlay);

	if (!controller->isLastItemAuto()) {
		controller->logLastValues();
		
		if(controller->generalSettings->rmtype == RatingMethodType::SAMVIQ && lastPlayButtonID != 0) {
			buttonPannel->clearComments(lastPlayButtonID);
			for each (RatingGroup^ rg in listSliders) {
				if(rg->GetType() == SliderGroup::typeid) {
					SliderGroup ^sl = dynamic_cast<SliderGroup^>(rg);
					if(sl != nullptr)
						buttonPannel->addComment(lastPlayButtonID, sl->Text + ": " + System::Convert::ToString(static_cast<int>(sl->getSlider()->getValue())));
				}
				if(rg->GetType() == ButtonGroup::typeid) {
					ButtonGroup ^bt = dynamic_cast<ButtonGroup^>(rg);
					if(bt != nullptr) {
						System::Collections::Generic::List<System::Windows::Forms::RadioButton^>^ listButtons = bt->getButtons();
						System::String^ label;
						for(int i = 0 ; i < listButtons->Count ; ++i)
							if(listButtons[i]->Checked) {
								label = listButtons[i]->Text;
							}
						buttonPannel->addComment(lastPlayButtonID, bt->Text + ": " + label);
					}
				}
			}
		}

		if (volumeForm != nullptr)
			controller->storeVolume(volumeForm->getVolume());
	}

	if(button != nullptr) {
		controller->LogVideoIndex(button->videoIndx);
	}

	System::String ^arguments;
	for each (RatingGroup^ rg in listSliders) {
		if(rg->GetType() == SliderGroup::typeid) {
			SliderGroup ^sl = dynamic_cast<SliderGroup^>(rg);
			if(sl != nullptr)
				arguments += " " + System::Convert::ToString(sl->getSlider()->getValue());
		}
		if(rg->GetType() == ButtonGroup::typeid) {
			ButtonGroup ^bt = dynamic_cast<ButtonGroup^>(rg);
			if(bt != nullptr) {
				System::Collections::Generic::List<System::Windows::Forms::RadioButton^>^ listButtons = bt->getButtons();
				System::String^ label;
				for(int i = 0 ; i < listButtons->Count ; ++i)
					if(listButtons[i]->Checked) {
						label = listButtons[i]->Text;
					}
				arguments += " " + label;
			}
		}
	}

	for each (RatingGroup^ rg in listSliders) {
		rg->Reset();
		rg->Enabled = false;
	}

	// volume form for audio training phase
	if (volumeForm != nullptr) {
		if (volumeForm->Visible)
			controller->setVolume(volumeForm->getVolume());
	}

	if(!(controller->generalSettings->rmtype == RatingMethodType::SAMVIQ && button->videoIndx == 0))
		buttonPannel->setEnable(false);
	btnRepeat->Enabled = false;

	if(btnNextSerie != nullptr)
		btnNextSerie->Enabled = false;

	bool nextAuto = controller->isNextItemAuto();
	if (nextAuto && controller->generalSettings->rmtype != RatingMethodType::SAMVIQ) {
#ifndef _DEBUG
		unclipMouse();	// For auto item accessibility
#endif
	} else
	{
		controller->endplay = false;
		timerWhileVideoPlaying->Start();	// mouse gets clipped when video is over

#ifndef _DEBUG
		unclipMouse();		// was clipMouse() before. But together with mHook, it will be clipped-visible in VLC10
		disableMouse();		// and disable while video plays
#endif
	}

	if(button == nullptr || button->videoIndx == -1)
		controller->PlayNext();
	else
		controller->PlayOneNext(button->videoIndx);

	if(labelNameSeqUnderEval != nullptr) {
		labelNameSeqUnderEval->Text = controller->getLastItemName();
		int found = controller->getLastItemName()->LastIndexOf("\\");
		if(found == -1)
			labelNameSeqUnderEval->Text = controller->getLastItemName();
		else
			labelNameSeqUnderEval->Text = controller->getLastItemName()->Substring(found+1);

		labelNameSeqUnderEval->Width = this->Width - (labelPosition->Right + 50);
	}

	// if(nextAuto) clipMouse();// do NOT enable this. Still clipped when regularly quitted (eg nomoreitems). // reclip again, only for auto items
	if(controller->generalSettings->rmtype == RatingMethodType::Standard)
		labelPosition->Text = System::String::Format("Item: {0}/{1}", controller->GetVideoPos() + 1, controller->GetTotalVideoNumber());
	else if(controller->generalSettings->rmtype == RatingMethodType::SAMVIQ)
		labelPosition->Text = System::String::Format("Item: {0}/{1}", controller->GetPaneNumber(), controller->GetTotalVideoNumber());
	else
		labelPosition->Text = System::String::Format("Item: {0}", controller->GetItemsPlayed() );

	if (!controller->isLastItemAuto()) {
		btnRepeat->Enabled = true;
		for each (RatingGroup^ s in listSliders)
		{
			if(!(controller->generalSettings->rmtype == RatingMethodType::SAMVIQ && button->videoIndx == 0))
				s->makeRateable();
			else
				buttonPannel->setEnable(true);

			if(!( (controller->generalSettings->display == Display::Local) &&
				(controller->generalSettings->localPlayer == LocalPlayer::WMP) ))
				s->Focus();// This gives the focus, so that keypressing for exit typing works

			if(controller->generalSettings->rmtype == RatingMethodType::SAMVIQ && button->videoIndx == 0)
				if(controller->PoolEvaluated())
					if(btnNextSerie != nullptr)
						btnNextSerie->Enabled = true;
		}
	}
	else {
		buttonPannel->setEnable(true);
	}

	// volume form for audio training phase hide/show
	if (controller->needVolumeWindow() && volumeForm != nullptr) {
		volumeForm->Visible = true;
	}
	else if (volumeForm != nullptr) {
		volumeForm->Visible = false;
	}

	if(button != nullptr)
		lastPlayButtonID = button->videoIndx;
}

// callback, called by every slider that gets changed
// check if all sliders have been rated and if so, enable the playbutton
void sliderChanged() {
	for each (RatingGroup^ s in listSliders)
		if (!s->hasChanged()) return;

	((PlayButtonPannel^)(btnPlay))->setEnable(true);
	if(controller->PoolEvaluated())
		if(btnNextSerie != nullptr)
			btnNextSerie->Enabled = true;
}
private: System::Void FormSoftwareEnd_FormClosed(System::Object^  sender, FormClosedEventArgs^  e) { 
#ifndef _DEBUG
			unclipMouse();
#endif
		 }	
private: System::Void FormSoftwareEnd_Load(System::Object^  sender, System::EventArgs^  e) { 
	clipRect = System::Drawing::Rectangle(this->Bounds.Left + 4,this->Bounds.Top + 4,this->Bounds.Width - 8,this->Bounds.Height - 8);
		 }
private: System::Void FormSoftwareEnd_Shown(System::Object^  sender, System::EventArgs^  e) { 
	if (controller->generalSettings->volumeFormType == VolumeFormType::Slider )
		volumeForm = VolumeForm::sliderVolumeFactory(this->Bounds, userculture);
	else if (controller->generalSettings->volumeFormType == VolumeFormType::Button )
		volumeForm = VolumeForm::buttonVolumeFactory(this->Bounds,userculture);

	if(controller->generalSettings->rmtype == RatingMethodType::Standard)
		labelPosition->Text = System::String::Format("Item: {0}/{1}", controller->GetVideoPos() + 1, controller->GetTotalVideoNumber());
	else if(controller->generalSettings->rmtype == RatingMethodType::SAMVIQ)
		labelPosition->Text = System::String::Format("Item: {0}/{1}", controller->GetPaneNumber(), controller->GetTotalVideoNumber());
	else
		labelPosition->Text = System::String::Format("Item: {0}", controller->GetItemsPlayed()+1);

	if(kplayinitfile)
		controller->PlayInit();
}

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

#ifndef _DEBUG
	if (!nextAuto)
		disableMouse();
#endif

	controller->PlayCurrent();

#ifndef _DEBUG
	if (!nextAuto)
		enableMouse();
#endif

	btnRepeat->Enabled = true;
	for each (RatingGroup^ s in listSliders)
	{
		s->makeRateable();
		if(!( (controller->generalSettings->display == Display::Local) &&
			(controller->generalSettings->localPlayer == LocalPlayer::WMP) ))
			s->Focus();// This gives the focus, so that keypressing for exit typing works
	}
}

#ifdef USE_QUITBUTTON
private: System::Void quitbutton_Click(System::Object^  sender, System::EventArgs^  e) {
			quitAppFromUser();
		 }
#endif 
};
}
