
#include "audio_node.h"


void ControlPort::set_visible(bool p_visible) {

	visible=p_visible;
}
bool ControlPort::is_visible() const {

	return visible;
}


void ControlPort::set_normalized(float p_val) {

	p_val*=get_max()-get_min();
	p_val+=get_min();
	set(p_val);

}

float ControlPort::get_normalized() const {

	float v=get();
	v-=get_min();
	v/=get_max()-get_min();
	return v;
}

String ControlPort::get_value_as_text(float p_value) const {

	return String::num(get());
}


ControlPort::Hint ControlPort::get_hint() const {

	return HINT_RANGE;
}

ControlPort::ControlPort() {

	visible=false;
}
ControlPort::~ControlPort() {

}


/*****************/


void AudioNode::set_node_config(const AudioNodeConfig* p_config) {

	_node_config=p_config;
}

void AudioNode::set_x(int p_x) {

	_x=p_x;
}
int AudioNode::get_x() const {

	return _x;
}

void AudioNode::set_y(int p_y) {

	_y=p_y;
}
int AudioNode::get_y() const {

	return _y;
}


void AudioNode::set_layer(int p_layer) {

	_layer=p_layer;
}
int AudioNode::get_layer() const {

	return _layer;
}


AudioNode::AudioNode() {

	_node_config=0;
	_x=-1;
	_y=-1;
	_layer=-1;	
}
AudioNode::~AudioNode() {

}
