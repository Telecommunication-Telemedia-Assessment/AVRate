#include "StdAfx.h"
#include "KeypressSlider.h"

KeypressSlider::KeypressSlider()
{
	stopwatch = gcnew System::Diagnostics::Stopwatch();
}
#ifdef DEBUG
void KeypressSlider::readFromSliderLost() 
{
	readFromSlider();
}
#endif
void KeypressSlider::readFromSlider() 
{

}
void KeypressSlider::storeVal(System::Windows::Forms::Keys k) 
{
	if (stopwatch->IsRunning) 		 
	{
		float secs = (stopwatch->ElapsedMilliseconds / 1000.f);
  		valueList.Add(secs);
	}
}
void KeypressSlider::playbackStarted() 
{
	valueList.Clear();
	stopwatch->Reset();
	stopwatch->Start();
}
void KeypressSlider::playbackStopped() 
{
	// Not entirely necessary; Timer will get resetted on a new start
	if (stopwatch->IsRunning) 		 
		stopwatch->Stop();
}
