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
        if (!head->next) {
            head->next = prev;
            return head;
        }
        ListNode* n = head->next;
        head->next = prev;
        return reverseUsingRecursion(n, head);
    }
    ListNode* reverseList(ListNode* head) {
        if(!head)return NULL;
        if(!head->next)return head;
        ListNode* dummy = new ListNode(10000);
        ListNode* reverseLL = reverseUsingRecursion(head, dummy);
        ListNode* temp = head;
        while (temp->next->next) {
            temp = temp->next;
        }
        temp->next = NULL;
        return reverseLL;
    }
};