class Solution {
public:
    vector<int> getFreq(string &s, unordered_set<char> &st) {
        vector<int> freq(128, 0);
        for (char c : s) {
            if (st.count(c))
                freq[c]++;
        }
        return freq;
    }

    bool valid(vector<int> &windowFreq, vector<int> &targetFreq) {
        for (int i = 0; i < 128; i++) {
            if (windowFreq[i] < targetFreq[i])
                return false;
        }
        return true;
    }

    string minWindow(string s, string t) {
        unordered_set<char> st(begin(t), end(t));

        vector<int> targetFreq = getFreq(t, st);
        vector<int> windowFreq(128, 0);

        int n = s.size();
        int i = 0, j = 0;
        int startIdx = 0;
        int minLen = INT_MAX;

        while (j < n) {
            if (st.count(s[j]))
                windowFreq[s[j]]++;

            while (i <= j && valid(windowFreq, targetFreq)) {
                if (j - i + 1 < minLen) {
                    minLen = j - i + 1;
                    startIdx = i;
                }

                if (st.count(s[i]))
                    windowFreq[s[i]]--;

                i++;
            }

            j++;
        }

        return (minLen == INT_MAX) ? "" : s.substr(startIdx, minLen);
    }
};