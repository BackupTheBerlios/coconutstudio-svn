//
// C++ Implementation: audio_graph
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "audio_graph.h"
#include "error_macros.h"
#include "audio_control.h"

bool AudioGraph::recompute_process_order() {

	
	graph_order_valid=false;
	
	if (nodes.size()<2)
		return false;
	
	/* This is my own algorithm for non-recursive graph solving, I call it Squirrel-Graph-Sort  */
	
	/* Fill the domain/image inverted process order list 
	   this list maps node->order, this is why it must
	   be filled with the inverted numbers */
	
	inv_process_order.resize(nodes.size());
	for (int i=0;i<(int)nodes.size();i++)
		inv_process_order[i]=i;
	
	
	int iterations=0;
	int swaps;
	
	int worst_case=POW2(connections.size()); //worst case of a bubblesort is pow2
	do {
		swaps=0;
		
		
		for (int i=0;i<(int)connections.size();i++) {
			
			AudioConnection conn=connections[i];
			
			if (inv_process_order[conn.from_node] > inv_process_order[conn.to_node]) {
				
				/* Swap */
			
				int aux=inv_process_order[conn.from_node];
				inv_process_order[conn.from_node]=inv_process_order[conn.to_node];
				inv_process_order[conn.to_node]=aux;
				swaps++;				
			}
		}
		
		iterations++;
	
	} while (iterations<=worst_case && swaps>0);
	/* I'm guessing as a worst case that the maximum iterations (worst case) 
	   is the amount of connections to process, just like the bubble sort, and if it
	   goes beyond this, then there must be a cyclic link. Another possibility
	   to detect cyclic links may be comparing if the input and output are the same 
	   and swaps>0.
	
	*/
	
	//printf("Iterations needed to solve graph: %i ( %i being worst case  )\n",iterations,worst_case);
	
	if (iterations>=(int)nodes.size()) {
		
		//printf("graph solving FAILED.\n");
		return true; /* we fucked it */
	}

	process_order.resize( inv_process_order.size() );

	/* invert domain/image and place into process_order */
	for (int i=0;i<(int)nodes.size();i++) {

		process_order[ inv_process_order[i] ] = i;
	}
	
	graph_order_valid=true;	
	return false;
}


void AudioGraph::recompute_graph() {

	graph_process.clear();
	
	if (!graph_order_valid) {
		/* just put the nodes in any order */		
		for (int i=0;i<nodes.size();i++) {

			graph_process.add_node(nodes[i]);
		}
		/* but dont make any connections */
		graph_process.configure_connections(); 
		
		return;
	}
	
	
	for (int i=0;i<nodes.size();i++) {
		//printf("%i: %lls\n",i, nodes[process_order[i]]->get_caption().c_str() );
		graph_process.add_node(nodes[process_order[i]]);
	}
	
	
	for (int i=0;i<connections.size();i++) {
		
		//printf("**connecting %lls,%i to %lls,%i\n",nodes[connections[i].from_node]->get_caption().c_str(),connections[i].from_port,nodes[connections[i].to_node]->get_caption().c_str(),connections[i].to_port);
		
		graph_process.add_connection(connections[i]);
		
	}
	
	
	graph_process.configure_connections();
	
	
}


int AudioGraph::get_node_count() {

	return nodes.size();	

}

AudioNode* AudioGraph::get_node(int p_index) {

	ERR_FAIL_INDEX_V(p_index,(int)nodes.size(),NULL);
	return nodes[p_index];
}

void AudioGraph::add_node(AudioNode *p_node,int p_at_index) {

	AudioControl::mutex_lock();
	
	if (p_at_index<0 || p_at_index>=nodes.size())
		nodes.push_back(p_node);
	else
		nodes.insert( nodes.begin() + p_at_index, p_node );
		
	p_node->set_sampling_rate(sampling_rate);
	
	recompute_process_order();
	recompute_graph();
	
	AudioControl::mutex_unlock();
	
}


/**
 * Helper to adjust an index from the array, according to a deleted node. If the index
 * is greater than the deleted note, the index is decremented by 1
 * @param p_index index to adjust
 * @param p_deleted index being deleted
 * @return true if index to adjust equals index being deleted
 */
static bool index_adjust(int &p_index,int p_deleted) {
	if (p_index>p_deleted)
		p_index--;
	else if (p_index==p_deleted)
		return true;
	
	return false;
}

void AudioGraph::erase_node(int p_index) {

	ERR_FAIL_INDEX(p_index,(int)nodes.size());

	AudioControl::mutex_lock();	
	
	/* erase node */
	nodes.erase( nodes.begin() + p_index );
	
	recompute_process_order();
	recompute_graph();
	
	
	AudioControl::mutex_unlock();
	
}

AudioGraph::ConnectError AudioGraph::connect(const AudioConnection& p_connection) {

	ERR_FAIL_INDEX_V(p_connection.from_node,(int)nodes.size(),CONNECT_INVALID_NODE);
	ERR_FAIL_INDEX_V(p_connection.to_node,(int)nodes.size(),CONNECT_INVALID_NODE);
	switch (p_connection.type) {
		
		case AudioNode::PORT_AUDIO: {
			ERR_FAIL_INDEX_V(p_connection.from_port,nodes[p_connection.from_node]->get_out_audio_port_count(),CONNECT_INVALID_PORT);
			ERR_FAIL_INDEX_V(p_connection.to_port,nodes[p_connection.to_node]->get_in_audio_port_count(),CONNECT_INVALID_PORT);
		} break;
		case AudioNode::PORT_EVENT: {
			ERR_FAIL_INDEX_V(p_connection.from_port,nodes[p_connection.from_node]->get_out_event_port_count(),CONNECT_INVALID_PORT);
			ERR_FAIL_INDEX_V(p_connection.to_port,nodes[p_connection.to_node]->get_in_event_port_count(),CONNECT_INVALID_PORT);
		} break;
		case AudioNode::PORT_CONTROL: {
			ERR_FAIL_INDEX_V(p_connection.from_port,nodes[p_connection.from_node]->get_control_port_count(),CONNECT_INVALID_PORT);
			ERR_FAIL_INDEX_V(p_connection.to_port,nodes[p_connection.to_node]->get_control_port_count(),CONNECT_INVALID_PORT);
		} break;
	}
	
	AudioControl::mutex_lock();
	
	/* Check if connection exists first */
	for (int i=0;i<(int)connections.size();i++) {
		
		if (p_connection == connections[i]) {
			
			last_error=CONNECT_ALREADY_EXISTS;
			return CONNECT_ALREADY_EXISTS;
		}
	}
		
	if (p_connection.type==AudioNode::PORT_AUDIO) {
		if ( nodes[p_connection.from_node]->get_out_audio_port_channel_count(p_connection.from_port) !=
			nodes[p_connection.to_node]->get_in_audio_port_channel_count(p_connection.to_port) ) {
			
			last_error=CONNECT_CHANNELS_DIFFER;
			return CONNECT_CHANNELS_DIFFER;
		}
	}
	
	/* Then make connection */
		
	connections.push_back(p_connection);
	
	/* If connection causes a cyclic link, erase it */
	if (recompute_process_order()) {
		connections.erase( connections.begin() + ( connections.size() - 1 ) );
		
		if (recompute_process_order()) {
		
			ERR_PRINT("It's fucked, basically");
			connections.clear();
		}
		
		recompute_graph();
		last_error=CONNECT_CYCLIC_LINK;
		return CONNECT_CYCLIC_LINK;
		
	}
	
	recompute_graph();
	last_error=CONNECT_OK;
	
	AudioControl::mutex_unlock();
	
	return CONNECT_OK;
}


void AudioGraph::disconnect(const AudioConnection& p_connection) {

	AudioControl::mutex_lock();
	
	for (int i=0;i<(int)connections.size();i++) {
		
		if (connections[i]==p_connection) {
			
			connections.erase( connections.begin() + i );
			i--;
		}
	}

	if ( recompute_process_order() ) {
		ERR_PRINT("recompute_process_order()");
		AudioControl::mutex_unlock();
		return;
	}
	
	recompute_graph();
	
	AudioControl::mutex_unlock();
	
}


bool AudioGraph::has_connection(const AudioConnection& p_connection) {

	for (int i=0;i<connections.size();i++)
		if (p_connection==connections[i])
			return true;
			
	return false;
}

bool AudioGraph::can_connect(const AudioConnection& p_connection) {

	if (has_connection(p_connection))
		return false;
		
	AudioControl::mutex_lock();
	
	ConnectError ce = connect( p_connection );
	
	if (ce==CONNECT_OK) {
	
		disconnect( p_connection );
	}
	
	AudioControl::mutex_unlock();
	
	return (ce==CONNECT_OK);

}


int AudioGraph::get_connection_count() {

	return connections.size();

}
const AudioConnection* AudioGraph::get_connection(int p_index) {

	ERR_FAIL_INDEX_V(p_index,(int)connections.size(),NULL);
	return &connections[p_index];
}

int AudioGraph::get_node_index(AudioNode* p_fromnode) {
	
	for (int i=0;i<nodes.size();i++)
		if (nodes[i]==p_fromnode)
			return i;
	
	return -1;
}

void AudioGraph::swap_node_indices(int p_idx,int p_with_idx) {

	ERR_FAIL_INDEX(p_idx,nodes.size());
	ERR_FAIL_INDEX(p_with_idx,nodes.size());

	if (p_idx==p_with_idx)
		return;
		
	AudioControl::mutex_lock();
		
	SWAP(nodes[p_idx],nodes[p_with_idx]);
	
	for(int i=0;i<connections.size();i++) {
	
	
		AudioConnection &c=connections[i];
		
		if (c.from_node==p_idx)
			c.from_node=p_with_idx;
		else if (c.from_node==p_with_idx)
			c.from_node=p_idx;
			
		if (c.to_node==p_idx)
			c.to_node=p_with_idx;
		else if (c.to_node==p_with_idx)
			c.to_node=p_idx;
	}
	
	recompute_process_order();
	recompute_graph();
	
	AudioControl::mutex_unlock();
	
}


AudioGraph::ConnectError AudioGraph::get_last_conect_error() {
	
	return last_error;
}
int AudioGraph::process(int p_frames) {
	
	return graph_process.process(p_frames);	
}

void AudioGraph::clear() {
	
	nodes.clear();
	process_order.clear();
	inv_process_order.clear();
	connections.clear();
	recompute_process_order();
	recompute_graph();

}

void AudioGraph::set_sampling_rate(float p_hz) {

	AudioControl::mutex_lock();

	sampling_rate=p_hz;
	
	for (int i=0;i<nodes.size();i++) {
	
		nodes[i]->set_sampling_rate(p_hz);
	}

	AudioControl::mutex_unlock();

}

AudioGraph::AudioGraph() {
	
	last_error=CONNECT_OK;
	graph_order_valid=false;
	sampling_rate=48000.0;
}

AudioGraph::~AudioGraph() {

}
