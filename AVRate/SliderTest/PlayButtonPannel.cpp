#include "PlayButtonPannel.h"
#include <cmath>

PlayButtonPannel::PlayButtonPannel() 
{
	ListButtons.Add(this);
	ListLabels.Add(gcnew System::Windows::Forms::Label());
}

PlayButtonPannel::~PlayButtonPannel() {
	for(int i = 1 ; i < ListButtons.Count ; ++i) 
	{
		delete ListButtons[i];
	}
}

void PlayButtonPannel::updateButtonSize() 
{
	static System::Drawing::Size BASELINE_SIZE(this->Size);
	bool fit = true;

	// -----------------------------------------------------------------
	// what should be the size of the buttons
	System::Drawing::Size buttonSize;
	if(ListButtons.Count * (BASELINE_SIZE.Width + 10) > parentSize.Width)
	{
		buttonSize.Width = (parentSize.Width - 10*(ListButtons.Count+1)) / ListButtons.Count;
		buttonSize.Height = (int)(BASELINE_SIZE.Height * std::sqrt((float)(buttonSize.Width) / BASELINE_SIZE.Width));
	} else
	{
		buttonSize = BASELINE_SIZE;
	}


	// ------------------------------------------------------------------
	// update button positions & sizes

	int space = (parentSize.Width-20 - buttonSize.Width * ListButtons.Count) / (ListButtons.Count+1);
	for(int i = 0 ; i < ListButtons.Count ; ++i) 
	{
		PlayVideoButton^ b = ListButtons[i];		
		b->Location = System::Drawing::Point( space*(i+1)+i*buttonSize.Width
											, this->Location.Y);
		b->Size = buttonSize;

		ListLabels[i]->Location = System::Drawing::Point(b->Location.X, b->Location.Y + b->Size.Height);
		ListLabels[i]->Size = buttonSize;
	}
}

System::Windows::Forms::Button^ PlayButtonPannel::AddButtonInPannel()
{
	static System::Drawing::Size BASELINE_SIZE(this->Size);

	PlayVideoButton^ newButton = gcnew PlayVideoButton();
	ListButtons.Add(newButton);
	ListLabels.Add(gcnew System::Windows::Forms::Label());

	bool fit = true;

	// -----------------------------------------------------------------
	// what should be the size of the buttons
	System::Drawing::Size buttonSize;
	if(ListButtons.Count * (BASELINE_SIZE.Width + 10) > parentSize.Width)
	{
		buttonSize.Width = (parentSize.Width - 10*(ListButtons.Count+1)) / ListButtons.Count;
		buttonSize.Height = (int)(BASELINE_SIZE.Height * std::sqrt((float)(buttonSize.Width) / BASELINE_SIZE.Width));
	} else
	{
		buttonSize = BASELINE_SIZE;
	}



	// ------------------------------------------------------------------
	// update button positions & sizes

	int space = (parentSize.Width-20 - buttonSize.Width * ListButtons.Count) / (ListButtons.Count+1);
	//int tab[3] = {165, 10, 510};
	int c = 64;
	for(int i = 0 ; i < ListButtons.Count ; ++i) 
	{
		PlayVideoButton^ b = ListButtons[i];		
		b->Location = System::Drawing::Point( space*(i+1)+i*buttonSize.Width
											, this->Location.Y);
		b->Size = buttonSize;
		b->videoIndx = i;
		if(i > 0) 
		{
			++c;
			b->Text = gcnew System::String((char) c,1);
		}
		else
		{
			b->Text = "Ref";
		}

		ListLabels[i]->Location = System::Drawing::Point(b->Location.X, b->Location.Y + b->Size.Height);
		ListLabels[i]->Size = buttonSize;
		ListLabels[i]->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
	}

	newButton->Font = this->Font; 
	newButton->Location = System::Drawing::Point(ListButtons[ListButtons.Count-1]->Location.X + 10, ListButtons[ListButtons.Count-1]->Location.Y);
	newButton->Name = L"btnPlay" + ListButtons.Count;
	newButton->TabIndex = this->TabIndex+1;
	newButton->TabStop = false;
	newButton->UseVisualStyleBackColor = this->UseVisualStyleBackColor;


	return newButton;
}

void PlayButtonPannel::setEnable(bool enable)
{
	for(int i = 0 ; i < ListButtons.Count ; ++i) 
	{
		ListButtons[i]->Enabled = enable;	
	}
}

void PlayButtonPannel::disableAllButRef() 
{
	for(int i = 1 ; i < ListButtons.Count ; ++i)
		ListButtons[i]->Enabled = false;
}

void PlayButtonPannel::addComment(int btnID, System::String ^text) 
{
	if(btnID == -1)
		return;

	if(ListLabels[btnID]->Text->Length == 0)
		ListLabels[btnID]->Text = text;
	else
		ListLabels[btnID]->Text += "\n" + text;
}

void PlayButtonPannel::clearComments(int btnID)
{
	if(btnID >= 0)
		ListLabels[btnID]->Text = "";
}

void PlayButtonPannel::clearComments()
{
	for(int i = 1 ; i < ListLabels.Count ; ++i)
		ListLabels[i]->Text = "";
}

