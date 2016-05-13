#pragma once
// dll/AxInterop.WMPLib.1.0.dll;dll/Interop.WMPLib.1.0.dll;dll/K8055D_C.DLL;dll/libsndfile-1.dll;
namespace SliderTest {
[System::Flags]
enum class SettingTypes : System::Int32 {
	Zero = 0,
	otherError = 1,
	Rating = 2,
	Device = 4,
	Display = 8,
	Media = 16,
	Layout = 32,
	LocalPlayer = 64,
	VolumeFormType = 128,
	RatingMethodType = 256,
	Repeat = 512,
	RatingMethodRule = 2048,
	RatingMethodBreakCount = 4096,
	UserLanguage = 8192,
	SystemLanguage = 16384,
	SleepTime = 32768,
};

static enum class Rating {
	End,
	Continuous,
	Sync,
};

static enum class Device {
	Software,
	Hardware,
	Tablet
};

static enum class Display {
	Local,
	Remote,
	Server,
};

static enum class Media {
	Audio,
	Video,
};

static enum class Layout {
	Horizontal,
	Vertical,
	P800,
	OnScreen,
};

static enum class LocalPlayer {
	MPlayer,
	WMP,
	VLC10,
	STEREOSCOPIC_PLAYER_NVIDIA,
	STEREOSCOPIC_PLAYER_SBS,
	COMMAND_LINE
};

static enum class VolumeFormType {
	None,
	Button,
	Slider,
};

static enum class RatingMethodType {
	Standard,
	Constant,
	Staircase,
	Double,
	PEST,
	QUEST,
	SAMVIQ
};

// COMMAND LINE ARGS DEFINES
#define kcommandLineStartStr L"\nAVrate Command Line Version\nRevision 48\n"
#define kkeyPressStr L"\nPress any key to quit..."
//#define NOCLIPMOUSE
#define DEFAULTSLEEPTIME_LOCAL 0
#define DEFAULTSLEEPTIME_SERVER 2000
#define DEFAULTSLEEPTIME_REMOTE 1500	// prevents cutting off commands

public ref class GeneralSettings  {
public:
	GeneralSettings() {
		listID = gcnew System::Collections::Generic::List<System::String^>;
		waitTimeRating = 6000;
		samplingInterval = 500;
		ratingmethoduptarget = 1;
		ratingmethoddowntarget = 1;
		ratingmethodrule = 0;
		ratingmethodbreakcount = 0;
		logKeypress=0;
		showSequenceName=0;
		playOnDevice = false;
	}

	Rating rating;
	Device device;
	Display display;
	RatingMethodType rmtype;
	int ratingmethoduptarget;
	int ratingmethoddowntarget;
	int ratingmethodrule;
	int ratingmethodbreakcount;
	Media media;
	Layout layout;
	System::String^ remoteHost;
	bool repeat;
	LocalPlayer localPlayer;
	bool playOnDevice;
	System::String^ playerPath;
	System::String^ playerArguments;
	VolumeFormType volumeFormType; // for audio tests during training phase 
		// (form can contain a slider or 3 radio buttons to control the volume)
	bool logToFile;
	bool logToDatabase;
	bool logKeypress;
	bool keyOnExit;
	bool silentProcessing;
	bool startedFromConsole;

	System::String^ playlist;
	System::String^ settingsFile;
	System::Collections::Generic::List<System::String^>^ listID;

	// for hardware slider and more than one user, 
	// don't wait for a button press but just a certain amount of time
	int waitTimeRating;
	
	// time in ms between two samples during continuous rating
	int samplingInterval;
	System::Globalization::CultureInfo^ userculture;
	System::Globalization::CultureInfo^ systemculture;

	// empiric value for server/screen timing
	int sleepTime;
	
	// display the name of the sequence under evaluation
	int showSequenceName;
};
}