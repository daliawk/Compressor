#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <map>
#include "Alphabet.h"
using namespace std;
void Compress(ifstream& in, ofstream& out);
void Decompress(ifstream& in, ofstream& out);
void CountFrequency(string phrase, vector<char>& alph, vector<float>& prob);
void Heapify(vector<Alphabet>& A, int n, int i);
void BuildHeap(vector<Alphabet>& A);
void HeapSort(vector<Alphabet>& A);
void BinaryMergeTree(vector<Alphabet>& A);
int BinaryToDecimal(string b);

int main() {
	string file;

	ifstream in;
	ofstream out;
	string outputFile;
	int choice;

	cout << "Welcome to our program!! How can we help you today?\n" << "1. Compress\n" << "2.Decompress\n\n" << "Type in your choice: ";
	cin >> choice;


	cout << "Enter input file name: ";
	cin >> file;

	in.open(file);

	if (in.fail()) {
		cout << "Could not open file";
		exit(1);
	}

	cout << "Enter output file name: ";
	cin >> outputFile;

	out.open(outputFile);
	if (out.fail()) {
		cout << "Error in output file";
		exit(1);
	}

	if (choice == 1) {
		Compress(in, out);
	}
	else {
		Decompress(in, out);
	}



	return 0;
}

void Compress(ifstream& in, ofstream& out) {
	string sentence="" ;
	vector<char> alphabet;
	vector<float> probability;
	map<char, string> table;

	while (!in.eof()) {
		string temp;
		getline(in, temp);
		sentence += temp;
		if (!in.eof()) {
			sentence+='\n';
		}
	}

	//cout << sentence;
	CountFrequency(sentence, alphabet, probability);

	int size = alphabet.size();
	vector<Alphabet> nodeArray(size);
	for (int m = 0; m < size; m++) {
		nodeArray[m].Initialise(alphabet[m], probability[m]);
		//nodeArray[m].Print();
	}
	//cout << endl;

	HeapSort(nodeArray);

	/*	for (int i = 0; i < size; i++) {
			nodeArray[i].Print();
		}
		*/

	BinaryMergeTree(nodeArray);

	//nodeArray[0].Print();

	//cout << "Alphabet Code: \n";
	string code;
	for (int i = 0; i < alphabet.size(); i++) {
		nodeArray[0].Encode(code, alphabet[i]);
		//cout << alphabet[i] << "-> " << code << endl;
		table.emplace(alphabet[i], code);
		code.clear();
	}



	string buffer = "";
	for (int i = 0; i < sentence.size(); i++) {
		buffer += table[sentence[i]];
	}

	int reminder=8;

	//cout << "Original Binary :" << buffer << endl;
	while (!buffer.empty()) {
		string temp;
		int dec;
		if (buffer.size() >= 8) {
			temp = buffer.substr(0, 8);
			buffer.erase(0, 8);
		}
		else {
			temp = buffer;
			reminder = buffer.size();
			buffer.clear();
		}
		//cout << "Temp= " << temp << endl;
		//cout << "Buffer= " << buffer << endl;

		dec = BinaryToDecimal(temp);
		//cout << "Decimal: " << dec << endl;
		//cout << "Ascii char: " << char(dec) << endl;

		out << char(dec);
	}

	out << endl;
	out << reminder << endl;

	for (auto iter = table.begin(); iter != table.end(); iter++)
	{
		
		if (iter->first != '\n') {
			out << iter->first << " " << iter->second << endl;
		}
		else {
			out<< "endl" << " " << iter->second << endl;
		}
	}
}

void Decompress(ifstream& in, ofstream& out) {
	string compressed;
	string size;
	getline(in, compressed);
	cout << compressed;
	cout << endl;
	getline(in, size);

	//Create Table
	string line;
	map<char, string> table;
	while (getline(in, line)) {

		if (line.substr(0, 5) == "endl ") {
			table.emplace('\n', line.substr(5));
			cout << '\n' << " " << line.substr(5) << endl;
		}
		else {
			table.emplace(line[0], line.substr(2));
			cout << line[0] << " " << line.substr(2) << endl;
		}

	}

	//Compressed to integers

	//integars to binary
	int n = compressed.length();
	string finalbinary = "";

	for (int i = 0; i < n; i++)
	{
		// convert each char to 
		// ASCII value 
		int val = int((unsigned char)compressed[i]);

		// Convert ASCII value to binary 
		string bin = "";
		while (val > 0)
		{
			if (val % 2 == 0)
				bin.push_back('0');
			else
				bin.push_back('1');

			val /= 2;
		}
		reverse(bin.begin(), bin.end());

		if (i != n - 1)
			while (bin.length() < 8)
				bin = "0" + bin;
		else
			while (bin.length() < stoi(size))
				bin = "0" + bin;

		finalbinary += bin;
	}
	cout << finalbinary<<endl;

	//binary to char by table
	Alphabet Tree(table);
	string finalresult="";
	while (!finalbinary.empty())
	{
		finalresult += Tree.findChar(finalbinary);
	}

	cout << finalresult << endl;
	//output in file
	out << finalresult;
}

void CountFrequency(string phrase, vector<char>& alph, vector<float>& prob) {
	vector<bool> visited(phrase.size(), false);
	float count;

	for (int k = 0; k < phrase.size(); k++) {

		if (!visited[k]) {
			count = 1;

			for (int p = k + 1; p < phrase.size(); p++) {
				if (phrase[k] == phrase[p]) {
					count++;
					visited[p] = true;

				}
			}

			alph.push_back(phrase[k]);
			prob.push_back(count / phrase.size());
		}
	}
}

void Heapify(vector<Alphabet>& A, int n, int i)
{
	int smallest = i;
	int left = 2 * i + 1;
	int right = 2 * i + 2;

	if (left < n && A[left].GetProbability() > A[smallest].GetProbability())
		smallest = left;

	if (right < n && A[right].GetProbability() > A[smallest].GetProbability())
		smallest = right;

	if (smallest != i) {
		swap(A[i], A[smallest]);
		Heapify(A, n, smallest);
	}
}

void BuildHeap(vector<Alphabet>& A)
{
	for (int v = A.size() / 2 - 1; v >= 0; v--)
		Heapify(A, A.size(), v);
}

void HeapSort(vector<Alphabet>& A)
{
	BuildHeap(A);

	for (int b = A.size() - 1; b >= 0; b--) {
		swap(A[0], A[b]);
		Heapify(A, b, 0);
	}
}


void BinaryMergeTree(vector<Alphabet>& A) {

	while (A.size() > 1) {
		Alphabet a = A.front();
		//cout << "a - > ";
		//a.Print();
		A.erase(A.begin());
		Alphabet b = A.front();
		A.erase(A.begin());

		//cout << " - > ";
		//b.Print();


		Alphabet c = a.Merge(a, b);

		A.push_back(c);
		HeapSort(A);
	}
}

int BinaryToDecimal(string b) {
	int binary = stoi(b);

	int dec_value = 0;

	// Initializing base value to 1, i.e 2^0
	int base = 1;

	int temp = binary;
	while (temp) {
		int last_digit = temp % 10;
		temp = temp / 10;

		dec_value += last_digit * base;

		base = base * 2;
	}

	return dec_value;

}