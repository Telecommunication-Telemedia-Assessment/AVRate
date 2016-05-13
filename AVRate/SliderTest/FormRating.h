#pragma once
#include "Controller.h"
#include "Logging.h"
#include "SliderGroup.h"
#include "ButtonGroup.h"
#include "SoftwareSlider.h"
#include "HardwareSlider.h"
#include "ButtonSlider.h"
#include "KeypressSlider.h"
#include "SettingsReader.h"
#include "VolumeForm.h"
#include "MouseHook.h"
//#include "KeyboardHook.h"
#include "Playlist.h"
#include "FormHWtest.h"

namespace SliderTest {

// Base class for all Rating Forms
public ref class FormRating :public System::Windows::Forms::Form
{
public:
	FormRating(GeneralSettings ^gs)
	{
		controller = gcnew Controller(gs);
		controller->parentForm = this;

		if(gs->logKeypress)
		{
			keySlider = gcnew KeypressSlider();
			controller->setKeySlider(keySlider);
		}
		try
		{
			this->Icon = gcnew System::Drawing::Icon("AVrating.ico");
		}
		catch(Exception^)
		{
			this->Icon = gcnew System::Drawing::Icon(System::Reflection::Assembly::GetExecutingAssembly()->GetManifestResourceStream("circle_green.ico"));
		}
		finally
		{
			resman = gcnew System::Resources::ResourceManager("SliderTest.UImsg",System::Reflection::Assembly::GetExecutingAssembly());//typeof(FormSettings).Assembly);

			mHook = gcnew MouseHook(true, true);

			oldCursor = ::Cursor::Current;
			oldRect = ::Cursor::Clip;
			this->TabStop = false;
			// Can be enabled when all Forms have same fontscaling mechanism
			this->Cursor = gcnew System::Windows::Forms::Cursor( ::Cursor::Current->Handle );
			this->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &FormRating::FormRating_KeyPress);
			this->Shown += gcnew System::EventHandler(this, &FormRating::FormRating_Shown);
			this->FormClosed += gcnew System::Windows::Forms::FormClosedEventHandler(this, &FormRating::FormRating_FormClosed);
		}
	}
	~FormRating()
	{
		unclipMouse();
		this->Cursor = oldCursor;
	}
	void clipMouse() {
		//http://msdn.microsoft.com/de-de/library/system.windows.forms.cursor.aspx
		// Application..::.DoEvents 
		::Cursor::Clip = clipRect;	// not valid until subclass Form shown
		//this->Cursor->Clip = clipRect;	// not valid until subclass Form shown
	}
	void unclipMouse() {
		::Cursor::Clip = oldRect;
	}
	void enableMouse() {
		::Cursor::Show();// evtl. weg
		mHook->cursorVisible(true);
	}
	void disableMouse() {
		//::Cursor::Clip = Rectangle(0,System::Windows::Forms::Screen::PrimaryScreen->Bounds.Bottom,0,System::Windows::Forms::Screen::PrimaryScreen->Bounds.Bottom);
		::Cursor::Hide();// evtl. weg
		mHook->cursorVisible(false);
	}
	void installMouseHook() {
		mHook->installMouseHook();
	}
	void removeMouseHook() {
		mHook->removeMouseHook();
	}
public:
	virtual void quitAppFromUser() 
	{
		this->Hide();	// if not, it freezes visually with Console
		controller->LogNoDate(resman->GetString("LOG_EvalAbort",systemculture));
		controller->finishAllOpenLogs();
		if((controller->generalSettings->startedFromConsole == true) && 
			(controller->generalSettings->keyOnExit == true) )
		{
			if(controller->generalSettings->silentProcessing == false)
				Console::WriteLine(kkeyPressStr);
			Console::ReadKey();
			if(controller->generalSettings->silentProcessing == false)
				Console::Write(Environment::NewLine);
		}
		Application::Exit();
	}
protected:
		System::Drawing::Rectangle clipRect;
		System::Drawing::Rectangle oldRect;
		System::Windows::Forms::Cursor^ oldCursor;
		System::Globalization::CultureInfo^ systemculture;
		System::Globalization::CultureInfo^ userculture;
		System::Resources::ResourceManager^ resman;
		Controller^ controller;
		KeypressSlider^ keySlider;
		//KeyboardHook^ kHook;
private:
	MouseHook^ mHook;
private: System::Void FormRating_FormClosed(System::Object^  sender, FormClosedEventArgs^  e) 
		 { 
			unclipMouse();
		 }	
private: System::Void FormRating_Shown(System::Object^  sender, System::EventArgs^  e) 
		 { 
			 if(controller->errorInitializing == true)
			 {
				 //unclipMouse();
				 controller->quitAppRegular();
			 }
#ifndef NOCLIPMOUSE
			 else
				clipMouse();
#endif
		 }
private: System::Void FormRating_KeyPress(System::Object^  sender, System::Windows::Forms::KeyPressEventArgs^  e) 
		 {
			#ifdef DEBUG
			 String^ ustr = "u";
			 String^ cstr = "c";
			 String^ xstr = "x";
			 if(e->KeyChar == ustr[0])
				 unclipMouse();
			 if(e->KeyChar == cstr[0])
				 clipMouse();
			 if(e->KeyChar == xstr[0])
				quitAppFromUser();
			#endif

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
				MessageBoxButtons buttons = MessageBoxButtons::YesNo;
				MessageBoxIcon icon = MessageBoxIcon::Warning;
				#ifdef DEBUG
				MessageBoxDefaultButton defbutton = MessageBoxDefaultButton::Button1;
				#else
				MessageBoxDefaultButton defbutton = MessageBoxDefaultButton::Button2;
				#endif
				System::Windows::Forms::DialogResult result;
			
				String^ caption = resman->GetString("QuitQuestionHeader",controller->getSystemCulture());
				String^ message = resman->GetString("QuitQuestionMessage",controller->getSystemCulture());
				result = MessageBox::Show( this, message, caption, buttons, icon, defbutton );
				if ( result == ::DialogResult::Yes )
					quitAppFromUser();
				else
					quitkeynum = 0;
			 }

			 // TEST PRESSED
			 else if (testkeynum >= testString->Length)
			 {
				 testkeynum = 0;
				 quitkeynum = 0;
				 unclipMouse();
				 System::Windows::Forms::Form^ testForm = gcnew FormHWtest();
				 testForm->ShowDialog();
			 }
		 }

private: System::Void InitializeComponent() {
			 this->SuspendLayout();
			 // 
			 // FormRating
			 // 
			 this->ClientSize = System::Drawing::Size(292, 273);
			 this->Name = L"FormRating";
			 this->ResumeLayout(false);
		 }
};
}