class Solution {
public:
    int digitSum(int n) {
        n = abs(n);
        int sum = 0;

        while (n > 0) {
            sum += n % 10;
            n /= 10;
        }

        return sum;
    }
    int minSwaps(vector<int>& nums) {
        auto v = nums;
        sort(nums.begin(), nums.end(), [&](int a, int b) {
            int sumA = digitSum(a);
            int sumB = digitSum(b);

            if (sumA != sumB)
                return sumA < sumB;

            return a < b;
        });
        int n = v.size(), count = 0;
        unordered_map<int, int> mp;
        for (int i = 0; i < n; i++)
            mp[v[i]] = i;
        for (int i = 0; i < n; i++) {
            if (v[i] != nums[i]) {
                auto curr = nums[i];
                int idx = mp[nums[i]];
                mp[v[i]] = idx;
                mp[v[idx]] = i;
                swap(v[i], v[idx]);
                count++;
            }
        }
        return count;
    }
};