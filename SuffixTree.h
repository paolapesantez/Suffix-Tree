#ifndef SUFFIXTREE_H_INCLUDED
#define SUFFIXTREE_H_INCLUDED

#include "Header.h"
#include "Node.h"


class SuffixTree
{
	private:				
		Node* root;		
		string name_sequence;
		string sequence;
		string alphabet;	
		string file_name;
		
		Node* deepest_internal_node;
		int number_nodes;
		int number_suffix;
		int suffix_tree_depth;
		int string_length_deepest_internal_node;
		int longest_exact_matching_repeat_start;
		int longest_exact_matching_repeat_end;		
		stringstream traversing_string;
		
		int createSuffixBranch();
		Node* insert(Node* prev_ptr, int in_head);
		Node* findPath(Node* wk_ptr, int in_head); 
		void findDeepestInternalNode(Node* node);        
		void printNodesLevel(Node* node, int level);
		void printNodeChildren(Node* node);
		void printSuffixTree(Node* node, int level);
		void DFS(Node* node);        
		void postOrder(Node* node);
		void BWT(Node* node);		
		void deleteSuffixTree(Node* node);
		void generateReportOutputFile(string text);

	public:
		SuffixTree();		  
		~SuffixTree();		  
		string correctSequence(string sequence);		
		void createSuffixTree(string input_sequence,string input_alphabet);		
		void printReport(string input_file_name);
		void DFS();        
		void postOrder();	
		void BWT();        
		void displayRandomNodeChildren();
		void deleteSuffixTree();
		void setTraversingString();
		string getTraversingString();
		void setAlphabetBWT();
		void setAlphabet(string input_alphabet);
};
#endif
 

 