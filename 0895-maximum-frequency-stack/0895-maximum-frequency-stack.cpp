class FreqStack {
public:
    unordered_map<int, int> mp;
    int count = 0;
    priority_queue<vector<int>> pq;

    FreqStack() {}

    void push(int val) {
        count++;
        mp[val]++;
        pq.push({mp[val], count,val});
    }

    int pop() {
        auto p = pq.top();
        pq.pop();
        auto v = p[2];
        mp[v]--;
        return v;
    }
};

/**
 * Your FreqStack object will be instantiated and called as such:
 * FreqStack* obj = new FreqStack();
 * obj->push(val);
 * int param_2 = obj->pop();
 */