// ===============================
// Brute Force Approach
// ===============================

// Approach:
// The brute force approach checks all possible contiguous subarrays of the array.
// For each starting index `i`, we iterate through all possible ending indices `j` and
// maintain a running sum. If the running sum equals the `goal`, we increment our count.

// Algorithm:
// 1. Initialize `count` to 0 to store the number of valid subarrays.
// 2. Loop `i` from 0 to `n - 1` to define the start of the subarray.
// 3. For each `i`, initialize `currentSum` to 0.
// 4. Inner loop `j` from `i` to `n - 1` to define the end of the subarray.
// 5. Add `nums[j]` to `currentSum` and check if it equals `goal`.
// 6. If it equals `goal`, increment `count`.
// 7. Return `count` at the end.

// Time Complexity:
// O(N^2) where N is the size of the array, since we use two nested loops to explore all subarrays.

// Space Complexity:
// O(1) as we only use a few extra variables for indices and sums.

namespace BruteForce {
class Solution {
public:
    int numSubarraysWithSum(vector<int>& nums, int goal) {
        int n = nums.size(); // Get the size of the array to control loop boundaries.
        int count = 0; // Initialize a counter to track the number of subarrays meeting the goal.
        
        // Iterate through each potential starting position of a subarray.
        for (int i = 0; i < n; i++) {
            int currentSum = 0; // Reset sum for subarrays starting at index i.
            
            // Expand the subarray to the right, updating the sum incrementally.
            for (int j = i; j < n; j++) {
                currentSum += nums[j]; // Add the current element to the running sum.
                
                // If the sum matches the target goal, increment the valid subarray count.
                if (currentSum == goal) {
                    count++; // Increment count as we found a valid subarray.
                }
            }
        }
        return count; // Return the total count of valid subarrays found.
    }
};
}

// ===============================
// Better Approach
// ===============================

// Approach:
// This approach uses a prefix sum array/hash map to find subarrays with the target sum in O(N) time.
// Since the sum of a subarray nums[i...j] is prefixSum[j] - prefixSum[i-1], the condition
// prefixSum[j] - prefixSum[i-1] = goal can be rewritten as prefixSum[i-1] = prefixSum[j] - goal.
// By storing the frequency of all prefix sums in a hash map as we iterate, we can check in O(1) time
// if a prefix sum equal to (currentSum - goal) has occurred previously and how many times.

// Algorithm:
// 1. Maintain an unordered_map `prefixSumFreq` to store the frequency of each prefix sum encountered.
// 2. Initialize `prefixSumFreq[0] = 1` because a prefix sum of 0 is conceptually seen once before the array starts.
// 3. Initialize `currentSum` and `count` to 0.
// 4. Iterate through `nums`. For each element, add it to `currentSum`.
// 5. If `currentSum - goal` exists in our map, add its frequency to `count`.
// 6. Record the current `currentSum` in the map by incrementing its frequency.
// 7. Return `count`.

// Time Complexity:
// O(N) where N is the size of the array, since we iterate through the array once and perform O(1) average time hash map lookups.

// Space Complexity:
// O(N) for storing the prefix sums and their frequencies in the hash map.

namespace Better {
class Solution {
public:
    int numSubarraysWithSum(vector<int>& nums, int goal) {
        unordered_map<int, int> prefixSumFreq; // Map to store prefix sums and their frequencies.
        prefixSumFreq[0] = 1; // Base case: a prefix sum of 0 has occurred once initially.
        int currentSum = 0; // Variable to keep track of the running prefix sum.
        int count = 0; // Counter to accumulate the number of subarrays with the target sum.
        
        // Iterate through each number in the array to compute running prefix sums.
        for (int num : nums) {
            currentSum += num; // Update the running prefix sum.
            
            // Check if the prefix sum (currentSum - goal) has occurred before.
            // If it has, it means there are subarrays ending at the current index that sum to goal.
            if (prefixSumFreq.count(currentSum - goal)) {
                count += prefixSumFreq[currentSum - goal]; // Add the frequency of the required prefix sum to count.
            }
            
            // Record/update the frequency of the current prefix sum in the map.
            prefixSumFreq[currentSum]++;
        }
        return count; // Return the total number of valid subarrays.
    }
};
}

// ===============================
// Optimal Approach
// ===============================

// Approach:
// The optimal approach uses a sliding window (two pointers) in O(1) extra space.
// Since elements are only 0 and 1, the prefix sum is non-decreasing, which makes the sliding window applicable.
// When the window sum exceeds the goal or when the left element is 0 (which does not affect the sum), 
// we shrink the window from the left. By doing this, we keep track of how many prefix zeros 
// are within the current window. Each prefix zero allows us to form an additional valid subarray 
// with the same sum ending at the current right pointer `j`.

// Algorithm:
// 1. Initialize `prefixZeros` to count the leading zeros of the current valid window.
// 2. Use two pointers: `i` for the left boundary and `j` for the right boundary.
// 3. For each `j` from 0 to `n-1`, expand the window by adding `nums[j]` to `sum`.
// 4. Shrink the window from the left while `sum > goal` or `nums[i] == 0` (and `i < j`).
//    - If we shrink because of `sum > goal`, we subtract `nums[i]`, and reset `prefixZeros = 0` if `nums[i] == 1`.
//    - If `nums[i] == 0`, we increment `prefixZeros` as it represents a leading zero that can be excluded to form a new subarray.
// 5. If `sum == goal`, we add `1 + prefixZeros` to `count` (1 for the subarray starting at the current `i`, plus one for each leading zero).
// 6. Return `count`.

// Time Complexity:
// O(N) where N is the size of the array, since both pointers `i` and `j` traverse the array at most once.

// Space Complexity:
// O(1) as we only use a constant amount of extra space.

namespace Optimal {
class Solution {
public:
    int numSubarraysWithSum(vector<int>& nums, int goal) {
        int prefixZeros=0; // Count of consecutive zeros at the start of the current window.
        int n = nums.size(), sum = 0, count = 0, i = 0; // Initialize window pointers, size, sum, and count.
        
        // Expand the right boundary of the window.
        for (int j = 0; j < n; j++) {
            sum += nums[j]; // Include the current element in the window sum.
            
            // Shrink the window from the left if the sum exceeds the goal, or if there's a leading zero.
            // Leading zeros don't change the sum but affect subarray counts, so we count and skip them.
            while ((sum > goal || nums[i] == 0) && i < j) {
                sum -= nums[i]; // Remove the left element from the window sum.
                
                // If the removed element is zero, it increases the count of valid prefix zeros.
                if (nums[i] == 0) {
                    prefixZeros++; // Increment prefix zeros since removing it keeps the same sum.
                } else {
                    prefixZeros = 0; // Reset prefix zeros if we had to remove a 1 to reduce sum > goal.
                }
                i++; // Move the left pointer forward to shrink the window.
            }
            
            // If the current window sum matches the goal, add the valid subarrays to count.
            if (sum == goal) {
                count += 1 + prefixZeros; // Add 1 for the base window and prefixZeros for each leading zero subarray variation.
            }
        }
        return count; // Return the total count of binary subarrays with the target sum.
    }
};
}