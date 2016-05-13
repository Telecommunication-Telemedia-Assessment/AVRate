#pragma once

delegate System::IntPtr mHookProc(int nCode, System::IntPtr wParam, System::IntPtr lParam);

ref class MouseHook {
public:
	MouseHook(bool discC, bool discM); // discC == discard all mouseclicks, discM == discard all mouse movement
	System::IntPtr myHookProc(int nCode, System::IntPtr wParam, System::IntPtr lParam);
	void installMouseHook(); // start the hook
	void removeMouseHook(); // stop the hook
	void cursorVisible(bool b);

	System::EventHandler^ clickHandler;
	System::IntPtr hookHandle;
	mHookProc^ MouseHookProcedure;

private:
	bool discardClick;
	bool discardMovement;
	int cursorX, cursorY;
};