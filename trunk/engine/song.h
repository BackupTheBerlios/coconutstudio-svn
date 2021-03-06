//
// C++ Interface: song
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef SONG_H
#define SONG_H

#include "engine/audio_graph.h"
/**
	@author Juan Linietsky <reduzio@gmail.com>
*/
class Song {

	AudioGraph audio_graph;
public:

	AudioGraph *get_audio_graph();
	Song();
	~Song();

};

#endif
