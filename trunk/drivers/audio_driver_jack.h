//
// C++ Interface: audio_driver_jack
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef AUDIO_DRIVER_JACK_H
#define AUDIO_DRIVER_JACK_H

#include "engine/audio_driver.h"
#include <vector>
#include <jack/jack.h>
/**
	@author Juan Linietsky <reduzio@gmail.com>
*/
class AudioDriverJACK : public AudioDriver {

	struct JackNode : public AudioNode {
	
		String name;
		NodeType type;
		bool input;
		int chans;
		std::vector<jack_port_t*> jack_ports;
		std::vector<float*> audio_buffers;
		MusicEvent* event_buffer;
		
		/* Audio Port API */
		
		virtual int get_in_audio_port_count() const;
		virtual int get_out_audio_port_count() const;
		
		virtual int get_in_audio_port_channel_count(int p_port) const;
		virtual int get_out_audio_port_channel_count(int p_port) const;
			
		virtual void connect_in_audio_port_channel_buffer(int p_port,int p_channel,float*);
		virtual void connect_out_audio_port_channel_buffer(int p_port,int p_channel,float*);
		
		/* Control Port API */
				
		
		virtual int get_control_port_count() const;
		virtual float get_control_port(int p_port) const;
		virtual void set_control_port(int p_port,float p_value);
		
		virtual ControlPortInfo get_control_port_info(int p_port) const;
		virtual String get_control_port_text_for_value(int p_port,float p_value);
			
		/* Event port API */
		
		virtual int get_in_event_port_count() const;
		virtual int get_out_event_port_count() const;
			
		virtual void connect_in_event_port_buffer(int p_port,MusicEvent*);
		virtual void connect_out_event_port_buffer(int p_port,MusicEvent*);
		
		/* Config */
		
		
		/* AudioNode Info */
		
		virtual String get_node_name() const;
		
		/* Process */
		
		virtual void process();
		
	};

	int hw_audio_in_count;
	int hw_audio_out_count;
	int hw_event_in_count;
	int hw_event_out_count;

	bool active;
	
	float srate;
	
	std::vector<JackNode*> ports;
	
	jack_client_t *client;

	String error_text;
	
	static int _process(jack_nframes_t nframes, void *arg) { 
		AudioDriverJACK *adj=(AudioDriverJACK*)arg; return adj->process(nframes);
	}
	static int _srate(jack_nframes_t nframes, void *arg) { 
		AudioDriverJACK *adj=(AudioDriverJACK*)arg; return adj->set_sampling_rate(nframes);
	}
	static void _shutdown(void *arg) { 
		AudioDriverJACK *adj=(AudioDriverJACK*)arg; return adj->finish();
	}
		
	int process(jack_nframes_t nframes);
	int set_sampling_rate(jack_nframes_t nframes);

	void add_node(int p_chans,int p_at_index,NodeType p_type,bool p_input);

public:

	/* Input and Output Nodes */

	virtual int get_node_count() const;
	
	virtual AudioNode *get_node(int p_index);
	virtual NodeType get_node_type(int p_index) const;
	
	virtual void add_audio_input(int p_chans,int p_at_index=-1);
	virtual void add_event_input(int p_chans);
	virtual void add_audio_output(int p_chans,int p_at_index=-1);
	virtual void add_event_output(int p_chans);
	
	virtual void erase_node(int p_index);
				
	/* ports used for settings */
		
	virtual int get_port_count() const;
	virtual float get_port(int p_port) const;
	virtual void set_port(int p_port,float p_value);
	
	virtual AudioNode::ControlPortInfo get_port_info(int p_port) const;
	virtual String get_port_text_for_value(int p_port,float p_value);
	
	/* Idendification */
	
	virtual String get_name() const;
	
	
	virtual bool init(); // true of ok, false if error
	virtual bool is_active() const; 
	virtual void finish();
	
	virtual void lock();
	virtual void unlock();
			
	AudioDriverJACK();
	~AudioDriverJACK();
};

#endif