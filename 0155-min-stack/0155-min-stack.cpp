class MinStack {
public:
    stack<long long> st, minSt;

    MinStack() {}

    void push(int val) {
        st.push(val);
        if (minSt.empty())
            minSt.push(val);
        else
            minSt.push(min(val * 1LL, minSt.top()));
    }

    void pop() {
        if (!st.empty()) {
            st.pop();
            minSt.pop();
        }
    }

    int top() { return st.top(); }

    int getMin() { return minSt.top(); }
};