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
// Traverse the linked list and push the values of the nodes onto a stack.
// Then, traverse the linked list again and pop the values from the stack to overwrite 
// the node values, effectively reversing the list's values without changing the structure of the list.

// Algorithm:
// 1. Initialize a stack to store the integer values of the nodes.
// 2. Traverse the list from head to the end, pushing each node's value onto the stack.
// 3. Reset the current node pointer to the head of the list.
// 4. Traverse the list again, assigning the top value of the stack to the current node's value and popping it.
// 5. Move to the next node and repeat until the end of the list is reached.
// 6. Return the head of the list.

// Time Complexity:
// O(N) where N is the number of nodes in the linked list. We traverse the list twice.

// Space Complexity:
// O(N) to store the values of the nodes in the stack.

namespace BruteForce {
class Solution {
public:
    ListNode* reverseList(ListNode* head) {
        // Stack to store values of the nodes in LIFO order
        stack<int> values;
        // Pointer to traverse the list, starting from head
        ListNode* curr = head;

        // Traverse the linked list and push each node's value onto the stack
        while (curr != nullptr) {
            // Push the current node's value onto the stack
            values.push(curr->val);
            // Move to the next node in the list
            curr = curr->next;
        }

        // Reset the current pointer to the head of the list for the second pass
        curr = head;

        // Traverse the list again, replacing values with those popped from the stack
        while (curr != nullptr) {
            // Overwrite the current node's value with the top value from the stack
            curr->val = values.top();
            // Pop the used value from the stack
            values.pop();
            // Move to the next node in the list
            curr = curr->next;
        }

        // Return the head of the reversed list
        return head;
    }
};
}

// ===============================
// Better Approach
// ===============================

// Approach:
// We use a recursive method to reverse the linked list. At each step, we update 
// the current node's next pointer to point to the previous node (prev), and then 
// recursively process the next node in the original list.

// Algorithm:
// 1. Define a helper function reverseUsingRecursion that takes the current node (head) and the previous node (prev).
// 2. If head is null, return prev (which will be the new head of the reversed list).
// 3. Save the next node: next = head->next.
// 4. Reverse the pointer direction of the current node: head->next = prev.
// 5. Recursively invoke the helper function with the parameters (next, head).
// 6. The main reverseList function starts the recursion with reverseUsingRecursion(head, nullptr).

// Time Complexity:
// O(N) where N is the number of nodes in the linked list. We visit each node exactly once.

// Space Complexity:
// O(N) due to the call stack of the recursion which can go up to depth N.

namespace Better {
class Solution {
public:
    // Helper function that recursively reverses the linked list pointers
    ListNode* reverseUsingRecursion(ListNode* head, ListNode* prev) {
        // Base case: if head is null, we have reached the end of the original list,
        // and prev points to the new head of the reversed list
        if (head == nullptr)
            return prev;

        // Temporarily store the next node before modifying the current node's pointer
        ListNode* next = head->next;
        // Reverse the direction of the pointer: make current node point back to prev
        head->next = prev;
        // Recursively call for the next node, passing the current node as the new 'prev'
        return reverseUsingRecursion(next, head);
    }

    // Main entry point for reversing the list using recursion
    ListNode* reverseList(ListNode* head) {
        // Call the recursive helper starting with head and prev initialized to nullptr
        return reverseUsingRecursion(head, nullptr);
    }
};
}

// ===============================
// Optimal Approach
// ===============================

// Approach:
// We traverse the linked list iteratively while keeping track of three pointers: 
// prev (initially nullptr), curr (initially head), and next (temporarily stored).
// At each step, we update curr->next to point to prev, then advance prev and curr.

// Algorithm:
// 1. Initialize prev pointer to nullptr and curr pointer to head.
// 2. Loop through the list while curr is not nullptr.
// 3. Save the next node in the original list: next = curr->next.
// 4. Reverse the link: curr->next = prev.
// 5. Move prev to curr.
// 6. Move curr to next.
// 7. After the loop, return prev, which points to the new head of the list.

// Time Complexity:
// O(N) where N is the number of nodes in the linked list. We traverse the list in a single pass.

// Space Complexity:
// O(1) auxiliary space as we only use a few pointer variables.

namespace Optimal {
class Solution {
public:
    ListNode* reverseList(ListNode* head) {
        // Pointer to track the previous node in the reversed list, initialized to nullptr
        ListNode* prev = nullptr;
        // Pointer to track the current node during traversal
        ListNode* curr = head;

        // Iterate through the list until the current pointer reaches the end
        while (curr != nullptr) {
            // Temporarily save the next node before modifying the current pointer
            ListNode* next = curr->next;
            // Reverse the pointer of the current node to point to the previous node
            curr->next = prev;
            // Move the prev pointer one step forward to the current node
            prev = curr;
            // Move the curr pointer one step forward to the next node
            curr = next;
        }

        // Return prev, which now points to the new head of the reversed list
        return prev;
    }
};
}