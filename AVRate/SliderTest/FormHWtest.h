#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;

#include "HardwareSlider.h"

namespace SliderTest {

	/// <summary>
	/// Summary for FormHWtest
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class FormHWtest : public System::Windows::Forms::Form
	{
	public:
		FormHWtest(void)
		{
			InitializeComponent();

			testHWslider1 = gcnew HardwareSlider(0, 0, 100);
			testHWslider2 = gcnew HardwareSlider(1, 0, 100);
			testHWslider3 = gcnew HardwareSlider(2, 0, 100);

			timerCheckHWdevice->Start();
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~FormHWtest()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::GroupBox^  groupBox1;
	private: System::Windows::Forms::GroupBox^  groupBox2;
	private: System::Windows::Forms::GroupBox^  groupBox3;
	private: System::Windows::Forms::Button^  button1;
	private: System::Windows::Forms::Label^  HWlabel1;
	private: System::Windows::Forms::Label^  HWlabel2;
	private: System::Windows::Forms::Label^  HWlabel3;
	private: System::Windows::Forms::Timer^  timerCheckHWdevice;
	private:	HardwareSlider^ testHWslider1;
	private:	HardwareSlider^ testHWslider2;
	private:	HardwareSlider^ testHWslider3;

	private: System::ComponentModel::IContainer^  components;
	protected: 

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>


#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			this->groupBox1 = (gcnew System::Windows::Forms::GroupBox());
			this->HWlabel1 = (gcnew System::Windows::Forms::Label());
			this->groupBox2 = (gcnew System::Windows::Forms::GroupBox());
			this->HWlabel2 = (gcnew System::Windows::Forms::Label());
			this->groupBox3 = (gcnew System::Windows::Forms::GroupBox());
			this->HWlabel3 = (gcnew System::Windows::Forms::Label());
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->timerCheckHWdevice = (gcnew System::Windows::Forms::Timer(this->components));
			this->groupBox1->SuspendLayout();
			this->groupBox2->SuspendLayout();
			this->groupBox3->SuspendLayout();
			this->SuspendLayout();
			// 
			// groupBox1
			// 
			this->groupBox1->Controls->Add(this->HWlabel1);
			this->groupBox1->Location = System::Drawing::Point(13, 13);
			this->groupBox1->Name = L"groupBox1";
			this->groupBox1->Size = System::Drawing::Size(200, 235);
			this->groupBox1->TabIndex = 0;
			this->groupBox1->TabStop = false;
			this->groupBox1->Text = L"Slider 1";
			// 
			// HWlabel1
			// 
			this->HWlabel1->AutoSize = true;
			this->HWlabel1->Location = System::Drawing::Point(20, 37);
			this->HWlabel1->Name = L"HWlabel1";
			this->HWlabel1->Size = System::Drawing::Size(54, 13);
			this->HWlabel1->TabIndex = 0;
			this->HWlabel1->Text = L"HWlabel1";
			// 
			// groupBox2
			// 
			this->groupBox2->Controls->Add(this->HWlabel2);
			this->groupBox2->Location = System::Drawing::Point(234, 13);
			this->groupBox2->Name = L"groupBox2";
			this->groupBox2->Size = System::Drawing::Size(200, 235);
			this->groupBox2->TabIndex = 0;
			this->groupBox2->TabStop = false;
			this->groupBox2->Text = L"Slider 2";
			// 
			// HWlabel2
			// 
			this->HWlabel2->AutoSize = true;
			this->HWlabel2->Location = System::Drawing::Point(20, 37);
			this->HWlabel2->Name = L"HWlabel2";
			this->HWlabel2->Size = System::Drawing::Size(54, 13);
			this->HWlabel2->TabIndex = 0;
			this->HWlabel2->Text = L"HWlabel2";
			// 
			// groupBox3
			// 
			this->groupBox3->Controls->Add(this->HWlabel3);
			this->groupBox3->Location = System::Drawing::Point(456, 13);
			this->groupBox3->Name = L"groupBox3";
			this->groupBox3->Size = System::Drawing::Size(200, 235);
			this->groupBox3->TabIndex = 1;
			this->groupBox3->TabStop = false;
			this->groupBox3->Text = L"Slider3";
			// 
			// HWlabel3
			// 
			this->HWlabel3->AutoSize = true;
			this->HWlabel3->Location = System::Drawing::Point(20, 37);
			this->HWlabel3->Name = L"HWlabel3";
			this->HWlabel3->Size = System::Drawing::Size(54, 13);
			this->HWlabel3->TabIndex = 0;
			this->HWlabel3->Text = L"HWlabel3";
			// 
			// button1
			// 
			this->button1->Location = System::Drawing::Point(234, 274);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(200, 23);
			this->button1->TabIndex = 2;
			this->button1->Text = L"Click to Close or press RETURN";
			this->button1->UseVisualStyleBackColor = true;
			this->button1->Click += gcnew System::EventHandler(this, &FormHWtest::button1_Click);
			// 
			// timerCheckHWdevice
			// 
			this->timerCheckHWdevice->Tick += gcnew System::EventHandler(this, &FormHWtest::timerCheckHWdevice_Tick);
			// 
			// FormHWtest
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(676, 309);
			this->ControlBox = false;
			this->Controls->Add(this->button1);
			this->Controls->Add(this->groupBox3);
			this->Controls->Add(this->groupBox2);
			this->Controls->Add(this->groupBox1);
			this->Name = L"FormHWtest";
			this->ShowIcon = false;
			this->ShowInTaskbar = false;
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterScreen;
			this->Text = L"Hardware Slider Test";
			this->groupBox1->ResumeLayout(false);
			this->groupBox1->PerformLayout();
			this->groupBox2->ResumeLayout(false);
			this->groupBox2->PerformLayout();
			this->groupBox3->ResumeLayout(false);
			this->groupBox3->PerformLayout();
			this->ResumeLayout(false);

		}
#pragma endregion
	private: System::Void button1_Click(System::Object^  sender, System::EventArgs^  e) {
 			 this->Hide();
			 }
private: System::Void timerCheckHWdevice_Tick(System::Object^  sender, System::EventArgs^  e) {
		if(testHWslider1->isConnected())
		{
			System::String^ but1str = "OFF";
			if(testHWslider1->getButton() == true)
				but1str = "ON";
			int aval1 = testHWslider1->getRawValue();
			HWlabel1->Text = "Device : 0 \nButton : " + but1str + System::String::Format("\nValue : {0}", aval1);
		}
		else
			HWlabel1->Text = "Device : 0 NOT CONNECTED";
		if(testHWslider2->isConnected())
		{
			System::String^ but2str = "OFF";
			if(testHWslider2->getButton() == true)
				but2str = "ON";
			int aval2 = testHWslider2->getRawValue();
			HWlabel2->Text = "Device : 1 \nButton : " + but2str + System::String::Format("\nValue : {0}", aval2);
		}
		else
			HWlabel2->Text = "Device : 1 NOT CONNECTED";
		if(testHWslider3->isConnected())
		{
			System::String^ but3str = "OFF";
			if(testHWslider3->getButton() == true)
				but3str = "ON";
			int aval3 = testHWslider3->getRawValue();
			HWlabel3->Text = "Device : 2 \nButton : " + but3str + System::String::Format("\nValue : {0}", aval3);
		}
		else
			HWlabel3->Text = "Device : 2 NOT CONNECTED";
	 }
};
}
