#pragma once

//#include "Logging.h"
//#include "Playlist.h"
//#include "Controller.h"
//#include "MouseHook.h"
#include "FormRating.h"

namespace SliderTest {

	/// <summary>
	/// Summary for FormOnVideoRating
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>

	public ref class FormOnVideoRating : public FormRating
	{

	public: 
	private:
		//System::ComponentModel::BackgroundWorker^  backgroundWorker1;
		System::Windows::Forms::GroupBox^  groupBox1;
	private: System::Windows::Forms::Timer^  timer1;
			 MouseHook^ myHook;
	

	public:
		//FormOnVideoRating(void)
		FormOnVideoRating(SliderTest::GeneralSettings^ gs):FormRating(gs)
		{

			this->systemculture = gs->systemculture;
			this->userculture = gs->userculture;

			InitializeComponent();

			initScales();

			myHook = gcnew MouseHook(true, false);
			myHook->clickHandler += gcnew System::EventHandler(this, &SliderTest::FormOnVideoRating::HandleClick);

			samplingInterval = gs->samplingInterval;
			timer1->Start();
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~FormOnVideoRating()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Button^  btnPlay;
	protected: 

	private: System::Windows::Forms::Label^  label1;

	private: System::Windows::Forms::Label^  label2;
	private: System::ComponentModel::IContainer^  components;
	protected: 

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>

	int videoWidth;
	int videoHeight;
	int videoLeft;
	int videoTop;
	int samplingInterval;
	
#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			this->btnPlay = (gcnew System::Windows::Forms::Button());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->groupBox1 = (gcnew System::Windows::Forms::GroupBox());
			this->timer1 = (gcnew System::Windows::Forms::Timer(this->components));
			this->groupBox1->SuspendLayout();
			this->SuspendLayout();
			// 
			// btnPlay
			// 
			this->btnPlay->Location = System::Drawing::Point(12, 20);
			this->btnPlay->Name = L"btnPlay";
			this->btnPlay->Size = System::Drawing::Size(75, 23);
			this->btnPlay->TabIndex = 0;
			this->btnPlay->Text = L"Weiter";
			this->btnPlay->UseVisualStyleBackColor = true;
			this->btnPlay->Click += gcnew System::EventHandler(this, &FormOnVideoRating::button1_Click);
			// 
			// timer1
			// 
			this->timer1->Interval = 10;
			this->timer1->Tick += gcnew System::EventHandler(this, &FormOnVideoRating::timer1_Tick);
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(12, 54);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(35, 13);
			this->label1->TabIndex = 1;
			this->label1->Text = L"label1";
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(12, 76);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(35, 13);
			this->label2->TabIndex = 2;
			this->label2->Text = L"label2";
			// 
			// groupBox1
			// 
			this->groupBox1->Controls->Add(this->btnPlay);
			this->groupBox1->Controls->Add(this->label2);
			this->groupBox1->Controls->Add(this->label1);
			this->groupBox1->Location = System::Drawing::Point(5, 4);
			this->groupBox1->Name = L"groupBox1";
			this->groupBox1->Size = System::Drawing::Size(100, 100);
			this->groupBox1->TabIndex = 3;
			this->groupBox1->TabStop = false;
			// 
			// FormOnVideoRating
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->BackColor = System::Drawing::SystemColors::Control;
			this->ClientSize = System::Drawing::Size(110, 109);
			this->ControlBox = false;
			this->Controls->Add(this->groupBox1);
			this->Name = L"FormOnVideoRating";
			this->StartPosition = System::Windows::Forms::FormStartPosition::Manual;
			this->Text = L"OnVideoRating";
			this->TransparencyKey = System::Drawing::Color::Red;
			this->groupBox1->ResumeLayout(false);
			this->groupBox1->PerformLayout();
			this->ResumeLayout(false);



		}
#pragma endregion

	void initScales();

	private: System::Void button1_Click(System::Object^  sender, System::EventArgs^  e) {

		// timer not checked (tODO)
		timer1->Stop();
		timer1->Interval = samplingInterval;
	
		if (controller->getPlayStatus() == false) {
			if (!controller->isNextItemAuto()) {
				installMouseHook();//check if needed this way
			}
			btnPlay->Enabled = false;

			timer1->Start();
			controller->PlayNext();
		}
	}

	void PlayingDoneCallback(void) {
		timer1->Interval = 10;
		btnPlay->Enabled = true;
		removeMouseHook();
	}


	void HandleClick(Object^ /*sender*/, System::EventArgs^ e) {
		System::Windows::Forms::MouseEventArgs^ me = (System::Windows::Forms::MouseEventArgs^) e;
		int x = me->X;
		int y = me->Y;
		this->label1->Text = System::Convert::ToString(x) + " " +
			 System::Convert::ToString(y) + " " +
			 System::Convert::ToString("LMB");

		// convert the mouse click to a tuple of two values between 0 and 1
		// origin is lower left corner of the video
		float rateX = (float)(x - videoLeft);
		float rateY = (float)(y - videoTop);
		rateX /= (float)videoWidth;
		rateY /= (float)videoHeight;

		if (rateX < 0) rateX = 0;
		else if (rateX > 1) rateX = 1;

		if (rateY < 0) rateY = 0;
		else if (rateY > 1) rateY = 1;

		rateY = 1 - rateY;
	}
		
		// TODO: store values in the controller together with the time of the click
		// seems not so easy to do, since the controller logs everything with one time
		// so either don't store here but log directly or don't log the time but an offset
		// of the start time (i.e. when the play button was pressed, not very good)
		// ----------------------

	private: System::Void timer1_Tick(System::Object^  sender, System::EventArgs^  e) {
	//  TODO: check the timer implementation (when is controller storing slidervals

		if(controller->endplay == true)
		{
			controller->endplay = false;	// set this! (because timer is still running)
			PlayingDoneCallback();
		}
	}
};
}