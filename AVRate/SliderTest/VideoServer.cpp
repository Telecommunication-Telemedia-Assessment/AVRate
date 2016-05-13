#include "StdAfx.h"
#include "VideoServer.h"

using namespace System;
using namespace System::Text;
using namespace System::IO;
using namespace System::Net;
using namespace System::Net::Sockets;
using namespace System::Collections;
using namespace System::Collections::Generic;
using namespace Threading;
using namespace System::Diagnostics;


VideoServer::VideoServer(Controller^ c,String^ remoteHost) {
	array<String^>^ fields = remoteHost->Split(':');
	serverAddress = fields[0];
	port = Convert::ToInt32(fields[1]);
	slaveAddress = "xxx";
	haveSlave = false;
	this->controller = c;
	connectionError = false;
	startthreaded = false;

	//Connect();	// try getting a connection to initialize the server
					// this did not show significant performance gain
					// and if its on, there should be more object handling (gives exception)
	#ifdef DEBUG
	debugList = gcnew List<String^>();
	#endif
}
void VideoServer::errorNoConnection(String^ error) 
{
	connectionError = true;

	System::Windows::Forms::Cursor::Current->Show();
	System::Windows::Forms::Cursor::Current->Clip = System::Windows::Forms::Screen::PrimaryScreen->Bounds;

	connectionErrorStr = controller->getSystemCultureStr("ERR_noConnect") + error;
	if((controller->generalSettings->startedFromConsole == true) && 
		(controller->generalSettings->silentProcessing == false) )
			Console::WriteLine(connectionErrorStr);
	else
	{
		bool cdis = controller->disableClipping;
		controller->disableClipping = true;
		System::Windows::Forms::MessageBox::Show(connectionErrorStr, "Socket Error",
			System::Windows::Forms::MessageBoxButtons::OK, System::Windows::Forms::MessageBoxIcon::Error);
		controller->disableClipping = cdis;
	}
	if(!startthreaded)	
	{
		// prevent inter-threading access
		// when threaded, this is handled in PlaybackProcessStopped
		controller->LogNoDate(connectionErrorStr);
		controller->quitAppRegular();
	}
}

void VideoServer::Connect() {
	if(connectionError)	return;

	//IPHostEntry^ hostEntry = Dns::GetHostEntry(serverAddress);

	//array<IPAddress^>^IPAddresses = hostEntry->AddressList;

	// Evaluate the socket and receive host IPAddress and IPEndPoint.
	IPAddress^ address; // = IPAddresses[0];
	
	address = IPAddress::Parse(serverAddress);
	IPEndPoint^ endPoint = gcnew IPEndPoint(address, port);

	// Creates the Socket to send data over a TCP connection.
	s = gcnew Socket(AddressFamily::InterNetwork, SocketType::Stream, ProtocolType::Tcp);

	try
	{
		s->Connect(endPoint);
	}
	catch(SocketException^ e)
	{
		errorNoConnection(e->Message);
	}
}

// send a command to the video server without waiting for an answer
void VideoServer::SendCommandNoReceive(String^ cmd) 
{
	if(connectionError)	return;

	cmd += "\n";
	array<Byte>^ cmdArray = Encoding::UTF8->GetBytes(cmd);
	
	#ifdef DEBUG
	debugList->Add(cmd);
	System::DateTime^d = DateTime::Now;
	Debug::WriteLine(d->Second + " " + d->Millisecond + " " + cmd);
	#endif

	try
	{
		int res = s->Send(cmdArray);
		//Receive();
	}
	catch(SocketException^ e)
	{
		errorNoConnection(e->Message);
	}
}

// send a command to the video server and sleep (to bypass the length of the video)
void VideoServer::SendCommand(String^ cmd, int idleTime) {
	if(connectionError)	return;

	cmd += "\n";
	array<Byte>^ cmdArray = Encoding::UTF8->GetBytes(cmd);
	
	#ifdef DEBUG
	debugList->Add(cmd);
	System::DateTime^d = DateTime::Now;
	Debug::WriteLine(d->Second + " " + d->Millisecond + " " + cmd);
	#endif

	try
	{
		int res = s->Send(cmdArray);
		Receive();
	}
	catch(SocketException^ e)
	{
		errorNoConnection(e->Message);
	}

	Thread::Sleep(idleTime);
}

// send a command and block-wait for an answer
void VideoServer::SendCommand(String^ cmd) {
	if(connectionError)	return;

	cmd += "\n";
	array<Byte>^ cmdArray = Encoding::UTF8->GetBytes(cmd);

	#ifdef DEBUG
	debugList->Add(cmd);
	System::DateTime^d = DateTime::Now;
	Debug::WriteLine(d->Second + " " + d->Millisecond + " " + cmd);
	#endif

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

void VideoServer::Receive() {
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
	
	String^ msgStr = Encoding::ASCII->GetString(receivedMsg);
	msgStr = msgStr;//for future use
	#ifdef DEBUG
	debugList->Add(msgStr);
	System::DateTime^d = DateTime::Now;
	Debug::WriteLine(d->Second + " " + d->Millisecond + " " + msgStr);
	#endif
}


// do some stuff to prepare the playing, but don't send the DisplayForward yet
void VideoServer::PreparePlay(PlaylistItemPlayable^ item) {
	int start = item->getStart();
	int length = item->getLength();
	int frame = item->getFrame();

	long seqEnd = start + length;

	String^ frameCmd = "smpte274m" + frame + "p";

	Connect();

	SendCommand("<command context=\"EditTool\" name=\"DisplayStop\" id=\"1235\"></command>"); // 900

//	if (haveSlave)
//		slaveSocket->Send(Encoding::UTF8->GetBytes("SetRaster Timeline1 " + frameCmd + "\n"));

	SendCommand("<command context=\"EditTool\" name=\"SetCurrentRaster\"><parameter name=\"raster\">" +	frameCmd + "</parameter></command>");

	SendCommand("<command context=\"EditTool\" name=\"Goto\" id=\"1235\"><parameter name=\"frame\">" + seqEnd + "</parameter></command>"); // 500

	SendCommand("<command context=\"EditTool\" name=\"SetOutpoint\" id=\"1234\"	></command>"); // 900

	SendCommand("<command context=\"EditTool\" name=\"Goto\" id=\"1235\"><parameter name=\"frame\">" + start + "</parameter></command>"); // 500

	SendCommand("<command context=\"EditTool\" name=\"SetInpoint\" id=\"1234\"></command>"); // 900

	Thread::Sleep(controller->generalSettings->sleepTime);
}
void VideoServer::Play(PlaylistItemPlayable^ item) {
	//PlayThreaded(item);
	//return;
	int length = item->getLength();
	int frame = item->getFrame();
	float milliseconds = 1000;

	controller->setPlayStatus(true);
	controller->setKeytrackStatus(true);
	SendCommand("<command context=\"EditTool\" name=\"DisplayForward\" id=\"1234\"></command>", (int)(length*milliseconds/(float)frame));
	controller->invokeEndPlayEvent(item->getName());
	controller->setPlayStatus(false);
	controller->setKeytrackStatus(false);
}
void VideoServer::PlayThreaded(PlaylistItemPlayable^ item) {
	videoFileName = item->getName();

	startthreaded = true;
	backgroundWorker1 = (gcnew System::ComponentModel::BackgroundWorker());
	backgroundWorker1->DoWork += gcnew System::ComponentModel::DoWorkEventHandler(this, &VideoServer::StartPlaybackProcess);
	backgroundWorker1->RunWorkerCompleted += gcnew System::ComponentModel::RunWorkerCompletedEventHandler(this, &VideoServer::PlaybackProcessStopped);

	int playbackduration = (int)(float(item->getLength())*1000.f/(float)item->getFrame());
	backgroundWorker1->RunWorkerAsync(playbackduration);
}
void VideoServer::StartPlaybackProcess(System::Object ^ sender, System::ComponentModel::DoWorkEventArgs ^ args) 
{
	int playbackduration = (int)args->Argument;
	SendCommandNoReceive("<command context=\"EditTool\" name=\"DisplayForward\" id=\"1234\"></command>");
	Receive();
	controller->setPlayStatus(true);
	controller->setKeytrackStatus(true);
	#ifdef DEBUG
	System::DateTime^d = DateTime::Now;
	Debug::WriteLine(d->Second + " " + d->Millisecond + " setKeytrackStatus(true)" );
	#endif

	Thread::Sleep(playbackduration);

	controller->invokeEndPlayEvent(videoFileName);
	controller->setPlayStatus(false);
	controller->setKeytrackStatus(false);
	EndPlay();
	args->Result = connectionErrorStr;
}
void VideoServer::PlaybackProcessStopped(Object^ , System::ComponentModel::RunWorkerCompletedEventArgs^ e) {
	if(e->Result != nullptr)
	{
		if(e->Result->ToString() != "")
		{
			// Error while playback?
			controller->LogNoDate(e->Result->ToString());
			controller->quitAppRegular();
		}
	}
}

void VideoServer::EndPlay() {
	if(connectionError)	return;

	SendCommand("<command context=\"EditTool\" name=\"DisplayStop\" id=\"1234\"></command>");
	SendCommand("<command context=\"EditTool\" name=\"Goto\" id=\"1235\"><parameter name=\"frame\">0</parameter></command>");

	try
	{
		s->Close();
		if (haveSlave)
			slaveSocket->Close();
	}
	catch(SocketException^ e)
	{
		errorNoConnection(e->Message);
	}
}