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
    ListNode* rotateRight(ListNode* head, int k) {
        ListNode* start = head;
        ListNode* end;
        int size = 1;

        if (head == NULL || k == 0)
            return head;

        while (start->next != NULL) {
            start = start->next;
            size++;
        }

        end = start;
        start = head;
        end->next = head;

        for (int i = 0; i < size - (k % size)-1; i++) {
            start = start->next;
        }

        end = start;
        start = start->next;
        end->next = NULL;

        return start;
    }
};
