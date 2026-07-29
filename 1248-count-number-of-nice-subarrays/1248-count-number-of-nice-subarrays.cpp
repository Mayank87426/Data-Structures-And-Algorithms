// ===============================
// Brute Force Approach
// ===============================

// Approach:
// The brute force approach checks all possible subarrays of nums.
// We iterate through every possible starting position `i` and ending position `j` of a subarray.
// For each subarray, we count the number of odd numbers. If the count of odd numbers is exactly `k`,
// we increment our result.

// Algorithm:
// 1. Initialize a variable `count` to 0 to keep track of nice subarrays.
// 2. Loop through the array with a pointer `i` representing the start of the subarray.
// 3. For each `i`, initialize `oddCount` to 0.
// 4. Loop with a pointer `j` from `i` to `n - 1` representing the end of the subarray.
// 5. If `nums[j]` is odd, increment `oddCount`.
// 6. If `oddCount` equals `k`, increment `count`.
// 7. If `oddCount` exceeds `k`, we can break early since adding more elements can only increase or keep `oddCount` same.
// 8. Return `count`.

// Time Complexity:
// O(N^2) where N is the size of the array nums. We use two nested loops to explore all subarrays.

// Space Complexity:
// O(1) as we only use a few integer variables for counters.

namespace BruteForce {
class Solution {
public:
    int numberOfSubarrays(vector<int>& nums, int k) {
        int n = nums.size(); // Get the size of the array to define loop boundaries.
        int count = 0; // Initialize the total count of nice subarrays to return.
        
        // Loop with pointer i representing the starting index of a subarray.
        for (int i = 0; i < n; i++) {
            int oddCount = 0; // Track the number of odd numbers in the subarray starting at i.
            
            // Loop with pointer j representing the ending index of a subarray.
            for (int j = i; j < n; j++) {
                if (nums[j] % 2 != 0) { // If the current number is odd (non-zero remainder when divided by 2).
                    oddCount++; // Increment count of odd elements in the current subarray nums[i...j].
                }
                
                if (oddCount == k) { // If the current subarray has exactly k odd elements.
                    count++; // Increment count as it satisfies the nice subarray condition.
                } else if (oddCount > k) { // Since oddCount increases monotonically as j moves right.
                    break; // Break early to save unnecessary checks as any wider window will have > k odd numbers.
                }
            }
        }
        return count; // Return the accumulated number of nice subarrays.
    }
};
}

// ===============================
// Better Approach
// ===============================

// Approach:
// We can transform the problem by treating odd numbers as 1 and even numbers as 0.
// A subarray with exactly k odd numbers is equivalent to a subarray with a sum equal to k.
// We can use the prefix sum technique combined with a hash map (or a frequency array) to count the subarrays.
// As we iterate through nums, we keep track of the cumulative count of odd numbers (currentPrefixSum).
// The number of subarrays ending at the current index with exactly k odd numbers is the number of times 
// a prefix sum of (currentPrefixSum - k) has been seen so far.

// Algorithm:
// 1. Initialize `count` to 0, `currentPrefixSum` to 0.
// 2. Create a frequency array `prefixSumFreq` of size `n + 1` initialized to 0, where `n` is the size of `nums`.
// 3. Set `prefixSumFreq[0] = 1` because a prefix sum of 0 has occurred once initially.
// 4. Iterate through `nums` using a loop.
// 5. For each element, if it is odd, increment `currentPrefixSum`.
// 6. If `currentPrefixSum >= k`, add `prefixSumFreq[currentPrefixSum - k]` to `count`.
// 7. Increment the frequency of the current prefix sum: `prefixSumFreq[currentPrefixSum]++`.
// 8. Return `count`.

// Time Complexity:
// O(N) where N is the size of the array nums. We traverse the array once.

// Space Complexity:
// O(N) to store the prefix sum frequencies in a vector of size N + 1.

namespace Better {
class Solution {
public:
    int numberOfSubarrays(vector<int>& nums, int k) {
        int n = nums.size(); // Get the size of the array to size prefixSumFreq appropriately.
        int count = 0; // Initialize the count of nice subarrays.
        int currentPrefixSum = 0; // Running count of odd numbers encountered.
        vector<int> prefixSumFreq(n + 1, 0); // Stores the frequency of each prefix sum (max possible odds is n).
        
        prefixSumFreq[0] = 1; // Base case: A prefix sum of 0 (no odd numbers) is seen once before processing any element.
        
        // Iterate through each number in the array.
        for (int i = 0; i < n; i++) {
            if (nums[i] % 2 != 0) { // Check if the current element is odd.
                currentPrefixSum++; // Increment the running prefix sum of odd numbers.
            }
            if (currentPrefixSum >= k) { // If the total odd numbers from start to index i is at least k.
                count += prefixSumFreq[currentPrefixSum - k]; // Add the number of prefixes that had (currentPrefixSum - k) odds.
            }
            prefixSumFreq[currentPrefixSum]++; // Increment the frequency count of the current prefix sum.
        }
        return count; // Return the total number of nice subarrays found.
    }
};
}

// ===============================
// Optimal Approach
// ===============================

// Approach:
// The optimal approach uses a sliding window (two-pointer) technique.
// To find the number of subarrays with exactly k odd numbers, we can calculate:
// (number of subarrays with at most k odd numbers) - (number of subarrays with at most k - 1 odd numbers).
// The sliding window maintains a window [l, r] containing at most k odd numbers.
// For each position of the right pointer `r`, the number of valid subarrays ending at `r` is `r - l + 1`.

// Algorithm:
// 1. Implement a helper function `atMostKOddSubarrays` which computes the count of subarrays with at most `k` odd numbers.
// 2. In `atMostKOddSubarrays`:
//    a. Initialize `l` and `r` pointers to 0, `oddCount` to 0, and `count` to 0.
//    b. Expand the window by moving `r` from 0 to `n - 1`. If `nums[r]` is odd, increment `oddCount`.
//    c. If `oddCount` exceeds `k`, shrink the window from the left by moving `l` forward and decrementing `oddCount` if `nums[l]` is odd.
//    d. Add the number of valid subarrays ending at `r` (which is `r - l + 1`) to `count`.
//    e. Repeat until the end of the array.
// 3. In `numberOfSubarrays`, return the difference between `atMostKOddSubarrays(nums, k)` and `atMostKOddSubarrays(nums, k - 1)`.

// Time Complexity:
// O(N) where N is the size of the array nums. The helper function traverses the array with two pointers,
// each moving at most N times. We run it twice, which is 2 * O(N) = O(N).

// Space Complexity:
// O(1) as we only use a constant amount of extra space for pointers and counters.

namespace Optimal {
class Solution {
public:
    // Helper function to count all subarrays containing at most k odd numbers.
    int atMostKOddSubarrays(vector<int>& nums, int k) {
        int n = nums.size(); // Get the array size to control the right boundary.
        int l = 0, r = 0; // Initialize left and right pointers of the sliding window.
        int oddCount = 0; // Track the count of odd numbers within the current window [l, r].
        int count = 0; // Accumulate the total count of subarrays with at most k odd numbers.
        
        // Expand the right boundary of the window.
        while (r < n) {
            if (nums[r] % 2) // Check if the newly added element at the right boundary is odd.
                oddCount++; // Increment the odd count for the current window.
            
            // If the current window has more than k odd elements, shrink it from the left.
            while (l <= r && oddCount > k) {
                if (nums[l] % 2) // If the element leaving the window on the left is odd.
                    oddCount--; // Decrement the odd count of the window.
                l++; // Slide the left boundary to the right.
            }
            
            count += (r - l + 1); // Add the number of subarrays ending at r (each having at most k odds).
            r++; // Move the right pointer forward to continue checking.
        }
        return count; // Return the final count of subarrays with at most k odd numbers.
    }
    
    // Main function to count subarrays containing exactly k odd numbers.
    int numberOfSubarrays(vector<int>& nums, int k) {
        // Subtracting subarrays with at most (k-1) odds from at most k odds isolates subarrays with exactly k odds.
        return atMostKOddSubarrays(nums, k) - atMostKOddSubarrays(nums, k - 1);
    }
};
}