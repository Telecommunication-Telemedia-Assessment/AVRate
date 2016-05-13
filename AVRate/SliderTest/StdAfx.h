// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
#pragma once
/*
  _WIN32_WINNT_NT4 = 0x0400,
  _WIN32_WINNT_WIN2K = 0x0500,
  _WIN32_WINNT_WINXP = 0x0501,
  _WIN32_WINNT_WS03 = 0x0502,
  _WIN32_WINNT_VISTA = 0x0600,
  _WIN32_WINNT_WIN7 = 0x0601
*/
#define WINVER 0x0501
#define _WIN32_WINNT 0x0501

//#define WARNING_SNDFILE_TAG_COMMENTED	// in sndfile.h, because there were warnings. can be reverted.

#ifdef DEBUG
#define kDefaultPlaylistLocation "playlist.lst"
#define kDefaultSettingsLocation "settings.xml"
#else
#define kDefaultPlaylistLocation "playlist.lst"
#define kDefaultSettingsLocation "settings.xml"
#endif

//#define USE_QUITBUTTON	// define to display a quit button on the SWend Form
#define kplayinitfile 0	// maybe put in settings check

// Graphic Alignement Defines
//#define kSCALEX 1.f//1.7f
//#define kSCALEY 1.f//1.6f
#define kSTDSCREENWIDTH 1024.f	// Standard screen dimensions
#define kSTDSCREENHEIGHT 768.f	// Standard screen dimensions
#define switchPositions false	// determines if slider labels are written in two lines 
#define kBOXLEFTMARGIN 20	// (DO NOT CHANGE) margin from left border to first Group, ABSOLUTE
#define kBOXTOBOXMARGIN 20	//ok margin from one Group box to another, ABSOLUTE
#define kBOXTOPMARGIN 110	//ok standard group box distance from top, must include Play button, GETS SCALED
#define kSLIDERLENGTH 300 // Pierre 300	// Default Slider width/height for hori/vert sliders, GETS SCALED
#define kBOXWIDTH 200 //200		//ok standard group box width, GETS SCALED (if it is in slider dimension, it is disabled and gets calced automatically from SliderHeight)
#define kBOXHEIGHT 160		//ok standard group box heigth, GETS SCALED (if it is in slider dimension it is disabled and gets calced automatically from SliderHeight)
#define kSLIDERH 40			//40 NOT SCALED: Fixed Slider 'broadness', sets also the margin to labels and ticks
#define kSLIDERMARGINX 25	//25 // 50 NOT SCALED: X Margin from slider to inside box border, set automatic for slider dimension(hor/vert)
#define kSLIDERMARGINY 30	//40 NOT SCALED: Y Margin from slider to inside box border, set automatic for slider dimension(hor/vert)
#define kBUTTONW 100		//standard button width, GETS SCALED
#define kBUTTONH 80			//standard button heigth, GETS SCALED

#define kPLAYBUTTONW 168		// Play-Button width, gets scaled
#define kPLAYBUTTONH 70			// Play-Button height, gets scaled.

#define kNEXTBUTTONW 40		// Play-Button width, gets scaled
#define kNEXTBUTTONH 40			// Play-Button height, gets scaled.

#define DEFAULT_MAXNUMTRIALS 10	// for constant method
#define DEFAULT_MAXNUMTURNS 3	// double & staircase
#define MAXOF_MINEXCEEDED 3	// RatingMethodStaircase : Maximum amount of exceeding the limits of playlist
#define MAXOF_MAXEXCEEDED 3	// RatingMethodStaircase : Maximum amount of exceeding the limits of playlist

// Finished Playlist Results
#define RESULT_BEGINOFPLAYLIST -1
#define RESULT_MAXNUMTURNS -5
#define RESULT_MAXNUMTRIALS -6
#define RESULT_MINEXCEEDED -8
#define RESULT_MAXEXCEEDED -9
#define RESULT_ALLITEMSPLAYED -10

#include <windows.h>