//***********************************************************//
//** Name: Dhiyaa Al Jorf                                  **//
//** Net ID: da2863                                        **//
//** Date Created: November 2, 2021                        **//
//** Assignment 3: Computer Engineering Case Study:        **//
//** Concordance of a text file                            **//
//** Program: Assignment3.cpp                              **//
//***********************************************************//

#include <iostream>
#include <fstream> // To read from and write into files
#include <string> // To manipulate strings
#include <iomanip> // To change the format of the output
#define W 25 // Width for setw();

using namespace std;

// Function Prototypes
void readFile(string**& ptrArr, int& size);
void add(string**& ptrArr, int& size, string value);
void trim(string str, int& size, string**& ptrArr);
void sort(string**& ptrArr, int size);
void swap(string*& x, string*& y);
void concordance(string** ptrArr, string**& ptrFreq, string**& ptrUnique, int size, int& sizeUnique);
void outputAndSaveConcordance(string** ptrUnique, string** ptrFreq, int sizeUnique);
void deleteArray(string** ptr, int size);

// Main Function
int main() {
	int size = 0, sizeUnique = 0; // Both sizes start at 0
	// Initializing the arrays
	string** ptrArr = new string * [size];
	string** ptrFreq = new string * [sizeUnique];
	string** ptrUnique = new string * [sizeUnique];
	// Reading strings into the array and editting them as required
	readFile(ptrArr, size);
	// Sorting the array of strings
	sort(ptrArr, size);
	// Filling in the ptrUnique and ptrFreq arrays with concordance info
	concordance(ptrArr, ptrFreq, ptrUnique, size, sizeUnique);
	// Saving and outputting the concordance
	outputAndSaveConcordance(ptrUnique, ptrFreq, sizeUnique);
	// Deleting all memory we previously allocated on the heap
	deleteArray(ptrArr, size);
	deleteArray(ptrFreq, sizeUnique);
	deleteArray(ptrUnique, sizeUnique);
	return (0);
}

// readFile() reads all the strings from a user-specified file and performs necessary edits through the trim() function before saving the strings into an array
void readFile(string**& ptrArr, int& size) {
	// Ask the user for the file to find the concordance of
	string fileName;
	cout << "Enter the file name (include file extension): ";
	getline(cin, fileName);
	ifstream reader(fileName, ios::in);
	// If the file doesn't exist, output an error message and exit the program
	if (reader.fail()) {
		cerr << "ERROR: Failed to open file . . ." << endl;
		exit(-1);
	}
	string str;
	// Loop to read all strings, perform necessary edits, then save them into an array
	while (reader >> str) {
		trim(str, size, ptrArr);
	}
	reader.close();
}

// add() expands an array and adds a specified value to the end
void add(string**& ptrArr, int& size, string value) {
	string** tempPtr = new string * [size + 1]; // Allocate memory in the stack for an array with a size 1 more than the arrays current size
	for (int i = 0; i < size; i++) {
		tempPtr[i] = ptrArr[i]; // Copy all the values from the smaller array to the larger array
	}
	tempPtr[size] = new string(value); // Add a string to the last element of the array
	delete[] ptrArr; // Delete the smaller array
	ptrArr = tempPtr; // Make the pointer point to the larger array
	size++;
}

// trim() tokenizes the incoming string and saves the tokens into the array
void trim(string str, int& size, string**& ptrArr) {
	int start = 0;
	for (int i = 0; i < str.length(); i++) { // Goes through all the characters of a string (which can be treated as a character array)
		if (!isalpha(str[i]) || (i == (str.length() - 1) && isalpha(str[i]))) { // (Checks if the current charcter is non-alphabetical) or (the last character and alphabetical)
			if (isalpha(str[i])) {
				str[i] = tolower(str[i]);
				i++;
			}
			char* res = new char[(i + 1) - start]; // Character array of a size 1 more than the number of characters in the word to be added (the extra character is for the \0 character)
			for (int j = 0; j < i - start; j++)
				res[j] = str[j + start];
			res[i - start] = '\0'; // Sets the last character as the end of string character so it can be saved into a string
			string result = res;
			delete[] res; // Deallocate the memory set for res
			start = i + 1;
			if (result != "") // Checks if the word isn't empty. That can happen in cases in which the whole string is nonalphabetical
				add(ptrArr, size, result); // Add the cut out portion to the array
		}
		else {
			str[i] = tolower(str[i]); // Turns the current letter into it's lower case form
		}
	}
}

// sort() sorts an array using selection sort
void sort(string**& ptrArr, int size) {
	for (int i = 0; i < size - 1; i++) {
		int minIndex = i;
		for (int j = i + 1; j < size; j++) {
			if ((*ptrArr[minIndex]) > (*ptrArr[j]))
				minIndex = j;
		}
		swap(ptrArr[minIndex], ptrArr[i]);
	}
}

// swap() swaps 2 strings
void swap(string*& x, string*& y) {
	string* temp = x;
	x = y;
	y = temp;
}

// Finds the unique words in an array and the frequency of each unique word
void concordance(string** ptrArr, string**& ptrFreq, string**& ptrUnique, int size, int& sizeUnique) {
	// Outer loop with 2 counters: i is used for ptrArr and j is used for the concordance related arrays
	for (int i = 0, j = 0; i < size; i++, j++) {
		add(ptrUnique, sizeUnique, (*ptrArr[i])); // Add a new string to ptrUnique
		sizeUnique--; // The add function increases the size variable but we need the size to stay the same because it will now represent the size of ptrFreq
		int frequency = 1;
		// Inner loop increments frequency as long as the current string in ptrArr is the same as the current string in ptrUnique
		for (i++; i < size && *ptrUnique[j] == *ptrArr[i]; i++) {
			frequency++;
		}
		add(ptrFreq, sizeUnique, to_string(frequency)); // Add the frequency
		i--; // Correct for the extra increment that happens when the inner loop condition becomes false
	}
}

// Outputs the concordance on the output screen and writes it into a file
void outputAndSaveConcordance(string** ptrUnique, string** ptrFreq, int sizeUnique) {
	// Outputing to the output screen in tabular format
	cout << left << setw(W) << "WORD:" << setw(W) << "FREQUENCY:" << endl;
	for (int i = 0; i < sizeUnique; i++) {
		cout << setw(W) << *ptrUnique[i] << setw(W) << *ptrFreq[i] << endl;
	}
	// Ask the user for where to save the file
	string fileName;
	cout << "Where would you like to save the concordance (include .txt)?: ";
	getline(cin, fileName);
	// The loop makes sure the user is saving the file as a .txt file
	while (fileName.find(".txt") == string::npos) { // The find() function returns string::npos if the latter string isn't found in the former string
		cout << "Please include .txt: ";
		getline(cin, fileName);
	}
	ofstream writer(fileName, ios::trunc);
	if (writer.fail()) {
		cerr << "ERROR: Failed to create the file . . ." << endl;
		exit(-1);
	}
	// Saves the concordance in tabular form into the user-specified file
	writer << left << setw(W) << "WORD:" << setw(W) << "FREQUENCY:" << endl;
	for (int i = 0; i < sizeUnique; i++) {
		writer << left << setw(W) << *ptrUnique[i] << setw(W) << *ptrFreq[i] << endl;
	}
	// Confimation message
	cout << "Saving performed successfully . . ." << endl;
	writer.close();
}

// Deallocates all the memory used by an array of string pointers including the space used for the strings
void deleteArray(string** ptr, int size) {
	for (int i = 0; i < size; i++) {
		delete ptr[i]; // Deallocates all strings that the pointers in the array are pointing to
	}
	delete[] ptr; // Deallocates the block of memory that was used for the array of string pointers
}
