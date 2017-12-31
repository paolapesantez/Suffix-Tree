#include "SuffixTreeFiles.h"
#include "SuffixTree.h"



//Function taht allows to check whether the option selected from the menu is accurate
bool readInt(string text, int &output)
{
	bool result = false;
	try
	{
		output = stoi(text);
		result = true;
	}
	catch (...)
	{
		cout << endl << "::: Invalid Input. Please try again :::" << endl;
		system("pause");									
		system("cls");
		output = -2;
	}
	return result;
}
 

/* ************************************************************************************************************************* */
//User friendly interface so the user can construct the suffix tree from several input files 


int main()
{	
	try
	{				
		string user_input;
		int user_int =0;		
		while (user_int != 2)
		{	//Principal Menu	
			cout << endl <<"::: Menu Suffix Trees:::" << endl << string(53,'-') << endl;
			cout << "1. Create Suffix Tree\n";
			cout << "2. Exit program\n" << endl;
			cout << "Selection: ";
			getline(cin, user_input);
			readInt(user_input,user_int);	
				if(user_int == 1)
				{
					SuffixTreeFiles suffix_files; //create an instance of the class SuffixFiles
					SuffixTree  suffix_tree;	//create an instance of the class SuffixTree
					string input_file_name;
					string alphabet_file_name;
					cout << endl << "Insert alphabet file name: ";
					getline(cin, alphabet_file_name);														
					int pass = suffix_files.readFileAlphabet(alphabet_file_name);//read alphabet file
					if(pass == 0)//correct
					{
						cout << endl << "Insert input file name: ";
						getline(cin, input_file_name);																			
						pass = suffix_files.readFileInputSequence(input_file_name);//read input file
						if(pass==0)//correct
						{
							clock_t t1,t2;    
							float diff=0;
							float seconds = 0;
							system("cls");
							suffix_files.verifySequence(); //input sequence should have just characters from the alphabet																			
							suffix_files.printDataFromFiles();	//print the data from files							
							t1=clock();
							suffix_tree.createSuffixTree(suffix_files.getSequence(),suffix_files.getAlphabet());	
							t2=clock();
							diff = ((float)t2-(float)t1);
							seconds = diff / CLOCKS_PER_SEC;
							cout<<endl<<"Running Time: " << seconds<<endl<<endl;							
							suffix_tree.printReport(input_file_name);
							system("pause");		
							system("cls");
							while (user_int != 5)
							{//Menu of different forms to display the information in the suffix tree
								cout << endl <<"::: Menu Display Suffix Trees:::" << endl << string(53,'-') << endl;
								cout << "1. Display random nodes\n";
								cout << "2. Display Suffix Tree Depth First Search\n";
								cout << "3. Display Suffix Tree using Post-order transversal\n";
								cout << "4. Display RTW\n";
								cout << "5. Exit display\n" << endl;
								cout << "Selection: ";
								getline(cin, user_input);
								bool band = readInt(user_input,user_int);	
								switch(user_int)
								{
									case 1:
										suffix_tree.displayRandomNodeChildren();
										break;
									case 2:														
										suffix_tree.DFS();
										suffix_files.generatingOutputFiles(suffix_tree.getTraversingString(),1);
										suffix_tree.setTraversingString();									
										break;
									case 3:										
										suffix_tree.postOrder();
										suffix_files.generatingOutputFiles(suffix_tree.getTraversingString(),2);
										suffix_tree.setTraversingString();							
										break;
									case 4:										
										suffix_tree.BWT();
										suffix_files.generatingOutputFiles(suffix_tree.getTraversingString(),3);
										suffix_tree.setTraversingString();							
										break;									
								}	
								if((user_int != 5) && (band==true))
								{
									system("pause");									
									system("cls");
								}
								if(user_int == 5)
								{
									suffix_tree.setAlphabet(suffix_files.getAlphabet());
									suffix_tree.deleteSuffixTree();								
								}
							}							
							system("cls");
						}
						else
						{
							cerr << "::: Invalid input sequence file name :::" << endl;
							system("pause");		
							system("cls");
						}
					}
					else
					{
						cerr << "::: Invalid alphabet file name :::" << endl;
						system("pause");		
						system("cls");
					}					
					
				}																											
		}		
	}		
	catch (std::runtime_error &e)
	{
		std::cout<<e.what()<<std::endl;
	}
	catch(...)
	{
		std::cout<<"Unknown error Main"<<std::endl;
	}
    
		cout << endl << "::: Suffix Tree Construction has finished :::"<< endl;
		system("pause");
		return 0;	
}	

