class Solution {
public:
    bool isAbleToReach(unordered_map<int, vector<int>>& adj, int u, int v,
                       vector<bool>& vis) {
        if (u == v)
            return true;
        vis[u] = true;
        for (auto nbr : adj[u]) {
            cout << nbr;
            if (!vis[nbr]) {
                if (isAbleToReach(adj, nbr, v, vis))
                    return true;
            }
        }
        return false;
    }
    bool equationsPossible(vector<string>& equations) {
        unordered_map<int, vector<int>> adj;
        for (auto s : equations) {

            if (s[1] == '!')
                continue;
            int u = s[0] - 'a', v = s[3] - 'a';
            adj[u].push_back(v);
            adj[v].push_back(u);
        }
        for (auto s : equations) {
            vector<bool> vis(26, false);
            if (s[1] == '!' && isAbleToReach(adj, s[0] - 'a', s[3] - 'a', vis))
                return false;
            if (s[1] == '=' && !isAbleToReach(adj, s[0] - 'a', s[3] - 'a', vis))
                return false;
        }
        return true;
    }
};