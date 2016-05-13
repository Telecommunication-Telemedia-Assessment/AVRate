#pragma once

public ref class AbstractSlider abstract {// nonpublic sr
public:
	AbstractSlider(void);

	float getSliderValue(); // returns the last read value from slider

	System::Collections::Generic::List<float>^ getValueList();
	void StartCapture(); // add sampled values to the list (clears the list)
	void StopCapture();

	virtual bool getButton() { return false; }
	virtual void readFromSlider() abstract;	// internally read a value from the slider and 
											//add it to the list/update the current value
	virtual void storeVal(System::Windows::Forms::Keys k) {};
protected:
	float analogueValf; // slider value between 0 and 1
	System::Collections::Generic::List<float> valueList;
	bool capturing;
	System::Threading::Timer^ sampleTimer;
#ifdef TESTMODE
public:	System::Collections::Generic::List<System::String^>^ getXinfoList();
public:	virtual void readFromSlider(System::String^) abstract;	// internally read a value from the slider and //add it to the list/update the current value
protected:	System::Collections::Generic::List<System::String^> xinfoList;
#endif
#ifdef DEBUG
public:	virtual void readFromSliderLost() abstract;	// internally read a value from the slider and //add it to the list/update the current value
#endif
};
