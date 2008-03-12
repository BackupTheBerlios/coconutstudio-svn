


#ifndef AUDIO_NODE_H
#define AUDIO_NODE_H

#include "rstring.h"
#include "engine/music_event.h"

struct AudioNodeConfig {

	int audio_buffer_size;
	int event_buffer_size;	
};

class ControlPort {

	bool visible;
public:

	enum Hint {
		
		HINT_RANGE, ///< just a range (Default)
		HINT_TOGGLE, ///< valid values 0.0f , 1.0f
		HINT_ENUM, ///< asking integer values to get_Value_as_text will return them
	};	

	virtual String get_name() const =0;

	virtual float get_min() const=0;
	virtual float get_max() const=0;
	virtual float get_step() const=0;
	virtual float get_default() const=0;
	virtual float get() const=0;
	
	virtual void set(float p_val,bool p_make_default=false)=0; //set, optionally make the value the default too
	virtual void set_normalized(float p_val); // set in range 0-1, internally converted to range
	virtual float get_normalized() const;
	
	virtual String get_value_as_text(float p_value) const;
	virtual Hint get_hint() const;
	
	virtual void set_visible(bool p_visible);
	virtual bool is_visible() const;
		
	ControlPort();
	virtual ~ControlPort();
};


class AudioNode {

	int _x,_y; // coordinates in graph
	int _layer; // layer in graph, <0 means all layers

	const AudioNodeConfig* _node_config;
public:
	
	/* Audio Port API */
	
	enum PortType {
		
		PORT_AUDIO,
		PORT_EVENT,
		PORT_CONTROL
	};
	
	virtual int get_in_audio_port_count() const=0;
	virtual int get_out_audio_port_count() const=0;
	
	virtual int get_in_audio_port_channel_count(int p_port) const=0;
	virtual int get_out_audio_port_channel_count(int p_port) const=0;
		
	virtual void connect_in_audio_port_channel_buffer(int p_port,int p_channel,float*)=0;
	virtual void connect_out_audio_port_channel_buffer(int p_port,int p_channel,float*)=0;
	
	/* Control Port API */
		
	virtual int get_control_port_count() const=0;
	virtual ControlPort* get_control_port(int p_port)=0;
		
	/* Event port API */
	
	virtual int get_in_event_port_count() const=0;
	virtual int get_out_event_port_count() const=0;
		
	virtual void connect_in_event_port_buffer(int p_port,MusicEvent*)=0;
	virtual void connect_out_event_port_buffer(int p_port,MusicEvent*)=0;
	
	/* Config */
	
	virtual void set_node_config(const AudioNodeConfig* p_config);
	inline const AudioNodeConfig* get_node_config() const { return _node_config; }
	
	/* AudioNode Info */
	
	virtual String get_name() const=0;
	
	/* Process */
	
	virtual void process()=0;
	virtual void set_sampling_rate(float p_hz)=0;	
	
	/* Graph Positioning and Layering */
	
	void set_x(int p_x);
	int get_x() const;
	void set_y(int p_y);
	int get_y() const;
	void set_layer(int p_layer);
	int get_layer() const;
		
	AudioNode();
	virtual ~AudioNode();
	
};


#endif
