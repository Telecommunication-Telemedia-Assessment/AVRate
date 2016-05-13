#pragma once

#include "Types.h"
#include "Logging.h"

public ref class LoggingFile : Logging
{
public:
	LoggingFile();
	
	virtual void LogResult(System::Collections::Generic::List<float>^ l, System::String^ itemName) override; // logs the recorded data into logfile
	virtual void LogResult(float, System::String^ itemName)  override; // logs the recorded single data into logfile
	virtual void LogHistResult(float, int, System::String^ itemName) override;   // logs the recorded hist data into logfile
#ifdef TESTMODE
	virtual void LogResult(System::Collections::Generic::List<float>^ l,System::Collections::Generic::List<System::String^>^ x, System::String^ itemName) override; // logs the recorded data into logfile
#endif
	virtual void Log(System::String^ s) override;
	virtual void LogHeader(void) ;
	virtual void LogNoDate(System::String^ s) override;

private:
	System::IO::StreamWriter ^logFile;
};