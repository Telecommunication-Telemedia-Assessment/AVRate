#include "StdAfx.h"
#include "LoggingDatabase.h"

LoggingDatabase::LoggingDatabase(void) {
}
#ifdef TESTMODE
void LoggingDatabase::LogResult(System::Collections::Generic::List<float>^ l,System::Collections::Generic::List<System::String^>^ x, System::String^ itemName) {
}
#endif

void LoggingDatabase::LogResult(System::Collections::Generic::List<float>^ l, System::String^ itemName) {
}

void LoggingDatabase::Log(System::String^ s) {
}

void LoggingDatabase::LogNoDate(System::String^ s) {
}