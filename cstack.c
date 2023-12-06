#include "cstack.h"
#include <stddef.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define UNUSED(VAR) (void)(VAR)
#define INIT_SIZE 10

struct node                      // ������� ����� (����)
{
    const struct node* prev;     // ��������� �� ���������� �������
    unsigned int size;           // ������ ������ � ������ � ������ �������� �����
    char data[0];                // ������ ���������� � �������� �����
};
typedef struct node* stack_t;

struct stack_entry                // �������������� ���������� ��� �������� �����
{
    int reserved;                 // ��������� ����, ��� ������ stack_entry ������ ����������� ����������� ����
    stack_t stack;                // ��������� �� ������� ������� �����, �.�. �� ��������� node
};

typedef struct stack_entry stack_entry_t;

struct stack_entries_table        
{
    unsigned int size;            // ���������� ������
    stack_entry_t* entries;       // ���������� ����� � ������ � ������� ���������� �� �������� �����
};                                // ��������� �� ����������� ���������� ������ ��������� stack_entry

struct stack_entries_table g_table = { 0u, NULL };

hstack_t stack_new(void)  
{
    g_table.entries = (stack_entry_t*)realloc(g_table.entries, sizeof(stack_entry_t) * (g_table.size + 1));  // ��������� ������ ��� ������ ��������� stack_entry


    if (g_table.entries == NULL)                         // ���� �� ������� �������� ������ ���������� ������
    {
        return -1;
    }
    else                                                 // ���� ������� �������� ������ ���������� ������
    {
        g_table.entries[g_table.size].stack = NULL;      // �������������� ��������� �� ���� ���� 
        g_table.entries[g_table.size].reserved = 1;      // ��������� ���������, ��� ���� �����������
        int s = g_table.size;
        g_table.size++;                                  // ����������� ������� ������
        return s;                                        // ���������� �������, ����� �����
    }
}

void stack_free(const hstack_t hstack)
{
    if (stack_valid_handler(hstack))
    {
        return;
    }
    g_table.entries[hstack].reserved = 0;
    free(g_table.entries[hstack].stack->data);
    while (g_table.entries[hstack].stack->prev != NULL)
    {
        g_table.entries[hstack].stack = (stack_t)g_table.entries[hstack].stack->prev;
        stack_pop(hstack, g_table.entries[hstack].stack->data, g_table.entries[hstack].stack->size);
    }
    free(g_table.entries[hstack].stack);
}

int stack_valid_handler(const hstack_t hstack)
{
    if (g_table.entries[hstack].reserved = 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

unsigned int stack_size(const hstack_t hstack)
{
    unsigned int size = 0;

    if (g_table.entries[hstack].stack == NULL)
    {
        return 0;
    }

    stack_t p = g_table.entries[hstack].stack;
    
    while (p->prev != NULL)
    {
        p = g_table.entries[hstack].stack->prev;
        g_table.entries[hstack].stack = (stack_t)g_table.entries[hstack].stack->prev;
        stack_pop(hstack, g_table.entries[hstack].stack->data, g_table.entries[hstack].stack->size);
        size++;
    }
    return size + 1;
}

void stack_push(const hstack_t stack, const void* data_in, const unsigned int size)
{
    stack_t ptr = (stack_t)malloc(sizeof(struct node) + size);     // ������� ��������� ��������� �� ���� 
    if (ptr != NULL)
    {
        ptr->data[0] = (char)malloc(size);
        memcpy(ptr->data, data_in, size);
        ptr->prev = g_table.entries[stack].stack;
        ptr->size = size;
        g_table.entries[stack].stack = ptr;                // ��������� ��������� �� ���� � ������� ������� stuck_entry     
    }
    free(ptr);
}

unsigned int stack_pop(const hstack_t hstack, void* data_out, const unsigned int size)
{
    if (stack_valid_handler(hstack))
    {
        return 0;
    }
    memcpy(data_out, g_table.entries[hstack].stack->data, size);

    free(g_table.entries[hstack].stack->data);
    g_table.entries[hstack].stack = (stack_t)g_table.entries[hstack].stack->prev;

    return (sizeof data_out);
}
































/* �� ���� ���
stack_t n = NULL;     //�������� �������� ����
n->size = INIT_SIZE;       //������� ���������� ������� ������ ����
n->prev = NULL;
 //= malloc(sizeof(stack->size) + sizeof(stack->prev) + sizeof(stack->data[0])); // ���������� ���� ������ �������� �����
// �������� ����������� ������, ����� ������ ������������� � �� ����� ���� �� ��������� ����� ������
stack_entry_t stack_entry = { a, n }; // �������� ������(����������) ����
a++;

g_table.size = a;                          // ���������� ������ �� ���� � �������
g_table.entries = &stack_entry;

if (n == NULL)                           // �������� �� ������ ����������
{
    return -1;
}
else
{
    return stack_entry.reserved;            // � ������ ������ ����� �������(����� ���������� �����)
}
*/

/* �� ���� ���
if ((g_table.entries[stack].stack->prev == NULL) && (g_table.entries->reserved == stack))         // ���� � ������ ����� ��� ��� ���������,
{
    ptr->prev = NULL;
    g_table.entries->stack = ptr;                // �� ��������� � ������� ������ ��������� �� ������ ������� �����
}
else if ((g_table.entries->stack->prev != NULL) && (g_table.entries->reserved == stack))          // ���� � ������ ����� ��� ���� ��������
{
    ptr->prev = g_table.entries->stack;
    g_table.entries->stack = ptr
}
*/