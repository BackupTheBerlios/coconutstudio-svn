//
// C++ Implementation: audio_settings_dialog
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "audio_settings_dialog.h"
#include "containers/center_container.h"
#include "bundles/window_box.h"
#include "bundles/margin_group.h"
#include "engine/audio_driver_manager.h"
#include "widgets/separator.h"
#include "gui_common/common_skin.h"

struct AudioNodeAddDialog : public Window {

	LineEdit *name;
	ComboBox *type_combo;
	ComboBox *channels_combo;
	MarginGroup *channels_mg;
	bool typed;
	
	Button *add;
	
	void selected(int p_which) {
	
		if (p_which<2)
			channels_mg->show();
		else
			channels_mg->hide();
			
		if (!typed) {
		
			autoset_name();
			
		}
		
	}

	void autoset_name() {
	
		String proposed_name;
		
		switch(type_combo->get_selected()) {
			case 0: proposed_name="Audio Out"; break;
			case 1: proposed_name="Audio In"; break;
			case 2: proposed_name="Event Out"; break;
			case 3: proposed_name="Event In"; break;	
		}
		
		int index=1;
		while(true) {
		
			AudioDriver *ad=AudioDriverManager::get_driver( AudioDriverManager::get_current() );
			ERR_FAIL_COND(!ad);
			bool name_exists=false;
			String try_name=(index==1)?proposed_name:(proposed_name+" "+String::num(index));
			
			for (int i=0;i<ad->get_node_count();i++) {
			
				if (ad->get_node(i)->get_name()==try_name) {
								
					name_exists=true;
					break;
				}
			}
		
			if (name_exists) {
			
				index++;
				continue;
			}
			proposed_name=try_name;
			break;
		}
		
		name->set_text(proposed_name);
	
	}

	void typed_cbk(String) {
	
		typed=true;
	}
	
	void show() {
	
		typed=false;
		autoset_name();
		Window::show();
		name->get_focus();
		name->select_all();
	}

	AudioNodeAddDialog(Window *w) : Window(w,Window::MODE_POPUP,Window::SIZE_CENTER ) {
	
		WindowBox *wb = new WindowBox("Add New Node");
		set_root_frame(wb);
		
		MarginGroup *mg = wb->add( new MarginGroup("Name:") );
		name = mg->add( new LineEdit );
		name->text_changed_signal.connect(this,&AudioNodeAddDialog::typed_cbk);
		
		mg = wb->add( new MarginGroup("Type:                    ") );
		type_combo = mg->add( new ComboBox );
		
		type_combo->add_string("Audio OUT");
		type_combo->add_string("Audio IN");
		type_combo->add_string("Event OUT");
		type_combo->add_string("Event IN");
		type_combo->selected_signal.connect(this,&AudioNodeAddDialog::selected);
		
		channels_mg = wb->add( new MarginGroup("Channels:") );
		channels_combo = channels_mg->add( new ComboBox );
		channels_combo->add_string("Mono");
		channels_combo->add_string("Stereo");
		channels_combo->add_string("3");
		channels_combo->add_string("Quad");
		channels_combo->add_string("4");
		channels_combo->add_string("5");
		channels_combo->add_string("6");
		channels_combo->add_string("7");
		channels_combo->add_string("8");
		channels_combo->select(1);
		
		add = wb->add( new CenterContainer )->set( new Button("Add") );
	}
	
};

void AudioSettingsDialog::node_add() {

	
	AudioDriver *ad=AudioDriverManager::get_driver( AudioDriverManager::get_current() );
	ERR_FAIL_COND(!ad);

	AudioDriver::NodeType nt;
	int chans;
	bool input;
	
	switch(node_add_dialog->type_combo->get_selected()) {
	
		case 0: nt = AudioDriver::TYPE_AUDIO; input=false; chans=node_add_dialog->channels_combo->get_selected()+1; break;
		case 1: nt = AudioDriver::TYPE_AUDIO; input=true; chans=node_add_dialog->channels_combo->get_selected()+1; break;
		case 2: nt = AudioDriver::TYPE_EVENT; input=false; break;
		case 3: nt = AudioDriver::TYPE_EVENT; input=true;  break;
	}

	EditCommands::get_singleton()->audio_driver_add_node(
		AudioDriverManager::get_audio_graph(), 
		ad,		
		nt,
		chans,
		input
	);
	
	EditCommands::get_singleton()->audio_driver_set_node_name(ad,ad->get_node_count()-1,node_add_dialog->name->get_text());
	
	node_add_dialog->hide();
	update_node_tree();
}

void AudioSettingsDialog::node_add_show_dialog() {

	node_add_dialog->show();
}


void AudioSettingsDialog::node_selected(int p_column,int p_row) {

	selected_node=p_row;
}

void AudioSettingsDialog::update_node_tree() {

	nodes->clear();
	
	AudioDriver *ad=AudioDriverManager::get_driver( AudioDriverManager::get_current() );
	ERR_FAIL_COND(!ad);
	
	TreeItem * root = nodes->create_item(NULL);
	root->set_text(0, ad->get_name() );
	root->set_text(1, "  Type:" );
	root->set_text(2, "  Connection:" );
	root->set_text(3, "  Mapper:" );
	
	for(int i=0;i<ad->get_node_count();i++) {
			
		TreeItem *item = nodes->create_item(root);
		item->set_text(0, ad->get_node(i)->get_name() );
		item->set_bitmap(0, ad->is_node_active(i)?get_skin()->get_bitmap( BITMAP_ICON_ACTIVE ) : get_skin()->get_bitmap( BITMAP_ICON_INACTIVE ));
		
		
		String type;
		switch(ad->get_node_type(i)) {
		 
			case AudioDriver::TYPE_AUDIO: {
			 

			 	if (ad->get_node(i)->get_in_audio_port_count()) {
			 		type="AUDIO-IN ("+String(get_chan_name(ad->get_node(i)->get_in_audio_port_channel_count(0)))+")";
			 	} else {
			 		type="AUDIO-OUT ("+String(get_chan_name(ad->get_node(i)->get_out_audio_port_channel_count(0)))+")";
			 	}
			} break;
			case AudioDriver::TYPE_EVENT: {
			 
			 	if (ad->get_node(i)->get_in_event_port_count()) {
				 	type="EVENT-IN";	
				 } else {
				 	type="EVENT-OUT";	
				 }
			} break;
		}
		
		
		item->set_text(1, type );
		item->set_selectable(0,true);	
		item->set_selectable(1,false);	
		item->set_selectable(2,false);	
		item->set_cell_mode( 2, CELL_MODE_CUSTOM );
		item->set_editable( 2, true );
		item->set_text(2, ad->get_node_external_connection(i));
		item->set_selectable(3,false);	
		
		if (selected_node==i)
			item->select(0);
		item->selected_signal.connect( Method1<int>( Method2<int,int>( this, &AudioSettingsDialog::node_selected ), i) );
		item->custom_popup_signal.connect( Method2<int,Rect>( Method3<int,Rect,int>( this, &AudioSettingsDialog::node_request_edit), i) );
	}

}

void AudioSettingsDialog::node_connect(int p_node, String p_to) {

	AudioDriver *ad=AudioDriverManager::get_driver( AudioDriverManager::get_current() );
	ERR_FAIL_COND(!ad);
	ad->connect_node_to_external(p_node,p_to);
	update_node_tree();
	
}


void AudioSettingsDialog::node_request_edit(int, Rect p_rect, int p_node) {

	AudioDriver *ad=AudioDriverManager::get_driver( AudioDriverManager::get_current() );
	ERR_FAIL_COND(!ad);

	connect_node_popup->clear();
	connect_node_popup->add_item("Disconnect");
	connect_node_popup->add_separator();
							
	std::list<String> connectable = ad->get_connectable_external_list( ad->get_node_type(p_node), ad->is_node_input(p_node), ad->get_node_channels(p_node) );
	
	std::list<String>::iterator I = connectable.begin();
	
	for (;I!=connectable.end();I++) {
		// using shitty syntax because of gcc 3 bug
		Method2<int,String> m2=Method2<int,String>(this,&AudioSettingsDialog::node_connect);
		Method1<int> m1=Method1<int>( Method2<int,String>(this,&AudioSettingsDialog::node_connect), *I);
		Method method( m1 , p_node );
		
		connect_node_popup->add_item(*I,method);
		
	}
	
	connect_node_popup->set_pos( get_global_pos()+p_rect.pos/*+Point(0,p_rect.size.y)*/ );
	connect_node_popup->set_size( Size( p_rect.size.x, 0 ) );
	connect_node_popup->popup();

}

void AudioSettingsDialog::init_default() {

	AudioDriverManager::set_current(0);
	if (AudioDriverManager::get_driver(0)) {
	
		AudioDriverManager::get_driver(0)->init();
	}

	update_all();
}


void AudioSettingsDialog::update_all() {

	updating=true;

	driver_combo->clear();
	for (int i=0;i<AudioDriverManager::get_driver_count();i++) {
	
		driver_combo->add_string( AudioDriverManager::get_driver(i)->get_name() );
	}
	
	driver_combo->select( AudioDriverManager::get_current() );
	update_node_tree();
	 
	AudioDriver *ad=AudioDriverManager::get_driver( AudioDriverManager::get_current() );
	
	if (ad && ad->is_active()) {
	
		driver_status->set_text("Sound Driver Enabled.");
	} else if (ad) {
	
		driver_status->set_text("Disabled: "+ad->get_last_error());
	
	}
	updating=false;
	
}

void AudioSettingsDialog::driver_disable() {

	AudioDriver *ad=AudioDriverManager::get_driver( AudioDriverManager::get_current() );
	ERR_FAIL_COND(!ad);
	ad->finish();
	update_all();

}

void AudioSettingsDialog::driver_restart() {

	AudioDriver *ad=AudioDriverManager::get_driver( AudioDriverManager::get_current() );
	ERR_FAIL_COND(!ad);
	if (ad->is_active())
		ad->finish();
	ad->init();
	update_all();
}


AudioSettingsDialog::AudioSettingsDialog(Window* p_parent,GUI_UpdateNotify *p_update_notify) : Window(p_parent,Window::MODE_POPUP,Window::SIZE_TOPLEVEL_CENTER) {

	WindowBox *wb = new WindowBox("Audio & MIDI Settings");
	set_root_frame(wb);
	MarginGroup *mg = wb->add( new MarginGroup("Driver:"),1 );
	HBoxContainer *top_hb = mg->add( new HBoxContainer );
	driver_combo = top_hb->add( new ComboBox, 1);
	Button *restart = top_hb->add( new Button("Restart") );
	restart->pressed_signal.connect( this, &AudioSettingsDialog::driver_restart);
	Button *stop = top_hb->add( new Button("Disable") );
	stop->pressed_signal.connect( this, &AudioSettingsDialog::driver_disable);
	tab_box = mg->add( new TabBox, 1 );
	mg = wb->add( new MarginGroup("Driver Status:"));
	driver_status = mg->add( new LineEdit("Disabled") );
	
	/* Nodes */
	
	VBoxContainer *nodes_vb = tab_box->add("Nodes",new VBoxContainer); 
	ScrollBox *sb = nodes_vb->add(new ScrollBox,1);
	sb->set_expand_h(true);
	sb->set_expand_v(true);
	sb->set_scroll_v(true);
	HBoxContainer *nodes_hb = nodes_vb->add( new HBoxContainer );
	Button* add_node;
	add_node = nodes_hb->add( new CenterContainer,1 )->set( new Button("Add") );
	add_node->pressed_signal.connect(this,&AudioSettingsDialog::node_add_show_dialog);
	Button* remove_node = nodes_hb->add( new CenterContainer,1 )->set( new Button("Remove") );
	nodes = sb->set( new Tree(4) );
	
	/* MIDI Map */
	
	VBoxContainer *mmap_vb = tab_box->add("MIDI Map",new VBoxContainer); 
	
	/* Driver Settings */
	
	VBoxContainer *driver_vb = tab_box->add("Driver Settings",new VBoxContainer); 
	
	node_add_dialog = new AudioNodeAddDialog(get_root());
	node_add_dialog->add->pressed_signal.connect(this,&AudioSettingsDialog::node_add);
	selected_node=-1;
	
	connect_node_popup = new PopUpMenu(get_root());

	updating=false;
	
	p_update_notify->sound_driver_changed_signal.connect(this, &AudioSettingsDialog::update_all);

}


AudioSettingsDialog::~AudioSettingsDialog()
{
}


