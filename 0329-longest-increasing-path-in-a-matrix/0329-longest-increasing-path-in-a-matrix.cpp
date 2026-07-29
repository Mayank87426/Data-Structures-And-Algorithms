// ===============================
// Brute Force Approach
// ===============================

// Approach:
// The brute force approach performs a depth-first search (DFS) starting from every cell in the matrix.
// For each cell, we recursively explore all four directions (up, down, left, right).
// We only proceed to a neighboring cell if its value is strictly greater than the current cell's value.
// No memoization is used, leading to redundant calculations of paths for overlapping subproblems.

// Algorithm:
// 1. Iterate through every cell (i, j) in the matrix as a potential starting point.
// 2. For each cell, invoke a recursive helper function `solve` with the initial previous value set to -1.
// 3. In the helper function:
//    a. Check boundary conditions and verify if the current cell's value is greater than the previous cell's value. If not, return 0.
//    b. Recursively call `solve` for the four neighboring directions.
//    c. The longest increasing path starting at the current cell is 1 + the maximum of the results from the four directions.
// 4. Update the global maximum path length found.

// Time Complexity:
// O(4^(M * N)) in the worst case, as from each cell we can branch in up to 4 directions.

// Space Complexity:
// O(M * N) for the recursion call stack in the worst-case scenario.

namespace BruteForce {
class Solution {
public:
    // Helper function to perform recursive search from a cell (i, j)
    int solve(vector<vector<int>>& matrix, int i, int j, int prev) {
        // Retrieve the dimensions of the grid
        int n = matrix.size(), m = matrix[0].size();
        
        // Base case: if we go out of bounds or the current value is not strictly greater than the previous value, the path cannot continue
        if (i < 0 || j < 0 || i >= n || j >= m || matrix[i][j] <= prev)
            return 0;
            
        // Recursively compute the longest increasing path length starting from each of the 4 neighbors
        int up = solve(matrix, i - 1, j, matrix[i][j]);
        int left = solve(matrix, i, j - 1, matrix[i][j]);
        int down = solve(matrix, i + 1, j, matrix[i][j]);
        int right = solve(matrix, i, j + 1, matrix[i][j]);
        
        // Return 1 (for the current cell) plus the maximum length achievable from the neighboring paths
        return 1 + max({up, left, right, down});
    }
    
    int longestIncreasingPath(vector<vector<int>>& matrix) {
        // Retrieve the dimensions of the grid
        int n = matrix.size(), m = matrix[0].size();
        
        // Initialize the overall longest path length to at least 1 (any single cell is a path of length 1)
        int maxIncreasingPath = 1;
        
        // Try starting the path from every single cell in the grid to find the overall maximum
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                // Update the maximum path found so far by calling solve for each cell with previous value initialized to -1
                maxIncreasingPath =
                    max(maxIncreasingPath, solve(matrix, i, j, -1));
            }
        }
        return maxIncreasingPath;
    }
};
}

// ===============================
// Better Approach
// ===============================

// Approach:
// The Better approach uses memoization with a hash map (`unordered_map<string, int>`) to avoid redundant calculations of subproblems.
// We serialize the state `(i, j, prev)` into a unique string key. If we encounter a state that has already been computed,
// we return the cached result. This reduces the number of unique recursive evaluations.
// However, string serialization (`to_string`) and hash map lookups introduce significant overhead.
// This matches the exact logic of the original solution.

// Algorithm:
// 1. Define a global hash map `mp` to store the computed path length for each state.
// 2. Iterate through each cell as a potential starting point and call `solve(matrix, i, j, -1)`.
// 3. In the helper function `solve`:
//    a. Return 0 if the cell is out of bounds or doesn't satisfy the strictly increasing condition (`matrix[i][j] <= prev`).
//    b. Form a unique string cache key using the current coordinates `(i, j)` and the `prev` value.
//    c. If the key exists in the map `mp`, return the cached value.
//    d. Otherwise, recursively find the longest paths from the 4 neighbors.
//    e. Store the maximum path length plus 1 in the map `mp` and return it.

// Time Complexity:
// O(M * N) since each unique state is computed once, though string operations and hash map lookups add a large constant factor.

// Space Complexity:
// O(M * N) for the memoization map and recursion stack.

namespace Better {
class Solution {
public:
    // Hash map to cache the results of subproblems to avoid redundant DFS traversals
    unordered_map<string, int> mp;

    // Helper function to compute the longest increasing path starting from (i, j) with the restriction of matching the prev constraint
    int solve(vector<vector<int>>& matrix, int i, int j, int prev) {
        // Retrieve the dimensions of the grid
        int n = matrix.size(), m = matrix[0].size();

        // Check if the current position is out of grid boundaries or if the current element is not strictly greater than prev
        if (i < 0 || j < 0 || i >= n || j >= m || matrix[i][j] <= prev)
            return 0;

        // Serialize the state (i, j, prev) into a unique string key for hash map storage
        string key =
            to_string(i) + "_" + to_string(j) + "_" + to_string(prev) + "_";

        // Check if this subproblem state has already been solved and cached
        if (mp.count(key))
            return mp[key];

        // Perform recursive search in all four directions (up, left, down, right) using current value as the new prev limit
        int up = solve(matrix, i - 1, j, matrix[i][j]);
        int left = solve(matrix, i, j - 1, matrix[i][j]);
        int down = solve(matrix, i + 1, j, matrix[i][j]);
        int right = solve(matrix, i, j + 1, matrix[i][j]);

        // Memoize the result: 1 (for current cell) + maximum of paths from the four directions, and return it
        return mp[key] = 1 + max({up, left, right, down});
    }

    int longestIncreasingPath(vector<vector<int>>& matrix) {
        // Retrieve the dimensions of the grid
        int n = matrix.size(), m = matrix[0].size();

        // Initialize the overall longest path length to at least 1 (any single cell is a path of length 1)
        int maxIncreasingPath = 1;

        // Iterate through all grid cells to consider each cell as a potential starting point of the path
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                // Update the maximum path found so far by calling solve for each cell with previous value initialized to -1
                maxIncreasingPath =
                    max(maxIncreasingPath, solve(matrix, i, j, -1));
            }
        }

        // Return the length of the longest increasing path found in the matrix
        return maxIncreasingPath;
    }
};
}

// ===============================
// Optimal Approach
// ===============================

// Approach:
// The Optimal approach optimizes the memoization scheme.
// Instead of caching on (i, j, prev), we observe that the longest increasing path starting at cell (i, j)
// is independent of how we reached cell (i, j). Thus, the state only depends on (i, j).
// We can use a 2D integer array/vector `memo[i][j]` instead of a hash map with string keys.
// This completely eliminates string serialization and hashing overhead, making access O(1) and extremely fast.

// Algorithm:
// 1. Initialize a 2D array `memo` of size M x N with 0, indicating all states are unvisited.
// 2. Iterate through every cell (i, j) in the matrix.
// 3. For each cell, call `solve(matrix, i, j, memo)`.
// 4. In `solve`:
//    a. If `memo[i][j]` is already computed (non-zero), return it immediately.
//    b. Check all four directions. If a neighbor is valid and has a strictly greater value, recursively compute its longest increasing path.
//    c. Store `1 + max(path length from valid greater neighbors)` in `memo[i][j]` and return it.
// 5. Find the maximum value in `memo` to get the global longest increasing path.

// Time Complexity:
// O(M * N) since each cell is processed exactly once, and each transition takes O(1) time.

// Space Complexity:
// O(M * N) for the 2D memoization table and recursion stack.

namespace Optimal {
class Solution {
public:
    // Helper function to find the longest increasing path starting at (i, j)
    int solve(vector<vector<int>>& matrix, int i, int j, vector<vector<int>>& memo) {
        // If the result for the current cell has already been calculated, return it
        if (memo[i][j] != 0)
            return memo[i][j];

        // Retrieve dimensions of the grid
        int n = matrix.size(), m = matrix[0].size();
        
        // Initialize path lengths from neighbors to 0
        int up = 0, down = 0, left = 0, right = 0;

        // Check if moving up is valid and the destination cell has a strictly greater value
        if (i - 1 >= 0 && matrix[i - 1][j] > matrix[i][j])
            up = solve(matrix, i - 1, j, memo);

        // Check if moving down is valid and the destination cell has a strictly greater value
        if (i + 1 < n && matrix[i + 1][j] > matrix[i][j])
            down = solve(matrix, i + 1, j, memo);

        // Check if moving left is valid and the destination cell has a strictly greater value
        if (j - 1 >= 0 && matrix[i][j - 1] > matrix[i][j])
            left = solve(matrix, i, j - 1, memo);

        // Check if moving right is valid and the destination cell has a strictly greater value
        if (j + 1 < m && matrix[i][j + 1] > matrix[i][j])
            right = solve(matrix, i, j + 1, memo);

        // Cache the result: 1 (for the current cell) plus the maximum of the paths from all valid neighbors
        return memo[i][j] = 1 + max({up, left, right, down});
    }

    int longestIncreasingPath(vector<vector<int>>& matrix) {
        // Retrieve dimensions of the grid
        int n = matrix.size(), m = matrix[0].size();
        
        // Initialize a 2D memoization table with 0 (representing unvisited/uncomputed states)
        vector<vector<int>> memo(n, vector<int>(m, 0));
        
        // Initialize overall maximum increasing path length to 0
        int maxIncreasingPath = 0;

        // Find the longest increasing path starting from each cell in the grid
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                // Keep track of the global maximum path length
                maxIncreasingPath = max(maxIncreasingPath, solve(matrix, i, j, memo));
            }
        }

        // Return the global maximum path length
        return maxIncreasingPath;
    }
};
}