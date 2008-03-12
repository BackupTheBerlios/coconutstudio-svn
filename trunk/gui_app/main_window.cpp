//
// C++ Implementation: main_window
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "main_window.h"

#include "widgets/separator.h"
#include "widgets/icon.h"
#include "bundles/menu_box.h"
#include "gui_common/common_skin.h"


void MainWindow::init(String p_config_file) {

	AudioDriverManager::set_audio_graph( song.get_audio_graph() );

	if (p_config_file=="") {
	
		
		audio_settings_dialog->init_default();
	}

}

void MainWindow::menu_callback(int p_option) {

	switch(p_option) {
		case TRACK_ADD_PATTERN: {
		
			new_track_dialog->show("New Pattern Track",Track::TYPE_PATTERN);
		} break;
		case TRACK_ADD_AUDIO: {
		
			new_track_dialog->show("New Audio Track",Track::TYPE_AUDIO);
		} break;
		case TRACK_ADD_AUTOMATION: {
			new_track_dialog->show("New Automation Track",Track::TYPE_AUTOMATION);
		} break;
		case TOOL_AUDIO_SETTINGS: {
		
			audio_settings_dialog->show();
		} break;
	}

}


void MainWindow::quit_request() {

	quit=true;
}

bool MainWindow::must_quit() {

	return quit;
}


MainWindow::MainWindow(Painter *p_painter,Timer *p_timer,Skin *p_skin) : Window(p_painter,p_timer,p_skin) {

	main_vbc = new VBoxContainer;
	set_root_frame(main_vbc);
	
	HBoxContainer *hb = main_vbc->add(new HBoxContainer);
	
	tab_bar = hb->add( new TabBar );
	
	tab_bar->add_tab("Global");
	tab_bar->add_tab("Tracks");
	tab_bar->add_tab("Graph");
	
	hb->add( new VSeparator );
	
	MenuBox * file_menu = hb->add( new MenuBox("File") );
	
	file_menu->item_selected_signal.connect( this, &MainWindow::menu_callback );
	
	file_menu->add_item("New Song",FILE_NEW_SONG);
	file_menu->add_item("Open Song",FILE_OPEN_SONG);
	file_menu->add_item("Save Song",FILE_SAVE_SONG);
	file_menu->add_item("Save Song As",FILE_SAVE_SONG_AS);
	file_menu->add_separator();
	file_menu->add_item("Import MIDI File",FILE_IMPORT_MIDI);
	file_menu->add_item("Export MIDI File",FILE_EXPORT_MIDI);
	file_menu->add_separator();
	file_menu->add_item("Export WAV",FILE_EXPORT_WAV);
	file_menu->add_separator();
	file_menu->add_item("Quit",FILE_QUIT);
	
	MenuBox * track_menu = hb->add( new MenuBox("Track") );
	
	track_menu->item_selected_signal.connect( this, &MainWindow::menu_callback );
	
	track_menu->add_item("Add Pattern Track",TRACK_ADD_PATTERN);
	track_menu->add_item("Add Audio Track",TRACK_ADD_AUDIO);
	track_menu->add_item("Add Automation Track",TRACK_ADD_AUTOMATION);
	
	MenuBox * graph_menu = hb->add( new MenuBox("Graph") );
	graph_menu->add_item("Add Node",GRAPH_ADD_NODE);
	graph_menu->add_separator();
	graph_menu->add_item("Add Layer",GRAPH_ADD_LAYER);
	graph_menu->add_item("Change Layer",GRAPH_CHANGE_LAYER);
	graph_menu->add_item("Erase Layer",GRAPH_ERASE_LAYER);
	
	hb->add( new VSeparator );
	
	hb->add( new MenuButton(get_skin()->get_bitmap(BITMAP_ICON_UNDO)) )->pressed_signal.connect( Method( Method1<int>(this, &MainWindow::menu_callback), (int)EDIT_UNDO ));
	hb->add( new MenuButton(get_skin()->get_bitmap(BITMAP_ICON_REDO)) )->pressed_signal.connect( Method(Method1<int>(this, &MainWindow::menu_callback), (int)EDIT_REDO ));	
	
	hb->add( new VSeparator );

	hb->add( new MenuButton(get_skin()->get_bitmap(BITMAP_CONTROL_RW)) )->pressed_signal.connect( Method( Method1<int>(this, &MainWindow::menu_callback), (int)CONTROL_RW ));
	hb->add( new MenuButton(get_skin()->get_bitmap(BITMAP_CONTROL_PLAY)) )->pressed_signal.connect( Method( Method1<int>(this, &MainWindow::menu_callback), (int)CONTROL_PLAY ));
	hb->add( new MenuButton(get_skin()->get_bitmap(BITMAP_CONTROL_LOOP)) )->pressed_signal.connect( Method( Method1<int>(this, &MainWindow::menu_callback), (int)CONTROL_LOOP ));
	hb->add( new MenuButton(get_skin()->get_bitmap(BITMAP_CONTROL_PAUSE)) )->pressed_signal.connect( Method( Method1<int>(this, &MainWindow::menu_callback), (int)CONTROL_PAUSE ));
	hb->add( new MenuButton(get_skin()->get_bitmap(BITMAP_CONTROL_STOP)) )->pressed_signal.connect( Method( Method1<int>(this, &MainWindow::menu_callback), (int)CONTROL_STOP ));
	hb->add( new MenuButton(get_skin()->get_bitmap(BITMAP_CONTROL_FF)) )->pressed_signal.connect( Method( Method1<int>(this, &MainWindow::menu_callback), (int)CONTROL_FF ));
	hb->add( new MenuButton(get_skin()->get_bitmap(BITMAP_CONTROL_REC)) )->pressed_signal.connect( Method( Method1<int>(this, &MainWindow::menu_callback), (int)CONTROL_REC ));
	hb->add( new MenuButton(get_skin()->get_bitmap(BITMAP_CONTROL_REC_AUTO)) )->pressed_signal.connect( Method( Method1<int>(this, &MainWindow::menu_callback), (int)CONTROL_REC_AUTO ));
	
	hb->add( new VSeparator );
	
	hb->add( new Widget, 1 ); //expand
	
	hb->add( new VSeparator );
	
	MenuBox * tools_menu = hb->add( new MenuBox("Config") );
	
	tools_menu->item_selected_signal.connect( this, &MainWindow::menu_callback );
	
	tools_menu->add_item("Audio && MIDI",TOOL_AUDIO_SETTINGS);
	tools_menu->add_item("Keyboard",TOOL_KEYBOARD);
	tools_menu->add_item("Interface",TOOL_INTERFACE);
	tools_menu->add_item("Paths",TOOL_DEFAULT_PATHS);
	
	MenuBox * help_menu = hb->add( new MenuBox("Help") );	
	
	help_menu->item_selected_signal.connect( this, &MainWindow::menu_callback );
	
	help_menu->add_item("Help!",HELP_HELP);
	help_menu->add_item("About",HELP_ABOUT);
	
	main_stack = main_vbc->add( new StackContainer, 1 );
	
	graph_screen = main_stack->add(new GraphScreen(song.get_audio_graph()) );
	
	
	hb = main_vbc->add( new HBoxContainer );
	
	hb->add( new Icon( get_skin()->get_bitmap( BITMAP_ICON_INFO ) ) );
	
	info_line = hb->add( new LineEdit, 4 );
	
	hb->add( new Icon( get_skin()->get_bitmap( BITMAP_ICON_TIME ) ) );
	
	time_line = hb->add( new LineEdit );
	time_line->set_minimum_size( Size(150,-1));
	
	/* Pages */
	
	
//	global_view_frame =main_stack->add( new GlobalViewFrame(data.editor) );
	//edit_view_frame =main_stack->add( new EditViewFrame(update_notify,data.editor) );
	//rack_view_frame =main_stack->add( new RackViewFrame(update_notify, data.editor) );
	
	tab_bar->tab_changed_signal.connect( main_stack, &StackContainer::raise );
	
	/* UPDATE NOTIFY ASSIGN */
	//update_notify->notify_action_signal.connect(this, &MainWindow::notify_action_slot);
	//update_notify->track_list_changed_signal.connect(this,&MainWindow::rebuild_track_lists);
	
	//edit_view_frame->show_track_dialog_signal.connect( track_dialog, &TrackDialog::show );
	
//	create_keybindings();	
		
	new_track_dialog = new NewTrackDialog(this);
	audio_settings_dialog = new AudioSettingsDialog(this,&update_notify);
	quit=false;
}


MainWindow::~MainWindow()
{
}


