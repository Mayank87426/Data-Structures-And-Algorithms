// ===============================
// Brute Force Approach
// ===============================

// Approach:
// The brute force approach computes the sum of the first n positive odd numbers
// and the first n positive even numbers by iteratively generating each term
// and accumulating them using loops. Once the sums are computed, it uses the
// GCD function to find their greatest common divisor.

// Algorithm:
// 1. Initialize `sume = 0` to store the sum of even numbers.
// 2. Use a loop that runs `n` times, starting with the first even number `i = 2`.
//    In each iteration, add `i` to `sume` and increment `i` by 2.
// 3. Initialize `sum = 0` to store the sum of odd numbers.
// 4. Use a second loop that runs `n` times, starting with the first odd number `i = 1`.
//    In each iteration, add `i` to `sum` and increment `i` by 2.
// 5. Compute and return the greatest common divisor of `sume` and `sum`.

// Time Complexity:
// O(n)

// Space Complexity:
// O(1)

namespace BruteForce {
class Solution {
public:
    int gcdOfOddEvenSums(int n) {
        // Initialize an accumulator sume to store the sum of even numbers.
        int sume = 0;
        // Set counter k to n and starting even number i to 2.
        int k = n, i = 2;
        // Loop n times to sum the first n positive even numbers.
        while (k--) {
            // Add the current even number to the even sum accumulator.
            sume += i;
            // Move to the next consecutive positive even number.
            i += 2;
        }
        // Initialize an accumulator sum to store the sum of odd numbers.
        int sum = 0;
        // Reset i to 1, the first positive odd number.
        i = 1;
        // Loop n times using the remaining value of n to sum the first n positive odd numbers.
        while (n--) {
            // Add the current odd number to the odd sum accumulator.
            sum += i;
            // Move to the next consecutive positive odd number.
            i += 2;
        }
        // Calculate and return the greatest common divisor of the even and odd sums.
        return gcd(sume, sum);
    }
};
}

// ===============================
// Better Approach
// ===============================

// Approach:
// Instead of using loops, we can use the mathematical formulas for the sum of
// arithmetic progressions to compute the sums in O(1) time:
// - The sum of the first n positive odd numbers (1 + 3 + ... + 2n-1) is n^2.
// - The sum of the first n positive even numbers (2 + 4 + ... + 2n) is n * (n + 1).
// Then, we calculate the GCD of these two formulas. This reduces the time complexity
// of the sum calculation to O(1), leaving only the GCD calculation.

// Algorithm:
// 1. Compute `sumOdd` as `n * n`.
// 2. Compute `sumEven` as `n * (n + 1)`.
// 3. Compute and return the greatest common divisor of `sumOdd` and `sumEven`.

// Time Complexity:
// O(log n)

// Space Complexity:
// O(1)

namespace Better {
class Solution {
public:
    int gcdOfOddEvenSums(int n) {
        // Use the arithmetic progression sum formula for the first n odd numbers: n * n.
        int sumOdd = n * n;
        // Use the arithmetic progression sum formula for the first n even numbers: n * (n + 1).
        int sumEven = n * (n + 1);
        // Calculate the greatest common divisor of the two sums.
        return gcd(sumOdd, sumEven);
    }
};
}

// ===============================
// Optimal Approach
// ===============================

// Approach:
// We can simplify the expression GCD(sumOdd, sumEven) mathematically:
// GCD(sumOdd, sumEven) = GCD(n^2, n * (n + 1))
// Since both terms share a factor of n, we can factor it out:
// GCD(n^2, n * (n + 1)) = n * GCD(n, n + 1)
// The numbers n and n + 1 are consecutive integers, which means they are always coprime.
// Therefore, GCD(n, n + 1) = 1.
// Substituting this back gives:
// n * 1 = n.
// Thus, the GCD of the two sums is always exactly equal to n itself, which we can return immediately in O(1) time.

// Algorithm:
// 1. Return the input value `n` directly.

// Time Complexity:
// O(1)

// Space Complexity:
// O(1)

namespace Optimal {
class Solution {
public:
    int gcdOfOddEvenSums(int n) {
        // Return n directly because the mathematical simplified form of GCD(n^2, n * (n + 1)) is always n.
        return n;
    }
};
}