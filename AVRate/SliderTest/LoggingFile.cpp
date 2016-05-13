#include "StdAfx.h"
#include "LoggingFile.h"

using namespace System::Collections::Generic;
using namespace System::IO;
using namespace System;

LoggingFile::LoggingFile() {
	//logFormat = "%d	%m	%r";
	logFormat = "%d %m %r";

	// look for a free LogXXX.txt filename
	System::String^ path;
	Directory::CreateDirectory("logs");
	int number = 0;
	bool found = false;
	while (!found) {
		path = System::String::Format("logs\\Log{0:d3}.txt", number);
		if (!System::IO::File::Exists(path)) {
			found = true;
			logFile = gcnew StreamWriter(path);
			break;
		}
		number++;
	}
}


// logs the list of results together with date, delta (how long the logging time was), 
// and playlist info (item name)
void LoggingFile::LogResult(List<float>^ l, String^ itemName) {
	String^ seperator = " ";
	String^ time = lastLogStart.ToString();
	time = time->Replace(" ", seperator);
	time += seperator + String::Format("{0,5:f1}", (DateTime::Now - lastLogStart).TotalSeconds) + "sec";
	//time += "	" + String::Format("{0:f1}", (DateTime::Now - lastLogStart).TotalSeconds);
	
	String^ rating = gcnew String("");
	bool intScale = false; // log all values as integers

	for (int i = 0; i < l->Count; i++) {
		float v = l[i];
		if (intScale)
			rating += String::Format("{0}", (int)v);
		else
			rating += String::Format("{0,4:f2}	", v);
	}

	logStr = gcnew String(logFormat);

	logStr = logStr->Replace(" ", seperator);
	logStr = logStr->Replace("%d", time);
	logStr = logStr->Replace("%r", rating);
	logStr = logStr->Replace("%m", itemName);
	logStr = logStr->Replace("\\\\","\\");
	logFile->WriteLine(logStr);
	logFile->Flush();
}
// logs the list of results together with date, delta (how long the logging time was), 
// and playlist info (item name)
void LoggingFile::LogResult(float v, String^ itemName) {
	String^ seperator = " ";
	itemName += seperator + String::Format("{0:f1}	", v);

	itemName = itemName->Replace("\\\\","\\");
	logFile->WriteLine(itemName);
	logFile->Flush();
}
void LoggingFile::LogHistResult(float v, int numtotal, String^ itemName) {
	String^ seperator = " ";
	itemName += seperator + String::Format("{0,5:f1}", v) + " of " + String::Format("{0}", numtotal);

	itemName = itemName->Replace("\\\\","\\");
	logFile->WriteLine(itemName);
	logFile->Flush();
}
#ifdef TESTMODE
void LoggingFile::LogResult(List<float>^ l,List<String^>^ xinfo, String^ itemName) {
	String^ time = "Run started " + lastLogStart.ToString();
	//time = time->Replace(" ", "	");
	//time += "	" + String::Format("{0:f1}", (DateTime::Now - lastLogStart).TotalSeconds);
	itemName += Environment::NewLine;
	itemName = itemName->Replace("\\\\","\\");
	time += ", Time from Start : " + String::Format("{0:f1}", (DateTime::Now - lastLogStart).TotalSeconds) + Environment::NewLine ;
	
	String^ rating = gcnew String("");
	bool intScale = false; // log all values as integers

	for (int i = 0; i < l->Count; i++) {
		float v = l[i];
		String^ xs = xinfo[i];
		if (intScale)
			rating += String::Format("{0}", (int)v);
		else
			rating += String::Format("{0:f1}	", v);
		rating += "TICK: " + xs + Environment::NewLine; //"  \r\n";
	}

	logStr = gcnew String(logFormat);

	logStr = logStr->Replace("%d", time);
	logStr = logStr->Replace("%r", rating);
	logStr = logStr->Replace("%m", itemName);
	logFile->WriteLine(logStr);
	logFile->Flush();
}
#endif
void LoggingFile::LogHeader() {
	String^ time = DateTime::Now.ToString();
	
	logFile->WriteLine("Log Created " + time);// TODO Multilangugage
	logFile->Flush();
}
void LoggingFile::Log(String^ s) {
	String^ time = DateTime::Now.ToString();
	//time = time->Replace(" ", "	");
		logFile->WriteLine("Log Created " + time + Environment::NewLine + s + Environment::NewLine );// TODO Multilangugage
	//logFile->WriteLine(time + "	" + s);
	logFile->Flush();
}

void LoggingFile::LogNoDate(System::String^ s) {
	logFile->WriteLine(s);
	logFile->Flush();
}