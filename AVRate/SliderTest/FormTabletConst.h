#pragma once

#include "FormRating.h"
#include "NetworkStream.h"

namespace SliderTest {

	/// <summary>
	/// Summary for FormTabletConst
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class FormTabletConst : public FormRating
	{

	public:
		public:
		FormTabletConst(SliderTest::GeneralSettings^ gs):FormRating(gs)
		{
			InitializeComponent();
			// 
			//TODO: Add the constructor code here
			//


		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~FormTabletConst()
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
		System::ComponentModel::Container ^components;
		System::ComponentModel::BackgroundWorker^  backgroundWorker;
		System::Windows::Forms::TextBox ^logBox;
		System::Threading::Thread^ newThread;

		NetworkStream *network;
		bool connection;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			connection = false;

			this->components = gcnew System::ComponentModel::Container();
			this->Size = System::Drawing::Size(300,300);
			this->Text = L"Log connection to tablet";
			this->Padding = System::Windows::Forms::Padding(0);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->logBox = gcnew System::Windows::Forms::TextBox();

			this->SuspendLayout();

			this->logBox->Location = System::Drawing::Point(5, 5);
			this->logBox->Size = System::Drawing::Size(275, 250);
			this->logBox->Enabled = false;
			this->logBox->Multiline = true;
			this->logBox->ScrollToCaret();

			this->Controls->Add(logBox);
			
			this->ResumeLayout();
			this->PerformLayout();
			this->FormClosing += gcnew System::Windows::Forms::FormClosingEventHandler(this, &FormTabletConst::Form1_FormClosing);
			
			System::Threading::ThreadStart^ mThread = gcnew   System::Threading::ThreadStart(this,&FormTabletConst::InitializeTabletConnection);
			newThread = gcnew System::Threading::Thread(mThread);
			newThread->SetApartmentState(System::Threading::ApartmentState::STA);			
			newThread->Start();

        }

		void Form1_FormClosing(System::Object ^sender, System::Windows::Forms::FormClosingEventArgs ^e) {
			controller->quitAppRegular();
			if(network != NULL)	
				delete network;
			System::Windows::Forms::Application::Exit();
		}
#pragma endregion



		void InitializeTabletConnection();
		void sendFile(NetworkStream *network, String ^file);

	};
}
