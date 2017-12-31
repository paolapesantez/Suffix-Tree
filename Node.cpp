#include "Node.h"


Node::Node(int id, const string alphabet)
{
  node_ID = id;
  node_string_depth = 0;
  node_suffix_link = NULL;
  node_parent_ptr = NULL;  
  node_suffix_number = -1; //It will be different of -1 just when the node is a leaf
  node_parent_edge_label_start= 0;
  node_parent_edge_label_end = 0;
  for(int i = 0; i < alphabet.length(); i++)
  {
    if(alphabet[i] != ' ')
    {
      node_children_ptrs[alphabet[i]] = 0;
    }  
  }   
}


/* ************************************************************************************************************************* */


string Node::printNodeStringDepth()
{	
	stringstream text;
	text << node_ID << ":" << node_string_depth << " ";
	return text.str();	
}

/* ************************************************************************************************************************* */


string Node::printNodeSuffixNumber()
{	
	stringstream text;
	text << node_suffix_number << " ";
	return text.str();	
}


/* ************************************************************************************************************************* */


string Node::printChild()
{	
	stringstream text;
	text << "Child ID: " << node_ID << endl;
	return text.str();	
}


