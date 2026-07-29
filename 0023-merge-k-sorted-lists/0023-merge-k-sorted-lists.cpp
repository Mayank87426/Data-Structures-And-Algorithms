// ===============================
// Brute Force Approach
// ===============================

// Approach:
// The brute force approach collects all node pointers from the k sorted lists into a single flat vector. 
// Once all node pointers are collected, we sort them based on their values. Finally, we iterate 
// through the sorted pointers and link them sequentially, ensuring we terminate the list by setting the 
// last node's next pointer to nullptr.

// Algorithm:
// 1. Traverse each linked list in lists, and push every node pointer into a vector `nodes`.
// 2. If the vector is empty, return nullptr.
// 3. Sort the vector `nodes` using a custom lambda function comparing node values.
// 4. Traverse the sorted vector and set `nodes[i]->next = nodes[i+1]` for all indices from 0 to size-2.
// 5. Set `nodes.back()->next = nullptr` to terminate the list.
// 6. Return `nodes[0]`.

// Time Complexity:
// O(N log N), where N is the total number of nodes across all k lists. Collecting the nodes takes O(N), sorting them takes O(N log N), and relinking them takes O(N).

// Space Complexity:
// O(N) auxiliary space to store the node pointers in a vector.

namespace BruteForce {
class Solution {
public:
    ListNode* mergeKLists(vector<ListNode*>& lists) {
        // Collect all non-null node pointers from all lists into a single container
        vector<ListNode*> nodes;
        for (ListNode* list : lists) {
            ListNode* curr = list;
            while (curr) {
                nodes.push_back(curr);
                curr = curr->next;
            }
        }

        // If the collection is empty, there are no lists or all of them are empty
        if (nodes.empty()) {
            return nullptr;
        }

        // Sort all nodes in-place using their values to establish the sorted order
        sort(nodes.begin(), nodes.end(), [](ListNode* a, ListNode* b) {
            return a->val < b->val;
        });

        // Relink the nodes in their new sorted order sequentially
        for (size_t i = 0; i < nodes.size() - 1; ++i) {
            nodes[i]->next = nodes[i + 1];
        }
        
        // Terminate the last node's pointer to avoid cycles in the list
        nodes.back()->next = nullptr;

        // Return the first node which has the smallest value
        return nodes[0];
    }
};
}

// ===============================
// Better Approach
// ===============================

// Approach:
// Instead of collecting all nodes and sorting them, we can merge lists sequentially. 
// We start with the first list as our accumulated list. Then, we iteratively merge the 
// accumulated list with the next list in the array using a standard two-pointer merge function, 
// until all lists are merged.

// Algorithm:
// 1. If lists is empty, return nullptr.
// 2. Initialize a pointer `head` with `lists[0]`.
// 3. For each list `lists[i]` from index 1 to k-1:
//    a. Merge `head` and `lists[i]` using `mergeTwoLists` helper.
//    b. Update `head` with the merged list.
// 4. In `mergeTwoLists`:
//    a. Use a dummy node and a tail pointer.
//    b. Compare the values at the heads of the two lists.
//    c. Attach the smaller node to tail, advance that list's pointer.
//    d. Repeat until one list is exhausted, then append the remaining part of the other list.
// 5. Return `head`.

// Time Complexity:
// O(N * k), where N is the total number of nodes and k is the number of linked lists. In the worst case, we do k merges. The average list length is N/k, making the merge steps grow from N/k to N, resulting in O(N * k) time.

// Space Complexity:
// O(1) auxiliary space, as the merge operation is done in-place by adjusting node pointers without creating any new nodes or using call stack (iterative merge).

namespace Better {
class Solution {
private:
    // Helper method to merge two sorted lists in-place using two pointers
    ListNode* mergeTwoLists(ListNode* l1, ListNode* l2) {
        // Dummy node acts as a placeholder for the head of the merged list
        ListNode dummy;
        // Tail pointer is used to build the list by appending nodes
        ListNode* tail = &dummy;
        
        // Interleave the nodes from both lists in ascending order
        while (l1 && l2) {
            if (l1->val < l2->val) {
                tail->next = l1;
                l1 = l1->next;
            } else {
                tail->next = l2;
                l2 = l2->next;
            }
            tail = tail->next;
        }
        
        // Append any remaining nodes from l1 or l2
        if (l1) {
            tail->next = l1;
        } else {
            tail->next = l2;
        }
        
        // Return the start of the merged list
        return dummy.next;
    }

public:
    ListNode* mergeKLists(vector<ListNode*>& lists) {
        // If there are no lists to merge, return nullptr immediately
        if (lists.empty()) {
            return nullptr;
        }
        
        // Start with the first list as the accumulated result
        ListNode* head = lists[0];
        
        // Progressively merge each of the remaining lists into the result
        for (size_t i = 1; i < lists.size(); ++i) {
            head = mergeTwoLists(head, lists[i]);
        }
        
        return head;
    }
};
}

// ===============================
// Optimal Approach
// ===============================

// Approach:
// The optimal approach uses a Min-Heap (Priority Queue) to always retrieve the node with the smallest 
// value among the active heads of all k lists. We initialize the heap with the first node of each list. 
// At each step, we extract the minimum element from the heap, append it to our merged list, 
// and push the next node of that list into the heap (if it exists). This keeps the heap size 
// capped at k and ensures we always get the overall minimum node in O(log k) time.

// Algorithm:
// 1. Define a min-heap with a custom comparator comparing node values.
// 2. Push the first node of every list in `lists` (if it is not null) into the min-heap.
// 3. Initialize a `dummy` node and a `tail` pointer pointing to `dummy`.
// 4. While the min-heap is not empty:
//    a. Pop the top node `curr` (which is the current smallest node).
//    b. Append `curr` to the merged list (`tail->next = curr`).
//    c. Advance `tail` to `curr`.
//    d. If `curr->next` is not null, push `curr->next` to the min-heap.
// 5. Return `dummy.next`.

// Time Complexity:
// O(N log k), where N is the total number of nodes and k is the number of linked lists. Each node is pushed into and popped from the priority queue of size at most k, which takes O(log k) time per node.

// Space Complexity:
// O(k) auxiliary space for the priority queue storing at most one node from each of the k lists.

namespace Optimal {
class Solution {
public:
    // Custom comparator to order nodes in ascending order in the priority queue
    struct Compare {
        // Greater-than comparison to ensure a min-heap structure based on node values
        bool operator()(ListNode* a, ListNode* b) const {
            return a->val > b->val;
        }
    };

    ListNode* mergeKLists(vector<ListNode*>& lists) {
        // A min-heap to keep track of the smallest node among all the active list heads
        priority_queue<ListNode*, vector<ListNode*>, Compare> pq;

        // Push the head node of each non-empty list into the min-heap to initialize it
        for (ListNode* node : lists) {
            if (node)
                pq.push(node);
        }

        // Dummy node to simplify the linked list construction without checking for head nullity
        ListNode dummy;
        // Tail pointer to construct the merged list sequentially by appending nodes
        ListNode* tail = &dummy;

        // Process nodes until the min-heap is empty
        while (!pq.empty()) {
            // Retrieve the smallest element currently in the min-heap
            ListNode* curr = pq.top();
            // Remove the processed element from the min-heap
            pq.pop();

            // Link the smallest node to our merged list
            tail->next = curr;
            // Advance the tail pointer to the newly appended node
            tail = curr;

            // If the popped node has a next node, push it to maintain the candidate set from all lists
            if (curr->next)
                pq.push(curr->next);
        }

        // Return the head of the merged sorted list, skipping the dummy node
        return dummy.next;
    }
};
}