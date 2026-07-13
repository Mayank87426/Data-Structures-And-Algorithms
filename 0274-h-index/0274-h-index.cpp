class Solution {
public:
    int hIndex(vector<int>& citations) {
        int n = citations.size();
        sort(begin(citations), end(citations));
        int h_idx = n;
        while (h_idx >= 1) {
            auto it = lower_bound(begin(citations), end(citations), h_idx);
            if (it == citations.end()) {
                h_idx--;
                continue;
            }
            int idx = it - citations.begin();
            if (idx <= n - h_idx )
                return h_idx;
            h_idx--;
        }
        return h_idx;
    }
};