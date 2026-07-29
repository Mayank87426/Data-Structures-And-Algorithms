// ===============================
// Brute Force Approach
// ===============================

// Approach:
// The brute force approach generates all possible contiguous subarrays of the given array.
// For each subarray, we iterate through its elements to find the minimum value.
// Finally, we sum up these minimums to obtain the answer.
// This is done using three nested loops.

// Algorithm:
// 1. Initialize `ans` to 0.
// 2. Iterate through all possible starting indices `i` from 0 to `n - 1`.
// 3. For each starting index `i`, iterate through all possible ending indices `j` from `i` to `n - 1`.
// 4. For each subarray from index `i` to `j`, find the minimum element by looping from `i` to `j`.
// 5. Add the minimum to `ans` modulo 10^9 + 7.
// 6. Return `ans`.

// Time Complexity:
// O(N^3) where N is the length of the array, as there are O(N^2) subarrays and finding the minimum takes O(N) time for each.

// Space Complexity:
// O(1) as we only use a few variables for counting and storing the minimum and final sum.

namespace BruteForce {
class Solution {
public:
    int sumSubarrayMins(vector<int>& arr) {
        int n = arr.size(); // Get the size of the array
        long long ans = 0;  // Initialize the sum accumulator
        const int MOD = 1e9 + 7; // Define modulo constant to avoid integer overflow

        // Iterate through all possible starting indices of subarrays
        for (int i = 0; i < n; i++) {
            // Iterate through all possible ending indices of subarrays
            for (int j = i; j < n; j++) {
                int min_val = arr[i]; // Start with the first element as the initial minimum
                // Scan the subarray from index i to j to find the actual minimum element
                for (int k = i; k <= j; k++) {
                    if (arr[k] < min_val) {
                        min_val = arr[k]; // Update the minimum if a smaller element is found
                    }
                }
                // Accumulate the minimum of the current subarray to the total sum
                ans = (ans + min_val) % MOD;
            }
        }

        return ans; // Return the final accumulated sum
    }
};
}

// ===============================
// Better Approach
// ===============================

// Approach:
// Instead of finding the minimum of each subarray from scratch in O(N) time, we can optimize the process
// by keeping track of the running minimum as we expand the subarray.
// Since the minimum of subarray `arr[i...j]` is `min(min(arr[i...j-1]), arr[j])`, we can compute it in O(1)
// time per subarray, reducing the overall time complexity to O(N^2).

// Algorithm:
// 1. Initialize `ans` to 0.
// 2. Loop through each starting index `i` from 0 to `n - 1`.
// 3. For each `i`, initialize `min_val` to `arr[i]`.
// 4. Loop through each ending index `j` from `i` to `n - 1`.
// 5. Update `min_val` as the minimum of `min_val` and `arr[j]`.
// 6. Add `min_val` to `ans` modulo 10^9 + 7.
// 7. Return `ans`.

// Time Complexity:
// O(N^2) because we use two nested loops to check all subarrays.

// Space Complexity:
// O(1) since we only maintain a few integer variables.

namespace Better {
class Solution {
public:
    int sumSubarrayMins(vector<int>& arr) {
        int n = arr.size(); // Get the size of the input array
        long long ans = 0;  // Initialize the sum accumulator
        const int MOD = 1e9 + 7; // Define modulo constant to avoid integer overflow

        // Traverse each possible starting index of subarrays
        for (int i = 0; i < n; i++) {
            int min_val = arr[i]; // Initialize the running minimum with the starting element
            // Expand the subarray to the right and update the running minimum
            for (int j = i; j < n; j++) {
                if (arr[j] < min_val) {
                    min_val = arr[j]; // Update the running minimum when a smaller element is encountered
                }
                // Add the running minimum of the subarray arr[i...j] to the total sum
                ans = (ans + min_val) % MOD;
            }
        }

        return ans; // Return the final accumulated sum
    }
};
}

// ===============================
// Optimal Approach
// ===============================

// Approach:
// The optimal approach uses a Monotonic Stack to find the number of subarrays in which each element `arr[i]`
// is the minimum. For each element `arr[i]`, we find the index of the next smaller element to its left (NSL)
// and the next smaller element to its right (NSR).
// The number of elements to the left that are strictly greater than (or equal to, to avoid duplicate counting)
// `arr[i]` is `left = i - NSL[i]`.
// The number of elements to the right that are strictly greater than `arr[i]` is `right = NSR[i] - i`.
// Thus, `arr[i]` is the minimum in `left * right` subarrays.
// We add `(left * right) * arr[i]` to the answer for each element.

// Algorithm:
// 1. Compute `NSL` (Next Smaller element on Left) for all elements using a monotonic stack.
//    To handle duplicate elements, we use `<=` comparison on the left so that we stop at elements that are smaller or equal.
// 2. Compute `NSR` (Next Smaller element on Right) for all elements using a monotonic stack.
//    We use `<` comparison on the right so that we stop only at strictly smaller elements.
// 3. For each index `i`, compute the distance to its NSL (`left = i - NSL[i]`) and to its NSR (`right = NSR[i] - i`).
// 4. Accumulate `(left * right % MOD) * arr[i] % MOD` into the answer.
// 5. Return the final answer.

// Time Complexity:
// O(N) where N is the size of the array, since each element is pushed and popped from the stack at most once.

// Space Complexity:
// O(N) to store the NSL, NSR arrays, and the stack.

namespace Optimal {
class Solution {
public:
    // Helper function to find the index of the Next Smaller Element on the Left (NSL)
    vector<int> findNSL(vector<int>& nums) {
        int n = nums.size(); // Get the size of the input array
        vector<int> NSL(n);  // Vector to store the index of the next smaller element on the left for each element
        stack<int> st;       // Monotonic stack to keep track of indices of elements in increasing order

        // Iterate through the array from left to right
        for (int i = 0; i < n; i++) {
            // Maintain monotonic stack property: pop elements that are greater than or equal to current element
            // Using <= handles duplicate elements on one side to prevent double counting of subarrays
            while (!st.empty() && nums[i] <= nums[st.top()]) {
                st.pop();    // Pop indices of elements that are not smaller than nums[i]
            }

            // If the stack is empty, no smaller element exists on the left; otherwise, the top of the stack is the NSL
            NSL[i] = st.empty() ? -1 : st.top();
            st.push(i);      // Push current index onto the stack for subsequent elements
        }

        return NSL;          // Return the calculated NSL array
    }

    // Helper function to find the index of the Next Smaller Element on the Right (NSR)
    vector<int> findNSR(vector<int>& nums) {
        int n = nums.size(); // Get the size of the input array
        vector<int> NSR(n);  // Vector to store the index of the next smaller element on the right for each element
        stack<int> st;       // Monotonic stack to keep track of indices of elements in increasing order

        // Iterate through the array from right to left
        for (int i = n - 1; i >= 0; i--) {
            // Maintain monotonic stack property: pop elements that are strictly greater than current element
            // Using < (strict comparison) here ensures we don't double count subarrays containing duplicate elements
            while (!st.empty() && nums[i] < nums[st.top()]) {
                st.pop();    // Pop indices of elements that are strictly greater than nums[i]
            }

            // If the stack is empty, no smaller element exists on the right; otherwise, the top of the stack is the NSR
            NSR[i] = st.empty() ? n : st.top();
            st.push(i);      // Push current index onto the stack for subsequent elements
        }

        return NSR;          // Return the calculated NSR array
    }

    // Main function to calculate the sum of subarray minimums
    int sumSubarrayMins(vector<int>& arr) {
        int n = arr.size();  // Get the size of the input array
        const int MOD = 1e9 + 7; // Define the modulo value to prevent integer overflow
        vector<int> NSL = findNSL(arr); // Compute indices of the next smaller element to the left
        vector<int> NSR = findNSR(arr); // Compute indices of the next smaller element to the right
        long long ans = 0;   // Initialize the answer variable to accumulate the sum

        // Compute the contribution of each element arr[i] to the total sum
        for (int i = 0; i < n; i++) {
            long long left = i - NSL[i];   // Calculate the number of valid subarrays ending at/including i to the left
            long long right = NSR[i] - i;  // Calculate the number of valid subarrays starting at/including i to the right

            // Calculate contribution: arr[i] is the minimum in (left * right) subarrays
            ans = (ans + (left * right % MOD) * arr[i]) % MOD;
        }

        return ans;          // Return the accumulated sum modulo 10^9 + 7
    }
};
}