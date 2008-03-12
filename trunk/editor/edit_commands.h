//
// C++ Interface: edit_commands
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef EDIT_COMMANDS_H
#define EDIT_COMMANDS_H

#include "editor/undo_redo.h"
#include "engine/audio_driver_manager.h"
#include "engine/audio_graph.h"
/**
	@author Juan Linietsky <reduzio@gmail.com>
*/
class EditCommands : public UndoRedo {


	struct AudioDriverPortCreateData {
	
		AudioDriver::NodeType type;
		int chans;
		bool input;
		int index;
		int graph_index;
	};


	static int x_add_ofs,y_add_ofs;

	void _audio_graph_connect_helper(AudioGraph *p_graph,AudioConnection p_connection);
	void _audio_graph_disconnect_helper(AudioGraph *p_graph,AudioConnection p_connection);
	void _audio_graph_move_node(AudioGraph *p_graph,int p_node_idx,int p_x,int p_y);	
	void _audio_graph_swap_nodes(AudioGraph *p_graph,int p_node_idx,int p_with_node_idx);	
	
	void _audio_driver_add_node(AudioGraph *p_graph,AudioDriver *p_driver,AudioDriverPortCreateData p_create_data);
	void _audio_driver_remove_node(AudioGraph *p_graph,AudioDriver *p_driver,int p_at_index);
	void _audio_driver_set_node_name(AudioDriver *p_driver,int p_node,String p_name);
	
	static EditCommands *singleton;
	
public:

	static EditCommands *get_singleton();

	void audio_graph_clear_all_node_connections(AudioGraph *p_graph,int p_node_idx);
	void audio_graph_disconnect(AudioGraph *p_graph,const AudioConnection& p_connection);
	void audio_graph_connect(AudioGraph *p_graph,const AudioConnection& p_connection);	
	void audio_graph_move_node(AudioGraph *p_graph,int p_node_idx,int p_x,int p_y);	
	void audio_graph_swap_nodes(AudioGraph *p_graph,int p_node_idx,int p_with_node_idx);	

	void audio_driver_add_node(AudioGraph *p_graph,AudioDriver *p_driver,AudioDriver::NodeType p_type,int p_chans, bool p_input,int p_at_index=-1);
	void audio_driver_remove_node(AudioGraph *p_graph,AudioDriver *p_driver,int p_index);
	void audio_driver_set_node_name(AudioDriver *p_driver,int p_node,String p_name);

	EditCommands();
	~EditCommands();

};

#endif
