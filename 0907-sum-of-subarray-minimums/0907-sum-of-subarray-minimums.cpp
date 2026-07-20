class Solution {
public:
    vector<int> findNSL(vector<int>& nums) {
        int n = nums.size();
        vector<int> NSL(n);
        stack<int> st;

        for (int i = 0; i < n; i++) {
            while (!st.empty() && nums[i] <= nums[st.top()]) {
                st.pop();
            }

            NSL[i] = st.empty() ? -1 : st.top();
            st.push(i);
        }

        return NSL;
    }

    vector<int> findNSR(vector<int>& nums) {
        int n = nums.size();
        vector<int> NSR(n);
        stack<int> st;

        for (int i = n - 1; i >= 0; i--) {
            while (!st.empty() && nums[i] < nums[st.top()]) {
                st.pop();
            }

            NSR[i] = st.empty() ? n : st.top();
            st.push(i);
        }

        return NSR;
    }

    int sumSubarrayMins(vector<int>& arr) {
        int n = arr.size();
        const int MOD = 1e9 + 7;
        vector<int> NSL = findNSL(arr);
        vector<int> NSR = findNSR(arr);
        long long ans = 0;
        for (int i = 0; i < n; i++) {
            long long left = i - NSL[i];
            long long right = NSR[i] - i;

            ans = (ans + (left * right % MOD) * arr[i]) % MOD;
        }

        return ans;
    }
};