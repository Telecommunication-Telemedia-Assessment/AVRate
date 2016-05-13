# Introduction

AVrate is a tool to conduct audiovisual evaluation experiments.
It performs the playback of audiovisual content and then asks test persons to rate it with a software- or hardware-based input. Software inputs can be sliders, to rate on a position on a scale, or buttons, which can be clicked. Additionally, timing information about keyboard pressing can be used for detection tests. A Hardware input is an external slider attached over USB.

The playback will be executed by common local or remote audio/video playback software or by a remote video server.

# Contents and Installation

- `AVrate.exe`: The Application
- `AVrating.ico`: Icon for rating window, can be exchanged
- `AVrate.pdb`: Program database, contains debug information
- `AxInterop.WMPLib.1.0.dll`: Library needed for Windows media player
- `Interop.WMPLib.1.0.dll`: Library needed for Windows media player
- `Libsndfile-1.dll`: Audio library
- `K8055D_C.DLL`: Library needed for the Hardware Slider
- `Settings.xml`: Example settings file
- `Playlist.pls`: Example playlist
- `AVrateDocumentation.pdf`: This documentation
- `/de`: Language Resource Files (German)

Library files must be in the same directory as AVrate.exe.

The .NET framework (V2.0) must be installed on the system.

The Icon for the Rating window can be customized by overwriting the file `AVrating.ico` with a different icon file.

The tool has been tested to run on a Windows XP operating system.

# Compatibility and Known Issues

Video playback is supported for the following Soft/Hardware:

- DVS Pronto HD.2 V2.2.2.3 Build 27661
- Windows Media Player (as ActiveX Control),
- VLC Player V2.0.x,
- MPlayer V1.0rc2

Playback performance is videoplayer-dependant. (See `Performance` sections below)

Specific settings are not available (see Settings description below)
Sleeptime in conjunction with continuous rating and local players (except WMP) expands the results list (because Rating values are sampled and logged as well while sleeping)
Initialization procedures of local video players, especially on the first playback, can lead to cutting-off of video display (Sleeptime can only prevent this in certain cases)

Appearance issues:

- Mouse is hidden when Network Socket Error Messagebox appears
- Server/ Remote setting:
  - Mouse reappears when moved
 	- Item count display is updated after playback finished
- Fontsize small on uncommon display resolutions

# Using AVrate

Run `AVrate.exe` to start or use it as a command line tool (see also section `Command Line Usage`).

Using AVrate proceeds in three configuration steps:
-	configuring the main XML settings file (see sections `Settings Overview` and `Settings Details`)
-	Generating a playlist file containing information about the data that is to be played (see section `Playlist`)
-	Providing some user data and paths to files on AVrate`s start screen.


## Hidden keys - Special commands

To stop the evaluation: Type `exit` (hidden) to quit the program.

To test the connection and functionality of the Hardware Slider: Type `tst` (hidden) and a window will appear showing the status of connected Hardware Sliders.

## Command Line Usage

Run `AVrate.exe –h` in the console to see available arguments.
Batch file processing is possible. To have Messageboxes appear in between consecutive tests, AVrate allows to manually pop up Message boxes with a customizable text.


# Settings overview

The preferences for an evaluation session are written in an XML file which must be selected in the start screen of AVrate, or specified in the commandline. See also the accompanying example settings file `settings.xml` for syntax. Greyed out values are currently under progress and might only work with standard values or are completely non-functional. Bold values are standard values.

When using the hardware slider at a local display, the player should be Mplayer (under discussion).

Possible values include:

### rating = {continuous, *end*, sync}

- continuous: rating while playback (only the first slider is being used (also if device = software) It is recommended to auto-hide the taskbar (Right-click on Start button -> Properties -> Auto-hide the taskbar)
- end: rating after playback
- sync: combined keylogging while playback and rating after playback

### ratingmethod = {standard, constant, staircase, double}

the method used to proceed to next playlist items throughout the test

- Standard = play files in the order of the playlist
- Constant = Method of Constant Stimuli, random order playback
- Staircase = Adaptive staircase method
- Double = Double staircase method

Note that special Playlist items are evaluated only for Standard method.

### ratingmethodrule = {valuestring}

evaluation rule parameter for ratingmethod.

- for Standard method: No parameter needed
- for Constant method: No parameter needed
- for Staircase and Double Staircase method:

  Specifies the step rule, e.g. UD for 1up-1down  
	Example target threshold levels:

        UD    50.0 %
        UDD   70.7 %
        UDDD  79.4 %
        UDDDD	84.1 %

  Note: These methods are designed for use with 2 buttons (eg YES/NO answering scheme). If there are sliders AND buttons, only the last specified of those will be evaluated for the method. So specify buttons AFTER sliders in the settings file.
  The first specified button in the settings file relates to a `NO` answer. For an UDD rule, pressing this button once (1-up-rule) results in going up  one step in the playlist, corresponding to an intensity increase. Because of this, it is assumed that the playlist is sorted in decreasing intensity.

### ratingmethodbreakcount = {valuestring}

break criterion parameter for ratingmethod

- for Standard method: no parameter needed
- for Constant method: Specifies the number of trials to execute before break
- for Staircase method: Specifies the number of answer reversals until break
- for Double method: Specifies the number of answer reversals until break

Note that a general break criterion for Staircase and Double method is, when the limit of the playlist in one or the other direction has been exceeded according to `MAXOF_MINEXCEEDED` and `MAXOF_MAXEXCEEDED` (currently hardcoded to 3)

### device = {software, hardware}

- software: Uses an internal software slider for Rating
- hardware: Uses a specific hardware interface. `Layout` setting will be ignored. From the first slider, only the min and max values are being used.

### sleeptime = {value}

Introduces a fixed delay in milliseconds in the playback process to prevent cutting off of playback.

If nothing is specified, the default values will be assigned (RECOMMENDED).

- For Local playback (Default: 0): The value simply delays the start of playback. Note that using a sleeptime together with continuous rating, the number of sampled rating values will increase (Except WMP player).
- For Remote playback (Default: 1500): The value does not affect the actual playback starting point as seen by the user. The video starts immediately, but the sleeptime is still important to be attached internally before the connection is closed.
- For Server playback (Default: 2000): The values delays the start of playback which may be important to stabilize the picture on the TV. In opposition to local playback, setting this value together with continuous rating, the number of sampled rating values will NOT increase.


### layout = {horizontal, vertical, p800, onscreen}

layout orientation of slider/buttons.

The highest slider value/button number is right (for horizontal layout) and top (for vertical layout).

- p800: device is ignored, also the sliders/buttons below, rating always "end"
- onscreen: onscreen video rating, needs two sliders (for the horizontal and vertical scale), only sensible with local video

### media = {audio, video}

currently not used. Audio playback is handled by Windows Media Player (internally as ActiveX Control)

### display = {local, remote, server}

- local: play the files on the same computer as the tool is running on
- remote: AVrate runs locally while the video plays back on a remote computer.

  Example configuration with direct Ethernet connection:
	- Local IP :				192.168.0.116
	- Remote IP: 				192.168.0.220
  - AVrate `remotehost` setting :	192.168.0.220:42219

   Start VLC on remote computer with this command line :
`vlc.exe --extraintf=rc --rc-host=192.168.0.220:42219 --rc-quiet --qt-display-mode=2 --no-video-title-show`

  Check your firewall settings if no connection can be made.

- server: AVrate runs locally while the video plays back on a DVS Pronto HD.2 system. Specify Pronto Server IP and port in the `remotehost` setting.

### remotehost = {<ip:port>}

### repeat = {true, false}

possibility to repeat a playlist item (how should it be done for hardware slider rating?)

### localplayer = {wmp, vlc10, mplayer, stereoscopic-player-nvidia, stereoscopic-player-sbs, command_line}

Available video players:

- Windows Media Player (as ActiveX Control)
- VLC Player V2.x
- MPlayer V1.0rc2
- Stereoscopic player 2.3
- command_line allows to run a command line based video player

Available audio players:

- Windows Media Player (as ActiveX Control), does not need to be specified

### playerpath = complete path to the executable of localplayer

### playerarguments = the arguments to pass to the player in case the selected local player is "command_line". In the arguments, %1 is a placeholder for the file to play according to the playlist. Ex. -i %1 --fullscreen

### volumeform = { none, slider, button }

allow changing the volume during the training phase of an audio test

### userlanguage = { en, de }

Language for test person

### systemlanguage = { en, de }

Language for supervisor

### logging = { file, database }

for every needed logging mechanism, add one logging entry (if you want no logging, don't add any)

### waittime = <float>

Time in secs to wait between items, when there is more than 1 user and hardware sliders are being used

### samplinginterval = <int>

Interval between two sampling points for continuous rating (in ms) -> sampling frequency = 1 / samplinginterval

### Slider settings

Note: When used with a Hardware slider, only min and max values are evaluated.

- name: Name for the surrounding box, for no name, set it empty
- label: one entry for each label, labels will be evenly distributed along the slider
- min, max: lowest and highest value the slider will return
- ticks: number of evenly distributed ticks (little marks) besides the slider
- fixed: determines if only the values at each tick can be chosen (set to true) or continuous values (set to false)
- shownumbers: if true, a number will be shown besides each tick with the corresponding numerical value of the slider at that position

### Button settings

Note: The first specified button in the user interface relates to a `NO` answer. See also the notes in `ratingmethodparameter` if you use other methods than Standard.

- name: Name for the surrounding box, for no name, set it empty
- label: one entry for each labelled button, the first one has a `0` or `NO` answer meaning internally

# Playlist

- Consists of one item per line
- Playable items (video/audio files)
- Special items (Only for Standard Ratingmethod)

The playlist must contain elements according to the following rules:

### Audio Items

    <WAV filename>

### Video Items with Local or Remote Playback

    <Video filename>

Due to startup delays of video players, depending on the computers performance, it is recommended to double the first entry in continuous rating in order to get valid values beginning from the second entry.

### Video Items with Server Playback

    <Name> <Start point> <Number of frames> <FPS>

Special items like pop-up boxes starting with ":" are available, not all are useful in conjunction with every media/display combination. The items are ignored if the Ratingmethod is not set to standard. Starting AVrate from command line allows to manually pop up Message boxes with a customizable text.

### Pop-ups

Note: not useful in conjunction with video server/slider rating

- `:TS`: indicate the start of the training phase
- `:TE`: indicate the end of the training phase
- `:PP`: pause
- `:PE`:	end of the playlist
- `:CC`:	pop-up with own text

### Special items for the video server

Note: This feature is not tested as of 30-Jul-2009.

- `:Frame <frame>`: show a certain frame on the VS (e.g. "Pause")
- `:WaitFrame <frame> <time>`: like `:Frame`, but also present a popup and wait for the supervisor to click OK. Then wait another `<time>` seconds, then play the next item. Useful together with hardware sliders and more than one user, when items should be played automatically and not by someone pushing a button.  
  It is also possible to have several files be played consecutively and then have a final rating for these files. For that purpose, all the files meant to be played consecutively are placed after each other on a single line. For elements needing more than one string (like Video/Server), the sets of strings are placed after each other.

# Results logfile

When Logging is set to `file` in the Settings file, results are written with ascending file name numbers to the `logs` directory which is located in the current working directory.

In the `Settings` paragraph of the logfile, the currently attached Settings are logged.

In the `Results` paragraph, the main results are logged line per line, in chronological playing order, with these entries:

    [Date] [Time] [Rating Duration] [Filename] [Flag (optional)] [Result Value(s)]

The Rating Duration measures the time between clicks on the Play button. Note that all delay times are summed in, like processing delays, video length and sleeptime value.

The optional Flag shows:

- `<TURN>` when reversal of rating direction within Staircase or Double Methods occured
- `<KEYS>` followed by timing info about pressed keyboard keys during the test (Sync rating)

Possible result values are:
-	0 or 1 for buttons
-	A comma value in the specified range, or -1 when no rating was made (eg. In Continuous rating)
-	The corresponding abbreviation for Special Items, eg `:TS` (see Playlist section)

The `Rating Histogram` paragraph shows the number of YES-answers over the number of times an item was played. This is intented to be used in conjunction with rating buttons, as the values of sliders just add up to a sum which is not very meaningful.

The `Additional Results` paragraph gives a summary of the items that caused a reversal of rating direction within Staircase or Double Methods, logged, in chronological playing order.


# Acknowledgment


The work done in the development of this software has been carried out thanks to the funding of Deutsche Telekom AG. 
This development was performed in the context of the development of audiovisual quality models performed at the Technical University of Berlin in the group Assessment of IP-based Application.  



# License

GNU General Public License v3

This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program. If not, see http://www.gnu.org/licenses/.


