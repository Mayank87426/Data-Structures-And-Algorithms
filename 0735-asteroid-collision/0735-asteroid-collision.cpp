class Solution {
public:
    vector<int> asteroidCollision(vector<int>& asteroids) {
        stack<int> st;
        for (auto& x : asteroids) {
            if (x > 0) {
                st.push(x);
            } else {
                if (!st.empty() && st.top() < 0) {
                    st.push(x);
                    continue;
                }
                while (!st.empty() && st.top() > 0 && abs(st.top()) < abs(x)) {
                    st.pop();
                }
                if (st.empty() || st.top()<0){
                    st.push(x);
                    continue;
                }
                if (!st.empty() && abs(st.top()) == abs(x)) {
                    st.pop();
                    continue;
                }
            }
        }
        vector<int> remainingAsteroids;
        while (!st.empty()) {
            remainingAsteroids.push_back(st.top());
            st.pop();
        }
        reverse(begin(remainingAsteroids), end(remainingAsteroids));
        return remainingAsteroids;
    }
};