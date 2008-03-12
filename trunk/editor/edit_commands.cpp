//
// C++ Implementation: edit_commands
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "edit_commands.h"
#include "editor/update_notify.h"


#define GRAPH_ADD_OFS 15

int EditCommands::x_add_ofs=10;
int EditCommands::y_add_ofs=10;


EditCommands *EditCommands::singleton=NULL;


EditCommands *EditCommands::get_singleton() {

	return singleton;

}

/****************************/
/**** AUDIO GRAPH FUNCS *****/
/****************************/

void EditCommands::audio_graph_clear_all_node_connections(AudioGraph *p_graph,int p_node_idx) {

	std::list<AudioConnection> connections;
	
	for (int i=0;i<p_graph->get_connection_count();i++) {
	
		AudioConnection c=*p_graph->get_connection(i);
		
		if (c.from_node==p_node_idx || c.to_node==p_node_idx) {
		
			connections.push_back(c);
		}
	}
	
	begin_group("Audio Graph - Clear Node Connections");
	
	for(std::list<AudioConnection>::iterator I=connections.begin();I!=connections.end();I++) {
	
		audio_graph_disconnect(p_graph,*I);
	}
	
	end_group();

}

void EditCommands::_audio_graph_connect_helper(AudioGraph *p_graph,AudioConnection p_connection) {

	p_graph->connect( p_connection );

}
void EditCommands::_audio_graph_disconnect_helper(AudioGraph *p_graph,AudioConnection p_connection) {

	p_graph->disconnect( p_connection );

}

void EditCommands::audio_graph_disconnect(AudioGraph *p_graph,const AudioConnection& p_connection) {

	ERR_FAIL_COND(!p_graph->has_connection(p_connection));
	
	AudioConnection c=p_connection;
	
	CommandBase *cmd_do=command( this, &EditCommands::_audio_graph_disconnect_helper, p_graph,c ); 
	CommandBase *cmd_undo=command( this, &EditCommands::_audio_graph_connect_helper, p_graph,c ); 

	add_action( "Audio Graph - Disconnect",cmd_do, cmd_undo );

}
void EditCommands::audio_graph_connect(AudioGraph *p_graph,const AudioConnection& p_connection) {


	ERR_FAIL_COND(!p_graph->can_connect(p_connection));
	
	AudioConnection c=p_connection;
	
	CommandBase *cmd_do=command( this, &EditCommands::_audio_graph_connect_helper, p_graph,c ); 
	CommandBase *cmd_undo=command( this, &EditCommands::_audio_graph_disconnect_helper, p_graph,c ); 

	add_action( "Audio Graph - Connect",cmd_do, cmd_undo );

}

void EditCommands::audio_graph_move_node(AudioGraph *p_graph,int p_node_idx,int p_x,int p_y) {

	ERR_FAIL_INDEX(p_node_idx,p_graph->get_node_count());
	
	AudioNode *n = p_graph->get_node(p_node_idx);
	ERR_FAIL_COND(!n);

	CommandBase *cmd_do=command( this, &EditCommands::_audio_graph_move_node, p_graph, p_node_idx, p_x,p_y ); 
	CommandBase *cmd_undo=command( this, &EditCommands::_audio_graph_move_node, p_graph, p_node_idx, n->get_x(),n->get_y() ); 

	add_action( "Audio Graph - Move Node",cmd_do, cmd_undo );

}

void EditCommands::_audio_graph_move_node(AudioGraph *p_graph,int p_node_idx,int p_x,int p_y) {

	ERR_FAIL_INDEX(p_node_idx,p_graph->get_node_count());
	
	AudioNode *n = p_graph->get_node(p_node_idx);
	ERR_FAIL_COND(!n);
	
	n->set_x(p_x);
	n->set_y(p_y);

	UpdateNotify::get_singleton()->audio_graph_changed();

}

void EditCommands::audio_graph_swap_nodes(AudioGraph *p_graph,int p_node_idx,int p_with_node_idx) {

	ERR_FAIL_INDEX(p_node_idx,p_graph->get_node_count());
	ERR_FAIL_INDEX(p_with_node_idx,p_graph->get_node_count());
	if (p_node_idx==p_with_node_idx)
		return;

	CommandBase *cmd_do=command(this,&EditCommands::_audio_graph_swap_nodes,p_graph,p_node_idx,p_with_node_idx ); 
	CommandBase *cmd_undo=command(this,&EditCommands::_audio_graph_swap_nodes,p_graph,p_with_node_idx,p_node_idx ); 

	add_action( "Audio Graph - Swap Nodes",cmd_do, cmd_undo );

}

void EditCommands::_audio_graph_swap_nodes(AudioGraph *p_graph,int p_node_idx,int p_with_node_idx) {

	p_graph->swap_node_indices(p_node_idx,p_with_node_idx);
	UpdateNotify::get_singleton()->audio_graph_changed();
	
}

/****************************/
/**** AUDIO DRIVER PORT *****/
/****************************/

void EditCommands::_audio_driver_add_node(AudioGraph *p_graph,AudioDriver *p_driver,AudioDriverPortCreateData p_create_data) {

	ERR_FAIL_INDEX(p_create_data.index,p_driver->get_node_count()+1);


	if (p_create_data.type==AudioDriver::TYPE_AUDIO) {
	
		if (p_create_data.input)
			p_driver->add_audio_input(p_create_data.chans,p_create_data.index);
		else
			p_driver->add_audio_output(p_create_data.chans,p_create_data.index);
	} else {
	
		if (p_create_data.input)
			p_driver->add_event_input(p_create_data.index);
		else
			p_driver->add_event_output(p_create_data.index);
	}

	p_graph->add_node( p_driver->get_node( p_create_data.index ), p_create_data.graph_index );
	
	UpdateNotify::get_singleton()->sound_driver_changed();
	
}

void EditCommands::_audio_driver_remove_node(AudioGraph *p_graph,AudioDriver *p_driver,int p_at_index) {

	ERR_FAIL_INDEX(p_at_index,p_driver->get_node_count());
	
	AudioNode *n = p_driver->get_node(p_at_index);
	ERR_FAIL_COND(!n);

	int graph_idx = p_graph->get_node_index(n);

	ERR_FAIL_COND( graph_idx < 0 );
	
	p_graph->erase_node(graph_idx);
	p_driver->erase_node(p_at_index);
	
	UpdateNotify::get_singleton()->sound_driver_changed();
	
}

void EditCommands::audio_driver_add_node(AudioGraph *p_graph,AudioDriver *p_driver,AudioDriver::NodeType p_type, int p_chans,bool p_input,int p_at_index) {

	if (p_at_index<0)
		p_at_index=p_driver->get_node_count();
	if (p_at_index>p_driver->get_node_count())
		p_at_index=p_driver->get_node_count();
		
	AudioDriverPortCreateData pcd;
	pcd.chans=p_chans;
	pcd.type=p_type;
	pcd.input=p_input;
	pcd.index=p_at_index;
	pcd.graph_index=-1;
		
	CommandBase *cmd_do=command( this, &EditCommands::_audio_driver_add_node, p_graph, p_driver, pcd ); 
	CommandBase *cmd_undo=command( this, &EditCommands::_audio_driver_remove_node, p_graph, p_driver, p_at_index ); 

	begin_group("Sound Driver - Add New Node");


	add_action( "Sound Driver - Add Port",cmd_do, cmd_undo );
	
	printf("Adding at %i,%i\n",x_add_ofs,y_add_ofs);
	audio_graph_move_node(p_graph,p_at_index,x_add_ofs,y_add_ofs);
	x_add_ofs+=GRAPH_ADD_OFS;
	y_add_ofs+=GRAPH_ADD_OFS;
	
	end_group();
	
}

void EditCommands::audio_driver_remove_node(AudioGraph *p_graph,AudioDriver *p_driver,int p_index) {

	ERR_FAIL_INDEX(p_index,p_driver->get_node_count());
	
	AudioNode *n = p_driver->get_node(p_index);
	ERR_FAIL_COND(!n);
	
	AudioDriverPortCreateData pcd;
		
	if (n->get_in_audio_port_count()) {
	
		pcd.chans=n->get_in_audio_port_channel_count(0);
		pcd.input=true;
		pcd.type=AudioDriver::TYPE_AUDIO;
	} else if (n->get_out_audio_port_count()) {
	
		pcd.chans=n->get_out_audio_port_channel_count(0);
		pcd.input=false;
		pcd.type=AudioDriver::TYPE_AUDIO;
	} if (n->get_in_event_port_count()) {
	
		pcd.chans=1;
		pcd.input=true;
		pcd.type=AudioDriver::TYPE_EVENT;
	} else if (n->get_out_event_port_count()) {
	
		pcd.chans=1;
		pcd.input=false;
		pcd.type=AudioDriver::TYPE_EVENT;
	} else {
	
		ERR_PRINT("BUG");
		return;
	}

	pcd.index=p_index;
	pcd.graph_index = p_graph->get_node_index(n);
	ERR_FAIL_INDEX( pcd.graph_index, p_graph->get_node_count() );
	

	begin_group("Sound Driver - Remove Port");

	/* Remove connections to it */
	
	audio_graph_clear_all_node_connections(p_graph,pcd.graph_index);
	
	/* Remove node */
	

	CommandBase *cmd_do=command( this, &EditCommands::_audio_driver_remove_node, p_graph, p_driver, p_index ); 
	CommandBase *cmd_undo=command( this, &EditCommands::_audio_driver_add_node, p_graph, p_driver, pcd ); 

	add_action( "Sound Driver - Remove Port",cmd_do, cmd_undo );
	
	end_group();

}



void EditCommands::_audio_driver_set_node_name(AudioDriver *p_driver,int p_node,String p_name) {

	ERR_FAIL_INDEX(p_node,p_driver->get_node_count());
	
	p_driver->set_node_name(p_node,p_name);

	UpdateNotify::get_singleton()->sound_driver_changed();

}

void EditCommands::audio_driver_set_node_name(AudioDriver *p_driver,int p_node,String p_name) {

	ERR_FAIL_INDEX(p_node,p_driver->get_node_count());
	
	printf("setting name to %i - %ls\n",p_node,p_name.c_str());
	String name = p_driver->get_node(p_node)->get_name();
	
	CommandBase *cmd_do=command( this, &EditCommands::_audio_driver_set_node_name, p_driver, p_node,p_name ); 
	CommandBase *cmd_undo=command( this, &EditCommands::_audio_driver_set_node_name, p_driver, p_node,name ); 
	
	add_action("Sound Driver - Change Port Name",cmd_do,cmd_undo);
}

EditCommands::EditCommands() {

	ERR_FAIL_COND(singleton);
	singleton=this;
}


EditCommands::~EditCommands()
{
}


