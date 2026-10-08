class Solution {
public:
    string removeOuterParentheses(string s) {
        int i = 0, j = 0;
        unordered_set<int> st;
        int sum = 0;
        while (j < s.length()) {
            sum += s[j] == '(' ? 1 : -1;
            if (sum == 0) {
                st.insert(i);
                st.insert(j);
                sum = 0;
                i = j + 1;
            }
            j++;
        }
        string ans;
        for (int i = 0; i < s.length(); i++) {
            if (!st.count(i))
                ans.push_back(s[i]);
        }
        return ans;
    }
};