// ===============================
// Brute Force Approach
// ===============================

// Approach:
// The brute force approach simulates shifting the grid by one position, repeated k times.
// In each shift step, we shift elements from left to right. The element at the end of a row
// shifts to the start of the next row, and the element at the bottom-right corner shifts
// to the top-left corner.
// We can optimize the number of shifts by taking k % (rows * cols).

// Algorithm:
// 1. Calculate grid dimensions: rows (m) and cols (n).
// 2. Calculate the total number of elements: total = rows * cols.
// 3. Reduce k using modulo: k %= total.
// 4. Repeat k times:
//    a. Create a temporary grid of size m x n.
//    b. For each element at (i, j), if it is not at the end of the row, move it to (i, j + 1).
//    c. If it is at the end of a row but not the last row, move it to (i + 1, 0).
//    d. If it is at the last row and last column, move it to (0, 0).
//    e. Update the grid with the temporary grid.
// 5. Return the shifted grid.

// Time Complexity:
// O(k * m * n) - where m is the number of rows, n is the number of columns, and k is the number of shifts.
// In the worst case, k can be up to m * n - 1, leading to O((m * n)^2) time complexity.

// Space Complexity:
// O(m * n) - to store the temporary grid during each shift operation.

namespace BruteForce {
class Solution {
public:
    vector<vector<int>> shiftGrid(vector<vector<int>>& grid, int k) {
        // Get the number of rows in the grid to determine vertical boundaries
        int rows = grid.size();
        // Get the number of columns in the grid to determine horizontal boundaries
        int cols = grid[0].size();
        // Calculate the total number of elements in the 2D grid
        int total = rows * cols;

        // Reduce k to prevent redundant full cycles around the grid
        k %= total;

        // Perform the single-shift simulation k times
        for (int shift = 0; shift < k; shift++) {
            // Create a temporary grid to hold the state after one shift
            vector<vector<int>> temp(rows, vector<int>(cols));

            // Traverse the grid to move each element one position to the right
            for (int i = 0; i < rows; i++) {
                // Traverse each column of the current row
                for (int j = 0; j < cols; j++) {
                    // Check if the current column is not the last column
                    if (j + 1 < cols) {
                        // Element moves to the next column in the same row
                        temp[i][j + 1] = grid[i][j];
                    // Check if the current row is not the last row
                    } else if (i + 1 < rows) {
                        // Element at the end of the row moves to the start of the next row
                        temp[i + 1][0] = grid[i][j];
                    // Otherwise, we are at the bottom-right corner
                    } else {
                        // Element at the bottom-right moves to the top-left (wrap around)
                        temp[0][0] = grid[i][j];
                    }
                }
            }
            // Update the grid with the shifted state
            grid = temp;
        }

        // Return the final shifted grid after all k operations
        return grid;
    }
};
}

// ===============================
// Better Approach
// ===============================

// Approach:
// The Better approach uses math/coordinate mapping to place each element directly in its final position in a single pass.
// By treating the 2D grid as a 1D array of size rows * cols, an element at (i, j) is at index `i * cols + j`.
// After shifting k times, its new 1D index is `(i * cols + j + k) % (rows * cols)`.
// We can map this new 1D index back to 2D coordinates and place the element in a new result grid.
// Note: This matches the original code's logic.

// Algorithm:
// 1. Get the dimensions: rows and cols.
// 2. Compute total elements: total = rows * cols.
// 3. Optimize k by taking k % total.
// 4. Create an output grid 'ans' of size rows x cols.
// 5. Loop through each cell (i, j) in the grid:
//    a. Compute its 1D index: idx = i * cols + j.
//    b. Compute the target 1D index: newIdx = (idx + k) % total.
//    c. Map newIdx back to 2D coordinates: row = newIdx / cols, col = newIdx % cols.
//    d. Assign grid[i][j] to ans[row][col].
// 6. Return 'ans'.

// Time Complexity:
// O(m * n) - We traverse each cell of the grid exactly once.

// Space Complexity:
// O(m * n) - A new grid 'ans' is allocated to store the shifted values.

namespace Better {
class Solution {
public:
    vector<vector<int>> shiftGrid(vector<vector<int>>& grid, int k) {
        // Get the number of rows in the grid to determine vertical boundaries
        int rows = grid.size();
        // Get the number of columns in the grid to determine horizontal boundaries
        int cols = grid[0].size();
        // Calculate the total number of elements in the 2D grid
        int total = rows * cols;

        // Reduce k to avoid redundant full grid rotations
        k %= total;

        // Allocate a new grid to store the shifted elements
        vector<vector<int>> ans(rows, vector<int>(cols));

        // Traverse each row of the grid
        for (int i = 0; i < rows; i++) {
            // Traverse each column of the current row
            for (int j = 0; j < cols; j++) {
                // Calculate the 1D flattened index representing the current 2D coordinate
                int idx = i * cols + j;
                // Calculate the new 1D index after shifting by k positions with wrap-around
                int newIdx = (idx + k) % total;

                // Place the current element into the calculated target position in the result grid
                ans[newIdx / cols][newIdx % cols] = grid[i][j];
            }
        }

        // Return the newly constructed grid containing all shifted elements
        return ans;
    }
};
}

// ===============================
// Optimal Approach
// ===============================

// Approach:
// The Optimal approach performs the shift in-place without allocating an extra 2D grid.
// This is achieved by treating the 2D grid as a 1D array and applying the array rotation reversal algorithm:
// 1. Reverse the first part of the array of size total - k (elements from index 0 to total - k - 1).
// 2. Reverse the second part of the array of size k (elements from index total - k to total - 1).
// 3. Reverse the entire array (elements from index 0 to total - 1).
// This achieves the right shift by k positions in-place.

// Algorithm:
// 1. Get the dimensions: rows and cols. Calculate total elements: total = rows * cols.
// 2. Optimize k by taking k % total.
// 3. If k is 0, return the grid immediately.
// 4. Define a helper function `reverse` to reverse a range of elements in the 2D grid using their 1D index mapping.
// 5. Reverse the range [0, total - k - 1].
// 6. Reverse the range [total - k, total - 1].
// 7. Reverse the entire range [0, total - 1].
// 8. Return the modified grid.

// Time Complexity:
// O(m * n) - Each element is visited and swapped a constant number of times.

// Space Complexity:
// O(1) auxiliary space - No new 2D grid is allocated; all operations are done in-place.

namespace Optimal {
class Solution {
private:
    // Reverses a range of elements in the grid, treated as a flattened 1D array
    void reverse(vector<vector<int>>& grid, int start, int end, int cols) {
        // Loop from both ends towards the middle
        while (start < end) {
            // Get the 2D coordinates for start and end 1D indices
            int r1 = start / cols, c1 = start % cols;
            int r2 = end / cols, c2 = end % cols;
            // Swap the elements manually to avoid external dependencies
            int temp = grid[r1][c1];
            grid[r1][c1] = grid[r2][c2];
            grid[r2][c2] = temp;
            // Advance start towards the center
            start++;
            // Decrement end towards the center
            end--;
        }
    }

public:
    vector<vector<int>> shiftGrid(vector<vector<int>>& grid, int k) {
        // Get the number of rows in the grid
        int rows = grid.size();
        // Get the number of columns in the grid
        int cols = grid[0].size();
        // Calculate the total number of elements in the grid
        int total = rows * cols;

        // Modulo k by total elements to get the effective shift amount
        k %= total;

        // If k is 0, no shift is needed, return the grid as is
        if (k == 0) {
            return grid;
        }

        // 1. Reverse the first part of the grid (from index 0 to total - k - 1)
        reverse(grid, 0, total - k - 1, cols);
        // 2. Reverse the second part of the grid (from index total - k to total - 1)
        reverse(grid, total - k, total - 1, cols);
        // 3. Reverse the entire grid (from index 0 to total - 1) to achieve final shift
        reverse(grid, 0, total - 1, cols);

        // Return the modified grid in-place
        return grid;
    }
};
}