// ===============================
// Brute Force Approach
// ===============================

// Approach:
// For every positive integer starting from 1 up to n + 1 (where n is the size of the array),
// we search the entire array to see if it is present. The first integer that we cannot find
// in the array is the smallest missing positive integer.

// Algorithm:
// 1. Iterate with a candidate integer `candidate` starting from 1 up to `n + 1`.
// 2. For each `candidate`, perform a linear search in `nums`.
// 3. If `candidate` is not found in `nums`, return `candidate`.

// Time Complexity:
// O(n^2) where n is the size of the array. In the worst case, we search for 1, 2, ..., n + 1,
// performing a linear search of size n each time.

// Space Complexity:
// O(1) auxiliary space as we only use a few loop variables.

namespace BruteForce {
class Solution {
public:
    int firstMissingPositive(vector<int>& nums) {
        // Determine the array size to define the maximum search range.
        int n = nums.size();
        
        // The smallest missing positive integer must lie in the range [1, n + 1].
        for (int candidate = 1; candidate <= n + 1; ++candidate) {
            // Track if the current candidate integer exists in the input array.
            bool found = false;
            
            // Perform a linear search to check if the candidate is present.
            for (int num : nums) {
                // If the candidate is found, set the flag and exit the search early.
                if (num == candidate) {
                    found = true;
                    break;
                }
            }
            
            // If the candidate was not found in the array, it is the first missing positive.
            if (!found) {
                return candidate;
            }
        }
        
        // Fallback return, though one missing positive will always be found in the loop.
        return 1;
    }
};
}

// ===============================
// Better Approach
// ===============================

// Approach:
// We can optimize the search by storing all elements of the array in a hash set.
// This allows us to perform lookups in O(1) average time. We then check positive
// integers starting from 1 upwards until we find one that is not in the set.

// Algorithm:
// 1. Insert all elements of `nums` into an unordered_set.
// 2. Initialize `firstPos` to 1.
// 3. Keep incrementing `firstPos` if it is present in the set.
// 4. Return `firstPos` once we find a candidate not present in the set.

// Time Complexity:
// O(n) average time complexity, as inserting n elements into a hash set takes O(n) time,
// and lookup operations take O(1) average time.

// Space Complexity:
// O(n) auxiliary space to store the unique elements of `nums` in the hash set.

namespace Better {
class Solution {
public:
    int firstMissingPositive(vector<int>& nums) {
        // Store all elements in a hash set to achieve O(1) average lookup times.
        unordered_set<int> st(begin(nums), end(nums));
        
        // Start checking candidate integers from 1, the smallest positive integer.
        int firstPos = 1;
        
        // Keep searching incrementally until we find a positive integer that is not in the set.
        while (true) {
            // Check if the current candidate integer exists in our lookup set.
            if (st.find(firstPos) != st.end())
                // Increment the candidate to check the next integer since this one is present.
                firstPos++;
            else
                // Stop searching because the current candidate is not present in the set.
                break;
        }
        
        // Return the first positive integer that was missing from the set.
        return firstPos;
    }
};
}

// ===============================
// Optimal Approach
// ===============================

// Approach:
// To achieve O(n) time and O(1) auxiliary space, we place each number `nums[i]`
// at its correct index `nums[i] - 1` (i.e., placing 1 at index 0, 2 at index 1, etc.)
// using swap operations. This is also known as cycle sort.
// After placing all valid numbers in their correct positions, we scan the array.
// The first index `i` where `nums[i] != i + 1` tells us that `i + 1` is the missing positive.
// If all indices are correct, then `n + 1` is the missing positive.

// Algorithm:
// 1. Iterate through the array. For each element `nums[i]`, while it is in the range [1, n]
//    and not at its correct index (`nums[i] - 1`), swap it with the element at its correct index.
// 2. To avoid infinite loops, we ensure that we only swap if the target position contains a different value.
// 3. Scan the array from left to right. If `nums[i] != i + 1`, return `i + 1`.
// 4. If all elements are at their correct index, return `n + 1`.

// Time Complexity:
// O(n) because each swap operation places at least one element in its correct final position.
// There can be at most n such correct placements in total.

// Space Complexity:
// O(1) auxiliary space because the array is modified in-place.

namespace Optimal {
class Solution {
public:
    int firstMissingPositive(vector<int>& nums) {
        // Determine the array size to know the maximum possible contiguous positive integer range [1, n].
        int n = nums.size();
        
        // Traverse the array to place each valid element into its corresponding index position.
        for (int i = 0; i < n; i++) {
            // Cycle swap the current element into its correct target index (nums[i] - 1)
            // if it falls within the range [1, n] and doesn't duplicate the element already there.
            while (nums[i] > 0 && nums[i] <= n && nums[nums[i] - 1] != nums[i]) {
                // Swap nums[i] with the element at its correct index to place it properly.
                swap(nums[i], nums[nums[i] - 1]);
            }
        }
        
        // Iterate through the rearranged array to find the first position that contains an incorrect value.
        for (int i = 0; i < n; i++) {
            // If the element at index i is not i + 1, then i + 1 is the smallest missing positive integer.
            if (nums[i] != i + 1) {
                return i + 1;
            }
        }
        
        // If all positions are correct, all numbers from 1 to n are present, so the first missing positive is n + 1.
        return n + 1;
    }
};
}