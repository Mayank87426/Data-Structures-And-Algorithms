// ===============================
// Brute Force Approach
// ===============================

// Approach:
// The brute force approach uses simple recursion. To find the minimum number of
// perfect squares that sum to n, we can try every perfect square j*j less than or
// equal to n, subtract it from n, and recursively find the solution for the remainder (n - j*j).
// We take the minimum across all choices.

// Algorithm:
// 1. Base case: If n is 0, the number of squares needed is 0.
// 2. Initialize a min_squares variable to n (since we can always represent n as sum of n 1s).
// 3. Loop through all integers j starting from 1 such that j * j <= n.
// 4. For each j, recursively call the function with n - j * j.
// 5. Update min_squares with the minimum of current min_squares and the result of the recursive call + 1.
// 6. Return min_squares.

// Time Complexity:
// O(n^(n/2)) - The recursion tree can have a depth up to n, and at each level, there are up to sqrt(n) branches.

// Space Complexity:
// O(n) - The recursion stack depth can go up to n in the worst case (e.g., n = 1 + 1 + ... + 1).

namespace BruteForce {
class Solution {
public:
    int numSquares(int n) {
        // Base case: if n is 0, we need 0 perfect squares to sum up to it.
        if (n == 0) {
            return 0;
        }
        
        // Initialize the minimum count of squares to n, since any positive integer n
        // can be represented by at most n ones (1^2 + 1^2 + ... + 1^2).
        int min_squares = n;
        
        // Loop through all integers j starting from 1 whose square is less than or equal to n.
        for (int j = 1; j * j <= n; ++j) {
            // Recursively calculate the minimum squares required for the remaining sum (n - j * j),
            // and update min_squares if this path yields a smaller number of squares (+1 for j*j itself).
            min_squares = min(min_squares, numSquares(n - j * j) + 1);
        }
        
        // Return the minimum count of perfect squares found for the given n.
        return min_squares;
    }
};
}

// ===============================
// Better Approach
// ===============================

// Approach:
// The Better approach uses 1D Dynamic Programming to avoid redundant subproblem calculations.
// We maintain an array dp where dp[i] stores the minimum number of perfect square numbers
// that sum to i. For each number i from 1 to n, we compute dp[i] by considering all perfect
// squares j*j <= i and finding the minimum of dp[i - j*j] + 1.

// Algorithm:
// 1. Initialize a DP array 'dp' of size n + 1, with all elements set to n + 1 (representing infinity).
// 2. Set the base case dp[0] = 0, as 0 can be represented by 0 perfect squares.
// 3. Loop through each integer i from 1 to n to fill the DP table.
// 4. For each i, loop through all j such that j * j <= i.
// 5. Update dp[i] to be the minimum of its current value and dp[i - j * j] + 1.
// 6. Return dp[n], which holds the answer for n.

// Time Complexity:
// O(n * sqrt(n)) - We iterate from 1 to n, and for each number i, we check up to sqrt(i) perfect squares.

// Space Complexity:
// O(n) - We allocate a DP array of size n + 1.

namespace Better {
class Solution {
public:
    int numSquares(int n) {
        // Create a DP vector of size n + 1 initialized to n + 1, representing a value larger than any possible answer.
        vector<int> dp(n + 1, n + 1);
        
        // Base case: 0 requires 0 perfect squares to sum up to it. This serves as our starting point.
        dp[0] = 0;

        // Iterate through all values from 1 up to n to compute the minimum squares needed for each subproblem.
        for (int i = 1; i <= n; i++) {
            // For each i, try subtracting every perfect square j * j that is less than or equal to i.
            for (int j = 1; j * j <= i; j++) {
                // Update the DP state to keep the minimum squares representation by adding 1 to the result of (i - j*j).
                dp[i] = min(dp[i], dp[i - j * j] + 1);
            }
        }

        // Return the computed minimum number of perfect squares for the input n.
        return dp[n];
    }
};
}

// ===============================
// Optimal Approach
// ===============================

// Approach:
// The Optimal approach uses Lagrange's Four-Square Theorem and Legendre's Three-Square Theorem.
// According to Lagrange's Four-Square Theorem, every natural number can be represented as the sum of
// at most four integer squares.
// According to Legendre's Three-Square Theorem, a natural number can be represented as the sum of three
// squares of integers if and only if n is not of the form n = 4^a * (8b + 7) for non-negative integers a and b.
// Therefore, the answer can only be 1, 2, 3, or 4:
// 1. If n is a perfect square, the answer is 1.
// 2. If n is of the form 4^a * (8b + 7), the answer is 4.
// 3. If n can be expressed as a^2 + b^2, the answer is 2.
// 4. Otherwise, the answer is 3.

// Algorithm:
// 1. Check if n is a perfect square (i.e. sqrt(n) * sqrt(n) == n). If so, return 1.
// 2. Reduce n by dividing by 4 as long as n is divisible by 4 (to extract the 4^a factor).
// 3. After reduction, if n % 8 == 7, then by Legendre's theorem, n requires 4 squares. Return 4.
// 4. Check if n is the sum of two perfect squares by iterating i from 1 to sqrt(n) and checking if n - i*i is a perfect square. If so, return 2.
// 5. If none of the conditions above are met, the minimum number of squares must be 3. Return 3.

// Time Complexity:
// O(sqrt(n)) - The sum of two squares check iterates up to sqrt(n) times. The reduction step runs in O(log n) time.

// Space Complexity:
// O(1) - Only a few integer variables are used for checks, requiring constant extra memory.

namespace Optimal {
class Solution {
public:
    int numSquares(int n) {
        // Check if the number n itself is a perfect square.
        int r = sqrt(n);
        // If it is a perfect square, we only need 1 square (n = r * r).
        if (r * r == n) {
            return 1;
        }

        // Divide n by 4 repeatedly to remove the 4^a factor for Legendre's Three-Square Theorem.
        int temp = n;
        while (temp % 4 == 0) {
            // Dividing temp by 4 preserves the property of being representable as a sum of squares.
            temp /= 4;
        }
        // If the remaining number is of the form 8b + 7, it cannot be represented as the sum of three squares.
        // Thus, by Lagrange's Four-Square Theorem, it must require exactly 4 squares.
        if (temp % 8 == 7) {
            return 4;
        }

        // Check if n can be represented as the sum of two perfect squares (n = i*i + j*j).
        for (int i = 1; i * i <= n; i++) {
            // Calculate the remaining part after subtracting the square of i.
            int remaining = n - i * i;
            // Compute the integer square root of the remaining value.
            int j = sqrt(remaining);
            // If the remaining value is a perfect square, then n is the sum of two squares (i*i + j*j).
            if (j * j == remaining) {
                return 2;
            }
        }

        // If n is not a perfect square, not the sum of two squares, and not of the form 4^a * (8b + 7),
        // then it must be the sum of exactly three squares.
        return 3;
    }
};
}