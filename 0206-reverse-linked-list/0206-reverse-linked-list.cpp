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
class Solution {
public:
    ListNode* reverseUsingRecursion(ListNode* head, ListNode* prev) {
        if (head == nullptr)
            return prev;

        ListNode* next = head->next;
        head->next = prev;
        return reverseUsingRecursion(next, head);
    }

    ListNode* reverseList(ListNode* head) {
        return reverseUsingRecursion(head, nullptr);
    }
};