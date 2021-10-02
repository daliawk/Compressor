#ifndef ALPHABET
#define ALPHABET
#include <iostream>
#include <string>
using namespace std;

class Alphabet {
private:
	string letters;
	float probability;

	Alphabet* left;
	Alphabet* right;
public:
	Alphabet() { letters = ""; probability = 0; left = NULL; right = NULL; }
	Alphabet(map<char, string> table);
	Alphabet(string l, float prob) { letters = l; probability = prob; }
	void Initialise(char l, float prob) { letters += l; probability = prob; }
	void Print() { cout << letters << " " << probability << endl; }
	float GetProbability() { return probability; }
	string findChar(string& finalbinary);

	Alphabet Merge(Alphabet& a, Alphabet& b) {
		Alphabet c(a.letters + b.letters, a.probability + b.probability);
		//c.Initialise(a.letters + b.letters, a.probability + b.probability);
		//c.left= &a;
		//c.right = &b;

		c.left = new Alphabet;
		*c.left = a;

		c.right = new Alphabet;
		*c.right = b;

		//cout << "c - > ";
		//c.Print();
		//cout << "Left child: ";
		//c.left->Print();
		//cout << "Right child: ";
		//c.right->Print();
		return c;
	}

	void Encode(string& s, char c) {
		if (this->left==NULL && this->right==NULL) {
			return;
		}
		else {
			if (this->left != NULL) {
				int temp = this->left->letters.find(c);
				if (temp != string::npos) {
					s += '0';
					this->left->Encode(s, c);
				}
			}
			if (this->right != NULL) {
				int temp = this->right->letters.find(c);
				if (temp != string::npos) {
					s += '1';
					this->right->Encode(s, c);
				}
			}
		}
	}
};

Alphabet::Alphabet(map<char, string> table)
{

	for (auto it = table.begin(); it != table.end(); ++it)
	{
		char charcter; 
		string binary; 

		charcter = it->first;
		binary = it->second;

		letters += charcter;
		Alphabet* pointer;
		pointer = this;
		while (binary.size()>0)
		{
			if (binary[0] == '1')
			{
				if(pointer->right==NULL)
					pointer->right = new Alphabet;

				pointer = pointer->right;
			}
			else
			{
				if (pointer->left == NULL)
					pointer->left = new Alphabet;
				pointer = pointer->left;
			}
			pointer->letters += charcter;
			binary.erase(0, 1);
		}

	}
}

string Alphabet::findChar(string& finalbinary)
{
	if (this->left == NULL && this->right == NULL)
		return this->letters;
	else
	{
		if (finalbinary[0] == '0')
		{
			finalbinary.erase(0, 1);
			return this->left->findChar(finalbinary);
		}
		else 
		{
			finalbinary.erase(0, 1);
			return this->right->findChar(finalbinary);
		}

	}
}


#endif