#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Node
{
    int data;
    struct Node *next;
};

/* Add a node to linked list */
void addNode(struct Node **head, int value)
{
    struct Node *newNode = malloc(sizeof(struct Node));

    if (newNode == NULL)
    {
        printf("Memory allocation failed!\n");
        return;
    }

    newNode->data = value;
    newNode->next = *head;
    *head = newNode;
}

/* Display linked list */
void displayList(struct Node *head)
{
    printf("Linked List: ");

    while (head != NULL)
    {
        printf("%d -> ", head->data);
        head = head->next;
    }

    printf("NULL\n");
}

/* Free linked list */
void freeList(struct Node *head)
{
    struct Node *temp;

    while (head != NULL)
    {
        temp = head;
        head = head->next;
        free(temp);
    }
}

int main()
{
    int size = 3;

    /* Dynamically allocate array */
    int *array = malloc(size * sizeof(int));

    if (array == NULL)
    {
        printf("Memory allocation failed!\n");
        return 1;
    }

    printf("Enter %d integers:\n", size);

    for (int i = 0; i < size; i++)
    {
        scanf("%d", &array[i]);
    }

    printf("Original array: ");

    for (int i = 0; i < size; i++)
    {
        printf("%d ", array[i]);
    }

    printf("\n");

    /* Resize array */
    int new_size = 5;

    int *temp = realloc(array, new_size * sizeof(int));

    if (temp == NULL)
    {
        printf("Array resizing failed!\n");
        free(array);
        return 1;
    }

    array = temp;

    printf("Enter 2 more integers:\n");

    for (int i = size; i < new_size; i++)
    {
        scanf("%d", &array[i]);
    }

    size = new_size;

    printf("Resized array: ");

    for (int i = 0; i < size; i++)
    {
        printf("%d ", array[i]);
    }

    printf("\n");

    /* Prevent buffer overflow */
    char buffer[20];

    printf("Enter a short string: ");

    scanf("%19s", buffer);

    printf("String entered: %s\n", buffer);

    /* Linked list */
    struct Node *head = NULL;

    addNode(&head, 10);
    addNode(&head, 20);
    addNode(&head, 30);

    displayList(head);

    /* Free linked list */
    freeList(head);

    /* Free dynamically allocated array */
    free(array);

    printf("All allocated memory has been released.\n");

    return 0;
}
