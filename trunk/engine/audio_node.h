


#ifndef AUDIO_NODE_H
#define AUDIO_NODE_H

#include "rstring.h"
#include "engine/music_event.h"

struct AudioNodeConfig {

	int audio_buffer_size;
	int event_buffer_size;	
};

class AudioNode {

	const AudioNodeConfig* _node_config;
public:
	
	/* Audio Port API */
	
	virtual int get_in_audio_port_count() const=0;
	virtual int get_out_audio_port_count() const=0;
	
	virtual int get_in_audio_port_channel_count(int p_port) const=0;
	virtual int get_out_audio_port_channel_count(int p_port) const=0;
		
	virtual void connect_in_audio_port_channel_buffer(int p_port,int p_channel,float*)=0;
	virtual void connect_out_audio_port_channel_buffer(int p_port,int p_channel,float*)=0;
	
	/* Control Port API */
	
	
	struct ControlPortInfo {
		
		String name; ///< control port name
		enum Hint {
			
			HINT_NONE, ///< no hints
			HINT_TOGGLE, ///< valid values 0.0f , 1.0f
   			HINT_RANGE, ///< use min,max,step
      			HINT_ENUM, ///< 0 .. max-1,
      
		};	
		Hint hint;
		float min,max,step,default_val;
		String suffix;
		bool realtime; // if port is not realtime, don't show as available or don't allow connection
		ControlPortInfo() { hint=HINT_NONE; min=max=step=default_val=0; realtime=true; }
	};
	
	virtual int get_control_port_count() const=0;
	virtual float get_control_port(int p_port) const=0;
	virtual void set_control_port(int p_port,float p_value)=0;
	
	virtual ControlPortInfo get_control_port_info(int p_port) const;
	virtual String get_control_port_text_for_value(int p_port,float p_value);
		
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
	
	AudioNode();
	virtual ~AudioNode();
	
};


#endif
