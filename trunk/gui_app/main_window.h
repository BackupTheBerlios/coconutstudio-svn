//
// C++ Interface: main_window
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include "base/window.h"
#include "containers/box_container.h"
#include "containers/stack_container.h"
#include "widgets/label.h"
#include "widgets/line_edit.h"

#include "widgets/tab_bar.h"
#include "widgets/menu_button.h"

#include "gui_app/new_track_dialog.h"
#include "gui_app/audio_settings_dialog.h"
#include "gui_app/gui_update_notify.h"
#include "gui_app/graph_screen.h"

#include "engine/song.h"

using namespace GUI;
/**
	@author Juan Linietsky <reduzio@gmail.com>
*/
class MainWindow : public Window {

	enum MenuOption {
		
		FILE_NEW_SONG,
  		FILE_OPEN_SONG,
    		FILE_SAVE_SONG,
      		FILE_SAVE_SONG_AS,
      		FILE_IMPORT_MIDI,
      		FILE_EXPORT_MIDI,
      		FILE_EXPORT_WAV,
		FILE_QUIT,
  		TRACK_ADD_PATTERN,
  		TRACK_ADD_AUDIO,
  		TRACK_ADD_AUTOMATION,
  		GRAPH_ADD_NODE,
  		GRAPH_ADD_LAYER,
  		GRAPH_CHANGE_LAYER,
  		GRAPH_ERASE_LAYER,
      		EDIT_UNDO,
      		EDIT_REDO,
		CONTROL_PLAY,
		CONTROL_LOOP,
		CONTROL_PAUSE,
		CONTROL_STOP,
		CONTROL_FF,
		CONTROL_RW,
		CONTROL_REC,
		CONTROL_REC_AUTO,
      		TOOL_AUDIO_SETTINGS,
      		TOOL_KEYBOARD,
      		TOOL_INTERFACE,
      		TOOL_DEFAULT_PATHS,
  		HELP_HELP,
    		HELP_ABOUT,
      		
	};

	
	TabBar * tab_bar;
	
	StackContainer * main_stack;
	
	LineEdit *info_line;
	
	LineEdit *time_line;
	
	void menu_callback(int p_option);
		
//	GlobalView *global_view;
	//TrackView *track_view;
	GraphScreen *graph_screen;

	NewTrackDialog *new_track_dialog;
	AudioSettingsDialog *audio_settings_dialog;

	VBoxContainer *main_vbc;
	
	
//	GUI_UpdateNotify * update_notify;
//	void create_keybindings();	


	/* DATA */
	
	Song song;

	bool quit;
	
	GUI_UpdateNotify update_notify;
	EditCommands edit_comands; // singleton here
public:
	
	void init(String p_config_file="");
	
	void quit_request();
	bool must_quit();
	
	MainWindow(Painter *p_painter,Timer *p_timer,Skin *p_skin);	
	~MainWindow();

};

#endif
