// SliderTest.cpp : main project file.

#include "stdafx.h"
#include "FormSettings.h"

//[DllImport("kernel32.dll", SetLastError = true)]
//    static extern bool AttachConsole(uint dwProcessId);

using namespace SliderTest;

[System::STAThreadAttribute]
int main(array<System::String ^> ^args)
{
	BOOL err;
	bool newConsole = false;
	//bool silent = false;
	if(args->Length > 0)
	{
		//for each(String^ s in args)
		//{
		//	if(s=="-silent")
		//		silent = true;
		//}
		//if(silent == false)
		{
			// WINXP only. Look for AttachConsole at http://msdn.microsoft.com/en-us/magazine/cc164023.aspx
			// If the function fails, the return value is zero. To get extended error information, call GetLastError.		err=AttachConsole(ATTACH_PARENT_PROCESS);// 0 is Error
			// A process can be attached to at most one console. 
			// If the calling process is already attached to a console, 
			// the error code returned is ERROR_ACCESS_DENIED (5). 
			// If the specified process does not have a console, the error code returned is ERROR_INVALID_HANDLE (6).
			// If the specified process does not exist, the error code returned is ERROR_GEN_FAILURE (31).

			err=AttachConsole(ATTACH_PARENT_PROCESS);// 0 is Error
			if(err==0)
			{
#ifdef DEBUG
				AllocConsole();
				Console::WriteLine("ConsoleAllocPost, Errcode " + System::Convert::ToString((int)GetLastError()));
				newConsole = true;
#else
				AllocConsole(); 
				newConsole = true;
#endif
			}
		}
	}

	// Enabling Windows XP visual effects before any controls are created
	System::Windows::Forms::Application::EnableVisualStyles();
	System::Windows::Forms::Application::SetCompatibleTextRenderingDefault(false); 

	System::Windows::Forms::Form^ f = gcnew FormSettings(args, newConsole );

	if(args->Length == 0)
		System::Windows::Forms::Application::Run(f);

	return 0;
}
