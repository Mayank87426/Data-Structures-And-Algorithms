class Solution {
public:
    long long power(long long a, long long b) {
        long long result = 1;
        while (b > 0) {
            if (b & 1)
                result *= a;
            a *= a;
            b >>= 1;
        }
        return result;
    }
    int nthUglyNumber(int n) {
        unordered_set<long long> nums;
        for (int i = 0; i <= 30; i++) {
            for (int j = 0; j <= 20; j++) {
                for (int k = 0; k <= 15; k++) {
                    long long a = power(2, i);
                    long long b = power(3, j);
                    long long c = power(5, k);
                    if (a > INT_MAX / b)
                        continue;
                    long long ab = a * b;
                    if (ab > INT_MAX / c)
                        continue;
                    long long val = ab * c;
                    nums.insert(val);

                }
            }
        }
        vector<long long> v;
        for (auto it : nums)
            v.push_back(it);
        sort(begin(v), end(v));
        return v[n - 1];
    }
};