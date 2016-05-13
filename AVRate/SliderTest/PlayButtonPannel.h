#pragma once

public ref class PlayVideoButton : public System::Windows::Forms::Button 
{
public:
	int videoIndx;

	PlayVideoButton() : System::Windows::Forms::Button() 
	{
		videoIndx = -1;
	}
};

public ref class PlayButtonPannel : public PlayVideoButton 
{

	System::Collections::Generic::List< PlayVideoButton^ >					ListButtons;
	System::Collections::Generic::List< System::Windows::Forms::Label^ >	ListLabels;
	System::Drawing::Size parentSize;

public:
	PlayButtonPannel();
	virtual ~PlayButtonPannel();
	System::Windows::Forms::Button^ AddButtonInPannel();
	void		updateButtonSize();

	inline void setParentSize(System::Drawing::Size ps)			{parentSize = ps;}
	void		setEnable(bool enable);
	void		disableAllButRef();
	
	inline System::Windows::Forms::Label^ 
				getLabel(int btnID)								{return ListLabels[btnID];}
	void		addComment(int btnID, System::String ^text);
	void		clearComments(int btnID);
	void		clearComments();

};
