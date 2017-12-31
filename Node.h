#ifndef NODE_H_INCLUDED
#define NODE_H_INCLUDED

#include "Header.h"

class Node
{
	 friend class SuffixTree; // In order to allow the class SuffixTree to have access to the private elements of Node

	 public:
	    Node(int id, const string alphabet);
		string printNodeStringDepth();
		string printNodeSuffixNumber();
		string printChild();

	private:
		int node_ID;
		int node_string_depth;
		Node* node_suffix_link;
		Node* node_parent_ptr;		
		map<char, Node*> node_children_ptrs;
		int node_suffix_number;					//If node is a leaf
		int node_parent_edge_label_start;
		int node_parent_edge_label_end;
		
};
#endif


