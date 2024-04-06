#ifndef _POWERGRID_UTILITY
#define _POWERGRID_UTILITY

#include <vector>
#include <algorithm>
#include <iostream>

using std::max_element;
using std::vector;
using std::cout;

int max_index(vector<int> &v) {
    auto mx = max_element(v.begin(), v.end());

    return std::distance(v.begin(), mx);
}

template <typename T>
void debug_print_vector(vector<T> &v) {
    for (auto i: v) {
        cout << i << " ";
    }
    cout << "\n";
}

template <typename T>
void print_answer(vector<T> &v) {
    for (auto i: v) {
        cout << i;
    }
}

int greedy_estimation(int N, vector<vector<int>> &edges, vector<int> degree) {
    int answer = 0;
    int notSelected = N;

    vector<bool> included(N, false);
    
    while (notSelected > 0) {
        int idx = max_index(degree);

        ++answer;
        --notSelected;

        included[idx] = true;

        for (auto &v: edges[idx]) {
            if (!included[v]) {
                --notSelected;
            }

            included[v] = true;
            degree[v] = -1;
        }

        degree[idx] = -1;
    }

    return answer;
}

#endif