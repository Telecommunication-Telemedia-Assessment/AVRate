#pragma once
#include "contentserver.h"

ref class VideoRemote :
public ContentServer
{
public:
	VideoRemote(Controller^ c,System::String^ remoteHost);

	virtual void PreparePlay(PlaylistItemPlayable^ item) override;
	virtual void Play(PlaylistItemPlayable^ item) override;
	virtual void EndPlay() override;

private:
	void Connect();
	void SendCommand(System::String^ cmd, int idleTime); // sends cmd to the video server and sleep an optional time
	void SendCommand(System::String^ cmd);
	void Receive();
	void errorNoConnection(System::String^);

	Controller^ controller;
	System::Net::Sockets::Socket ^s;
	System::String^ serverAddress; // IP of remote video server
	int port; // port of remote video server
	static bool connectionError;
};
