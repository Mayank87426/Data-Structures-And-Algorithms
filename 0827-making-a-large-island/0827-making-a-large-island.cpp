class DisjointSet {
public:
    int n;
    vector<int> parent;
    vector<int> size;

    DisjointSet(int n) {
        this->n = n;
        parent.resize(n);
        size.resize(n);

        for (int i = 0; i < n; i++) {
            parent[i] = i;
            size[i] = 1;
        }
    }

    int findParent(int x) {
        if (parent[x] == x)
            return parent[x];

        return parent[x] = findParent(parent[x]);
    }

    void unionBySize(int x, int y) {
        int px = findParent(x);
        int py = findParent(y);

        if (px == py)
            return;

        if (size[px] >= size[py]) {
            parent[py] = px;
            size[px] += size[py];
        } else {
            parent[px] = py;
            size[py] += size[px];
        }
    }
};

class Solution {
public:
    vector<vector<int>> dir{{1, 0}, {-1, 0}, {0, 1}, {0, -1}};

    bool safe(int n, int m, int i, int j) {
        return i >= 0 && i < n && j >= 0 && j < m;
    }

    int findArea(vector<vector<int>>& grid, int i, int j, DisjointSet& ds) {
        int n = grid.size();
        int m = grid[0].size();

        grid[i][j] = 1;
        int ans = 1;

        vector<int> parents;

        for (auto d : dir) {
            int new_i = i + d[0];
            int new_j = j + d[1];

            if (!safe(n, m, new_i, new_j))
                continue;

            if (!grid[new_i][new_j])
                continue;

            int v = new_i * m + new_j;
            int pv = ds.findParent(v);

            if (find(parents.begin(), parents.end(), pv) == parents.end()) {
                ans += ds.size[pv];
                parents.push_back(pv);
            }
        }

        grid[i][j] = 0;

        return ans;
    }

    int largestIsland(vector<vector<int>>& grid) {
        int n = grid.size();
        int m = grid[0].size();

        int totalNodes = n * m;

        DisjointSet ds(totalNodes);

        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                if (!grid[i][j])
                    continue;

                int u = i * m + j;

                for (auto d : dir) {
                    int new_i = i + d[0];
                    int new_j = j + d[1];

                    if (!safe(n, m, new_i, new_j))
                        continue;

                    if (!grid[new_i][new_j])
                        continue;

                    int v = new_i * m + new_j;

                    int pu = ds.findParent(u);
                    int pv = ds.findParent(v);

                    if (pu != pv) {
                        ds.unionBySize(pu, pv);
                    }
                }
            }
        }

        int ans = 0;

        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                if (grid[i][j]) {
                    int p = ds.findParent(i * m + j);
                    ans = max(ans, ds.size[p]);
                }
            }
        }

        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                if (!grid[i][j]) {
                    ans = max(ans, findArea(grid, i, j, ds));
                }
            }
        }

        return ans;
    }
};