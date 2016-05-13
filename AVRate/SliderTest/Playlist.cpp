#include "StdAfx.h"
#include "Playlist.h"

#include "ContentServer.h"
#include "VideoServer.h"
#include "VideoRemote.h"
#include "VideoLocal.h"
#include "AudioLocal.h"
#include "Types.h"
#include "RatingMethod.h"

using namespace System::Windows::Forms;
using namespace System;
using namespace System::Collections::Generic;
using namespace System::IO;
using namespace SliderTest;

Playlist::Playlist(Controller^ c) {
	numPlayableItems = 0;
	numAutoItems = 0;
	itemsPlayed=0;
	numComparedSequences = 1;
	resman = gcnew System::Resources::ResourceManager("SliderTest.UImsg",System::Reflection::Assembly::GetExecutingAssembly());//typeof(FormSettings).Assembly);
	playlist = gcnew System::Collections::Generic::List<PlaylistItem^>();
	controller = c;
}

void Playlist::AddItem(PlaylistItem^ p) {
	playlist->Add(p);
}

int Playlist::Initialize(SliderTest::GeneralSettings^ gs) {
	StreamReader^ sr = gcnew StreamReader(gs->playlist);
	// Fill the playlist with entries from the playlist file.
	// Two categories: special items (starting with ":") and normal
	// items with specific formats
	
	// For normal items, if there are more than the usual amount of fields,
	// a combined item is assumed, meaning the entries will be played consequetively
	// by using PlaylistItemSeveral

	int filesNotExisting = 0;		
	String^ nonExistingItems = Environment::NewLine;
	disp = gs->display ;

	RatingMethodType rmtype = gs->rmtype;
	if(rmtype == RatingMethodType::Standard)
		ratingMethod = gcnew RatingMethodStandard(controller);
	else if(rmtype == RatingMethodType::Constant)
		ratingMethod = gcnew RatingMethodConstant(controller);
	else if(rmtype == RatingMethodType::Staircase)
		ratingMethod = gcnew RatingMethodStaircase(controller);
	else if(rmtype == RatingMethodType::Double)
		ratingMethod = gcnew RatingMethodDouble(controller);
	else if(rmtype == RatingMethodType::SAMVIQ)
		ratingMethod = gcnew RatingMethodSAMVIQ(controller);


	try {
		while ( sr->Peek() >= 0 ) {
			array<String^>^ fields = sr->ReadLine()->Split(' ');
			
			// remove empty or single space fields F
			// (will happen for example, if the last char of a line is a space)
			List<String^>^ tmpList = gcnew List<String^>();
			
			for (int i = 0; i < fields->Length; i++) {
				if (fields[i] != "" && fields[i] != " ") {
					tmpList->Add(fields[i]);
				}
			}
			
			fields = tmpList->ToArray();

			if(fields->Length == 0)
				continue;

			//if(kplayinitfile)
			//	this->AddItem(gcnew PlaylistItemInit
			// entries beginning with ':' -> assume pause etc.
			if (fields[0][0] == ':')  {
				if(!(gs->rmtype==RatingMethodType::Standard) ) {
					//showMessage("No special items allowed in Playlist\nwhen the Ratingmethod is other than Standard",MessageBoxButtons::OK, MessageBoxIcon::Exclamation);
				}
				else if (fields[0] == ":PP")
				{
					this->AddItem(gcnew PlaylistItemPause(this));
					numAutoItems++;
				}
				else if (fields[0] == ":TE")
				{
					this->AddItem(gcnew PlaylistItemTrainEnd(this));
					numAutoItems++;
				}
				else if (fields[0] == ":TS")
				{
					this->AddItem(gcnew PlaylistItemTrainStart(this));
					numAutoItems++;
				}
				else if (fields[0] == ":PE")
				{
					this->AddItem(gcnew PlaylistItemEnd(this));
					numAutoItems++;
				}
				else if (fields[0] == ":CC") {
					String^ popupStr;
					for (int i = 1; i < fields->Length; i++)
						popupStr += fields[i] + " ";
					this->AddItem(gcnew PlaylistItemCustomPopup(this, popupStr));
					numAutoItems++;
				}
				else if (fields[0] == ":Frame")
				{
					this->AddItem(gcnew PlaylistItemMessageVS(this,
					Convert::ToInt32(fields[1]),
					gcnew VideoServer(controller,gs->remoteHost)
					));
					numAutoItems++;
				}
				else if (fields[0] == ":WaitFrame")
				{
					this->AddItem(gcnew PlaylistItemWaitFrameVS(this,
					Convert::ToInt32(fields[1]),
					Convert::ToInt32(fields[2]),
					gcnew VideoServer(controller,gs->remoteHost)
					));
					numAutoItems++;
				}
				else {
					String^ result = resman->GetString("ERR_unknownItem1",gs->systemculture);
					result += fields[0];
					result += resman->GetString("ERR_unknownItem2",gs->systemculture);
					if( (gs->startedFromConsole == true) )
					{
						if(gs->silentProcessing == false)
							Console::WriteLine(result);
					}
					else
						MessageBox::Show( result, resman->GetString("PlaylistErrorHeader",gs->systemculture),MessageBoxButtons::OK, MessageBoxIcon::Error);
					return(-1);
				}
			}
			// assume playable item entry
			else {
				//Test if playlist files exist
				if( (gs->display == Display::Local) )
				{
					for (int i = 0; i < fields->Length; i++) {
						// check local files but do not check address with http
						if(fields[i]->Length > 4 && !(fields[i][0] == 'h' && fields[i][1] == 't' && fields[i][2] == 't' && fields[i][3] == 'p')) {
							if(!System::IO::File::Exists(fields[i]))
							{			
								filesNotExisting++;	
								if(filesNotExisting<12)
									nonExistingItems += fields[i]+Environment::NewLine;
								else if(filesNotExisting==20)
									nonExistingItems += "(...)";	
							}
						}
					}
				}
				// --- Audio ---
				// Continuous
				if (gs->rating == Rating::Continuous && 
					gs->media == SliderTest::Media::Audio) {
						AddItem(PlaylistItemConstantRating::factoryAudio(this, fields[0],
						gcnew AudioLocal(),
						numPlayableItems++
						));
				}
				// End
				else if ( (gs->rating == Rating::End || gs->rating == Rating::Sync) && 
					gs->media == SliderTest::Media::Audio) {
						if (fields->Length > 1) {
							List<PlaylistItemPlayable^>^ itemList =	gcnew List<PlaylistItemPlayable^>();
							numPlayableItems++;
							for (int i = 0; i < fields->Length; i++) {
								itemList->Add(PlaylistItemEndRating::factoryAudio(this, fields[i],
									gcnew AudioLocal(),
									numPlayableItems
									));
							}
							AddItem(gcnew PlaylistItemSeveral(this, itemList, numPlayableItems));
						}
						else {
							AddItem(PlaylistItemEndRating::factoryAudio(this, fields[0],
								gcnew AudioLocal(),
								numPlayableItems++
								));
						}
				}
				// --- Video ---
				// -- Local --
				// Continuous
				else if (gs->rating == Rating::Continuous && 
					gs->media == SliderTest::Media::Video &&
					gs->display == Display::Local) {
						AddItem(gcnew PlaylistItemConstantRating(this, fields[0],
							gcnew VideoLocal(controller, gs->localPlayer, gs->playerPath),
							numPlayableItems++
							));
				}
				// End
				else if ((gs->rating == Rating::End || gs->rating == Rating::Sync) && 
					gs->media == SliderTest::Media::Video &&
					gs->display == Display::Local) {
						if (fields->Length > 1) {
							List<PlaylistItemPlayable^>^ itemList =	gcnew List<PlaylistItemPlayable^>();
							numPlayableItems++;
							for (int i = 0; i < fields->Length; i++) {
								itemList->Add(gcnew PlaylistItemEndRating(this, fields[i],
									gcnew VideoLocal(controller, gs->localPlayer, gs->playerPath),
									numPlayableItems
									));
							}
							AddItem(gcnew PlaylistItemSeveral(this, itemList, numPlayableItems));
						}
						else {
							AddItem(gcnew PlaylistItemEndRating(this, fields[0],
								gcnew VideoLocal(controller, gs->localPlayer, gs->playerPath),
								numPlayableItems++
								));
						}
				}
				// -- Server --
				// Continuous
				else if (gs->rating == Rating::Continuous && 
					gs->media == SliderTest::Media::Video &&
					gs->display == Display::Server) {
						if((fields->Length != 4))// 4 entries ?
						{
							String^ result = resman->GetString("ERR_unknownServerItem",gs->systemculture);
							if( (gs->startedFromConsole == true) )
							{
								if(gs->silentProcessing == false)
									Console::WriteLine(result);
							}
							else
								MessageBox::Show( result, resman->GetString("PlaylistErrorHeader",gs->systemculture),MessageBoxButtons::OK, MessageBoxIcon::Error);
							return(-1);
						}
						AddItem(PlaylistItemConstantRating::factoryVideoServer(this, fields[0],
							Convert::ToInt32(fields[1]),
							Convert::ToInt32(fields[2]),
							Convert::ToInt32(fields[3]),
							gcnew VideoServer(controller,gs->remoteHost),
							numPlayableItems++
							));
				}
				// End
				else if (gs->rating == Rating::End && 
					gs->media == SliderTest::Media::Video &&
					gs->display == Display::Server) {
						if((fields->Length % 4))// Multiple of 4 entries ?
						{
							String^ result = resman->GetString("ERR_unknownServerItemMul",gs->systemculture);
							if( (gs->startedFromConsole == true) )
							{
								if(gs->silentProcessing == false)
									Console::WriteLine(result);
							}
							else
								MessageBox::Show( result, resman->GetString("PlaylistErrorHeader",gs->systemculture),MessageBoxButtons::OK, MessageBoxIcon::Error);
							return(-1);
						}
						else if (fields->Length > 4) {
							List<PlaylistItemPlayable^>^ itemList =	gcnew List<PlaylistItemPlayable^>();
							numPlayableItems++;
							for (int i = 0; i < fields->Length; i += 4) {
								itemList->Add(PlaylistItemEndRating::factoryVideoServer(this, fields[i + 0],
									Convert::ToInt32(fields[i + 1]),
									Convert::ToInt32(fields[i + 2]),
									Convert::ToInt32(fields[i + 3]),
									gcnew VideoServer(controller,gs->remoteHost),
									numPlayableItems
									));
							}
							AddItem(gcnew PlaylistItemSeveral(this, itemList, numPlayableItems));
						}
						else {
							AddItem(PlaylistItemEndRating::factoryVideoServer(this, fields[0],
								Convert::ToInt32(fields[1]),
								Convert::ToInt32(fields[2]),
								Convert::ToInt32(fields[3]),
								gcnew VideoServer(controller,gs->remoteHost),
								numPlayableItems++
								));
						}
				}
				// Sync
				else if (gs->rating == Rating::Sync && 
					gs->media == SliderTest::Media::Video &&
					gs->display == Display::Server) {
						if((fields->Length % 4))// Multiple of 4 entries ?
						{
							String^ result = resman->GetString("ERR_unknownServerItemMul",gs->systemculture);
							if( (gs->startedFromConsole == true) )
							{
								if(gs->silentProcessing == false)
									Console::WriteLine(result);
							}
							else
								MessageBox::Show( result, resman->GetString("PlaylistErrorHeader",gs->systemculture),MessageBoxButtons::OK, MessageBoxIcon::Error);
							return(-1);
						}
						else if (fields->Length > 4) {
							List<PlaylistItemPlayable^>^ itemList =	gcnew List<PlaylistItemPlayable^>();
							numPlayableItems++;
							for (int i = 0; i < fields->Length; i += 4) {
								itemList->Add(PlaylistItemSyncRating::factoryVideoServer(this, fields[i + 0],
									Convert::ToInt32(fields[i + 1]),
									Convert::ToInt32(fields[i + 2]),
									Convert::ToInt32(fields[i + 3]),
									gcnew VideoServer(controller,gs->remoteHost),
									numPlayableItems
									));
							}
							AddItem(gcnew PlaylistItemSeveral(this, itemList, numPlayableItems));
						}
						else {
							AddItem(PlaylistItemSyncRating::factoryVideoServer(this, fields[0],
								Convert::ToInt32(fields[1]),
								Convert::ToInt32(fields[2]),
								Convert::ToInt32(fields[3]),
								gcnew VideoServer(controller,gs->remoteHost),
								numPlayableItems++
								));
						}
				}
				// -- Remote --
				// Continuous
				else if (gs->rating == Rating::Continuous && 
					gs->media == SliderTest::Media::Video &&
					gs->display == Display::Remote) {
						AddItem(gcnew PlaylistItemConstantRating(this, fields[0],
						gcnew VideoRemote(controller,gs->remoteHost),
						numPlayableItems++
						));
				}
				// End
				else if (gs->rating == Rating::End && 
					gs->media == SliderTest::Media::Video &&
					gs->display == Display::Remote) {
						if (fields->Length > 1) {
							List<PlaylistItemPlayable^>^ itemList =	gcnew List<PlaylistItemPlayable^>();
							numPlayableItems++;
							for (int i = 0; i < fields->Length; i++) {
								itemList->Add(gcnew PlaylistItemEndRating(this, fields[i],
									gcnew VideoRemote(controller,gs->remoteHost),
									numPlayableItems
									));
							}
							AddItem(gcnew PlaylistItemSeveral(this, itemList, numPlayableItems));
						}
						else {
							AddItem(gcnew PlaylistItemEndRating(this, fields[0],
								gcnew VideoRemote(controller,gs->remoteHost),
								numPlayableItems++
								));
						}
				}
				}
			}
			
		if(filesNotExisting)
		{
			MessageBoxButtons buttons = MessageBoxButtons::OK;
			//MessageBoxDefaultButton defbutton = MessageBoxDefaultButton::Button2;
			String^ message = resman->GetString("PlaylistFilesNotFound", gs->systemculture)+Environment::NewLine + nonExistingItems;
			if( (gs->startedFromConsole == true) )
			{
				if(gs->silentProcessing == false)
					Console::WriteLine(message);
			}
			else
				MessageBox::Show( message, resman->GetString("PlaylistErrorHeader",gs->systemculture), buttons, MessageBoxIcon::Error );
			return(-1);
		}
	}
	finally	{
		delete sr;
	}
	ratingMethod->Init(gs);

	return(0);
}

bool Playlist::isNextItemAuto() {
	int nextpos = getNextItem();
	if (nextpos >= 0) {
		return playlist[nextpos]->playAutomatically;
	}
	return true;
	//if (currentPos + 1 < playlist->Count) {
	//	return playlist[currentPos + 1]->playAutomatically;
	//}
	//return false;
}
System::String^ Playlist::getItemNameAt(int pos) {	
	if(pos < playlist->Count)
		return(playlist[pos]->name);
	else
		return("");
}
int Playlist::getNextItem(void) {
	return(ratingMethod->getNextItem());
}
int Playlist::getsetNextItem(void) {
	return(ratingMethod->getsetNextItem());
}
bool Playlist::wasLastTurn(void) {
	return(ratingMethod->wasLastTurn());
}
void Playlist::PlayNext(void) {
	// prepare item before playback
	int currentpos = getsetNextItem();
	if(currentpos < 0)
	{
		String^ result="";
		switch(currentpos)
		{
			case RESULT_BEGINOFPLAYLIST :	result=resman->GetString("RESULT_BEGINOFPLAYLIST",controller->getSystemCulture());	break;
			case RESULT_MAXNUMTURNS :		result=resman->GetString("RESULT_MAXNUMTURNS",controller->getSystemCulture());	break;
			case RESULT_MINEXCEEDED :		result=resman->GetString("RESULT_MINEXCEEDED",controller->getSystemCulture());	break;
			case RESULT_MAXEXCEEDED :		result=resman->GetString("RESULT_MAXEXCEEDED",controller->getSystemCulture());	break;
			case RESULT_ALLITEMSPLAYED :	result=resman->GetString("RESULT_ALLITEMSPLAYED",controller->getSystemCulture());	break;
			default :	result="";	break;
		}
		controller->LogNoDate(result);
		/* No Game Over window
#ifdef DEBUG
		System::Windows::Forms::MessageBox::Show(result=resman->GetString("MSG_noMoreItems",controller->getUserCulture())+ Environment::NewLine + result, "Game Over",
			System::Windows::Forms::MessageBoxButtons::OK, System::Windows::Forms::MessageBoxIcon::Asterisk);
#else
		System::Windows::Forms::MessageBox::Show(result=resman->GetString("MSG_noMoreItems",controller->getUserCulture()), "Game Over",
			System::Windows::Forms::MessageBoxButtons::OK, System::Windows::Forms::MessageBoxIcon::Asterisk);
#endif
		*/
		controller->quitAppRegular();
	}
	else
	{
		PlayCurrent();
		controller->setLastItemAuto(playlist[currentpos]->playAutomatically);
	}
}
System::String^ Playlist::VirtualPlayNext(void) {
	// prepare item before playback
	int currentpos = getsetNextItem();
	System::String ^toPlay;
	if(currentpos < 0)
	{
		String^ result="";
		switch(currentpos)
		{
			case RESULT_BEGINOFPLAYLIST :	result=resman->GetString("RESULT_BEGINOFPLAYLIST",controller->getSystemCulture());	break;
			case RESULT_MAXNUMTURNS :		result=resman->GetString("RESULT_MAXNUMTURNS",controller->getSystemCulture());	break;
			case RESULT_MINEXCEEDED :		result=resman->GetString("RESULT_MINEXCEEDED",controller->getSystemCulture());	break;
			case RESULT_MAXEXCEEDED :		result=resman->GetString("RESULT_MAXEXCEEDED",controller->getSystemCulture());	break;
			case RESULT_ALLITEMSPLAYED :	result=resman->GetString("RESULT_ALLITEMSPLAYED",controller->getSystemCulture());	break;
			default :	result="";	break;
		}
		controller->LogNoDate(result);
		/* No Game Over window
#ifdef DEBUG
		System::Windows::Forms::MessageBox::Show(result=resman->GetString("MSG_noMoreItems",controller->getUserCulture())+ Environment::NewLine + result, "Game Over",
			System::Windows::Forms::MessageBoxButtons::OK, System::Windows::Forms::MessageBoxIcon::Asterisk);
#else
		System::Windows::Forms::MessageBox::Show(result=resman->GetString("MSG_noMoreItems",controller->getUserCulture()), "Game Over",
			System::Windows::Forms::MessageBoxButtons::OK, System::Windows::Forms::MessageBoxIcon::Asterisk);
#endif
		*/
		controller->quitAppRegular();
	}
	else
	{
		toPlay = playlist[currentpos]->name;
		controller->setLastItemAuto(false);
	}

	return toPlay;
}
void Playlist::PlayOneNext(int i) {
	PlayCurrent(i);
	controller->setLastItemAuto(playlist[controller->currentPos]->playAutomatically);
}
void Playlist::PlayInit() {
	// first time playing if necessary
	int inititem = 0;	// todo: find a better solution than playing the first entry
	playlist[inititem]->prepareItem();
	playlist[inititem]->PlayItem();
}
//void Playlist::PlayCurrent() {
//	// for backwards compatibility, should be removed sometime
//	PlayCurrentPre(2000);
//}
void Playlist::PlayCurrent() {
	// prepare item before playback
	// eg for Local playback
	playlist[controller->currentPos]->prepareItem();
	//System::Threading::Thread::Sleep(presleeptime);// NOW IN LOCAL/SERVER/REMOTE
	playlist[controller->currentPos]->PlayItem();
}
void Playlist::PlayCurrent(int i) {
	// prepare item before playback
	// eg for Local playback
	playlist[controller->currentPos]->prepareItem();
	//System::Threading::Thread::Sleep(presleeptime);// NOW IN LOCAL/SERVER/REMOTE

	PlaylistItemSeveral^ severalItems = dynamic_cast<PlaylistItemSeveral^>(playlist[controller->currentPos]);
	if(severalItems == nullptr)
		playlist[controller->currentPos]->PlayItem();
	else
		severalItems->PlayOneItem(i);

}
void PlaylistItemConstantRating::prepareItem() {
	server->PreparePlay(this);
}

void PlaylistItemConstantRating::PlayItem() {
	// call the server, do stuff

	// inform to start capturing
	parent->controller->StartLogging();
	parent->controller->StartCapture();

	// first increase, then play!
	parent->itemPos = itemNumber;
	parent->itemsPlayed++;

	// play the item
	server->PlayThreaded(this);
}
void PlaylistItemSyncRating::prepareItem() {
	server->PreparePlay(this);
}
void PlaylistItemSyncRating::PlayItem() {
	// inform to start capturing
	parent->controller->StartLogging();
	//parent->controller->StartCapture();

	// first increase, then play!
	parent->itemPos = itemNumber;
	parent->itemsPlayed++;

	// play the item
	server->PlayThreaded(this);
}
void PlaylistItemEndRating::prepareItem() {
	server->PreparePlay(this);
}
void PlaylistItemEndRating::PlayItem() {
	//server->PreparePlay(this);	// happens in Playlist::PlayCurrent()

	parent->controller->StartLogging();

	// play the item
	server->Play(this);

	parent->controller->StopLogging(name);

	// do some final stuff
	server->EndPlay();// check: war vorher aus.

	parent->itemPos = itemNumber;
	parent->itemsPlayed++;
}

void PlaylistItemSeveral::prepareItem() {}//new sr

// plays a list of items without rating pause, the log item will be called
// "Combined XXX", where XXX is the item's number
void PlaylistItemSeveral::PlayItem() {
	parent->controller->StartLogging();
	
	String^ logName = "";

	for each (PlaylistItemPlayable^ item in itemList) {
		logName += item->getName() + " ";
	
		// call the server, do stuff
		item->getServer()->PreparePlay(item);

		// play the item
		item->getServer()->Play(item);

		// do some final stuff
		item->getServer()->EndPlay();
	}
	
	parent->itemPos = itemNumber;
	parent->itemsPlayed++;// not cehcked, new
	parent->controller->StopLogging(logName);
}

void PlaylistItemSeveral::PlayOneItem(int i) {
	parent->controller->StartLogging();
	
	String^ logName = "";

	PlaylistItemPlayable^ item = itemList[i];
	logName = item->getName();
	
	// call the server, do stuff
	item->getServer()->PreparePlay(item);

	// play the item
	item->getServer()->Play(item);

	// do some final stuff
	item->getServer()->EndPlay();
	
	parent->itemPos = itemNumber;
	parent->itemsPlayed++;// not cehcked, new
	parent->controller->StopLogging(logName);
}



void PlaylistItem::showMessage(String^ msg, String^ header) {
	// Different handling of Messageboxes when a Hardwareslider is connected 
	if(parent->controller->generalSettings->device == Device::Hardware)
	{
		msgForm = gcnew Form();
		msgForm->ControlBox = false;	 	 
		msgForm->StartPosition = FormStartPosition::CenterScreen;
		msgForm->Width =  260;
		msgForm->Height = 150;
		msgForm->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedDialog;
		msgForm->Text = header;
		//msgForm->TopMost = true;
		msgForm->ShowInTaskbar = false;
		//msgForm->Shown += gcnew System::EventHandler(this, &PlaylistItem::FormMessage_Shown);
		//msgForm->FormClosed += gcnew System::Windows::Forms::FormClosedEventHandler(this, &PlaylistItem::FormMessage_FormClosed);
		Button^ but = gcnew Button;
		but->Text = L"OK";
		but->Width = 70;
		but->Location = System::Drawing::Point(msgForm->Width/2 - but->Width/2,msgForm->Height/5*3);

		System::Windows::Forms::Label^ msgLabel = gcnew System::Windows::Forms::Label;
		msgLabel->Size = System::Drawing::Size(msgForm->Width, msgForm->Height/2);
		msgLabel->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
		//msgLabel->AutoSize = true;
		//msgLabel->Location = System::Drawing::Point(16, 17);
		//msgLabel->Name = L"label1";
		msgLabel->Text = msg;
		msgForm->AcceptButton = but;
		msgForm->Controls->Add(but);
		msgForm->Controls->Add(msgLabel);
		timerCheckButtonPressed = gcnew System::Windows::Forms::Timer;
		timerCheckButtonPressed->Interval = 10;
		timerCheckButtonPressed->Tick += gcnew System::EventHandler(this, &PlaylistItem::timerCheckButtonPressed_Tick);

		// Wait while button still pressed from last pressing
		while(parent->controller->GetSliderButton() == true)
			System::Threading::Thread::Sleep(50);

		timerCheckButtonPressed->Start();

		msgForm->ShowDialog();
	}
	else
		MessageBox::Show(msg, header , MessageBoxButtons::OK, MessageBoxIcon::Information );
}
System::Void PlaylistItem::timerCheckButtonPressed_Tick(System::Object^  sender, System::EventArgs^  e) {
	// Check for button press
	if(parent->controller->GetSliderButton() == true)
	{
		timerCheckButtonPressed->Stop();

		// Wait for button unpress
		while(parent->controller->GetSliderButton() == true)
			System::Threading::Thread::Sleep(50);//check value of 50

		// Remove Dialog
		msgForm->Close();
	}
}

void PlaylistItemTrainStart::prepareItem() {}//new sr
void PlaylistItemTrainStart::PlayItem() {
	// train start message
	parent->controller->setPlayStatus(true);//important for HWend
	String^ header = parent->resman->GetString("MSGITEM_header", parent->controller->getUserCulture());
	String^ msg = parent->resman->GetString("MSGITEM_ts", parent->controller->getUserCulture());
	showMessage(msg, header);
	parent->controller->LogNoDate(":TS");
	parent->controller->setTrainingPhase(true);
	parent->controller->setPlayStatus(false);
}
void PlaylistItemTrainEnd::prepareItem() {}//new sr
void PlaylistItemTrainEnd::PlayItem() {
	// train end message
	parent->controller->setPlayStatus(true);
	String^ header = parent->resman->GetString("MSGITEM_header", parent->controller->getUserCulture());
	String^ msg = parent->resman->GetString("MSGITEM_te", parent->controller->getUserCulture());
	showMessage(msg, header);
	parent->controller->LogNoDate(":TE");
	parent->controller->setTrainingPhase(false);
	parent->controller->setPlayStatus(false);
}

void PlaylistItemPause::prepareItem() {}//new sr
void PlaylistItemPause::PlayItem() {
	// pause message
	parent->controller->setPlayStatus(true);
	String^ header = parent->resman->GetString("MSGITEM_header", parent->controller->getUserCulture());
	String^ msg = parent->resman->GetString("MSGITEM_pp", parent->controller->getUserCulture());
	showMessage(msg, header);
	parent->controller->LogNoDate(":PP");
	parent->controller->setPlayStatus(false);
}

void PlaylistItemEnd::prepareItem() {}//new sr
void PlaylistItemEnd::PlayItem() {
	// end message
	parent->controller->setPlayStatus(true);
	String^ header = parent->resman->GetString("MSGITEM_header", parent->controller->getUserCulture());
	String^ msg = parent->resman->GetString("MSGITEM_pe", parent->controller->getUserCulture());
	showMessage(msg, header);
	parent->controller->quitAppRegular();
	parent->controller->setPlayStatus(false);
}

void PlaylistItemCustomPopup::prepareItem() {}//new sr
void PlaylistItemCustomPopup::PlayItem() {
	// custom message
	parent->controller->setPlayStatus(true);
	showMessage(msgStr, titleStr);
	parent->controller->LogNoDate(":CC");
	parent->controller->setPlayStatus(false);
}

void PlaylistItemMessageVS::prepareItem() {}//new sr
void PlaylistItemMessageVS::PlayItem() {
	//NOT CHECKED!
	parent->controller->setPlayStatus(true);//NOT CHECKED!
	server->PreparePlay(this);
	parent->controller->LogNoDate(":MessageFrame");
	parent->controller->setPlayStatus(false);//NOT CHECKED!
}

void PlaylistItemWaitFrameVS::prepareItem() {}//new sr
void PlaylistItemWaitFrameVS::PlayItem() {
	parent->controller->setPlayStatus(true);//NOT CHECKED!
	server->PreparePlay(this);
	String^ header = parent->resman->GetString("MSGITEM_header", parent->controller->getUserCulture());
	String^ msg = parent->resman->GetString("MSGITEM_waitframe", parent->controller->getUserCulture());
	//NOT CHECKED!
	showMessage(msg, header );
	Threading::Thread::Sleep(time * 1000);
	parent->controller->LogNoDate(":WaitFrame");
	parent->controller->setPlayStatus(false);//NOT CHECKED!
}