//
// Created by thear on 2/27/2026.
//

#include <vector>
#include <queue>
#include <unordered_set>
#include <unordered_map>
#include "Cache.h"
using namespace std;

int simFIFO(int k, vector<int>& requestSequence) {
    unordered_set<int> cache;
    queue<int> q;
    int numMisses = 0;

    for (int i = 0; i < requestSequence.size(); i++) {
        if (cache.find(requestSequence[i]) != cache.end()) {
            continue;
        }
        numMisses++;

        if (cache.size() == k) {
            int oldestItem = q.front();
            q.pop();
            cache.erase(oldestItem);
        }

        cache.insert(requestSequence[i]);
        q.push(requestSequence[i]);
    }

    return numMisses;
}

int simLRU(int k, vector<int>& requestSequence) {
    unordered_map<int, int> cache;
    vector<int> orderedAccess;
    int numMisses = 0;

    for (int i = 0; i < requestSequence.size(); i++) {
        int request = requestSequence[i];
        if (cache.find(request) != cache.end()) {
            for (int j = 0; j < orderedAccess.size(); j++) {
                if (orderedAccess[j] == request) {
                    orderedAccess.erase(orderedAccess.begin() + j);
                    break;
                }
            }

            orderedAccess.insert(orderedAccess.begin(), request);
            continue;
        }
        numMisses++;

        if (cache.size() == k) {
            int oldestAccessed = orderedAccess.back();
            orderedAccess.pop_back();
            cache.erase(oldestAccessed);
        }

        orderedAccess.insert(orderedAccess.begin(), request);
        cache.insert({request, 1});
    }

    return numMisses;
}