
class Solution {
public:
    static const int MOD = 1000000007;
    static const int MAXN = 1000;

    long long fact[MAXN + 1], invFact[MAXN + 1];

    long long modPow(long long a, long long b) {
        long long res = 1;
        while (b > 0) {
            if (b & 1) res = res * a % MOD;
            a = a * a % MOD;
            b >>= 1;
        }
        return res;
    }

    void precompute() {
        fact[0] = 1;
        for (int i = 1; i <= MAXN; i++)
            fact[i] = fact[i - 1] * i % MOD;

        invFact[MAXN] = modPow(fact[MAXN], MOD - 2);

        for (int i = MAXN; i >= 1; i--)
            invFact[i - 1] = invFact[i] * i % MOD;
    }

    long long nCr(int n, int r) {
        if (r < 0 || r > n) return 0;
        return fact[n] * invFact[r] % MOD * invFact[n - r] % MOD;
    }

    long long solve(vector<int>& nums) {
        int n = nums.size();
        if (n < 3) return 1;

        int root = nums[0];
        vector<int> left, right;

        for (int i = 1; i < n; i++) {
            if (nums[i] < root)
                left.push_back(nums[i]);
            else
                right.push_back(nums[i]);
        }

        long long x = solve(left);
        long long y = solve(right);
        long long z = nCr(n - 1, left.size());

        return x * y % MOD * z % MOD;
    }

    int numOfWays(vector<int>& nums) {
        precompute();
        return (solve(nums) - 1 + MOD) % MOD;
    }
};
