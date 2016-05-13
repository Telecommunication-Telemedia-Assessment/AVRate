#include "StdAfx.h"
#include "FormOnVideoRating.h"
#include "SettingsReader.h"

using namespace System;
using namespace System::Runtime::InteropServices;
using namespace System::Diagnostics;
using namespace System::Drawing;
using namespace System::Windows::Forms;
using namespace System::Collections::Generic;

/*
#include <windows.h>

[DllImport("user32.dll", CharSet = CharSet::Auto, SetLastError = true)]
extern int SetWindowPos(System::IntPtr hwnd, System::IntPtr hafter, int x, int y, int cx, int cy, int flags);
*/

namespace SliderTest {

void paintVerticalRuler(Object ^s, PaintEventArgs^ e) {
	Pen^ pen;
	pen = gcnew Pen(Color::Red);

	int ticks = 50;

	if (ticks > 1) {
		for (int i = 0; i < ticks; i++) {
			int y = 200;
			e->Graphics->DrawLine(pen, 0 , i * 20, 4, i * 20);
		}
	}
}

void paintHorizontalRuler(Object ^s, PaintEventArgs^ e) {
	Pen^ pen;
	pen = gcnew Pen(Color::Red);

	int ticks = 50;

	if (ticks > 1) {
		for (int i = 0; i < ticks; i++) {
			e->Graphics->DrawLine(pen, i * 20, 0, i * 20, 4);
		}
	}
}

// init the scales with the first two scales in the XML
void FormOnVideoRating::initScales() {
	
	videoWidth = 800; // FIMXE needs to be set to the actual values of the movie
	videoHeight = 600;

	int scaleWidth = 50;
	int screenHeight = Screen::PrimaryScreen->WorkingArea.Height;
	int screenWidth = System::Windows::Forms::Screen::PrimaryScreen->WorkingArea.Width;


	videoLeft = (screenWidth - videoWidth) / 2;
	videoTop = (screenHeight - videoHeight) / 2;

	System::Windows::Forms::Form^ formScaleVert = gcnew System::Windows::Forms::Form();
	formScaleVert->ShowInTaskbar = false;
	formScaleVert->StartPosition = System::Windows::Forms::FormStartPosition::CenterScreen;
	formScaleVert->FormBorderStyle = System::Windows::Forms::FormBorderStyle::None;

	formScaleVert->Width = scaleWidth;
	formScaleVert->Height = videoHeight;
	formScaleVert->Left = videoLeft - scaleWidth;
	formScaleVert->Top = videoTop;

	formScaleVert->TopMost = true;

	formScaleVert->BackColor = Color::Bisque;
	formScaleVert->TransparencyKey = formScaleVert->BackColor;

	formScaleVert->Paint += gcnew PaintEventHandler(paintVerticalRuler);

	System::Windows::Forms::Form^ formScaleHori = gcnew System::Windows::Forms::Form();
	formScaleHori->ShowInTaskbar = false;
	formScaleHori->ShowIcon = false;
	formScaleHori->StartPosition = System::Windows::Forms::FormStartPosition::CenterScreen;
	formScaleHori->FormBorderStyle = System::Windows::Forms::FormBorderStyle::None;

	formScaleHori->Width = videoWidth;
	formScaleHori->Height = scaleWidth;
	formScaleHori->Left = videoLeft;
	formScaleHori->Top = videoTop + videoHeight;

	formScaleHori->Paint += gcnew PaintEventHandler(paintHorizontalRuler);

	formScaleHori->BackColor = Color::Bisque;
	formScaleHori->TransparencyKey = formScaleVert->BackColor;

	formScaleHori->TopMost = true;

	formScaleHori->Show();
	formScaleVert->Show();

	this->Left = videoLeft - (this->Width + 20);
	this->Top = videoTop + videoHeight + 20;
}

}