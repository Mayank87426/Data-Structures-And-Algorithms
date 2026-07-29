/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     ListNode *next;
 *     ListNode() : val(0), next(nullptr) {}
 *     ListNode(int x) : val(x), next(nullptr) {}
 *     ListNode(int x, ListNode *next) : val(x), next(next) {}
 * };
 */

// ===============================
// Brute Force Approach
// ===============================

// Approach:
// Store all nodes in an array/vector to gain random access.
// This allows us to easily find the node to be removed by index.
// The node to be removed is at index (L - n), where L is the total size of the list.

// Algorithm:
// 1. Traverse the linked list and push all node pointers into a vector.
// 2. If the size of the vector is equal to n, it means the head itself needs to be removed.
//    In this case, return head->next.
// 3. Otherwise, the predecessor node is at index (L - n - 1). Update its next pointer
//    to skip the node at index (L - n).
// 4. Return the head of the list.

// Time Complexity:
// O(N) where N is the number of nodes in the list. We traverse the list once.

// Space Complexity:
// O(N) to store the pointers of the nodes in the vector.

namespace BruteForce {
class Solution {
public:
    ListNode* removeNthFromEnd(ListNode* head, int n) {
        // Vector to store pointers to all list nodes for random access
        vector<ListNode*> nodes;
        ListNode* curr = head;
        
        // Traverse the list and populate the vector
        while (curr != nullptr) {
            nodes.push_back(curr);
            curr = curr->next;
        }
        
        int length = nodes.size();
        
        // If the node to remove is the head (first node)
        if (length == n) {
            return head->next;
        }
        
        // Get the node before the one to be removed
        ListNode* prev = nodes[length - n - 1];
        // Skip the target node by pointing to the node after it
        prev->next = prev->next->next;
        
        return head;
    }
};
}

// ===============================
// Better Approach
// ===============================

// Approach:
// We traverse the linked list to find its total length L.
// Then, we can find the node to be removed, which is at position (L - n + 1) from the beginning.
// We traverse the list a second time to reach the (L - n)-th node, which is the predecessor
// of the node to be deleted, and update its next pointer.

// Algorithm:
// 1. Traverse the linked list starting from head to count the total number of nodes, L.
// 2. If L is equal to n, it means we need to remove the first node (head). Return head->next.
// 3. Otherwise, traverse the list again up to the (L - n)-th node.
// 4. Update the next pointer of the (L - n)-th node to skip the next node (i.e., node->next = node->next->next).
// 5. Return the original head of the list.

// Time Complexity:
// O(N) where N is the number of nodes in the linked list. We make at most two passes over the list.

// Space Complexity:
// O(1) as we only use a few pointers for iteration.

namespace Better {
class Solution {
public:
    ListNode* removeNthFromEnd(ListNode* head, int n) {
        int length = 0;
        ListNode* curr = head;
        
        // First pass: count the total number of nodes in the list
        while (curr != nullptr) {
            length++;
            curr = curr->next;
        }
        
        // If we need to remove the head node (first node from start)
        if (length == n) {
            return head->next;
        }
        
        // Reset curr to head for the second pass
        curr = head;
        
        // Traverse to the node right before the one we want to remove
        for (int i = 1; i < length - n; i++) {
            curr = curr->next;
        }
        
        // Skip the Nth node from the end
        curr->next = curr->next->next;
        
        return head;
    }
};
}

// ===============================
// Optimal Approach
// ===============================

// Approach:
// We use a two-pointer technique (fast and slow) to remove the Nth node from the end in a single pass.
// By advancing the fast pointer n steps ahead, we establish a gap of n nodes between slow and fast.
// Then, we move both pointers together until fast reaches the last node. At this point,
// slow will point to the node right before the one to be removed.

// Algorithm:
// 1. Check if the list has only one node; if so, removing it leaves the list empty, so return NULL.
// 2. Initialize fast and slow pointers to the head.
// 3. Advance the fast pointer by n steps to create a gap of n nodes.
// 4. If fast becomes NULL, it means n equals the length of the list, so the head itself is to be removed. Return head->next.
// 5. Move both fast and slow pointers one step at a time until fast->next is NULL.
// 6. Output slow->val to print the value of the predecessor node.
// 7. Update slow->next to slow->next->next, skipping the target node.
// 8. Return head.

// Time Complexity:
// O(N) where N is the number of nodes in the linked list. We traverse the list in a single pass.

// Space Complexity:
// O(1) since we only use two pointers.

namespace Optimal {
class Solution {
public:
    ListNode* removeNthFromEnd(ListNode* head, int n) {
        // Check if the list contains only a single node
        if (head->next == NULL)
            // If there's only one node, removing it results in an empty list
            return NULL;

        // Initialize fast pointer to the head of the list
        ListNode* fast = head;
        // Initialize slow pointer to the head of the list
        ListNode* slow = head;
        // Move the fast pointer forward by n nodes to create a gap of n
        while (n--) {
            fast = fast->next;
        }
        // Check if fast has become NULL after moving n steps
        if(!fast){
            // If fast is NULL, the list length is exactly n, so the head node must be removed
            return head->next;
        }

        // Loop until the fast pointer reaches the last node of the list
        while (fast->next != NULL) {
            // Move fast forward to progress towards the end of the list
            fast = fast->next;
            // Move slow forward to keep it exactly n steps behind fast
            slow = slow->next;
        }
        // Print the value of the node right before the one to be deleted
        cout<<slow->val;
        // Bypass the target node by setting slow's next pointer to point to the node after it
        slow->next = slow->next->next;
        // Return the head of the modified list
        return head;
    }
};
}