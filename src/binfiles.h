/**
* @file binfiles.h
*/

#ifndef BINFILES_H
#define BINFILES_H
#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <bitset>
#include <stdint.h>

#ifndef MYUNI
#define MYUNI

/** \union myUnion
*	\brief Used for conversion.
*/
union myUnion {
	uint64_t myInt;
	char byteArray[8];
};
#endif // MYUNI



using namespace std;

	/** \brief Sums the ASCII values of given string.
	 */
static char getChecksum(string text) {
	char sum = 0;
	for (unsigned int i = 0; i < text.length(); i++)
		sum += text[i];

	return sum;
}
	/** \brief Handles reading student's score from given file.
	*
	*	Also checks file's integrity and the checksum.
	*/
static string readScore(string path = "input.bin") {
	myUnion uni;
	ifstream file;
	string input = "";
	string entry = "";
	char ch, checkSum;
	uint64_t entriesCount;
	uint64_t entrySize;


	file.open(path.c_str());
	if (!file.good()){
		cout << "problem with file" << endl;
		return "";
	}

	for (unsigned int i = 0; i < 8; i++) {
		if(!file.good()) {
			cout << "corrupted file" << endl;
			return "";
		}
		file.read(&ch, sizeof (ch));
		uni.byteArray[i] = ch;
	}
	entriesCount = ~uni.myInt;
	for (uint64_t i = 0; i < entriesCount; i++) {
		if(!file.good()){
			cout << "corrupted file" << endl;
			return "";
		}
		for (unsigned int k = 0; k < 8; k++) {
			if(!file.good()) {
			cout << "corrupted file" << endl;
			return "";
		}
			file.read(&ch, sizeof (ch));
			uni.byteArray[k] = ch;
		}
		entrySize = ~uni.myInt;

		for (uint64_t j = 0; j < entrySize; j++) {
			if(!file.good()){
			cout << "corrupted file" << endl;
			return "";
		}
			file.read(&ch, sizeof (ch));

			ch = ~ch;
			entry += ch;
		}

		if (!file.good()){
			cout << "corrupted file" << endl;
			return "";
		}

		file.read(&checkSum, sizeof(checkSum));
		checkSum = ~checkSum;
		if (getChecksum(entry) != checkSum) {
			cout << "bad checksum" << endl;
			return "sum";
		}
		input += entry;
		entry = "";

	}
	file.close();
	return input;
}

	/** \brief Adds given entry to given file.
	 */
static bool writeScore(string output, string path = "output.bin") {
	myUnion uni;
	fstream file;
	streampos size;
	char * memblock;
	char ch;

	file.open(path.c_str(), ios::binary | ios::in);
	if (file.good()) {                              // read everything
		file.seekg(0, ios::end);
		size = file.tellg();
		file.seekg(0, ios::beg);
		memblock = new char [size];
		file.read(memblock, size);

	} else {
		size = 0;
	}
	file.close();

	if (size == 0) {
		uni.myInt = 1;
		uni.myInt = ~uni.myInt;
	} else {

		for (unsigned int k = 0; k < 8; k++) {
			uni.byteArray[k] = memblock[k];
		}
		uni.myInt--; //add one entry
	}


	file.open(path.c_str(), ios::binary | ios::out | ios::trunc); //truncate = erase contents

	if (!file.good()) return false;

	file.write(uni.byteArray, 8); //number of entries

	for (unsigned int i = 8; i < size; i++) { //copy older entries back
		file << memblock[i];
	}
	if (size > 0 ) delete [] memblock;

	uni.myInt = output.length();
	uni.myInt = ~uni.myInt;
	file.write(uni.byteArray, 8); //length of this entry

	for (unsigned int i = 0; i < output.length(); ++i) { //write this entry
		ch = output[i];
		ch = ~ch;
		file << ch;
	}

	ch = getChecksum(output);
	ch = ~ch;
	file << ch;
	file.close();
	return true;
}



#endif // BINFILES_H
