//Eric Minh Hanh Nguyen
//This is my current compiler project with the lexical analysis finished

//These includes are all I need to make this work fortunately
#include <iostream>
#include <string>
#include <fstream>
#include <regex> 
#include <map>
#include <stack>
#include <queue>

using namespace std;

//This is for the symbol table with the value variable being there to hold arbitrary values
struct symbolT
{
	string	iDen;
	int		memL;
	string	typeN;
	int		value;
};

//This function takes in an int and a string before processing them through the switch statement
//to produce an output as well as return a token.
string Lexor(int stateInput, string lexem)
{
	string token;

	switch (stateInput)
	{
	case 0:
		cout << "KEYWORD   " << "     " << "=" << "     " << lexem << endl;
		token = "KEYWORD";
		break;
	case 1:
		cout << "IDENTIFIER" << "     " << "=" << "     " << lexem << endl;
		token = "IDENTIFIER";
		break;
	case 2:
		cout << "SEPARATOR " << "     " << "=" << "     " << lexem << endl;
		token = "SEPARATOR";
		break;
	case 3:
		cout << "OPERATOR  " << "     " << "=" << "     " << lexem << endl;
		token = "OPERATOR";
		break;
	case 4:
		cout << "INTEGER   " << "     " << "=" << "     " << lexem << endl;
		token = "INTEGER";
		break;
	case 5:
		cout << "REAL      " << "     " << "=" << "     " << lexem << endl;
		token = "REAL";
		break;
	case 6:
		cout << "UNKNOWN   " << "     " << "=" << "     " << lexem << endl;
		token = "UNKNOWN";
		break;
	}

	return token;
}

//The next following functions all serve to check the lexem and confirm their token before returning
//a corresponding input for the lexor and moving the state to the final one, calling said lexor

//This function checks if the lexem is a keyword and attempts to save some time by moving the state to the
//integer and real checks should it be a vaild numerial string
int checkKeyword(string lexem)
{
	int		stateConfirm = 0;
	regex	checkValid("^[a-zA-Z]{1}[a-zA-Z0-9$]*$");
	regex	checkNum("^[0-9]*[.]?[0-9]+$");
	string	keyList[18] = { "int", "float", "bool", "if", "else", "then", "endif", "while", "whileend", "do", "doend", "for",
	"forend", "input", "output", "and", "or", "function" };

	if (regex_match(lexem, checkValid) == true)
	{
		for (int i = 0; i < (sizeof(keyList) / sizeof(keyList[0])); i++)
		{
			if (lexem == keyList[i])
			{
				stateConfirm = 7;
			}
	
		}
	}
	
	if(regex_match(lexem, checkNum))
	{
		stateConfirm = 4;
	}

	if (stateConfirm == 0)
	{
		stateConfirm++;
	}

	return stateConfirm;
}

int checkIdentifier(string lexem)
{
	int		stateConfirm = 0;
	regex	checkValid("^[a-zA-Z]{1}[a-zA-Z0-9$]*$");

	if (regex_match(lexem, checkValid) == true)
	{
		stateConfirm = 7;
	}
	else
	{
		stateConfirm = 2;
	}

	return stateConfirm;
}

//This function checks if the lexem is a seperator
int checkSeparator(string lexem)
{
	int		stateConfirm = 0;
	string	keyList[11] = { "'", "(", ")", "{", "}", "[", "]", ",", ".", ":", ";" };

	for (int i = 0; i < (sizeof(keyList) / sizeof(keyList[0])); i++)
	{
		if (lexem == keyList[i])
		{
			stateConfirm = 7;
		}
	}

	if (stateConfirm == 0)
	{
		stateConfirm = 3;
	}

	return stateConfirm;
}

//This function checks if the lexem is an operator
int checkOperator(string lexem)
{
	int		stateConfirm = 0;
	string	keyList[8] = { "*", "+", "-", "=", "/", ">", "<", "%" };

	for (int i = 0; i < (sizeof(keyList) / sizeof(keyList[0])); i++)
	{
		if (lexem == keyList[i])
		{
			stateConfirm = 7;
		}
	}

	if (stateConfirm == 0)
	{
		stateConfirm = 4;
	}

	return stateConfirm;
}

//This function checks if the lexem is a integer
int checkInteger(string lexem)
{
	int		stateConfirm = 0;
	regex	checkInt("^[0-9]+$");

	if (regex_match(lexem, checkInt) == true)
	{
		stateConfirm = 7;
	}
	else
	{
		stateConfirm = 5;
	}

	return stateConfirm;
}

//This function checks if the lexem is a real
//The else returns a value that progress into the unknown state which doesn't have a regex + function due to complication
int checkReal(string lexem)
{
	int		stateConfirm = 0;
	regex	checkReal("^[0-9]*[.]{1}[0-9]+$");

	if (regex_match(lexem, checkReal) == true)
	{
		stateConfirm = 7;
	}
	else
	{
		stateConfirm = 6;
	}

	return stateConfirm;
}

int main()
{
	//These variables are the necessary ingredients for this main()
	//The two lists are currently set to 100 valid lexems but can be increased accordingly
	int		i = 0, j = 0, sandwhichGate = 0, combinationGate = 0, statePosition = 0, r = 0, c = 0, stateFinal = 0;
	string	lexemList[100], tokenList[100];
	int		stateTable[1][8] = { 0, 1, 2, 3, 4, 5, 6, 7};

	//This mess here is the finicky, touchy list of regex patterns. 
	smatch	match;
	regex	commentStart("^[!]{1}");
	regex	commentEnd("[!]{1}$");
	regex	legalWord("^[a-zA-Z]{1}[a-zA-Z0-9$]*$");
	regex	legalWordL("[a-zA-Z]{1}[a-zA-Z0-9$]*$");
	regex	legalWordR("^[a-zA-Z]{1}[a-zA-Z0-9$]*");
	regex	legalSepL("^[(\\{\\[,:;]{1}");
	regex	legalSepR("[)\\}\\],:;]{1}$");
	regex	legalOp("^[*+\\-=/><%]{1}$");
	regex	legalSepOp("[(\\{\\[)\\}\\],:;*+\\-=/><%]{1}");
	regex	legalNum("^[0-9]*[.]?[0-9]+$");
	regex	legalNumL("[0-9]*[.]?[0-9]+$");
	regex	legalNumR("^[0-9]*[.]?[0-9]+");
	regex	legalSandwich("[a-zA-Z]{1}[a-zA-Z0-9$]*");

	//These are simple file working variables
	fstream		file;					
	string		word, filename, custom;		
	int			choice;

	//A basic interface possessing two given test cases and one that creates a test case that the user can use to have analyzed
	cout << endl << "Welcome to my finished project, please choose a test file to compile or input your own." << endl << endl;
	cout << "Please take it easy on the custom test case as every combination of code input has not been done yet for the sake of time." << endl << endl;
	cout << "1: Add test case" << endl;
	cout << "2: Subtract test case" << endl;
	cout << "3: Multiply test case" << endl;
	cout << "4: Divide test case" << endl;
	cout << "Please enter your choice: ";
	cin >> choice;
	cout << endl << endl;

	if (choice == 1)
	{
		filename = "testCaseA.txt";
		file.open(filename.c_str());
	}
	else if (choice == 2)
	{
		filename = "testCaseS.txt";
		file.open(filename.c_str());
	}
	else if (choice == 3)
	{
		filename = "testCaseM.txt";
		file.open(filename.c_str());
	}
	else if (choice == 4)
	{
		filename = "testCaseD.txt";
		file.open(filename.c_str());
	}
	else
	{
		filename = "testCaseNoEscape.txt";
		file.open(filename.c_str());
	}

	//My thought process on all of this is simply making a processing filter that finds and puts all valid code 
	//into an array word by word instead character by character because it was much simplier for me to implement

	//This will loop through until all words of code are picked up
	while (file >> word)
	{
		//This regex search specifically skips over comments including the comment symbols
		if (regex_match(word, commentStart) == true)
		{
			file >> word;
			while (regex_search(word, match, commentEnd) == false)
			{
				file >> word;
			}
			file >> word;
		}
		

		if (regex_search(word, match, legalWord) == true)
		{
			for (auto x : match)
			{
				lexemList[i] = x;
				i++;
				j++;
				combinationGate++;
			}
		}

		//The next two paired regex searches are used to scan and breaking up these two cases: (code or code)
		if (regex_search(word, match, legalSepL) == true)
		{
			for (auto x : match)
			{
				lexemList[i] = x;
				i++;
				j++;
				sandwhichGate++;
				combinationGate++;
			}

			if (regex_search(word, match, legalWordL) == true)
			{
				for (auto x : match)
				{
					lexemList[i] = x;
					i++;
					j++;
				}
			}
		}

		

		if (regex_search(word, match, legalSepR) == true)
		{
			for (auto x : match)
			{
				lexemList[i] = x;
				i++;
				j++;
				sandwhichGate++;
				combinationGate++;

			}

			
			if (regex_search(word, match, legalWordR) == true)
			{
				for (auto x : match)
				{
					lexemList[i] = lexemList[i-1];
					lexemList[i-1] = x;
					i++;
					j++;
				}
			}			
		}

		if (regex_search(word, match, legalOp) == true)
		{
			for (auto x : match)
			{
				lexemList[i] = x;
				i++;
				j++;
				combinationGate++;
			}
		}

		if (regex_search(word, match, legalNum) == true)
		{
			for (auto x : match)
			{
				lexemList[i] = x;
				i++;
				j++;
				combinationGate++;
			}
		}

		//This specifically scans for single word of code in between two valid symbols: (code)
		if (sandwhichGate == 2)
		{
			if (regex_search(word, match, legalSandwich) == true)
			{
				for (auto x : match)
				{
					lexemList[i] = lexemList[i - 1];
					lexemList[i - 1] = x;
					i++;
					j++;
				}
			}
		}

		//This one is looking for code(code with the ( being any sort of valid symbol
		if (combinationGate == 0)
		{
			if (regex_search(word, match, legalWordR) == true)
			{
				for (auto x : match)
				{
					lexemList[i] = x;
					i++;
					j++;
				}
			}
			else if (regex_search(word, match, legalNumR) == true)
			{
				for (auto x : match)
				{
					lexemList[i] = x;
					i++;
					j++;
				}
			}

			if (regex_search(word, match, legalSepOp) == true)
			{
				for (auto x : match)
				{
					lexemList[i] = x;
					i++;
					j++;
				}
			}

			if (regex_search(word, match, legalWordL) == true)
			{
				for (auto x : match)
				{
					lexemList[i] = x;
					i++;
					j++;
				}
			}
			else if (regex_search(word, match, legalNumL) == true)
			{
				for (auto x : match)
				{
					lexemList[i] = x;
					i++;
					j++;
				}
			}
		}

		//Have to reset the gates every loop through
		sandwhichGate = 0;
		combinationGate = 0;
	}
	file.close();

	//This for loop goes through each word in the array while not going beyond into the empty space of the array
	for (i = 0; i < j; i++)
	{
		c = 0;

		//This while loop is the lexem running through the state table
		while (statePosition != 7)
		{
			//This is the current state
			statePosition = stateTable[r][c];

			//The switch statement calls functions based on state position who then returns values that state a change
			//and those returning a 7 have confirmed the lexem as that type of toxen and progresses to the final state
			//while giving an input for the lexor
			switch (statePosition)
			{
				case 0:
					c = checkKeyword(lexemList[i]);
					if (c == 7)
					{
						stateFinal = 0;
					}
					break;
				case 1:
					c = checkIdentifier(lexemList[i]);
					if (c == 7)
					{
						stateFinal = 1;
					}
					break;
				case 2:
					c = checkSeparator(lexemList[i]);
					if (c == 7)
					{
						stateFinal = 2;
					}
					break;
				case 3:
					c = checkOperator(lexemList[i]);
					if (c == 7)
					{
						stateFinal = 3;
					}
					break;
				case 4:
					c = checkInteger(lexemList[i]);
					if (c == 7)
					{
						stateFinal = 4;
					}
					break;
				case 5:
					c = checkReal(lexemList[i]);
					if (c == 7)
					{
						stateFinal = 5;
					}
					break;
				case 6: //This is the implemented unknown case for the sake of having something for it despite no regex or function
					c = 7;
					stateFinal = 6;
					break;
				case 7:
					tokenList[i] = Lexor(stateFinal, lexemList[i]);
					break;
			}
		}
		statePosition = 0;
	}
	cout << endl << endl;

	//This preps the lexemList for the parsing table and the addition in the token slot is just to fill it up so it
	//...is not an empty slot
	lexemList[j] = "$";
	tokenList[j] = "ENDPARSER";

	//This chunk of code here just outputs the result of the analysis into an output txt file but the spacing is not
	//...perfect.
	file.open("outputLexical.txt", ios::out | ios::trunc);
	file << "TOKENS	" << "          " << "		Lexemes" << endl;
	for ( i = 0; i < j; i++)
	{
		file << tokenList[i] << "	     " << "	=" << "		" << lexemList[i] << endl;
	}
	file.close();

	//There is an error check for the choice input if you wish to see that
	//Assignment 2 Note: Checking will default the parser.
	//--------------------------------------------------------------------------------------------------------------------------
	//This is the beginning of the parser where we create the map and stack to use later.
	map< string, map<string, int> > parseTable;
	stack<string> parser;

	//These are the new variables for the assembly addition to the parser 
	stack<int> asmStack;
	queue<string> asmQueue;

	symbolT symTable[10];

	//Just to make sure there are no unexpected errors, I've inputted a placeholder string to be written over in the comparison down below
	symTable[0].iDen = "Placeholder";

	int	symFlag = 0, assFlag = 0, memLocation = 5000, symCounter = 0, oper = 0, noRepeat = 0;

	//We set up the stack for use along with resetting the i to use for element access
	i = 0;
	parser.push("$");
	parser.push("S");

	//We set up the table definitions with the non-terminals on the left/rows and terminals as the top/columns
	//The assigned value is the corresponding rule that will be utilized
	parseTable["S"]["ID"] = 1;
	parseTable["A"]["ID"] = 2;
	parseTable["E"]["ID"] = 3;
	parseTable["E"]["("] = 3;
	parseTable["E'"]["+"] = 4;
	parseTable["E'"]["-"] = 5;
	parseTable["E'"][")"] = 6;
	parseTable["E'"]["$"] = 6;
	parseTable["T"]["ID"] = 7;
	parseTable["T"]["("] = 7;
	parseTable["T'"]["+"] = 6;
	parseTable["T'"]["-"] = 6;
	parseTable["T'"]["*"] = 8;
	parseTable["T'"]["/"] = 9;
	parseTable["T'"][")"] = 6;
	parseTable["T'"]["$"] = 6;
	parseTable["F"]["ID"] = 10;
	parseTable["F"]["("] = 11;

	//While the parser runs, we have a file open to print output to the outputSyntax.txt
	//I also made it so console receives an output as well so there are two output lines
	file.open("outputSyntax.txt", ios::out | ios::trunc);
	cout << "Token: " << lexemList[i] << "   " << "Lexeme: " << tokenList[i] << endl;
	file << "Token: " << lexemList[i] << "   " << "Lexeme: " << tokenList[i] << endl;

	//This parser is a basic one which uses the combined assignment and arithmetic rules
	while (!parser.empty())
	{	
		if (lexemList[i] == parser.top())
		{
			if (tokenList[i] == "IDENTIFIER")
			{
				//This section creates the Symbol Table entries
				for (int j = 0; j < 5; j++)
				{
					if (lexemList[i] == symTable[j].iDen)
					{
						symFlag++;
					}
				}
				if (symFlag == 0)
				{
					symTable[symCounter].iDen = lexemList[i];
					symTable[symCounter].memL = memLocation;
					symTable[symCounter].typeN = "INTEGER"; //Manually inputting this in since we done have declarative rules implemented
					symCounter++;
					memLocation++;
				}
			}
			symFlag--; //Have to reset the flag afterwards

			//This flag triggers the actual assembly section of the code
			if (lexemList[i] == "=")
			{
				assFlag++;
			}


			if (assFlag != 0)
			{
				//To streamline and limit bloating the code even more, after the first identifier is read, it triggers a flag to avoid
				//reading another identifier with the varying operator choices leading to the rest of the assembly production.
				if (tokenList[i] == "IDENTIFIER")
				{
					if (noRepeat == 0)
					{
						asmStack.push(symTable[0].memL);
						asmQueue.push("PUSHM 5000");
						noRepeat++;
					}
				}
				else if (lexemList[i] == "+")
				{
					asmStack.push(symTable[0].memL);
					asmStack.pop();
					asmStack.pop();
					oper = symTable[0].value + symTable[0].value;
					asmStack.push(oper);
					asmStack.pop();
					symTable[0].value = oper;

					asmQueue.push("PUSHM 5000");
					asmQueue.push("ADD");
					asmQueue.push("POPM 5000");
				}
				else if (lexemList[i] == "-")
				{
					asmStack.push(symTable[0].memL);
					asmStack.pop();
					asmStack.pop();
					oper = symTable[0].value - symTable[0].value;
					asmStack.push(oper);
					asmStack.pop();
					symTable[0].value = oper;

					asmQueue.push("PUSHM 5000");
					asmQueue.push("SUB");
					asmQueue.push("POPM 5000");
				}
				else if (lexemList[i] == "*")
				{
					asmStack.push(symTable[0].memL);
					asmStack.pop();
					asmStack.pop();
					oper = symTable[0].value * symTable[0].value;
					asmStack.push(oper);
					asmStack.pop();
					symTable[0].value = oper;

					asmQueue.push("PUSHM 5000");
					asmQueue.push("MUL");
					asmQueue.push("POPM 5000");
				}
				else if (lexemList[i] == "/")
				{
					asmStack.push(symTable[0].memL);
					asmStack.pop();
					asmStack.pop();
					oper = symTable[0].value / symTable[0].value;
					asmStack.push(oper);
					asmStack.pop();
					symTable[0].value = oper;

					asmQueue.push("PUSHM 5000");
					asmQueue.push("DIV");
					asmQueue.push("POPM 5000");
				}
			}

			i++;
			parser.pop();
			//The additional if statement here is to prevent an empty output
			if (!parser.empty())
			{
				cout << "Token: " << lexemList[i] << "   " << "Lexeme: " << tokenList[i] << endl;
				file << "Token: " << lexemList[i] << "   " << "Lexeme: " << tokenList[i] << endl;
			}
		}
		else
		{	//Each case is a rule with the default being there to catch invalid lexems
			switch (parseTable[parser.top()][lexemList[i]])
			{
			case 1:
				parser.pop();
				parser.push("A");
				cout << "S -> A" << endl;
				file << "S -> A" << endl;
				break;

			case 2:
				parser.pop();
				parser.push("E");
				parser.push("=");
				parser.push("ID");
				cout << "A -> ID = E" << endl;
				file << "A -> ID = E" << endl;
				break;

			case 3:
				parser.pop();
				parser.push("E'");
				parser.push("T");
				cout << "E -> TE'" << endl;
				file << "E -> TE'" << endl;
				break;

			case 4:
				parser.pop();
				parser.push("E'");
				parser.push("T");
				parser.push("+");
				cout << "E' -> +TE'" << endl;
				file << "E' -> +TE'" << endl;
				break;

			case 5:
				parser.pop();
				parser.push("E'");
				parser.push("T");
				parser.push("-");
				cout << "E' -> -TE'" << endl;
				file << "E' -> -TE'" << endl;
				break;

			case 6:
				parser.pop();
				cout << "E' / T' -> epsilon" << endl;
				file << "E' / T' -> epsilon" << endl;
				break;

			case 7:
				parser.pop();
				parser.push("T'");
				parser.push("F");
				cout << "T -> FT'" << endl;
				file << "T -> FT'" << endl;
				break;

			case 8:
				parser.pop();
				parser.push("T'");
				parser.push("F");
				parser.push("*");
				cout << "T' -> *FT'" << endl;
				file << "T' -> *FT'" << endl;
				break;

			case 9:
				parser.pop();
				parser.push("T'");
				parser.push("F");
				parser.push("/");
				cout << "T' -> /FT'" << endl;
				file << "T' -> /FT'" << endl;
				break;

			case 10:
				parser.pop();
				parser.push("ID");
				cout << "F -> ID" << endl;
				file << "F -> ID" << endl;
				break;

			case 11:
				parser.pop();
				parser.push(")");
				parser.push("E");
				parser.push("(");
				cout << "F -> (E)" << endl;
				file << "F -> (E)" << endl;
				break;

			//I kept the error checking to a minimum for the sake of not making the code more confusing than it needs to be
			default:
				cout << "parsing table defaulted" << endl;
				file << "parsing table defaulted" << endl;
				system("pause");
				return 0;
				break;
			}
		}
	}
	file.close();

	//Here is all of the assembly output and symbol table output
	cout << endl;
	cout << "               ASSEMBLY CODE LISTING" << endl;
	while (!asmQueue.empty())
	{
		cout << asmQueue.front() << endl;
		asmQueue.pop();
	}
	cout << endl;

	cout << "               SYMBOL TABLE" << endl;
	cout << "Identifier     " << "     MemoryLocation     " << "     Type" << endl;
	cout << symTable[0].iDen << "                  " << symTable[0].memL << "                    " << symTable[0].typeN << endl;
	system("pause");
	return 0;

}
