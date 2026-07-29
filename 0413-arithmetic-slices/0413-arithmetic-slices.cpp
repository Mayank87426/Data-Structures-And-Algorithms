// ===============================
// Brute Force Approach
// ===============================

// Approach:
// The brute force approach checks all possible contiguous subarrays of length 3 or more.
// For each subarray nums[i...j], we iterate through it and calculate the difference between 
// adjacent elements. If all adjacent elements in the subarray have the same difference,
// we count it as a valid arithmetic slice.

// Algorithm:
// 1. Iterate through all possible starting positions 'i' from 0 to n - 3.
// 2. For each starting position, iterate through all ending positions 'j' from i + 2 to n - 1.
// 3. For each candidate subarray nums[i...j], calculate the common difference 'diff = nums[i+1] - nums[i]'.
// 4. Verify if all consecutive pairs within nums[i...j] have the difference 'diff'.
// 5. If the validation passes, increment the total count of arithmetic slices.
// 6. Return the total count.

// Time Complexity:
// O(N^3) where N is the number of elements in 'nums'. There are O(N^2) subarrays of length >= 3, 
// and validating each subarray takes up to O(N) time.

// Space Complexity:
// O(1) auxiliary space as we only use a few tracking variables.

namespace BruteForce {
class Solution {
public:
    int numberOfArithmeticSlices(vector<int>& nums) {
        int n = nums.size(); // Retrieve the size of the array to control boundary limits for the search loops.
        int count = 0; // Initialize a running sum to record the number of valid arithmetic slices found.
        
        for (int i = 0; i < n - 2; i++) { // Loop to select the starting element of subarrays, stopping at n-3 because slices need at least 3 elements.
            for (int j = i + 2; j < n; j++) { // Loop to select the ending element of subarrays, starting at i+2 to guarantee a minimum length of 3.
                int diff = nums[i + 1] - nums[i]; // Compute the initial difference between the first two elements of the current subarray.
                bool isArithmetic = true; // Use a boolean flag to track whether the current subarray maintains a constant difference throughout.
                
                for (int k = i + 2; k <= j; k++) { // Traverse the remainder of the subarray from the third element onwards to verify differences.
                    if (nums[k] - nums[k - 1] != diff) { // Compare the current difference against the initial difference to detect any deviation.
                        isArithmetic = false; // Mark the subarray as invalid since the common difference has changed.
                        break; // Stop validating this subarray early to avoid redundant comparisons.
                    }
                }
                
                if (isArithmetic) { // Check if the subarray successfully maintained the arithmetic property.
                    count++; // Increment the total count of arithmetic slices.
                }
            }
        }
        
        return count; // Return the total number of arithmetic slices identified.
    }
};
}

// ===============================
// Better Approach
// ===============================

// Approach:
// The Better approach uses Dynamic Programming. We define 'dp[i]' as the number of arithmetic 
// slices ending at index 'i'. If the difference between nums[i] and nums[i-1] is the same as the 
// difference between nums[i-1] and nums[i-2], then any arithmetic slice ending at 'i-1' can be 
// extended to 'i' by appending nums[i]. Additionally, the triplet [nums[i-2], nums[i-1], nums[i]] 
// forms a new arithmetic slice. Therefore, dp[i] = dp[i-1] + 1. If the difference is not the 
// same, then no arithmetic slice can end at 'i', so dp[i] = 0. The total number of slices is 
// the sum of all values in the 'dp' array.

// Algorithm:
// 1. If the input array has less than 3 elements, return 0 as no arithmetic slice is possible.
// 2. Initialize a 'dp' array of size 'n' with all zeros.
// 3. Keep a 'total_slices' accumulator initialized to 0.
// 4. Iterate 'i' from 2 to n - 1.
// 5. If nums[i] - nums[i-1] == nums[i-1] - nums[i-2], set dp[i] = dp[i-1] + 1 and add dp[i] to 'total_slices'.
// 6. Return 'total_slices'.

// Time Complexity:
// O(N) where N is the number of elements in 'nums'. We iterate through the array once.

// Space Complexity:
// O(N) to store the 'dp' array of size N.

namespace Better {
class Solution {
public:
    int numberOfArithmeticSlices(vector<int>& nums) {
        int n = nums.size(); // Retrieve the size of the array to handle boundary checks and size initializations.
        if (n < 3) return 0; // Return 0 immediately because an arithmetic slice requires a minimum of 3 elements.
        
        vector<int> dp(n, 0); // Allocate a DP table where dp[i] stores the count of arithmetic slices ending exactly at index i.
        int total_slices = 0; // Initialize an accumulator to sum up all arithmetic slices found across the array.
        
        for (int i = 2; i < n; ++i) { // Start checking from index 2 since we need at least three elements to compare two consecutive differences.
            if (nums[i] - nums[i - 1] == nums[i - 1] - nums[i - 2]) { // Check if the current difference matches the previous difference.
                dp[i] = dp[i - 1] + 1; // If they match, extend all slices ending at i-1 by 1 and count the new triplet [i-2, i-1, i].
                total_slices += dp[i]; // Accumulate the slices ending at index i into the global count.
            }
        }
        
        return total_slices; // Return the total accumulated count of valid arithmetic slices.
    }
};
}

// ===============================
// Optimal Approach
// ===============================

// Approach:
// The Optimal approach optimizes the space complexity of the DP approach to O(1) using a sliding
// window/two-pointer approach. We maintain a window [i, j] representing the current contiguous 
// arithmetic progression. The pointer 'i' marks the start of the current arithmetic progression, 
// and 'j' extends the progression. For each element at 'j', if the difference matches the 
// established difference of the current sequence, we add (j - i - 1) to the count. This represents
// the number of new arithmetic slices ending at 'j' (where length L = j - i + 1, so L - 2 new slices).
// If the difference changes, we reset the window to start at 'j - 1' with 'j' starting there as well.

// Algorithm:
// 1. Initialize count to 0, start pointer 'i' and end pointer 'j' to 0, and 'diff' to INT_MAX.
// 2. While 'j' is less than the array size 'n':
//    a. Expand the window while elements maintain the same 'diff' (or if 'diff' is not yet established).
//    b. If i != j, compute/update 'diff' as nums[j] - nums[j - 1].
//    c. Add max(0, j - i - 1) to count.
//    d. Increment 'j'.
// 3. When the difference changes, reset 'diff' to INT_MAX, set 'i = j - 1', 'j = i', and repeat.
// 4. Return 'count'.

// Time Complexity:
// O(N) where N is the number of elements in 'nums'. Each element is visited at most twice by the pointers.

// Space Complexity:
// O(1) auxiliary space as only a few tracking variables (i, j, diff, count) are used.

namespace Optimal {
class Solution {
public:
    int numberOfArithmeticSlices(vector<int>& nums) {
        int n = nums.size(); // Retrieve the size of the input array to determine boundary conditions for the search loops.
        int count = 0; // Initialize a running total to accumulate the number of arithmetic slices found.
        int i = 0, j = 0; // Initialize sliding window pointers; 'i' marks the start of a sequence, and 'j' is the current element under evaluation.
        int diff = INT_MAX; // Store the common difference of the current arithmetic slice, initialized to a sentinel value to signify no difference has been established yet.

        while (j < n) { // Loop through the array to process each element as a potential member of an arithmetic subarray.

            while (j < n &&
                   (diff == INT_MAX || (nums[j] - nums[j - 1]) == diff)) { // Continue expanding the current window while the elements maintain the established common difference or when establishing the first difference.

                if (i != j) // Avoid calculating a difference when the window contains only one element, as we need at least two elements to compute a step difference.
                    diff = nums[j] - nums[j - 1]; // Define the target common difference for the current sequence based on the last two adjacent elements.

                count += max(0, j - i - 1); // For every new element added to the sequence of length L >= 3, add (L - 2) new arithmetic slices that end at index j.
                j++; // Move the window's leading pointer forward to test the next element in the array.
            }

            diff = INT_MAX; // Reset the common difference sentinel since the sequence has broken and a new difference needs to be computed.

            if (j == n) break; // Terminate the outer loop early if the end of the array is reached to avoid redundant window repositioning.

            i = j - 1; // Start the next potential arithmetic window from the last two elements of the current window, as they could begin a new sequence.
            j = i; // Reset the leading window pointer to align with the new start pointer for the next iteration.
        }

        return count; // Return the total accumulated count of valid arithmetic slices found in the array.
    }
};
}