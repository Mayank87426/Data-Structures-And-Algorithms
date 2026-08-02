class Solution {
public:
    vector<string> findAllRecipes(vector<string>& recipes,
                                  vector<vector<string>>& ingredients,
                                  vector<string>& supplies) {
        queue<string> q;
        unordered_map<string, int> indegree;
        for (auto s : supplies) {
            q.push(s);
        }
        unordered_map<string, vector<string>> adj;
        int n = recipes.size();
        for (int i = 0; i < n; i++) {
            auto req = ingredients[i];
            for (auto r : req) {
                indegree[recipes[i]]++;
                adj[r].push_back(recipes[i]);
            }
        }
        vector<string> dishes;
        while (!q.empty()) {
            auto d = q.front();
            q.pop();
            for (auto next : adj[d]) {
                indegree[next]--;
                if (indegree[next] == 0) {
                    q.push(next);
                    dishes.push_back(next);
                }
            }
        }
        return dishes;
    }
};