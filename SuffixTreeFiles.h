#ifndef SUFFIXTREEFILES_H_INCLUDED
#define SUFFIXTREEFILES_H_INCLUDED

#include "Header.h"

#define NUMBER_PARAMETERS 3 //number the parameters defined to be used when executing the program
#define NUMBER_NODES_DISPLAY 10

class SuffixTreeFiles
{
	private:
		string sequence;
		string alphabet;			
		map<int,string> output_file_name;


	public:
		SuffixTreeFiles();
		~SuffixTreeFiles();
		int readFileAlphabet(string alphabet_file_name);
		int readFileInputSequence(string input_file_name);
		void createOutputFiles(string input_file_name) ;
		void printDataFromFiles();
		void verifySequence();
		string getSequence() const;
		string getAlphabet() const;
		void generatingOutputFiles(string traversing, int option);

		
		
};
#endif