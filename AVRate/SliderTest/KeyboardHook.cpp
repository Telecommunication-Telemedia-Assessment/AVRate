#pragma once

#include "StdAfx.h"
#include "Controller.h"
#include "KeyboardHook.h"

#include <stdio.h>
#include <windows.h>

using namespace System;
using namespace System::Runtime::InteropServices;
using namespace System::Diagnostics;
using namespace System::Windows::Forms;
using namespace System::Collections::Generic;

/*/Declare the wrapper managed POINT class.
[StructLayout(LayoutKind::Sequential)]
public ref class MyPoint {
public: int x;
	int y;
};
*/

//Declare the wrapper managed KeyboardHookStruct class.
[StructLayout(LayoutKind::Sequential)]
public ref class KeyboardHookStruct 
{
public:
	int vkCode; //Specifies a virtual-key code. The code must be a value in the range 1 to 254.
	int scanCode; // Specifies a hardware scan code for the key.
	int flags; // Specifies the extended-key flag, event-injected flag, context code, and transition-state flag.
	int time; // Specifies the time stamp for this message.
	int dwExtraInfo; // Specifies extra information associated with the message.
};

[DllImport("user32.dll", CharSet = CharSet::Auto, SetLastError = true)]
extern System::IntPtr SetWindowsHookEx(int idHook, kHookProc ^lpfn, System::IntPtr hMod, System::UInt32 dwThreadId);

[DllImport("user32.dll", CharSet = CharSet::Auto, SetLastError = true)]
extern System::IntPtr UnhookWindowsHookEx(System::IntPtr hookID);

[DllImport("user32.dll", CharSet = CharSet::Auto, SetLastError = true)]
extern System::IntPtr CallNextHookEx(System::IntPtr hhk, int nCode, System::IntPtr wParam, System::IntPtr lParam);

[DllImport("kernel32.dll", CharSet = CharSet::Auto, SetLastError = true)]
extern System::IntPtr GetModuleHandle(System::String ^lpModuleName);

//[DllImport("user32")]
//extern int ToAscii(int uVirtKey, //[in] Specifies the virtual-key code to be translated.
//int uScanCode, // [in] Specifies the hardware scan code of the key to be translated. The high-order bit of this value is set if the key is up (not pressed).
//byte[] lpbKeyState, // [in] Pointer to a 256-byte array that contains the current keyboard state. Each element (byte) in the array contains the state of one key. If the high-order bit of a byte is set, the key is down (pressed). The low bit, if set, indicates that the key is toggled on. In this function, only the toggle bit of the CAPS LOCK key is relevant. The toggle state of the NUM LOCK and SCROLL LOCK keys is ignored.
//byte[] lpwTransKey, // [out] Pointer to the buffer that receives the translated character or characters.
//int fuState); // [in] Specifies whether a menu is active. This parameter must be 1 if a menu is active, or 0 otherwise.
//
////The GetKeyboardState function copies the status of the 256 virtual keys to the specified buffer.
//[DllImport("user32")]
//extern int GetKeyboardState(byte[] pbKeyState);

KeyboardHook::KeyboardHook(bool discardkey, Controller^ c) 
{
	this->discardKey = discardkey;
	this->controller = c;
}
System::IntPtr KeyboardHook::myHookProc(int nCode, System::IntPtr wParam, System::IntPtr lParam) 
{
 	if (hookEnabled && nCode >= 0 && wParam == (System::IntPtr)WM_KEYDOWN)
	{
		int vkCode = Marshal::ReadInt32(lParam);
		controller->storeKeypressTimeTick((Keys)vkCode);
		if(discardKey)
			return (IntPtr)1;
	}

	//return 1;	// stops further processing of keypress
	return CallNextHookEx(hookHandle, nCode, wParam, lParam);
}

void KeyboardHook::installKeyboardHook() {
    //int WH_KEYBOARD_LL = 13;
	//int WH_MOUSE_LL = 14;
	if (hookHandle != System::IntPtr::Zero) return;

	KeyboardHookProcedure = gcnew kHookProc(this, &KeyboardHook::myHookProc);

	Process ^curProcess = gcnew Process();
    curProcess = Process::GetCurrentProcess();
    System::Diagnostics::ProcessModule ^curModule = curProcess->MainModule;
	//hookHandle = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardHookProcedure, (IntPtr)LoadLibrary("User32"), 0);
	hookHandle = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardHookProcedure, GetModuleHandle(curModule->ModuleName), 0);
}

void KeyboardHook::removeKeyboardHook() {
	UnhookWindowsHookEx(hookHandle);
	hookHandle = System::IntPtr::Zero;
}
