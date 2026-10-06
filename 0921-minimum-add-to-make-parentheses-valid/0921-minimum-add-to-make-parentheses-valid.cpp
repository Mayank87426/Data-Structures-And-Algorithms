class Solution {
public:
    int minAddToMakeValid(string s) {
        int n = s.length();
        int count = 0;
        stack<char> st;
        for (auto x : s) {
            if (x == '(')
                st.push(x);
            else {
                while (!st.empty() && st.top() != '(')
                    st.pop();
                if (!st.empty())
                    st.pop();
                else
                    count++;
            }
        }
        return st.size() + count;
    }
};