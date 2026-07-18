class Solution {
public:
    int carFleet(int target, vector<int>& position, vector<int>& speed) {
        int n = position.size();
        if (n == 0) return 0;

        vector<pair<int, double>> posAndTime;

        for (int i = 0; i < n; i++) {
            double time = (double)(target - position[i]) / speed[i];
            posAndTime.push_back({position[i], time});
        }

        sort(posAndTime.begin(), posAndTime.end(), greater<>());

        int count = 1;
        double fleetTime = posAndTime[0].second;

        for (int i = 1; i < n; i++) {
            if (posAndTime[i].second > fleetTime) {
                count++;
                fleetTime = posAndTime[i].second;
            }
        }

        return count;
    }
};