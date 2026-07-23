/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     ListNode *next;
 *     ListNode() : val(0), next(nullptr) {}
 *     ListNode(int x) : val(x), next(nullptr) {}
 *     ListNode(int x, ListNode *next) : val(next) {}
 * };
 */
class Solution {
public:
    // Reverse exactly k nodes
    ListNode* reverseLL(ListNode* head, int k, ListNode* prev) {
        if (k == 0)
            return prev;

        ListNode* next = head->next;
        head->next = prev;
        return reverseLL(next, k - 1, head);
    }

    ListNode* reverseKGroup(ListNode* head, int k) {
        // Check whether there are at least k nodes
        ListNode* temp = head;
        for (int i = 0; i < k; i++) {
            if (temp == NULL)
                return head;
            temp = temp->next;
        }

        // temp now points to the node after the k-group
        ListNode* newHead = reverseLL(head, k, temp);

        // head becomes the tail after reversal
        head->next = reverseKGroup(temp, k);

        return newHead;
    }
};