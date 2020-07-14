#include <iostream>
#include <string>

using namespace std;

void intro() {
	cout << "This program searches a search string in a source string with the option of wildcards.\n" << endl
		 << "Example inputs with their meaning in parantheses:" << endl
		 << "1 thislectureisawesome (to enter a new source string)" << endl
		 << "2 lecture (to enter a new source string)" << endl
		 << "2 t?r? (to enter a source string with wildcards)  " << endl
		 << "3 (to exit the program)" << endl;
}

bool inputCheck(string inpChoice, string inpInput) {
	bool generalCheck = true, letterCheck = false;
	if (inpChoice == "1") { 
		for (int i=0; i<inpInput.length();i++) {
			if (!(inpInput.at(i) >= 'a' && inpInput.at(i) <= 'z')) {     // checking if every letter in the word is lowercase
				if (generalCheck) {cout << "Wrong input format! Try again." << endl;}		// it will print just once since generalCheck would be false after the first one, no need for another boolean type variable
				generalCheck = false;
			}
		}
	} else if (inpChoice == "2") {
		int charCount = inpInput.length();
		for (int i=0; i<inpInput.length();i++) {
				if ((inpInput.at(i) >= 'a' && inpInput.at(i) <= 'z')) {     // checking if there is at least one lowercase letter
					letterCheck = true;
					charCount -= 1;
				}
				if (inpInput.at(i) == '?') {								// checking how many question marks there are and counting them
					charCount -= 1;
				}
		}
		if (charCount != 0 || !letterCheck) {
			cout << "Wrong input format! Try again." << endl;
			generalCheck = false;
		}
	} else {cout << "Choice can be 1, 2 or 3! Try again." << endl; generalCheck = false;}
	return generalCheck;
}

void procedure(string inpChoice, string inpInput, string sourceWord) {
	int length = inpInput.length();
	bool check = false, questionMarkCheck = false;
	if (inpChoice=="2") {
		cout << "For the source word \"" << sourceWord << "\" and search word \"" << inpInput << "\",";
		for (int j = 0; j< length; j++) {
			if (inpInput.at(j) == '?') {questionMarkCheck = true;}
		} 
		for (int i = 0 ; i< sourceWord.length(); i++) {
			if (sourceWord.at(i) == inpInput.at(0) || inpInput.at(0) == '?') { 
				if (!questionMarkCheck && sourceWord.substr(i,length) == inpInput) {
					check = true;
					cout << "\n\"" << sourceWord.substr(i,length) << "\" has been found at index " << i;
				} else if (questionMarkCheck) {
					int numberCount = 0;
					for (int j = 0; j < length; j++) {
						if (i < sourceWord.length() - length+1) {if (inpInput.at(j) == sourceWord.at(i+j) || inpInput.at(j) == '?') {numberCount++;}}
					}
					if (length==numberCount) {
						check = true;
						cout << "\n\"" << sourceWord.substr(i,length) << "\" has been found at index " << i;
					}
				}
			}
		}
		if (!check) {cout << " no match has been found.";}
		cout << endl;
	} else {} // pass and get new input in the main function 
}


int main() {
	intro();
	string userInput, userChoice, sourceWord;
	bool checkStatus = true, sourceWordExist = false;		// to check if the user choice is "3"
	cout << "\nEnter your choice and string: ";
	cin >> userChoice;
	while (!(userChoice == "1" || userChoice == "2" || userChoice == "3")) {cout << "Choice can be 1, 2 or 3! Try again." << endl << "\nEnter your choice and string: "; cin >> userChoice;} 
	while (userChoice!= "3") 
		{cin >> userInput;
		if (inputCheck(userChoice, userInput)) {
			if (userChoice=="1") {
				sourceWordExist = true;
				sourceWord = userInput;
				cout << "Source word has been changed to \"" << sourceWord << "\"" << endl;
			} else { // User choice is "2"
				if (sourceWordExist)
				{procedure(userChoice, userInput, sourceWord);}
				else {cout << "Wrong input format! Try again." << endl;}
			}
		}
		cout << "\nEnter your choice and string: ";
		cin >> userChoice;
		while (!(userChoice == "1" || userChoice == "2" || userChoice == "3")) {cout << "Choice can be 1, 2 or 3! Try again." << endl << "\nEnter your choice and string: "; cin >> userChoice;}
	}
	cout << "See you!" << endl;
	return 0;
}