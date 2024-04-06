#include <bits/stdc++.h>

using namespace std;

int main(int argc, char **argv)
{
    if (argc < 3) {
        cout << "Usage: ./validator <input_path> <output_path>";
        exit(1);
    }

    freopen(argv[1], "r", stdin);

    int N, M;
    cin >> N >> M;

    vector<vector<int>> edges(N, vector<int>());

    for (int i = 0; i < M; ++i)
    {
        int u, v;

        cin >> u >> v;

        edges[u].push_back(v);
        edges[v].push_back(u);
    }

    freopen(argv[2], "r", stdin);

    string ans;
    cin >> ans;

    vector<int> selected(N);

    for (int i = 0; i < N; ++i) {
        int is_selected = ans[i] - '0';
        if (is_selected) {
            selected[i] = true;
            for (auto &j: edges[i]) {
                selected[j] = true;
            }
        }
    }

    for (int i = 0; i < N; ++i) {
        assert(selected[i]);
    }

    cout << "Vertex is covered (but might not optimal)" << "\n";
}