class Solution {
public:
    int maximumPopulation(vector<vector<int>>& logs) {
        sort(begin(logs), end(logs));
        int n = logs.size();
        int maxYear = 0, maxPopulation = 0;
        for (auto life : logs) {
            for (auto year : life) {
                int count = 0;
                for (auto people : logs) {
                    int b = people[0], d = people[1];
                    if (b <= year && d > year) {
                        count++;
                    }
                }
                if (count > maxPopulation ||
                    (count == maxPopulation && year < maxYear)) {
                    maxPopulation = count;
                    maxYear = year;
                }
            }
        }
        return maxYear;
    }
};