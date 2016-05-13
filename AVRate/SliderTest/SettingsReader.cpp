#include "StdAfx.h"
#include "SettingsReader.h"

using namespace System::Collections::Generic;
using namespace System::Xml;
using namespace System;
using namespace SliderTest;


SettingsReader::SettingsReader(String^ file) {
	reader = gcnew XmlTextReader(file);
}

List<List<KeyValueSetting^>^>^ SettingsReader::ReadSliderList() {
	List<KeyValueSetting^>^ currentList;
	List<List<KeyValueSetting^>^>^ sliderLists = gcnew List<List<KeyValueSetting^>^>;

	reader->WhitespaceHandling = Xml::WhitespaceHandling::None;

	bool doParsing = false;

	while (reader->Read()) {
		String^ name;
		String^ value;
	
		name = reader->Name;

		// found beginning of a slider entry, now write all its props into a list
		if (name->ToLower() == "slider") {
			if (!doParsing) {
				currentList = gcnew List<KeyValueSetting^>;
				sliderLists->Add(currentList);
				currentList->Add(gcnew KeyValueSetting("type", "slider"));
				doParsing = true;
			}
			else
				doParsing = false;
		}
		else if (name->ToLower() == "buttons") {
				if (!doParsing) {
				currentList = gcnew List<KeyValueSetting^>;
				sliderLists->Add(currentList);
				currentList->Add(gcnew KeyValueSetting("type", "buttons"));
				doParsing = true;
			}
			else
				doParsing = false;
		}
		else if (name != "" && doParsing) {
			value = reader->ReadString();
			currentList->Add(gcnew KeyValueSetting(name, value));
		}
	}

	return sliderLists;
}


SliderTest::GeneralSettings^ SettingsReader::ReadGeneralSettings() {
	List<KeyValueSetting^>^ currentList = gcnew List<KeyValueSetting^>;

	reader->WhitespaceHandling = Xml::WhitespaceHandling::None;

	bool doParsing = false;

	while (reader->Read()) {
		String^ name;
		String^ value;
	
		name = reader->Name;

		if (name->ToLower() == "general") {
			if (!doParsing)
				doParsing = true;
			else
				doParsing = false;
		}
		else if (name != "" && doParsing) {
			value = reader->ReadString();
			currentList->Add(gcnew KeyValueSetting(name, value));
		}
	}

	
	GeneralSettings^ generalSettings = gcnew GeneralSettings();

	// obligatory settings that need to be set
	SettingTypes bitNeeded =	
					SettingTypes::Rating |
					SettingTypes::Device |
					SettingTypes::Display | 
					SettingTypes::Layout | 
					SettingTypes::VolumeFormType | 
					SettingTypes::SleepTime | 
					SettingTypes::RatingMethodRule | 
					SettingTypes::RatingMethodBreakCount | 
					SettingTypes::RatingMethodType ;
	SettingTypes bitSet=SettingTypes::Zero;

	// Set Default settings first
				generalSettings->rating = Rating::End;
				generalSettings->rmtype = RatingMethodType::Standard;
				generalSettings->device = Device::Software;
				generalSettings->layout = SliderTest::Layout::Vertical;
				generalSettings->media = SliderTest::Media::Video;
				generalSettings->display = SliderTest::Display::Local;
				generalSettings->repeat = false;
				generalSettings->localPlayer = LocalPlayer::WMP;
				generalSettings->volumeFormType = VolumeFormType::None;
				generalSettings->userculture = gcnew System::Globalization::CultureInfo("en");
				generalSettings->systemculture = gcnew System::Globalization::CultureInfo("en");
				//generalSettings->sleepTime = -1;

		// settings from lists to GeneralSettings
	for each (KeyValueSetting^ kv in currentList) {
		if (kv->key == "rating") {
			bitSet = bitSet | SettingTypes::Rating;
			if (kv->value->ToLower() == "continuous")
				generalSettings->rating = Rating::Continuous;
			else if (kv->value->ToLower() == "end")
				generalSettings->rating = Rating::End;
			else if (kv->value->ToLower() == "sync")
			{
				generalSettings->logKeypress=true;
				generalSettings->rating = Rating::Sync;
			}
			else
			{
				bitSet = bitSet & ~SettingTypes::Rating;
			}
		}
		else if (kv->key == "ratingmethod") {
			bitSet = bitSet | SettingTypes::RatingMethodType;
			if (kv->value->ToLower() == "standard")
			{
				generalSettings->rmtype = RatingMethodType::Standard;
				bitSet = bitSet | SettingTypes::RatingMethodRule;// cos not needed
				bitSet = bitSet | SettingTypes::RatingMethodBreakCount;// cos not needed
			}
			else if (kv->value->ToLower() == "constant")
			{
				generalSettings->rmtype = RatingMethodType::Constant;
				bitSet = bitSet | SettingTypes::RatingMethodRule;// cos not needed
				if((bitSet & SettingTypes::RatingMethodBreakCount) == SettingTypes::Zero)
				{
					generalSettings->ratingmethodbreakcount = 10;// set to default
					bitSet = bitSet | SettingTypes::RatingMethodBreakCount;
				}
			}
			else if (kv->value->ToLower() == "staircase")
			{
				generalSettings->rmtype = RatingMethodType::Staircase;
				if((bitSet & SettingTypes::RatingMethodRule) == SettingTypes::Zero)
				{
					generalSettings->ratingmethoduptarget = 1;// set to default
					generalSettings->ratingmethoddowntarget = 1;
					bitSet = bitSet | SettingTypes::RatingMethodRule;
				}
				if((bitSet & SettingTypes::RatingMethodBreakCount) == SettingTypes::Zero)
				{
					generalSettings->ratingmethodbreakcount = 3;// set to default
					bitSet = bitSet | SettingTypes::RatingMethodBreakCount;
				}
			}
			else if (kv->value->ToLower() == "double")
			{
				generalSettings->rmtype = RatingMethodType::Double;
				if((bitSet & SettingTypes::RatingMethodRule) == SettingTypes::Zero)
				{
					generalSettings->ratingmethoduptarget = 1;// set to default
					generalSettings->ratingmethoddowntarget = 1;
					bitSet = bitSet | SettingTypes::RatingMethodRule;
				}
				if((bitSet & SettingTypes::RatingMethodBreakCount) == SettingTypes::Zero)
				{
					generalSettings->ratingmethodbreakcount = 3;// set to default
					bitSet = bitSet | SettingTypes::RatingMethodBreakCount;
				}
			}
			else if (kv->value->ToLower() == "samviq")
			{
				generalSettings->rmtype = RatingMethodType::SAMVIQ;
				bitSet = bitSet | SettingTypes::RatingMethodRule;// cos not needed
				bitSet = bitSet | SettingTypes::RatingMethodBreakCount;// cos not needed

			}
			else
			{
				bitSet = bitSet & ~SettingTypes::RatingMethodType;
			}
		}
		else if (kv->key == "ratingmethodrule") {
			//if(	generalSettings->rmtype == RatingMethodType::Standard )
			if( (generalSettings->rmtype == RatingMethodType::Staircase ) ||
					(generalSettings->rmtype == RatingMethodType::Double ) )
			{
				generalSettings->ratingmethoduptarget = 0;
				generalSettings->ratingmethoddowntarget = 0;
				for(int i=0; i<kv->value->Length;i++)
				{
					String^ t = kv->value[i].ToString();

					if((t=="U")||(t=="u"))

						generalSettings->ratingmethoduptarget++;
					else if((t=="D")||(t=="d"))

						generalSettings->ratingmethoddowntarget++;
				}
				if( (generalSettings->ratingmethoduptarget) && (generalSettings->ratingmethoddowntarget) )
					bitSet = bitSet | SettingTypes::RatingMethodRule;
			}
		}
		else if (kv->key == "ratingmethodbreakcount") {
			if( (generalSettings->rmtype == RatingMethodType::Constant ) ||
				(generalSettings->rmtype == RatingMethodType::Staircase) ||
				(generalSettings->rmtype == RatingMethodType::Double ) )
			{
				generalSettings->ratingmethodbreakcount = Convert::ToInt32(kv->value);
				if(generalSettings->ratingmethodbreakcount)
					bitSet = bitSet | SettingTypes::RatingMethodBreakCount;
			}
		}
		else if (kv->key == "device") {
			bitSet = bitSet | SettingTypes::Device;
			if (kv->value->ToLower() == "software")
				generalSettings->device = Device::Software;
			else if (kv->value->ToLower() == "hardware")
				generalSettings->device = Device::Hardware;
			else if (kv->value->ToLower() == "tablet")
				generalSettings->device = Device::Tablet;
			else {
				bitSet = bitSet & ~SettingTypes::Device;
			}
		}
		else if (kv->key == "playondevice") {
			if(kv->value->ToLower() == "true")
				generalSettings->playOnDevice = true;
		}
		else if (kv->key == "layout") {
			bitSet = bitSet | SettingTypes::Layout;
			if (kv->value->ToLower() == "horizontal")
				generalSettings->layout = SliderTest::Layout::Horizontal;
			else if (kv->value->ToLower() == "vertical")
				generalSettings->layout = SliderTest::Layout::Vertical;
			else if (kv->value->ToLower() == "p800")
				generalSettings->layout = SliderTest::Layout::P800;
			else if (kv->value->ToLower() == "onscreen")
				generalSettings->layout = SliderTest::Layout::OnScreen;
			else
			{
				bitSet = bitSet & ~SettingTypes::Layout;
			}
		}
		else if (kv->key == "media") {
			bitSet = bitSet | SettingTypes::Media;
			if (kv->value->ToLower() == "audio")
				generalSettings->media = SliderTest::Media::Audio;
			else if (kv->value->ToLower() == "video")
				generalSettings->media = SliderTest::Media::Video;
			else
			{
				bitSet = bitSet & ~SettingTypes::Media;
			}
		}
		else if (kv->key == "display") {
			bitSet = bitSet | SettingTypes::Display;
			if (kv->value->ToLower() == "local")
			{
				generalSettings->display = SliderTest::Display::Local;
				if((bitSet & SettingTypes::SleepTime) == SettingTypes::Zero)
				{
					bitSet = bitSet | SettingTypes::SleepTime;
					generalSettings->sleepTime = DEFAULTSLEEPTIME_LOCAL;
				}
			}
			else if (kv->value->ToLower() == "remote")
			{
				generalSettings->display = SliderTest::Display::Remote;
				if((bitSet & SettingTypes::SleepTime) == SettingTypes::Zero)
				{
					bitSet = bitSet | SettingTypes::SleepTime;
					generalSettings->sleepTime = DEFAULTSLEEPTIME_REMOTE;
				}
			}
			else if (kv->value->ToLower() == "server")
			{
				generalSettings->display = SliderTest::Display::Server;
				if((bitSet & SettingTypes::SleepTime) == SettingTypes::Zero)
				{
					bitSet = bitSet | SettingTypes::SleepTime;
					generalSettings->sleepTime = DEFAULTSLEEPTIME_SERVER;
				}			}
			else
			{
				bitSet = bitSet & ~SettingTypes::Display;
			}
		}
		else if (kv->key == "remotehost") {
			generalSettings->remoteHost = kv->value;
		}
		else if (kv->key == "repeat") {
			bitSet = bitSet | SettingTypes::Repeat;
			if (kv->value == "false")
				generalSettings->repeat = false;
			else if (kv->value == "true")
				generalSettings->repeat = true;
			else
			{
				bitSet = bitSet & ~SettingTypes::Repeat;
			}
		}
		else if (kv->key == "localplayer") {
			bitSet = bitSet | SettingTypes::LocalPlayer;
			if (kv->value->ToLower() == "mplayer")
				generalSettings->localPlayer = LocalPlayer::MPlayer;
			else if (kv->value->ToLower() == "vlc10")
				generalSettings->localPlayer = LocalPlayer::VLC10;
			else if (kv->value->ToLower() == "wmp")
				generalSettings->localPlayer = LocalPlayer::WMP;
			else if (kv->value->ToLower() == "stereoscopic-player-nvidia")
				generalSettings->localPlayer = LocalPlayer::STEREOSCOPIC_PLAYER_NVIDIA;
			else if (kv->value->ToLower() == "stereoscopic-player-sbs")
				generalSettings->localPlayer = LocalPlayer::STEREOSCOPIC_PLAYER_SBS;
			else if (kv->value->ToLower() == "command_line")
				generalSettings->localPlayer = LocalPlayer::COMMAND_LINE;
			else
			{
				bitSet = bitSet & ~SettingTypes::LocalPlayer;
			}
		}
		else if (kv->key == "playerpath") {
			generalSettings->playerPath = kv->value;
		}
		else if (kv->key == "volumeform") {
			bitSet = bitSet | SettingTypes::VolumeFormType;
			if (kv->value->ToLower() == "none")
				generalSettings->volumeFormType = VolumeFormType::None;
			else if (kv->value->ToLower() == "button")
				generalSettings->volumeFormType = VolumeFormType::Button;
			else if (kv->value->ToLower() == "slider")
				generalSettings->volumeFormType = VolumeFormType::Slider;
			else
			{
				bitSet = bitSet & ~SettingTypes::VolumeFormType;
			}
		}
		else if (kv->key == "userlanguage") {
			bitSet = bitSet | SettingTypes::UserLanguage;
			if (kv->value->ToLower() == "en")
				generalSettings->userculture = gcnew System::Globalization::CultureInfo("en");
			else if (kv->value->ToLower() == "de")
				generalSettings->userculture = gcnew System::Globalization::CultureInfo("de");
			else
				bitSet = bitSet & ~SettingTypes::UserLanguage;
		}
		else if (kv->key == "systemlanguage") {
			bitSet = bitSet | SettingTypes::SystemLanguage;
			if (kv->value->ToLower() == "en")
				generalSettings->systemculture = gcnew System::Globalization::CultureInfo("en");
			else if (kv->value->ToLower() == "de")
				generalSettings->systemculture = gcnew System::Globalization::CultureInfo("de");
			else
				bitSet = bitSet & ~SettingTypes::SystemLanguage;
		}
		else if (kv->key == "logging") {
			if (kv->value->ToLower() == "file")
				generalSettings->logToFile = true;
			else if (kv->value->ToLower() == "database")
				generalSettings->logToDatabase = true;
		}
		else if (kv->key == "waittime") {
			generalSettings->waitTimeRating = Convert::ToInt32(kv->value) * 1000;
		}
		else if (kv->key == "sleeptime") {
			// Default settings are set in Display selection
			if(kv->value != "")
			{
				bitSet = bitSet | SettingTypes::SleepTime;
				generalSettings->sleepTime = Convert::ToInt32(kv->value);
			}
		}
		else if (kv->key == "showsequencename") {
			if(kv->value != "") 
			{
				generalSettings->showSequenceName = Convert::ToInt32(kv->value);
			}

		}
		else if (kv->key == "samplinginterval") {
			generalSettings->samplingInterval = Convert::ToInt32(kv->value);
		} else if(kv->key == "playerarguments") {
			generalSettings->playerArguments = kv->value;
		}

	}

	bitSet = bitSet & bitNeeded;	// take only the needed bits to confirm
	if(bitSet != bitNeeded)
	{
		bitSet = bitSet ^ bitNeeded;
		System::Resources::ResourceManager^ resman = gcnew System::Resources::ResourceManager("SliderTest.UImsg",System::Reflection::Assembly::GetExecutingAssembly());
		String^ errmsg = resman->GetString("MSG_checkset", generalSettings->systemculture);
		lastErrorMessage = errmsg + Convert::ToString(bitSet);
		return(nullptr);
	}

	// check for Playerpath
	if(	(generalSettings->display == SliderTest::Display::Local) )
	{	
		if( (generalSettings->localPlayer == LocalPlayer::MPlayer) ||
			(generalSettings->localPlayer == LocalPlayer::VLC10) ||
			(generalSettings->localPlayer == LocalPlayer::STEREOSCOPIC_PLAYER_SBS) ||
			(generalSettings->localPlayer == LocalPlayer::COMMAND_LINE) ||
			(generalSettings->localPlayer == LocalPlayer::STEREOSCOPIC_PLAYER_NVIDIA))
		{
			if(!(System::IO::File::Exists(generalSettings->playerPath)))
			{
				System::Resources::ResourceManager^ resman = gcnew System::Resources::ResourceManager("SliderTest.UImsg",System::Reflection::Assembly::GetExecutingAssembly());
				lastErrorMessage = resman->GetString("LocalPlayerNotFound", generalSettings->systemculture);
				return(nullptr);
			}
		}
	}
	// check for non-working combinations
	if( (generalSettings->display == SliderTest::Display::Remote) &&
		(generalSettings->rating == Rating::Continuous) )
	{
		System::Resources::ResourceManager^ resman = gcnew System::Resources::ResourceManager("SliderTest.UImsg",System::Reflection::Assembly::GetExecutingAssembly());
		lastErrorMessage = resman->GetString("SetCombiNotSupported", generalSettings->systemculture);
		return(nullptr);
	}
	return generalSettings;
}