#pragma once

#include "MySlider.h"
#include "sndfile.hh"
#undef SNDFILE_tag
#pragma comment(lib, "libsndfile-1.lib")

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;
using namespace System::Net;
using namespace System::Net::Sockets;
using namespace System::Text;


namespace SliderTest {



	public ref class FormTryout : public System::Windows::Forms::Form
	{
	public:
		FormTryout(void)
		{
			InitializeComponent();
			
			mySlider = gcnew MySlider();
			this->Controls->Add(mySlider);
			sw = gcnew System::Diagnostics::Stopwatch();
			timer1->Start();
				 
			useVS = false;

			SndfileHandle* sfh = new SndfileHandle("d:\\test1.wav");
			int size = (int)(sfh->channels() * sfh->frames());
			double* buffer = (double*)malloc(size * 8);
			int amount = (int)sfh->frames();
			sfh->readf(buffer, amount);

			if (useVS) {
				String^ serverAddress = "10.117.33.208";
				int port = 42219;
				IPHostEntry^ hostEntry = Dns::GetHostEntry(serverAddress);

				array<IPAddress^>^IPAddresses = hostEntry->AddressList;

				// Evaluate the socket and receive host IPAddress and IPEndPoint.
				IPAddress^ address = IPAddresses[0];
				IPEndPoint^ endPoint = gcnew IPEndPoint(address, port);

				// Creates the Socket to send data over a TCP connection.
				s = gcnew Socket(AddressFamily::InterNetwork, SocketType::Stream, ProtocolType::Tcp);

				s->Connect(endPoint);
			}
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~FormTryout()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Label^  label1;
	private: System::ComponentModel::IContainer^  components;
	private: System::Windows::Forms::Timer^  timer1;
	private: System::Windows::Forms::Button^  button1;
			 
	System::Net::Sockets::Socket ^s;
	MySlider^ mySlider;
	bool useVS;
	private: System::Windows::Forms::Button^  button2;
	private: System::Windows::Forms::Button^  button3;
	private: System::Windows::Forms::Label^  label2;

	private: System::Windows::Forms::Button^  button4;
	private: AxWMPLib::AxWindowsMediaPlayer^  axWindowsMediaPlayer1;
	protected: 

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>

		System::Diagnostics::Stopwatch^ sw;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(FormTryout::typeid));
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->timer1 = (gcnew System::Windows::Forms::Timer(this->components));
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->button2 = (gcnew System::Windows::Forms::Button());
			this->button3 = (gcnew System::Windows::Forms::Button());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->button4 = (gcnew System::Windows::Forms::Button());
			this->axWindowsMediaPlayer1 = (gcnew AxWMPLib::AxWindowsMediaPlayer());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->axWindowsMediaPlayer1))->BeginInit();
			this->SuspendLayout();
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 48, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->label1->Location = System::Drawing::Point(221, 218);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(204, 73);
			this->label1->TabIndex = 0;
			this->label1->Text = L"label1";
			// 
			// timer1
			// 
			this->timer1->Interval = 10;
			this->timer1->Tick += gcnew System::EventHandler(this, &FormTryout::timer1_Tick);
			// 
			// button1
			// 
			this->button1->Location = System::Drawing::Point(290, 396);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(75, 23);
			this->button1->TabIndex = 1;
			this->button1->Text = L"start display";
			this->button1->UseVisualStyleBackColor = true;
			this->button1->Click += gcnew System::EventHandler(this, &FormTryout::button1_Click);
			// 
			// button2
			// 
			this->button2->Location = System::Drawing::Point(371, 396);
			this->button2->Name = L"button2";
			this->button2->Size = System::Drawing::Size(75, 23);
			this->button2->TabIndex = 2;
			this->button2->Text = L"stop display";
			this->button2->UseVisualStyleBackColor = true;
			this->button2->Click += gcnew System::EventHandler(this, &FormTryout::button2_Click);
			// 
			// button3
			// 
			this->button3->Location = System::Drawing::Point(12, 396);
			this->button3->Name = L"button3";
			this->button3->Size = System::Drawing::Size(75, 23);
			this->button3->TabIndex = 3;
			this->button3->Text = L"slider";
			this->button3->UseVisualStyleBackColor = true;
			this->button3->Click += gcnew System::EventHandler(this, &FormTryout::button3_Click);
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(12, 422);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(35, 13);
			this->label2->TabIndex = 4;
			this->label2->Text = L"label2";
			// 
			// button4
			// 
			this->button4->Location = System::Drawing::Point(398, 134);
			this->button4->Name = L"button4";
			this->button4->Size = System::Drawing::Size(75, 23);
			this->button4->TabIndex = 6;
			this->button4->Text = L"button4";
			this->button4->UseVisualStyleBackColor = true;
			this->button4->Click += gcnew System::EventHandler(this, &FormTryout::button4_Click);
			// 
			// axWindowsMediaPlayer1
			// 
			this->axWindowsMediaPlayer1->Enabled = true;
			this->axWindowsMediaPlayer1->Location = System::Drawing::Point(159, 330);
			this->axWindowsMediaPlayer1->Name = L"axWindowsMediaPlayer1";
			this->axWindowsMediaPlayer1->OcxState = (cli::safe_cast<System::Windows::Forms::AxHost::State^  >(resources->GetObject(L"axWindowsMediaPlayer1.OcxState")));
			this->axWindowsMediaPlayer1->Size = System::Drawing::Size(75, 23);
			this->axWindowsMediaPlayer1->TabIndex = 7;
			// 
			// FormTryout
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(497, 502);
			this->Controls->Add(this->axWindowsMediaPlayer1);
			this->Controls->Add(this->button4);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->button3);
			this->Controls->Add(this->button2);
			this->Controls->Add(this->button1);
			this->Controls->Add(this->label1);
			this->Name = L"FormTryout";
			this->Text = L"FormTryout";
			this->KeyDown += gcnew System::Windows::Forms::KeyEventHandler(this, &FormTryout::FormTryout_KeyDown);
			this->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &FormTryout::FormTryout_MouseDown);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->axWindowsMediaPlayer1))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void FormTryout_KeyDown(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e) {
				 if (sw->IsRunning)
					 sw->Stop();
				 else 
					 sw->StartNew();
			 }
	private: System::Void timer1_Tick(System::Object^  sender, System::EventArgs^  e) {
				 int secs = (int)(sw->ElapsedMilliseconds / 1000);
				 int millis = (int)(sw->ElapsedMilliseconds - (secs * 1000));
				 label1->Text = secs + ":" + millis;
			 }
	private: System::Void FormTryout_MouseDown(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {
				 if (sw->IsRunning)
					 sw->Stop();
				 else
					 sw->StartNew();			 
			 }
	private: System::Void button1_Click(System::Object^  sender, System::EventArgs^  e) {
				 if (sw->IsRunning) {
					 sw->Stop();
				 }
				 else {
					 sw->Reset();
					 sw->Start();
					 if (useVS) {
						 array<Byte>^ cmdArray = Encoding::UTF8->GetBytes("DisplayForward\n");
						 int res = s->Send(cmdArray);
					 }
				 }
			 }

private: System::Void button2_Click(System::Object^  sender, System::EventArgs^  e) {
			 if (useVS) {
				 array<Byte>^ cmdArray = Encoding::UTF8->GetBytes("DisplayStop\n");
				 int res = s->Send(cmdArray);
				 cmdArray = Encoding::UTF8->GetBytes("goto 100\n");
				 res = s->Send(cmdArray);
			 }
			 sw->Reset();
		 }
		 void mySliderHandling(System::Object^  sender, System::EventArgs^  e) {
			 label2->Text = Convert::ToString(mySlider->getValue());
		 }

private: System::Void button3_Click(System::Object^  sender, System::EventArgs^  e) {
			mySlider->setBounds(50, 50, 70, 700);// (NOT YET:) TODO: check if still correct(after setBounds Change)
			mySlider->valueChanged += gcnew EventHandler(this, &SliderTest::FormTryout::mySliderHandling);
		 }
private: System::Void button4_Click(System::Object^  sender, System::EventArgs^  e) {
//			 axWindowsMediaPlayer1->URL = "bla";
			System::Diagnostics::ProcessStartInfo^ startInfo = gcnew System::Diagnostics::ProcessStartInfo("d:\\mplayer.exe");
			startInfo->Arguments = "-identify -frames 0 -vo null -ao null d:\\clock-15secs-indeo.avi";
			startInfo->RedirectStandardOutput = true;
			startInfo->UseShellExecute = false;
			startInfo->WindowStyle = System::Diagnostics::ProcessWindowStyle::Minimized;

			System::Diagnostics::Process^ p = System::Diagnostics::Process::Start(startInfo);
			IO::StreamReader^ sr = p->StandardOutput;
			System::String^ str = sr->ReadToEnd();
			p->WaitForExit();
		}
};
}
