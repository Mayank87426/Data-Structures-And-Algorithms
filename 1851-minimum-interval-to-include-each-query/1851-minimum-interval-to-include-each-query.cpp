// ===============================
// Brute Force Approach
// ===============================

// Approach:
// For each query, iterate through all the intervals to check if the query falls within [left, right].
// Among all such intervals that contain the query, track the one with the minimum size.
// This is simple but inefficient, leading to Time Limit Exceeded (TLE) for large inputs.

// Algorithm:
// 1. Initialize the result array ans of size queries.size() with -1.
// 2. For each query, iterate through every interval in intervals.
// 3. If the query falls in [left, right] of the interval, calculate its size (right - left + 1).
// 4. Track the minimum size seen so far.
// 5. Store the minimum size for the query in ans.
// 6. Return the ans array.

// Time Complexity:
// O(q * n) where q is the number of queries and n is the number of intervals.

// Space Complexity:
// O(1) auxiliary space (excluding the output array).

namespace BruteForce {
class Solution {
public:
    vector<int> minInterval(vector<vector<int>>& intervals,
                            vector<int>& queries) {
        // Cache the size of queries to initialize the result vector.
        int q = queries.size();
        // Initialize the result vector with -1 to default to no containing interval found.
        vector<int> ans(q, -1);

        // Iterate through each query individually to search for containing intervals.
        for (int i = 0; i < q; i++) {
            int query = queries[i];
            int min_len = -1;

            // Check every interval in the input list for containment.
            for (const auto& interval : intervals) {
                int left = interval[0];
                int right = interval[1];

                // Check if the query lies within the boundaries of the current interval.
                if (left <= query && query <= right) {
                    int len = right - left + 1;
                    // If this is the first containing interval found, or it is smaller than the minimum found so far, update the minimum.
                    if (min_len == -1 || len < min_len) {
                        min_len = len;
                    }
                }
            }
            // Assign the smallest containing interval size (or -1) to the current query index.
            ans[i] = min_len;
        }

        // Return the resulting answers for all queries.
        return ans;
    }
};
}

// ===============================
// Better Approach
// ===============================

// Approach:
// Sort the intervals by their length (size) in ascending order. Store all queries in a self-balancing
// binary search tree (std::set) along with their original indices. For each interval, find all active
// queries that fall within its bounds, assign the interval's size as their answer, and erase them from the set.
// Since we process intervals from smallest to largest, the first interval that contains a query is
// guaranteed to be the smallest containing interval for that query.

// Algorithm:
// 1. Sort intervals by size in ascending order.
// 2. Insert all queries as pairs of {query_value, original_index} into a std::set.
// 3. Iterate through each sorted interval [left, right]:
//    a. Use lower_bound on the set to locate the first query >= left.
//    b. While the query value is <= right, assign the interval's size to ans[original_index].
//    c. Erase the answered query from the set so that it is not considered again.
// 4. Return the ans array.

// Time Complexity:
// O(n * log(n) + q * log(q) + n * log(q)) where n is the number of intervals and q is the number of queries.
// Specifically, sorting intervals takes O(n * log(n)), inserting queries into the set takes O(q * log(q)),
// performing lower_bound for each interval takes O(n * log(q)), and erasing each query from the set takes O(q * log(q)) in total.

// Space Complexity:
// O(n + q) to store the copy of sorted intervals and the active queries in the set.

namespace Better {
class Solution {
public:
    vector<int> minInterval(vector<vector<int>>& intervals,
                            vector<int>& queries) {
        // Cache the sizes of intervals and queries for processing.
        int n = intervals.size();
        int q = queries.size();
        
        // Copy intervals to sort them by size without modifying the original input structure.
        vector<vector<int>> sorted_intervals = intervals;
        // Sort intervals in ascending order of their size to process smaller intervals first.
        sort(sorted_intervals.begin(), sorted_intervals.end(), [](const vector<int>& a, const vector<int>& b) {
            return (a[1] - a[0]) < (b[1] - b[0]);
        });
        
        // Use a set of {query_value, original_index} to quickly look up and erase queries that fall within current interval.
        set<pair<int, int>> active_queries;
        for (int i = 0; i < q; i++) {
            active_queries.insert({queries[i], i});
        }
        
        // Initialize result vector with -1 to handle queries that are not covered by any interval.
        vector<int> ans(q, -1);
        
        // Process each interval starting from the smallest size.
        for (const auto& interval : sorted_intervals) {
            int left = interval[0];
            int right = interval[1];
            int size = right - left + 1;
            
            // Find the first query value that is greater than or equal to the left boundary of the interval.
            auto it = active_queries.lower_bound({left, 0});
            
            // Iterate through and answer all queries that lie within the right boundary of the interval.
            while (it != active_queries.end() && it->first <= right) {
                // Since intervals are sorted by size, the first time we cover a query, it is guaranteed to be the minimum size.
                ans[it->second] = size;
                // Remove the query from the set so it is not processed or overwritten by larger intervals.
                it = active_queries.erase(it);
            }
        }
        
        // Return the final answers for all queries.
        return ans;
    }
};
}

// ===============================
// Optimal Approach
// ===============================

// Approach:
// Sort both queries and intervals in ascending order. Use a min-priority queue (min-heap) to keep track
// of active intervals. For each query, add all intervals that start before or at the query into the heap,
// and discard intervals from the heap that end before the query. The top of the min-heap will represent
// the smallest interval currently containing the query.

// Algorithm:
// 1. Pair each query with its original index, then sort them in ascending order.
// 2. Sort intervals by their start times.
// 3. Initialize a min-priority queue containing pairs of {interval_size, end_time}.
// 4. For each sorted query:
//    a. Push all intervals whose start time <= query onto the min-heap.
//    b. Pop intervals from the heap whose end time < query since they cannot cover the current or subsequent queries.
//    c. If the heap is not empty, the top of the heap is the smallest interval containing the query. Store its size in the answer array at the query's original index. Otherwise, store -1.
// 5. Return the answer array.

// Time Complexity:
// O(n * log(n) + q * log(q)) where n is the number of intervals and q is the number of queries.
// Sorting intervals takes O(n * log(n)) and sorting queries takes O(q * log(q)). Each interval is pushed
// and popped from the min-priority queue at most once, taking O(n * log(n)) total time.

// Space Complexity:
// O(n + q) to store the query-index pairs and the intervals in the min-priority queue.

namespace Optimal {
class Solution {
public:
    vector<int> minInterval(vector<vector<int>>& intervals,
                            vector<int>& queries) {
        // Cache the size of intervals and queries for loop boundaries and vector initialization.
        int n = intervals.size(), q = queries.size();
        // Initialize the result vector with size q to store answers for all queries.
        vector<int> ans(q);

        // Store queries with their original indices to reconstruct the answer array in the correct order after sorting.
        vector<pair<int, int>> qs;
        for (int i = 0; i < q; i++)
            qs.push_back({queries[i], i});

        // Sort queries in ascending order to process them offline and match with sorted intervals efficiently.
        sort(qs.begin(), qs.end());
        // Sort intervals by start time to process candidates in a sweep-line fashion.
        sort(intervals.begin(), intervals.end());

        // Min-heap to store active intervals as {interval_size, end_time}, prioritizing the smallest size.
        priority_queue<pair<int, int>, vector<pair<int, int>>,
                       greater<pair<int, int>>> pq;

        // Pointer to track the current interval being considered for insertion into the min-heap.
        int i = 0;

        // Iterate through each sorted query to find its minimum containing interval.
        for (auto &[query, idx] : qs) {
            // Push all intervals that start before or at the current query value into the min-heap.
            while (i < n && intervals[i][0] <= query) {
                pq.push({intervals[i][1] - intervals[i][0] + 1,
                         intervals[i][1]});
                i++;
            }

            // Remove intervals from the min-heap that end before the current query, as they cannot cover it or any subsequent queries.
            while (!pq.empty() && pq.top().second < query)
                pq.pop();

            // If the heap is not empty, the top element has the minimum size and covers the current query; otherwise, no interval covers it.
            ans[idx] = pq.empty() ? -1 : pq.top().first;
        }

        // Return the final reconstructed answers for all queries.
        return ans;
    }
};
}