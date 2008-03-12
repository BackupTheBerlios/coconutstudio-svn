//
// C++ Interface: audio_settings_dialog
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef AUDIO_SETTINGS_DIALOG_H
#define AUDIO_SETTINGS_DIALOG_H

#include "editor/edit_commands.h"
#include "engine/audio_driver_manager.h"
#include "base/window.h"
#include "widgets/tree.h"
#include "bundles/scroll_box.h"
#include "bundles/tab_box.h"
#include "bundles/combo_box.h"
#include "bundles/popup_menu.h"
#include "gui_app/gui_update_notify.h"

using namespace GUI;

/**
	@author Juan Linietsky <reduzio@gmail.com>
*/


class AudioNodeAddDialog;

class AudioSettingsDialog : public Window { 

	ComboBox *driver_combo;
	
	TabBox *tab_box;
	
	LineEdit *driver_status;

	Tree *nodes;
	
	AudioNodeAddDialog *node_add_dialog;
	
	int selected_node;
	void update_node_tree();
	void node_tree_selected(int p_which);
		
	
	void node_selected(int p_column,int p_row);

	void node_add();
	void node_add_show_dialog();
	
	PopUpMenu *connect_node_popup;
	
	void node_request_edit(int, Rect p_rect, int p_node);
	void node_connect(int, String p_to);	
	
	bool updating;
	
	void driver_disable();
	void driver_restart();
	
public:

	void update_all();

	void init_default();

	AudioSettingsDialog(Window* p_parent,GUI_UpdateNotify *p_update_notify);
	~AudioSettingsDialog();

};

#endif
