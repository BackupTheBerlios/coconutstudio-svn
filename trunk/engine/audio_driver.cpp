//
// C++ Implementation: audio_driver
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "audio_driver.h"

bool AudioDriver::is_node_input(int p_index)  {

	ERR_FAIL_INDEX_V(p_index,get_node_count(),false);
	
	AudioNode *an=get_node(p_index);
	ERR_FAIL_COND_V(!an,false);
	
	switch( get_node_type(p_index) ) {
	
		case TYPE_AUDIO: return an->get_out_audio_port_count(); break;
		case TYPE_EVENT: return an->get_out_event_port_count();  break;

	}

	return false;
}

int AudioDriver::get_node_channels(int p_index)  {

	ERR_FAIL_INDEX_V(p_index,get_node_count(),-1);
	
	AudioNode *an=get_node(p_index);
	ERR_FAIL_COND_V(!an,-1);
		
	switch( get_node_type(p_index) ) {
	
		case TYPE_AUDIO: 

			return is_node_input(p_index)?an->get_in_audio_port_channel_count(p_index):an->get_out_audio_port_channel_count(p_index); 
		break;
		case TYPE_EVENT: return 1;  break;

	}

	return -1;

}

AudioDriver::AudioDriver()
{
}


AudioDriver::~AudioDriver()
{
}


