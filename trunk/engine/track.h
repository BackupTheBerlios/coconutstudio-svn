//
// C++ Interface: track
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef TRACK_H
#define TRACK_H

/**
	@author Juan Linietsky <reduzio@gmail.com>
*/
class Track{
public:
	enum Type {
		TYPE_PATTERN,
		TYPE_AUDIO,
		TYPE_AUTOMATION
	};



	Track();
	
	~Track();

};

#endif
