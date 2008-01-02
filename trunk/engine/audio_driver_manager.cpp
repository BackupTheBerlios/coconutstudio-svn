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
	
	
void AudioDriverManager::add_driver(AudioDriver *p_audio_driver) {

	ERR_FAIL_COND(audio_driver_count>=MAX_AUDIO_DRIVERS);
	audio_drivers[audio_driver_count++]=p_audio_driver;
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
