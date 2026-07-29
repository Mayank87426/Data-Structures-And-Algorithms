// ===============================
// Brute Force Approach
// ===============================

// Approach:
// The brute force approach involves converting the linked list to an array/vector
// of node pointers. Once all node pointers are stored, we can easily reverse the
// nodes in groups of k by swapping pointers within the vector. Finally, we relink
// all nodes sequentially according to their new order in the vector and mark the
// next pointer of the final node as nullptr. This avoids modifying node values
// while simplifying the reversal logic at the cost of O(N) extra space.

// Algorithm:
// 1. Traverse the linked list from head to tail and store each node pointer in a vector.
// 2. Loop through the vector in steps of k.
// 3. For each group of size k, reverse the pointers in the vector using a two-pointer swap.
// 4. Traverse the vector and update each node's next pointer to point to the subsequent node.
// 5. Set the next pointer of the last node in the vector to nullptr.
// 6. Return the first node in the modified vector as the new head.

// Time Complexity:
// O(N) - We traverse the list once to populate the vector, perform swaps on elements
// (reversing chunks takes O(N) in total), and do a final pass to relink nodes.

// Space Complexity:
// O(N) - We store N node pointers in the vector.

namespace BruteForce {
class Solution {
public:
    ListNode* reverseKGroup(ListNode* head, int k) {
        // If the list is empty, contains only one node, or k is 1, no reversal is needed
        if (!head || !head->next || k == 1) {
            return head; // Return the head as no modification is necessary
        }

        // Store all nodes in a vector to allow easy random access by index
        vector<ListNode*> nodes;
        ListNode* curr = head; // Start traversal from the head of the list
        
        // Traverse the entire linked list and store each node pointer
        while (curr) {
            nodes.push_back(curr); // Add the current node pointer to the vector
            curr = curr->next;     // Move to the next node in the list
        }

        int n = nodes.size(); // Total number of nodes in the list
        
        // Loop through the vector in chunks of size k
        for (int i = 0; i + k <= n; i += k) {
            // Reverse the elements within the current group [i, i + k - 1]
            for (int j = 0; j < k / 2; j++) {
                swap(nodes[i + j], nodes[i + k - 1 - j]); // Swap elements to reverse the subsegment
            }
        }

        // Relink the nodes in the vector sequentially to form the new list structure
        for (int i = 0; i < n - 1; i++) {
            nodes[i]->next = nodes[i + 1]; // Set next pointer to the next node in the reordered vector
        }
        nodes[n - 1]->next = nullptr; // Terminate the modified list by setting the last node's next to null

        return nodes[0]; // Return the node that is now at the head of the list
    }
};
}

// ===============================
// Better Approach
// ===============================

// Approach:
// The recursive approach reverses the first k nodes of the linked list and then
// recursively solves the subproblem for the remaining part of the list. By reversing
// only the current group of size k and connecting its tail (which was the head before
// reversal) to the result of the recursive call for the next group, we simplify the
// pointer manipulation. If there are fewer than k nodes left, they are returned as-is
// without reversal.

// Algorithm:
// 1. Traverse the list up to k nodes to check if a group of size k can be formed.
// 2. If the remaining nodes are fewer than k, return the head as-is.
// 3. If a group of size k is available, reverse these k nodes iteratively.
// 4. After reversing, the original head becomes the tail of this group. Recursively
//    call reverseKGroup on the remaining list (the start of the next group).
// 5. Connect the tail of the current reversed group (the original head) to the
//    result of the recursive call.
// 6. Return the new head of the reversed group (the k-th node before reversal).

// Time Complexity:
// O(N) - Every node is traversed a constant number of times (once to count, once to reverse).

// Space Complexity:
// O(N / k) - The recursion stack depth will be proportional to the number of groups,
// which is N / k. In the worst case (k = 1), it takes O(N) space.

namespace Better {
class Solution {
public:
    ListNode* reverseKGroup(ListNode* head, int k) {
        ListNode* curr = head; // Start traversing from the current head
        int count = 0;         // Track the number of nodes in the current group
        
        // Traverse to locate the k-th node to check if a full group exists
        while (curr && count < k) {
            curr = curr->next; // Move to the next node
            count++;           // Increment the count of traversed nodes
        }
        
        // If the number of remaining nodes is less than k, keep them as-is
        if (count < k) {
            return head; // Return the head of this unreversed segment
        }
        
        // Reverse the current group of k nodes in-place
        ListNode* prev = nullptr; // Pointer to keep track of the previous node
        ListNode* next = nullptr; // Pointer to temporarily store the next node
        curr = head;              // Reset curr to the beginning of this group
        
        // Iteratively reverse next pointers for k nodes
        for (int i = 0; i < k; i++) {
            next = curr->next; // Save the reference to the next node
            curr->next = prev; // Reverse the current node's next pointer
            prev = curr;       // Move the previous pointer to the current node
            curr = next;       // Move the current pointer to the next node
        }
        
        // If there are more nodes left, recursively solve the subproblem
        if (next) {
            // Connect the tail of the current reversed group (original head) to the result of the rest of the list
            head->next = reverseKGroup(next, k);
        }
        
        return prev; // Return the new head of this reversed group
    }
};
}

// ===============================
// Optimal Approach
// ===============================

// Approach:
// The optimal approach reverses the nodes in groups of k iteratively and in-place
// using O(1) auxiliary space. We traverse the list to locate the k-th node of the
// current group. If found, we detach this group from the rest of the list, reverse
// it using a standard iterative linked list reversal function, and then link it back
// to the previous reversed segment and the next group. This is repeated until we
// cannot find a complete group of size k.

// Algorithm:
// 1. Define a helper function to retrieve the k-th node from a starting node.
// 2. Define a helper function to reverse a given linked list section in-place.
// 3. Keep track of the current segment head (temp) and the tail of the previous
//    reversed segment (prevNode).
// 4. Find the k-th node from temp. If it does not exist, connect prevNode to temp
//    and terminate.
// 5. If it exists, split the k-group from the rest of the list by storing the next
//    group's head (nextNode) and setting the k-th node's next pointer to nullptr.
// 6. Reverse the k-group.
// 7. Update the head of the entire list if we reversed the first group. Otherwise,
//    connect the previous segment's tail (prevNode) to the new head of the reversed group.
// 8. Update prevNode to temp (which is now the tail of the reversed group) and move
//    temp to nextNode.
// 9. Repeat until all possible groups of size k are reversed.

// Time Complexity:
// O(N) - We traverse each node of the linked list a constant number of times (once
// to find the k-th node and once to reverse).

// Space Complexity:
// O(1) - No extra data structures are used, and the operations are performed iteratively
// without recursion stack overhead.

namespace Optimal {
class Solution {
public:
    // Helper function to find the k-th node from a given starting node
    ListNode* getKthNode(ListNode* head, int k) {
        // Traverse k-1 steps forward to reach the k-th node, if it exists
        while (head && k > 1) {
            head = head->next; // Move to the next node
            k--;               // Decrement the remaining step count
        }
        return head; // Return the k-th node, or nullptr if the list ended early
    }

    // Helper function to reverse a linked list in-place
    void reverseLL(ListNode*& head) {
        ListNode* prev = nullptr; // Track the previous node, initialized to null
        ListNode* curr = head;    // Track the current node being processed

        // Iterate through all nodes to reverse their next pointers
        while (curr) {
            ListNode* nextNode = curr->next; // Store the next node before modifying the pointer
            curr->next = prev;               // Reverse the current node's connection
            prev = curr;                     // Move the previous pointer to the current node
            curr = nextNode;                 // Move the current pointer to the next node
        }

        head = prev; // Update the head reference to point to the new front of the list
    }

    // Main function to reverse the nodes of the list k at a time
    ListNode* reverseKGroup(ListNode* head, int k) {
        ListNode* temp = head;        // Temp pointer to traverse the list and mark group starts
        ListNode* prevNode = nullptr; // Track the tail of the previous reversed group

        // Process the list chunk by chunk
        while (temp) {
            ListNode* kthNode = getKthNode(temp, k); // Retrieve the k-th node of the current group

            // If a complete group of size k cannot be formed
            if (!kthNode) {
                if (prevNode)
                    prevNode->next = temp; // Connect the last reversed group's tail to the remaining unreversed segment
                break; // Exit the loop as no more complete groups can be reversed
            }

            ListNode* nextNode = kthNode->next; // Temporarily store the head of the next group
            kthNode->next = nullptr;            // Terminate the current group's sub-list for reversing

            ListNode* groupStart = temp; // Reference to the start of the current group
            reverseLL(groupStart);       // Reverse the current group's sub-list in-place

            // Update the main head pointer if we reversed the very first group
            if (temp == head)
                head = groupStart; // The new head of the whole list becomes the head of the first reversed group
            else
                prevNode->next = groupStart; // Connect the previous group's tail to the head of the reversed group

            prevNode = temp; // The starting node of the group becomes the tail after reversal
            temp = nextNode; // Move to the start of the next group
        }

        return head; // Return the head of the modified linked list
    }
};
}