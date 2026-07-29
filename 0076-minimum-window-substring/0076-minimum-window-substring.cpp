// ===============================
// Brute Force Approach
// ===============================

// Approach:
// The brute force approach checks every possible substring of s to see if it contains 
// all characters of t with the required frequencies. If a valid substring is found, 
// we compare its length with the minimum length found so far and update our best window.

// Algorithm:
// 1. Iterate through all possible starting indices `i` of the substring in `s`.
// 2. For each starting index `i`, iterate through all ending indices `j` of the substring.
// 3. For each substring `s[i...j]`, check if it contains all characters of `t`.
//    This is done by tracking a copy of character frequencies of `t` and checking if 
//    the substring satisfies the requirements.
// 4. If the substring is valid, compare its length with the minimum length found so far.
//    If it is smaller, update the minimum length and starting index.
// 5. Finally, return the minimum window substring, or an empty string if no valid window exists.

// Time Complexity:
// O(m * n^2) where n is the length of s and m is the length of t. There are O(n^2) substrings, and for each substring, we perform a verification step that takes O(n + m) time in the worst case.

// Space Complexity:
// O(m) to store the character frequency map of the target string t.

namespace BruteForce {
class Solution {
public:
    string minWindow(string s, string t) {
        // Store the length of string s to limit our window boundary checks
        int n = s.length();
        // Store the length of string t to compare with s and detect impossible cases
        int m = t.length();
        
        // If s is shorter than t, it is impossible for s to contain all of t's characters
        if (n < m) return "";
        
        // Initialize to infinity so that the first valid window found will always update this value
        int minLen = INT_MAX;
        // Keep track of the starting index of the minimal window to extract it later
        int startIdx = -1;
        
        // Iterate through all possible starting positions of the window in string s
        for (int i = 0; i < n; i++) {
            // Iterate through all possible ending positions of the window starting from index i
            for (int j = i; j < n; j++) {
                // Create a frequency map of characters in t to verify if they are fully matched in s[i..j]
                unordered_map<char, int> t_count;
                for (char c : t) {
                    t_count[c]++;
                }
                
                // Traverse the current candidate window s[i..j] and reduce the count of characters found
                for (int k = i; k <= j; k++) {
                    if (t_count.find(s[k]) != t_count.end()) {
                        t_count[s[k]]--;
                    }
                }
                
                // Determine if all character requirements for string t are satisfied (remaining count <= 0)
                bool possible = true;
                for (auto& pair : t_count) {
                    if (pair.second > 0) {
                        possible = false;
                        break;
                    }
                }
                
                // If the candidate window contains all characters of t, check if it's the smallest so far
                if (possible) {
                    if (j - i + 1 < minLen) {
                        minLen = j - i + 1;
                        startIdx = i;
                    }
                }
            }
        }
        
        // Extract and return the smallest valid window substring, or return empty string if none exists
        return startIdx == -1 ? "" : s.substr(startIdx, minLen);
    }
};
}

// ===============================
// Better Approach
// ===============================

// Approach:
// The better approach optimizes the window checks by using a sliding window with two pointers (i and j).
// Instead of checking all substrings from scratch, we incrementally update a character frequency map 
// for the current window. We verify if the window is valid by comparing the window's frequency map 
// with t's frequency map, which takes O(U) time where U is the number of unique characters in t (at most 52).

// Algorithm:
// 1. Build a frequency map `targetMap` for all characters in `t`.
// 2. Maintain a frequency map `windowMap` for characters in the active sliding window `[i, j]`.
// 3. Expand the window by iterating the right pointer `j` from 0 to n-1, incrementing the count of `s[j]` in `windowMap`.
// 4. After each expansion, check if the current window is valid by ensuring `windowMap` contains at least
//    the required frequency for all characters in `targetMap`.
// 5. While the window is valid, update the minimum length and start index if the current window is smaller.
//    Then, shrink the window by decrementing `windowMap[s[i]]` and incrementing the left pointer `i`.
// 6. Return the smallest valid substring found.

// Time Complexity:
// O(U * n + m) where n is the length of s, m is the length of t, and U is the number of unique characters in t (U <= 52).
// In each step of sliding window, checking validity takes O(U) time.

// Space Complexity:
// O(U + K) where U is the number of unique characters in t and K is the number of unique characters in s.

namespace Better {
class Solution {
public:
    string minWindow(string s, string t) {
        // Store length of s to traverse the string using sliding window
        int n = s.length();
        // Create a frequency map to keep track of the required characters from t
        unordered_map<char, int> targetMap;
        for (char c : t) {
            targetMap[c]++;
        }
        
        // Create a frequency map to keep track of characters inside our active window
        unordered_map<char, int> windowMap;
        // Track the minimum window size found so far, initialized to infinity
        int minLen = INT_MAX;
        // Track the starting index of the minimal window to extract it later
        int startIdx = -1;
        
        // Define a helper function to verify if the active window meets all target character requirements
        auto isValid = [&]() {
            for (auto& pair : targetMap) {
                if (windowMap[pair.first] < pair.second) {
                    return false;
                }
            }
            return true;
        };
        
        // Initialize the left pointer of the sliding window
        int i = 0;
        // Expand the right pointer of the sliding window across string s
        for (int j = 0; j < n; j++) {
            // Include the current character in the active window count
            windowMap[s[j]]++;
            
            // While the current window is valid, try to minimize it by shrinking from the left
            while (isValid() && i <= j) {
                // If the current window is smaller than the minimum found, update details
                if (j - i + 1 < minLen) {
                    minLen = j - i + 1;
                    startIdx = i;
                }
                // Decrement the count of the character leaving the window from the left
                windowMap[s[i]]--;
                // Move the left pointer forward to shrink the window
                i++;
            }
        }
        
        // Return the minimum window substring if a valid window was found, else empty string
        return startIdx == -1 ? "" : s.substr(startIdx, minLen);
    }
};
}

// ===============================
// Optimal Approach
// ===============================

// Approach:
// The optimal approach improves validity checking from O(U) to O(1) time. 
// We use a single frequency map `mp` for string `t` and a `count` variable tracking 
// the number of characters still needed to make the window valid.
// As we expand the window with the right pointer `j`, we decrement `mp[s[j]]` and, if the 
// character was needed (pre-decrement value > 0), we decrement `count`.
// When `count` reaches 0, the window is valid. We then contract from the left by incrementing `mp[s[i]]`. 
// If `mp[s[i]]` becomes positive, we have lost a required character, so we increment `count` and stop contracting.

// Algorithm:
// 1. Build a frequency map `mp` of character counts in `t`.
// 2. Initialize `count` to the length of `t`, and set left pointer `i` and right pointer `j` to 0.
// 3. Expand the window by advancing `j`. If `mp[s[j]] > 0`, decrement `count`. Decrement `mp[s[j]]`.
// 4. If `count == 0` (window is valid), update the minimum window length if the current window is smaller.
//    Increment `mp[s[i]]` to remove the character at `i` from the window.
//    If `mp[s[i]] > 0`, increment `count` (window is no longer valid). Move `i` forward.
// 5. Repeat until `j` reaches the end of `s`. Return the minimal window or empty string.

// Time Complexity:
// O(n + m) where n is the length of s and m is the length of t. Each character is processed at most twice.

// Space Complexity:
// O(U) where U is the number of unique characters in t (at most 52).

namespace Optimal {
class Solution {
public:
    string minWindow(string s, string t) {
        // Hash map to store the frequency of characters in string t
        unordered_map<char, int> mp;
        // Populate the frequency map with characters of target string t
        for (auto x : t)
            mp[x]++;
        
        // count tracks the remaining number of characters of t that need to be matched in the window
        int count = t.length(), n = s.length();
        // Initialize the minimum length with infinity and start index of the window to 0
        int maxLen = INT_MAX, startIdx = 0;
        // Define sliding window pointers, i is the left boundary and j is the right boundary
        int i = 0, j = 0;
        
        // Loop through s with the right pointer to expand the sliding window
        while (j < n) {
            // If the character at the right pointer is needed (frequency > 0), decrement count
            if (mp[s[j]] > 0) {
                count--;
            }
            // Decrement the frequency of s[j] in the map (negative count indicates extra characters)
            mp[s[j]]--;
            
            // If the current window contains all characters of t, try to shrink from the left
            while (count == 0 && i <= j) {
                // Update the minimum window length and start index if the current window is smaller
                if (maxLen > j - i + 1) {
                    maxLen = j - i + 1;
                    startIdx = i;
                }
                // Restore the frequency count of the character s[i] as it is leaving the window
                mp[s[i]]++;
                // If s[i] was a required character and its frequency becomes positive, we need to match it again
                if (mp[s[i]] > 0) {
                    count++;
                }
                // Move the left pointer forward to shrink the window
                i++;
            }
            // Move the right pointer forward to expand the window
            j++;
        }

        // Return the minimum window substring if found, otherwise return an empty string
        return maxLen == INT_MAX ? "": s.substr(startIdx, maxLen);
    }
};
}