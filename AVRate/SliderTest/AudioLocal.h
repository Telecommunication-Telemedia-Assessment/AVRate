#pragma once
#include "contentserver.h"
#include "Playlist.h"

ref class AudioLocal : public ContentServer {
public:
	AudioLocal(void);
	virtual void PreparePlay(PlaylistItemPlayable^ item) override;
	virtual void Play(PlaylistItemPlayable^ item) override;
	virtual void EndPlay() override;
};
