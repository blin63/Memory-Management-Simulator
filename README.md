## Memory Management Simulator

**NOTE: This program was written for a past school assignment
while learning how to code in C**

This program simulates how older computer OS' manage the memory
allocated for processes from running programs on the computer.

The simulated memory that needs to managed
is defined in this program as a linked list in the main
function. The linked list in main can be modified to simulate
another variation of processes that need to be managed
by defining if a simulated allocated block of memory
is a hole or a process that needs to be managed.

The linked list should look similiar to below:

```
//Test Cases
Link node7 = createNode("H (Hole)", 26, 6);
Link node6 = createNodeWithNextNode("P3", 16, 10, node7);
Link node5 = createNodeWithNextNode("H (Hole)", 15, 1, node6);
Link node4 = createNodeWithNextNode("H (Hole)", 11, 4, node5);
Link node3 = createNodeWithNextNode("H (Hole)", 7, 4, node4);
Link node2 = createNodeWithNextNode("P17", 6, 1, node3);
Link node1 = createNodeWithNextNode("P1", 0, 6, node2);
```

To create a new node without linking, use 

```createNode(char* process, int base, int limit)```

Where ```char* process``` is the process name or hole in the simulated
memory

Where ```int base``` is the start index of where the simulated memory
begins in the allocated block of memory

Where ```int limit``` is the size of the process that needs to be managed
in the allocated block of memory

The ```CreateNode``` function is typically used for the last
process that's in the allocated block of memory to be managed. For the other
processes that are not the last process in the linked list, use
```createNodeWithNextNode(char* process, int base, int limit, Link next)```

**Please note that the link names must be numbered like in the code
example shown above using the format node#**
