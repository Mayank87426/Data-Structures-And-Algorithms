class Solution {
public:
    int firstMissingPositive(vector<int>& nums) {
        unordered_set<int> st(begin(nums), end(nums));
        int firstPos = 1;
        while (true) {
            if (st.find(firstPos) != st.end())
                firstPos++;
            else
                break;
        }
        return firstPos;
    }
};