//
// C++ Implementation: audio_driver_manager
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "audio_driver_manager.h"
#include "error_macros.h"
#include <stddef.h>
	
AudioDriver* AudioDriverManager::audio_drivers[MAX_AUDIO_DRIVERS];
int AudioDriverManager::audio_driver_count=0;
int AudioDriverManager::current_driver=-1;
AudioGraph * AudioDriverManager::audio_graph=NULL;
	
void AudioDriverManager::set_audio_graph(AudioGraph *p_graph) {

	audio_graph=p_graph;
	for (int i=0;i<audio_driver_count;i++) {
		
		audio_drivers[i]->set_audio_graph(audio_graph);
	}
}

AudioGraph * AudioDriverManager::get_audio_graph() {

	return audio_graph;
}

int AudioDriverManager::get_driver_count() {

	return audio_driver_count;
}



void AudioDriverManager::add_driver(AudioDriver *p_audio_driver) {

	ERR_FAIL_COND(audio_driver_count>=MAX_AUDIO_DRIVERS);
	audio_drivers[audio_driver_count++]=p_audio_driver;
	printf("added driver, %i drivers\n",audio_driver_count);
	if (audio_graph)
		p_audio_driver->set_audio_graph(audio_graph);
}

void AudioDriverManager::set_current(int p_index) {

	ERR_FAIL_INDEX(p_index,audio_driver_count);

	bool restart=false;
	if (current_driver>=0 && current_driver<audio_driver_count) {
	
		if (audio_drivers[current_driver]->is_active()) {
		
			/* fill up info */
			audio_drivers[current_driver]->finish();
			restart=true;
		}
	}
	current_driver=p_index;
	if (restart)
		audio_drivers[current_driver]->init();
		
	
}
int AudioDriverManager::get_current() {

	return current_driver;
}
AudioDriver *AudioDriverManager::get_driver(int p_index) {

	ERR_FAIL_INDEX_V(p_index,audio_driver_count,NULL);
	return audio_drivers[p_index];
}
