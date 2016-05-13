#pragma once
#include "contentserver.h"
#include "Playlist.h"
#include "Types.h"

ref class VideoLocal :
public ContentServer
{
public: 
	VideoLocal::VideoLocal(Controller^ c,LocalPlayer l, System::String^ exeStr);
	virtual void PreparePlay(PlaylistItemPlayable^ item) override;
	virtual void Play(PlaylistItemPlayable^ item) override;
	virtual void PlayThreaded(PlaylistItemPlayable^ item) override;
	virtual void EndPlay() override;

private:
	void VLC10StarterPlay(System::String^ fileName);
	void VLC10StartProcess(System::Object ^, System::ComponentModel::DoWorkEventArgs ^); 
	void VLC10StartProcess(System::String^ fileName); 
	void StereoscopicPlayerNVidiaStarterPlay(System::String^ fileName); 
	void StereoscopicPlayerNVidiaStartProcess(System::Object ^, System::ComponentModel::DoWorkEventArgs ^); 
	void StereoscopicPlayerSbSStarterPlay(System::String^ fileName); 
	void StereoscopicPlayerSbSStartProcess(System::Object ^, System::ComponentModel::DoWorkEventArgs ^); 
	void CommandLinePlayerStarterPlay(System::String^ fileName); 
	void CommandLinePlayerStartProcess(System::Object ^, System::ComponentModel::DoWorkEventArgs ^); 

	void ProcessStopped(void); 	 
	void ProcessStopped(System::Object ^, System::ComponentModel::RunWorkerCompletedEventArgs ^); 	 
	void WMPStarterPlay(void);
	void WMPStarterPrepare(System::String^ fileName);
	void MPlayerStarterPlay(System::String^ fileName);
	void MPlayerStartProcess(System::String^ fileName); 	 
	void MPlayerStartProcess(System::Object ^, System::ComponentModel::DoWorkEventArgs ^); 	 
	void wmpForm_Shown(Object^ obj, System::EventArgs^ e);
	void playstateChangedWMP(System::Object ^,AxWMPLib::_WMPOCXEvents_PlayStateChangeEvent ^e);
	void myTimerProc(Object^ /*myObject*/, System::EventArgs^ /*myEventArgs*/);

	LocalPlayer localPlayer;
	Controller^ controller;
	int currentmediawidth;
	int currentmediaheight;
	int targetposx,targetposy;	// Screen target Position based on mediawidth/height
	bool startthreaded;	// indicates if a Backgroundworker should start the video process
	System::String^ playerPath;
	AxWMPLib::AxWindowsMediaPlayer^ myWMP;
	System::ComponentModel::BackgroundWorker^  backgroundWorker1;
	System::ComponentModel::BackgroundWorker^  backgroundWorker2;
	System::Windows::Forms::Timer^ t;
	System::Windows::Forms::Form^ myForm;
	System::Windows::Forms::Form^ wmpTempForm;
	System::String^ videoFileName;
};
