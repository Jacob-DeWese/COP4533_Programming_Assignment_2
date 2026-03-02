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

// How the OPTFF works in principle gathered from here: https://tddg.github.io/cs571-spring20/public/lecs/lec8c-mem-caching-belady-offline.pdf
int simOPTFF(int k, vector<int>& requestSequence) {
    unordered_set<int> cache;
    int numMisses = 0;

    for (int i = 0; i < requestSequence.size(); i++) {
        if (cache.find(requestSequence[i]) != cache.end()) {
            continue;
        }
        numMisses++;

        if (cache.size() == k) {
            /*
             * Get currNum from vector and store as int
             * Get first num from cache (cacheNum). From currNum index (i) forward, check if cacheNum is in vector.
             *  If no, evict and add currNum to the cache.
             *  If yes, store the index of cacheNum from the vector (int latestAccessIndex)
             *      For all remaining numbers, if the index > latestAccess, update the latestAccessNum to the cacheNum
             *  Evict latestAccessNum from cache and add currNum
             */
            int currNum = requestSequence[i];
            int latestAccessIndex = -1;
            int latestAccessNum = -1;
            for (auto cachedNums : cache) {
                int currNumNextInstance = -1;

                for (int j = i + 1; j < requestSequence.size(); j++) {
                    if (requestSequence[j] == cachedNums) {
                        currNumNextInstance = j;
                        break;
                    }
                }

                if (currNumNextInstance == -1) {
                    latestAccessNum = cachedNums;
                    break;
                }

                if (currNumNextInstance > latestAccessIndex) {
                    latestAccessIndex = currNumNextInstance;
                    latestAccessNum = cachedNums;
                }
            }

            cache.erase(latestAccessNum);
        }

        cache.insert(requestSequence[i]);
    }

    return numMisses;
}