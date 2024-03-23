#include <bits/stdc++.h>

using std::cin;
using std::cout;
using std::vector;

int max_index(vector<int> &v) {
    auto mx = max_element(v.begin(), v.end());

    return std::distance(v.begin(), mx);
}

void print_vector(vector<int> &v) {
    for (auto &i: v) {
        cout << i << " ";
    }
    cout << "\n";
}

int greedy_estimation(int N, int M, vector<vector<int>> &edges, vector<int> degree) {
    int answer = 0;
    int notSelected = N;
    bool isCompleted = false;

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

int main(int argc, char **argv)
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    if (argc < 3) {
        cout << "Usage: ./power_grid <input_path> <output_path>";
        exit(1);
    }

    freopen(argv[1], "r", stdin);
    freopen(argv[2], "w", stdout);

    int N, M;
    std::cin >> N >> M;

    std::vector<int> degree(N);
    std::vector<vector<int>> edges(N, vector<int>());

    for (int i = 0; i < M; ++i)
    {
        int u, v;

        cin >> u >> v;

        edges[u].push_back(v);
        edges[v].push_back(u);

        ++degree[u];
        ++degree[v];
    }

    cout << greedy_estimation(N, M, edges, degree);
}