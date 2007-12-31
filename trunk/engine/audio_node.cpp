
#include "audio_node.h"



AudioNode::ControlPortInfo AudioNode::get_control_port_info(int p_port) const {

	ControlPortInfo cpi;
	cpi.name=String("Port "+String::num(p_port));
	return cpi;
}
String AudioNode::get_control_port_text_for_value(int p_port,float p_value) {

	return String::num(p_value);
}

String AudioNode::get_control_port_text_for_value(int p_port,float p_value);


void AudioNode::set_node_config(const AudioNodeConfig* p_config) {

	_node_config=p_config;
}

AudioNode::AudioNode() {

	_node_config=0;
}
AudioNode::~AudioNode() {

}
