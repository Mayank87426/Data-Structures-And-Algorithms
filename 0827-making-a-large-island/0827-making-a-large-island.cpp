// ===============================
// Brute Force Approach
// ===============================

// Approach:
// For every water cell (0) in the grid, we temporarily flip it to a land cell (1)
// and run a depth-first search (DFS) to find the size of the largest island in the
// modified grid. We keep track of the maximum island size seen across all possible flips.
// If the grid has no water cells, the maximum size is simply the total grid area.

// Algorithm:
// 1. Iterate through all coordinates (r, c) in the grid.
// 2. If grid[r][c] is 0, change it to 1.
// 3. Perform a complete grid traversal using DFS to find the size of the largest connected island.
// 4. Update the overall maximum island size with the result of this DFS.
// 5. Restore grid[r][c] back to 0.
// 6. If no 0 was found in the grid, return n * n. Otherwise, return the maximum size found.

// Time Complexity:
// O(n^4)

// Space Complexity:
// O(n^2)

namespace BruteForce {
class Solution {
private:
    // Helper function to perform Depth First Search (DFS) to find the size of a connected component of 1s.
    int dfs(vector<vector<int>>& grid, int r, int c, vector<vector<bool>>& visited) {
        int n = grid.size();
        
        // Return 0 if the coordinates are out of bounds, or if the cell is water, or if it is already visited.
        // This prevents out-of-bounds access and infinite recursion loop.
        if (r < 0 || r >= n || c < 0 || c >= n || grid[r][c] == 0 || visited[r][c]) {
            return 0;
        }
        
        // Mark the current cell as visited so it won't be counted again during this traversal.
        visited[r][c] = true;
        
        // Recursively traverse all four cardinal directions to calculate the total size of the island.
        int size = 1;
        size += dfs(grid, r - 1, c, visited); // Move up
        size += dfs(grid, r + 1, c, visited); // Move down
        size += dfs(grid, r, c - 1, visited); // Move left
        size += dfs(grid, r, c + 1, visited); // Move right
        
        // Return the accumulated size of the connected island component.
        return size;
    }

public:
    int largestIsland(vector<vector<int>>& grid) {
        int n = grid.size();
        int maxIsland = 0;
        bool hasZero = false; // Flag to track if we encountered any water cell.

        // Loop through every cell in the grid.
        for (int r = 0; r < n; ++r) {
            for (int c = 0; c < n; ++c) {
                // If a water cell is found, we try to flip it to 1 and measure the resulting island.
                if (grid[r][c] == 0) {
                    hasZero = true;
                    
                    // Temporarily flip the cell to 1 to simulate making an island larger.
                    grid[r][c] = 1;
                    
                    // Initialize a fresh visited matrix for DFS on the current grid state.
                    vector<vector<bool>> visited(n, vector<bool>(n, false));
                    int currentMax = 0;
                    
                    // Find the size of the largest island by starting DFS from each unvisited land cell.
                    for (int i = 0; i < n; ++i) {
                        for (int j = 0; j < n; ++j) {
                            if (grid[i][j] == 1 && !visited[i][j]) {
                                // Update current maximum island size found for this configuration.
                                currentMax = max(currentMax, dfs(grid, i, j, visited));
                            }
                        }
                    }
                    
                    // Update global maximum size across all possible single-cell flips.
                    maxIsland = max(maxIsland, currentMax);
                    
                    // Restore the cell back to water to keep grid integrity for future flips.
                    grid[r][c] = 0;
                }
            }
        }

        // If the grid has no zeros, the whole grid is one single island.
        if (!hasZero) {
            return n * n;
        }

        return maxIsland;
    }
};
}

// ===============================
// Better Approach
// ===============================

// Approach:
// Instead of recalculating island sizes from scratch for every flip, we precompute the sizes of all 
// existing islands. We run DFS to label each distinct island with a unique ID (starting from 2, 
// to distinguish them from water (0) and unvisited land (1)) and store their sizes in an array/vector.
// Then, we iterate through each water cell (0) and check its 4 neighbors. We collect the unique 
// island IDs of its neighbors, sum up their precalculated sizes, and add 1 (for the flipped cell itself).

// Algorithm:
// 1. Traverse the grid. For each unvisited land cell (1), perform DFS to label all connected cells 
//    with a new unique island ID (2, 3, etc.) and calculate its size.
// 2. Store the sizes of these islands in a lookup vector/map indexed by their island ID.
// 3. Track the maximum size of any single pre-existing island.
// 4. Iterate through all cells. For each water cell (0), query its 4 neighbors (up, down, left, right).
// 5. Use a set to collect the unique IDs of adjacent islands (to avoid double-counting the same island).
// 6. Sum the sizes of these unique neighboring islands, add 1, and update the global maximum island size.
// 7. Return the maximum island size.

// Time Complexity:
// O(n^2)

// Space Complexity:
// O(n^2)

namespace Better {
class Solution {
private:
    // Helper function to label a connected island with a unique ID and return its total size.
    int getIslandSize(vector<vector<int>>& grid, int r, int c, int id) {
        int n = grid.size();
        
        // Return 0 if we exceed grid boundaries or encounter a cell that is not part of the current unvisited island.
        if (r < 0 || r >= n || c < 0 || c >= n || grid[r][c] != 1) {
            return 0;
        }
        
        // Label the current cell with the unique island ID to group it and prevent re-visiting.
        grid[r][c] = id;
        
        // Sum the sizes of connected island cells in all four directions.
        int size = 1;
        size += getIslandSize(grid, r - 1, c, id); // Up
        size += getIslandSize(grid, r + 1, c, id); // Down
        size += getIslandSize(grid, r, c - 1, id); // Left
        size += getIslandSize(grid, r, c + 1, id); // Right
        
        // Return the total size calculated for this island ID.
        return size;
    }

public:
    int largestIsland(vector<vector<int>>& grid) {
        int n = grid.size();
        
        // Vector to store the size of each island, indexed by its unique island ID.
        // Since at most n*n components can exist, n*n + 2 is a safe upper bound.
        vector<int> islandSizes(n * n + 2, 0);
        int islandId = 2; // IDs start at 2 because 0 is water and 1 is unvisited land.
        int maxIsland = 0;

        // Step 1: Precalculate sizes of all islands and label them with distinct IDs.
        for (int r = 0; r < n; ++r) {
            for (int c = 0; c < n; ++c) {
                if (grid[r][c] == 1) {
                    // Find the total size of the island and assign it the current islandId.
                    int size = getIslandSize(grid, r, c, islandId);
                    islandSizes[islandId] = size;
                    
                    // Keep track of the largest island size without any flips.
                    maxIsland = max(maxIsland, size);
                    islandId++;
                }
            }
        }

        // Step 2: Try converting each water cell (0) to a land cell (1) and connect adjacent islands.
        for (int r = 0; r < n; ++r) {
            for (int c = 0; c < n; ++c) {
                if (grid[r][c] == 0) {
                    // Set to keep track of unique adjacent island IDs.
                    // This prevents double counting when a 0 is surrounded by the same island on multiple sides.
                    set<int> neighborIds;
                    
                    // Offsets for the 4 cardinal directions.
                    int drow[] = {-1, 0, 1, 0};
                    int dcol[] = {0, -1, 0, 1};
                    
                    // Check all 4 adjacent neighbors of the current water cell.
                    for (int k = 0; k < 4; ++k) {
                        int nr = r + drow[k];
                        int nc = c + dcol[k];
                        
                        // If neighbor is within bounds and belongs to a labeled island, record its ID.
                        if (nr >= 0 && nr < n && nc >= 0 && nc < n && grid[nr][nc] > 1) {
                            neighborIds.insert(grid[nr][nc]);
                        }
                    }
                    
                    // 1 represents the newly flipped cell itself.
                    int potentialSize = 1;
                    
                    // Add sizes of all unique neighboring islands.
                    for (int id : neighborIds) {
                        potentialSize += islandSizes[id];
                    }
                    
                    // Update global maximum island size after this simulated flip.
                    maxIsland = max(maxIsland, potentialSize);
                }
            }
        }

        // Return the largest possible island size obtained.
        return maxIsland;
    }
};
}

// ===============================
// Optimal Approach
// ===============================

// Approach:
// We use a Disjoint Set Union (DSU) data structure to connect adjacent land cells into islands.
// First, we run a union operation on all adjacent cells with a value of 1.
// Then, for every water cell (0), we check its four neighbors. Using DSU, we find the ultimate
// parent (representative) of each adjacent island. We insert these parents into a set to avoid
// double-counting. The total potential island size if we flip the water cell is 1 (for the flipped cell)
// plus the sum of sizes of these unique neighboring islands.
// Finally, we handle the edge case where no zeros exist by taking the maximum size of any island.

// Algorithm:
// 1. Initialize a DSU of size n * n. Each cell (r, c) is mapped to a unique 1D index (r * n + c).
// 2. Iterate through the grid. For each land cell (1), check its neighbors. If a neighbor is also land (1),
//    merge their sets using union by size.
// 3. For each water cell (0), check all 4 neighbors. For each adjacent land cell, query its ultimate
//    parent and insert it into a set.
// 4. Calculate the size of the combined island by summing the sizes of the unique component components
//    plus 1. Track the maximum size found.
// 5. In case there are no zeros (all cells are 1), iterate through all cells to find the size of the 
//    largest component parent. Return the overall maximum.

// Time Complexity:
// O(n^2 * alpha(n^2))

// Space Complexity:
// O(n^2)

namespace Optimal {

class DisjointSet {

public:
    // Dynamic arrays to hold rank, parent pointers, and sizes of subsets.
    vector<int> rank, parent, size;

    // Constructor to initialize disjoint set components for n elements.
    DisjointSet(int n) {
        // Resize subset metadata arrays to size n + 1 to handle 0-based and 1-based indexing options.
        rank.resize(n + 1, 0);
        parent.resize(n + 1);
        size.resize(n + 1);

        // Initially, each element is a singleton component of size 1 and acts as its own representative parent.
        for (int i = 0; i <= n; i++) {
            parent[i] = i; // Point parent pointer to itself to declare it as a root.
            size[i] = 1;   // Singleton components have size 1.
        }
    }

    // Find the representative (ultimate parent) of the subset containing 'node'.
    int findParent(int node) {
        // Base case: if 'node' is its own parent, we've found the representative root of this component.
        if (node == parent[node])
            return node;

        // Path compression: update parent pointer directly to the ultimate parent to optimize future queries.
        return parent[node] = findParent(parent[node]);
    }

    // Merge components containing 'u' and 'v' using rank-based comparison.
    void unionByRank(int u, int v) {

        // Retrieve the ultimate representatives of both components.
        int ulp_u = findParent(u);
        int ulp_v = findParent(v);

        // If they share the same ultimate parent, they already belong to the same component.
        if (ulp_u == ulp_v)
            return;

        // Attach the smaller rank tree under the larger rank tree to maintain balance.
        if (rank[ulp_u] < rank[ulp_v]) {
            parent[ulp_u] = ulp_v; // Parent of representative 'ulp_u' becomes 'ulp_v'.
        }
        else if (rank[ulp_v] < rank[ulp_u]) {
            parent[ulp_v] = ulp_u; // Parent of representative 'ulp_v' becomes 'ulp_u'.
        }
        else {
            parent[ulp_v] = ulp_u; // Arbitrarily attach 'ulp_v' under 'ulp_u'.
            rank[ulp_u]++;         // Increment rank as tree depth has increased.
        }
    }

    // Merge components containing 'u' and 'v' using size-based comparison.
    void unionBySize(int u, int v) {

        // Find the ultimate parents of the elements.
        int ulp_u = findParent(u);
        int ulp_v = findParent(v);

        // Avoid self-union if they already belong to the same island component.
        if (ulp_u == ulp_v)
            return;

        // Attach the smaller component to the larger one to keep tree depths minimal.
        if (size[ulp_u] < size[ulp_v]) {
            parent[ulp_u] = ulp_v; // Point smaller component's root to the larger one.
            size[ulp_v] += size[ulp_u]; // Accumulate sizes of both merged components.
        }
        else {
            parent[ulp_v] = ulp_u; // Point smaller component's root to the larger one.
            size[ulp_u] += size[ulp_v]; // Accumulate sizes of both merged components.
        }
    }
};

class Solution {
public:
    int largestIsland(vector<vector<int>>& grid) {

        int n = grid.size(); // The side length of the n x n grid.

        // Create disjoint set of size n * n to manage connectivity between grid coordinates.
        // A cell at (row, col) translates to the 1D index: row * n + col.
        DisjointSet ds(n * n);

        // =====================================================
        // STEP 1 : Connect all existing land cells (1's)
        // =====================================================
        //
        // Loop through all cells to build connected components of pre-existing islands.
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {

                // Skip water cells as we are only tracking connectivity of land cells (1s).
                if (grid[i][j] == 0)
                    continue;

                // Offsets for the 4 cardinal directions (up, left, down, right).
                int drow[] = {-1, 0, 1, 0};
                int dcol[] = {0, -1, 0, 1};

                // Check all 4 neighbours of the current land cell.
                for (int k = 0; k < 4; k++) {

                    int adjRow = i + drow[k]; // Neighbor cell row coordinate.
                    int adjCol = j + dcol[k]; // Neighbor cell column coordinate.

                    // Verify boundaries and check if the neighbor is also a land cell.
                    if (adjRow >= 0 && adjCol >= 0 &&
                        adjRow < n && adjCol < n &&
                        grid[adjRow][adjCol] == 1) {

                        int node = i * n + j; // Get 1D index of current cell.
                        int adjNode = adjRow * n + adjCol; // Get 1D index of neighbor cell.

                        // Union current cell's component with its neighbor's component to build the island.
                        ds.unionBySize(node, adjNode);
                    }
                }
            }
        }

        // =====================================================
        // STEP 2 : Try converting every 0 into 1
        // =====================================================
        //
        int mx = 0; // Tracks the maximum island size possible by flipping exactly one 0.

        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {

                // Skip land cells; we are only interested in simulating a flip on water cells (0s).
                if (grid[i][j] == 1)
                    continue;

                // Set to store the unique ultimate parent IDs of neighboring islands.
                // Using a set prevents double-counting the same island if it borders this water cell on multiple sides.
                set<int> components;

                int drow[] = {-1, 0, 1, 0};
                int dcol[] = {0, -1, 0, 1};

                // Find all neighbouring islands of the current water cell.
                for (int k = 0; k < 4; k++) {

                    int adjRow = i + drow[k]; // Neighbor cell row.
                    int adjCol = j + dcol[k]; // Neighbor cell col.

                    // Check boundaries and if the neighbor is part of a land island.
                    if (adjRow >= 0 && adjCol >= 0 &&
                        adjRow < n && adjCol < n &&
                        grid[adjRow][adjCol] == 1) {

                        // Insert the representative parent of the adjacent island.
                        components.insert(
                            ds.findParent(adjRow * n + adjCol)
                        );
                    }
                }

                // Sum sizes of all distinct neighbouring islands.
                int sizeTotal = 0;

                // Iterate through each unique adjacent component's parent and add its component size.
                for (auto parent : components) {
                    sizeTotal += ds.size[parent];
                }

                // Add 1 for the flipped cell itself and update the maximum island size found.
                mx = max(mx, sizeTotal + 1);
            }
        }

        // =====================================================
        // STEP 3 : Special case
        // =====================================================
        //
        // If the grid consists entirely of 1s, Step 2 is skipped.
        // We find the largest component already present in the grid.
        for (int i = 0; i < n * n; i++) {
            // Find representative parent of cell 'i' and compare its component size.
            mx = max(mx, ds.size[ds.findParent(i)]);
        }

        return mx; // Return the maximum island size.
    }
};

}