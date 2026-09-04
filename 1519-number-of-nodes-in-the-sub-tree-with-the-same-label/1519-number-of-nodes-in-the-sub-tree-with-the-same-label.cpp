class Solution {
public:
    vector<int> ans;

    vector<int> dfs(int node, int parent, vector<vector<int>>& adj,
                    string& labels) {
        vector<int> freq(26, 0);
        freq[labels[node] - 'a']++;

        for (auto x : adj[node]) {
            if (x == parent)
                continue;

            auto v = dfs(x, node, adj, labels);

            for (int i = 0; i < 26; i++) {
                freq[i] += v[i];
            }
        }

        ans[node] = freq[labels[node] - 'a'];
        return freq;
    }

    vector<int> countSubTrees(int n, vector<vector<int>>& edges,
                              string labels) {
        vector<vector<int>> adj(n);

        for (auto e : edges) {
            int u = e[0], v = e[1];
            adj[u].push_back(v);
            adj[v].push_back(u);
        }

        ans.assign(n, 0);

        dfs(0, -1, adj, labels);

        return ans;
    }
};
