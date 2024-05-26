#ifndef LIST_H
#define LIST_H

#define OffsetOf(type, member)  ((unsigned)&(((type*)0)->member))

#define ContainerOf(ptr, type, member)                  \
({                                                      \
      const typeof(((type*)0)->member)* __mptr = (ptr); \
      (type*)((char*)__mptr - OffsetOf(type, member));  \
})

#define IsEqual(a, b)      (a == b)

struct ListNode {
    struct ListNode* next;
    struct ListNode* prev;
};

typedef ListNode List;

#define List_ForEach(list, pos)        for(pos=(list)->next; !IsEqual(list, pos); pos=pos->next)
#define List_Node(ptr, type, member)   ContainerOf(ptr, type, member)

void List_Init(List* list);
void List_Add(List* list, ListNode* node);
void List_AddTail(List* list, ListNode* node);
void List_AddBefore(ListNode* before, ListNode* node);
void List_AddAfter(ListNode* after, ListNode* node);
void List_DelNode(ListNode* node);
void List_Replace(ListNode* old, ListNode* node);
int List_IsLast(const List* list, const ListNode* node);
int List_IsEmpty(const List* list);

#endif
