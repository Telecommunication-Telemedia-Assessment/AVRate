#pragma once
#include "AbstractSlider.h"
#include "MySlider.h"

public ref class KeypressSlider : public AbstractSlider
{
public:
	KeypressSlider();
	void playbackStarted();
	void playbackStopped();

	virtual void readFromSlider() override;
	virtual void storeVal(System::Windows::Forms::Keys k) override;
	#ifdef DEBUG
	virtual void readFromSliderLost() override ;
	#endif
private:
	int secsplayed;
	int msecsplayed;
	System::Diagnostics::Stopwatch^ stopwatch;
};
