#include "SuffixTree.h"
#include "SuffixTreeFiles.h"


SuffixTree::SuffixTree()
{
	root = NULL;
	number_nodes = 0;
	number_suffix = 0;
	suffix_tree_depth = 0;
	deepest_internal_node = NULL;
	string_length_deepest_internal_node = 0;  
	longest_exact_matching_repeat_start = 0;
	longest_exact_matching_repeat_end = 0;	
	traversing_string.str("");		
	file_name ="";
}


/* ************************************************************************************************************************* */


SuffixTree::~SuffixTree()
{	
}

/* ************************************************************************************************************************* */

//Function that divides the sequence in two parts: name and content
//This is because when I read a sequence from the file I store it like s1/AAAAGTGGGT. So to perform the alignment we jsut need the content
string SuffixTree::correctSequence(string sequence)
{
		string result;

		int pos = sequence.find_first_of("/");
		name_sequence = sequence.substr(0,pos);
		result = sequence.substr(pos+1,sequence.length());		
		return result;
}


/* ************************************************************************************************************************* */

//This procedure calls all the different procedures needed to create the suffix tree for the given sequence
void SuffixTree::createSuffixTree(string input_sequence,string input_alphabet)
{  							
	sequence = correctSequence(input_sequence); //divide the sequence : name/sequence
	sequence.append("$");						//we need to append $ to the sequence
	
	//When $ is the smaller symbol
	alphabet = "$";							//we need to append $ to the alphabet
	alphabet.append(input_alphabet);						

	//When $ is the greater symbol
	//alphabet = input_alphabet;
	//alphabet.append("$");
	
	//Initialize the tree ::: Create the root node
	root = new Node(0, alphabet);
	root->node_parent_edge_label_start = -1;
	root->node_parent_edge_label_end = -1;
	root->node_suffix_link = root;
	root->node_parent_ptr = root;	
	root->node_suffix_number = number_suffix++;  
	number_nodes++;
  
	createSuffixBranch();			
}


/* ************************************************************************************************************************* */

void SuffixTree::printReport(string input_file_name)
{
	findDeepestInternalNode(root);	
	if(sequence.length()<100)
		printSuffixTree(root,0);
	int number_leaf_nodes = number_suffix-1;
	int number_internal_nodes = number_nodes - number_leaf_nodes - 1;
	int suffix_tree_size_bytes = (sizeof(Node) * number_nodes) + (sizeof(SuffixTree)); 
	int avg_depth = suffix_tree_depth / number_internal_nodes;
  
	int pos = input_file_name.find_first_of(".");
	file_name = (input_file_name.substr(0,pos)).append("_Report.txt");
	ofstream outfile(file_name);  	
	outfile << "::: Report :::"  << endl << string(45,'-') << endl << endl;
	outfile.close();


	//Show the suffix tree information results to the screen
	stringstream line;
	//cout << endl << endl << "::: Report :::"<< endl << string(45,'-') << endl;  
	line << "Total number of nodes: " << number_nodes << endl;
	generateReportOutputFile(line.str());
	line.str("");
	line << "Number of leaves: " << number_leaf_nodes << endl;
	generateReportOutputFile(line.str());
	line.str("");	
	line << "Number of internal nodes: " << number_internal_nodes << endl;
	generateReportOutputFile(line.str());
	line.str("");
	line << "Total memory: " << suffix_tree_size_bytes << "b" << endl;
	generateReportOutputFile(line.str());
	line.str("");
	line << "Average string depth of internal node: " << avg_depth << endl;
	generateReportOutputFile(line.str());
	line.str("");	
	line << "String length of deepest internal node: " << string_length_deepest_internal_node << endl << endl;			
	generateReportOutputFile(line.str());
	line.str("");

	
	//Show the Longest Exact Matching Repeat
	line << endl << "::: Longest Exact Matching Repeat :::"<< endl << string(40,'-') << endl;  
	generateReportOutputFile(line.str());
	line.str("");
	line << "Repeating Subsequence: ";
	generateReportOutputFile(line.str());
	line.str("");
	for(int i=longest_exact_matching_repeat_start; i<=longest_exact_matching_repeat_end;i++)
	{   line << sequence[i];
		generateReportOutputFile(line.str());
		line.str("");
	}
	line << endl << "Initial Possition:" << longest_exact_matching_repeat_start+1 << endl;
	generateReportOutputFile(line.str());
	line.str("");
	line << "Final Possition:" << longest_exact_matching_repeat_end+1 << endl;	
	generateReportOutputFile(line.str());
	line.str("");
}
	
/* ************************************************************************************************************************* */

//Start of the algorithm where we start adding suffixes to construct the tree
int SuffixTree::createSuffixBranch()
{
	Node* current_node = root;
	int current_head = 0;
	int tail = sequence.length() - 1;	
	 
	while(current_head <= tail) //start walking through the characters in the sequence
	{    			
		current_node = insert(current_node, current_head);
	    current_head++;		
	}
	return 0;
}


/* ************************************************************************************************************************* */

//Function that inserts a node in the tree acording the two possible cases given by McCreight's algorithm
Node* SuffixTree::insert(Node* previous_node, int initial_head)
{
	int tail = sequence.length() - 1;
    Node* hop_node;
	bool band = true;
    
	//case A
    hop_node = previous_node->node_parent_ptr; //go to u's parent since u got created in previous iteration and it has no suffix link yet
    if(hop_node->node_suffix_link != NULL) //If u's parent has a suffix link
    {
      hop_node = hop_node->node_suffix_link; //Suffix link exists and we use it to reduce character comparisons
      while(band == true)
      {
			int tmp_head = initial_head + hop_node->node_string_depth; //move the head at the start of Betha
			Node* child_node = hop_node->node_children_ptrs[sequence[tmp_head]];
			if(child_node != 0)//If the node is not new, it was already there in iteration T(i-1)
			{
				int i = tmp_head;
				int j = child_node->node_parent_edge_label_start;
				bool mismatch = false;
				int parent_edge_label_end = child_node->node_parent_edge_label_end;
				while(j <= parent_edge_label_end)
				{
					if(sequence[i] != sequence[j])
					{
						mismatch = true;
						break;
					}
					i++;
					j++;
				}
				if(mismatch)
				{
					break;
				}
				else
				{
					hop_node = child_node;
				}
			}
			else
			{
				band = false;
			}
      }// node hops done          
      previous_node = findPath(hop_node, initial_head);
    }    
    else //If u's parent doesn't have a suffix link
    {
		Node* next_node;
		bool suffix_flag = true;
		int suffix_len = hop_node->node_parent_edge_label_end - hop_node->node_parent_edge_label_start + 1;
		if(hop_node->node_parent_ptr->node_ID == 0)
		{
			suffix_len = suffix_len - 1;
		}
		hop_node = hop_node->node_parent_ptr->node_suffix_link; //go one ancestor up too work with that suffix link
		
		band = true;
		while(band == true)
		{
			int tmp_head = initial_head + hop_node->node_string_depth;
			Node* child_node = hop_node->node_children_ptrs[sequence[tmp_head]];
			if(child_node != 0)
			{
				int i = tmp_head;
				int j = child_node->node_parent_edge_label_start;
				bool mismatch = false;
				int parent_edge_label_end = child_node->node_parent_edge_label_end;
				while(j <= parent_edge_label_end)
				{
					if(sequence[i] != sequence[j])
					{
						mismatch = true;
						break;
					}
					i++;
					j++;
					suffix_len--;
					if(suffix_len <= 0 && suffix_flag == true)
					{
						next_node = hop_node;
						suffix_flag = false;
					}
				}
				if(mismatch)
				{
					break;
				}
				else
				{            
					hop_node = child_node;					
				}
			}
			else
			{
				band = false;
			}
      }// node hops done 
      
	  //create the suffix link of u's parent
      Node* link_node = previous_node->node_parent_ptr;
      previous_node = findPath(hop_node, initial_head);

	  if(link_node->node_suffix_link == 0)
      {        
			if(link_node->node_string_depth == 1)
			{
				link_node->node_suffix_link = root;
			}
			else
			{          
				link_node->node_suffix_link = next_node;         
			}
      }
    }
    return previous_node;
}


/* ************************************************************************************************************************* */

//This function returns the node created when matching stops in insert
Node* SuffixTree::findPath(Node* hop_node, int initial_head)
{ 
	int tail = sequence.length() - 1;  
	initial_head += hop_node->node_string_depth;
	char key = sequence[initial_head];
	if(hop_node->node_children_ptrs[key] == 0)//if node doesn't exist create the new leaf node
	{
		Node* new_leaf_node = new Node(number_nodes++,alphabet); 
		new_leaf_node->node_parent_edge_label_start = initial_head;
		new_leaf_node->node_parent_edge_label_end = tail;
    
		new_leaf_node->node_parent_ptr = hop_node;
		hop_node->node_children_ptrs[key] = new_leaf_node;
		new_leaf_node->node_string_depth = new_leaf_node->node_parent_ptr->node_string_depth + (new_leaf_node->node_parent_edge_label_end - new_leaf_node->node_parent_edge_label_start + 1);
		new_leaf_node->node_suffix_number = number_suffix++;
		return new_leaf_node;  
	}
	else //if node exists we need to divide the edge label
	{
		Node* child_node = hop_node->node_children_ptrs[key];
		int i = initial_head;
		int j = child_node->node_parent_edge_label_start;
		bool mismatch = false;
		int parent_edge_label_end = j + child_node->node_parent_edge_label_end;
		while( j <= parent_edge_label_end )
		{
			if(sequence[i] != sequence[j])
			{
				mismatch = true;
				break;
			}
			i++;
			j++;
		}
		if(mismatch) //create the internal node and the leaf node that result from dividing the edge label
		{
			Node* new_internal_node = new Node(number_nodes++, alphabet);  // fix internal node     
			new_internal_node->node_parent_edge_label_start = initial_head;
			new_internal_node->node_parent_edge_label_end = i-1;
			new_internal_node->node_parent_ptr = hop_node;
			new_internal_node->node_children_ptrs[sequence[j]] = child_node;
			new_internal_node->node_string_depth = new_internal_node->node_parent_ptr->node_string_depth + (new_internal_node->node_parent_edge_label_end - new_internal_node->node_parent_edge_label_start + 1);            
			hop_node->node_children_ptrs[sequence[initial_head]] = new_internal_node; // fix parent node      
			child_node->node_parent_ptr = new_internal_node; // fix existing child node
			child_node->node_parent_edge_label_start = j;      
			// create new leaf
			Node* leaf_node = new Node(number_nodes++, alphabet); // fix new child node      
			leaf_node->node_parent_edge_label_start = i;
			leaf_node->node_parent_edge_label_end = tail;
			leaf_node->node_parent_ptr = new_internal_node;
			leaf_node->node_string_depth = leaf_node->node_parent_ptr->node_string_depth + ( leaf_node->node_parent_edge_label_end - leaf_node->node_parent_edge_label_start + 1);
			leaf_node->node_suffix_number = number_suffix++;			
			new_internal_node->node_children_ptrs[sequence[i]] = leaf_node; // add to internal node's child list			
			return leaf_node;
		}  
	}
}


/* ************************************************************************************************************************* */

//Procedure that finds the deepest internal node applying a DFS over the internal nodes of the suffix tree
void SuffixTree::findDeepestInternalNode(Node* node)
{	
	if(node->node_suffix_number == -1)
	{
		suffix_tree_depth += node->node_string_depth;
		if(node->node_string_depth > string_length_deepest_internal_node)
		{
			string_length_deepest_internal_node = node->node_string_depth;			
			longest_exact_matching_repeat_end = node->node_parent_edge_label_end;
			longest_exact_matching_repeat_start = node->node_parent_edge_label_end - string_length_deepest_internal_node + 1;
			deepest_internal_node = node;
		}
	}
	for(int i = 0; i < alphabet.length(); i++)
	{
		if(node->node_children_ptrs[alphabet[i]] != 0)
		{
			findDeepestInternalNode(node->node_children_ptrs[alphabet[i]]);
		}
	}  
}


/* ************************************************************************************************************************* */

//Print the Suffix Tree to the screen one level at the time
void SuffixTree::printNodesLevel(Node* node, int level)
{
	for(int current_level = 0; current_level < level; current_level++)
	{
		cout << "\t";
	}
	cout << ">---[" << node->node_ID << " ";
	if (node->node_ID == 0)
	{
		cout << "Root";
	}
	else
	{    
		for(int i = node->node_parent_edge_label_start; i <= node->node_parent_edge_label_end; i++)
		{
			cout << sequence[i];
		}		
	}
	if(node->node_suffix_number > 0)
	{		
		cout << " (" << node->node_suffix_number <<")]" << endl;
	}
	else
	{
		cout << "]" << endl;
	}		 
}


/* ************************************************************************************************************************* */


void SuffixTree::printSuffixTree(Node* node, int level)
{
	printNodesLevel(node, level);  
	for(int i = 0; i < alphabet.length(); i++)
	{
		if(node->node_children_ptrs[alphabet[i]] != 0)
		{
			printSuffixTree(node->node_children_ptrs[alphabet[i]], level + 1);
		}
	}
}


/* ************************************************************************************************************************* */

//DFS implementation
void SuffixTree::DFS(Node* node)
{	
	traversing_string << node->printNodeStringDepth();
	for(int i = 0; i < alphabet.length(); i++)
	{
		if(node->node_children_ptrs[alphabet[i]] != 0)
		{
			DFS(node->node_children_ptrs[alphabet[i]]);
		}
	}  	
}


/* ************************************************************************************************************************* */

//DFS implementation: left child - right child - parent
void SuffixTree::postOrder(Node* node)
{
	for(int i = 0; i < alphabet.length(); i++)
	{
		if(node->node_children_ptrs[alphabet[i]] != 0)
		{
			postOrder(node->node_children_ptrs[alphabet[i]]);
		}
	}
	traversing_string << node->printNodeStringDepth();
}


/* ************************************************************************************************************************* */

void SuffixTree::BWT(Node* node)
{		
	for(int i=0; i < alphabet.length(); i++)
	{
		if(node->node_children_ptrs[alphabet[i]] != 0)
		{			
			if(node->node_children_ptrs[alphabet[i]]->node_suffix_number > 0)
			{
				traversing_string << node->node_children_ptrs[alphabet[i]]->printNodeSuffixNumber();				
			}
			else
			{
				BWT(node->node_children_ptrs[alphabet[i]]);		
			}
		}	
		
	}  	
}

/* ************************************************************************************************************************* */

void SuffixTree::printNodeChildren(Node* node)
{		
  	
	for(int i=0; i < alphabet.length(); i++)
	{
		if(node->node_children_ptrs[alphabet[i]] != 0)
		{
			cout << node->node_children_ptrs[alphabet[i]]->printChild();
		}
	}
}

/* ************************************************************************************************************************* */

void SuffixTree::displayRandomNodeChildren()
{		
  /*srand (time(NULL)); //initialize random seed  
  int random_node = rand() % number_nodes + 1;  //generate random number between 1 and 10*/	
	int i=0;
	Node* node;
	cout << endl << endl << "::: Children List from 2 Nodes in the Suffix Tree :::" << endl << string(55,'-') << endl;

	while(i<2)
	{
		if (i == 0)
		{	node = root;			
			cout << "Node ID:" << node->node_ID << "[Root]" << endl;
		}
		else
		{
			node = deepest_internal_node;
			cout << endl << "Node ID:" << node->node_ID << "[Deepest Internal Node]" << endl;
		}
		printNodeChildren(node);		
		i++;	
	}
}

/* ************************************************************************************************************************* */

//Deallocate the memory occupied by the suffix tree
void SuffixTree::deleteSuffixTree(Node* node)
{
	for(int i = 0; i < alphabet.length(); i++)
	{
		if(node->node_children_ptrs[alphabet[i]] != 0)
		{
			delete node->node_children_ptrs[alphabet[i]];
		}
	}
	traversing_string << node->printNodeStringDepth();
}



/* ************************************************************************************************************************* */

void SuffixTree::DFS()
{
	DFS(root);
}


/* ************************************************************************************************************************* */

void SuffixTree::postOrder()
{
	postOrder(root);
}


/* ************************************************************************************************************************* */

void SuffixTree::BWT()
{
	BWT(root);
}


/* ************************************************************************************************************************* */

void SuffixTree::deleteSuffixTree()
{
	deleteSuffixTree(root);
}


/* ************************************************************************************************************************* */

void SuffixTree::setTraversingString()
{
	traversing_string.str("");
}


/* ************************************************************************************************************************* */

string SuffixTree::getTraversingString()
{
	return traversing_string.str();
}


/* ************************************************************************************************************************* */


void SuffixTree::setAlphabetBWT()
{
	alphabet = "$" + alphabet.substr(0,alphabet.length()-1);
}


/* ************************************************************************************************************************* */


void SuffixTree::setAlphabet(string input_alphabet)
{
	alphabet = input_alphabet + "$";
}


/* ************************************************************************************************************************* */

void SuffixTree::generateReportOutputFile(string text)
{	
	ofstream outfile;
    outfile.open(file_name, ios::app );
	outfile << text;
	outfile.close();
	cout << text;	
}
