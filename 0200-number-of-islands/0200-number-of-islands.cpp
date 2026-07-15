class DisJointSet {
    vector<int> parent, size;

public:
    DisJointSet(int n) {
        parent.resize(n + 1);
        size.resize(n + 1, 1);
        for (int i = 0; i <= n; i++) {
            parent[i] = i;
        }
    }

    int findParent(int x) {
        if (parent[x] == x)
            return x;
        return parent[x] = findParent(parent[x]);
    }

    void unionBySize(int u, int v) {
        int rootU = findParent(u);
        int rootV = findParent(v);

        if (rootU == rootV)
            return;

        if (size[rootU] < size[rootV]) {
            parent[rootU] = rootV;
            size[rootV] += size[rootU];
        } else {
            parent[rootV] = rootU;
            size[rootU] += size[rootV];
        }
    }
};

class Solution {
public:
    bool valid(int n, int m, int x, int y) {
        return x >= 0 && x < n && y >= 0 && y < m;
    }
    
    int numIslands(vector<vector<char>>& grid) {
        int n = grid.size(), m = grid[0].size();
        DisJointSet ds(n * m);
        vector<vector<int>> dir = {{-1, 0}, {1, 0}, {0, 1}, {0, -1}};
        int count = 0;
        
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                if (grid[i][j] == '1') {
                    count++;
                    
                    for (auto d : dir) {
                        int new_i = i + d[0], new_j = j + d[1];
                        
                        if (!valid(n, m, new_i, new_j))
                            continue;
                            
                        if (grid[new_i][new_j] == '1') {
                            int x = i * m + j;
                            int y = new_i * m + new_j;
                            
                            if (ds.findParent(x) != ds.findParent(y)) {
                                ds.unionBySize(x, y);
                                count--;
                            }
                        }
                    }
                }
            }
        }
        return count;
    }
};