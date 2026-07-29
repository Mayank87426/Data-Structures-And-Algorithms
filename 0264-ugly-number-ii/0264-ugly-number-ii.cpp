// ===============================
// Brute Force Approach
// ===============================

// Approach:
// The brute force approach checks every positive integer starting from 1 to see 
// if it is an ugly number. We keep a count of ugly numbers found. An integer 
// is ugly if it can be reduced to 1 by repeatedly dividing by its allowed 
// prime factors: 2, 3, and 5. The first number to reach the count of n is returned.

// Algorithm:
// 1. Initialize count of ugly numbers found to 0 and current candidate number to 1.
// 2. For each candidate number, divide it by 2, 3, and 5 as much as possible.
// 3. If the final value is 1, it is an ugly number. Increment count.
// 4. If count equals n, return the current candidate number.
// 5. Otherwise, increment the candidate number and repeat.

// Time Complexity:
// O(result * log(result)) where result is the nth ugly number. Since the nth ugly 
// number grows quickly (e.g., for n = 1690, the result is 2,123,366,400), this 
// approach will result in Time Limit Exceeded (TLE).

// Space Complexity:
// O(1) as we only use a few helper variables for checks.

namespace BruteForce {
class Solution {
public:
    // Helper function to check if a given number is ugly (only has prime factors 2, 3, and 5)
    bool isUgly(int num) {
        // Divide by 2 as long as it is divisible to eliminate 2 as a prime factor
        while (num % 2 == 0) num /= 2;
        // Divide by 3 as long as it is divisible to eliminate 3 as a prime factor
        while (num % 3 == 0) num /= 3;
        // Divide by 5 as long as it is divisible to eliminate 5 as a prime factor
        while (num % 5 == 0) num /= 5;
        // If the number reduces to 1, its only prime factors were 2, 3, or 5
        return num == 1;
    }

    int nthUglyNumber(int n) {
        // Track the count of ugly numbers found so far
        int count = 0;
        // Start testing integers from 1 onwards
        int num = 1;
        // Keep searching until we find the nth ugly number
        while (true) {
            // Check if the current candidate integer is an ugly number
            if (isUgly(num)) {
                // Increment the counter since we found another ugly number
                count++;
                // If we've reached the target count, return the current number
                if (count == n) {
                    return num;
                }
            }
            // Move to the next candidate integer
            num++;
        }
    }
};
}

// ===============================
// Better Approach
// ===============================

// Approach:
// Instead of checking all numbers, we can generate ugly numbers dynamically.
// Starting with 1, any ugly number multiplied by 2, 3, or 5 will also be an ugly 
// number. We can use a min-heap to keep track of the generated numbers in 
// ascending order, and a hash set to ensure we do not push duplicate values.

// Algorithm:
// 1. Initialize a min-heap (priority queue) and a hash set with the first ugly number, 1.
// 2. Pop the smallest element from the min-heap. This is the next ugly number.
// 3. For the popped number, multiply it by 2, 3, and 5 to generate new candidates.
// 4. If a candidate is not present in the hash set, add it to both the set and the min-heap.
// 5. Repeat steps 2-4 for n times. The nth popped value is the answer.

// Time Complexity:
// O(n * log(n)) as we perform pop and push operations on the heap n times, 
// and each operation takes logarithmic time with respect to the heap size.

// Space Complexity:
// O(n) to store the generated ugly numbers in the heap and the hash set.

namespace Better {
class Solution {
public:
    int nthUglyNumber(int n) {
        // Min-heap to store generated ugly numbers in ascending order
        priority_queue<long long, vector<long long>, greater<long long>> pq;
        // Hash set to keep track of already seen/generated numbers to prevent duplicates
        unordered_set<long long> seen;
        
        // Push the base case ugly number 1 into the heap and the set
        pq.push(1);
        seen.insert(1);
        
        // Track the current smallest ugly number popped from the heap
        long long curr = 1;
        // Pop the smallest element n times to retrieve the nth ugly number
        for (int i = 0; i < n; i++) {
            // Retrieve the smallest available ugly number from the min-heap
            curr = pq.top();
            // Remove the retrieved element from the heap
            pq.pop();
            
            // Generate next potential ugly numbers by multiplying the current one by 2, 3, and 5
            for (int factor : {2, 3, 5}) {
                // Calculate candidate product using long long to avoid integer overflow
                long long next_ugly = curr * factor;
                // If this product hasn't been generated yet, insert it
                if (seen.find(next_ugly) == seen.end()) {
                    // Mark as seen to prevent duplicates in the future
                    seen.insert(next_ugly);
                    // Add the new candidate to the heap for future minimum checks
                    pq.push(next_ugly);
                }
            }
        }
        
        // Cast the final popped value to int and return it as the nth ugly number
        return (int)curr;
    }
};
}

// ===============================
// Optimal Approach
// ===============================

// Approach:
// The optimal approach uses dynamic programming. We construct the sequence of 
// ugly numbers directly. The next ugly number is always the minimum of the next 
// multiples of 2, 3, and 5 from previously computed ugly numbers.
// We maintain three pointers (i2, i3, i5) indicating the next indices in the DP table 
// to multiply by 2, 3, and 5 respectively. In each step, we pick the minimum, 
// and advance the pointer(s) that produced this minimum.

// Algorithm:
// 1. Create a DP array of size n+1, with the first ugly number set to 1.
// 2. Initialize pointers i2, i3, and i5 to 1.
// 3. Loop from 2 to n. Calculate three candidates: t[i2] * 2, t[i3] * 3, and t[i5] * 5.
// 4. Choose the minimum of these three as the next ugly number t[i].
// 5. Advance the pointer(s) (i2, i3, or i5) whose candidate matches t[i].
// 6. Return t[n].

// Time Complexity:
// O(n) as we compute the DP table linearly in a single pass.

// Space Complexity:
// O(n) to store the first n ugly numbers in the DP array.

namespace Optimal {
class Solution {
public:
    int nthUglyNumber(int n) {
        // Create a vector to store the first n ugly numbers, 1-indexed for convenience
        vector<int> t(n+1);
        // The first ugly number is defined to be 1 by default
        t[1] = 1; 
        
        // Declare pointers/indices for the multiples of 2, 3, and 5
        int i2;
        int i3; 
        int i5; 
        // Initialize all pointers to index 1, pointing to the first ugly number
        i2 = i3 = i5 = 1;
        
        // Iteratively compute ugly numbers from index 2 up to n
        for(int i = 2; i<=n; i++) {
            // Calculate the next candidate ugly number using the 2-multiplier pointer
            int i2th_ugly = t[i2] * 2;
            
            // Calculate the next candidate ugly number using the 3-multiplier pointer
            int i3rd_ugly = t[i3] * 3;
            
            // Calculate the next candidate ugly number using the 5-multiplier pointer
            int i5th_ugly = t[i5] * 5;
            
            // The next ugly number is the minimum of the three generated candidates to maintain sorted order
            t[i] = min({i2th_ugly, i3rd_ugly, i5th_ugly});
            
            // If the minimum choice came from the 2-multiplier, advance its pointer to avoid using it again
            if(t[i] == i2th_ugly)
                i2++;
            
            // If the minimum choice came from the 3-multiplier, advance its pointer to avoid duplicate usage
            if(t[i] == i3rd_ugly)
                i3++;
            
            // If the minimum choice came from the 5-multiplier, advance its pointer to avoid duplicate usage
            if(t[i] == i5th_ugly)
                i5++;
        }
        
        // Return the nth ugly number stored at index n
        return t[n];
    }
};
}
