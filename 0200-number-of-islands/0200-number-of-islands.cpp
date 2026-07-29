// ===============================
// Brute Force Approach
// ===============================

// Approach:
// The brute-force approach finds the number of islands by traversing the grid and using 
// an auxiliary 2D boolean array to keep track of visited cells. This avoids mutating 
// the input grid. For every cell that is land ('1') and has not been visited, we increment 
// our island count and trigger a Depth-First Search (DFS) to mark all adjacent land 
// cells as visited.

// Algorithm:
// 1. Initialize a 2D boolean array `visited` of size m x n to all false.
// 2. Iterate through each cell of the grid.
// 3. If cell (i, j) is land ('1') and is not visited:
//    a. Increment the island counter.
//    b. Run DFS starting from (i, j) to recursively visit all adjacent land cells (up, down, left, right) and mark them as visited.
// 4. Return the island count.

// Time Complexity:
// O(m * n) where m is the number of rows and n is the number of columns. We visit every cell a constant number of times.

// Space Complexity:
// O(m * n) due to the auxiliary visited array, plus the DFS recursion stack in the worst case (when the grid is entirely land).

namespace BruteForce {
class Solution {
private:
    // Helper function to check if cell coordinates (x, y) lie within the grid boundaries
    bool valid(int n, int m, int x, int y) {
        // Ensure row index and column index are non-negative and strictly less than grid dimensions
        return x >= 0 && x < n && y >= 0 && y < m;
    }

    // Depth-First Search to mark all connected land cells as visited in the external matrix
    void dfs(int r, int c, vector<vector<char>>& grid, vector<vector<bool>>& visited) {
        // Mark the current land cell as visited in our external lookup table
        visited[r][c] = true;
        // Direction offsets for checking up, down, left, and right neighbors
        int dr[] = {-1, 1, 0, 0};
        int dc[] = {0, 0, -1, 1};

        // Check all 4 adjacent directions
        for (int i = 0; i < 4; i++) {
            // Compute neighbor coordinates
            int nr = r + dr[i];
            int nc = c + dc[i];

            // Visit neighbor if it's within bounds, is land, and hasn't been visited yet
            if (valid(grid.size(), grid[0].size(), nr, nc) && grid[nr][nc] == '1' && !visited[nr][nc]) {
                // Recursively traverse connected land cells
                dfs(nr, nc, grid, visited);
            }
        }
    }

public:
    int numIslands(vector<vector<char>>& grid) {
        // If the grid is empty, there are no islands to find
        if (grid.empty()) return 0;
        // Get the row and column dimensions of the grid
        int n = grid.size(), m = grid[0].size();
        // Create an auxiliary visited table to avoid modifying the input grid directly
        vector<vector<bool>> visited(n, vector<bool>(m, false));
        // Counter to keep track of the number of connected components (islands)
        int count = 0;

        // Traverse each cell of the grid
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                // If we encounter an unvisited land cell, we found a new island
                if (grid[i][j] == '1' && !visited[i][j]) {
                    // Increment the counter for the newly discovered island
                    count++;
                    // Run DFS to mark all cells belonging to this island
                    dfs(i, j, grid, visited);
                }
            }
        }
        // Return the total number of disjoint islands found
        return count;
    }
};
}

// ===============================
// Better Approach
// ===============================

// Approach:
// The Better approach optimizes memory usage by eliminating the auxiliary visited 
// array. Instead, we mark visited land cells in-place by changing '1' to '0' (sinking 
// the island). When a land cell is encountered, we increment the island count and 
// trigger a DFS to sink all connected land cells.

// Algorithm:
// 1. Iterate through each cell of the grid.
// 2. If cell (i, j) is land ('1'):
//    a. Increment the island counter.
//    b. Run DFS starting from (i, j).
// 3. Inside DFS:
//    a. Mark current cell as water ('0').
//    b. Recursively check all 4 neighbors and run DFS on any land cell.
// 4. Return the island count.

// Time Complexity:
// O(m * n) where m is rows and n is columns, visiting each cell at most a constant number of times.

// Space Complexity:
// O(m * n) in the worst case for the recursion stack if the grid is filled with land. Auxiliary space is O(1).

namespace Better {
class Solution {
private:
    // Helper function to check if cell coordinates (x, y) lie within the grid boundaries
    bool valid(int n, int m, int x, int y) {
        // Ensure row index and column index are non-negative and strictly less than grid dimensions
        return x >= 0 && x < n && y >= 0 && y < m;
    }

    // Depth-First Search to sink the connected island by modifying grid cells in-place
    void dfs(int r, int c, vector<vector<char>>& grid) {
        // Mark the cell as water ('0') to indicate it has been visited
        grid[r][c] = '0';
        // Direction offsets for moving to adjacent cells
        int dr[] = {-1, 1, 0, 0};
        int dc[] = {0, 0, -1, 1};

        // Recurse on all 4 valid neighbors
        for (int i = 0; i < 4; i++) {
            // Compute neighbor coordinates
            int nr = r + dr[i];
            int nc = c + dc[i];

            // If neighbor is within bounds and is land, recursively sink it
            if (valid(grid.size(), grid[0].size(), nr, nc) && grid[nr][nc] == '1') {
                // Recursively traverse connected land cells
                dfs(nr, nc, grid);
            }
        }
    }

public:
    int numIslands(vector<vector<char>>& grid) {
        // If the grid is empty, return zero islands
        if (grid.empty()) return 0;
        // Get the row and column dimensions of the grid
        int n = grid.size(), m = grid[0].size();
        // Counter for counting independent islands
        int count = 0;

        // Scan the entire grid cell by cell
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                // When land ('1') is encountered, it triggers a new island discovery
                if (grid[i][j] == '1') {
                    // Increment the counter for the newly discovered island
                    count++;
                    // Sink all connected land parts using in-place DFS
                    dfs(i, j, grid);
                }
            }
        }
        // Return the total number of disjoint islands found
        return count;
    }
};
}

// ===============================
// Optimal Approach
// ===============================

// Approach:
// The optimal approach uses a Disjoint Set Union (DSU) / Union-Find data structure.
// This matches the original solution's logic. We treat each land cell as an individual 
// component and map the 2D grid coordinates to a 1D representation. We increment our 
// count for each land cell. For every land cell, we check its neighbors. If a neighbor 
// is also land and belongs to a different set, we union them and decrement the island count.

// Algorithm:
// 1. Initialize a DSU representing each grid cell as a separate component.
// 2. Iterate through each cell (i, j) in the grid:
//    a. If cell is '1', increment island count.
//    b. Check neighbors (up, down, left, right).
//    c. If a neighbor is valid and is land ('1'):
//       - Map current cell and neighbor cell to 1D indices.
//       - Find parent representatives. If they differ, union the sets and decrement the island count.
// 3. Return the final island count.

// Time Complexity:
// O(m * n * alpha(m * n)) where alpha is the inverse Ackermann function, which is nearly constant O(1).

// Space Complexity:
// O(m * n) to store parent and size vectors in the Disjoint Set Union data structure.

namespace Optimal {
class DisJointSet {
    // parent array keeps track of the representative/parent of each cell, size array is used for union-by-size optimization
    vector<int> parent, size;

public:
    // Constructor initializes DSU for n elements (0 to n)
    DisJointSet(int n) {
        // Allocate space for n + 1 elements to handle 0-based indexing up to n
        parent.resize(n + 1);
        // Initialize size of each set to 1 as each element starts as its own component
        size.resize(n + 1, 1);
        // Set parent of each node to itself initially
        for (int i = 0; i <= n; i++) {
            parent[i] = i;
        }
    }

    // Finds the representative parent of a cell with path compression
    int findParent(int x) {
        // If x is its own parent, x is the representative of the set
        if (parent[x] == x)
            return x;
        // Recursively find the root and compress path by directly connecting x to the root for O(1) subsequent lookups
        return parent[x] = findParent(parent[x]);
    }

    // Unites two sets containing u and v based on their sizes to keep the tree balanced
    void unionBySize(int u, int v) {
        // Find ultimate representatives of both elements
        int rootU = findParent(u);
        int rootV = findParent(v);

        // If they already share the same representative, they belong to the same set, so no action is needed
        if (rootU == rootV)
            return;

        // Attach the smaller tree under the larger tree to keep tree height minimal
        if (size[rootU] < size[rootV]) {
            parent[rootU] = rootV; // Make rootV the parent of rootU
            size[rootV] += size[rootU]; // Update the size of the combined set
        } else {
            parent[rootV] = rootU; // Make rootU the parent of rootV
            size[rootU] += size[rootV]; // Update the size of the combined set
        }
    }
};

class Solution {
public:
    // Helper function to check if cell coordinates (x, y) lie within the grid boundaries
    bool valid(int n, int m, int x, int y) {
        // Ensure row index and column index are non-negative and strictly less than grid dimensions
        return x >= 0 && x < n && y >= 0 && y < m;
    }
    
    // Calculates the total number of islands using Disjoint Set Union (DSU)
    int numIslands(vector<vector<char>>& grid) {
        // n is rows count, m is columns count of the grid
        int n = grid.size(), m = grid[0].size();
        // Initialize DSU with n * m elements, representing all cells mapped to a 1D index space
        DisJointSet ds(n * m);
        // Directions array containing offset coordinates for moving Up, Down, Right, and Left
        vector<vector<int>> dir = {{-1, 0}, {1, 0}, {0, 1}, {0, -1}};
        // Counter to track the net number of disjoint island sets
        int count = 0;
        
        // Traverse through each cell in the 2D grid
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                // Process the cell only if it represents land
                if (grid[i][j] == '1') {
                    // Treat each land cell as an independent island initially
                    count++;
                    
                    // Explore all four neighboring directions
                    for (auto d : dir) {
                        // Compute neighbor's coordinates
                        int new_i = i + d[0], new_j = j + d[1];
                        
                        // Ignore the neighbor if it is outside grid boundaries
                        if (!valid(n, m, new_i, new_j))
                            continue;
                            
                        // If the neighbor is also land, try to merge it
                        if (grid[new_i][new_j] == '1') {
                            // Map 2D grid coordinates (i, j) to 1D index for DSU lookup
                            int x = i * m + j;
                            // Map neighbor's 2D coordinates to 1D index
                            int y = new_i * m + new_j;
                            
                            // Check if the current cell and neighbor are in different sets
                            if (ds.findParent(x) != ds.findParent(y)) {
                                // Merge the two disjoint sets into a single set
                                ds.unionBySize(x, y);
                                // Decrement count because two previously disjoint components are now connected
                                count--;
                            }
                        }
                    }
                }
            }
        }
        // Return the final count of disjoint sets, representing the number of islands
        return count;
    }
};
}