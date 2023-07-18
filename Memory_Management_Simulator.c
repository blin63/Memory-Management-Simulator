//Program created by: Brendan Lin

//Includes
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

//Error Codes
#define MEMORY_ALLOCATION_ERROR 2
#define EMPTY_LIST_ERROR 3

//Constants
#define SET_COUNTER 1
#define SET_BASE_COUNTER 0
#define FIRST_PROCESS_CHAR 0
#define HOLE 'H'
#define PROCESS 'P'
#define HOLE_PROCESS "H (Hole)"

//Struct
struct Node {
    char *process;
    int base;
    int limit;
    struct Node *next;
};

//Link Definition
typedef struct Node* Link;

//Functions
Link createNode(char *process, int base, int limit) {
    Link link = (Link) malloc(sizeof(struct Node));

    if (!link) {
        perror("Memory Allocation failed\n");
        exit(MEMORY_ALLOCATION_ERROR);
    }

    link->process = process;
    link->base = base;
    link->limit = limit;
    link->next = NULL;
    return link;
}

Link createNodeWithNextNode(char *process, int base, int limit, Link next) {
    Link link = createNode(process, base, limit);
    link->next = next;
    return link;
}

Link getTail(Link head) {
    if (!head) {
        return NULL;
    }

    if (!head->next) {
        return head;
    }

    return getTail(head->next);
}

Link getLastNonHoleProcess(Link head) {
    Link nonHoleProcess = head;

    while (head) {
        if (head->process[FIRST_PROCESS_CHAR] != HOLE) {
            nonHoleProcess = head;
        }

        head = head->next;
    }

    return nonHoleProcess;
}

int pop(Link *head) {
    int limit = SET_COUNTER;

    if (!*head) {
        perror("Empty list was passed to the pop function\n");
        exit(EMPTY_LIST_ERROR);
    }

    if (!(*head)->next) {
        limit = (*head)->limit;
        free((*head));
    } else {
        limit = (*head)->limit;
        Link oldHead = *head;
        *head = (*head)->next;
        free(oldHead);
    }

    return limit;
}

int popCompaction(Link *head, Link *previous) {
    int limit = SET_COUNTER;

    if (!*head) {
        perror("Empty list was passed to the pop function\n");
        exit(EMPTY_LIST_ERROR);
    }

    limit = (*head)->limit;
    Link oldHead = *head;
    *head = (*head)->next;
    free(oldHead);
    (*previous)->next = *head;
    return limit;
}

void addLast(Link *head, char* process, int base, int limit) {
    Link newTail = createNode(process, base, limit);

    if (!*head) {
        *head = newTail;
        return;
    }

    Link currentTail = getTail(*head);
    currentTail->next = newTail;
}

int getLimitTotalAndPopNextHoles(Link *head) {
    if (!(*head) || (*head)->process[FIRST_PROCESS_CHAR] != HOLE) {
        return 0;
    }

    return pop(head) + getLimitTotalAndPopNextHoles(head);
}

void mergeFreeBlocks(Link *head) {
    int limit = SET_COUNTER;

    if (!*head) {
        perror("Empty list passed to mergeFreeBlocks function\n");
        exit(EMPTY_LIST_ERROR);
    }

    while (*head) {
        if ((*head)->process[FIRST_PROCESS_CHAR] == HOLE && (*head)->next
            && (*head)->next->process[FIRST_PROCESS_CHAR] == HOLE) {
            Link firstHole = *head;

            if ((*head)->next->next) {
                head = &(*head)->next;
            } else {
                if (!(*head)->next->next && (*head)->process[FIRST_PROCESS_CHAR] == HOLE &&
                    (*head)->next->process[FIRST_PROCESS_CHAR] == HOLE) {
                    head = &(*head)->next;
                }
            }

            limit = getLimitTotalAndPopNextHoles(head);
            firstHole->limit += limit;

            if (!firstHole->next || firstHole->next->process[FIRST_PROCESS_CHAR] != PROCESS) {
                firstHole->next = NULL;
            }
        }

        if (*head) {
            head = &(*head)->next;
        }
    }
}

void compaction(Link *head) {
    int limit;
    int baseCounter = SET_BASE_COUNTER;
    bool holeExists = false;
    Link lastNonHoleProcess = getLastNonHoleProcess(*head);
    Link front = *head;
    Link previous = NULL;

    if (!*head) {
        perror("Empty list passed to compaction function\n");
        exit(EMPTY_LIST_ERROR);
    }

    //Move all holes to the end of the list and cascade any changes in base to all remaining
    // bases in list up to the first hole at the end of the list
    while (*head != lastNonHoleProcess->next) {
        if ((*head)->next && (*head)->next->process[FIRST_PROCESS_CHAR] == HOLE
            && (*head)->process[FIRST_PROCESS_CHAR] != HOLE) {
            previous = *head;
        }

        if (lastNonHoleProcess->next &&
            lastNonHoleProcess->next->process[FIRST_PROCESS_CHAR] == HOLE) {
            holeExists = true;

            if (!lastNonHoleProcess->next->next && (*head)->process[FIRST_PROCESS_CHAR] == HOLE &&
                    (*head)->next->process[FIRST_PROCESS_CHAR] != HOLE) {
                lastNonHoleProcess->next->next = *head;
            }
        }

        if ((*head)->process[FIRST_PROCESS_CHAR] == HOLE) {
            limit = popCompaction(head, &previous);
            addLast(head, HOLE_PROCESS, (*head)->base, limit);
        } else {
            (*head)->base = baseCounter;
            baseCounter += (*head)->limit;
            *head = (*head)->next;
        }
    }

    if (holeExists) {
        //Update first hole base to new cascaded base
        (*head)->base = baseCounter;

        //Merge all holes moved to the end of the list into one big hole
        mergeFreeBlocks(head);
    }

    *head = front;
}

void printMemory(Link head) {
    int nodeCounter = SET_COUNTER;

    while (head) {
        printf("Node %d: %s, base = %d, limit = %d\n", nodeCounter, head->process,
               head->base, head->limit);
        head = head->next;
        nodeCounter++;
    }
}

void freeMemoryAllocation(Link *head) {
    while (*head) {
        Link oldHead = *head;
        *head = (*head)->next;
        free(oldHead);
    }
}

int main() {
    //Test Cases
    Link node7 = createNode("H (Hole)", 26, 6);
    Link node6 = createNodeWithNextNode("P3", 16, 10, node7);
    Link node5 = createNodeWithNextNode("H (Hole)", 15, 1, node6);
    Link node4 = createNodeWithNextNode("H (Hole)", 11, 4, node5);
    Link node3 = createNodeWithNextNode("H (Hole)", 7, 4, node4);
    Link node2 = createNodeWithNextNode("P17", 6, 1, node3);
    Link node1 = createNodeWithNextNode("P1", 0, 6, node2);
    compaction(&node1);
    printMemory(node1);
    freeMemoryAllocation(&node1);
    return 0;
}