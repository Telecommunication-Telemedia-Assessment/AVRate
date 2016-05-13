#pragma once

#include "ContentServer.h"

ref class VideoServer : ContentServer
{
public:
	VideoServer(Controller^ c,System::String^ remoteHost);

	virtual void PreparePlay(PlaylistItemPlayable^ item) override;
	virtual void Play(PlaylistItemPlayable^ item) override;
	virtual void PlayThreaded(PlaylistItemPlayable^ item) override;
	virtual void EndPlay() override;
	void StartPlaybackProcess(System::Object ^, System::ComponentModel::DoWorkEventArgs ^); 	 
	void PlaybackProcessStopped(Object^ , System::ComponentModel::RunWorkerCompletedEventArgs^ );

private:
	void Connect();
	void SendCommand(System::String^ cmd, int idleTime); // sends cmd to the video server and sleep an optional time
 	void SendCommand(System::String^ cmd); // sends cmd and waits blocks until somthing is sent in response
 	void SendCommandNoReceive(System::String^ cmd); // sends cmd without response
	void Receive();
	void errorNoConnection(System::String^);
	
	Controller^ controller;
	System::Net::Sockets::Socket ^s;
	System::String^ serverAddress; // IP of video server
	int port; // port of video server
	static bool connectionError;
	System::String^ connectionErrorStr;

	System::ComponentModel::BackgroundWorker^  backgroundWorker1;
	bool startthreaded;
	System::String^ videoFileName;

	System::String^ slaveAddress; // need slave to set raster, since that cannot be done by the master
	bool haveSlave;
	System::Net::Sockets::Socket ^slaveSocket;
#ifdef DEBUG
	System::Collections::Generic::List<System::String^>^ debugList;
#endif
};




