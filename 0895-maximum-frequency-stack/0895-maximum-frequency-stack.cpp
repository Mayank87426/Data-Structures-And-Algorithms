// ===============================
// Brute Force Approach
// ===============================

// Approach:
// A straightforward simulation of a stack using a dynamic array (std::vector).
// For every pop operation, we scan the entire stack to compute the frequencies of all elements.
// The element with the maximum frequency is identified. If there are multiple elements with the 
// same maximum frequency, we scan the stack from top to bottom (right to left) to locate the one 
// closest to the top of the stack. We then remove it from our vector.

// Algorithm:
// 1. In `push(val)`, append `val` to the end of the `st` vector.
// 2. In `pop()`:
//    a. Traverse `st` to build a frequency map of all elements currently present.
//    b. Find the maximum frequency (`max_freq`) from the frequency map.
//    c. Iterate from the end of the vector to the beginning (from top to bottom).
//    d. The first element whose frequency matches `max_freq` is chosen.
//    e. Erase this element from the vector and return its value.

// Time Complexity:
// - push: O(1)
// - pop: O(N) where N is the number of elements in the stack (due to vector scanning and element erasure).

// Space Complexity:
// - O(N) auxiliary space to store elements in the vector and build the frequency map.

namespace BruteForce {
class FreqStack {
public:
    // Vector to store elements in their insertion order
    vector<int> st;

    FreqStack() {}

    void push(int val) {
        // Append the value to the end of the vector to maintain insertion order
        st.push_back(val);
    }

    int pop() {
        // Hash map to calculate the frequency of each element in the stack
        unordered_map<int, int> freq;
        // Variable to track the maximum frequency found in the stack
        int max_freq = 0;

        // Count the occurrences of all elements currently in the stack
        for (int x : st) {
            freq[x]++;
        }

        // Determine the highest frequency among all elements
        for (auto const& [val, count] : freq) {
            if (count > max_freq) {
                max_freq = count;
            }
        }

        // Scan from right to left (top to bottom of the stack) to find the element to remove
        for (int i = st.size() - 1; i >= 0; i--) {
            // If the element's frequency matches the maximum frequency, it is the target
            if (freq[st[i]] == max_freq) {
                int val = st[i];
                // Erase the element from the vector to update the stack state
                st.erase(st.begin() + i);
                // Return the value of the popped element
                return val;
            }
        }
        // Fallback value if pop is called on empty stack (should not happen per constraints)
        return -1;
    }
};
}

// ===============================
// Better Approach
// ===============================

// Approach:
// This is the original implementation. It optimizes the search for the maximum frequency element 
// by utilizing a max priority queue (max-heap). Since the priority queue needs to resolve ties 
// in favor of the element closest to the top of the stack, we assign a global insertion counter 
// (timestamp) to each pushed element. The priority queue stores elements as tuples/vectors: 
// {frequency, timestamp, value}. Max priority queue compares vectors lexicographically, 
// prioritizing by frequency first, then by timestamp (larger timestamp means more recent/closer to top).

// Algorithm:
// 1. Maintain a frequency map `mp` to keep track of each element's frequency.
// 2. Keep a global integer `count` representing the insertion timestamp.
// 3. In `push(val)`:
//    a. Increment `count`.
//    b. Increment the frequency of `val` in `mp`.
//    c. Push the vector `{mp[val], count, val}` onto the priority queue `pq`.
// 4. In `pop()`:
//    a. Extract the top element `p` from `pq` (which has the highest frequency and is the most recent).
//    b. Remove it from `pq`.
//    c. Decrement the frequency of this element in `mp`.
//    d. Return the value of this element (`p[2]`).

// Time Complexity:
// - push: O(log N) where N is the number of elements in the stack (heap insertion).
// - pop: O(log N) where N is the number of elements in the stack (heap removal).

// Space Complexity:
// - O(N) to store elements in the priority queue and frequency map.

namespace Better {
class FreqStack {
public:
    // Maps each element to its current frequency in the stack
    unordered_map<int, int> mp;
    // Global counter acting as a timestamp to track insertion order and resolve ties
    int count = 0;
    // Max heap to prioritize elements by frequency first, then by insertion order (timestamp)
    priority_queue<vector<int>> pq;

    FreqStack() {}

    void push(int val) {
        // Increment the global counter to assign a unique, increasing timestamp
        count++;
        // Increment the frequency of the pushed value in the frequency map
        mp[val]++;
        // Push the element state to the priority queue. Since vector comparison is lexicographical,
        // it automatically prioritizes by frequency, then by timestamp (closest to top), then by value.
        pq.push({mp[val], count, val});
    }

    int pop() {
        // Retrieve the element at the top of the priority queue, which has the maximum frequency
        // and is closest to the top of the stack in case of ties.
        auto p = pq.top();
        // Remove the top element from the priority queue
        pq.pop();
        // Extract the value of the popped element (index 2 of the stored vector)
        auto v = p[2];
        // Decrement the frequency of this element in the frequency map to keep it updated
        mp[v]--;
        // Return the popped value
        return v;
    }
};
}

// ===============================
// Optimal Approach
// ===============================

// Approach:
// An optimal O(1) solution that groups elements by their frequency. We map each frequency level 
// to a stack of elements that have reached that frequency. This naturally preserves the insertion 
// order (LIFO) for elements with the same frequency, satisfying the tie-breaking rule.
// We also maintain a `maxfreq` variable representing the current maximum frequency of any element 
// in the stack.

// Algorithm:
// 1. Maintain a frequency map `freq` mapping each element to its current frequency.
// 2. Maintain a group map `group` mapping each frequency level to a stack of elements.
// 3. Maintain `maxfreq` to keep track of the maximum frequency level.
// 4. In `push(val)`:
//    a. Increment `freq[val]`.
//    b. If `freq[val]` is greater than `maxfreq`, update `maxfreq = freq[val]`.
//    c. Push `val` to the stack at `group[freq[val]]`.
// 5. In `pop()`:
//    a. Get the top element `val` of the stack at `group[maxfreq]`.
//    b. Pop `val` from `group[maxfreq]`.
//    c. Decrement `freq[val]`.
//    d. If the stack at `group[maxfreq]` becomes empty, decrement `maxfreq`.
//    e. Return `val`.

// Time Complexity:
// - push: O(1) on average (hash map insertions/lookups and stack push operations).
// - pop: O(1) on average (hash map insertions/lookups and stack pop operations).

// Space Complexity:
// - O(N) where N is the number of elements in the stack, to store the frequencies and grouped stacks.

namespace Optimal {
class FreqStack {
public:
    // Maps each element to its current frequency
    unordered_map<int, int> freq;
    // Maps each frequency to a stack of elements that have reached this frequency
    unordered_map<int, stack<int>> group;
    // Tracks the current maximum frequency in the FreqStack
    int maxfreq = 0;

    FreqStack() {}

    void push(int val) {
        // Increment the frequency of the pushed value
        freq[val]++;
        // Update maxfreq if the element's new frequency exceeds the current maxfreq
        if (freq[val] > maxfreq) {
            maxfreq = freq[val];
        }
        // Add the value to the stack corresponding to its new frequency
        group[freq[val]].push(val);
    }

    int pop() {
        // The most frequent element is at the top of the stack associated with maxfreq
        int val = group[maxfreq].top();
        // Remove the element from the stack of the current maximum frequency
        group[maxfreq].pop();
        // Decrement the frequency of the popped element
        freq[val]--;
        // If there are no more elements with maxfreq, decrement maxfreq
        if (group[maxfreq].empty()) {
            maxfreq--;
        }
        // Return the popped element value
        return val;
    }
};
}

/**
 * Your FreqStack object will be instantiated and called as such:
 * FreqStack* obj = new FreqStack();
 * obj->push(val);
 * int param_2 = obj->pop();
 */