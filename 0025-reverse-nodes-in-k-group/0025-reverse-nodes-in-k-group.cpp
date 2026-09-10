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
    ListNode* reverseLL(ListNode* head) {
        ListNode* prev = nullptr;
        ListNode* temp = head;

        while (temp != nullptr) {
            ListNode* next = temp->next;
            temp->next = prev;
            prev = temp;
            temp = next;
        }

        return prev;
    }
    ListNode* getKthNode(ListNode* head, int k) {
        auto temp = head;
        while (temp && k > 1) {
            temp = temp->next;
            k--;
        }
        return temp;
    }
    ListNode* reverseKGroup(ListNode* head, int k) {
        if (!head)
            return nullptr;
        if (!getKthNode(head, k))
            return head;
        auto kthNode = getKthNode(head, k);
        auto n = kthNode->next;
        kthNode->next = NULL;
        auto rev = reverseLL(head);
        auto t = rev;
        while (t->next) {
            t = t->next;
        }
        t->next = reverseKGroup(n, k);
        return rev;
    }
};