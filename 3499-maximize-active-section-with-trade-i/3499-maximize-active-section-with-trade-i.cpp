// ===============================
// Brute Force Approach
// ===============================

// Approach:
// The brute force approach simulates the process for each contiguous block of '1's in the string.
// We identify every block of '1's, check if it is surrounded by '0's (in the augmented string),
// and if so, perform the trade: convert the block to '0's, find the newly formed contiguous block of '0's,
// and convert it to '1's. We then count the total active sections and keep track of the maximum.

// Algorithm:
// 1. Augment the string with '1' at both ends.
// 2. Identify each contiguous block of '1's.
// 3. For each block, verify if it has a '0' on both its left and right boundaries.
// 4. If yes, copy the string and simulate the trade.
// 5. Count the active sections in the simulated string and update the maximum.

// Time Complexity:
// O(N^2) where N is the length of the string, since there can be O(N) blocks of '1's and each simulation takes O(N) time.

// Space Complexity:
// O(N) to store the copy of the string during simulation.

namespace BruteForce {
class Solution {
public:
    int maxActiveSectionsAfterTrade(string s) {
        // Count initial active sections to establish a baseline count.
        int oneCount = count(s.begin(), s.end(), '1');
        // If there are no active sections initially, no valid trade can be performed.
        if (oneCount == 0) return 0;

        // Augment the string with '1' at both ends to handle boundary conditions.
        string t = "1" + s + "1";
        int n = t.length();
        // The maximum count is at least the initial count (i.e. if we do not trade).
        int maxActive = oneCount;

        // Iterate through the augmented string to find blocks of '1's.
        for (int i = 1; i < n - 1; ) {
            // Skip '0's to find the start of a block of '1's.
            if (t[i] == '0') {
                i++;
                continue;
            }

            // Record the start index of the current block of '1's.
            int l = i;
            // Scan to find the end of this block.
            while (i < n && t[i] == '1') i++;
            // Record the end index of the current block of '1's.
            int r = i - 1;

            // Check if this block is surrounded by '0's.
            if (l > 0 && t[l - 1] == '0' && r < n - 1 && t[r + 1] == '0') {
                // Copy the augmented string to simulate the trade.
                string temp = t;
                // Convert the chosen contiguous '1's block to '0's.
                for (int j = l; j <= r; j++) {
                    temp[j] = '0';
                }

                // Find the new contiguous block of '0's containing the converted block.
                int zero_start = l;
                while (zero_start > 0 && temp[zero_start - 1] == '0') {
                    zero_start--;
                }
                int zero_end = r;
                while (zero_end < n - 1 && temp[zero_end + 1] == '0') {
                    zero_end++;
                }

                // Verify if the newly formed '0's block is surrounded by '1's.
                if (zero_start > 0 && temp[zero_start - 1] == '1' && zero_end < n - 1 && temp[zero_end + 1] == '1') {
                    // Convert the entire block of '0's to '1's.
                    for (int j = zero_start; j <= zero_end; j++) {
                        temp[j] = '1';
                    }
                    // Count the active sections in the simulated string (excluding augmented ends).
                    int currentOneCount = 0;
                    for (int j = 1; j < n - 1; j++) {
                        if (temp[j] == '1') {
                            currentOneCount++;
                        }
                    }
                    // Update the maximum active sections.
                    maxActive = max(maxActive, currentOneCount);
                }
            }
        }
        return maxActive;
    }
};
}

// ===============================
// Better Approach
// ===============================

// Approach:
// We precompute the counts of contiguous '0's to the left and right of every index.
// This allows us to determine in O(1) time the size of the neighboring zero blocks
// for any given block of '1's. If a block of '1's is surrounded by '0's, performing
// the trade will merge the left zero block, the '1's block, and the right zero block.
// After converting them to '1's, the net gain in '1's is the sum of the sizes of
// the left and right zero blocks.

// Algorithm:
// 1. Augment the string with '1' at both ends.
// 2. Compute the prefix array where prefix[i] is the number of consecutive '0's ending at i-1.
// 3. Compute the suffix array where suffix[i] is the number of consecutive '0's starting at i+1.
// 4. Iterate through the string, find each contiguous block of '1's [l, r].
// 5. If prefix[l] > 0 and suffix[r] > 0, update the maximum gain with prefix[l] + suffix[r].
// 6. Return the initial count of '1's plus the maximum gain.

// Time Complexity:
// O(N) since we perform a few linear scans over the string of length N.

// Space Complexity:
// O(N) to store the prefix and suffix arrays.

namespace Better {
class Solution {
public:
    int maxActiveSectionsAfterTrade(string s) {
        // Count the initial active sections in the input string to establish a baseline.
        int oneCount = count(s.begin(), s.end(), '1');
        // If there are no active sections initially, we cannot perform any valid trade.
        if (oneCount == 0)
            return 0;

        // Augment the string with '1' at both ends to handle boundary cases uniformly.
        string str = "1" + s + "1";
        // Store the length of the augmented string for loop bounds.
        int n = str.length();

        // Arrays to store the length of consecutive '0's to the left and right of each index.
        vector<int> prefix(n, 0), suffix(n, 0);

        // Precompute the count of contiguous '0's immediately preceding each index.
        for (int i = 1; i < n; i++) {
            // If the previous character is '0', extend the consecutive zero count.
            if (str[i - 1] == '0')
                prefix[i] = prefix[i - 1] + 1;
        }

        // Precompute the count of contiguous '0's immediately succeeding each index.
        for (int i = n - 2; i >= 0; i--) {
            // If the next character is '0', extend the consecutive zero count.
            if (str[i + 1] == '0')
                suffix[i] = suffix[i + 1] + 1;
        }

        // Variable to track the maximum number of '0's we can convert to '1's.
        int maxGain = 0;

        // Iterate through the augmented string to find candidate blocks of '1's.
        for (int i = 1; i < n - 1;) {
            // Skip '0's as we are looking for a block of '1's.
            if (str[i] == '0') {
                i++;
                continue;
            }

            // Identify the boundaries [l, r] of the contiguous '1's block.
            int l = i;
            // Scan to find the end of this '1's block.
            while (i < n && str[i] == '1') i++;
            // Set the right boundary of the '1's block.
            int r = i - 1;

            // Check if the block is surrounded by at least one '0' on both sides.
            if (prefix[l] > 0 && suffix[r] > 0) {
                // Update maxGain with the combined length of zero blocks on both sides.
                maxGain = max(maxGain, prefix[l] + suffix[r]);
            }
        }

        // Total active sections is the initial count plus the maximum gained active sections.
        return oneCount + maxGain;
    }
};
}

// ===============================
// Optimal Approach
// ===============================

// Approach:
// Instead of storing precomputed prefix and suffix zero counts in helper arrays,
// we can track the zero block sizes on the fly as we iterate.
// As we move through the augmented string (which always starts with a '1'),
// we alternate between blocks of '1's and '0's.
// When we process a block of '1's, we count the number of '0's immediately following it.
// If there was a non-zero count of preceding '0's (prevZeroes > 0) and there is a non-zero
// count of succeeding '0's (nextZeroes > 0), the block of '1's is surrounded by '0's.
// The gain of trading this block is the sum of the two zero block lengths: prevZeroes + nextZeroes.
// We then update the maximum gain and set the current nextZeroes as the prevZeroes for the next '1's block.

// Algorithm:
// 1. Augment the string with '1' at both ends.
// 2. Traverse the augmented string using a single pointer.
// 3. Identify each block of '1's.
// 4. Measure the length of the '0's block immediately following it (nextZeroes).
// 5. If prevZeroes > 0 and nextZeroes > 0, update maxGain with (prevZeroes + nextZeroes).
// 6. Set prevZeroes to nextZeroes for the next iteration.
// 7. Return the initial '1's count plus the maximum gain.

// Time Complexity:
// O(N) since we traverse the string of length N in a single pass.

// Space Complexity:
// O(1) auxiliary space as we only use a few integer variables.

namespace Optimal {
class Solution {
public:
    int maxActiveSectionsAfterTrade(string s) {
        // Count the initial active sections in the input string to establish a baseline.
        int oneCount = count(s.begin(), s.end(), '1');
        // If there are no active sections initially, we cannot perform any valid trade.
        if (oneCount == 0)
            return 0;

        // Augment the string with '1' at both ends to handle boundary cases uniformly.
        string str = "1" + s + "1";
        // Store the length of the augmented string for loop bounds.
        int n = str.length();

        // Variable to track the maximum number of '0's we can convert to '1's.
        int maxGain = 0;
        // Keep track of the number of '0's preceding the current block of '1's.
        int prevZeroes = 0;

        // Traverse the augmented string to identify blocks of '1's and '0's.
        for (int i = 0; i < n; ) {
            // We start at a block of '1's since the augmented string begins with '1'.
            int start = i;
            // Scan to find the end of this '1's block.
            while (i < n && str[i] == '1') i++;
            
            // Count the number of '0's immediately following this block of '1's.
            int nextZeroes = 0;
            // If we are not at the end of the string and encounter a '0', count the block.
            if (i < n && str[i] == '0') {
                // Record the start index of the succeeding '0's block.
                int zeroStart = i;
                // Scan to find the end of the succeeding '0's block.
                while (i < n && str[i] == '0') i++;
                // Record the count of succeeding zeroes.
                nextZeroes = i - zeroStart;
            }

            // If the '1's block is surrounded by '0's on both sides, calculate the gain.
            if (prevZeroes > 0 && nextZeroes > 0) {
                // Update maxGain with the combined length of the surrounding zero blocks.
                maxGain = max(maxGain, prevZeroes + nextZeroes);
            }

            // The succeeding zero block becomes the preceding zero block for the next iteration.
            prevZeroes = nextZeroes;
        }

        // Total active sections is the initial count plus the maximum gained active sections.
        return oneCount + maxGain;
    }
};
}