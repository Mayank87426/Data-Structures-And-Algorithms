class DisjointSet {

public:
    vector<int> rank, parent, size;

    // Constructor
    DisjointSet(int n) {
        rank.resize(n + 1, 0);
        parent.resize(n + 1);
        size.resize(n + 1);

        // Initially every node is its own parent
        // and every component has size = 1
        for (int i = 0; i <= n; i++) {
            parent[i] = i;
            size[i] = 1;
        }
    }

    // Find Ultimate Parent (with path compression)
    int findParent(int node) {
        if (node == parent[node])
            return node;

        return parent[node] = findParent(parent[node]);
    }

    // Union by Rank
    void unionByRank(int u, int v) {

        int ulp_u = findParent(u);
        int ulp_v = findParent(v);

        if (ulp_u == ulp_v)
            return;

        if (rank[ulp_u] < rank[ulp_v]) {
            parent[ulp_u] = ulp_v;
        }
        else if (rank[ulp_v] < rank[ulp_u]) {
            parent[ulp_v] = ulp_u;
        }
        else {
            parent[ulp_v] = ulp_u;
            rank[ulp_u]++;
        }
    }

    // Union by Size
    void unionBySize(int u, int v) {

        int ulp_u = findParent(u);
        int ulp_v = findParent(v);

        if (ulp_u == ulp_v)
            return;

        // Attach the smaller component to the larger one
        if (size[ulp_u] < size[ulp_v]) {
            parent[ulp_u] = ulp_v;
            size[ulp_v] += size[ulp_u];
        }
        else {
            parent[ulp_v] = ulp_u;
            size[ulp_u] += size[ulp_v];
        }
    }
};

class Solution {
public:
    int largestIsland(vector<vector<int>>& grid) {

        int n = grid.size();

        // Each cell is treated as one node.
        // Node number = row * n + col
        DisjointSet ds(n * n);

        // =====================================================
        // STEP 1 : Connect all existing land cells (1's)
        // =====================================================
        //
        // Build connected components of all islands.
        //
        // Example:
        // 1 1 0
        // 0 1 1
        //
        // After this loop, every island becomes one DSU component
        // and DSU stores its size.
        //

        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {

                if (grid[i][j] == 0)
                    continue;

                int drow[] = {-1, 0, 1, 0};
                int dcol[] = {0, -1, 0, 1};

                // Check all 4 neighbours
                for (int k = 0; k < 4; k++) {

                    int adjRow = i + drow[k];
                    int adjCol = j + dcol[k];

                    if (adjRow >= 0 && adjCol >= 0 &&
                        adjRow < n && adjCol < n &&
                        grid[adjRow][adjCol] == 1) {

                        int node = i * n + j;
                        int adjNode = adjRow * n + adjCol;

                        // Merge both land cells into one island
                        ds.unionBySize(node, adjNode);
                    }
                }
            }
        }

        // =====================================================
        // STEP 2 : Try converting every 0 into 1
        // =====================================================
        //
        // For every water cell:
        // Look at its 4 neighbours.
        //
        // It may connect multiple islands together.
        //
        // Example:
        //
        // 1 0 1
        //
        // Flipping middle 0 joins left and right islands.
        //
        // We collect unique parent components using a set,
        // so the same island isn't counted twice.
        //

        int mx = 0;

        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {

                if (grid[i][j] == 1)
                    continue;

                set<int> components;

                int drow[] = {-1, 0, 1, 0};
                int dcol[] = {0, -1, 0, 1};

                // Find all neighbouring islands
                for (int k = 0; k < 4; k++) {

                    int adjRow = i + drow[k];
                    int adjCol = j + dcol[k];

                    if (adjRow >= 0 && adjCol >= 0 &&
                        adjRow < n && adjCol < n &&
                        grid[adjRow][adjCol] == 1) {

                        // Store only ultimate parent
                        components.insert(
                            ds.findParent(adjRow * n + adjCol)
                        );
                    }
                }

                // Sum sizes of all distinct neighbouring islands
                int sizeTotal = 0;

                for (auto parent : components) {
                    sizeTotal += ds.size[parent];
                }

                // +1 for the flipped cell itself
                mx = max(mx, sizeTotal + 1);
            }
        }

        // =====================================================
        // STEP 3 : Special case
        // =====================================================
        //
        // What if the grid has no zero?
        //
        // Example:
        // 1 1
        // 1 1
        //
        // Step 2 never runs.
        //
        // Simply return the largest existing component.
        //

        for (int i = 0; i < n * n; i++) {
            mx = max(mx, ds.size[ds.findParent(i)]);
        }

        return mx;
    }
};