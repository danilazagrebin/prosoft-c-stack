#include "cstack.h"
#include <stddef.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define UNUSED(VAR) (void)(VAR)
#define INIT_SIZE 10

struct node                      // элемент стека (узел)
{
    const struct node* prev;     // указатель на предыдущий элемент
    unsigned int size;           // размер данных в байтах в данном элементе стека
    char data[0];                // данные хранящиеся в элементе стека
};
typedef struct node* stack_t;

struct stack_entry                // дополнительные надстройки для элемента стека
{
    int reserved;                 // индикация того, что данный stack_entry сейчас обслуживает действующий стек
    stack_t stack;                // указатель на верхний элемент стека, т.е. на структуру node
};

typedef struct stack_entry stack_entry_t;

struct stack_entries_table        
{
    unsigned int size;            // количество стеков
    stack_entry_t* entries;       // дескриптор стека и индекс в массиве указателей на верхушки стека
};                                // указатель на динамически выделенный массив элементов stack_entry

struct stack_entries_table g_table = { 0u, NULL };

hstack_t stack_new(void)  
{
    g_table.entries = (stack_entry_t*)realloc(g_table.entries, sizeof(stack_entry_t) * (g_table.size + 1));  // выделение памяти под массив элементов stack_entry


    if (g_table.entries == NULL)                         // если не удалось выделить нужное количество памяти
    {
        return -1;
    }
    else                                                 // если удалось выделить нужное количество памяти
    {
        g_table.entries[g_table.size].stack = NULL;      // инициализируем указатель на стек нулём 
        g_table.entries[g_table.size].reserved = 1;      // добавляем индикацию, что стек действующий
        int s = g_table.size;
        g_table.size++;                                  // увеличиваем счетчик стеков
        return s;                                        // возвращаем хендлер, номер стека
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
    stack_t ptr = (stack_t)malloc(sizeof(struct node) + size);     // создаем локальный указатель на узел 
    if (ptr != NULL)
    {
        ptr->data[0] = (char)malloc(size);
        memcpy(ptr->data, data_in, size);
        ptr->prev = g_table.entries[stack].stack;
        ptr->size = size;
        g_table.entries[stack].stack = ptr;                // добавляем указатель на узел в элемент массива stuck_entry     
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
































/* из стек нью
stack_t n = NULL;     //создание нулевого узла
n->size = INIT_SIZE;       //задание начального размера данных узла
n->prev = NULL;
 //= malloc(sizeof(stack->size) + sizeof(stack->prev) + sizeof(stack->data[0])); // количество всех данных элемента стека
// добавить оптимизацию памяти, чтобы номера освобождались и их можно было бы присвоить новым стекам
stack_entry_t stack_entry = { a, n }; // создание записи(надстройки) узла
a++;

g_table.size = a;                          // добавление записи об узле в таблицу
g_table.entries = &stack_entry;

if (n == NULL)                           // проверка на ошибку выполнения
{
    return -1;
}
else
{
    return stack_entry.reserved;            // в случае успеха вернёт хендлер(номер созданного стека)
}
*/

/* из стек пуш
if ((g_table.entries[stack].stack->prev == NULL) && (g_table.entries->reserved == stack))         // если в нужном стеке еще нет элементов,
{
    ptr->prev = NULL;
    g_table.entries->stack = ptr;                // то добавляем в таблицу стеков указатель на первый элемент стека
}
else if ((g_table.entries->stack->prev != NULL) && (g_table.entries->reserved == stack))          // если в нужном стеке уже есть элементы
{
    ptr->prev = g_table.entries->stack;
    g_table.entries->stack = ptr
}
*/