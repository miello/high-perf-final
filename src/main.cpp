#include <vector>
#include <iostream>
#include "utils.h"
#include "brute_force.h"

using std::cin;
using std::ios_base;
using std::vector;

int main(int argc, char **argv)
{
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    if (argc < 3) {
        cout << "Usage: ./power_grid <input_path> <output_path>";
        exit(1);
    }

    freopen(argv[1], "r", stdin);
    freopen(argv[2], "w", stdout);

    int N, M;
    cin >> N >> M;

    vector<int> degree(N);
    vector<vector<int>> edges(N, vector<int>());
    vector<vector<int>> adj(N, vector<int>(N));

    for (int i = 0; i < M; ++i)
    {
        int u, v;

        cin >> u >> v;

        edges[u].push_back(v);
        edges[v].push_back(u);

        ++degree[u];
        ++degree[v];

        adj[u][v] = 1;
        adj[v][u] = 1;
    }

    auto ans = greedy_estimation(N, edges, degree);

    if (N > 3) {
        bruteforce_solve(N, edges, ans);
    }

    print_answer(ans.second);
}