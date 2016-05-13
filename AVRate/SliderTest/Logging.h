#pragma once

#include "Types.h"

// abstract base class for all logging mechanisms
public ref class Logging abstract {
public:
	Logging() {};
	
	virtual void LogResult(System::Collections::Generic::List<float>^ l, System::String^ itemName) = 0; // logs the recorded data into logfile
	virtual void LogResult(float, System::String^ itemName) = 0; // logs single recorded data into logfile
	virtual void LogHistResult(float, int, System::String^ itemName) = 0;   // logs the recorded hist data into logfile
#ifdef TESTMODE
	virtual void LogResult(System::Collections::Generic::List<float>^ l,System::Collections::Generic::List<System::String^>^ x, System::String^ itemName) = 0; // logs the recorded data into logfile
#endif
	virtual void Log(System::String^ s) abstract; // log a single string with date
	virtual void LogNoDate(System::String^ s) abstract; // log a single string without time
	void LogStart(); // store the start time of the next item to log
	void SetLogFormat(System::String^ l) { logFormat = l; }

protected:
	System::DateTime lastLogStart;
	System::String^ logFormat; // %d date & time, %m movie info, %r rating
	System::String^ logStr; // gets set in logResult() and finally written to file in writeLogStr
};