class Solution {
public:
    ListNode* getKthNode(ListNode* head, int k) {
        while (head && k > 1) {
            head = head->next;
            k--;
        }
        return head;
    }

    void reverseLL(ListNode*& head) {
        ListNode* prev = nullptr;
        ListNode* curr = head;

        while (curr) {
            ListNode* nextNode = curr->next;
            curr->next = prev;
            prev = curr;
            curr = nextNode;
        }

        head = prev;
    }

    ListNode* reverseKGroup(ListNode* head, int k) {
        ListNode* temp = head;
        ListNode* prevNode = nullptr;

        while (temp) {
            ListNode* kthNode = getKthNode(temp, k);

            if (!kthNode) {
                if (prevNode)
                    prevNode->next = temp;
                break;
            }

            ListNode* nextNode = kthNode->next;
            kthNode->next = nullptr;

            ListNode* groupStart = temp;
            reverseLL(groupStart);

            if (temp == head)
                head = groupStart;
            else
                prevNode->next = groupStart;

            prevNode = temp; 
            temp = nextNode;
        }

        return head;
    }
};