#include "StdAfx.h"
#include "Controller.h"
#include "Playlist.h"
#include "AbstractSlider.h"
#include "LoggingFile.h"
#include "LoggingDatabase.h"
#include "KeyboardHook.h"

using namespace System;
using namespace System::Globalization;
using namespace System::Resources;
using namespace System::Collections::Generic;
using namespace System::Windows::Forms;

Controller::Controller(GeneralSettings^ gs) {
//Controller::Controller(GeneralSettings^ gs, System::Windows::Forms::Form^ pform) {
	playlist = gcnew Playlist(this);
	lastAuto = true;
	lastAutoDouble = true;
	generalSettings = gs;
	endplay = false;
	disableClipping = false;
	currentPos = -1;
	currentPosDouble = -1;
	currentAnswer = -1;	//if 0 then first lastAnswer not correct
	lastDirection = 0;
	lastDirectionDouble = 0;
	paneNumber = 0;
	//lastWasTurn = false;
	//lastWasTurnDouble = false;
	errorInitializing = false;
	videoIndexes = gcnew System::Collections::Generic::List<int>();

	if(gs->logKeypress)
	{
		kHook = gcnew KeyboardHook(true, this);
		kHook->installKeyboardHook();
	}

	splitLogs = (gs->listID->Count > 1);
	listLogging = gcnew List<Logging^>;
	int i = 0;
	// for every ID in the listID (= every user) generate logging objects (one for every 
	// specified logging mechanism (file, database etc.))
	for each (String^ s in gs->listID) {
		if ( (gs->logToFile) )
		{
			LoggingFile^ l = gcnew LoggingFile();
			l->LogHeader();
			System::String^ sep = ": ";
			l->LogNoDate("Settings File: " + gs->settingsFile );
			l->LogNoDate("Playlist File: " + gs->playlist); 
			l->LogNoDate("ID: " + gs->listID[i]+ Environment::NewLine);
			l->LogNoDate("--- Settings -------------------------"  + Environment::NewLine );
			//l->LogNoDate("Rating " + sep + Convert::ToString(gs->rating) );
			//l->LogNoDate("RatingMethodType " + sep + Convert::ToString(gs->rmtype));
			//l->LogNoDate("RatingMethodRule " + sep + Convert::ToString(gs->ratingmethodrule));
			l->LogNoDate("Up rule count " + sep + Convert::ToString(gs->ratingmethoduptarget) );
			l->LogNoDate("Down rule count " + sep + Convert::ToString(gs->ratingmethoddowntarget) );
			l->LogNoDate("RatingMethodBreakCount " + sep + Convert::ToString(gs->ratingmethodbreakcount));
			//l->LogNoDate("Device " + sep + Convert::ToString(gs->device) );
			//l->LogNoDate("Layout " + sep + Convert::ToString(gs->layout) );
			//l->LogNoDate("Media " + sep + Convert::ToString(gs->media) );
			//l->LogNoDate("Display " + sep + Convert::ToString(gs->display) );
			l->LogNoDate("Sleeptime " + sep + Convert::ToString(static_cast<unsigned int>(gs->sleepTime)) );
			l->LogNoDate("Repeat " + sep + Convert::ToString(gs->repeat) );
			l->LogNoDate("Samplinginterval " + sep + Convert::ToString(gs->samplingInterval) );
			//l->LogNoDate("LocalPlayer " + sep + Convert::ToString(gs->localPlayer) );
			l->LogNoDate("Waittime " + sep + Convert::ToString(gs->waitTimeRating) );
			l->LogNoDate(Environment::NewLine + "--- Results ---------------------------"  + Environment::NewLine);
			listLogging->Add(l);
			i++;
		}
	}

	i = 0;
	for each (String^ s in gs->listID) {
		if (generalSettings->logToDatabase) {
			LoggingDatabase^ l = gcnew LoggingDatabase();
			l->Log("Playlist: " + generalSettings->playlist + Environment::NewLine + 
				"ID: " + generalSettings->listID[i]);
			listLogging->Add(l);
			i++;
		}
	}

	int result = playlist->Initialize(generalSettings);
	if(result)
	{
		errorInitializing = true;
		for each (Logging^ l in listLogging)
			l->LogNoDate("Error in playlist");
	}

	listSlider = gcnew List<AbstractSlider^>;
	listKeySlider = gcnew List<AbstractSlider^>;
	listVolumeValues = gcnew List<float>;

	// Make a useful copy of the playlist for Histogram evaluation
	fullNumPlayedList = gcnew List<int>;
	fullAnswerList = gcnew List<float>;
	fullNameList = gcnew List<String^>;
	turnNameList = gcnew List<String^>;
	turnNameListDouble = gcnew List<String^>;
	fullNumTurnsList = gcnew List<int>;
	int num = playlist->numAutoItems + playlist->numPlayableItems;
	for (int i=0; i<num; i++)
	{
		fullNumPlayedList->Add(0);
		fullAnswerList->Add(0);
		fullNameList->Add(playlist->getItemNameAt(i));
		fullNumTurnsList->Add(0);
	}

	volume = 1;
}
void Controller::quitAppRegular(void) {
	// Quits without error messages
	
	disableClipping = true;
	if(parentForm != nullptr)
		parentForm->Hide();

	if(kHook != nullptr)
		kHook->removeKeyboardHook();

	finishAllOpenLogs();

	if((generalSettings->startedFromConsole == true) && 
		(generalSettings->keyOnExit == true) )
	{
		if(generalSettings->silentProcessing == false)
			Console::WriteLine(kkeyPressStr);
		Console::ReadKey();
		if(generalSettings->silentProcessing == false)
			Console::Write(Environment::NewLine);
	}
	Application::Exit();
}

void Controller::setSlider(AbstractSlider^ sl) {
	listSlider->Add(sl);
}
void Controller::setKeySlider(AbstractSlider^ sl) {
	listKeySlider->Add(sl);
	keySlider = (KeypressSlider^)sl;
}

void Controller::switchPlaylistSet() {
	// Swap the data set for RatingMethod Double
	int cp = currentPos;
	float ca = currentAnswer;
	int ldir = lastDirection;
	float lans = lastAnswer;
	bool lauto = lastAuto;
	String^ liname = lastItemName;
	String^ lname = lastName;
	//bool lturn = lastWasTurn;
	System::Collections::Generic::List<System::String^>^ tlist = turnNameList;

	currentPos = currentPosDouble;
	currentAnswer = currentAnswerDouble;
	lastDirection = lastDirectionDouble;
	lastAnswer = lastAnswerDouble;
	lastAuto = lastAutoDouble;
	lastItemName = lastItemNameDouble;
	lastName = lastNameDouble;
	//lastWasTurn = lastWasTurnDouble;
	turnNameList = turnNameListDouble;
	
	currentPosDouble = cp;
	currentAnswerDouble = ca;
	lastDirectionDouble = ldir;
	lastAnswerDouble = lans;
	lastAutoDouble = lauto;
	lastItemNameDouble = liname;
	lastNameDouble = lname;
	//lastWasTurnDouble = lturn;
	turnNameListDouble = tlist;
}
float Controller::getCurrentAnswer() {
	//if (splitLogs) 
	//	MessageBox("splitLogs not handled\n");
	return (currentAnswer);
}
/*int Controller::getLastAnswer() {
	if (splitLogs) 
		MessageBox("splitLogs not handled\n");
	return int(lastAnswer);
}*/
float Controller::getLastAnswer() {
	//if (splitLogs) 
	//	MessageBox("splitLogs not handled\n");
	return (lastAnswer);
}
int Controller::getLastDirection() {
	return (lastDirection);
}
void Controller::setLastDirection(int ldir) {
	lastDirection = ldir;
}
/*
bool Controller::getLastWasTurn() {
	return (lastWasTurn);
}
void Controller::setLastWasTurn(bool state) {
	lastWasTurn = state;
}*/
void Controller::PlayInit() {
		playlist->PlayInit();
}
void Controller::PlayNext() {
	endplay = false;
	playlist->PlayNext();
}
System::String^ Controller::VirtualPlayNext() {
	endplay = true;
	return playlist->VirtualPlayNext();
}
void Controller::PreparePoolSequences()
{
	playlist->getsetNextItem();
	if(currentPos < 0)
	{
		String^ result="";
		switch(currentPos)
		{
			case RESULT_BEGINOFPLAYLIST :	result=playlist->resman->GetString("RESULT_BEGINOFPLAYLIST",getSystemCulture());	break;
			case RESULT_MAXNUMTURNS :		result=playlist->resman->GetString("RESULT_MAXNUMTURNS",getSystemCulture());	break;
			case RESULT_MINEXCEEDED :		result=playlist->resman->GetString("RESULT_MINEXCEEDED",getSystemCulture());	break;
			case RESULT_MAXEXCEEDED :		result=playlist->resman->GetString("RESULT_MAXEXCEEDED",getSystemCulture());	break;
			case RESULT_ALLITEMSPLAYED :	result=playlist->resman->GetString("RESULT_ALLITEMSPLAYED",getSystemCulture());	break;
			default :	result="";	break;
		}
		LogNoDate(result);
		quitAppRegular();
	}
	videoIndexes->Clear();
	++paneNumber;
}
void Controller::PlayOneNext(int i) {
	endplay = false;
	playlist->PlayOneNext(i);
}
void Controller::PlayNext(System::Object ^, System::ComponentModel::DoWorkEventArgs ^) {
	PlayNext();
}
void Controller::PlayCurrent() {
	playlist->PlayCurrent();
}
void Controller::PlayCurrent(System::Object ^, System::ComponentModel::DoWorkEventArgs ^) {
	PlayCurrent();
}
System::String^ Controller::getLastItemName(void)
{
	return lastItemName;
}
void Controller::invokeEndPlayEvent(String^ name) {
		lastItemName=name;
		endplay=true;
	}
void Controller::Log(String^ s) {
	for each (Logging^ l in listLogging)
		l->Log(s);
}
void Controller::LogNoDate(String^ s) {
	for each (Logging^ l in listLogging)
		l->LogNoDate(s);
}

void Controller::StartLogging() {
	for each (Logging^ l in listLogging)
		l->LogStart();
}

void Controller::StopLogging(String^ itemName) {
	lastName = itemName;
}

void Controller::StartCapture() {
	for each (AbstractSlider^ s in listSlider)
		s->StartCapture();
	for each (AbstractSlider^ s in listKeySlider)
		s->StartCapture();
}

void Controller::StopCapture() {
	for each (AbstractSlider^ s in listSlider)
		s->StopCapture();
	for each (AbstractSlider^ s in listKeySlider)
		s->StopCapture();
}

int Controller::GetVideoPos() {
	// Gets video position (STARTING FROM 0)
	// NOT CHECKED for Double Staircase method!
	return playlist->itemPos;
}
int Controller::GetItemsPlayed() {
	return playlist->itemsPlayed;
}
int Controller::GetTotalVideoNumber() {
	return playlist->numPlayableItems;
}
int Controller::GetPaneNumber() {
	return paneNumber;
}
int Controller::GetNumberOfcomparedSequences() {
	return playlist->numComparedSequences;
}

int Controller::GetNumAutoItems() {
	return playlist->numAutoItems;
}

bool Controller::GetSliderButton() {
	return listSlider[0]->getButton();
}
CultureInfo^ Controller::getUserCulture() 
{
	return (generalSettings->userculture);
}
CultureInfo^ Controller::getSystemCulture() 
{
	return (generalSettings->systemculture);
}
System::String^ Controller::getUserCultureStr(String^ str) 
{
	ResourceManager^ resman = gcnew ResourceManager("SliderTest.UImsg",System::Reflection::Assembly::GetExecutingAssembly());//typeof(FormSettings).Assembly);
	str = resman->GetString(str, generalSettings->userculture);
	return (str);
}
System::String^ Controller::getSystemCultureStr(String^ str) 
{
	ResourceManager^ resman = gcnew ResourceManager("SliderTest.UImsg",System::Reflection::Assembly::GetExecutingAssembly());//typeof(FormSettings).Assembly);
	str = resman->GetString(str, generalSettings->systemculture);
	return (str);
}

bool Controller::getPlayStatus() {
	return isPlaying;
}
void Controller::setPlayStatus(bool status) 
{
	isPlaying = status;
}
// Additional method declaring the playback status, its only for keypress notification. (setPlayStatus is static, thats the reason)
void Controller::setKeytrackStatus(bool status) 
{
	if(generalSettings->logKeypress && status)
	{
		keySlider->playbackStarted();
		kHook->hookEnabled = true;
	}
	else if(generalSettings->logKeypress && (status==false) )
	{
		kHook->hookEnabled = false;
		keySlider->playbackStopped();
	}
}

void Controller::setLastItemAuto(bool a) {
	lastAuto = a;
}

bool Controller::isLastItemAuto() {
	return lastAuto;
}

bool Controller::isNextItemAuto() {
	return playlist->isNextItemAuto();
}

void Controller::storeSliderValues() {
	for each (AbstractSlider^ s in listSlider) {
		s->readFromSlider();
	}
}
void Controller::storeKeypressTimeTick(System::Windows::Forms::Keys k) 
{
	for each (AbstractSlider^ s in listKeySlider) {
 		s->storeVal(k);
	}
}
#ifdef DEBUG
void Controller::storeSliderValuesLost() {
	for each (AbstractSlider^ s in listSlider) {
		s->readFromSliderLost();
	}
}
#endif
#ifdef TESTMODE
void Controller::storeSliderValues(String^ time) {
	for each (AbstractSlider^ s in listSlider) {
		s->readFromSlider(time);
	}
}
#endif

void Controller::LogValueList(System::Collections::Generic::List<float>^ listVal) {

	if (lastAuto) return;	// only log values, if the last played item was not a pause etc.
	
	for each (Logging^ l in listLogging)
		l->LogResult(listVal, lastName);
}
void Controller::LogValueHistogram(void) 
{
	// Additional logging procedure as histogram, at the end of evaluation

	if(!( (generalSettings->rating == Rating::End)||(generalSettings->rating == Rating::Sync) ))
		return;

	for each (Logging^ l in listLogging)
	{
		l->LogNoDate(Environment::NewLine + "--- Rating Histogram ------------------" + Environment::NewLine );
		try{
			//for each(String^ s in fullNameList)
			for(int count = 0; count<fullNameList->Count; count++)
			{
				String^ s = fullNameList[count];
				float answ = fullAnswerList[count];
				int num = fullNumPlayedList[count];
				if(s && !s->StartsWith(":"))
					l->LogHistResult(answ, num, s);
			}
		}
		catch(ArgumentOutOfRangeException^)
		{
			l->LogNoDate("\nHistogram Count Error\n");
		}
	}
}

void Controller::LogAdditionalResult(void) 
{
	// Additional logging procedure at the end of evaluation

	// Double, SCase : Number of turns / file
	// Double, SCase : threshold estimation
	if(!( (generalSettings->rating == Rating::End)||(generalSettings->rating == Rating::Sync) ))
		return;

	if(! ((generalSettings->rmtype == RatingMethodType::Staircase) || (generalSettings->rmtype == RatingMethodType::Double)))
		return;

	for each (Logging^ l in listLogging)
	{
		l->LogNoDate(Environment::NewLine + "--- Additional Results ------------------" + Environment::NewLine );
		try{
			if(generalSettings->rmtype == RatingMethodType::Double)
			{
				l->LogNoDate("--- List of reversals, procedure 1 :" );
				for each(String^ s in turnNameList)
					l->LogNoDate(s->Replace("\\\\","\\"));

				l->LogNoDate(Environment::NewLine + "--- List of reversals, procedure 2 :" );
				for each(String^ s in turnNameListDouble)
					l->LogNoDate(s->Replace("\\\\","\\"));
			}
			else
			{
				l->LogNoDate("--- List of reversals :" );
				for each(String^ s in turnNameList)
					l->LogNoDate(s->Replace("\\\\","\\"));
			}
		}
		catch(ArgumentOutOfRangeException^)
		{
			l->LogNoDate("\nReversal Count Error\n");
		}
	}
}
void Controller::finishAllOpenLogs() 
{
	// gets called right before Exit
	//if( (generalSettings->rmtype==RatingMethodType::Constant) ||
	//	(generalSettings->rmtype==RatingMethodType::)
	static bool logged = false;	// prevent double calls
	if(!logged)	
	{
		logged = true;
		LogValueHistogram();
		LogAdditionalResult();
		LogNoDate(Environment::NewLine + "--- EOF ---------------------------------");
	}
}
void Controller::logLastValues() 
{
	if (lastAuto) return;	// only log values, if the last played item was not a pause etc.

	// put the current values of all slider into a list and log these values
	// do this also for hardware sliders: if there are more than one hardware slider,
	// this means several users which means splitLogs == true
	List<float>^ listVal = gcnew List<float>;
	for each (AbstractSlider^ s in listSlider) {
		s->readFromSlider();
		listVal->Add((float)s->getSliderValue());
		
		lastAnswer = currentAnswer;
		currentAnswer = s->getSliderValue();
	}
	// add name and other countings only ONCE. Values and wasTurn corresponds then always to LAST slider
	fullNameList[currentPos] = lastName;
	fullNumPlayedList[currentPos]++;
	fullAnswerList[currentPos] += currentAnswer;

	if(playlist->wasLastTurn())
	{
		fullNumTurnsList[currentPos]++;
		turnNameList->Add(lastName);
	}

	if (splitLogs) {
		int i = 0;
		for each (Logging^ l in listLogging) {
			List<float>^ tmpList = gcnew List<float>;
			tmpList->Add(listVal[i]);
			l->LogResult(tmpList, lastName);
			// in case of more than one logging mechanism, "i" will always be in the range
			// of listVal
			i = (i + 1) % listSlider->Count;
		}
	}
	else {
		for each (Logging^ l in listLogging)
		{
			// corresponds to LAST slider
			if(playlist->wasLastTurn() && ((generalSettings->rmtype == RatingMethodType::Staircase) || (generalSettings->rmtype == RatingMethodType::Double)))
				l->LogResult(listVal, lastName + " <TURN>");
			else
				l->LogResult(listVal, lastName);
		}
	}
}

void Controller::logStoredValues() {
	// only log values, if the last played item was not a pause etc.
	if (lastAuto) return;

	// every slider gets logged into a separate logging file
	int i = 0;
	for each (Logging^ l in listLogging) {
		// if there are less sliders than loggers, this will give an exception
		// this should not happen, since for every ID in listID, there should be one
		// (hardware) slider
#ifdef TESTMODE
		l->LogResult(listSlider[i]->getValueList(),listSlider[i]->getXinfoList(), lastName);
#else
		l->LogResult(listSlider[i]->getValueList(), lastName);
#endif
		i = (i + 1) % generalSettings->listID->Count;
	}
}
void Controller::logStoredKeyValues() {
	// only log values, if the last played item was not a pause etc.
	if (lastAuto) return;

	// every slider gets logged into a separate logging file
	int i = 0;
	for each (Logging^ l in listLogging) {
		// if there are less sliders than loggers, this will give an exception
		// this should not happen, since for every ID in listID, there should be one
		// (hardware) slider
		l->LogResult(listKeySlider[i]->getValueList(), lastName + " <KEYS>");
		i = (i + 1) % generalSettings->listID->Count;
	}
}

bool Controller::getTrainingPhase() {
	return trainingPhase;
}

void Controller::setTrainingPhase(bool b) {
	trainingPhase = b;

	// at the end of the training, calculate the mean value of all volumes and replace all
	// values by this mean
	if (!b && listVolumeValues->Count > 0) {
		float tmpVol = 0;
		for each (float v in listVolumeValues) {
			tmpVol += v;	
		}
		volume = tmpVol / listVolumeValues->Count;
	}
}
void Controller::LogVideoIndex(int i)
{
	videoIndexes->Add(i);
}
bool Controller::PoolEvaluated() 
{
	int numComparedSequences = playlist->numComparedSequences;
	for(int i = 0 ; i < numComparedSequences ; ++i)
	{
		bool founded = false;
		for(int j = 0 ; j < videoIndexes->Count ; ++j)
		{
			if(videoIndexes[j] == i)
				founded = true;
		}

		if(!founded)
			return false;
	}
	return true;
}

bool Controller::needVolumeWindow() {
	return (trainingPhase && generalSettings->media == SliderTest::Media::Audio);
}

void Controller::storeVolume(float v) {
	listVolumeValues->Add(v);	 
}

void Controller::setVolume(float v) {
	volume = v;
}

float  Controller::getVolume() {
	return volume;
}
