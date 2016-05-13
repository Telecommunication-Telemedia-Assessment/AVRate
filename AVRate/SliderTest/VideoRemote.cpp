#include "StdAfx.h"
#include "VideoRemote.h"

using namespace System;
using namespace System::Text;
using namespace System::IO;
using namespace System::Net;
using namespace System::Net::Sockets;
using namespace System::Collections;
using namespace Threading;


VideoRemote::VideoRemote(Controller^ c,String^ remoteHost) {
	array<String^>^ fields = remoteHost->Split(':');
	serverAddress = fields[0];
	port = Convert::ToInt32(fields[1]);
	this->controller = c;
	connectionError = false;
}
void VideoRemote::errorNoConnection(String^ error) 
{
	connectionError = true;

	String^ errmsg = controller->getSystemCultureStr("ERR_noConnect") + error;
	if((controller->generalSettings->startedFromConsole == true) && 
		(controller->generalSettings->silentProcessing == false) )
			Console::WriteLine(errmsg);
	else
	{
		bool cdis = controller->disableClipping;
		controller->disableClipping = true;
		System::Windows::Forms::Cursor::Current->Show();
		System::Windows::Forms::MessageBox::Show(controller->getSystemCultureStr("ERR_noConnect")+error, "Socket Error",
			System::Windows::Forms::MessageBoxButtons::OK, System::Windows::Forms::MessageBoxIcon::Error);
		controller->disableClipping = cdis;
	}
	controller->LogNoDate(errmsg);
	controller->quitAppRegular();
}

void VideoRemote::Connect() {
	if(connectionError)	return;

	IPHostEntry^ hostEntry = Dns::GetHostEntry(serverAddress);

	array<IPAddress^>^IPAddresses = hostEntry->AddressList;

	// Evaluate the socket and receive host IPAddress and IPEndPoint.
	IPAddress^ address = IPAddresses[0];
	IPEndPoint^ endPoint = gcnew IPEndPoint(address, port);

	try
	{
		// Creates the Socket to send data over a TCP connection.
		s = gcnew Socket(AddressFamily::InterNetwork, SocketType::Stream, ProtocolType::Tcp);
		s->Connect(endPoint);
	}
	catch(SocketException^ e)
	{
		errorNoConnection(e->Message);
	}
}

// send a command to the video server and sleep
void VideoRemote::SendCommand(String^ cmd, int idleTime) {
	if(connectionError)	return;
	System::Diagnostics::Debug::Print(cmd);

	cmd += "\n";
	array<Byte>^ cmdArray = Encoding::UTF8->GetBytes(cmd);
	
	try
	{
		int res = s->Send(cmdArray);
	}
	catch(SocketException^ e)
	{
		errorNoConnection(e->Message);
	}

	Thread::Sleep(idleTime);
}

// send a command and block-wait for an answer
void VideoRemote::SendCommand(String^ cmd) {
	if(connectionError)	return;
#ifdef DEBUG
	System::Diagnostics::Debug::Print(cmd);
#endif

	cmd += "\n";
	array<Byte>^ cmdArray = Encoding::UTF8->GetBytes(cmd);
	
	try
	{
		int res = s->Send(cmdArray);
		Receive();
	}
	catch(SocketException^ e)
	{
		errorNoConnection(e->Message);
	}
}

void VideoRemote::Receive() {
	if(connectionError)	return;

	array<Byte>^ receivedMsg = gcnew array<Byte>(256);
	try
	{
		int ret = s->Receive(receivedMsg);
	}
	catch(SocketException^ e)
	{
		errorNoConnection(e->Message);
	}
}

// do some stuff to prepare the playing, but don't send the DisplayForward yet
void VideoRemote::PreparePlay(PlaylistItemPlayable^ item) {
		Connect();
		SendCommand("clear");
}

void VideoRemote::Play(PlaylistItemPlayable^ item) {
	String^ name = item->getName();

#ifdef LENGTHAVAILABLE
	int len = item->getLength();	// is 0, not implemented!
//#else
//	int len = DEFAULTSLEEPTIME_REMOTE;//ok, vorher
#endif

	controller->setPlayStatus(true);
	controller->setKeytrackStatus(true);

	SendCommand("add " + name, controller->generalSettings->sleepTime);
	//SendCommand("add " + name, len);

#ifdef LENGTHAVAILABLE
	SendCommand("stop");
#endif

	controller->invokeEndPlayEvent(item->getName());
	controller->setPlayStatus(false);
	controller->setKeytrackStatus(false);
}

void VideoRemote::EndPlay() {
	if(connectionError)	return;

#ifdef LENGTHAVAILABLE
	SendCommand("clear");
#endif
	try
	{
		s->Close();
	}
	catch(SocketException^ e)
	{
		errorNoConnection(e->Message);
	}
}
