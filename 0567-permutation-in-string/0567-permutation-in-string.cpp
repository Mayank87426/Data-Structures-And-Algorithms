class Solution {
public:
    vector<int> find_freqVector(string& s, int i, int j) {
        string str = "";
        vector<int> freq(26, 0);
        for (int k = i; k <= j; k++) {
            freq[s[k] - 'a']++;
        }
        return freq;
    }
    bool checkInclusion(string s1, string s2) {
        int n = s2.length();
        int m = s1.length();
        vector<int> s1Freq = find_freqVector(s1, 0, m - 1);
        int l = 0, r = 0;
        vector<int> currFreq(26, 0);
        while (r < n) {
            currFreq[s2[r] - 'a']++;
            while (l <= r && r - l + 1 > m) {
                currFreq[s2[l] - 'a']--;
                l++;
            }
            if (r - l + 1 == m) {

                if (s1Freq == currFreq)
                    return true;
            }
            r++;
        }
        return false;
    }
};