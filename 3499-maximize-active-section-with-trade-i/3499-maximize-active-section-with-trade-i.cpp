class Solution {
public:
    int maxActiveSectionsAfterTrade(string s) {
        int oneCount = count(s.begin(), s.end(), '1');
        if (oneCount == 0)
            return 0;

        string str = "1" + s + "1";
        int n = str.length();

        vector<int> prefix(n, 0), suffix(n, 0);

        for (int i = 1; i < n; i++) {
            if (str[i - 1] == '0')
                prefix[i] = prefix[i - 1] + 1;
        }

   
        for (int i = n - 2; i >= 0; i--) {
            if (str[i + 1] == '0')
                suffix[i] = suffix[i + 1] + 1;
        }

        int maxGain = 0;

        for (int i = 1; i < n - 1;) {
            if (str[i] == '0') {
                i++;
                continue;
            }

            int l = i;
            while (i < n && str[i] == '1') i++;
            int r = i - 1;

            if (prefix[l] > 0 && suffix[r] > 0) {
                maxGain = max(maxGain, prefix[l] + suffix[r]);
            }
        }

        return oneCount + maxGain;
    }
};