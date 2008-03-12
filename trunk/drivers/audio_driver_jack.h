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

#ifdef JACK_ENABLED

#include "engine/audio_driver.h"
#include <vector>
#include <jack/jack.h>
#include <pthread.h>
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
		String connected_to;
		
		/* Audio Port API */
		
		virtual int get_in_audio_port_count() const;
		virtual int get_out_audio_port_count() const;
		
		virtual int get_in_audio_port_channel_count(int p_port) const;
		virtual int get_out_audio_port_channel_count(int p_port) const;
			
		virtual void connect_in_audio_port_channel_buffer(int p_port,int p_channel,float*);
		virtual void connect_out_audio_port_channel_buffer(int p_port,int p_channel,float*);
		
		/* Control Port API */
				
		
		virtual int get_control_port_count() const;
		virtual ControlPort* get_control_port(int p_port);
			
		/* Event port API */
		
		virtual int get_in_event_port_count() const;
		virtual int get_out_event_port_count() const;
			
		virtual void connect_in_event_port_buffer(int p_port,MusicEvent*);
		virtual void connect_out_event_port_buffer(int p_port,MusicEvent*);
		
		/* Config */
		
		
		/* AudioNode Info */
		
		virtual String get_name() const;
		
		/* Process */
		
		virtual void process();
		virtual void set_sampling_rate(float p_hz) {}
		static int ofs_mix;		
		static int cbk_nframes;		
		
	};



	int hw_audio_in_count;
	int hw_audio_out_count;
	int hw_event_in_count;
	int hw_event_out_count;

	bool active;
	
	float srate;
	
	std::vector<JackNode*> ports;
	
	jack_client_t *client;
	pthread_mutex_t mutex;

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

	AudioGraph * audio_graph;

public:

	/* Input and Output Nodes */

	virtual int get_node_count() const;
	
	virtual AudioNode *get_node(int p_index);
	virtual NodeType get_node_type(int p_index) const;
	virtual void set_node_name(int p_index,String p_name);
	
	virtual void add_audio_input(int p_chans,int p_at_index=-1);
	virtual void add_event_input(int p_chans);
	virtual void add_audio_output(int p_chans,int p_at_index=-1);
	virtual void add_event_output(int p_chans);
	
	virtual void erase_node(int p_index);
				
	virtual bool is_node_active(int p_index) const;
				
	virtual String get_node_external_connection(int p_index) const;
	virtual void connect_node_to_external(int p_index,String p_to);
	virtual std::list<String> get_connectable_external_list(NodeType p_type,bool p_input,int p_channels) const;
				
	/* ports used for settings */
		
	virtual int get_control_port_count() const;
	virtual ControlPort* get_control_port(int p_port);
	
	/* Idendification */
	
	virtual String get_name() const;
	
	
	virtual bool init(); // true of ok, false if error
	virtual bool is_active() const; 
	virtual void finish();
	virtual String get_last_error() const;
	
	virtual void lock();
	virtual void unlock();
			
	/* Audio Graph */
	
	void set_audio_graph(AudioGraph * p_audio_graph);

			
	AudioDriverJACK();
	~AudioDriverJACK();
};

#endif
#endif