#pragma once

#include "Logging.h"

ref class LoggingDatabase : public Logging
{
public:
	LoggingDatabase();
	virtual void LogResult(System::Collections::Generic::List<float>^ l, System::String^ itemName) override;
	virtual void LogResult(float l, System::String^ itemName) override{};
	virtual void LogHistResult(float, int, System::String^ itemName) override{};   // logs the recorded hist data into logfile
#ifdef TESTMODE
	virtual void LogResult(System::Collections::Generic::List<float>^ l,System::Collections::Generic::List<System::String^>^ x, System::String^ itemName) override; // logs the recorded data into logfile
#endif
	virtual void Log(System::String^ s) override;
	virtual void LogNoDate(System::String^ s) override;
};
