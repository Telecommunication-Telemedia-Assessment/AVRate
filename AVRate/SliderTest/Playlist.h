#pragma once

#include "Controller.h"
#include "ContentServer.h"
#include "Types.h"

using namespace SliderTest;

ref class Playlist;
ref class Controller;
ref class ContentServer;
ref class RatingMethod;

// abstract base class for all playlist items
ref class PlaylistItem abstract {
public:
	PlaylistItem() {}
	PlaylistItem(Playlist ^p) { playAutomatically = true; parent = p;}
	virtual void PlayItem() = 0;
	virtual void prepareItem() = 0;// new sr
	virtual void showMessage(System::String^,System::String^);
	//void FormMessage_Shown(System::Object^  sender, System::EventArgs^);
	//void FormMessage_FormClosed(System::Object^  sender, System::Windows::Forms::FormClosedEventArgs^  e) ; 
	System::Void timerCheckButtonPressed_Tick(System::Object^  sender, System::EventArgs^  e);
	bool playAutomatically;
	Playlist^ parent;
	int itemNumber; // == -1 for non-playable items, otherwise starting at 0#
	System::String^ name;
	System::Windows::Forms::Form^ msgForm;
	System::Windows::Forms::Timer^  timerCheckButtonPressed;
};

// base class for the items that contain actual content
ref class PlaylistItemPlayable abstract : public PlaylistItem {
public:
	virtual void PlayItem() override {};
	virtual void prepareItem() override {};
	int getLength() {return length; }
	int getFrame() {return frame; }
	int getStart() {return start; }
	System::String^ getName() {return name; }
	ContentServer^ getServer() {return server; }

protected:
	//System::String^ name;
	int start;
	int length;
	int frame;
	ContentServer^ server;
};


// playlist item with a continuous rating
ref class PlaylistItemConstantRating : public PlaylistItemPlayable {
public:
	static PlaylistItemConstantRating^ factoryAudio(Playlist ^p, System::String^ n, ContentServer^ serv, int num) {
		PlaylistItemConstantRating^ item = gcnew PlaylistItemConstantRating(p, n, serv, num);
		return item;
	}

	static PlaylistItemConstantRating^ factoryVideoServer(Playlist ^p, System::String^ n, int s, int l, int f, ContentServer^ serv, int num) {
		PlaylistItemConstantRating^ item = gcnew PlaylistItemConstantRating(p, n, serv, num);
		item->start = s;
		item->length = l;
		item->frame = f;
		return item;
	}

	PlaylistItemConstantRating(Playlist ^p, System::String^ n, ContentServer^ serv, int num) { 
		parent = p;
		name = n;
		server = serv;
		playAutomatically = false;
		itemNumber = num;
	}

	virtual void PlayItem() override;
	virtual void prepareItem() override;//new sr
};

// playlist entry that gets rated at the end
ref class PlaylistItemEndRating : public PlaylistItemPlayable {
public:
	static PlaylistItemEndRating^ factoryAudio(Playlist ^p, System::String^ n, ContentServer^ serv, int num) {
		PlaylistItemEndRating^ item = gcnew PlaylistItemEndRating(p, n, serv, num);
		return item;
	}

	static PlaylistItemEndRating^ factoryVideoServer(Playlist ^p, System::String^ n, int s, int l, int f, ContentServer^ serv, int num) {
		PlaylistItemEndRating^ item = gcnew PlaylistItemEndRating(p, n, serv, num);
		item->start = s;
		item->length = l;
		item->frame = f;
		return item;
	}

	PlaylistItemEndRating(Playlist ^p, System::String^ n, ContentServer^ serv, int num) { 
		parent = p;
		name = n;
		server = serv;
		playAutomatically = false;
		itemNumber = num;
	}

	virtual void PlayItem() override;
	virtual void prepareItem() override;//new sr
};

// playlist entry that gets rated at the end but is started threaded for monitoring keypress activity
ref class PlaylistItemSyncRating : public PlaylistItemPlayable {
public:
	static PlaylistItemSyncRating^ factoryAudio(Playlist ^p, System::String^ n, ContentServer^ serv, int num) {
		PlaylistItemSyncRating^ item = gcnew PlaylistItemSyncRating(p, n, serv, num);
		return item;
	}

	static PlaylistItemSyncRating^ factoryVideoServer(Playlist ^p, System::String^ n, int s, int l, int f, ContentServer^ serv, int num) {
		PlaylistItemSyncRating^ item = gcnew PlaylistItemSyncRating(p, n, serv, num);
		item->start = s;
		item->length = l;
		item->frame = f;
		return item;
	}

	PlaylistItemSyncRating(Playlist ^p, System::String^ n, ContentServer^ serv, int num) { 
		parent = p;
		name = n;
		server = serv;
		playAutomatically = false;
		itemNumber = num;
	}

	virtual void PlayItem() override;
	virtual void prepareItem() override;//new sr
};

// special playlist items, showing pop-ups and the like
// can be used in the playlist with their respective marker (":XX")

// :TS -- indicate the start of the training phase
ref class PlaylistItemTrainStart : public PlaylistItem {
public:
	PlaylistItemTrainStart(Playlist ^p) : PlaylistItem(p) { }
	virtual void PlayItem() override;
	virtual void prepareItem() override;//new sr
};

// :TE -- indicate the end of the training phase
ref class PlaylistItemTrainEnd : public PlaylistItem {
public:
	PlaylistItemTrainEnd(Playlist ^p) : PlaylistItem(p) { }
	virtual void PlayItem() override;
	virtual void prepareItem() override;//new sr
};

// :PP -- pause
ref class PlaylistItemPause : public PlaylistItem {
public:
	PlaylistItemPause(Playlist ^p) : PlaylistItem(p) { }
	virtual void PlayItem() override;
	virtual void prepareItem() override;//new sr
};

// :PE -- end of the playlist
ref class PlaylistItemEnd : public PlaylistItem {
public:
	PlaylistItemEnd(Playlist ^p) : PlaylistItem(p) { }
	virtual void PlayItem() override;
	virtual void prepareItem() override;//new sr
};

// :CC -- pop up with own text
ref class PlaylistItemCustomPopup : public PlaylistItem {
public:
	PlaylistItemCustomPopup(Playlist ^p, System::String^ str) : PlaylistItem(p) {
		titleStr = str->Split('|')[0];
		msgStr = str->Split('|')[1];
	
	}
	virtual void PlayItem() override;
	virtual void prepareItem() override;//new sr

	System::String^ titleStr;
	System::String^ msgStr;
};

// :Frame <frame> -- show a certain frame on the VS (e.g. "Pause")
ref class PlaylistItemMessageVS : public PlaylistItemPlayable {
public:
	PlaylistItemMessageVS(Playlist ^p, int s, ContentServer^ serv) { 
		parent = p;
		start = s;
		length = 0;
		server = serv;
		playAutomatically = true;
	}
	virtual void PlayItem() override;
	virtual void prepareItem() override;//new sr
};

// :WaitFrame <frame> <time> -- like :Frame, but also present 
// a popup and wait for the supervisor to click OK. Then wait another _time_ seconds,
// then play the next item
// only useful together with hardware sliders and more than one user
ref class PlaylistItemWaitFrameVS : public PlaylistItemPlayable {
public:
	PlaylistItemWaitFrameVS(Playlist ^p, int s, int t, ContentServer^ serv) { 
		parent = p;
		start = s;
		length = 0;
		server = serv;
		time = t;
		playAutomatically = true;
	}
	virtual void PlayItem() override;
	virtual void prepareItem() override;//new sr
	int time;
};



// playlist keeps the list of items and the current position in the list
ref class Playlist
{
public:
	Playlist(Controller^ c);
	void AddItem(PlaylistItem^ p);
	int Initialize(SliderTest::GeneralSettings^ gs);
	void PlayCurrent();
	void PlayCurrent(int i);
	//void PlayCurrentPre(int);
	void PlayInit();
	void PlayNext();
	System::String^ VirtualPlayNext();
	void PlayOneNext(int i);
	System::String^ getItemNameAt(int);
	int getNextItem();
	int getsetNextItem();
	bool isNextItemAuto();
	bool wasLastTurn();

	System::Collections::Generic::List<PlaylistItem^>^ playlist;
	int numPlayableItems;
	int numComparedSequences;
	int numAutoItems;
	int itemsPlayed;		// number of played 
	int itemPos;		// number of played items (videos etc.)
	Controller^ controller;
	System::Resources::ResourceManager^ resman;	
protected:
private:
	SliderTest::Display disp;	// display type locally stored (local/server/remote)
	RatingMethod^ ratingMethod;
};


// playlist entry that gets rated at the end
ref class PlaylistItemSeveral : public PlaylistItem {
public:
	PlaylistItemSeveral(Playlist ^p, System::Collections::Generic::List<PlaylistItemPlayable^>^ il, int num) { 
		parent = p;
		playAutomatically = false;
		itemNumber = num;
		itemList = il;
		if(p->playlist->Count != 0 && p->numComparedSequences != il->Count && p->controller->generalSettings->rmtype == RatingMethodType::SAMVIQ) {
			String^ message = p->resman->GetString("PlaylistFilesMisssing", p->controller->generalSettings->systemculture);
			MessageBox::Show( message, p->resman->GetString("PlaylistMissingEntries",p->controller->generalSettings->systemculture),MessageBoxButtons::OK, MessageBoxIcon::Error);
		}
		p->numComparedSequences = il->Count;
	}

	virtual void PlayItem() override;
	virtual void PlayOneItem(int i);
	virtual void prepareItem() override;//new sr
	
	private:
	System::Collections::Generic::List<PlaylistItemPlayable^>^ itemList;
};

