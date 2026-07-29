class Solution {
public:
    string find_freqVector(string& s, int i, int j) {
        string str = "";
        vector<int> freq(26, 0);
        for (int k = i; k <= j; k++) {
            freq[s[k] - 'a']++;
        }
        for (auto x : freq) {
            str += x + '0';
        }
        return str;
    }
    bool checkInclusion(string s1, string s2) {
        int n = s2.length();
        int m = s1.length();
        string s1Freq = find_freqVector(s1, 0, m - 1);
        int l = 0, r = 0;
        while (r < n) {
            while (l <= r && r - l + 1 > m) {
                l++;
            }
            if (r - l + 1 == m) {
                string s2Freq = find_freqVector(s2, l,r);
                if (s1Freq == s2Freq)
                    return true;
            }
            r++;
        }
        return false;
    }
};