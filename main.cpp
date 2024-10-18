#ifndef __PROGTEST__

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cstdint>
#include <cassert>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;
#endif /* __PROGTEST__ */


bool readUTF8 (const char *inFile, char32_t & character, ifstream & fin, char (& array)[4]) {


	if ((array[0] & 128) == 0) {
		character = (array[0] & 127);
		return true;
	}




	if (((array[0] & 192) == 192) && ((array[0] & 32) == 0)) {
		fin.read(array + 1, 1);
		if (array[1] & 64)
			return false;
		if (!(array[1] & 128 ))
			return false;
		character = ((array[0] & 31) << 6);
		character |= (array[1] & 63);
		return true;
	}

	if (((array[0] & 224) == 224) && ((array[0] & 16) == 0)) {
		fin.read(array + 1, 2);
		if (array[1] & 64 || array[2] & 64)
			return false;
		if (!(array[1] & 128 && array[2] & 128))
			return false;
		character = ((array[0] & 15) << 6);
		character |= (array[1] & 63);
		character = character << 6;
		character |= (array[2] & 63);
		return true;
	}
	if (((array[0] & 240) == 240) && ((array[0] & 8) == 0)){
		fin.read(array + 1, 3);
		if (array[1] & 64 || array[2] & 64 || array[3] & 64)
			return false;
		if (!(array[1] & 128 && array[2] & 128 && array[3] & 128))
			return false;
		character = ((array[0] & 7) << 6);
		character |= (array[1] & 63);
		character = character << 6;
		character |= (array[2] & 63);
		character = character << 6;
		character |= (array[3] & 63);
		return true;
	}

	return false;

}

void addLastOne(vector<bool> & vec) {

	for (int i = 31; i >= 0; i--) {
		if (vec[i]) {
			vec[i + 1] = true;
			break;
		}
	}
}


void fibo(char32_t character, vector<bool> & buffer) {
	int depth = 0;
	int fib = 0;
	int fib1 = 0;
	int fib2 = 1;

	while (fib <= (int)character) {
		depth++;
		fib = fib1 + fib2;
		fib1 = fib2;
		fib2 = fib;
	}
	buffer[depth - 2] = true;

	character = character - fib1;
	if (character > 0) {
		fibo(character, buffer);
	}
	else {
		addLastOne(buffer);
	}
}

void shiftToLeft(char32_t & character) {


	int i;
	for (i = 32; i >= 1; i--) {
		if (((1 << 31) & (character)) == 0) {
			 character = character << 1;
		}
		else break;
	}
}

void printVec(vector<bool> vec) {
	int size = vec.size();

	for (int i = 0; i < size; i++) {
		if (vec[i] == true)
			cout << "1";
		else cout << "0";

	}
	cout << endl;
}

void connect(vector<bool> & dest, vector<bool> & source) {
	int i;
	for (i = 39; i >= 0; i--) {
		if (dest[i]) {
			break;				//na i + 1 je prvni pozice kam chci psat source
		}
	}
	//cout << endl << "prepisuju: ";
	i+=1;
	unsigned int j = 0;
	unsigned int sourcesize = source.size();
	for (;i <= 39 && j < sourcesize; j++, i++) {
		dest[i] = source[j];

	}
	source.clear();
	source.resize(32, false);
}

void print8(vector<bool> & vec, ofstream & fout) {
	char buffer[1];
	buffer[0] = 0;
	//cout << "zapsano" << endl;
	for (int i = 0; i <= 7; i++) {
		if (vec[0]) {
			buffer[0] += (1 << i);
		}
		//cout << vec[0];
		vec.erase(vec.begin());
	}
	//cout << endl << endl;
	vec.resize(40, false);
	fout.write(buffer, 1);
}

bool moreThan8(vector<bool> vec) {
	int i;
	for (i = 39; i >= 0; i--) {
		if (vec[i]) {
			break;				//na i + 1 je pozice ukoncovaci jednicky
		}
	}

	if (i >= 7)
		return true;
	return false;
}

bool validFile(ifstream & fin) {
	if (!fin.is_open()) {
		//cout << "Soubor neni otevreny.\n" << endl;
		return false;
	}
	if (fin.fail()) {
		//cout << "Nevalidni prace se souborem.\n" << endl;
		return false;
	}
	return true;
}

bool validOFile(ofstream & fin) {
	if (!fin.is_open()) {
		//cout << "Soubor neni otevreny.\n" << endl;
		return false;
	}
	if (fin.fail()) {
		//cout << "Nevalidni prace se souborem.\n" << endl;
		return false;
	}
	return true;
}

bool empty(const vector<bool> vec) {

	for (unsigned int i = 0; i < vec.size(); i++) {
		if (vec[i] == 1)
			return false;
	}
	return true;
}

bool utf8ToFibonacci(const char *inFile, const char *outFile) {
	char32_t character = 0;
	vector<bool> buffer;
	vector<bool> vec;
	buffer.resize(32, false);
	vec.resize(40, false);
	char array[4];

	ifstream fin(inFile, ios::binary);
	ofstream fout(outFile, ios::out | ios::binary);
	if (!validFile(fin)) {
		return false;
	}
	if (!validOFile(fout)) {
		return false;
	}

	while (fin.read(array, 1)) {
		if (!validFile(fin)) {
			return false;
		}
		if (!validOFile(fout)) {
			return false;
		}

		if (!readUTF8(inFile, character, fin, array))
			return false;


		fibo(character + 1, buffer);
		//cout << endl << "buffer: " << endl;
		//printVec(buffer);
		//cout << endl << "vec: " << endl;
		//printVec(vec);
		connect(vec, buffer);
		//cout << endl << "po connect vec:" << endl;
		//printVec(vec);
		while (moreThan8(vec)) {
			print8(vec, fout);
		}
	}

	if (!empty(vec))
		print8(vec, fout);
	if (fin.bad()) {
		return false;
	}

	if(fout.bad())
		return false;
	return true;
}

void reverse(char ( & array )[1], int & switched) {
	for (int i = 0; i <= 7; i++) {
		if (array[0] & (1 << i))
			switched += (1 << (7 - i));
	}
}

void intoVec(const int & character, vector<bool> & vec) {


	for (int i = 7; i >= 0; i--) {
		int k = (character & (1 << i));
		vec.push_back(k);
	}
}

bool contains2ones(const vector<bool> & vec) {

	if (vec.size() == 1 || vec.empty())
		return false;

	for (unsigned int i = 0; i < vec.size() - 1; i++) {
		if (vec[i] && vec[i+1])
			return true;
	}
	return false;
}


int convertFromFib(vector<bool> & vec, int & sum) {

	unsigned int i;
	if (vec.size() > 34)
		return false;
	for (i = 0; i < vec.size() - 1; i++) {
		if (vec[i] && vec[i+1])			// i is the last fib. number, i + 1 is ending 1
			break;
	}
	int fib = 0, fib1 = 1, fib2 = 2;


	for (unsigned int j = 0; j <= i; j++) {
		if (j == 0) {
			if (vec[0])
				sum += 1;

			vec.erase(vec.begin());
			continue;
		}
		if (j == 1) {
			if (vec[0])
				sum += 2;
			vec.erase(vec.begin());
			continue;
		}
		fib = fib1 + fib2;
		fib1 = fib2;
		fib2 = fib;
		if (vec[0]) {
			sum += fib;
		}
		vec.erase(vec.begin());
	}
	vec.erase(vec.begin());
	return sum;
}

bool utf8(int value, ofstream & fout) {

	uint8_t array[4] = {0, 0, 0, 0};

	if (value >= 0 && value <= 127) {
		array[0] += value;
		fout.write((char*)array, 1);
	}

	if (value >= 128 && value <= 2047) {

		array[0] += (128 + 64);
		array[1] += 128;

		for (int i = 6; i <= 10; i++) {
			if (value & (1 << i)) {
				array[0] += (1 << (i - 6));
			}
		}

		for (int i = 0; i <= 5; i++) {
			if (value & (1 << i))
				array[1] += (1 << i);
		}

		fout.write((char*)array, 2);
	}

	if (value >= 2048 && value <= 65535) {
		array[0] += (128 + 64 + 32);
		array[1] += 128;
		array[2] += 128;

		for (int i = 0; i <= 5; i++) {
			if (value & (1 << i))
				array[2] += (1 << i);
		}

		for (int i = 6; i <= 11; i++) {
			if (value & (1 << i)) {
				array[1] += (1 << (i - 6));
			}
		}

		for (int i = 12; i <= 15; i++) {
			if (value & (1 << i)) {
				array[0] += (1 << (i - 12));
			}
		}
		fout.write((char*)array, 3);
	}
	if(value >= 65536 && value <= 1114111) {
		array[0] += (128 + 64 + 32 + 16);
		array[1] += 128;
		array[2] += 128;
		array[3] += 128;
		for (int i = 0; i <= 5; i++) {
			if (value & (1 << i))
				array[3] += (1 << i);
		}

		for (int i = 6; i <= 11; i++) {
			if (value & (1 << i))
				array[2] += (1 << (i - 6));
		}

		for (int i = 12; i <= 17; i++) {
			if (value & (1 << i))
				array[1] += (1 << (i - 12));
		}
		for (int i = 18; i <= 20; i++) {
			if (value & (1 << i))
				array[0] += (1 << (i - 18));
		}

		fout.write((char*)array, 4);
	}
	if (value < 0 || value > 1114111)
		return false;
	return true;
}

bool contains1(vector<bool> & vec) {
	unsigned int size = vec.size();
	for (unsigned int i = 0; i < size; i++) {
		if (vec[i])
			return true;
	}
	return false;

}

bool fibonacciToUtf8(const char *inFile, const char *outFile) {

	char array[1];
	vector<bool> vec;
	array[0] = 1;

	ifstream fin(inFile, ios::binary);
	ofstream fout(outFile, ios::out | ios::binary);


	if (!validFile(fin)) {
		return false;
	}
	if (!validOFile(fout)) {
		return false;
	}

	while (fin.read(array, 1)) {
		if (!validFile(fin)) {
			return false;
		}
		if (!validOFile(fout)) {
			return false;
		}
		int switched = 0;
		//cout << "read ---------- " << endl;
		reverse(array, switched);
		intoVec(switched, vec);
		//cout << "vec:   ";
		//printVec(vec);
		//cout << endl;
		while (contains2ones(vec)) {
			int value = 0;
			if (!convertFromFib(vec, value))
				return false;
			value--;

			//cout << "value: " << value << endl;

			if (!utf8(value, fout))
				return false;
		}


	}
	if (contains1(vec))
		return false;

	if (fin.bad()) {
		return false;
	}

	if (fout.bad()) {
		return false;
	}

	return true;
}

#ifndef __PROGTEST__

bool identicalFiles(const char *file1,
		    const char *file2) {

	char array[2];
	bool r = true;
	ifstream fin1(file1, ios::binary);
	ifstream fin2(file2, ios::binary);
	while (fin1.read(array, 1)){
		fin2.read(array + 1, 1);

		if (array[0] != array[1])
			r =  false;
	}
	return r;
}

int main(int argc, char *argv[]) {




	fibonacciToUtf8("example/in_5031390.bin", "output.fib");
	utf8ToFibonacci("example/src_12.txt", "output.fib");
	assert (utf8ToFibonacci("example/src_0.utf8", "output.fib")
		&& identicalFiles("output.fib", "example/dst_0.fib"));

	assert (utf8ToFibonacci("example/src_1.utf8", "output.fib")
		&& identicalFiles("output.fib", "example/dst_1.fib"));
	assert (utf8ToFibonacci("example/src_2.utf8", "output.fib")
		&& identicalFiles("output.fib", "example/dst_2.fib"));
	assert (utf8ToFibonacci("example/src_3.utf8", "output.fib")
		&& identicalFiles("output.fib", "example/dst_3.fib"));
	assert (utf8ToFibonacci("example/src_4.utf8", "output.fib")
		&& identicalFiles("output.fib", "example/dst_4.fib"));

	assert (!utf8ToFibonacci("example/src_5.utf8", "output.fib"));

	assert (fibonacciToUtf8("example/src_6.fib", "output.utf8")
		&& identicalFiles("output.utf8", "example/dst_6.utf8"));
	assert (fibonacciToUtf8("example/src_7.fib", "output.utf8")
		&& identicalFiles("output.utf8", "example/dst_7.utf8"));
		assert (fibonacciToUtf8("example/src_8.fib", "output.utf8")
		&& identicalFiles("output.utf8", "example/dst_8.utf8"));
	assert (fibonacciToUtf8("example/src_9.fib", "output.utf8")
		&& identicalFiles("output.utf8", "example/dst_9.utf8"));
	assert (fibonacciToUtf8("example/src_10.fib", "output.utf8")
		&& identicalFiles("output.utf8", "example/dst_10.utf8"));
	assert (!fibonacciToUtf8("example/src_11.fib", "output.utf8"));

	return EXIT_SUCCESS;
}

#endif /* __PROGTEST__ */
/*
01100111 00011010

11100110 01011000

11 10011 001011
1   1+5   3+8

*/