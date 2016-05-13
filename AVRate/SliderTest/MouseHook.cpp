#include "StdAfx.h"
#include "MouseHook.h"

#include <stdio.h>
#include <windows.h>

using namespace System;
using namespace System::Runtime::InteropServices;
using namespace System::Diagnostics;
using namespace System::Windows::Forms;
using namespace System::Collections::Generic;

//Declare the wrapper managed POINT class.
[StructLayout(LayoutKind::Sequential)]
public ref class MyPoint {
public: int x;
	int y;
};


//Declare the wrapper managed MouseHookStruct class.
[StructLayout(LayoutKind::Sequential)]
public ref class MouseHookStruct {
public:
	MyPoint pt;
	int hwnd;
	int wHitTestCode;
	int dwExtraInfo;
};

[DllImport("user32.dll", CharSet = CharSet::Auto, SetLastError = true)]
extern System::IntPtr SetWindowsHookEx(int idHook, mHookProc ^lpfn, System::IntPtr hMod, System::UInt32 dwThreadId);

[DllImport("user32.dll", CharSet = CharSet::Auto, SetLastError = true)]
extern System::IntPtr UnhookWindowsHookEx(System::IntPtr hookID);

[DllImport("user32.dll", CharSet = CharSet::Auto, SetLastError = true)]
extern System::IntPtr CallNextHookEx(System::IntPtr hhk, int nCode, System::IntPtr wParam, System::IntPtr lParam);

[DllImport("kernel32.dll", CharSet = CharSet::Auto, SetLastError = true)]
extern System::IntPtr GetModuleHandle(System::String ^lpModuleName);

MouseHook::MouseHook(bool discC, bool discM) {
	discardClick = discC;
	discardMovement = discM;
}

void MouseHook::cursorVisible(bool b) {
	if (b) {
		// restore mouse
		Cursor::Position = System::Drawing::Point(cursorX, cursorY);
		removeMouseHook();
	}
	else {
		cursorX = Cursor::Position.X;
		cursorY = Cursor::Position.Y;
		
		// send the mouse to the right lower corner and "freeze" it with the hook
		Cursor::Position = System::Drawing::Point(Screen::PrimaryScreen->WorkingArea.Width,
			0); // no Taskbar below
			//Screen::PrimaryScreen->WorkingArea.Height);
		installMouseHook();
	}
}

System::IntPtr MouseHook::myHookProc(int nCode, System::IntPtr wParam, System::IntPtr lParam) {
	if (nCode >= 0 && wParam == (System::IntPtr)WM_KEYDOWN)
	{
		int vkCode = Marshal::ReadInt32(lParam);
		//Console::WriteLine((Keys)vkCode);
	}
	//else if (nCode >= 0 && wParam == (System::IntPtr)WH_MOUSE)
	else if (nCode >= 0 && wParam == (System::IntPtr)WM_MOUSEMOVE)
	{
		MouseHookStruct^ mhs = gcnew MouseHookStruct();
		Marshal::PtrToStructure(lParam, mhs);
		
		int x = mhs->pt.x;
		int y = mhs->pt.y;

		if (discardMovement)
			return (System::IntPtr)1; 
	}
	// left button click
	else if (nCode >= 0 && wParam == (System::IntPtr)WM_LBUTTONDOWN)
	{
		MouseHookStruct^ mhs = gcnew MouseHookStruct();
		Marshal::PtrToStructure(lParam, mhs);
		
		int x = mhs->pt.x;
		int y = mhs->pt.y;

		MouseEventArgs^ e = gcnew MouseEventArgs(MouseButtons::Left, 0, x, y, 0);
		if (clickHandler != nullptr)
			clickHandler(this, e);

		if (discardClick)
			return (System::IntPtr)1; // discard all left button clicks
	}
	

	return CallNextHookEx(hookHandle, nCode, wParam, lParam);
}

void MouseHook::installMouseHook() {
    //int WH_KEYBOARD_LL = 13;
	//int WH_MOUSE_LL = 14;
	if (hookHandle != System::IntPtr::Zero) return;

	MouseHookProcedure = gcnew mHookProc(this, &MouseHook::myHookProc);

	Process ^curProcess = gcnew Process();
    curProcess = Process::GetCurrentProcess();
    System::Diagnostics::ProcessModule ^curModule = curProcess->MainModule;
	hookHandle = SetWindowsHookEx(WH_MOUSE_LL, MouseHookProcedure, GetModuleHandle(curModule->ModuleName), 0);
}

void MouseHook::removeMouseHook() {
	UnhookWindowsHookEx(hookHandle);
	hookHandle = System::IntPtr::Zero;
}