//
// C++ Interface: audio_driver_manager
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef AUDIO_DRIVER_MANAGER_H
#define AUDIO_DRIVER_MANAGER_H


#include "engine/audio_driver.h"

/**
	@author Juan Linietsky <reduzio@gmail.com>
*/
class AudioDriverManager {

	enum {
	
		MAX_AUDIO_DRIVERS=8
	};

	static AudioDriver* audio_drivers[MAX_AUDIO_DRIVERS];
	static int audio_driver_count;
	static int current_driver;

public:

	static void add_driver(AudioDriver *p_audio_driver);

	static void set_current(int p_index);
	static int get_current();
	static AudioDriver *get_driver(int p_index);
	

};

#endif
