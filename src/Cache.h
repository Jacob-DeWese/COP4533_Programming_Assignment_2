//
// Created by thear on 2/27/2026.
//

#pragma once
#include <vector>
using namespace std;

int simFIFO(int k, vector<int>& requestSequence);
int simLRU(int k, vector<int>& requestSequence);
int simOPTFF(int k, vector<int>& requestSequence);