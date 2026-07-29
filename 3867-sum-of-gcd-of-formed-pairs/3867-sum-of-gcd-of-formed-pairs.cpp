// ===============================
// Brute Force Approach
// ===============================

// Approach:
// For each element, we find the maximum value from the start of the array up to that element's index by scanning the prefix subarray.
// We then compute the GCD of the element and this maximum. Finally, we sort the array of GCDs and use a two-pointer approach to pair 
// the smallest and largest elements, summing their GCDs.

// Algorithm:
// 1. Iterate i from 0 to n - 1.
// 2. For each i, run a nested loop j from 0 to i to find the maximum element mx.
// 3. Set prefixGcd[i] = gcd(nums[i], mx).
// 4. Sort the prefixGcd array.
// 5. Initialize pointers l = 0 and r = n - 1.
// 6. While l < r, compute gcd(prefixGcd[l], prefixGcd[r]), add it to the sum, and increment l and decrement r.
// 7. Return the accumulated sum.

// Time Complexity:
// O(n^2) due to the nested loop to compute prefix maximums.

// Space Complexity:
// O(n) to store the prefixGcd array.

namespace BruteForce {
class Solution {
public:
    long long gcdSum(vector<int>& nums) {
        int n = nums.size(); // Store the size of the array for boundary checks and iterations
        vector<int> prefixGcd(n); // Vector to store GCD of each element with the prefix maximum
        
        for (int i = 0; i < n; i++) { // Loop through each element to construct the prefix GCD array
            int mx = INT_MIN; // Initialize the maximum value for the subarray nums[0...i]
            for (int j = 0; j <= i; j++) { // Scan the prefix subarray to find the maximum element
                mx = max(mx, nums[j]); // Track the maximum value found in the subarray nums[0...i]
            }
            prefixGcd[i] = __gcd(mx, nums[i]); // Calculate GCD of nums[i] and mx as required by the problem
        }
        
        sort(begin(prefixGcd), end(prefixGcd)); // Sort in non-decreasing order to facilitate min-max pairing
        
        int l = 0, r = n - 1; // Initialize two pointers at both ends of the sorted array for pairing
        long long s = 0; // Initialize a 64-bit integer to prevent overflow during sum accumulation
        while (l < r) { // Loop until pointers meet, ignoring the middle element if n is odd
            int a = prefixGcd[l], b = prefixGcd[r]; // Extract the smallest and largest remaining unpaired elements
            s += __gcd(a, b); // Accumulate the GCD of the current pair into the total sum
            l++; // Move the left pointer forward to select the next smallest element
            r--; // Move the right pointer backward to select the next largest element
        }
        return s; // Return the final accumulated sum of pair GCDs
    }
};
}

// ===============================
// Better Approach
// ===============================

// Approach:
// To optimize finding the prefix maximum, we can precompute the maximum elements using an auxiliary array prefixMax in a single pass.
// We then construct the prefix GCD array in another pass, sort it, and pair the elements.

// Algorithm:
// 1. Create a prefixMax array of size n and compute prefix maximums where prefixMax[i] = max(prefixMax[i - 1], nums[i]).
// 2. Create a prefixGcd array where prefixGcd[i] = gcd(nums[i], prefixMax[i]).
// 3. Sort prefixGcd.
// 4. Use two pointers l and r to pair the smallest and largest elements, summing their GCDs.

// Time Complexity:
// O(n log n + n log(val)) due to the sorting step and the GCD computation.

// Space Complexity:
// O(n) to store prefixMax and prefixGcd arrays.

namespace Better {
class Solution {
public:
    long long gcdSum(vector<int>& nums) {
        int n = nums.size(); // Store the size of the array for boundary checks and iterations
        vector<int> prefixMax(n); // Auxiliary array to precompute the maximum element up to each index
        
        prefixMax[0] = nums[0]; // The first element's prefix max is the element itself
        for (int i = 1; i < n; i++) { // Populate the prefix maximum array in linear time
            prefixMax[i] = max(prefixMax[i - 1], nums[i]); // Choose the maximum between the previous prefix max and current element
        }
        
        vector<int> prefixGcd(n); // Vector to store GCD of each element with its prefix maximum
        for (int i = 0; i < n; i++) { // Loop to compute the prefix GCD for each index
            prefixGcd[i] = __gcd(prefixMax[i], nums[i]); // Calculate GCD of nums[i] and precomputed maximum prefixMax[i]
        }
        
        sort(begin(prefixGcd), end(prefixGcd)); // Sort in non-decreasing order to facilitate min-max pairing
        
        int l = 0, r = n - 1; // Initialize two pointers at both ends of the sorted array for pairing
        long long s = 0; // Initialize a 64-bit integer to prevent overflow during sum accumulation
        while (l < r) { // Loop until pointers meet, ignoring the middle element if n is odd
            int a = prefixGcd[l], b = prefixGcd[r]; // Extract the smallest and largest remaining unpaired elements
            s += __gcd(a, b); // Accumulate the GCD of the current pair into the total sum
            l++; // Move the left pointer forward to select the next smallest element
            r--; // Move the right pointer backward to select the next largest element
        }
        return s; // Return the final accumulated sum of pair GCDs
    }
};
}

// ===============================
// Optimal Approach
// ===============================

// Approach:
// We can combine the prefix maximum tracking and the prefix GCD computation into a single pass using a running maximum variable mx.
// This eliminates the need for the auxiliary prefixMax array, reducing the memory overhead.

// Algorithm:
// 1. Initialize mx to INT_MIN and a prefixGcd array of size n.
// 2. Iterate i from 0 to n - 1, update mx = max(mx, nums[i]), and calculate prefixGcd[i] = gcd(mx, nums[i]).
// 3. Sort prefixGcd in non-decreasing order.
// 4. Initialize l = 0 and r = n - 1 and accumulate gcd(prefixGcd[l], prefixGcd[r]) while moving the pointers inward.
// 5. Return the accumulated sum.

// Time Complexity:
// O(n log n + n log(val)) due to the sorting step and the GCD computation.

// Space Complexity:
// O(n) to store the prefixGcd array.

namespace Optimal {
class Solution {
public:
    long long gcdSum(vector<int>& nums) {
        int n=nums.size(); // Store the size of the array for boundary checks and iterations
        int mx=INT_MIN; // Track the running maximum element in nums[0...i] to avoid re-scanning
        vector<int>prefixGcd(n); // Vector to store the prefix GCD values for each index
        for(int i=0;i<n;i++){ // Loop through the array to populate the prefix GCD array in a single pass
           mx=max(mx,nums[i]); // Update the running maximum value observed so far up to index i
            prefixGcd[i]=__gcd(mx,nums[i]); // Compute the GCD of the current element and the prefix maximum in-place
        }
        sort(begin(prefixGcd),end(prefixGcd)); // Sort in non-decreasing order to facilitate min-max pairing
        int l=0,r=n-1; // Initialize two pointers at both ends of the sorted array for pairing
        long long s=0; // Initialize a 64-bit integer to prevent overflow during sum accumulation
        while(l<r){ // Loop until pointers meet, ignoring the middle element if n is odd
            int a=prefixGcd[l],b=prefixGcd[r]; // Extract the smallest and largest remaining unpaired elements
            s+=__gcd(a,b); // Accumulate the GCD of the current pair into the total sum
            l++; // Move the left pointer forward to select the next smallest element
            r--; // Move the right pointer backward to select the next largest element
        }
        return s; // Return the final accumulated sum of pair GCDs
    }
};
}