// ===============================
// Brute Force Approach
// ===============================

// Approach:
// The brute force approach checks every possible h-index value from n down to 1.
// For each candidate h, it performs a linear scan over the entire citations array
// to count how many papers have at least h citations. The largest candidate h
// for which the count is greater than or equal to h is returned.

// Algorithm:
// 1. Get the size of the citations array, n.
// 2. Iterate h from n down to 1:
//    a. Count the number of papers in the citations array with citations >= h.
//    b. If the count is >= h, return h immediately as it is the largest possible.
// 3. If the loop completes without returning, return 0.

// Time Complexity:
// O(n^2)

// Space Complexity:
// O(1)

namespace BruteForce {
class Solution {
public:
    int hIndex(vector<int>& citations) {
        // Get the total number of papers to establish the upper bound for the h-index.
        int n = citations.size();
        
        // Iterate through all possible h-index values from the maximum possible (n) down to 1.
        for (int h = n; h >= 1; h--) {
            // Count variable to keep track of papers with at least h citations.
            int count = 0;
            
            // Scan each paper's citation count to determine if it meets the threshold h.
            for (int c : citations) {
                // Check if the current paper has citations greater than or equal to the candidate h.
                if (c >= h) {
                    // Increment the count of qualified papers for the current h candidate.
                    count++;
                }
            }
            
            // If the number of papers with at least h citations is at least h, we found the maximum h-index.
            if (count >= h) {
                // Return the current candidate h as the h-index.
                return h;
            }
        }
        
        // If no positive h-index is found, the researcher has an h-index of 0.
        return 0;
    }
};
}

// ===============================
// Better Approach
// ===============================

// Approach:
// The sorting-based approach sorts the citations in ascending order first.
// Once sorted, it iterates the candidate h-index (h_idx) from n down to 1.
// For each candidate h_idx, it uses binary search (lower_bound) to find the first paper
// with citations >= h_idx. If the number of papers from that index to the end
// of the array is >= h_idx, then h_idx is a valid h-index.

// Algorithm:
// 1. Sort the citations array in ascending order.
// 2. Iterate h_idx from n down to 1.
// 3. Use binary search (lower_bound) to find the iterator pointing to the first paper with citations >= h_idx.
// 4. If lower_bound returns citations.end(), decrement h_idx.
// 5. Otherwise, find the index idx of that paper. The number of papers with citations >= h_idx is n - idx.
// 6. If n - idx >= h_idx (or idx <= n - h_idx), return h_idx.
// 7. If the loop completes, return h_idx (which will be 0).

// Time Complexity:
// O(n log n)

// Space Complexity:
// O(1)

namespace Better {
class Solution {
public:
    int hIndex(vector<int>& citations) {
        // Store the number of papers to determine the search range and index offsets.
        int n = citations.size();
        
        // Sort the citations in ascending order to enable binary search (lower_bound) and positional analysis.
        sort(begin(citations), end(citations));
        
        // Start checking candidate h-index values from the maximum possible, which is the total number of papers.
        int h_idx = n;
        
        // Loop downwards to find the largest h-index value that satisfies the condition.
        while (h_idx >= 1) {
            // Find the first citation that is at least h_idx using binary search.
            auto it = lower_bound(begin(citations), end(citations), h_idx);
            
            // If no citation is >= h_idx, this h_idx is not possible, so decrement and try the next value.
            if (it == citations.end()) {
                h_idx--;
                continue;
            }
            
            // Calculate the 0-based index of the first paper having citations >= h_idx.
            int idx = it - citations.begin();
            
            // Since the array is sorted, papers from index idx to n-1 (total n - idx papers) have >= h_idx citations.
            // If the number of such papers (n - idx) is at least h_idx (or idx <= n - h_idx), we found our h-index.
            if (idx <= n - h_idx )
                // Return the current valid h-index.
                return h_idx;
                
            // Decrement the candidate h-index if the condition is not met for the current value.
            h_idx--;
        }
        
        // Return 0 (or the remaining h_idx which is 0) if no positive h-index satisfies the condition.
        return h_idx;
    }
};
}

// ===============================
// Optimal Approach
// ===============================

// Approach:
// Using a counting sort (bucket sort) approach, we count the number of papers for each citation count.
// Since the maximum possible h-index is n (total papers), any citation count greater than n can be capped
// and counted at n. We then iterate from n down to 0, accumulating the counts of papers from the right.
// The first index h where the accumulated count of papers is >= h is the optimal h-index.

// Algorithm:
// 1. Initialize a count array of size n + 1 with all zeros.
// 2. Count the frequencies of citations: if a citation is >= n, increment count[n]; otherwise, increment count[citation].
// 3. Keep a running sum of papers starting from h = n down to 0.
// 4. At each step, add count[h] to the running sum of papers.
// 5. If the running sum is >= h, return h.
// 6. Return 0 if the loop finishes (fallback).

// Time Complexity:
// O(n)

// Space Complexity:
// O(n)

namespace Optimal {
class Solution {
public:
    int hIndex(vector<int>& citations) {
        // Store the number of papers to define the size of the count buckets.
        int n = citations.size();
        
        // Create buckets to count papers. Index i represents the citation count (capped at n).
        vector<int> count(n + 1, 0);
        
        // Populate the counts. Any citation count >= n is capped at n because h-index cannot exceed n.
        for (int c : citations) {
            // Direct citations >= n into the n-th bucket since h-index is capped at n.
            if (c >= n) {
                // Increment the n-th bucket for papers with high citations.
                count[n]++;
            } else {
                // Increment the count of papers with exactly c citations.
                count[c]++;
            }
        }
        
        // Accumulate papers starting from the highest citations (n) down to 0.
        int papers = 0;
        
        // Iterate downwards from max possible h-index to find the largest value satisfying the condition.
        for (int h = n; h >= 0; h--) {
            // Add the count of papers with exactly h citations to the running total.
            papers += count[h];
            
            // If the accumulated number of papers is at least h, we found our h-index.
            if (papers >= h) {
                // Return the current h as it is the maximum valid h-index.
                return h;
            }
        }
        
        // Return 0 if no valid h-index is found, representing no citations.
        return 0;
    }
};
}