// ===============================
// Brute Force Approach
// ===============================

// Approach:
// Iterate through every single integer in the range [low, high] and check if it has sequential digits.
// To determine if a number is sequential, convert the integer to a string and check if each digit
// is exactly one greater than the preceding digit.

// Algorithm:
// 1. Loop through all integers from low to high.
// 2. For each number, convert it to a string.
// 3. Compare adjacent digits to verify the sequential condition.
// 4. If the number is sequential, add it to the result vector.
// 5. Return the accumulated results.

// Time Complexity:
// O((high - low) * log10(high)) - We iterate through high - low numbers, and check each number's digits.

// Space Complexity:
// O(log10(high)) - Auxiliary space for storing the string representation of the numbers.

namespace BruteForce {
class Solution {
public:
    // Helper function to check if the digits of n are sequential.
    bool check(int n) {
        // Convert the integer to string to examine individual digits.
        string s = to_string(n);
        // Compare adjacent digits starting from the second digit.
        for (int i = 1; i < s.length(); i++) {
            // If any digit is not exactly one greater than the previous one, return false.
            if (s[i] != s[i - 1] + 1)
                return false;
        }
        // Return true if the condition is met for all digits.
        return true;
    }

    vector<int> sequentialDigits(int low, int high) {
        // Vector to store the resulting sequential numbers.
        vector<int> result;
        // Iterate through every number in the range [low, high] inclusive.
        for (int i = low; i <= high; i++) {
            // Check if the current number has sequential digits.
            if (check(i)) {
                // Add the valid sequential number to the result vector.
                result.push_back(i);
            }
        }
        // Return the collected sequential numbers.
        return result;
    }
};
}

// ===============================
// Better Approach
// ===============================

// Approach:
// Generate all possible sequential digits by varying the number of digits (length) from 2 to 9
// and the starting digit. This generates all valid sequential numbers directly. We then filter
// only those that fall within the [low, high] range. Since we iterate in order of length and starting
// digit, the generated numbers are naturally sorted.

// Algorithm:
// 1. Loop through lengths from 2 to 9 (since low >= 10, minimum length is 2).
// 2. Loop through all valid starting digits (1 to 9 - length + 1).
// 3. Mathematically construct the sequential number of the current length and starting digit.
// 4. If the constructed number lies in the range [low, high], append it to the result vector.
// 5. Return the result vector.

// Time Complexity:
// O(1) - There are only a total of 36 sequential numbers, so the generation loops run a constant number of times.

// Space Complexity:
// O(1) - No extra data structures are used other than the output vector.

namespace Better {
class Solution {
public:
    vector<int> sequentialDigits(int low, int high) {
        // Vector to store the valid sequential numbers.
        vector<int> result;
        // Loop through all possible lengths of sequential digits from 2 to 9.
        for (int len = 2; len <= 9; len++) {
            // Loop through all possible starting digits for the current length.
            for (int start = 1; start <= 9 - len + 1; start++) {
                int num = 0;
                // Construct the sequential number digit by digit.
                for (int i = 0; i < len; i++) {
                    // Shift the current number to the left and append the next digit.
                    num = num * 10 + (start + i);
                }
                // Check if the constructed number falls within the range [low, high].
                if (num >= low && num <= high) {
                    // Store the number if it is within bounds.
                    result.push_back(num);
                }
            }
        }
        // Return the naturally sorted vector of sequential numbers.
        return result;
    }
};
}

// ===============================
// Optimal Approach
// ===============================

// Approach:
// Start from the lower bound, find the first sequential number that is >= low, and then
// generate subsequent sequential digits of the same length by adding a step pattern (like 111, 1111).
// If a length boundary is exceeded, scale the pattern up to the next length and continue.
// This original logic steps directly through candidates and validates using a check helper.

// Algorithm:
// 1. Find the initial length of sequential digits based on the length of low.
// 2. Construct the start patterns (e.g. "123" and "111") for that length.
// 3. Calculate the first sequential digit >= low using helper nextSequential.
// 4. Loop while the current number n <= high:
//    - If n is not sequential, increment it by the step pattern j until a sequential number is found
//      or length increases, in which case we scale up the patterns.
//    - If n is within high and is sequential, add it to the results.
//    - Advance n by adding the step pattern j (or scale up if changing lengths).
// 5. Return the accumulated sequential digits.

// Time Complexity:
// O(1) - Since the maximum number of digits is 9 and there are only 36 total sequential numbers,
// the loops run a constant number of times.

// Space Complexity:
// O(1) - The space used for variables and auxiliary strings is bounded by the maximum number of digits (9).

namespace Optimal {
class Solution {
public:
    // Helper function to check if the digits of n are sequential (each digit is exactly 1 more than the previous).
    bool check(int n) {
        // Convert the integer to string to easily access individual digits.
        string s = to_string(n);
        // Iterate through the string starting from the second digit.
        for (int i = 1; i < s.length(); i++) {
            // Check if the current digit is not exactly one more than the previous digit.
            if (s[i] != s[i - 1] + 1)
                // If the sequential condition is violated, return false.
                return false;
        }
        // If all digits satisfy the sequential condition, return true.
        return true;
    }

    // Helper function to find the smallest sequential digit number that is greater than or equal to low.
    int nextSequential(int low) {
        // Get the number of digits in the 'low' integer to start checking sequential numbers of this length.
        int len = to_string(low).size();

        // Sequential numbers can only have length up to 9 (digits 1 to 9).
        while (len <= 9) {
            // Iterate over all possible starting digits for a sequential number of length 'len'.
            for (int start = 1; start + len - 1 <= 9; start++) {
                int num = 0;
                // Construct the sequential number digit by digit.
                for (int d = start; d < start + len; d++)
                    num = num * 10 + d;

                // If the constructed sequential number is at least 'low', we have found our starting point.
                if (num >= low)
                    return num;
            }
            // If no sequential number of length 'len' is >= low, increase length and try again.
            len++;
        }
        // Return INT_MAX if no such sequential number exists (e.g., if low is larger than 123456789).
        return INT_MAX;
    }

    vector<int> sequentialDigits(int low, int high) {
        // Find the number of digits in low to build our initial step pattern and starting sequence.
        int digits = to_string(low).length();

        // 'str' will hold the base sequential string (e.g., "123" for 3 digits), and 'd' will hold ones (e.g., "111").
        string str = "", d = "";
        // 'num' keeps track of the next digit to append when we scale up in length.
        int num = 1, k = digits;

        // Loop to construct initial sequential string and step pattern corresponding to the length of 'low'.
        while (k--) {
            // Append the digit to build the base sequential sequence (like '1', '2', '3'...).
            str.push_back(num + '0');
            // Append '1' to construct the step addition pattern (like '1', '1', '1'...).
            d.push_back('1');
            // Move to the next digit.
            num++;
        }

        // Find the first sequential number that is >= low.
        low = nextSequential(low);

        // 'n' will keep track of the current number we are evaluating.
        int n = low;
        // 'j' is the mathematical difference between sequential numbers of the current length (e.g., 111).
        int j = stoi(d);

        // Vector to store the result sequence.
        vector<int> sequenceDigits;

        // Loop to find and generate sequential digits within the [low, high] range.
        while (n <= high) {

            // If the current number is not sequential, add the step difference until a sequential one is reached or high is exceeded.
            while (!check(n) && n <= high) {
                // Increment 'n' by the step difference 'j'.
                n += j;

                // If incrementing 'n' changes its digit count, we must transition to the next length.
                if (to_string(n).length() > str.length()) {
                    // Append the next sequential digit to the base sequence string.
                    str.push_back(num + '0');
                    // Reset 'n' to the smallest sequential number of the new length.
                    n = stoi(str);
                    // Increment the next digit to be appended for subsequent length transitions.
                    num++;
                    // Scale the step increment to match the new length (e.g., from 111 to 1111).
                    j = j * 10 + 1;
                }
            }

            // Break if the generated number exceeds the high limit.
            if (n > high)
                break;

            // Add the valid sequential number to the result list if it's not already added.
            if (sequenceDigits.empty() || sequenceDigits.back() != n)
                sequenceDigits.push_back(n);

            // Check if the current sequential number is at the boundary of changing lengths.
            if (to_string(n).length() > str.length()) {
                // Scale up the sequential string with the next digit.
                str.push_back(num + '0');
                // Reset 'n' to the smallest sequential number of the new length.
                n = stoi(str);
                // Increment the next digit to be appended.
                num++;
                // Scale up the step increment 'j'.
                j = j * 10 + 1;
            } else {
                // Otherwise, move to the next sequential number of the same length by adding 'j'.
                n += j;
            }
        }

        // Return the final list of sorted sequential digits.
        return sequenceDigits;
    }
};
}