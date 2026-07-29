// ===============================
// Brute Force Approach
// ===============================

// Approach:
// The brute force approach checks all possible substrings of length equal to s1 in s2.
// For each substring, we sort the characters and compare them with the sorted version of s1.
// If they match, then that substring is a permutation of s1.

// Algorithm:
// 1. Check if the length of s1 is greater than s2; if so, s2 cannot contain a permutation of s1, so return false.
// 2. Sort the string s1 to establish a canonical representation for permutation matching.
// 3. Iterate through all possible starting positions of substrings of length m (s1's length) in s2.
// 4. For each starting index, extract the substring of length m, sort it, and compare it with the sorted s1.
// 5. If a match is found, return true.
// 6. If the loop completes without a match, return false.

// Time Complexity:
// O((n - m + 1) * m log m) where n is the length of s2 and m is the length of s1. Sorting each substring of length m takes O(m log m) time.

// Space Complexity:
// O(m) to store the copy of each substring of length m for sorting.

namespace BruteForce {
class Solution {
public:
    bool checkInclusion(string s1, string s2) {
        int n = s2.length(); // Store s2 length to determine the upper bound of substring matching
        int m = s1.length(); // Store s1 length to know the exact substring size to look for
        if (m > n) return false; // A permutation of s1 cannot exist in s2 if s1 is longer than s2
        
        sort(s1.begin(), s1.end()); // Sort s1 to establish a canonical character order for permutation verification
        
        for (int i = 0; i <= n - m; i++) { // Loop over all possible start positions in s2 where a substring of length m can fit
            string sub = s2.substr(i, m); // Extract a substring of length m to check if it's a permutation of s1
            sort(sub.begin(), sub.end()); // Sort the extracted substring so it can be directly compared with sorted s1
            if (sub == s1) { // Compare the sorted substring and sorted s1 to verify character identity
                return true; // Return true immediately if they match, signifying a permutation is found
            }
        }
        return false; // Return false if no substring matches s1's sorted characters
    }
};
}

// ===============================
// Better Approach
// ===============================

// Approach:
// Instead of sorting every substring, we can count the frequency of characters.
// A substring of s2 is a permutation of s1 if and only if they have the exact same character frequencies.
// We count the frequencies of characters in s1, and then for every substring of length m in s2,
// we compute its character frequency map and compare it with s1's frequency map.

// Algorithm:
// 1. If s1's length (m) is greater than s2's length (n), return false.
// 2. Compute the character frequency counts of s1 in a vector of size 26.
// 3. Slide a window of size m through s2 from index 0 to n - m.
// 4. For each window, compute the frequency counts of its characters.
// 5. Compare the window's frequency vector with s1's frequency vector.
// 6. If they are equal, return true.
// 7. If the loop completes without finding any match, return false.

// Time Complexity:
// O((n - m + 1) * m) where n is the length of s2 and m is the length of s1. Constructing the frequency vector for each substring takes O(m) time.

// Space Complexity:
// O(1) auxiliary space, as the frequency vectors use a constant size of 26 (for lowercase English letters).

namespace Better {
class Solution {
public:
    bool checkInclusion(string s1, string s2) {
        int n = s2.length(); // Store s2 length to define the limits of the substring checking loop
        int m = s1.length(); // Store s1 length to determine the fixed size of comparison substrings
        if (m > n) return false; // If s1 is longer than s2, a permutation of s1 cannot possibly fit in s2
        
        vector<int> s1Freq(26, 0); // Create a 26-size frequency vector to store character occurrences of s1
        for (char c : s1) { // Loop through each character of s1 to populate its frequency counts
            s1Freq[c - 'a']++; // Map character to 0-25 index and increment its count
        }
        
        for (int i = 0; i <= n - m; i++) { // Loop over all valid starting indices for substrings of size m in s2
            vector<int> subFreq(26, 0); // Create a frequency vector for the current window in s2
            for (int j = 0; j < m; j++) { // Traverse the characters of the substring starting at index i
                subFreq[s2[i + j] - 'a']++; // Increment the frequency of characters in the current substring
            }
            if (s1Freq == subFreq) { // Check if the character distributions of the current substring and s1 match
                return true; // Return true if the frequency maps match, indicating a valid permutation
            }
        }
        return false; // Return false if no substring matches s1's character distribution
    }
};
}

// ===============================
// Optimal Approach
// ===============================

// Approach:
// The optimal approach uses a sliding window technique. We maintain a running frequency counter
// of the characters in the current window of s2 of size m. Instead of recomputing the character
// frequencies from scratch for each substring (which takes O(m) time), we dynamically update the
// frequency counter by adding the new character entering the window on the right and removing
// the character leaving the window on the left in O(1) time.

// Algorithm:
// 1. Compute the frequency vector of s1 using the helper function find_freqVector.
// 2. Initialize a sliding window using two pointers, l (left) and r (right), both set to 0.
// 3. Iterate r from 0 to n - 1 (where n is the length of s2).
// 4. In each step, increment the frequency of s2[r] in the current window's frequency vector.
// 5. Shrink the window from the left using pointer l if the window size exceeds m.
// 6. If the window size is exactly m, check if the current window's frequency matches s1's frequency.
// 7. If they match, return true immediately.
// 8. Move the right pointer r forward.
// 9. If the window slides over all of s2 without a match, return false.

// Time Complexity:
// O(n) where n is the length of s2. The window is expanded and shrunk at most n times. Comparing two frequency vectors of size 26 takes O(26) = O(1) time.

// Space Complexity:
// O(1) auxiliary space, as only frequency vectors of size 26 are used.

namespace Optimal {
class Solution {
public:
    // Helper function to build a frequency map for a substring in s from index i to j
    vector<int> find_freqVector(string& s, int i, int j) {
        string str = ""; // Unused placeholder variable kept to preserve original logic exactly
        vector<int> freq(26, 0); // Initialize a frequency vector of size 26 for lowercase English letters
        for (int k = i; k <= j; k++) { // Loop through the specified range [i, j] to process characters
            freq[s[k] - 'a']++; // Increment the count for the character to record its frequency
        }
        return freq; // Return the constructed frequency counts
    }

    // Checks if any permutation of s1 is present as a substring in s2 using sliding window
    bool checkInclusion(string s1, string s2) {
        int n = s2.length(); // Get the size of s2 to define the bounds of the search space
        int m = s1.length(); // Get the size of s1 to set the target window size
        vector<int> s1Freq = find_freqVector(s1, 0, m - 1); // Get the frequency vector of s1 for target matching
        int l = 0, r = 0; // Initialize left and right pointers to track the window boundaries
        vector<int> currFreq(26, 0); // Frequency vector to track characters in the current window of s2
        while (r < n) { // Expand the window to the right until the end of s2 is reached
            currFreq[s2[r] - 'a']++; // Include the character at right pointer in the current window's frequency count
            while (l <= r && r - l + 1 > m) { // If the window length exceeds m, shrink it from the left
                currFreq[s2[l] - 'a']--; // Exclude the character at the left pointer from the window frequencies
                l++; // Move the left pointer to the right to maintain a window of size at most m
            }
            if (r - l + 1 == m) { // If the current window matches the exact size of the pattern string s1
                if (s1Freq == currFreq) // Compare the window's frequency profile with the target profile
                    return true; // Return true if the distributions are identical, representing a permutation
            }
            r++; // Slide the right pointer to the right to process the next character
        }
        return false; // Return false if the sliding window traverses s2 completely without a match
    }
};
}