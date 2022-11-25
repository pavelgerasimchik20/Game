#include "functions.h" 
#include <stdlib.h>
#include <stdio.h>
#include <windows.h>

typedef struct SNode
{
    PObject data;
    struct SNode* next;
}* PNode;

PNode node = NULL;

struct Node* newNode(PObject data, struct SNode* nextNode)
{
    node = (PNode)malloc(sizeof(struct SNode));
    node->data = data;
    node->next = nextNode;
    return node;
}

struct SNode* constructList()
{
    struct SNode* head = newNode(1, newNode(2, newNode(3, NULL)));
    return head;
}

void exampleAccessFunction(struct SNode* head)
{
    struct SNode* ptr = head;
    while (ptr)
    {
        //some iteration code with ptr->data
        printf("%d -> ", ptr->data);
        ptr = ptr->next;
    }
    // list is done
    printf("NULL");
}