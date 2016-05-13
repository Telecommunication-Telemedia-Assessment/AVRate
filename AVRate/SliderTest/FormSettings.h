#pragma once

#include "Types.h"
#include "Logging.h"
#include "FormHardwareConst.h"
#include "FormTabletConst.h"
#include "FormSoftwareEnd.h"
#include "FormSoftwareConst.h"
#include "FormSoftwareSync.h"
#include "FormHardwareEnd.h"
#include "FormOnVideoRating.h"
#include "FormP800.h"
#include "FormTryout.h"
#include "FormHWtest.h"

#undef GetCurrentDirectory	// Use .NET def instead of the WIN API

namespace SliderTest {
	/// <summary>
	/// Summary for Settings
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class FormSettings
		: public System::Windows::Forms::Form
	{
	public:
		FormSettings(array<System::String ^> ^args, bool newConsole)
		{
			if(args->Length != 0)
				startedFromConsole = true;
			else
				startedFromConsole = false;
			//this->Cursor->Current = System::Windows::Forms::Cursors::Hand; // this works for Command Line
			//this->Cursor = gcnew System::Windows::Forms::Cursor( ::Cursor::Current->Handle );
			::Cursor::Current = System::Windows::Forms::Cursors::Hand;
			
			//systemculture= System::Globalization::CultureInfo::CurrentCulture;	
			if(System::Globalization::CultureInfo::CurrentCulture->Name->StartsWith("de"))
				systemculture = gcnew System::Globalization::CultureInfo("de");	
			else if(System::Globalization::CultureInfo::CurrentCulture->Name->StartsWith("en"))
				systemculture = gcnew System::Globalization::CultureInfo("en");	
			else 
				systemculture = gcnew System::Globalization::CultureInfo("en");	

			// Set scaling factors from screen size
			scaleX = float(System::Windows::Forms::Screen::PrimaryScreen->Bounds.Right) / kSTDSCREENWIDTH;
			scaleY = float(System::Windows::Forms::Screen::PrimaryScreen->Bounds.Bottom) / kSTDSCREENHEIGHT;

			resman = gcnew System::Resources::ResourceManager("SliderTest.UImsg",System::Reflection::Assembly::GetExecutingAssembly());//typeof(FormSettings).Assembly);
			listLabelID = gcnew System::Collections::Generic::List<Label^>;
			listTextID = gcnew System::Collections::Generic::List<TextBox^>;

			// ugh....finally : http://social.msdn.microsoft.com/forums/en-US/vcgeneral/thread/e039ea61-8a87-4e31-a9c8-d805229f15bd
			this->Icon = gcnew System::Drawing::Icon(System::Reflection::Assembly::GetExecutingAssembly()->GetManifestResourceStream("circle_green.ico"));


			System::Drawing::Font^ stdFont = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", scaleX*8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->Font = stdFont;

			InitializeComponent();

			if(startedFromConsole == true)
			{
				System::String^ result = processArguments(args);
				if(silentProcessing == false)
					Console::WriteLine(kcommandLineStartStr);

				if(result!="")
				{
					if(silentProcessing == false)
						Console::WriteLine(result);
					exitApp();
				}
				else if(messageOnly == true)
				{
					System::Windows::Forms::MessageBox::Show
						(Environment::NewLine + customMessage + Environment::NewLine + Environment::NewLine, "Evaluation Message",
						MessageBoxButtons::OK, MessageBoxIcon::Asterisk);
					exitApp();
				}
				else
					startEvaluation();
			}
			else
			{
				Label^ l = gcnew Label();
				l->Text = "ID 1";
				l->Width = int(scaleX*30);
				l->Left = int(scaleX*16);
				l->Top = int(scaleY*(60 + 3)); 
				listLabelID->Add(l);
				this->Controls->Add(l);

				TextBox^ t = gcnew TextBox();
				t->Left = int(scaleX*50);
				t->Top = int(scaleY*60); 
				t->Width = int(scaleX*150);
				t->TabStop = true;
				t->TabIndex = 1;
				listTextID->Add(t);
				this->Controls->Add(t);

				t->Text = "ID001";
#ifdef DEBUG
#endif
			}
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~FormSettings()
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
	private: System::Windows::Forms::OpenFileDialog^  openFileDialog1;
	private: System::Windows::Forms::TextBox^  textPlaylist;
	private: System::Windows::Forms::Button^  btnOpenPlaylist;
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::Button^  btnOK;
	private: System::Windows::Forms::Button^  btnCancel;
	//private: System::Windows::Forms::Button^  btnTestHW;
	private: System::Windows::Forms::Button^  buttonSettings;
	private: System::Windows::Forms::TextBox^  textSettings;
	private: System::Windows::Forms::NumericUpDown^  numericUpDown1;
	private: System::Windows::Forms::Label^  label4;
	private: System::Windows::Forms::Label^  label5;
	private:	System::Windows::Forms::Form^ testHWForm;
	private:	System::Windows::Forms::Label^ HWlabel1;
	private:	System::Windows::Forms::Label^ HWlabel2;
	private:	System::Windows::Forms::Label^ HWlabel3;
	private:	System::Windows::Forms::Timer^ timerCheckHWdevice;
	private:	HardwareSlider^ testHWslider1;
	private:	HardwareSlider^ testHWslider2;
	private:	HardwareSlider^ testHWslider3;
	private: System::Collections::Generic::List<System::Windows::Forms::Label^>^ listLabelID;
	private: System::Collections::Generic::List<System::Windows::Forms::TextBox^>^ listTextID;
	private: System::Globalization::CultureInfo^ systemculture;
	private: System::Resources::ResourceManager^ resman;
	private: bool startedFromConsole;
			bool silentProcessing;
			bool messageOnly;
			bool keyOnExit;
			System::String^ customMessage;
			float scaleX;
			float scaleY;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->openFileDialog1 = (gcnew System::Windows::Forms::OpenFileDialog());
			this->textPlaylist = (gcnew System::Windows::Forms::TextBox());
			this->btnOpenPlaylist = (gcnew System::Windows::Forms::Button());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->btnOK = (gcnew System::Windows::Forms::Button());
			this->btnCancel = (gcnew System::Windows::Forms::Button());
			//this->btnTestHW = (gcnew System::Windows::Forms::Button());
			this->buttonSettings = (gcnew System::Windows::Forms::Button());
			this->textSettings = (gcnew System::Windows::Forms::TextBox());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->numericUpDown1 = (gcnew System::Windows::Forms::NumericUpDown());
			this->label5 = (gcnew System::Windows::Forms::Label());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numericUpDown1))->BeginInit();
			this->SuspendLayout();
			// 
			// openFileDialog1
			// 
			this->openFileDialog1->Filter = L"All files (*.*)|*.*";//default only
			this->openFileDialog1->InitialDirectory = System::IO::Directory::GetCurrentDirectory();
			// 
			// textPlaylist
			// 
			this->textPlaylist->Location = System::Drawing::Point(19, 183);
			this->textPlaylist->Name = L"textPlaylist";
			this->textPlaylist->Size = System::Drawing::Size(200, 20);
			this->textPlaylist->TabIndex = 6;
			// 
			// btnOpenPlaylist
			// 
			this->btnOpenPlaylist->Location = System::Drawing::Point(225, 181);
			this->btnOpenPlaylist->Name = L"btnOpenPlaylist";
			this->btnOpenPlaylist->Size = System::Drawing::Size(24, 23);
			this->btnOpenPlaylist->TabIndex = 7;
			this->btnOpenPlaylist->Text = L"...";
			this->btnOpenPlaylist->UseVisualStyleBackColor = true;
			this->btnOpenPlaylist->Click += gcnew System::EventHandler(this, &FormSettings::btnOpenPlaylist_Click);
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(16, 167);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(39, 13);
			this->label1->TabIndex = 2;
			this->label1->Text = L"Playlist";
			// 
			// btnOK
			// 
			this->btnOK->Location = System::Drawing::Point(174, 276);
			this->btnOK->Name = L"btnOK";
			this->btnOK->Size = System::Drawing::Size(75, 23);
			this->btnOK->TabIndex = 13;
			this->btnOK->Text = L"OK";
			this->btnOK->UseVisualStyleBackColor = true;
			this->btnOK->Click += gcnew System::EventHandler(this, &FormSettings::btnOK_Click);
			// 
			// btnCancel
			// 
			this->btnCancel->Location = System::Drawing::Point(19, 276);
			this->btnCancel->Name = L"btnCancel";
			this->btnCancel->Size = System::Drawing::Size(75, 23);
			this->btnCancel->TabIndex = 12;
			this->btnCancel->Text = L"Cancel";
			this->btnCancel->UseVisualStyleBackColor = true;
			this->btnCancel->Click += gcnew System::EventHandler(this, &FormSettings::btnCancel_Click);
			// 
			// buttonSettings
			// 
			this->buttonSettings->Location = System::Drawing::Point(225, 236);
			this->buttonSettings->Name = L"buttonSettings";
			this->buttonSettings->Size = System::Drawing::Size(24, 23);
			this->buttonSettings->TabIndex = 9;
			this->buttonSettings->Text = L"...";
			this->buttonSettings->UseVisualStyleBackColor = true;
			this->buttonSettings->Click += gcnew System::EventHandler(this, &FormSettings::buttonSettings_Click);
			// 
			// textSettings
			// 
			this->textSettings->Location = System::Drawing::Point(19, 238);
			this->textSettings->Name = L"textSettings";
			this->textSettings->Size = System::Drawing::Size(200, 20);
			this->textSettings->TabIndex = 8;
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->Location = System::Drawing::Point(16, 222);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(45, 13);
			this->label4->TabIndex = 9;
			this->label4->Text = L"Settings";
			// 
			// numericUpDown1
			// 
			this->numericUpDown1->Location = System::Drawing::Point(194, 12);
			this->numericUpDown1->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {3, 0, 0, 0});
			this->numericUpDown1->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 0});
			this->numericUpDown1->Name = L"numericUpDown1";
			this->numericUpDown1->Size = System::Drawing::Size(56, 20);
			this->numericUpDown1->TabIndex = 0;
			this->numericUpDown1->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 0});
			this->numericUpDown1->ValueChanged += gcnew System::EventHandler(this, &FormSettings::numericUpDown1_ValueChanged);
			// 
			// label5
			// 
			this->label5->AutoSize = true;
			this->label5->Location = System::Drawing::Point(16, 15);
			this->label5->Name = L"label5";
			this->label5->Size = System::Drawing::Size(125, 13);
			this->label5->TabIndex = 11;
			this->label5->Text = resman->GetString("NumberTestPersons",systemculture);//L"Anzahl der Testpersonen";
			// 
			// FormSettings
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(269, 311);
			this->Controls->Add(this->label5);
			this->Controls->Add(this->numericUpDown1);
			this->Controls->Add(this->label4);
			this->Controls->Add(this->buttonSettings);
			this->Controls->Add(this->textSettings);
			this->Controls->Add(this->btnCancel);
			this->Controls->Add(this->btnOK);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->btnOpenPlaylist);
			this->Controls->Add(this->textPlaylist);
			this->KeyPreview = true;
			this->Name = L"FormSettings";
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterScreen;
			this->Text = L"AVrate";
			this->Load += gcnew System::EventHandler(this, &FormSettings::Settings_Load);
			this->Shown += gcnew System::EventHandler(this, &FormSettings::Settings_Shown);
			this->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &FormSettings::FormSettings_KeyPress);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numericUpDown1))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion

System::String^	processArguments(array<System::String^> ^a)
{
	//array<System::String^> ^a = Environment::GetCommandLineArgs();//maybe TODO its a little more convenient
	bool settingsset = false;
	bool playlistset = false;
	bool idset = false;
	messageOnly = false;
	int idindex = 0;

	int numi = a->Length;
	for(int i=0;i<numi;i++)
	{
		String^ s = a[i];

		if(s->ToLower()== ("-s"))
		{
			if( ((i+1) < numi) && !(a[i+1]->StartsWith("-")))
			{
				textSettings->Text = a[i+1];
				settingsset = true;
				i++;// jump over next
				continue;
			}
			else
				return("Error in command line at : "+ s);
		}
		else if(s->ToLower()== ("-p"))
		{
			if( ((i+1) < numi) && !(a[i+1]->StartsWith("-")))
			{
				textPlaylist->Text = a[i+1];
				playlistset = true;
				i++;// jump over next
				continue;
			}
			else
				return("Error in command line at : "+ s);
		}
		else if(s->ToLower()== ("-id"))
		{
			if( ((i+1) < numi) && !(a[i+1]->StartsWith("-")))
			{
				if(idindex > 2)
					return("Maximum number of test persons is 3 ");
				TextBox^ t = gcnew TextBox();
				t->Text =a[i+1];
				listTextID->Add(t);
				idset = true;
				idindex++;
				i++;// jump over next
				continue;
			}
			else
				return("Error in command line at : "+ s);
		}
		else if(s->ToLower()== ("-msg"))
		{
			if( (i+1) < numi )
			{
				customMessage = a[i+1];
				messageOnly = true;
				i++;// jump over next
				continue;
			}
			else
				return("Error in command line at : "+ s);
		}
		else if(s->ToLower() == ("-silent"))
		{
			silentProcessing = true;
			continue;
		}
		else if(s->ToLower() == ("-key"))
		{
			keyOnExit = true;
			continue;
		}
		else if((s->ToLower() == ("-h")) || (s->ToLower() == ("help")) || (s->ToLower() == ("-?")) || (s->ToLower() == ("?")) || (s->ToLower() == ("-help")) )
		{
			keyOnExit = true;
			System::String^ commandLineHelpStr = 
				"Obligatory command line arguments :\n\n" 
				" -s\t[Settings file]\n" 
				" -p\t[Playlist file]\n" 
				" -id\t[ID of test person or name] ,can be specified up to 3 times\n" 
				"OR\n" 
				" -msg\t[Message] ,displays a custom text message box only\n\n"
				"Optional command line arguments :\n\n" 
				" -silent\tSilent Processing, no output to Console\n"
				" -key\t\tWait for key press before exit\n"
				" -h, help, -?\tThis help text\n" 
				"\nExamples :\n"
				" AVrate.exe -s D:\\Test\\settings1.xml -p D:\\Test\\Playlist99.pls -id DrVogel -id User1000 -silent\n"
				" AVrate.exe -msg \"Please take a 5 minute rest\" -silent\n"
				;
			return(commandLineHelpStr);
		}
		else
		{
			return("Unrecognized command : "+ s);
		}
	}
	if( (messageOnly == true) || (settingsset && playlistset && idset) )
		return("");

	return("Error in command line, missing arguments");
}
private: System::Void btnOpenPlaylist_Click(System::Object^  sender, System::EventArgs^  e) {
			System::String^ currentPath = System::IO::Directory::GetCurrentDirectory();
			openFileDialog1->Filter = L"Playlist files (*.lst)|*.lst|All files (*.*)|*.*";
			openFileDialog1->Title = L"Open Playlist File";
			openFileDialog1->FileName = L"";
			if (openFileDialog1->ShowDialog() == System::Windows::Forms::DialogResult::OK)
				textPlaylist->Text = openFileDialog1->FileName;
			System::IO::Directory::SetCurrentDirectory(currentPath);
		 }

private: System::Void btnOK_Click(System::Object^  sender, System::EventArgs^  e) {
			startEvaluation();
		 }
private: System::Void FormSettings_KeyPress(System::Object^  sender, System::Windows::Forms::KeyPressEventArgs^  e) {
		 static int quitkeynum = 0;
		 String^ exitString = "exit";
		 static int testkeynum = 0;
		 String^ testString = "tst";
		 
		 if( (System::String::IsNullOrEmpty(exitString) == false) && (e->KeyChar ==  exitString[quitkeynum]) )
		 {
			 quitkeynum++;
			 testkeynum = 0;
		 }
		 else if( (System::String::IsNullOrEmpty(testString) == false) && (e->KeyChar ==  testString[testkeynum]) )
		 {
			 testkeynum++;
			 quitkeynum = 0;
		 }
		 else 
		 {
			 testkeynum = 0;
			 quitkeynum = 0;
		 }

		 // QUIT pressed
		 if (quitkeynum >= exitString->Length)
		 {
			 testkeynum = 0;
			 quitkeynum = 0;
			 Application::Exit();
		 }
		 else if (testkeynum >= testString->Length)
		 {
			 testkeynum = 0;
			 quitkeynum = 0;
			 System::Windows::Forms::Form^ testForm = gcnew FormHWtest();
			 testForm->ShowDialog();
		 }
	}
private: System::Void btnTestHW_Click(System::Object^  sender, System::EventArgs^  e) {
		
	}
private: System::Void HWtestButton_Click(System::Object^  sender, System::EventArgs^  e) {
			 testHWForm->Hide();
		 }
private: System::Void timerCheckHWdevice_Tick(System::Object^  sender, System::EventArgs^  e)
	{
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

private: System::Void startEvaluation(void) {
			System::String^ errtxt = "";
			 // check IDs
			 for each (TextBox^ t in listTextID) {
				if (t->Text == "") {
					errtxt = resman->GetString("FillTextbox",systemculture);
				}
			}
			if (textSettings->Text == "") {
				errtxt = resman->GetString("FillTextbox",systemculture);
			}
			else if (textPlaylist->Text == "") {
				errtxt = resman->GetString("FillTextbox",systemculture);
			}
			else if (! System::IO::File::Exists(textPlaylist->Text)) {
				errtxt = resman->GetString("PlaylistNotFound",systemculture);
			}
			else if (! System::IO::File::Exists(textSettings->Text)) {
				errtxt = resman->GetString("SettingsNotFound",systemculture);
			}
			if(errtxt != "")
			{
				if(startedFromConsole == true)
				{ 
					if (silentProcessing == false)
						Console::WriteLine(errtxt);
					exitApp();
				}
				else
					System::Windows::Forms::MessageBox::Show(errtxt, "Problem",	
						System::Windows::Forms::MessageBoxButtons::OK, System::Windows::Forms::MessageBoxIcon::Error);
				return;
			}

			GeneralSettings^ generalSettings;
			SettingsReader^ reader = gcnew SettingsReader(textSettings->Text);
			generalSettings = reader->ReadGeneralSettings();
			if(generalSettings == nullptr)
			{
				if( (startedFromConsole == true) )
				{
					if(silentProcessing == false)
						Console::WriteLine(reader->lastErrorMessage);
				}
				else
					System::Windows::Forms::MessageBox::Show(reader->lastErrorMessage, "Problem",
						System::Windows::Forms::MessageBoxButtons::OK, System::Windows::Forms::MessageBoxIcon::Error);
				exitApp();
				return;
			}

			generalSettings->settingsFile = textSettings->Text;
			generalSettings->playlist = textPlaylist->Text;
			generalSettings->keyOnExit = keyOnExit;
			generalSettings->silentProcessing = silentProcessing;
			generalSettings->startedFromConsole = startedFromConsole;

			for each (TextBox^ t in listTextID) {
				generalSettings->listID->Add(t->Text);
			}

			System::Windows::Forms::Form ^f;

			// decide which form is needed and show it
			if (generalSettings->layout == SliderTest::Layout::P800)
				f = gcnew FormP800(generalSettings);
			else if (generalSettings->layout == SliderTest::Layout::OnScreen)
				f = gcnew FormOnVideoRating(generalSettings);
			else if (generalSettings->rating == Rating::Continuous && generalSettings->device == Device::Software)
				f = gcnew FormSoftwareConst(generalSettings);
			else if (generalSettings->rating == Rating::Continuous && generalSettings->device == Device::Hardware)
				f = gcnew FormHardwareConst(generalSettings);
			else if (generalSettings->device == Device::Tablet)
				f = gcnew FormTabletConst(generalSettings);
			else if (generalSettings->rating == Rating::End && generalSettings->device == Device::Hardware)
				f = gcnew FormHardwareEnd(generalSettings);
			//else if (generalSettings->rating == Rating::Sync && generalSettings->device == Device::Hardware)
			//	f = gcnew FormHardwareSync(generalSettings);
			else if (generalSettings->rating == Rating::End && generalSettings->device == Device::Software) {
				if (generalSettings->layout == SliderTest::Layout::Horizontal)
					f = gcnew FormSoftwareEnd(generalSettings, System::Windows::Forms::Orientation::Horizontal);
				else if (generalSettings->layout == SliderTest::Layout::Vertical)
					f = gcnew FormSoftwareEnd(generalSettings, System::Windows::Forms::Orientation::Vertical);
			}
			else if (generalSettings->rating == Rating::Sync && generalSettings->device == Device::Software) {
				if (generalSettings->layout == SliderTest::Layout::Horizontal)
					f = gcnew FormSoftwareSync(generalSettings, System::Windows::Forms::Orientation::Horizontal);
				else if (generalSettings->layout == SliderTest::Layout::Vertical)
					f = gcnew FormSoftwareSync(generalSettings, System::Windows::Forms::Orientation::Vertical);
			}

			this->Hide();
			if(startedFromConsole)
				System::Windows::Forms::Application::Run(f);
			else
				f->Show();
		}
private: System::Void exitApp(System::Void) {
			 if((startedFromConsole == true) && (keyOnExit==true) )
				{
					if(silentProcessing == false)
						Console::WriteLine(kkeyPressStr);
					Console::ReadKey();
					if(silentProcessing == false)
						Console::Write(Environment::NewLine);
				}
			 Application::Exit();
		 }
private: System::Void btnCancel_Click(System::Object^ sender, System::EventArgs^  e) {
			 System::Windows::Forms::Application::Exit();
		}
private: System::Void Settings_Load(System::Object^  sender, System::EventArgs^  e) {
			textSettings->Text = kDefaultSettingsLocation ;
			textPlaylist->Text = kDefaultPlaylistLocation;
		}
private: System::Void Settings_Shown(System::Object^  sender, System::EventArgs^  e) {
			this->Cursor = gcnew System::Windows::Forms::Cursor( ::Cursor::Current->Handle );
			::Cursor::Current = System::Windows::Forms::Cursors::Hand;
			if(startedFromConsole == true)
				this->Hide();
		 }
private: System::Void buttonSettings_Click(System::Object^  sender, System::EventArgs^  e) {
			System::String^ currentPath = System::IO::Directory::GetCurrentDirectory();
			openFileDialog1->Filter = L"Settings files (*.xml)|*.xml|All files (*.*)|*.*";
			openFileDialog1->Title = L"Open Settings File";
			openFileDialog1->FileName = L"";
			if (openFileDialog1->ShowDialog() == System::Windows::Forms::DialogResult::OK)
				textSettings->Text = openFileDialog1->FileName;
			System::IO::Directory::SetCurrentDirectory(currentPath);
		}
private: System::Void numericUpDown1_ValueChanged(System::Object^  sender, System::EventArgs^  e) {
			 // add or remove a user input field
			 for each (Control^ c in listTextID)
				 this->Controls->Remove(c);
			 for each (Control^ c in listLabelID)
				 this->Controls->Remove(c);
			 listTextID->Clear();
			 listLabelID->Clear();

			 Random r;

			 for (int i = 0; i < numericUpDown1->Value; i++) {
				 Label^ l = gcnew Label();
				 l->Text = "ID " + (i + 1);
				 l->Width = int(scaleX*30);
				 l->Left = int(scaleX*16);
				 l->Top = int(scaleY*(60 + i * 30 + 3)); 
				 listLabelID->Add(l);
				 this->Controls->Add(l);

				 TextBox^ t = gcnew TextBox();
				 t->Left = int(scaleX*50);
				 t->Top = int(scaleY*(60 + i * 30));
				 t->Width = int(scaleX*150);
				 t->Text = "" + r.Next(1000);
				 t->TabStop = true;
				 t->TabIndex = i;
				 listTextID->Add(t);
				 this->Controls->Add(t);
			 }
		 }
};
}
