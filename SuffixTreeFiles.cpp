#include "SuffixTreeFiles.h"
#include "StringSplitter.h"


//Function that deletes space from a whole string
string trim(const string &s)
{
  int last = s.size() - 1;
  while (last >= 0 && s[last] == ' ')
    --last;
  return s.substr(0, last + 1);
}


/* ****************************************************************************************************************** */
//Function that returns the positionof the nth occurrence of the specified character c inside a string s
int nthOccurrence(string str, string c, int n)
{
    int pos = str.find(c, 0);
    while (n-- >1 && pos != string::npos && pos<str.length()-1)
        pos = str.find(c, pos+1);
    return pos;
}

/* ****************************************************************************************************************** */

//Class constructor 
SuffixTreeFiles::SuffixTreeFiles()
{	
	sequence = "";
	alphabet = "";
}


/* ****************************************************************************************************************** */

//Class destructor
SuffixTreeFiles::~SuffixTreeFiles(){}


/* ****************************************************************************************************************** */

//Procedure that opens and read the file that contains the alphabet
int SuffixTreeFiles::readFileAlphabet(string alphabet_file_name)
{		
	
	ifstream input_file(alphabet_file_name);			
	bool is_good = true;
	bool band = true;
	try
	{
		if(input_file.is_open() == false)				//If the file hasn't been found
		{		
			return -1;									//no file has been found. End the program.
			is_good = false;
		}	
		if(is_good)									//If the file has been found
		{
			while((input_file.good())&&(band == true))				//read line by line
			{
				int items_found = 0;
				string str_line = "";
				getline(input_file,str_line);
				string* pieces = StringSplitter::split(str_line," ",items_found); //StringSplitter is a class that parses a line according a delimiter, dividing the line in its different parts.												
				for(int i=0; i<items_found;i++)
					alphabet = alphabet + pieces[i];
				band = false;
				delete[] pieces;
			}			
			input_file.close();			//close the file from which we were reading
			return 0;					//reading completed succesfully
		}
	}
	catch (runtime_error &e)
	{
		cout<<e.what()<< " read file alphabet" <<endl;
	}
	catch(...)
	{
		cout<<"Unknown error in readFileAlphabet"<<endl;
	}
}

/* ****************************************************************************************************************** */

//Procedure that opens and read the file that contains the input sequence that we want to create the suffix tree for
int SuffixTreeFiles::readFileInputSequence(string input_file_name)
{	
	ifstream input_file(input_file_name);
	bool is_good = true;
	try
	{
		if(input_file.is_open() == false)		//If the file hasn't been found
		{			
			return -1;							//no file has been found. End the program.
			is_good = false;
		}	
		if(is_good)								//If the file has been found
		{
			stringstream s;				//used to store the complete sequence that in the file occupies several lines			
			while (input_file.good())			//read line by line
			{
				int items_found = 0;				
				string str_line = "";
				getline(input_file,str_line);
				string *pieces = StringSplitter::split(str_line," ",items_found); //StringSplitter is a class that parses a line according a delimiter, dividing the line in its different parts.
				if ((items_found > 0) && (pieces[0].length()>0)) // When we find an empty line we know the next one will have the start of the next sequence
				{
					if (pieces[0][0] == '>')	//start of a sequence. Parsing the name of the sequence
					{
						s.str("");
						if(isalpha(pieces[0][1]))
						{
							s << pieces[0].substr(1,pieces[0].length()) << "/";//extract the name of the sequence and append / to it so we can separate the name from the sequence itself later
						}
						else
						{
							s << "s1" << "/"; //in case the name of the sequence doesn't have the default format we will give it the right name
						}								
					}	
					else //if we are actually reading the lines that represent the sequence content itself
					{
						s << trim(str_line);	//append the different lines in just one
					}
			   }				
			}
			sequence = s.str();			//we have finished reading the file so the last sequence also needs to be stored
			input_file.close();			//close the file from which we were reading
			createOutputFiles(input_file_name);
			return 0;					//reading completed succesfully
		}
	}
	catch (runtime_error &e)
	{
		cout<<e.what()<< " read file sequence" <<endl;
	}
	catch(...)
	{
		cout<<"Unknown error in readFileInputSequence"<<endl;
	}
}

/* ****************************************************************************************************************** */

//Printing to the screen the data that has been read for the files
void SuffixTreeFiles::printDataFromFiles()
{
	try
	{		
			int pos = sequence.find_first_of("/");
			int sequence_length = sequence.length()-(pos+1);
			cout << endl << endl;
			cout << "   ::: Suffix Tree Construction :::" << endl << string(40,'-') << endl << endl;			
			cout << "Sequence: " << "\""<< sequence.substr(0,pos) << "\",length = " << sequence_length << endl << endl;
			cout << "Alphabet: " << alphabet << endl << endl;
	}
	catch (runtime_error &e)
	{
		cout<<e.what()<< " printFile " <<endl;
	}
	catch(...)
	{
		cout<<"Unknown error in printFile"<<endl;
	}
}

/* ****************************************************************************************************************** */

//Procedure that creates the output files obtained from the Suffix Tree
void SuffixTreeFiles::createOutputFiles(string input_file_name) 
{
	string file_name;	
	int pos = input_file_name.find_first_of(".");
	file_name = (input_file_name.substr(0,pos)).append("_DFS.txt");
	output_file_name[1] = file_name;
	ofstream outfile1(file_name);  
	outfile1 << "::: Depth First Search Traversing :::" << endl << string(60,'-') << endl;
	outfile1 << "Each tuple is given by the format: Node ID:String Depth" << endl << endl;
	outfile1.close();

	file_name = "";
	file_name = (input_file_name.substr(0,pos)).append("_Post-Order.txt");
	output_file_name[2] = file_name;
	ofstream outfile2(file_name);  
	outfile2 << "::: Post-Order Traversing :::"  << endl << string(60,'-') << endl;
	outfile2 << "Each tuple is given by the format: Node ID:String Depth" << endl << endl;
	outfile2.close();

	file_name = "";
	file_name = (input_file_name.substr(0,pos)).append("_BWT.txt");
	output_file_name[3] = file_name;
	ofstream outfile3(file_name);  	
	outfile3 << "::: BWT Output :::"  << endl << string(25,'-') << endl << endl;
	outfile3.close();
}
					

/* ****************************************************************************************************************** */

//This procedure verifies that all the characters of the given string belong to the alphabet also given
void SuffixTreeFiles::verifySequence()
{		
	int pos = sequence.find_first_of("/");		
	string s = sequence.substr(pos+1,sequence.length());
	for(int i=0;i<s.length();i++)
	{
		if(alphabet.find(s[i]) == std::string::npos) 
		{
			cout << endl << "::: Sequence characters don't agree with the alphabet that is being used :::" << endl;
			cout << "::: Character: " <<s[i] <<" at position: " << i+1<< ":::" <<endl;
			exit(0);
			system("Pause");			
		}		
	}	
}

/* ****************************************************************************************************************** */

string SuffixTreeFiles::getSequence() const
{
	return sequence;
}

/* ****************************************************************************************************************** */

string SuffixTreeFiles::getAlphabet() const
{
	return alphabet;
}


/* ************************************************************************************************************************* */

void SuffixTreeFiles::generatingOutputFiles(string traversing, int option)
{
	bool band = true;
	switch(option)
	{
		case 1:
			cout << endl << endl << "::: Depth First Search Traversing :::" << endl << string(60,'-') << endl;
			cout << "Each tuple is given by the format: Node ID:String Depth" << endl << endl;
			break;
		case 2:
			cout << endl << endl << "::: Post-Order Traversing :::"  << endl << string(60,'-') << endl;
			cout << "Each tuple is given by the format: Node ID:String Depth" << endl << endl;
			break;
		case 3:
			cout << endl << endl << "::: BWT Output :::"  << endl << string(25,'-') << endl;
			band = false;
			break;	
	}
	if(band)
	{
		ofstream outfile;
		outfile.open(output_file_name[option], ios::app );		
		string line = traversing;		
		int pos = 0;
		while(pos<traversing.length())
		{
			int temp = nthOccurrence(line," ",NUMBER_NODES_DISPLAY);
			line = traversing.substr(pos,temp);			
			cout << line << endl;
			outfile << line << endl;
			pos = pos+temp+1;
			line = traversing.substr(pos,traversing.length());
		}	
		line = traversing.substr(pos,traversing.length());
	}
	else
	{
		int pos = sequence.find_first_of("/");		
		string s = sequence.substr(pos+1,sequence.length());	
		ofstream outfile;
		outfile.open(output_file_name[option], ios::app );
		int number_items;
		int index = 0;
		string* pieces = StringSplitter::split(traversing," ",number_items);
		for(int i=0;i<number_items-1;i++)
		{
			index = stoi(pieces[i]);
			if(index>1)
			{
				cout << s[index-2] << endl;
				outfile << s[index-2] << endl;
			}
			else
			{
				cout << "$" << endl;
				outfile << "$" << endl;
			}
		}		
	}
}


