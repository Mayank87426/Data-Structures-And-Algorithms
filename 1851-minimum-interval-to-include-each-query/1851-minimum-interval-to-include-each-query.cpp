class Solution {
public:
    vector<int> minInterval(vector<vector<int>>& intervals,
                            vector<int>& queries) {
        int n = intervals.size(), q = queries.size();
        vector<int> ans(q);

        vector<pair<int, int>> qs;
        for (int i = 0; i < q; i++)
            qs.push_back({queries[i], i});

        sort(qs.begin(), qs.end());
        sort(intervals.begin(), intervals.end());

        priority_queue<pair<int, int>, vector<pair<int, int>>,
                       greater<pair<int, int>>> pq;

        int i = 0;

        for (auto &[query, idx] : qs) {
            while (i < n && intervals[i][0] <= query) {
                pq.push({intervals[i][1] - intervals[i][0] + 1,
                         intervals[i][1]});
                i++;
            }

            while (!pq.empty() && pq.top().second < query)
                pq.pop();

            ans[idx] = pq.empty() ? -1 : pq.top().first;
        }

        return ans;
    }
};