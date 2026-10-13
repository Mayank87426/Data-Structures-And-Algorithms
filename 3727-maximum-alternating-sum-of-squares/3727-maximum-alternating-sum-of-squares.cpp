class Solution {
public:
    long long maxAlternatingSum(vector<int>& nums) {
        priority_queue<int> pq;
        long long ans = 0;
        for (auto x : nums)
            pq.push(abs(x));
        int n = (pq.size() +1)/2;
        cout<<n;
        while (n--) {
            int x = pq.top();
            ans += 1LL * pow(x, 2);
            pq.pop();
        }
        while (!pq.empty()) {
            int x = pq.top();
            ans -= 1LL * pow(x, 2);
            pq.pop();
        }
        return ans;
    }
};