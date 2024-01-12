#include "list.hpp"

void List_Init(List* const list)
{
    list->next = list;
    list->prev = list;
}

static void _List_Add(ListNode* const node, ListNode* const prev, ListNode* const next)
{
    next->prev = node;
    node->next = next;
    prev->next = node;
    node->prev = prev;
}

void List_Add(List* const list, ListNode* const node)
{
    _List_Add(node, list, list->next);
}

void List_AddTail(List* const list, ListNode* const node)
{
    _List_Add(node, list->prev, list);
}

void List_AddBefore(ListNode* const before, ListNode* const node)
{
    _List_Add(node, before->prev, before);
}

void List_AddAfter(ListNode* after, ListNode* node)
{
    _List_Add(node, after, after->next);
}

static void _List_Del(ListNode* const prev, ListNode* const next)
{
    prev->next = next;
    next->prev = prev;
}

void List_DelNode(ListNode* const node)
{
    _List_Del(node->prev, node->next);
    
    node->prev = nullptr;
    node->next = nullptr;
}

void List_Replace(ListNode* const old, ListNode* const node)
{
    node->next = old->next;
    node->next->prev = node;
    node->prev = old->prev;
    node->prev->next = node;
    
    old->prev = nullptr;
    old->next = nullptr;
}

int List_IsLast(const List* list, const ListNode* node)
{
    return IsEqual(list, node->next);
}

int List_IsEmpty(const List* list)
{
    const auto* next {list->next};

    return IsEqual(next, list) && IsEqual(next, list->prev);
}

