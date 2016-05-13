#include "StdAfx.h"
#include "AudioLocal.h"
#include "sndfile.hh"
#include <vcclr.h>

#include "rtaudio/RtAudio.h"

AudioLocal::AudioLocal(void) { }

struct OutputData {
  unsigned int channels;
  int* buffer;
  int currentFrame;
  int bufferByteLength;
  int bufferFrameLength;
  int frameSize;
  int itemSize;
};

// Interleaved buffers
int output( void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames,
            double streamTime, RtAudioStreamStatus status, void *data ) {
	OutputData *oData = (OutputData*) data;

	int bytePos = oData->currentFrame * oData->frameSize; // pos in the file in bytes
	int byteOffset = nBufferFrames * oData->frameSize;
	
	if (bytePos + byteOffset < oData->bufferByteLength) {
		memcpy(outputBuffer, (unsigned char*)oData->buffer + bytePos,
			nBufferFrames * oData->frameSize);
		oData->currentFrame += nBufferFrames;
	}
	else
		return 1;

	// fill the buffer with 0s, if the data is all played
	unsigned int count = oData->bufferFrameLength - oData->currentFrame;
	if ( count < nBufferFrames ) {
		unsigned int bytes = (nBufferFrames - count) * oData->channels * sizeof(int);
		unsigned int startByte = count * oData->channels * sizeof(int);
		memset( (unsigned char*)(outputBuffer)+startByte, 0, bytes );
		return 1;
	}


	return 0;
}

void AudioLocal::PreparePlay(PlaylistItemPlayable^ item) {}

void AudioLocal::Play(PlaylistItemPlayable^ item) {
	System::String^ name = item->getName();
	
	unsigned int bufferFrames = 512;
	unsigned int device = 0, offset = 0;
	
	RtAudio dac;
	if ( dac.getDeviceCount() < 1 ) {
		std::cout << "\nNo audio devices found!\n";
		exit( 0 );
	}
	
	// Set our stream parameters for output only.
	RtAudio::StreamParameters oParams;
	oParams.deviceId = device;
	oParams.firstChannel = offset;
	
	OutputData data;
	char* nameChar = new char[name->Length + 1];
	for (int i = 0; i < name->Length; i++)
		nameChar[i] = (char)name[i];
	nameChar[name->Length] = '\0';

	SndfileHandle* sfh = new SndfileHandle(nameChar);
	
	data.currentFrame = 0;
	data.channels = sfh->channels();
	int format = RTAUDIO_SINT32; // always read data as int
	data.itemSize = sizeof(int);

	data.frameSize = data.itemSize * data.channels;
	data.bufferFrameLength = (int)sfh->frames();
	
	int size = (int)(data.bufferFrameLength * data.frameSize);

	data.bufferByteLength = size;

	data.buffer = (int*)malloc(size);
	int amount = (int)sfh->frames();
	sfh->readf(data.buffer, amount);

	float volAmplify = item->parent->controller->getVolume();

	for (unsigned int i = 0; i < data.bufferFrameLength * data.channels; ++i) {
		data.buffer[i] = (int)(data.buffer[i] * volAmplify);
	}

	oParams.nChannels = data.channels;

	try {
		dac.openStream(&oParams, NULL, format, sfh->samplerate(), &bufferFrames, &output, (void *)&data);
		dac.startStream();
	}
	catch (RtError& e) {
		e.printMessage();
		return;
	}

	while ( 1 ) {
		System::Threading::Thread::Sleep(100); // wake every 100 ms to check if we're done
		if (dac.isStreamRunning() == false) break;
	}

	dac.closeStream();
	delete data.buffer;
}

void AudioLocal::EndPlay() { }