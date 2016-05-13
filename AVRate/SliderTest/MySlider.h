#pragma once

#include "StdAfx.h"

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;

public delegate void SliderValueDelegate();

// MySlider: replaces the normal slider, this one offers more control over
// the drawing of the pointer, ticks etc. and is easier to click at. Also, a
// click will set the pointer straight to the click position

public ref class MySlider : public System::Windows::Forms::Panel {
public:
	MySlider() {
		width = 40;		// Values will be overwritten
		height = 100;
		pointerHeight = 10;
		ticks = 5;
		sliderX = width / 2 - 2;
		sliderY = 10;
		sliderHeight = height - 2 * sliderY;
		min = 0;
		max = 10;
		orientation = System::Windows::Forms::Orientation::Horizontal;

		drawPointer = false;

		if (orientation == System::Windows::Forms::Orientation::Horizontal)
		{
			this->SetBounds(0, 0, height, width);
		}
		else
		{
			this->SetBounds(0, 0, width, height);
		}

		this->BorderStyle = Windows::Forms::BorderStyle::None;

		//this->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &MySlider::mySlider_KeyPress);
		// enable double buffering to remove flicker
		this->SetStyle(ControlStyles::AllPaintingInWmPaint |
			ControlStyles::UserPaint |
			ControlStyles::DoubleBuffer, true);
	}

	void setBounds(int newx, int newy, int newwidth, int newheight) {
		width = newwidth;
		height = newheight;
		sliderX = width / 2 - 2;

		if (orientation == System::Windows::Forms::Orientation::Horizontal)
		{
			sliderHeight = height - 2*kSLIDERMARGINX; //ok,l&r
			sliderY = kSLIDERMARGINX;//ok,==newy
			this->SetBounds(newy, newx, height, width);
		}
		else
		{
			sliderHeight = height - 2*kSLIDERMARGINY; //revert - ?;
			sliderY = kSLIDERMARGINY;//? revert
			this->SetBounds(newx, newy, width, height);
		}
		
		this->Invalidate();
	}

	void setMinMax(int newmin, int newmax) {
		min = newmin;
		max = newmax;
	}

	void setTicks(int t) {
		ticks = t;
	}
	void setFixedTicks(bool b) {
		fixedTicks = b;
	}

	void setOrientation(Orientation o) {
		orientation = o;
	}

	virtual void OnPaint(System::Windows::Forms::PaintEventArgs^ e) override {
		Pen^ pen;

		if (this->Enabled)
			pen = gcnew Pen(Color::Black);
		else
			pen = gcnew Pen(SystemColors::ControlDark);
		
		//float oldPenWidth = pen->Width;
		Brush^ brush = gcnew SolidBrush(SystemColors::ControlLight);

		if (orientation == System::Windows::Forms::Orientation::Vertical) {
			// ticks
			//pen->Width = pen->Width * scaleX;
			if (ticks > 1) {
				for (int i = 0; i < ticks; i++) {
					int y = (int)(i * sliderHeight / (float)(ticks - 1));
					e->Graphics->DrawLine(pen, sliderX + 11 , sliderY + y, sliderX + 14, sliderY + y);
					e->Graphics->DrawLine(pen, sliderX - 7 , sliderY + y, sliderX - 10, sliderY + y);
				}
			}
			//pen->Width = oldPenWidth;

			// slider bg
			e->Graphics->DrawRectangle(pen, width / 2 - 2, sliderY, 4, sliderHeight);

			// pointer
			if (drawPointer) {
				int pointerWidthHalf = 10;
				int x1 = pointerX - pointerWidthHalf + width / 2;
				int y1 = pointerY - pointerHeight / 2;
				int x2 = x1 + pointerWidthHalf * 2;
				int y2 = y1 + pointerHeight;

				pen->Color = SystemColors::ButtonFace;
				e->Graphics->FillRectangle(brush, x1, y1, pointerWidthHalf * 2, pointerHeight);
				pen->Color = Color::Black;
				e->Graphics->DrawRectangle(pen, x1, y1, pointerWidthHalf * 2, pointerHeight);
				
				// light/shadow
				pen->Color = SystemColors::ButtonHighlight;
				e->Graphics->DrawLine(pen, x1 + 1, y1 + 1, x2 - 1, y1 + 1);
				e->Graphics->DrawLine(pen, x1 + 1, y1 + 1, x1 + 1, y2 - 1);
				pen->Color = SystemColors::ButtonShadow;
				e->Graphics->DrawLine(pen, x1 + 1, y2 - 1, x2 - 1, y2 - 1);
				e->Graphics->DrawLine(pen, x2 - 1, y1 + 1, x2 - 1, y2 - 1);
			}
		}
		// horizontal, x and y exchanged
		else {
			// ticks
			if (ticks > 1) {
				//pen->Width = pen->Width * scaleX;
				for (int i = 0; i < ticks; i++) {
					int y = (int)(i * sliderHeight / (float)(ticks - 1));
					e->Graphics->DrawLine(pen, sliderY + y, sliderX + 11 , sliderY + y, sliderX + 14);
					e->Graphics->DrawLine(pen, sliderY + y, sliderX - 7 , sliderY + y, sliderX - 10);
				}
				//pen->Width = oldPenWidth;
			}

			// slider bg
			e->Graphics->DrawRectangle(pen, sliderY, width / 2 - 2, sliderHeight, 4);
			
			// pointer
			if (drawPointer) {
				int pointerWidthHalf = 10;
				int y1 = pointerX - pointerWidthHalf + width / 2;
				int x1 = pointerY - pointerHeight / 2;
				int y2 = y1 + pointerWidthHalf * 2;
				int x2 = x1 + pointerHeight;

				pen->Color = SystemColors::ButtonFace;
				e->Graphics->FillRectangle(brush, x1, y1, pointerHeight, pointerWidthHalf * 2);
				pen->Color = Color::Black;
				e->Graphics->DrawRectangle(pen, x1, y1, pointerHeight, pointerWidthHalf * 2);
				pen->Color = SystemColors::ButtonHighlight;
				e->Graphics->DrawLine(pen, x1 + 1, y1 + 1, x2 - 1, y1 + 1);
				e->Graphics->DrawLine(pen, x1 + 1, y1 + 1, x1 + 1, y2 - 1);
				pen->Color = SystemColors::ButtonShadow;
				e->Graphics->DrawLine(pen, x1 + 1, y2 - 1, x2 - 1, y2 - 1);
				e->Graphics->DrawLine(pen, x2 - 1, y1 + 1, x2 - 1, y2 - 1);
			}
		}

		delete pen;
		delete brush;
	}

	virtual void OnMouseMove(System::Windows::Forms::MouseEventArgs^ e) override {
		if (e->Button == Windows::Forms::MouseButtons::Left) {
			handleMouseInput(e);
		}
	}

	virtual void OnMouseDown(System::Windows::Forms::MouseEventArgs^ e) override {
		if (e->Button == Windows::Forms::MouseButtons::Left) {
			if (Enabled && !drawPointer) drawPointer = true;
			handleMouseInput(e);
		}
	}

	// set the pointer to the current mouse position
	void handleMouseInput(System::Windows::Forms::MouseEventArgs^ e) {
		if (e->Y == pointerY) return;

		int pos = 0;
		if (orientation == Orientation::Horizontal) 
			pos = e->X - sliderY;
		else if (orientation == Orientation::Vertical) 
			pos = e->Y - sliderY;

		if (pos < 0) pos = 0;
		else if (pos > sliderHeight) pos = sliderHeight;// eigentlich >= und slHeight-1

		// BB orig:
		//if (pos < sliderY) pos = 0;
		//else if (pos > sliderY + sliderHeight) pos = sliderHeight;
		//else if (pos >= sliderY && pos <= sliderHeight + sliderY) pos -= sliderY;
		
		value = (pos / (float)sliderHeight); // set between 0 and 1

		if(fixedTicks)
		{
			float t = float(ticks-1);
			value = float(Math::Round(value*t)) / t;
			pointerY = int(value*float(sliderHeight)) + sliderY;
		}
		else
			pointerY = pos + sliderY;

		//value = 1 - (pos / (float)sliderHeight);
		valueChanged(this, nullptr);
		this->Invalidate();
	}

	// value scaled from 0.0 - 1.0 to be between min and max
	float getValue() {
		// Returns the align-corrected Value. Means Vertical Slider is flipped, Horizontal Slider is normal.
		// Check with VolumeForm if has side effects TODO
		if (!drawPointer) return -1.0;
		if (orientation == Orientation::Horizontal) 
			return (value * (max - min)) + min;
		else //if (orientation == Orientation::Vertical) 
			return ((1.f-value) * (max - min)) + min;
	}

	// v between 0 and 1 (0 = min, 1 = max)
	void setValue(float v) {
		pointerY = (int)(v * sliderHeight + sliderY);
		value = v;
		valueChanged(this, nullptr);
		this->Invalidate();
	}

	// show the pointer and make it moveable
	void enablePointer(bool p) {
		drawPointer = p;
		if (!p)
			value = -1.0;
		this->Invalidate();
	}

private:
	int width, height;		// of the surrounding panel, slider itself is inset
	int pointerHeight;
	bool drawPointer;		// show the pointer for rating
	int pointerX, pointerY;	// position of the slider's pointer
	int sliderX, sliderY;	// position of the slider background on the panel
	int sliderHeight;		// height of the slider
	int ticks;
	bool fixedTicks;
	float value;			// slider position between 0 and 1, internal use
	int min, max;			// lowest and highest value the slider can have
	System::Windows::Forms::Orientation orientation;

public:
	event EventHandler^ valueChanged;
};