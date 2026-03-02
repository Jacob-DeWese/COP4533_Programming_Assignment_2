#include <iostream>
#include <string>
#include <fstream>
#include <vector>
using namespace std;
#include "Cache.h"
#include <chrono>

int main(int argc, char* argv[]) {
    string inputFileName;
    string outputFileName;

    if (argc >= 3) {
        inputFileName = argv[1];
        outputFileName = argv[2];
    }
    else {
        cout << "Please provide the input file (..\\input\\<input_file_name.txt>): " << endl;
        cin >> inputFileName;
        cout << "Please provide the output file (..\\output\\<output_file_name.txt>): " << endl;
        cin >> outputFileName;
    }

    ifstream inputFile(inputFileName);
    if (!inputFile.is_open()) {
        cerr << "Error opening file " << inputFileName << endl;
        return 1;
    }
    int k, m;
    inputFile >> k >> m;
    if (k < 1) {
        cerr << "Invalid input: k must be >= 1" << endl;
        return 1;
    }

    vector<int> inputSequence;
    int temp;
    for (int i = 0; i < m; i++) {
        if (!(inputFile >> temp)) {
            cerr << "Error: Number of requests in input file are less than the number of ids." << endl;
            return 1;
        }
        inputSequence.push_back(temp);
    }
    inputFile.close();

    ofstream outputFile(outputFileName);
    if (!outputFile.is_open()) {
        cerr << "Error opening file " << outputFileName << endl;
    }
    int fifoMisses = simFIFO(k, inputSequence);
    outputFile << "FIFO: " << fifoMisses << endl;
    int lruMisses = simLRU(k, inputSequence);
    outputFile << "LRU: " << lruMisses << endl;
    int optffMisses = simOPTFF(k, inputSequence);
    outputFile << "OPTFF: " << optffMisses << endl;

    outputFile.close();

    return 0;
}