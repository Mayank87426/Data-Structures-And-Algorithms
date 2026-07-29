// ===============================
// Brute Force Approach
// ===============================

// Approach:
// For each query [l, r], we extract the substring s[l...r]. Since the query is augmented with '1' at both ends 
// (i.e., t = '1' + s[l...r] + '1'), any contiguous block of '0's inside s[l...r] is surrounded by '1's.
// We iterate through the substring s[l...r] to locate all contiguous blocks of '0's and record their lengths.
// If there are fewer than two '0' blocks, we cannot perform any valid trade, so the answer is the baseline count of '1's.
// If there are two or more '0' blocks, we can choose to turn any block of '1's between two adjacent '0' blocks into '0's,
// merging them into a single larger '0' block, and then convert this merged block into '1's. The net gain in '1's is the
// sum of the lengths of the two merged '0' blocks. To maximize the '1's, we find the maximum sum of lengths of any two
// adjacent '0' blocks.

// Algorithm:
// 1. Count total occurrences of '1' in the original string s to set the baseline active section count.
// 2. For each query [l, r]:
//    a. Scan the substring s[l...r] and identify the lengths of all contiguous zero blocks.
//    b. If zeroBlocks.size() < 2, the query answer is the baseline count.
//    c. Otherwise, iterate through the list of zero block lengths, calculating the sum of every adjacent pair.
//    d. Add the maximum adjacent pair sum to the baseline count and record it.
// 3. Return the results for all queries.

// Time Complexity:
// O(Q * N)

// Space Complexity:
// O(N)

namespace BruteForce {
class Solution {
public:
    vector<int> maxActiveSectionsAfterTrade(string s, vector<vector<int>>& queries) {
        int n = s.length(); // Get the length of the string s
        int cnt1 = 0; // Initialize baseline count of '1's
        for (char c : s) { // Iterate through each character in the string
            if (c == '1') cnt1++; // Increment count if character is '1'
        }
        
        vector<int> ans; // Initialize the vector to store results for each query
        for (const auto& q : queries) { // Iterate through each query independently
            int l = q[0], r = q[1]; // Get start and end boundaries of the query
            vector<int> zeroBlocks; // Stores lengths of contiguous '0' blocks in the substring s[l...r]
            int i = l; // Start traversing from index l
            while (i <= r) { // Loop until the end index r
                if (s[i] == '0') { // Found the start of a zero block
                    int len = 0; // Initialize length of this block
                    while (i <= r && s[i] == '0') { // Count consecutive '0's
                        len++; // Increment block length
                        i++; // Move to next character
                    }
                    zeroBlocks.push_back(len); // Record the size of this zero block
                } else {
                    i++; // Move past '1's
                }
            }
            
            if (zeroBlocks.size() < 2) { // If there are fewer than 2 zero blocks
                ans.push_back(cnt1); // No trade is possible, answer is just the baseline count
            } else {
                int maxGain = 0; // Initialize max gain from combining adjacent zero blocks
                for (size_t k = 0; k < zeroBlocks.size() - 1; ++k) { // Check all adjacent pairs of zero blocks
                    maxGain = max(maxGain, zeroBlocks[k] + zeroBlocks[k + 1]); // Keep the maximum sum of adjacent block lengths
                }
                ans.push_back(cnt1 + maxGain); // Add the best gain to baseline count and save
            }
        }
        return ans; // Return the results for all queries
    }
};
}

// ===============================
// Better Approach
// ===============================

// Approach:
// Instead of scanning the substring for each query, we can precompute the contiguous zero blocks globally for the entire string s.
// We store their lengths, start indices, and end indices.
// For each query [l, r], we use binary search (lower_bound on end indices, upper_bound on start indices) to find the range of 
// zero blocks [i, j] that overlap with [l, r].
// Since the first block i and the last block j may only partially overlap with [l, r], we compute their actual lengths within the
// query range as firstLen and lastLen.
// If the range contains fewer than 2 blocks, no trade is possible.
// If the range contains exactly 2 blocks, they must be the first and last blocks, so the only possible merge is between them,
// yielding a gain of firstLen + lastLen.
// If there are more than 2 blocks, the maximum gain can be achieved by merging:
// - The first block (partially within range) and the second block (fully within range): firstLen + zeroBlocks[i+1].
// - The second-to-last block (fully within range) and the last block (partially within range): zeroBlocks[j-1] + lastLen.
// - Any two adjacent fully contained zero blocks in the middle: zeroBlocks[k] + zeroBlocks[k+1] for k in [i+1, j-2]. We find this
//   maximum using a simple linear scan over the middle blocks.

// Algorithm:
// 1. Count total occurrences of '1' in the original string s.
// 2. Perform a single pass over s to find all contiguous zero blocks, recording their lengths, start indices, and end indices.
// 3. For each query [l, r]:
//    a. Query the overlapping zero blocks [i, j] using binary search.
//    b. If i >= j, return baseline.
//    c. Calculate overlap lengths for the first block (firstLen) and the last block (lastLen).
//    d. If i + 1 == j, return baseline + firstLen + lastLen.
//    e. Otherwise, scan the range of fully contained blocks from i+1 to j-2 to find the maximum adjacent sum.
//    f. Combine with the boundary merges, take the maximum, and add to baseline.

// Time Complexity:
// O(N + Q * N)

// Space Complexity:
// O(N)

namespace Better {
class Solution {
public:
    vector<int> maxActiveSectionsAfterTrade(string s, vector<vector<int>>& queries) {
        int n = s.length(); // Get the length of the string s
        int cnt1 = count(s.begin(), s.end(), '1'); // Count total original '1's as the baseline active sections

        vector<int> zeroBlocks; // Stores lengths of each contiguous block of '0's
        vector<int> blockLeft; // Stores start indices of each contiguous block of '0's
        vector<int> blockRight; // Stores end indices of each contiguous block of '0's

        int i = 0; // Index pointer to traverse the string
        // Parse the string to identify and record zero blocks
        while (i < n) {
            int st = i; // Record the starting position of the current block
            // Move pointer forward as long as characters are identical
            while (i < n && s[i] == s[st]) {
                i += 1; // Advance the pointer
            }
            // If the block consists of '0's, record its properties
            if (s[st] == '0') {
                zeroBlocks.push_back(i - st); // Record the length of the zero block
                blockLeft.push_back(st); // Record the start index of the block
                blockRight.push_back(i - 1); // Record the end index of the block
            }
        }

        int m = zeroBlocks.size(); // Total number of contiguous zero blocks
        // If we have fewer than 2 zero blocks, no valid trade can be made
        if (m < 2) {
            // Return baseline '1' count for all queries since no trade is possible
            return vector<int>(queries.size(), cnt1);
        }

        vector<int> ans; // Stores the results for each query
        // Process each query independently
        for (const auto& q : queries) {
            int l = q[0], r = q[1]; // Extract query range boundaries
            // Binary search for the first zero block that ends at or after query left boundary
            int i = lower_bound(blockRight.begin(), blockRight.end(), l) - blockRight.begin();
            // Binary search for the last zero block that starts at or before query right boundary
            int j = upper_bound(blockLeft.begin(), blockLeft.end(), r) - blockLeft.begin() - 1;

            // If there is at most 1 zero block within the query range, no trade is possible
            if (i > m - 1 || j < 0 || i >= j) {
                ans.push_back(cnt1); // Push baseline answer
                continue; // Move to the next query
            }

            // Compute the overlap length of the first block within the query boundaries
            int firstLen = blockRight[i] - max(blockLeft[i], l) + 1;
            // Compute the overlap length of the last block within the query boundaries
            int lastLen = min(blockRight[j], r) - blockLeft[j] + 1;

            // If there are exactly two zero blocks in the query range
            if (i + 1 == j) {
                int bestGain = firstLen + lastLen; // The only possible trade merges these two blocks
                ans.push_back(cnt1 + bestGain); // Add gain to baseline count
                continue; // Move to the next query
            }

            // Case 1: Merge the first partial block and the second full block
            int val1 = firstLen + zeroBlocks[i + 1];
            // Case 2: Merge the second-to-last full block and the last partial block
            int val2 = zeroBlocks[j - 1] + lastLen;
            
            // Linear scan to find the maximum of adjacent zero blocks in the middle range
            int val3 = 0; // Initialize max middle gain to 0
            // Loop through all adjacent pairs of zero blocks fully contained in query range
            for (int k = i + 1; k <= j - 2; k++) {
                val3 = max(val3, zeroBlocks[k] + zeroBlocks[k + 1]); // Update maximum gain
            }
            
            int bestGain = max({val1, val2, val3}); // Determine the maximum active section gain
            ans.push_back(cnt1 + bestGain); // Save result for current query
        }

        return ans; // Return the list of query answers
    }
};
}

// ===============================
// Optimal Approach
// ===============================

// Approach:
// The Optimal approach optimizes the query time to O(log N) by using a Segment Tree to perform Range Maximum Queries (RMQ).
// We precompute the array of adjacent zero block sums tmpSum where tmpSum[k] = zeroBlocks[k] + zeroBlocks[k+1].
// We build a Segment Tree over tmpSum to find the maximum value in any subsegment in O(log N) time.
// When querying the middle range [i+1, j-2], instead of a linear scan, we query the Segment Tree in O(log N) time.

// Algorithm:
// 1. Parse the string s to find all zero blocks, storing their lengths, left indices, and right indices.
// 2. Construct tmpSum where tmpSum[k] stores the combined length of block k and block k+1.
// 3. Build a Segment Tree over tmpSum to support range maximum queries.
// 4. For each query [l, r]:
//    a. Locate the overlapping zero blocks [i, j] using binary search.
//    b. If i >= j, no trade is possible.
//    c. If i + 1 == j, return baseline + firstLen + lastLen.
//    d. If i + 1 < j, compute candidate gains: merging boundaries (firstLen + zeroBlocks[i+1], lastLen + zeroBlocks[j-1]) 
//       and querying the Segment Tree for the middle range [i+1, j-2].
//    e. Add the maximum gain to the baseline.

// Time Complexity:
// O(N + Q log N)

// Space Complexity:
// O(N)

namespace Optimal {
class SegmentTree {
private:
    int n; // Stores the size of the array for boundary checks
    vector<int> arr; // Stores the input array to construct the tree
    vector<int> seg; // Internal array representing the binary segment tree

    // Recursively builds the segment tree by splitting ranges in half
    void build(int p, int l, int r) {
        // Base case: leaf node represents a single element in the array
        if (l == r) {
            seg[p] = arr[l]; // Store the value of the array element at the leaf node
            return; // Terminate recursion
        }

        int mid = (l + r) >> 1; // Find the midpoint to partition the range
        build(p << 1, l, mid); // Recursively build the left child segment
        build(p << 1 | 1, mid + 1, r); // Recursively build the right child segment
        seg[p] = max(seg[p << 1], seg[p << 1 | 1]); // Parent node stores the maximum of its children
    }

    // Helper query method to perform range maximum query (RMQ)
    int _query(int p, int l, int r, int L, int R) {
        // If the current segment is completely within the query range [L, R]
        if (L <= l && r <= R) {
            return seg[p]; // Return the precomputed maximum value of this segment
        }

        int mid = (l + r) >> 1; // Partition the range at the midpoint
        int res = 0; // Initialize result to 0 (since block lengths are non-negative)
        // If query range overlaps with the left child
        if (L <= mid) {
            res = max(res, _query(p << 1, l, mid, L, R)); // Query the left child and update result
        }
        // If query range overlaps with the right child
        if (R > mid) {
            res = max(res, _query(p << 1 | 1, mid + 1, r, L, R)); // Query the right child and update result
        }

        return res; // Return the maximum value found in the overlap
    }

public:
    // Constructor initializes size, resizes tree array, and initiates build process
    SegmentTree(const vector<int>& arr) : arr(arr) {
        n = arr.size(); // Set the array size
        seg.resize(n << 2, 0); // Allocate up to 4*N nodes for the segment tree structure
        build(1, 0, n - 1); // Start building the tree from the root node at index 1
    }

    // Public method to query the maximum value in range [L, R]
    int query(int L, int R) {
        // If query range is invalid, return default 0
        if (L > R) {
            return 0; // No valid range to search, so the maximum gain is 0
        }

        return _query(1, 0, n - 1, L, R); // Query starting from the root node
    }
};

class Solution {
public:
    vector<int> maxActiveSectionsAfterTrade(string s,
                                            vector<vector<int>>& queries) {
        int n = s.length(); // Get the length of the binary string
        int cnt1 = count(s.begin(), s.end(), '1'); // Count total original '1's as the baseline active sections

        vector<int> zeroBlocks; // Stores lengths of each contiguous block of '0's
        vector<int> blockLeft; // Stores start indices of each contiguous block of '0's
        vector<int> blockRight; // Stores end indices of each contiguous block of '0's

        int i = 0; // Index pointer to traverse the string
        // Parse the string to identify and record zero blocks
        while (i < n) {
            int st = i; // Record the starting position of the current block
            // Move pointer forward as long as characters are identical
            while (i < n && s[i] == s[st]) {
                i += 1; // Advance the pointer
            }
            // If the block consists of '0's, record its properties
            if (s[st] == '0') {
                zeroBlocks.push_back(i - st); // Record the length of the zero block
                blockLeft.push_back(st); // Record the start index of the block
                blockRight.push_back(i - 1); // Record the end index of the block
            }
        }

        int m = zeroBlocks.size(); // Total number of contiguous zero blocks
        // If we have fewer than 2 zero blocks, no valid trade can be made
        if (m < 2) {  // continuous 0 blocks less than 2 segments, return the
                      // answer directly
            return vector<int>(queries.size(), cnt1); // Return baseline '1' count for all queries since no trade is possible
        }

        vector<int> tmpSum(m - 1); // Array to store sums of adjacent zero block lengths
        // Precalculate adjacent sums of zero block lengths for the segment tree
        for (int i = 0; i < m - 1; i++) {
            tmpSum[i] = zeroBlocks[i] + zeroBlocks[i + 1]; // Combine lengths of block i and block i+1
        }
        SegmentTree seg(tmpSum); // Build segment tree over adjacent sums to support range maximum queries
        vector<int> ans; // Stores the results for each query

        // Process each query independently
        for (const auto& q : queries) {
            int l = q[0], r = q[1]; // Extract query range boundaries
            // Binary search for the first zero block that ends at or after query left boundary
            int i = lower_bound(blockRight.begin(), blockRight.end(), l) -
                    blockRight.begin();
            // Binary search for the last zero block that starts at or before query right boundary
            int j = upper_bound(blockLeft.begin(), blockLeft.end(), r) -
                    blockLeft.begin() - 1;

            // at most 1 continuous block of 0s within the substring
            if (i > m - 1 || j < 0 || i >= j) {
                ans.push_back(cnt1); // Push baseline answer
                continue; // Move to the next query
            }
            int firstLen = blockRight[i] - max(blockLeft[i], l) +
                           1;  // actual length of the first consecutive block
                               // of 0s in the substring
            int lastLen = min(blockRight[j], r) - blockLeft[j] +
                          1;  // actual length of the last consecutive block of
                              // 0s in the substring
            // exactly 2 consecutive 0 blocks within the substring
            if (i + 1 == j) {
                int bestGain = firstLen + lastLen; // The only possible trade merges these two blocks
                ans.push_back(cnt1 + bestGain); // Add gain to baseline count
                continue; // Move to the next query
            }

            int val1 = firstLen + zeroBlocks[i + 1]; // Case 1: Merge the first partial block and the second full block
            int val2 = zeroBlocks[j - 1] + lastLen; // Case 2: Merge the second-to-last full block and the last partial block
            int val3 = seg.query(i + 1, j - 2); // Case 3: Merge any two fully contained adjacent zero blocks using segment tree
            int bestGain = max({val1, val2, val3}); // Determine the maximum active section gain
            ans.push_back(cnt1 + bestGain); // Save result for current query
        }

        return ans; // Return the list of query answers
    }
};
}