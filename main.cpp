#include <iostream>
#include <string>
#include <fstream>
#include <vector>
using namespace std;
#include "Cache.h"
#include <chrono>

int main(int argc, char* argv[]) {
    string pickFunction;
    string inputFileName;
    string outputFileName;

    if (argc >= 4) {
        pickFunction = argv[1];
        inputFileName = argv[2];
        outputFileName = argv[3];
    }
    else {
        cout << "Matcher or Verifier?" << endl;
        cout << "For Matcher, enter either 'Matcher', 'matcher', 'Match', or 'match'" << endl;
        cout << "For Verifier, enter either 'Verifier', 'verifier', 'Verify', or 'verify'" << endl;
        cin >> pickFunction;
        cout << "Please provide the input file (..\\input\\<input_file_name.txt>): " << endl;
        cin >> inputFileName;
        cout << "Please provide the output file (..\\output\\<output_file_name.txt>): " << endl;
        cin >> outputFileName;
    }


    if (pickFunction == "Matcher" || pickFunction == "matcher" || pickFunction == "Match" || pickFunction == "match") {
        ifstream inputFile(inputFileName);
        if (!inputFile.is_open()) {
            cerr << "Error opening file " << inputFileName << endl;
            return 1;
        }
        int numPeople;
        inputFile >> numPeople;
        vector<vector<int>> hospitalPreferenceList(numPeople, vector<int>(numPeople));
        vector<vector<int>> studentPreferenceList(numPeople, vector<int>(numPeople));
        // Jacob - added the direct conversion to int and relies on an input file with spaces for larger ints, and to ensure
        // this will work the input file reads directly from the every current integer in the file, rather than string preference method
        for(int i = 0; i < numPeople; i++){
           for(int j = 0; j < numPeople; j++){
               inputFile >> hospitalPreferenceList[i][j];
               hospitalPreferenceList[i][j] -= 1;
           }
        }
        for(int h = 0; h < numPeople; h++){
            for(int g = 0; g < numPeople; g++){
                inputFile >> studentPreferenceList[h][g];
                studentPreferenceList[h][g] -= 1;
            }
        }
        vector<vector<int>> studentRankingList(numPeople, vector<int>(numPeople));
        for(int g = 0; g < numPeople; g++){
           for(int h = 0; h < numPeople; h++){
               int studentRanking = studentPreferenceList[g][h];
               studentRankingList[g][studentRanking] = h;
           }
        }
        inputFile.close();
        GaleShapely galeShapely;
        auto start = chrono::steady_clock::now();
        vector<vector<int>> matchings = galeShapely.GaleShapelyAlgorithm(hospitalPreferenceList, studentRankingList);
        auto end = chrono::steady_clock::now();
        auto duration = chrono::duration_cast<chrono::nanoseconds>(end - start);
        cout << "Time taken by Gale-Shapely: " << duration.count() << " nanoseconds!" << endl;
        ofstream outputFile(outputFileName);
        if (!outputFile.is_open()) {
            cerr << "Error opening output file " << outputFileName << endl;
            return 1;
        }

        for(const auto& hospital : matchings){
            outputFile << hospital[0] + 1 << " " << hospital[1] + 1 << endl;
            cout << hospital[0] + 1 << " " << hospital[1] + 1<< endl;
        }
        outputFile.close();
    }
    else if (pickFunction == "Verifier" || pickFunction == "verifier" || pickFunction == "Verify" || pickFunction == "verify") {
        ifstream inputFile(inputFileName);
        ifstream matchFile(outputFileName);

        if (!inputFile.is_open()) {
            cerr << "Error opening input file " << inputFileName << endl;
            return 1;
        }
        if (!matchFile.is_open()) {
            cerr << "Error opening matching file " << outputFileName << endl;
            return 1;
        }
        int numStudents;
        inputFile >> numStudents;
        vector<vector<int>> hospitalPreferenceList(numStudents, vector<int>(numStudents));
        vector<vector<int>> studentPreferenceList(numStudents, vector<int>(numStudents));

        for (int i = 0; i < numStudents; i++) {
            for (int j = 0; j < numStudents; j++) {
                inputFile >> hospitalPreferenceList[i][j];
                hospitalPreferenceList[i][j] -= 1;
            }
        }

        for (int i = 0; i < numStudents; i++) {
            for (int j = 0; j < numStudents; j++) {
                inputFile >> studentPreferenceList[i][j];
                studentPreferenceList[i][j] -= 1;
            }
        }

        inputFile.close();

        vector<vector<int>> matches(numStudents, vector<int>(2));
        for (int i = 0; i < numStudents; i++) {
            matchFile >> matches[i][0] >> matches[i][1];
            matches[i][0] -= 1;
            matches[i][1] -= 1;
        }
        matchFile.close();

        Verifier verifier;
        string reason;
        auto start = chrono::steady_clock::now();
        bool isValid = verifier.checkIfValid(matches, numStudents, reason);
        pair<int, int> blockingPair;
        bool isStable = verifier.checkIfStable(matches, hospitalPreferenceList, studentPreferenceList, blockingPair);
        auto end = chrono::steady_clock::now();
        auto duration = chrono::duration_cast<chrono::nanoseconds>(end - start);
        cout << "Time taken by Verifier: " << duration.count() << " nanoseconds!" << endl;

        if (isValid == true && isStable == true) {
            cout << "VALID STABLE" << endl;
        }
        else {
            if (isValid == false) {
                cout << "INVALID: " << reason << endl;
            }
            if (isStable == false) {
                cout << "UNSTABLE. Blocking pair: " << blockingPair.first << ", " << blockingPair.second << endl;
            }
            if (isValid == false && isStable == false) {
                cout << "INVALID AND UNSTABLE" << endl;
            }
        }
    }
    else {
        cerr << "Error: Select either matcher or verifier" << endl;
        return 1;
    }

    return 0;
}