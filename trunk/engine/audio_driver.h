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
	
	virtual void add_audio_input(int p_chans,int p_at_index=-1)=0;
	virtual void add_event_input(int p_at_index=-1)=0;
	virtual void add_audio_output(int p_chans,int p_at_index=-1)=0;
	virtual void add_event_output(int p_at_index=-1)=0;
	
	virtual void erase_node(int p_index)=0;
				
	/* ports used for settings */
		
	virtual int get_port_count() const=0;
	virtual float get_port(int p_port) const=0;
	virtual void set_port(int p_port,float p_value)=0;
	
	virtual AudioNode::ControlPortInfo get_port_info(int p_port) const=0;
	virtual String get_port_text_for_value(int p_port,float p_value)=0;
	
	/* Idendification */
	
	virtual String get_name() const=0;
	
	
	virtual bool init()=0; // true of ok, false if error
	virtual bool is_active() const=0; 
	virtual void finish()=0;
	
	virtual void lock()=0;
	virtual void unlock()=0;

			
		
	AudioDriver();
	virtual ~AudioDriver();

};

#endif
