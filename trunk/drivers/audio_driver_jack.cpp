//
// C++ Implementation: audio_driver_jack
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//

#ifdef JACK_ENABLED

#include "audio_driver_jack.h"
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <map>
#include "error_macros.h"


int AudioDriverJACK::JackNode::ofs_mix=0;
int AudioDriverJACK::JackNode::cbk_nframes=0;
		
void AudioDriverJACK::set_node_name(int p_index,String p_name) {

	ERR_FAIL_INDEX(p_index,ports.size());
	ports[p_index]->name=p_name;

}


void AudioDriverJACK::set_audio_graph(AudioGraph * p_audio_graph) {

	audio_graph=p_audio_graph;
	audio_graph->set_sampling_rate(srate);
}

int AudioDriverJACK::JackNode::get_in_audio_port_count() const {

	return (type==TYPE_AUDIO && !input)?1:0;
		
}

int AudioDriverJACK::JackNode::get_out_audio_port_count() const {

	return (type==TYPE_AUDIO && input)?1:0;

}

int AudioDriverJACK::JackNode::get_in_audio_port_channel_count(int p_port) const {

	if (type!=TYPE_AUDIO)
		return 0;
	
	return !input?jack_ports.size():0;
	
}
int AudioDriverJACK::JackNode::get_out_audio_port_channel_count(int p_port) const {

	if (type!=TYPE_AUDIO)
		return 0;
		
	return input?jack_ports.size():0;

}
	
void AudioDriverJACK::JackNode::connect_in_audio_port_channel_buffer(int p_port,int p_channel,float* p_buff) {

	printf("%p, connecting port %i, channel %i to buffer %p\n",this,p_port,p_channel,p_buff);
	ERR_FAIL_COND(type!=TYPE_AUDIO || input);

	ERR_FAIL_INDEX(p_port,1);
	ERR_FAIL_INDEX(p_channel,audio_buffers.size());
	audio_buffers[p_channel]=p_buff;

}

void AudioDriverJACK::JackNode::connect_out_audio_port_channel_buffer(int p_port,int p_channel,float* p_buff) {

	ERR_FAIL_COND(type!=TYPE_AUDIO || !input);

	ERR_FAIL_INDEX(p_port,1);
	ERR_FAIL_INDEX(p_channel,audio_buffers.size());
	audio_buffers[p_channel]=p_buff;

}

/* Control Port API */
		


int AudioDriverJACK::JackNode::get_control_port_count() const {

	return 0;
}

ControlPort* AudioDriverJACK::JackNode::get_control_port(int p_port) {

	return NULL;
}
	
/* Event port API */

int AudioDriverJACK::JackNode::get_in_event_port_count() const {


	return (type==TYPE_EVENT && !input)?1:0;

}

int AudioDriverJACK::JackNode::get_out_event_port_count() const {

	return (type==TYPE_EVENT && input)?1:0;

}
	
void AudioDriverJACK::JackNode::connect_in_event_port_buffer(int p_port,MusicEvent*p_buff) {

	ERR_FAIL_COND(type!=TYPE_EVENT || input)

	ERR_FAIL_INDEX(p_port,1);
	event_buffer=p_buff;

}
void AudioDriverJACK::JackNode::connect_out_event_port_buffer(int p_port,MusicEvent* p_buff) {

	ERR_FAIL_COND(type!=TYPE_EVENT || !input);

	ERR_FAIL_INDEX(p_port,1);
	event_buffer=p_buff;

}

/* AudioNode Info */

String AudioDriverJACK::JackNode::get_name() const  {

	return name;
}

/* Process */

void AudioDriverJACK::JackNode::process()  {

	//get_node_config()
	
	switch(type) {
	
		case TYPE_AUDIO: {
		
			ERR_FAIL_COND( jack_ports.size() != audio_buffers.size() );

			int mix_frames=get_node_config()->audio_buffer_size;
		
			for (int c=0;c<jack_ports.size();c++) {
			
			 	
				jack_default_audio_sample_t *jack_buff = &((jack_default_audio_sample_t*)jack_port_get_buffer( jack_ports[c] , cbk_nframes ))[ofs_mix];
				float *audio_buff=audio_buffers[c];
			 	
			 	if (!audio_buff) {
			 		printf("%p no buffer %i, continuing\n",this,c);
			 		continue;
			 	}
				if (input) {
				
					for (int i=0;i<mix_frames;i++) {
					
						audio_buff[i]=jack_buff[i];
					}					
				} else {
			
					for (int i=0;i<mix_frames;i++) {
					
						jack_buff[i]=audio_buff[i];
					}					
				}
			}
		} break;
		case TYPE_EVENT: {
		
			if (input) {
			
			
			} else {
		
		
			}
		} break;
	}

}

/************************************************/
/************************************************/
/************************************************/
/************************************************/
/************************************************/
/************************************************/


/* Input and Output Nodes */

int AudioDriverJACK::get_node_count() const {

	return ports.size();
}

AudioNode *AudioDriverJACK::get_node(int p_index) {

	ERR_FAIL_INDEX_V(p_index,ports.size(),NULL);
	return ports[p_index];
}
AudioDriver::NodeType AudioDriverJACK::get_node_type(int p_index) const {

	ERR_FAIL_INDEX_V(p_index,ports.size(),TYPE_AUDIO);
	return ports[p_index]->type;

}


// inputs and outputs may be fixed depending on the platform, if this is the case, adding/erasing will have no effect

void AudioDriverJACK::add_node(int p_chans,int p_at_index,NodeType p_type,bool p_input) {

	if (p_type==TYPE_AUDIO) {
	
		ERR_FAIL_COND(p_chans<=0 || p_chans>8);
	} else {
	
		p_chans=1;
	}
	
	if (active)
		lock();
	
	String name;
	if (p_type==TYPE_AUDIO) {
		
		if (p_input) {
			hw_audio_in_count++;
			name = "HW_AUDIO_IN"+String::num(hw_audio_in_count);

		} else {
			hw_audio_out_count++;
			name = "HW_AUDIO_OUT"+String::num(hw_audio_out_count);
		}
	} else {
	
		if (p_input) {
			hw_event_in_count++;
			name = "HW_MIDI_IN"+String::num(hw_event_in_count);
		} else {
			hw_event_out_count++;
			name = "HW_MIDI_OUT"+String::num(hw_event_out_count);
		}
	
	}
		
	JackNode *jn = new JackNode;;
	jn->type=p_type;
	jn->name=name;
	jn->input=p_input;
	jn->chans=p_chans;
	jn->event_buffer=NULL;
	if (p_type==TYPE_AUDIO) {
		jn->audio_buffers.resize(p_chans);
		for (int i=0;i<p_chans;i++)
			jn->audio_buffers[i]=NULL;
		
	}
	
	
	for (int i=0;i<p_chans;i++) {
	
		/* Determine Name */	
		String port_name=name;
		if (p_chans==2) {
		
			port_name+=(i==0)?"_L":"_R";
		} else if (p_chans>2) {
		
			port_name+="_"+String::num(i+1);
		}
		
		if (active) {
			/* Create Port */
			jack_port_t* port = jack_port_register(client, port_name.ascii().get_data(), (p_type==TYPE_EVENT)?JACK_DEFAULT_MIDI_TYPE:JACK_DEFAULT_AUDIO_TYPE, p_input?JackPortIsInput:JackPortIsOutput, 0); 
		
			jn->jack_ports.push_back(port);
		} else {
		
			jn->jack_ports.push_back(NULL);
		}
		
	}
	
	if (active) {
		/* Connect to HW Ports not taken if Possible */
		
		const char **result,**iter;
		
		result = jack_get_ports (client, NULL, (p_type==TYPE_EVENT)?JACK_DEFAULT_MIDI_TYPE:JACK_DEFAULT_AUDIO_TYPE,JackPortIsPhysical|(p_input?JackPortIsOutput:JackPortIsInput));
		
		
		if (result) {
		
			std::vector<const char *> unclaimed_ports;
			iter=result;
			while(*iter) {
				
				bool claimed=false;
				
				for (int i=0;i<ports.size();i++) {
				
					if (ports[i]->type!=p_type)
						continue;
					if (ports[i]->input!=p_input)
						continue;
						
					for (int j=0;j<ports[i]->jack_ports.size();j++) {
					
					
						if (jack_port_connected_to(ports[i]->jack_ports[j],*iter)) {
							claimed=true;
							break;
						}
					}
					if (claimed)
						break;
				}
				
				if (!claimed)
					unclaimed_ports.push_back(*iter);
				
				iter++;
			}
			
			
			jn->connected_to="";
			for(int i=0;i<unclaimed_ports.size();i++) {
			
				if (i>=jn->jack_ports.size())
					break;
				
				if (jn->connected_to!="")
					jn->connected_to+=",";
				
				jack_connect(client, jack_port_name(jn->jack_ports[i]), unclaimed_ports[i]);
				jn->connected_to+=unclaimed_ports[i];
				
			}
			
			free(result);
			
		}	
	
	}
	
	/* insert */
	
	if (p_at_index<0 || p_at_index>=ports.size()) {
		ports.push_back(jn);
		printf("adding at index END\n");
	} else {
	
		printf("adding at index %i\n",p_at_index);
		std::vector<JackNode*>::iterator I=ports.begin();
		I+=p_at_index;
		ports.insert(I,jn);
	}
		
	if (active)
		unlock();


}

void AudioDriverJACK::add_audio_input(int p_chans,int p_at_index){

	
	add_node(p_chans,p_at_index,TYPE_AUDIO,true);

}

void AudioDriverJACK::add_event_input(int p_at_index){

	add_node(1,p_at_index,TYPE_EVENT,true);

}

void AudioDriverJACK::add_audio_output(int p_chans,int p_at_index){

	add_node(p_chans,p_at_index,TYPE_AUDIO,false);

}

void AudioDriverJACK::add_event_output(int p_at_index){

	add_node(1,p_at_index,TYPE_EVENT,false);

}

bool AudioDriverJACK::is_node_active(int p_index) const {

	return active;
}


void AudioDriverJACK::erase_node(int p_index){

	ERR_FAIL_COND(!active);
	
	ERR_FAIL_INDEX(p_index,ports.size());
	
	JackNode *jn = ports[p_index];
	
	if (active) {
		for (int i=0;i<jn->jack_ports.size();i++) {
		
			jack_port_unregister(client,jn->jack_ports[i]);
		}
	}
	
	delete ports[p_index];
	
	ports.erase( ports.begin() + p_index );

}
	
String AudioDriverJACK::get_node_external_connection(int p_index) const {

	ERR_FAIL_INDEX_V(p_index,ports.size(),"");

	return ports[p_index]->connected_to;
}

void AudioDriverJACK::connect_node_to_external(int p_index,String p_to) {

	ERR_FAIL_INDEX(p_index,ports.size());
	
	ERR_FAIL_COND(p_to.get_slice_count(",")!=ports[p_index]->jack_ports.size());
	
	if (active) {
		/* disconnect all first */
		for (int i=0;i<ports[p_index]->jack_ports.size();i++) {
		
			jack_port_disconnect( client,ports[p_index]->jack_ports[i]);
		}
		/* connect */
		for (int i=0;i<ports[p_index]->jack_ports.size();i++) {
		
			String dest=p_to.get_slice(",",i);
			jack_connect(client, jack_port_name(ports[p_index]->jack_ports[i]), dest.ascii().get_data());
		}
	}
	
	ports[p_index]->connected_to=p_to;
	

}

std::list<String> AudioDriverJACK::get_connectable_external_list(NodeType p_type,bool p_input,int p_channels) const {

	if (p_type==TYPE_EVENT)
		p_channels=1;
		
	std::list<String> clist;
	
	if (!active)
		return clist;
	
	const char **result;
	
	//JackPortIsPhysical| ?
	result = jack_get_ports (client, NULL, (p_type==TYPE_EVENT)?JACK_DEFAULT_MIDI_TYPE:JACK_DEFAULT_AUDIO_TYPE,(p_input?JackPortIsOutput:JackPortIsInput));

	typedef std::map< String, std::vector<String> > PortMap;
	
	PortMap port_map;


	if (result) {
	
		const char **iter=result;
		
		while(*iter) {
			
			String portname = *iter;
			if (portname.find(":")!=-1) {
			
				String port = portname.get_slice(":",0);
				String plug = portname.get_slice(":",1);
			
				PortMap::iterator I = port_map.find(port);
				
				if (I==port_map.end()) {
				
					port_map[port]=std::vector<String>();
				}
				
				port_map[port].push_back(plug);
			
			}			
			
			iter++;
		}	
		free(result);	
	}	
	
	PortMap::iterator I=port_map.begin();
	
	for(;I!=port_map.end();I++) {
	
		std::vector<String> &vec=I->second;
	
		printf("vec size %i, channels %i\n",vec.size(),p_channels);
		for(int i=0;i<(vec.size()/p_channels);i++) {
					
			String portname;
			
			for(int j=0;j<p_channels;j++) {
				
				if (j>0)
					portname+=",";
				portname+=I->first+":"+vec[i*p_channels+j];	
			}
			
			clist.push_back(portname);
		}
	}
	
	return clist;	
}
	
/* ports used for settings */
	
int AudioDriverJACK::get_control_port_count() const {

	return 0;
}

ControlPort* AudioDriverJACK::get_control_port(int p_port) {

	return NULL;
}
/* Idendification */

String AudioDriverJACK::get_name() const {

	return "JACK Sound Driver";
}

int AudioDriverJACK::set_sampling_rate(jack_nframes_t nframes) {

	srate=nframes;
	if (audio_graph)
		audio_graph->set_sampling_rate(nframes);
        return 0;
}
 

int AudioDriverJACK::process(jack_nframes_t nframes) {

	JackNode::ofs_mix=0;
	JackNode::cbk_nframes=nframes;
	
	if (pthread_mutex_trylock(&mutex)!=0)
		return 0; // couldn't process audio (locked)
	
	while (nframes) {
		int done = audio_graph->process(nframes);
		JackNode::ofs_mix+=done;
		nframes-=done;
	}		
	
	pthread_mutex_unlock(&mutex);
	
        return 0;
        
}

bool AudioDriverJACK::init() {

	if (active)
		return true;

	if ((client = jack_client_new("Coconut")) == 0) {
                error_text="Jack server not running.";
                return false;
        }

	/* Set Callbacks */
	jack_set_process_callback(client, _process, this);
        jack_set_sample_rate_callback(client, _srate, this);
        jack_on_shutdown(client, _shutdown, this);

	if (jack_activate (client)) {
		error_text="Cannot activate client.";
                return false;
        }

	error_text="JACK client active.";
	active=true;

	/* Connect all existing ports */
	
	for (int i=0;i<ports.size();i++) {
	
		int chans=ports[i]->jack_ports.size();
		
		for (int j=0;j<chans;j++) {
	
			if (ports[i]->jack_ports[j]) {
			
				ERR_PRINT("BUG: port exists?");
			}
							
			/* Determine Name */	
			String port_name=ports[i]->name;
			
			if (chans==2) {
			
				port_name+=(j==0)?"_L":"_R";
			} else if (chans>2) {
			
				port_name+="_"+String::num(j+1);
			}
			
			/* Create Port */
			jack_port_t* port = jack_port_register(client, port_name.ascii().get_data(), (ports[i]->type==TYPE_EVENT)?JACK_DEFAULT_MIDI_TYPE:JACK_DEFAULT_AUDIO_TYPE, ports[i]->input?JackPortIsInput:JackPortIsOutput, 0); 
		
			ports[i]->jack_ports[j]=port;
			
		}
	}

	return true;
}
bool AudioDriverJACK::is_active() const {

	return active;
}
void AudioDriverJACK::finish() {


	if (!active)
		return;
			
		
	for (int i=0;i<ports.size();i++) {
		
		int chans=ports[i]->jack_ports.size();
		
		for (int j=0;j<chans;j++) {
		
			ERR_CONTINUE( !ports[i]->jack_ports[j] );
			jack_port_unregister( client, ports[i]->jack_ports[j] );
			ports[i]->jack_ports[j]=NULL;
			
		}
	}
	
	jack_client_close(client);
		
	
	error_text="JACK client disabled.";
	active=false;
	hw_audio_in_count=0;
	hw_audio_out_count=0;
	hw_event_in_count=0;
	hw_event_out_count=0;
	client=NULL;

	
}

String AudioDriverJACK::get_last_error() const {

	return error_text;
}


void AudioDriverJACK::lock() {

	pthread_mutex_lock(&mutex);
}
void AudioDriverJACK::unlock() {

	
	pthread_mutex_unlock(&mutex);
	
}


AudioDriverJACK::AudioDriverJACK() {

		
	error_text="JACK client disabled.";
	active=false;
	hw_audio_in_count=0;
	hw_audio_out_count=0;
	hw_event_in_count=0;
	hw_event_out_count=0;
	audio_graph=0;
	pthread_mutexattr_t attr;
	pthread_mutexattr_init(&attr);
	pthread_mutexattr_settype(&attr,PTHREAD_MUTEX_RECURSIVE);
	pthread_mutex_init(&mutex,&attr);

}


AudioDriverJACK::~AudioDriverJACK() {

	if (active)
		jack_client_close(client);
	for (int i=0;i<ports.size();i++)
		delete ports[i];
	ports.clear();

}


#endif