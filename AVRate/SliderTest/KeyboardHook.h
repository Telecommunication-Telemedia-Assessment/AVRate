#pragma once

public delegate System::IntPtr kHookProc(int nCode, System::IntPtr wParam, System::IntPtr lParam);

public ref class KeyboardHook {
public:
	KeyboardHook(bool, Controller^); // discC == discard all mouseclicks, discM == discard all mouse movement
	System::IntPtr myHookProc(int nCode, System::IntPtr wParam, System::IntPtr lParam);
	void installKeyboardHook(); // start the hook
	void removeKeyboardHook(); // stop the hook

	System::EventHandler^ clickHandler;
	System::IntPtr hookHandle;
	kHookProc^ KeyboardHookProcedure;

	bool hookEnabled;
	Controller^ controller;

private:
	bool discardKey;
};