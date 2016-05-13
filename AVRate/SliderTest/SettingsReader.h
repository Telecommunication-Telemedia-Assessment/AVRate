#pragma once

#include "Types.h"

ref class KeyValueSetting {
public:
	KeyValueSetting(System::String^ k, System::String^ v) : key(k->ToLower()), value(v) {}
	System::String^ key;
	System::String^ value;
};

ref class SettingsReader {
public:
	SettingsReader(System::String^ file);
	System::Collections::Generic::List<System::Collections::Generic::List<KeyValueSetting^>^>^ ReadSliderList();
	SliderTest::GeneralSettings^ ReadGeneralSettings();
	System::String^ lastErrorMessage;

private:
	System::Xml::XmlTextReader^ reader;
};
