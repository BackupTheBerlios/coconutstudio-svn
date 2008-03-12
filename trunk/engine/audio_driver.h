//
// C++ Interface: audio_driver
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef AUDIO_DRIVER_H
#define AUDIO_DRIVER_H

#include "engine/audio_node.h"
#include "engine/audio_graph.h"
/**
	@author Juan Linietsky <reduzio@gmail.com>
*/
class AudioDriver{
public:

	enum NodeType {
		TYPE_AUDIO,
		TYPE_EVENT,
	};

	
	virtual int get_node_count() const=0;
	
	virtual AudioNode *get_node(int p_index)=0;
	virtual NodeType get_node_type(int p_index) const=0;
	bool is_node_input(int p_index); // helper
	int get_node_channels(int p_index); // helper
	virtual void set_node_name(int p_index,String p_name) =0;
	
	virtual void add_audio_input(int p_chans,int p_at_index=-1)=0;
	virtual void add_event_input(int p_at_index=-1)=0;
	virtual void add_audio_output(int p_chans,int p_at_index=-1)=0;
	virtual void add_event_output(int p_at_index=-1)=0;
	
	virtual void erase_node(int p_index)=0;
	virtual bool is_node_active(int p_index) const=0;
				
	virtual String get_node_external_connection(int p_index) const=0;
	virtual void connect_node_to_external(int p_index,String p_to)=0;
	virtual std::list<String> get_connectable_external_list(NodeType p_type,bool p_input,int p_channels) const=0;
				
	/* ports used for settings */
		
	virtual int get_control_port_count() const=0;
	virtual ControlPort* get_control_port(int p_port)=0;
			
	/* Idendification */
	
	virtual String get_name() const=0;
	
	
	virtual bool init()=0; // true of ok, false if error
	virtual bool is_active() const=0; 
	virtual void finish()=0;
	virtual String get_last_error() const=0;
	
	virtual void lock()=0;
	virtual void unlock()=0;

	virtual void set_audio_graph(AudioGraph * p_audio_graph)=0;
		
	AudioDriver();
	virtual ~AudioDriver();

};

#endif
