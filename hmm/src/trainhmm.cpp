#include <fstream>
#include <iostream>
using namespace std; 

#include "hmm.h"
#include <cmath>

void test();

void trainhmm(string race, int numStates, int maxIterations)
{
	Hmm hmm;
	hmm.makeEmitAndTransFiles(race, numStates);
	hmm.loadFromRace(race);
	ifstream istrm(race + "/data.csv");

	vector<vector<unsigned long>*> trainingSequences;
	hmm.readSeqs(istrm, trainingSequences);
	hmm.baumWelch(trainingSequences, maxIterations);
	hmm.saveProbs(race + "/hmm");
}

vector<unsigned long>* loadReplayData(string race, int idx) {
	vector<unsigned long>* result = new vector<unsigned long>();
	ifstream istrm(race + "/data.csv");
	string line;
	for (int i = 0; i <= idx; i++) {
		getline(istrm, line);
	}
	string::size_type begIdx, endIdx;
	begIdx = line.find_first_not_of(",");
	while (begIdx != string::npos) {
		endIdx = line.find_first_of(",", begIdx);
		if (endIdx == string::npos) {
			endIdx = line.length();
		}
		string word = line.substr(begIdx, endIdx - begIdx);
		result->push_back(atol(word.c_str()));
		begIdx = line.find_first_not_of(",", endIdx);
	}
	return result;
}

void testhmm(string race, int index)
{
	cout << "Testing with replay " << index << endl;
	Hmm hmm;
	hmm.loadFromRace(race);
	vector<unsigned long>* replay = loadReplayData(race, index);
	cout << "Replay length: " << replay->size() << endl;
	unsigned int missed = 0;
	for (vector<unsigned long>::iterator it = replay->begin(); it != replay->end(); it++) {
		int prediction = hmm.predictMax(1);
		cout << "Prediction: ";
		cout.width(4);
		cout << prediction << " Actual: ";
		cout.width(4);
		cout << (*it) << endl;
		if (prediction != *it) missed++;
		hmm.observe(*it);
	}
	cout << "Accuracy: " << (1.0 - missed / (double)replay->size()) << endl;
}

int main(int argc, char* argv[])
{
	if (argc == 3) {
		testhmm(argv[1], atoi(argv[2]));
	}
	else {
		system("time /t");
		trainhmm(argv[1], 32, 128);
		system("time /t");
	}

    system("pause");
}