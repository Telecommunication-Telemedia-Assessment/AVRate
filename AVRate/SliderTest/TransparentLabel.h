#pragma once

// Temporary try of a transparent Label,
// from
// http://social.msdn.microsoft.com/forums/en-US/winforms/thread/f7c12155-a4d6-4c50-a258-2ddbe5f4df81/

ref class TransparentLabel :
public System::Windows::Forms::Panel
{
public:
	TransparentLabel(void);
private: String mText;
    [Browsable(true)]
public: 
	property override System::String^ Text {
      get { return mText; }
      set { mText = value; this->Invalidate(); }
    } 
protected: void Invalidate() {
      if (this->Parent != null) this->Parent->Invalidate(this->Bounds);
      //base->Invalidate();
    }
protected: property override CreateParams CreateParams {
      get {
        CreateParams cp = base->CreateParams;
        cp->ExStyle |= 0x20;  // Turn on WS_EX_TRANSPARENT
        return cp;
      }
    }
    protected override void OnPaintBackground(PaintEventArgs e) {
      // Prevent erasing background
    }
    protected override void OnPaint(PaintEventArgs e) {
      // Paint text
      Rectangle rc = new Rectangle(0, 0, this.Width, this.Height);
      TextRenderer.DrawText(e.Graphics, this.Text, this.Font, rc, this.ForeColor, Color.Transparent);
    }
  }

};
