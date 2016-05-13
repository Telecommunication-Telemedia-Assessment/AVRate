#pragma once

#include "HardwareSlider.h"
#include "AbstractSlider.h"
#include "KeypressSlider.h"
//#include "KeyboardHook.h"
#include "Logging.h"
#include "Types.h"
//#include "FormRating.h"

ref class Playlist;
ref class FormRating;
ref class KeyboardHook;

// controller class for logging, slider and playlist
// sl and logger should be private, but aren't yet due to convenience

public ref class Controller// nonpublic sr
{
public:
	Controller(SliderTest::GeneralSettings^ gs);
	//Controller(SliderTest::GeneralSettings^ gs, System::Windows::Forms::Form^);

	void setSlider(AbstractSlider^ sl); // set (add) sliders, that will be logged
	void setKeySlider(AbstractSlider^ sl); // set (add) Keysliders, that will be logged

	void StartCapture();
	void StopCapture();
	void StartLogging();
	void finishAllOpenLogs();
	void StopLogging(System::String^ itemName);
	void Log(System::String^ s);
	void LogNoDate(System::String^ s);
	void logLastValues(); // log the currently set values (includes a read from the sliders)
	void logStoredValues(); // log all stored values of the first slider
	void logStoredKeyValues(); // log all stored values of the keypress 'slider'
	void LogValueHistogram();// log as histogram at the end
	void LogAdditionalResult();// log final results at the end
	bool PoolEvaluated();
	void LogVideoIndex(int i);
	
	// logs the list of values (P800 hack)
	void LogValueList(System::Collections::Generic::List<float>^ l);	 

	void quitAppRegular();
	//void quitAppError(System::String^);
	
	// play an item
	void PlayInit();
	void PlayNext();
	System::String^ VirtualPlayNext();
	void PreparePoolSequences();
	void PlayOneNext(int i);
	void PlayNext(System::Object ^,System::ComponentModel::DoWorkEventArgs ^);
	void PlayCurrent();
	void PlayCurrent(System::Object ^,System::ComponentModel::DoWorkEventArgs ^);
	delegate void EndPlayEventHandler(void);// the eventhandler is actually not used,
	event EndPlayEventHandler^ endPlayEvent;// gives an invokation exception when handled by the Form
	System::String^ getLastItemName(void);
	void invokeEndPlayEvent(System::String^ name);// {triggered=true;	endPlayEvent(); 
	//void invokeEndPlayEventAutoItem(void);	
	static bool endplay=false;	// MUST BE STATIC! state is true when an external server thread completed
	//				// maybe take IsPlaying instead, depends if independent threads are handled by this
	static System::String^ lastItemName="";
	static System::String^ lastItemNameDouble="";

	int GetVideoPos();
	int GetPaneNumber();
	int GetItemsPlayed();
	int GetTotalVideoNumber();
	int GetNumberOfcomparedSequences();
	int GetNumAutoItems();
	bool GetSliderButton();
	static bool getPlayStatus();
	static void setPlayStatus(bool ); 
	void setKeytrackStatus(bool ); 
	System::Globalization::CultureInfo^ getUserCulture();
	System::Globalization::CultureInfo^ getSystemCulture();
	System::String^ getUserCultureStr(System::String^);
	System::String^ getSystemCultureStr(System::String^);

	void setLastItemAuto(bool a);
	bool isLastItemAuto();
	bool isNextItemAuto();
	void switchPlaylistSet();// for Double Method
	
	property int currentPos;		// position in playlist (including non-playable items)
	property int currentPosDouble;		// position in playlist for Double staircase method

	void storeSliderValues(); // read from slider and update it's current value
	void storeKeypressTimeTick(System::Windows::Forms::Keys k); // store time to a keypress event
	#ifdef DEBUG
	void storeSliderValuesLost(); // read lost (unlogged) values from slider and update it's current value
	#endif
	#ifdef TESTMODE
	void storeSliderValues(System::String^ time); 
	#endif
	void setTrainingPhase(bool b);
	bool getTrainingPhase();
	bool needVolumeWindow();
	void setVolume(float v);
	void storeVolume(float v);
	float getVolume();

	//int getCurrentAnswer(void);
	float getCurrentAnswer(void);
	//int getLastAnswer(void);
	float getLastAnswer(void);
	int getLastDirection(void);
	void setLastDirection(int);
	//bool getLastWasTurn(void);
	//void setLastWasTurn(bool);

	SliderTest::GeneralSettings^ generalSettings;
	System::Windows::Forms::Form^ parentForm;
	KeypressSlider^ keySlider;
	KeyboardHook^ kHook;

	bool errorInitializing;
	static bool disableClipping; // passive Flag to inform GUI

private:
	bool splitLogs; // split logs for every slider (for more than one subject)
	System::Collections::Generic::List<Logging^>^ listLogging;
	System::Collections::Generic::List<AbstractSlider^>^ listSlider;
	System::Collections::Generic::List<AbstractSlider^>^ listKeySlider;
	
	System::String^ lastName; // name of the last played item
	System::String^ lastNameDouble; // name of the last played item
	bool lastAuto; // determines, if the last entry was an automatic (i.e. non rateable) entry
	bool lastAutoDouble;
	bool trainingPhase;
	static bool isPlaying;// Must be static! threaded access
	Playlist^ playlist;
	
	// volume for audio tests during the training phase
	System::Collections::Generic::List<float>^ listVolumeValues;
	// General collection of answers
	System::Collections::Generic::List<int>^ fullNumPlayedList;
	System::Collections::Generic::List<float>^ fullAnswerList;
	System::Collections::Generic::List<System::String^>^ fullNameList;
	System::Collections::Generic::List<System::String^>^ turnNameList;
	System::Collections::Generic::List<System::String^>^ turnNameListDouble;
	System::Collections::Generic::List<int>^ fullNumTurnsList;
	float volume;
	float currentAnswer;
	float currentAnswerDouble;
	int lastDirection;
	int lastDirectionDouble;
	float lastAnswer;
	float lastAnswerDouble;

	System::Collections::Generic::List<int>^ videoIndexes;
	int paneNumber;
	//bool lastWasTurn;
	//bool lastWasTurnDouble;
};
