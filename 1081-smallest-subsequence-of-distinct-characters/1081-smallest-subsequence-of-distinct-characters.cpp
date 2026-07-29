// ===============================
// Brute Force Approach
// ===============================

// Approach:
// We can use backtracking to generate all subsequences of `s` that contain each unique character of the string exactly once.
// Among all such valid subsequences, we choose the lexicographically smallest one.

// Algorithm:
// 1. Determine the number of unique characters present in the input string `s`, denoted as `total_distinct`.
// 2. Perform backtracking starting from the beginning of string `s` (index 0).
// 3. At each character, we have two choices:
//    a. If the character hasn't been included in our current subsequence, we can mark it as visited, append it to `current`, and recurse on the next index. After returning, we backtrack by removing the character and marking it as unvisited.
//    b. We can skip the character and recurse on the next index.
// 4. When the size of `current` matches `total_distinct`, it means we have successfully constructed a subsequence of distinct characters containing all unique characters. We compare this subsequence with the best one found so far and keep the lexicographically smaller one.
// 5. If we reach the end of the string without matching the length, we backtrack.
// 6. Return the lexicographically smallest subsequence found.

// Time Complexity:
// O(2^N)

// Space Complexity:
// O(N)

namespace BruteForce {
class Solution {
private:
    // Helper function to generate subsequences and find the lexicographically smallest valid one.
    void backtrack(string& s, int idx, string& current, vector<bool>& visited, int total_distinct, string& result) {
        // If the subsequence contains all unique characters, check if it's the lexicographically smallest.
        if (current.length() == total_distinct) {
            if (result.empty() || current < result) {
                result = current;
            }
            return;
        }
        
        // Stop recursion if we reach the end of the input string.
        if (idx == s.length()) {
            // Note: Since we haven't reached total_distinct, this path is invalid.
            return;
        }
        
        // Option 1: Try to include the current character if it hasn't been included already.
        char c = s[idx];
        if (!visited[c - 'a']) {
            visited[c - 'a'] = true;
            current.push_back(c);
            // Move to the next index with the character included.
            backtrack(s, idx + 1, current, visited, total_distinct, result);
            // Backtrack: remove the character to explore other subsequences.
            current.pop_back();
            visited[c - 'a'] = false;
        }
        
        // Option 2: Try to exclude the current character and move to the next index.
        backtrack(s, idx + 1, current, visited, total_distinct, result);
    }
public:
    string smallestSubsequence(string s) {
        // Record all unique characters present in the input string.
        vector<bool> exists(26, false);
        int total_distinct = 0;
        for (char c : s) {
            if (!exists[c - 'a']) {
                exists[c - 'a'] = true;
                total_distinct++;
            }
        }
        
        // Tracks characters that are already selected in the current branch of recursion.
        vector<bool> visited(26, false);
        string result = "";
        string current = "";
        // Start backtracking from index 0.
        backtrack(s, 0, current, visited, total_distinct, result);
        return result;
    }
};
}

// ===============================
// Better Approach
// ===============================

// Approach:
// We can use a greedy recursive approach. In the lexicographically smallest subsequence, the first character must be 
// the smallest possible character `s[pos]` in a prefix of the string. But we cannot choose a character too far to the 
// right, because we must ensure that the suffix `s[pos+1:]` contains all other distinct characters.
// Thus, the boundary of our search is the index of the first character in the string that has no more occurrences to 
// its right. We scan the string from left to right, maintaining the smallest character seen so far. As soon as we encounter 
// a character that is at its last occurrence, we must stop scanning because any further scan would miss including this character.
// Once we find the smallest character `s[pos]` within this valid range, we append it to our result, remove all occurrences 
// of `s[pos]` from the remaining suffix `s[pos+1:]`, and recursively solve for the suffix.

// Algorithm:
// 1. Count the occurrences of each character in `s`.
// 2. Scan the string from left to right to find the index `pos` of the lexicographically smallest character.
// 3. For each character `s[i]` scanned:
//    a. Update the candidate index `pos` if `s[i] < s[pos]`.
//    b. Decrement the count of `s[i]`.
//    c. If the count of `s[i]` becomes 0, it means `s[i]` does not appear further in the suffix. We must stop the scan to ensure `s[i]` is not lost.
// 4. Construct a new string `suffix` starting from index `pos + 1` but omitting all occurrences of the character `s[pos]`.
// 5. Return `s[pos] + smallestSubsequence(suffix)`.

// Time Complexity:
// O(K * N) where K is the number of distinct characters (at most 26) and N is the length of the string.

// Space Complexity:
// O(K * N) due to the recursion stack of depth up to K and creating new suffix strings at each level.

namespace Better {
class Solution {
public:
    string smallestSubsequence(string s) {
        // Base case: if the string is empty, we return an empty subsequence.
        if (s.empty()) return "";
        
        // Count frequency of each character to know if a character will appear later.
        int cnt[26] = {0};
        for (char c : s) {
            cnt[c - 'a']++;
        }
        
        // Locate the index of the best candidate for the start of the subsequence.
        int pos = 0; 
        for (int i = 0; i < s.size(); i++) {
            // Pick the lexicographically smallest character as our greedy choice.
            if (s[i] < s[pos]) {
                pos = i;
            }
            // Decrement frequency as we pass the character in the scan.
            cnt[s[i] - 'a']--;
            // If this was the last occurrence of s[i], we cannot search beyond this point.
            if (cnt[s[i] - 'a'] == 0) {
                break;
            }
        }
        
        // Construct the remaining suffix string, omitting any occurrences of the picked character.
        string suffix = "";
        for (int i = pos + 1; i < s.size(); i++) {
            if (s[i] != s[pos]) {
                suffix += s[i];
            }
        }
        
        // Recurse on the modified suffix and append it to our chosen character.
        return s[pos] + smallestSubsequence(suffix);
    }
};
}

// ===============================
// Optimal Approach
// ===============================

// Approach:
// We can build the result character-by-character using a monotonic stack (simulated via a string) and a greedy approach.
// As we iterate through the string, for each character:
// 1. If it is already present in the stack, we skip it because we want each character exactly once.
// 2. Otherwise, we compare the current character with the character at the top of the stack. If the top character is 
//    lexicographically greater than the current character AND it will appear again later in the string (which we know 
//    by checking its last occurrence index), we can safely pop it from the stack. We repeat this check until the stack 
//    is empty or the top character is smaller than or doesn't appear later.
// 3. Push the current character onto the stack and mark it as taken.
// This ensures that we maintain the smallest possible lexicographical order while guaranteeing that every distinct character is represented exactly once.

// Algorithm:
// 1. Precompute the last occurrence index of each character in `s`.
// 2. Maintain a `taken` boolean array of size 26 to keep track of characters currently in our stack.
// 3. Use a string `str` as a stack.
// 4. Iterate through `s` with index `i`:
//    a. If `s[i]` is already in `taken`, skip it.
//    b. While `str` is not empty, `str.back() > s[i]`, and the last index of `str.back()` is greater than `i`:
//       i. Mark `str.back()` as not taken.
//       ii. Pop `str.back()`.
//    c. Append `s[i]` to `str` and mark `s[i]` as taken.
// 5. Return the string `str`.

// Time Complexity:
// O(N)

// Space Complexity:
// O(1) auxiliary space (O(K) if output storage is counted, where K is the number of distinct characters).

namespace Optimal {
class Solution {
public:
    string smallestSubsequence(string s) {
        // Track if a character is already in the active subsequence to prevent duplicate entries.
        vector<bool> taken(26, false);
        // Keep track of the last index where each character appears in the string s.
        vector<int> lastIdx(26);

        // Record the last occurrence of each character so we know if it can be safely removed and re-added later.
        for (int i = 0; i < s.size(); i++) {
            lastIdx[s[i] - 'a'] = i;
        }

        // Use a string as a stack to construct the lexicographically smallest result.
        string str = "";

        // Iterate through each character in the string to build the subsequence.
        for (int i = 0; i < s.size(); i++) {
            char c = s[i];

            // If the character is already in the stack, we skip it as we only need unique characters.
            if (taken[c - 'a'])
                continue;

            // Maintain a monotonic character sequence: pop larger characters if they appear again later.
            while (!str.empty() &&
                   str.back() > c &&
                   lastIdx[str.back() - 'a'] > i) {
                // Mark the popped character as no longer in the stack.
                taken[str.back() - 'a'] = false;
                // Pop the character from our stack representation.
                str.pop_back();
            }

            // Append the current character to the stack.
            str += c;
            // Mark the current character as present in the stack to avoid adding it again.
            taken[c - 'a'] = true;
        }

        // The stack representation contains the smallest subsequence.
        return str;
    }
};
}