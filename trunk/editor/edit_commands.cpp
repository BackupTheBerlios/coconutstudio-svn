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


/****************************/
/**** AUDIO DRIVER PORT *****/
/****************************/

void EditCommands::_audio_driver_add_port(AudioGraph *p_graph,AudioDriver *p_driver,AudioDriverPortCreateData p_create_data) {

	ERR_FAIL_INDEX(p_create_data.index,p_driver->get_port_count()+1);


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
	
}

void EditCommands::_audio_driver_remove_port(AudioGraph *p_graph,AudioDriver *p_driver,int p_at_index) {

	ERR_FAIL_INDEX(p_at_index,p_driver->get_node_count());
	
	AudioNode *n = p_driver->get_node(p_at_index);
	ERR_FAIL_COND(!n);

	int graph_idx = p_graph->get_node_index(n);

	ERR_FAIL_COND( graph_idx < 0 );
	
	p_graph->erase_node(graph_idx);
	p_driver->erase_node(p_at_index);
}

void EditCommands::audio_driver_add_port(AudioGraph *p_graph,AudioDriver *p_driver,AudioDriver::NodeType p_type, int p_chans,bool p_input,int p_at_index) {

	if (p_at_index<0)
		p_at_index=p_driver->get_port_count();
	if (p_at_index>p_driver->get_port_count())
		p_at_index=p_driver->get_port_count();
		
	AudioDriverPortCreateData pcd;
	pcd.chans=p_chans;
	pcd.type=p_type;
	pcd.input=p_input;
	pcd.index=p_at_index;
	pcd.graph_index=-1;
		
	CommandBase *cmd_do=command( this, &EditCommands::_audio_driver_add_port, p_graph, p_driver, pcd ); 
	CommandBase *cmd_undo=command( this, &EditCommands::_audio_driver_remove_port, p_graph, p_driver, p_at_index ); 


	add_action( "Sound Driver - Add Port",cmd_do, cmd_undo );
	
}

void EditCommands::audio_driver_remove_port(AudioGraph *p_graph,AudioDriver *p_driver,int p_index) {

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
	
	/* Remove port */
	

	CommandBase *cmd_do=command( this, &EditCommands::_audio_driver_remove_port, p_graph, p_driver, p_index ); 
	CommandBase *cmd_undo=command( this, &EditCommands::_audio_driver_add_port, p_graph, p_driver, pcd ); 

	add_action( "Sound Driver - Remove Port",cmd_do, cmd_undo );
	
	end_group();

}


EditCommands::EditCommands()
{
}


EditCommands::~EditCommands()
{
}


