

#include "audio_graph_process.h"


/**
 * Possible optimization: move unused input buffers to silence buffers 
 * Use pointers to array instead of vector[] ?
 */
void AudioGraphProcess::clear() {


	for (int i=0;i<(int)process_nodes.size();i++) {
		
		for (int j=0;j<(int)process_nodes[i]->output_audio_buffers.size();j++) {
		
			if (process_nodes[i]->output_audio_buffers[j])
				delete process_nodes[i]->output_audio_buffers[j];
		}
		
		for (int j=0;j<(int)process_nodes[i]->input_audio_buffers.size();j++) {
		
			if (process_nodes[i]->input_audio_buffers[j])
				delete process_nodes[i]->input_audio_buffers[j];
		}
		
		for (int j=0;j<(int)process_nodes[i]->output_event_buffers.size();j++) {
		
			if (process_nodes[i]->output_event_buffers[j])
				delete process_nodes[i]->output_event_buffers[j];
		}
		
		for (int j=0;j<(int)process_nodes[i]->input_event_buffers.size();j++) {
		
			if (process_nodes[i]->input_event_buffers[j])
				delete process_nodes[i]->input_event_buffers[j];
		}
		
		
		delete process_nodes[i];		
	}
	
	process_nodes.clear();
	
	unconfigured=true;
}
/* Add nodes first, in order */



void AudioGraphProcess::add_node(AudioNode *p_node) {
	
	ProcessNode * process_node = new ProcessNode;
	
	process_node->node=p_node;

	/* by default the input ports read from silence buffer */
	process_node->input_audio_buffers.resize(p_node->get_in_audio_port_count());
	process_node->input_audio_sources.resize(p_node->get_in_audio_port_count());
	
	for (int i=0;i<p_node->get_in_audio_port_count();i++) {
		
		int port_channels=p_node->get_in_audio_port_channel_count(i);
		
		for (int j=0;j<port_channels;j++) {
		
			p_node->connect_in_audio_port_channel_buffer(i,j,&audio_silence_buffer[0]);
		}
				
		process_node->input_audio_buffers[i]=NULL;
	}		
	/* by default, set the output ports to write from trash buffer */
	
	process_node->output_audio_buffers.resize(p_node->get_out_audio_port_count());
	
	for (int i=0;i<p_node->get_out_audio_port_count();i++) {
		
		int port_channels=p_node->get_out_audio_port_channel_count(i);
		
		for (int j=0;j<port_channels;j++) {
		
			p_node->connect_out_audio_port_channel_buffer(i,j,&audio_trash_buffer[0]);
		}
		process_node->output_audio_buffers[i]=NULL;
	}	
		
	/* We'll store hoe many sources does each input have here */
	process_nodes.push_back(process_node);
	p_node->set_node_config( &audio_config );
}

/* Then Add Connections */

void AudioGraphProcess::add_connection(const AudioConnection& p_connection) {
	
	//printf("connecting %lls %i to %lls %i\n",process_nodes[p_connection.from_node]->node->get_caption().c_str(),p_connection.from_port,process_nodes[p_connection.to_node]->node->get_caption().c_str(),p_connection.to_port);
			
	
	ERR_FAIL_INDEX(p_connection.from_node,(int)process_nodes.size()); 
	ERR_FAIL_INDEX(p_connection.to_node,(int)process_nodes.size());
	ERR_FAIL_INDEX(p_connection.from_port,process_nodes[p_connection.from_node]->node->get_out_audio_port_count());
	ERR_FAIL_INDEX(p_connection.to_port,(int)process_nodes[p_connection.to_node]->input_audio_buffers.size());
	
	/* FIRST: Check if source node (an output) has a writing-to buffer, if not, create it  */
	
	ProcessNode *from_node=process_nodes[p_connection.from_node];
	ProcessNode *to_node=process_nodes[p_connection.to_node];
	
	
	switch(p_connection.type) {
	
	
		case AudioConnection::TYPE_AUDIO: {
	
			
			if (!from_node->output_audio_buffers[p_connection.from_port]) {
				
				from_node->output_audio_buffers[p_connection.from_port] = new AudioBuffer;
				from_node->output_audio_buffers[p_connection.from_port]->connect_out( from_node->node, p_connection.from_port );
				
			}
			
			/* SECOND - Check if destination node (INPUT port) is either empty/reads from buffer(1 connection) or reads from many */
			
			if (process_nodes[p_connection.to_node]->input_audio_sources[p_connection.to_port].empty()) { 
			
				/** No one connected to this node 
				*  Since so far after this it will have only ONE connection, 
				*  we can set it to read directly.
				*/
				
				AudioBuffer *buff = from_node->output_audio_buffers[p_connection.from_port];
				
				buff->connect_in( to_node->node, p_connection.to_port );		
				process_nodes[p_connection.to_node]->input_audio_sources[p_connection.to_port].push_back( buff );		
				
			} else {
				
				/**
				* IF it only has one connection, it means it's reading directly.
				* Since now it's going to have more than one, we need a buffer to
				* mix the data from the source connections.
				*/
				
				if (process_nodes[p_connection.to_node]->input_audio_sources[p_connection.to_port].size()==1) {
					
					ERR_FAIL_COND( process_nodes[p_connection.to_node]->input_audio_buffers[p_connection.to_port]!=NULL);
					
					AudioBuffer * buff=new AudioBuffer;
					process_nodes[p_connection.to_node]->input_audio_buffers[p_connection.to_port]=buff;
					buff->connect_in( to_node->node, p_connection.to_port );		
				}
				
				/**
				* Add the Source Buffer to the listof Source Buffers
				*/
				
				AudioBuffer *src_buff=from_node->output_audio_buffers[p_connection.from_port];
				process_nodes[p_connection.to_node]->input_audio_sources[p_connection.to_port].push_back(src_buff);
				
			}
		} break;
		case AudioConnection::TYPE_EVENT: {
		
			if (!from_node->output_event_buffers[p_connection.from_port]) {
				
				
				EventBuffer *ev = new EventBuffer;
				ev->resize(EVENT_BUFFER_SIZE);
				
				from_node->output_event_buffers[p_connection.from_port] = ev;
				from_node->node->connect_out_event_port_buffer(p_connection.from_port,&(*ev)[0]);
				
			}
			
			/* SECOND - Check if destination node (INPUT port) is either empty/reads from buffer(1 connection) or reads from many */
			
			if (process_nodes[p_connection.to_node]->input_event_sources[p_connection.to_port].empty()) { 
			
				/** No one connected to this node 
				*  Since so far after this it will have only ONE connection, 
				*  we can set it to read directly.
				*/
				
				EventBuffer *buff = from_node->output_event_buffers[p_connection.from_port];
				
				to_node->node->connect_in_event_port_buffer( p_connection.to_port, &(*buff)[0] );
				
				process_nodes[p_connection.to_node]->input_event_sources[p_connection.to_port].push_back( buff );		
				
			} else {
				
				/**
				* IF it only has one connection, it means it's reading directly.
				* Since now it's going to have more than one, we need a buffer to
				* mix the data from the source connections.
				*/
				
				if (process_nodes[p_connection.to_node]->input_event_sources[p_connection.to_port].size()==1) {
					
					ERR_FAIL_COND( process_nodes[p_connection.to_node]->input_event_buffers[p_connection.to_port]!=NULL);
					
					EventBuffer *ev = new EventBuffer;
					ev->resize(EVENT_BUFFER_SIZE);
					process_nodes[p_connection.to_node]->input_event_buffers[p_connection.to_port]=ev;
					to_node->node->connect_in_event_port_buffer(p_connection.to_port,&(*ev)[0]);
					
					
				}
				
				/**
				* Add the Source Buffer to the listof Source Buffers
				*/
				
				EventBuffer *src_buff=from_node->output_event_buffers[p_connection.from_port];
				process_nodes[p_connection.to_node]->input_event_sources[p_connection.to_port].push_back(src_buff);
								
			}
		
		} break;
		case AudioConnection::TYPE_CONTROL: {
		
		
		} break;
	}
}


void AudioGraphProcess::configure_connections() {
	
	unconfigured=false;
	
}

int AudioGraphProcess::process(int p_frames) {
	 
	if (unconfigured)
		return p_frames; // nothing processed
	
	if (p_frames>AUDIO_BUFFER_SIZE)
		p_frames=AUDIO_BUFFER_SIZE;
	
	
	audio_config.audio_buffer_size=p_frames;
	audio_config.event_buffer_size=EVENT_BUFFER_SIZE;
	
	/**
	 * Process the audio in the topologically solved graph 
	 * The audio should flow by itself, except when many outputs
	 * are connected to an input, in which case, they must be added
	 * manually.
	 */
	
	for (int i=0;i<(int)process_nodes.size();i++) {
		
		/** STEP 1: Check the Input Buffers.. */
		
		for (int j=0;j<process_nodes[i]->input_audio_buffers.size();j++) {
			
			AudioBuffer *buff=process_nodes[i]->input_audio_buffers[j];
			
			if (buff==NULL)
				continue; ///< Doesnt use an input buffer
		  
			/** STEP 2 - CLEAR The buffer */
			
			buff->clear(p_frames);
			
			/** STEP 3 - MIX The outputs into this input */
					 
			for (int k=0;k<process_nodes[i]->input_audio_sources[j].size();k++) {
				
				buff->add_from(process_nodes[i]->input_audio_sources[j][k],p_frames);
			}
		}
		
		/** STEP 4: Check the Input Event Buffers.. */
				
		for (int j=0;j<process_nodes[i]->input_event_buffers.size();j++) {
			
			EventBuffer *buff=process_nodes[i]->input_event_buffers[j];
			
			if (buff==NULL)
				continue; ///< Doesnt use an input buffer
		  
			/** STEP 5 - CLEAR The event buffer */
			
			MusicEvent *ebuff_dst = &(*buff)[0];
			ebuff_dst[0]=MusicEvent( MusicEvent::STREAM_TAIL, 0,0,0);
			int idx=0;
			
			/** STEP 6 - APPEND The outputs into this input */
					 
			for (int k=0;k<process_nodes[i]->input_event_sources[j].size();k++) {
				
				MusicEvent *ebuff_src = &(*process_nodes[i]->input_event_sources[j][k])[0];
				
				for (int l=0;l<EVENT_BUFFER_SIZE;l++) {
				
					if (idx==(EVENT_BUFFER_SIZE-1))
						break; // buffer full -1 (space for tail)
					
					if (ebuff_src->type==MusicEvent::STREAM_TAIL)
						break;
						
					ebuff_dst[idx]=*ebuff_src;
					idx++;
					ebuff_src++;
				
				}
				
			}
		}
		
		/** STEP 7: Check the Output Event Buffers.. */
		
		for (int j=0;j<process_nodes[i]->output_event_buffers.size();j++) {
		
			EventBuffer *buff=process_nodes[i]->output_event_buffers[j];
			
			if (buff==NULL)
				continue; ///< Doesnt use an input buffer
		
			(*buff)[0]=MusicEvent( MusicEvent::STREAM_TAIL, 0,0,0);
		}
				
		/** STEP 8: Process! */
				
		process_nodes[i]->node->process();
	}


	/* done ! */
		
	return p_frames;
}

AudioGraphProcess::AudioGraphProcess() {
	
	unconfigured=true;

	audio_trash_buffer.resize(AUDIO_BUFFER_SIZE);
	audio_silence_buffer.resize(AUDIO_BUFFER_SIZE);
	for (int i=0;i<AUDIO_BUFFER_SIZE;i++)
		audio_silence_buffer[i]=0;
	
	event_trash_buffer.resize(EVENT_BUFFER_SIZE);
	event_silence_buffer.resize(EVENT_BUFFER_SIZE);
	event_silence_buffer[0]=MusicEvent( MusicEvent::STREAM_TAIL, 0,0,0);

}
	
	
AudioGraphProcess::~AudioGraphProcess() {
	
	
	clear();
}
	














