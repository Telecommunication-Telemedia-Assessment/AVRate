#pragma once

#include "Playlist.h"

ref class PlaylistItemPlayable;

ref class ContentServer
{
public:
	ContentServer(void);

	virtual void PreparePlay(PlaylistItemPlayable^ item) {}
	virtual void Play(PlaylistItemPlayable^ item) {}
	virtual void PlayThreaded(PlaylistItemPlayable^ item) {}
	virtual void EndPlay() {}
};

ref class FakeServer : ContentServer {
public:
	FakeServer() {}
	virtual void PreparePlay(PlaylistItemPlayable^ item) override {}
	virtual void Play(PlaylistItemPlayable^ item) override { System::Threading::Thread::Sleep(3000); }
	virtual void EndPlay() override {}
};
