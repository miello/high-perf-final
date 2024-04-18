#ifndef _POWERGRID_UTILITY
#define _POWERGRID_UTILITY

#include <vector>
#include <algorithm>
#include <iostream>

using std::max_element;
using std::vector;
using std::cout;
using std::pair;

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
    int _default = 0;
    for (auto i: v) {
        if (i == 2) _default = 1;
    }
    for (auto i: v) {
        cout << ((i == -1 || i == 2) ? _default: i);
    }
}

pair<int, vector<int>> greedy_estimation(
    int N, 
    vector<vector<int>> &edges, 
    vector<int> degree
) {
    int answer = 0;
    int notSelected = N;

    vector<bool> included(N, false);
    vector<int> selected(N, 0);
    
    while (notSelected > 0) {
        int idx = max_index(degree);

        ++answer;
        --notSelected;

        included[idx] = true;
        selected[idx] = 1;

        for (auto &v: edges[idx]) {
            if (!included[v]) {
                --notSelected;
            }

            included[v] = true;
            degree[v] = -1;
        }

        degree[idx] = -1;
    }

    return make_pair(answer, selected);
}

#endif