// ===============================
// Brute Force Approach
// ===============================

// Approach:
// For each element in the circular array, we treat it as the starting element of a subarray.
// We then extend the subarray to include up to n elements (since a subarray cannot contain 
// duplicate elements from the same original index, i.e., length at most n). We calculate the sum 
// for all such subarrays and keep track of the maximum sum.

// Algorithm:
// 1. Loop `i` from 0 to `n-1` to choose the starting element.
// 2. Initialize `current_sum = 0`.
// 3. Loop `k` from 0 to `n-1` to choose the length of the subarray.
// 4. In each step of the inner loop, add `nums[(i + k) % n]` to `current_sum`.
// 5. Update `max_sum` with `current_sum`.
// 6. Return `max_sum`.

// Time Complexity:
// O(N^2) where N is the number of elements in the array.

// Space Complexity:
// O(1) auxiliary space.

namespace BruteForce {
class Solution {
public:
    int maxSubarraySumCircular(vector<int>& nums) {
        // Retrieve the size of the input array.
        // This helps us control the boundaries for starting positions and subarray lengths.
        int n = nums.size();
        
        // Initialize the global maximum sum to the minimum possible integer.
        // This ensures that any actual subarray sum we calculate will be larger than the initial value.
        int max_sum = INT_MIN;

        // Loop through each index in the array to act as the start of a circular subarray.
        // Since the array is circular, any element can be a valid starting point.
        for (int i = 0; i < n; i++) {
            // Initialize the running sum for subarrays starting at index i.
            // This will accumulate the sum of elements as we extend the subarray.
            int current_sum = 0;

            // Iterate through all possible lengths of the subarray from 1 to n.
            // A subarray cannot contain the same index twice, so the maximum length is n.
            for (int k = 0; k < n; k++) {
                // Add the current circular element to the running sum.
                // The modulo operator ensures the index wraps around to the beginning when it exceeds n-1.
                current_sum += nums[(i + k) % n];
                
                // Update the global maximum sum if the current subarray sum is greater.
                // This ensures we capture the maximum possible circular subarray sum across all starting points and lengths.
                max_sum = max(max_sum, current_sum);
            }
        }

        // Return the maximum sum found across all circular subarrays.
        // This guarantees that we return the optimal circular subarray sum found by brute force.
        return max_sum;
    }
};
}

// ===============================
// Better Approach
// ===============================

// Approach:
// A circular subarray can either wrap around the end of the array or not. 
// 1. If it does not wrap, standard Kadane's algorithm finds the maximum subarray sum.
// 2. If it wraps around, it consists of a suffix `nums[j...n-1]` and a prefix `nums[0...i]` where `i < j - 1`.
// We can compute the maximum prefix sum up to each index `i`. Then we iterate from right to left,
// computing the suffix sum from `j` to `n-1` and adding it to the maximum prefix sum ending at `j-1`.
// Finally, the result is the maximum of the non-wrapped sum and the wrapped sum.

// Algorithm:
// 1. Compute standard Kadane's algorithm to find the maximum subarray sum without wrapping, `max_kadane`.
// 2. Construct a `max_prefix` array of size `n` where `max_prefix[i]` is `max(prefix_sum[0...k])` for k <= i.
// 3. Initialize `suffix_sum = 0` and `max_wrapped = INT_MIN`.
// 4. Iterate `j` from `n-1` down to 1:
//    a. Add `nums[j]` to `suffix_sum`.
//    b. Update `max_wrapped` with `max(max_wrapped, suffix_sum + max_prefix[j-1])`.
// 5. Return `max(max_kadane, max_wrapped)`.

// Time Complexity:
// O(N) as we make a few linear passes over the array.

// Space Complexity:
// O(N) auxiliary space to store the `max_prefix` array.

namespace Better {
class Solution {
public:
    int maxSubarraySumCircular(vector<int>& nums) {
        // Retrieve the size of the input array.
        // This is needed to size our helper structures and control loop bounds.
        int n = nums.size();

        // Initialize variables for the standard Kadane's algorithm.
        // We need this to find the maximum sum of a subarray that does not wrap around.
        int maxEnding = nums[0];
        // Store the maximum non-wrapped subarray sum, initialized with the first element.
        int max_kadane = nums[0];

        // Run standard Kadane's algorithm to find the maximum subarray sum without wrapping.
        // This handles cases where the optimal subarray does not cross the boundary.
        for (int i = 1; i < n; i++) {
            // Either extend the previous subarray or start a new one at the current index.
            maxEnding = max(nums[i], maxEnding + nums[i]);
            // Update the maximum non-wrapped subarray sum found so far.
            max_kadane = max(max_kadane, maxEnding);
        }

        // Create an array to store the maximum prefix sum up to each index.
        // This is used to find the best prefix part of a wrapped subarray.
        vector<int> max_prefix(n);
        // Initialize the first prefix sum.
        int prefix_sum = nums[0];
        // Initialize the first entry of the maximum prefix sum array.
        max_prefix[0] = nums[0];

        // Compute prefix sums and the maximum prefix sums from left to right.
        // This helps us query the maximum prefix sum ending at or before any index in O(1) time.
        for (int i = 1; i < n; i++) {
            // Accumulate the prefix sum by adding the current element.
            prefix_sum += nums[i];
            // Store the maximum prefix sum seen up to index i.
            max_prefix[i] = max(max_prefix[i - 1], prefix_sum);
        }

        // Initialize variables to compute suffix sums from right to left.
        // We will combine these with prefix sums to find the maximum wrapped subarray.
        int suffix_sum = 0;
        // Initialize the maximum wrapped subarray sum to the minimum possible value.
        int max_wrapped = INT_MIN;

        // Iterate from right to left to evaluate all possible wrapped subarrays.
        // The wrapped subarray will consist of a suffix starting at index j and a prefix ending before j-1.
        for (int j = n - 1; j >= 1; j--) {
            // Accumulate the suffix sum by adding the current element.
            suffix_sum += nums[j];
            // Combine the current suffix sum with the maximum prefix sum ending before index j.
            // This computes the maximum wrapped subarray sum starting at j.
            max_wrapped = max(max_wrapped, suffix_sum + max_prefix[j - 1]);
        }

        // Return the maximum of the non-wrapped case and the wrapped case.
        // This covers all possible subarray configurations.
        return max(max_kadane, max_wrapped);
    }
};
}

// ===============================
// Optimal Approach
// ===============================

// Approach:
// There are only two possible configurations for the maximum circular subarray:
// 1. The maximum subarray does not wrap around.
//    - Find it using standard Kadane's algorithm.
// 2. The maximum subarray wraps around.
//    - Instead of finding what we take, find what we remove.
//    - Wrapped Sum = Total Sum - Minimum Subarray Sum.
// 
// Final Answer = max(Normal Kadane, Wrapped Sum).
//
// Edge Case:
// If all elements are negative, the minimum subarray is the entire array.
// This would mean Wrapped Sum = Total Sum - Total Sum = 0, representing an empty subarray,
// which is invalid. In this case, we ignore the wrapped answer and return the maximum element (from Kadane).

// Algorithm:
// 1. Calculate the total sum of the array.
// 2. Find the maximum subarray sum using standard Kadane's algorithm.
// 3. Find the minimum subarray sum using a modified Kadane's algorithm.
// 4. Calculate the maximum remaining (wrapped) sum as `totalSum - resMin`.
// 5. If `maxRemainingSum == 0`, it means all elements are negative. Set it to `INT_MIN` so it's ignored.
// 6. Return `max(resMax, maxRemainingSum)`.

// Time Complexity:
// O(N) as we traverse the array only once.

// Space Complexity:
// O(1) auxiliary space.

namespace Optimal {
class Solution {
public:
    int maxSubarraySumCircular(vector<int>& nums) {
        // Calculate the total sum of all elements in the array.
        // This is needed because the maximum circular subarray sum is equal to the total sum minus the minimum subarray sum.
        int totalSum = accumulate(nums.begin(), nums.end(), 0);

        // Initialize the maximum sum ending at the current index to the first element.
        // This acts as the starting point for standard Kadane's algorithm to find the maximum subarray sum.
        int maxEnding = nums[0];
        // Initialize the overall maximum subarray sum to the first element.
        // This will store the final result of the maximum non-wrapped subarray sum.
        int resMax = nums[0];

        // Initialize the minimum sum ending at the current index to the first element.
        // This acts as the starting point for finding the minimum subarray sum.
        int minEnding = nums[0];
        // Initialize the overall minimum subarray sum to the first element.
        // This will be subtracted from the total sum to find the maximum wrapped subarray sum.
        int resMin = nums[0];

        // Iterate through the array starting from the second element.
        // This allows us to dynamically compute maximum and minimum subarray sums using dynamic programming.
        for (int i = 1; i < nums.size(); i++) {

            // Determine the maximum subarray sum ending at the current index.
            // We either extend the existing subarray ending at i-1 or start a new subarray at index i.
            maxEnding = max(nums[i], maxEnding + nums[i]);
            // Update the global maximum subarray sum found so far.
            // This represents the maximum sum of a non-wrapped subarray.
            resMax = max(resMax, maxEnding);

            // Determine the minimum subarray sum ending at the current index.
            // We either extend the existing minimum subarray ending at i-1 or start a new one at index i.
            minEnding = min(nums[i], minEnding + nums[i]);
            // Update the global minimum subarray sum found so far.
            // This represents the minimum sum of a subarray to be removed.
            resMin = min(resMin, minEnding);
        }

        // Calculate the maximum sum if the subarray wraps around.
        // By subtracting the minimum subarray sum from the total sum, we get the sum of the remaining circular part.
        int maxRemainingSum = totalSum - resMin;

        // Check if the maximum remaining sum is zero.
        // If it is zero, it means the minimum subarray was the entire array, which would leave an empty subarray (invalid).
        if (maxRemainingSum == 0)
            // Set the remaining sum to INT_MIN to ensure it is ignored in the final comparison.
            maxRemainingSum = INT_MIN;

        // Return the maximum of the non-wrapped maximum sum and the wrapped maximum sum.
        // This compares both scenarios to give the absolute maximum circular subarray sum.
        return max(resMax, maxRemainingSum);
    }
};
}