#ifndef AUDIO_GRAPH_PROCESS_H
#define AUDIO_GRAPH_PROCESS_H

#include <vector>
#include "engine/audio_node.h"
#include "engine/music_event.h"
#include "engine/audio_connection.h"
#include "error_macros.h"

/**
 * This is an extension to AudioGraph that does the actual processing.
 * Only AudioGraph can use it, and onl
 */

class AudioGraphProcess {

	enum {
		
		AUDIO_BUFFER_SIZE=512, /* Internal buffer size */
		EVENT_BUFFER_SIZE=256
	};
	
	
	struct AudioBuffer {
	
		int chans;
		std::vector< std::vector<float> > buffer; // buffer for each chan
		
		void create(int p_chans) {
		
			buffer.resize(p_chans);
			for (int i=0;i<p_chans;i++)
				buffer[i].resize( AUDIO_BUFFER_SIZE );
				
			chans=p_chans;
		}
		
		void connect_in(AudioNode *p_node,int p_port) {
		
			ERR_FAIL_COND( p_node->get_in_audio_port_count() != chans );
			ERR_FAIL_INDEX( p_port, p_node->get_in_audio_port_count() );
			for (int i=0;i<chans;i++) {
			
				p_node->connect_in_audio_port_channel_buffer( p_port,i, &buffer[i][0] );
			}
		}
		
		void connect_out(AudioNode *p_node,int p_port) {
		
			ERR_FAIL_COND( p_node->get_out_audio_port_count() != chans );
			ERR_FAIL_INDEX( p_port, p_node->get_out_audio_port_count() );
			for (int i=0;i<chans;i++) {
			
				p_node->connect_out_audio_port_channel_buffer( p_port,i, &buffer[i][0] );
			}
		}
		
		inline void clear(int p_frames=AUDIO_BUFFER_SIZE) {
		
			ERR_FAIL_INDEX(p_frames,AUDIO_BUFFER_SIZE);
			
			
			for (int i=0;i<chans;i++) {
			
				float *buf=&buffer[i][0];
				for (int j=0;j<p_frames;j++)
					buf[j]=0;
			}
		
		}
		
		inline void add_from(AudioBuffer *p_src,int p_frames=AUDIO_BUFFER_SIZE) {
		
			ERR_FAIL_INDEX(p_frames,AUDIO_BUFFER_SIZE);
			ERR_FAIL_COND(chans!=p_src->chans);
			
			
			for (int i=0;i<chans;i++) {
			
				float *dst_buf=&buffer[i][0];
				float *src_buf=&p_src->buffer[i][0];
				
				for (int j=0;j<p_frames;j++)
					dst_buf[j]+=src_buf[j];
			}
		
		}
		
		AudioBuffer() { chans=0; }
		
	};
	
	
	typedef std::vector<MusicEvent> EventBuffer;
	
	struct ProcessNode {
		/* Audio Node Being Processed */
		AudioNode *node;
	
	
		/* Buffer for each input/output. Empty if not connected */
		
		// Audio
		std::vector< AudioBuffer* > output_audio_buffers; /* Buffer for each Output */
		std::vector< AudioBuffer* > input_audio_buffers; /* Buffer for each Input */
		std::vector< std::vector< AudioBuffer*> > input_audio_sources;
		
		// Event
		std::vector<EventBuffer*> output_event_buffers; /* Buffer for each Output */
		std::vector<EventBuffer*> input_event_buffers; /* Buffer for each Input */
		std::vector< std::vector<EventBuffer*> > input_event_sources;
		
		// Control
		
		struct ControlConnect {
		
			int node_from;
			int port_from;
			int port_to;
		};
		
		std::vector<ControlConnect> control_connections;
	};


	AudioNodeConfig audio_config;
	
	std::vector<ProcessNode*> process_nodes;
	std::vector<float> audio_trash_buffer;
	std::vector<float> audio_silence_buffer;
	
	EventBuffer event_trash_buffer;
	EventBuffer event_silence_buffer;

	bool unconfigured;	
	
friend class AudioGraph;	
	
	void clear();
	/* Add nodes first, in order */
	void add_node(AudioNode *p_node);
	/* Then Add Connections */
	void add_connection(const AudioConnection& p_connection);

	/* reconnect everything, creates internal buffers and everything */
	void configure_connections(); 
		
	/* process */
	int process(int p_frames);
	
	AudioGraphProcess();
	~AudioGraphProcess();
};




#endif
