// ===============================
// Brute Force Approach
// ===============================

// Approach:
// The brute force way to implement a Min Stack is to use a standard stack for push, pop, and top
// operations, but find the minimum element dynamically when getMin() is called.
// To find the minimum, we pop all elements from the stack one by one, keeping track of the minimum value
// found so far. We must store the popped elements in a temporary stack and push them back to the
// original stack afterwards to maintain the correct stack order and elements.

// Algorithm:
// 1. push(val): Push val onto the main stack st.
// 2. pop(): Pop the top element from st.
// 3. top(): Return the top element of st.
// 4. getMin():
//    a. Initialize minVal to the top of st.
//    b. Pop all elements from st, updating minVal = min(minVal, element) and pushing them to a temp stack.
//    c. Pop all elements from the temp stack and push them back to st.
//    d. Return minVal.

// Time Complexity:
// - push(): O(1)
// - pop(): O(1)
// - top(): O(1)
// - getMin(): O(N) where N is the number of elements currently in the stack.

// Space Complexity:
// - O(N) auxiliary space to store elements in the temporary stack during getMin().

namespace BruteForce {
class MinStack {
private:
    // A standard stack to store the inserted elements in LIFO order
    stack<int> st;

public:
    // Default constructor to initialize the stack object
    MinStack() {}

    // Pushes the element val onto the stack
    void push(int val) {
        // Add the element directly to the main stack
        st.push(val);
    }

    // Removes the element on the top of the stack
    void pop() {
        // Check if the stack is not empty to avoid popping from an empty stack
        if (!st.empty()) {
            // Remove the top element from the main stack
            st.pop();
        }
    }

    // Gets the top element of the stack
    int top() {
        // Return the top element of the main stack
        return st.top();
    }

    // Retrieves the minimum element in the stack
    int getMin() {
        // Create a temporary stack to hold elements during traversal
        stack<int> temp;
        // Start with the top element as the initial minimum candidate
        int minVal = st.top();
        
        // Traverse the entire stack by popping and transferring elements to temp
        while (!st.empty()) {
            // Update minVal if a smaller element is found
            minVal = min(minVal, st.top());
            // Store the popped element in temp stack to preserve it
            temp.push(st.top());
            // Pop the element from the main stack to proceed to the next
            st.pop();
        }
        
        // Restore all elements back to the main stack to maintain its state
        while (!temp.empty()) {
            // Push the top element of temp back to st
            st.push(temp.top());
            // Remove it from temp stack
            temp.pop();
        }
        
        // Return the computed minimum element
        return minVal;
    }
};
}

// ===============================
// Better Approach
// ===============================

// Approach:
// This is the original implementation. It uses two parallel stacks: st for values, and minSt to
// keep track of the minimum value at each depth of the stack.
// When a value is pushed, we push it onto st. For minSt, we push either the value itself (if empty)
// or the minimum of the value and the current top of minSt. This ensures that the top of minSt
// always holds the minimum element of all elements currently below and including the top of st.

// Algorithm:
// 1. push(val): Push val onto st. If minSt is empty, push val onto minSt; otherwise, push min(val, minSt.top()) onto minSt.
// 2. pop(): Pop from both st and minSt.
// 3. top(): Return the top element of st.
// 4. getMin(): Return the top element of minSt.

// Time Complexity:
// - push(): O(1)
// - pop(): O(1)
// - top(): O(1)
// - getMin(): O(1)

// Space Complexity:
// - O(N) extra space to store the running minimums in the second stack minSt.

namespace Better {
class MinStack {
public:
    // Define two stacks: st to store values, and minSt to track the minimum element at each level
    stack<long long> st, minSt;

    // Default constructor to initialize the MinStack object
    MinStack() {}

    // Pushes the element val onto the stack
    void push(int val) {
        // Push the val into st to maintain the main sequence of elements
        st.push(val);
        // If minSt is empty, the current val is the first element and thus the minimum
        if (minSt.empty())
            // Push val into minSt as the initial minimum element
            minSt.push(val);
        else
            // Compare the incoming value with the current top of minSt and push the smaller one to track the running minimum
            minSt.push(min(val * 1LL, minSt.top()));
    }

    // Removes the element on the top of the stack
    void pop() {
        // Verify that the stack is not empty before attempting to pop to prevent errors
        if (!st.empty()) {
            // Remove the top element from the main stack to keep it updated
            st.pop();
            // Remove the corresponding running minimum from the minSt stack
            minSt.pop();
        }
    }

    // Gets the top element of the stack
    int top() { 
        // Retrieve the top element of st without removing it
        return st.top(); 
    }

    // Retrieves the minimum element in the stack
    int getMin() { 
        // Return the top element of minSt which stores the current minimum element
        return minSt.top(); 
    }
};
}

// ===============================
// Optimal Approach
// ===============================

// Approach:
// To achieve O(1) auxiliary space, we use a single stack and a single variable minVal.
// When we encounter a value smaller than the current minimum, we update minVal to the new value,
// but we push an encoded value onto the stack instead of the value itself.
// The encoded value is 2 * val - minVal, which is mathematically guaranteed to be strictly less
// than the new minVal (since val < minVal_prev).
// This serves as a flag: whenever the top of the stack is less than minVal, we know that:
// 1. The actual value is minVal.
// 2. The previous minimum was 2 * minVal - top_element.
// This allows us to retrieve and restore the previous minimum value upon popping, using only O(1) extra space.

// Algorithm:
// 1. push(val):
//    - If stack is empty: set minVal = val and push val.
//    - If val >= minVal: push val directly.
//    - If val < minVal: push (2 * val - minVal) and set minVal = val.
// 2. pop():
//    - If top of stack < minVal: restore previous minimum minVal = 2 * minVal - st.top() and then st.pop().
//    - Else: st.pop().
// 3. top():
//    - If top of stack < minVal: return minVal.
//    - Else: return st.top().
// 4. getMin():
//    - Return minVal.

// Time Complexity:
// - push(): O(1)
// - pop(): O(1)
// - top(): O(1)
// - getMin(): O(1)

// Space Complexity:
// - O(1) auxiliary space since we only use a single stack and one minVal variable.

namespace Optimal {
class MinStack {
private:
    // A single stack storing encoded values to track the history of previous minimums
    stack<long long> st;
    // Stores the current minimum value in the stack
    long long minVal;

public:
    // Constructor to initialize the stack
    MinStack() {
        // Initialize minVal to 0 as a placeholder (will be set on first push)
        minVal = 0;
    }

    // Pushes the element val onto the stack
    void push(int val) {
        // Convert to long long to prevent integer overflow during calculations
        long long v = val;
        // If the stack is empty, set minVal to the first element and push it directly
        if (st.empty()) {
            minVal = v;
            st.push(v);
        }
        // If the new value is less than the current minimum, we update the minimum
        else if (v < minVal) {
            // Store an encoded value that is strictly less than v (the new minimum) to detect it on pop
            st.push(2 * v - minVal);
            // Update minVal to hold the new minimum value
            minVal = v;
        }
        // If the new value is greater than or equal to current minimum
        else {
            // Push the value directly as it does not affect the current minimum
            st.push(v);
        }
    }

    // Removes the element on the top of the stack
    void pop() {
        // Check if stack is empty to avoid popping from an empty stack
        if (st.empty()) return;
        
        // Get the top element which might be an encoded value
        long long topVal = st.top();
        // Remove the top element from the stack
        st.pop();
        
        // An encoded value is always less than minVal, signifying a minimum transition
        if (topVal < minVal) {
            // Restore the previous minimum value using the inverse of the encoding formula
            minVal = 2 * minVal - topVal;
        }
    }

    // Gets the top element of the stack
    int top() {
        // Return a default value if the stack is empty
        if (st.empty()) return -1;
        
        // Retrieve the top element of the stack
        long long topVal = st.top();
        // If topVal is less than minVal, it means the top element is encoded and equal to minVal
        if (topVal < minVal) {
            // Return minVal as the actual value of the top element
            return minVal;
        }
        // Otherwise, return the value directly
        return topVal;
    }

    // Retrieves the minimum element in the stack
    int getMin() {
        // Return the current minimum value stored in minVal
        return minVal;
    }
};
}