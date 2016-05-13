#pragma once

//#include "Controller.h"
//#include "Logging.h"
#include "FormRating.h"

namespace SliderTest {

	/// <summary>
	/// Summary for FormP800
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class FormP800 : public FormRating
	{
	public:
		//FormP800(void)
		FormP800(SliderTest::GeneralSettings^ gs):FormRating(gs)
		{

			this->systemculture = gs->systemculture;
			this->userculture = gs->userculture;

			InitializeComponent();

			if (!gs->repeat)
				btnRepeat->Visible = false;
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~FormP800()
		{
			if (components)
			{
				delete components;
			}
		}

	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::Label^  label3;
	private: System::Windows::Forms::Label^  label4;
	private: System::Windows::Forms::Label^  label5;
	private: System::Windows::Forms::Button^  btnNext;
	private: System::Windows::Forms::Label^  label6;
	private: System::Windows::Forms::GroupBox^  groupBox1;
	
	private: System::Windows::Forms::RadioButton^  radioButton5;
	private: System::Windows::Forms::RadioButton^  radioButton1;
	private: System::Windows::Forms::RadioButton^  radioButton2;
	private: System::Windows::Forms::RadioButton^  radioButton3;
	private: System::Windows::Forms::RadioButton^  radioButton4;
	private: System::Windows::Forms::Label^  labelPosition;
	private: System::Windows::Forms::Button^  btnRepeat;

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->label5 = (gcnew System::Windows::Forms::Label());
			this->btnNext = (gcnew System::Windows::Forms::Button());
			this->label6 = (gcnew System::Windows::Forms::Label());
			this->groupBox1 = (gcnew System::Windows::Forms::GroupBox());
			this->radioButton1 = (gcnew System::Windows::Forms::RadioButton());
			this->radioButton2 = (gcnew System::Windows::Forms::RadioButton());
			this->radioButton3 = (gcnew System::Windows::Forms::RadioButton());
			this->radioButton4 = (gcnew System::Windows::Forms::RadioButton());
			this->radioButton5 = (gcnew System::Windows::Forms::RadioButton());
			this->labelPosition = (gcnew System::Windows::Forms::Label());
			this->btnRepeat = (gcnew System::Windows::Forms::Button());
			this->groupBox1->SuspendLayout();
			this->SuspendLayout();
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->label1->Location = System::Drawing::Point(26, 102);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(57, 13);
			this->label1->TabIndex = 5;
			this->label1->Text = L"Schlecht";
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->label2->Location = System::Drawing::Point(136, 102);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(45, 13);
			this->label2->TabIndex = 6;
			this->label2->Text = L"Dürftig";
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->label3->Location = System::Drawing::Point(208, 102);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(99, 13);
			this->label3->TabIndex = 7;
			this->label3->Text = L"Durchschnittlich";
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->label4->Location = System::Drawing::Point(355, 102);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(27, 13);
			this->label4->TabIndex = 8;
			this->label4->Text = L"Gut";
			// 
			// label5
			// 
			this->label5->AutoSize = true;
			this->label5->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->label5->Location = System::Drawing::Point(418, 102);
			this->label5->Name = L"label5";
			this->label5->Size = System::Drawing::Size(90, 13);
			this->label5->TabIndex = 9;
			this->label5->Text = L"Ausgezeichnet";
			// 
			// btnNext
			// 
			this->btnNext->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->btnNext->Location = System::Drawing::Point(443, 235);
			this->btnNext->Name = L"btnNext";
			this->btnNext->Size = System::Drawing::Size(85, 33);
			this->btnNext->TabIndex = 10;
			this->btnNext->Text = L"Weiter";
			this->btnNext->UseVisualStyleBackColor = true;
			this->btnNext->Click += gcnew System::EventHandler(this, &FormP800::btnNext_Click);
			// 
			// label6
			// 
			this->label6->AutoSize = true;
			this->label6->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->label6->Location = System::Drawing::Point(191, 22);
			this->label6->Name = L"label6";
			this->label6->Size = System::Drawing::Size(137, 13);
			this->label6->TabIndex = 11;
			this->label6->Text = L"Bewertung der Qualität";
			// 
			// groupBox1
			// 
			this->groupBox1->Controls->Add(this->radioButton1);
			this->groupBox1->Controls->Add(this->radioButton2);
			this->groupBox1->Controls->Add(this->radioButton3);
			this->groupBox1->Controls->Add(this->radioButton4);
			this->groupBox1->Controls->Add(this->radioButton5);
			this->groupBox1->Controls->Add(this->label5);
			this->groupBox1->Controls->Add(this->label4);
			this->groupBox1->Controls->Add(this->label3);
			this->groupBox1->Controls->Add(this->label1);
			this->groupBox1->Controls->Add(this->label2);
			this->groupBox1->Location = System::Drawing::Point(12, 64);
			this->groupBox1->Name = L"groupBox1";
			this->groupBox1->Size = System::Drawing::Size(516, 151);
			this->groupBox1->TabIndex = 12;
			this->groupBox1->TabStop = false;
			// 
			// radioButton1
			// 
			this->radioButton1->Appearance = System::Windows::Forms::Appearance::Button;
			this->radioButton1->FlatStyle = System::Windows::Forms::FlatStyle::System;
			this->radioButton1->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->radioButton1->Location = System::Drawing::Point(442, 38);
			this->radioButton1->Name = L"radioButton1";
			this->radioButton1->Size = System::Drawing::Size(40, 40);
			this->radioButton1->TabIndex = 17;
			this->radioButton1->TabStop = true;
			this->radioButton1->Text = L"1";
			this->radioButton1->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			this->radioButton1->UseVisualStyleBackColor = true;
			this->radioButton1->CheckedChanged += gcnew System::EventHandler(this, &FormP800::radioButton1_CheckedChanged);
			// 
			// radioButton2
			// 
			this->radioButton2->Appearance = System::Windows::Forms::Appearance::Button;
			this->radioButton2->FlatStyle = System::Windows::Forms::FlatStyle::System;
			this->radioButton2->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->radioButton2->Location = System::Drawing::Point(348, 38);
			this->radioButton2->Name = L"radioButton2";
			this->radioButton2->Size = System::Drawing::Size(40, 40);
			this->radioButton2->TabIndex = 16;
			this->radioButton2->TabStop = true;
			this->radioButton2->Text = L"2";
			this->radioButton2->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			this->radioButton2->UseVisualStyleBackColor = true;
			this->radioButton2->CheckedChanged += gcnew System::EventHandler(this, &FormP800::radioButton2_CheckedChanged);
			// 
			// radioButton3
			// 
			this->radioButton3->Appearance = System::Windows::Forms::Appearance::Button;
			this->radioButton3->FlatStyle = System::Windows::Forms::FlatStyle::System;
			this->radioButton3->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->radioButton3->Location = System::Drawing::Point(237, 38);
			this->radioButton3->Name = L"radioButton3";
			this->radioButton3->Size = System::Drawing::Size(40, 40);
			this->radioButton3->TabIndex = 15;
			this->radioButton3->TabStop = true;
			this->radioButton3->Text = L"3";
			this->radioButton3->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			this->radioButton3->UseVisualStyleBackColor = true;
			this->radioButton3->CheckedChanged += gcnew System::EventHandler(this, &FormP800::radioButton3_CheckedChanged);
			// 
			// radioButton4
			// 
			this->radioButton4->Appearance = System::Windows::Forms::Appearance::Button;
			this->radioButton4->FlatStyle = System::Windows::Forms::FlatStyle::System;
			this->radioButton4->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->radioButton4->Location = System::Drawing::Point(138, 38);
			this->radioButton4->Name = L"radioButton4";
			this->radioButton4->Size = System::Drawing::Size(40, 40);
			this->radioButton4->TabIndex = 14;
			this->radioButton4->TabStop = true;
			this->radioButton4->Text = L"4";
			this->radioButton4->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			this->radioButton4->UseVisualStyleBackColor = true;
			this->radioButton4->CheckedChanged += gcnew System::EventHandler(this, &FormP800::radioButton4_CheckedChanged);
			// 
			// radioButton5
			// 
			this->radioButton5->Appearance = System::Windows::Forms::Appearance::Button;
			this->radioButton5->FlatStyle = System::Windows::Forms::FlatStyle::System;
			this->radioButton5->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->radioButton5->Location = System::Drawing::Point(35, 38);
			this->radioButton5->Name = L"radioButton5";
			this->radioButton5->Size = System::Drawing::Size(40, 40);
			this->radioButton5->TabIndex = 13;
			this->radioButton5->TabStop = true;
			this->radioButton5->Text = L"5";
			this->radioButton5->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			this->radioButton5->UseVisualStyleBackColor = true;
			this->radioButton5->CheckedChanged += gcnew System::EventHandler(this, &FormP800::radioButton5_CheckedChanged);
			// 
			// labelPosition
			// 
			this->labelPosition->AutoSize = true;
			this->labelPosition->Location = System::Drawing::Point(12, 245);
			this->labelPosition->Name = L"labelPosition";
			this->labelPosition->Size = System::Drawing::Size(43, 13);
			this->labelPosition->TabIndex = 13;
			this->labelPosition->Text = L"Item:";
			this->labelPosition->TextAlign = System::Drawing::ContentAlignment::MiddleRight;
			// 
			// btnRepeat
			// 
			this->btnRepeat->Enabled = false;
			this->btnRepeat->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->btnRepeat->Location = System::Drawing::Point(341, 235);
			this->btnRepeat->Name = L"btnRepeat";
			this->btnRepeat->Size = System::Drawing::Size(85, 33);
			this->btnRepeat->TabIndex = 14;
			this->btnRepeat->Text = L"Nochmal";
			this->btnRepeat->UseVisualStyleBackColor = true;
			this->btnRepeat->Click += gcnew System::EventHandler(this, &FormP800::btnRepeat_Click);
			// 
			// FormP800
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(540, 284);
			this->Controls->Add(this->btnRepeat);
			this->Controls->Add(this->labelPosition);
			this->Controls->Add(this->groupBox1);
			this->Controls->Add(this->label6);
			this->Controls->Add(this->btnNext);
			this->Name = L"FormP800";
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterScreen;
			this->Text = L"FormP800";
			this->Shown += gcnew System::EventHandler(this, &FormP800::FormP800_Shown);
			this->groupBox1->ResumeLayout(false);
			this->groupBox1->PerformLayout();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion

private: System::Void btnNext_Click(System::Object^  sender, System::EventArgs^  e) {
	// FIXME: fragt autoitem nicht ab
	float val = -1;
	if (radioButton1->Checked) val = 1;
	else if (radioButton2->Checked) val = 2;
	else if (radioButton3->Checked) val = 3;
	else if (radioButton4->Checked) val = 4;
	else if (radioButton5->Checked) val = 5;
	System::Collections::Generic::List<float>^ l = gcnew System::Collections::Generic::List<float>;
	l->Add(val);
	controller->LogValueList(l);

	disableRating();
	btnNext->Enabled = false;
	btnRepeat->Enabled = false;
	controller->StartLogging();
	controller->PlayNext();
	
	if(controller->generalSettings->rmtype == RatingMethodType::Standard)
		labelPosition->Text = System::String::Format("Position: {0}/{1}", controller->GetVideoPos() + 1, controller->GetTotalVideoNumber());
	else
		labelPosition->Text = System::String::Format("Item: {0}", controller->GetItemsPlayed());

	if (!controller->isLastItemAuto()) {
		enableRating();
		btnRepeat->Enabled = true;
	}
	else {
		btnNext->Enabled = true;
		disableRating();
	}
}

private: void enableRating() {
	groupBox1->Enabled = true;
	radioButton1->Checked = false;
	radioButton2->Checked = false;
	radioButton3->Checked = false;
	radioButton4->Checked = false;
	radioButton5->Checked = false;
}

private: void disableRating() {
	groupBox1->Enabled = false;
	radioButton1->Checked = false;
	radioButton2->Checked = false;
	radioButton3->Checked = false;
	radioButton4->Checked = false;
	radioButton5->Checked = false;
}

private: System::Void radioButton5_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
	btnNext->Enabled = true;
}
private: System::Void radioButton4_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
	btnNext->Enabled = true;
}
private: System::Void radioButton3_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
	btnNext->Enabled = true;
}
private: System::Void radioButton2_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
	btnNext->Enabled = true;
}
private: System::Void radioButton1_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
	btnNext->Enabled = true;
}
private: System::Void FormP800_Shown(System::Object^  sender, System::EventArgs^  e) {
	if(controller->generalSettings->rmtype == RatingMethodType::Standard)
		labelPosition->Text = System::String::Format("Position: {0}/{1}", controller->GetVideoPos() + 1, controller->GetTotalVideoNumber());
	else
		labelPosition->Text = System::String::Format("Item: {0}", controller->GetItemsPlayed());
	 disableRating();
}
private: System::Void btnRepeat_Click(System::Object^  sender, System::EventArgs^  e) {
	disableRating();
	btnRepeat->Enabled = false;
	controller->StartLogging();
	controller->PlayCurrent();
	
	enableRating();
	btnRepeat->Enabled = true;
}
};
}
