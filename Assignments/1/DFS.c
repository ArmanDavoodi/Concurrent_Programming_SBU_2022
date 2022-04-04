#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

const char* const QUERY_PRINT = "print";
const char* const QUERY_SEARCH = "search";
const char* const QUERY_SEARCH_FROM	 = "search_from";
const char* const QUERY_TRAVERSE = "traverse";
const char* const QUERY_TRAVERSE_FROM = "traverse_from";
const char* const QUERY_HELP = "help";
const char* const QUERY_CLEAR = "clear";
const char* const QUERY_EXIT = "exit";

typedef struct Node {
	int value;
	int numOfChildren;
	int* children;
	bool visited;

} Node;

typedef struct IntStack {
	int capacity;
	int size;
	int* stack;

} IntStack;

typedef struct Graph {
	int size;
	Node* nodes;
	IntStack stack;

} Graph;

void allocationError(void*, int, bool);
Graph* getGraph();
Graph* deleteGraph(Graph*);
void printGraph(Graph*);
void printHelp();
void run(Graph*);
void clearScreen();
inline int searchFrom(Graph*, int, int); // DFS search
inline void DFSTraverseFrom(Graph*, int); // DFS Traverse
inline void DFSTraverse(Graph*);

inline void setVisitedsFalse(Graph*);

inline void intStackPush(IntStack*, int);
inline int intStackPop(IntStack*);
inline void clearStack(IntStack*);

int main() {
	Graph* graph = getGraph();
	run(graph);
	graph = deleteGraph(graph);
}

inline void intStackPush(IntStack* stack, int value) {
	if (stack->size < stack->capacity) {
		stack->stack[stack->size] = value;
		++(stack->size);
	}
}

inline int intStackPop(IntStack* stack) {
	if (stack->size > 0)
		return stack->stack[--(stack->size)];
	return INT_MIN;
}

inline void clearStack(IntStack* stack) {
	stack->size = 0;
}

void allocationError(void* ptr, int size, bool noSize0) {
	if (noSize0 && size == 0) {
		printf("\nError: Size can not be 0.\n\n");
		exit(0);
	}
	else if (ptr == NULL) {
		printf("\nError: Memory Alocation failed.\n\n");
		exit(0);
	}
}

Graph* getGraph() {
	Graph* graph = NULL;
	graph = (Graph*)malloc(sizeof(Graph));
	allocationError(graph, 1, true);

	graph->size = -1;
	while (graph->size <= 0) {
		printf("Enter the number of nodes(it should be a positive non_zero integer): ");
		scanf("%d", &(graph->size));
	}

	graph->nodes = NULL;
	graph->nodes = (Node*)malloc(graph->size * sizeof(Node));
	allocationError(graph, graph->size, true);

	graph->stack.stack = (int*)malloc(graph->size * sizeof(int));
	graph->stack.capacity = graph->size;
	graph->stack.size = 0;

	for (int i = 0; i < graph->size; i++) {
		graph->nodes[i].visited = false;

		bool inRange = false;
		while (!inRange) {
			inRange = true;
			printf("Enter the \"value\" and the \"number of children\" for node number %d: ", i);
			scanf("%d%d", &(graph->nodes[i].value), &(graph->nodes[i].numOfChildren));

			if (graph->nodes[i].numOfChildren > graph->size) {
				printf("\nError: The number of children can not be more than the number of nodes in the graph!\n\n");
				inRange = false;
			}
			else if (graph->nodes[i].numOfChildren < 0) {
				printf("\nError: The number of children can not be less than 0!\n\n");
				inRange = false;
			}
		}

		graph->nodes[i].children = NULL;
		if (graph->nodes[i].numOfChildren != 0){
			graph->nodes[i].children = (int*)malloc(graph->nodes[i].numOfChildren * sizeof(int));
			allocationError(graph->nodes[i].children, graph->nodes[i].numOfChildren, false);
		}

		for (int j = 0; j < graph->nodes[i].numOfChildren; j++) {
			bool acceptable = false;
			while (!acceptable) {
				acceptable = true;
				printf("Enter index of child number %d for node number %d: ", j, i);
				scanf("%d", &(graph->nodes[i].children[j]));

				if (graph->nodes[i].children[j] < 0 || graph->nodes[i].children[j] >= graph->size) {
					printf("\nError: Node with index %d does not exists.\n\n", graph->nodes[i].children[j]);
					acceptable = false;
					continue;
				}
				for (int k = 0; k < j; k++) {
					if (graph->nodes[i].children[k] == graph->nodes[i].children[j]) {
						printf("\nError: Node number %d is already a child of node number %d. choose another node.\n\n", graph->nodes[i].children[k], i);
						acceptable = false;
						break;
					}
				}
			}
		}
		printf("__________________________________\n");
	}

	return graph;
}

Graph* deleteGraph(Graph* graph) {
	for (int i = 0; i < graph->size; i++) {
		if (graph->nodes[i].numOfChildren > 0)
			free(graph->nodes[i].children);
		graph->nodes[i].children = NULL;
	}
	free(graph->nodes);
	graph->nodes = NULL;
	free(graph->stack.stack);
	graph->stack.stack = NULL;
	free(graph);

	return NULL;
}

inline void setVisitedsFalse(Graph* graph) {
	for (int i = 0; i < graph->size; i++)
		graph->nodes[i].visited = false;
}

void printGraph(Graph* graph) {
	printf("the graph has %d nodes.\n", graph->size);
	for (int i = 0; i < graph->size; i++) {
		printf("node number %d has value %d and %d children:\nChildren indexes: ", i, graph->nodes[i].value, graph->nodes[i].numOfChildren);
		for (int j = 0; j < graph->nodes[i].numOfChildren; j++) {
			printf("%d ", graph->nodes[i].children[j]);
		}
		printf("-------------------\n");
	}
}

void printHelp() {
	printf("\n_______________HELP_______________\n");
	printf("HELP:\n");

	printf("\t* To print the graph enter: \"%s\" without the quotations.\n", QUERY_PRINT);

	printf("\t* To search the graph for a value from root(node index 0) enter: \"%s <value>\" without the quotations.\n"
		, QUERY_SEARCH);

	printf("\t\t- for example: \"%s 5\".\n", QUERY_SEARCH);

	printf("\t* To search the graph for a value from the node at index \'i\' enter: \"%s <value> <i>\" without the quotations.\n"
		, QUERY_SEARCH_FROM);

	printf("\t\t- for example: \"%s 5 2\".\n", QUERY_SEARCH_FROM);

	printf("\t* To traverse the whole graph starting from root(node index 0) enter: \"%s\" without the quotations.\n"
		, QUERY_TRAVERSE);

	printf("\t* To traverse the subgraph from the node at index \'i\' enter: \"%s <i>\" without the quotations.\n"
		, QUERY_TRAVERSE_FROM);

	printf("\t\t- for example: \"%s 3\".\n", QUERY_TRAVERSE_FROM);

	printf("\t* To print these information again enter: \"%s\" without the quotations.\n", QUERY_HELP);

	printf("\t* To clear the screen enter: \"%s\" without the quotations.\n", QUERY_CLEAR);

	printf("\t* To exit enter: \"%s\" without the quotations.\n", QUERY_EXIT);

	printf("\n+ Note that traverse and search algorithms are DFS implementations.\n");
	printf("\n+ Also Note that to increase the performance of the search algorithm,");
	printf("\n\tall children values are checked before they are pushed to the stack.\n");
}

void run(Graph* graph) {
	char query[100] = "";
	printHelp();

	while (strcmp(query, QUERY_EXIT)) {
		printf("\nEnter any command(to see all possible commands type help): ");

		scanf("%s", query);
		if (!strcmp(query, QUERY_PRINT)) {
			printGraph(graph);
		}
		else if (!strcmp(query, QUERY_SEARCH)) {
			int value;
			scanf("%d", &value);
			int index = searchFrom(graph, value, 0);
			setVisitedsFalse(graph);
			clearStack(&graph->stack);
			if (index == -1)
				printf("value %d was not found in the graph.\n", value);
			else
				printf("node at index %d has value %d.\n", index, value);
		}
		else if (!strcmp(query, QUERY_SEARCH_FROM)) {
			int value, root;
			scanf("%d%d", &value, &root);
			if (root < 0 || root >= graph->size) {
				printf("\nError: Invalid index %d for root.\n\n");
				continue;
			}

			int index = searchFrom(graph, value, root);
			setVisitedsFalse(graph);
			clearStack(&graph->stack);
			if (index == -1)
				printf("value %d was not found in the graph.\n", value);
			else
				printf("node at index %d has value %d.\n", index, value);
		}
		else if (!strcmp(query, QUERY_TRAVERSE)) {
			DFSTraverse(graph);
			setVisitedsFalse(graph);
			clearStack(&graph->stack);
		}
		else if (!strcmp(query, QUERY_TRAVERSE_FROM)) {
			int root;
			scanf("%d", &root);
			if (root < 0 || root >= graph->size) {
				printf("\nError: Invalid index %d for root.\n\n");
				continue;
			}

			DFSTraverseFrom(graph, root);
			setVisitedsFalse(graph);
			clearStack(&graph->stack);
		}
		else if (!strcmp(query, QUERY_CLEAR)) {
			clearScreen();
		}
		else if (!strcmp(query, QUERY_HELP)) {
			printHelp();
		}
		else if (strcmp(query, QUERY_EXIT)) {
			printf("\nError: Command not defined!\n\n");
		}
	}

}

void clearScreen() {
	system("cls");
}

inline int searchFrom(Graph* graph, int value, int root) {
	Node* nodes = graph->nodes;
	register int n = graph->size;
	IntStack stack = graph->stack;

	if (nodes[root].value == value)
		return root;

	register int lastNodeChecked = 0;
	intStackPush(&stack, root);
	nodes[root].visited = true;

	while (stack.size > 0) {
		Node node = nodes[intStackPop(&stack)];

		for (register int i = 0; i < node.numOfChildren; ++i) {
			int childIndex = node.children[i];

			if (!nodes[childIndex].visited) {
				if (nodes[childIndex].value == value)
					return childIndex;
				intStackPush(&stack, childIndex);
				nodes[childIndex].visited = true;
			}
		}

		if (stack.size == 0) {
			for (; lastNodeChecked < n; ++lastNodeChecked) {
				if (!nodes[lastNodeChecked].visited) {
					if (nodes[lastNodeChecked].value == value)
						return lastNodeChecked;
					else {
						intStackPush(&stack, lastNodeChecked);
						nodes[lastNodeChecked].visited = true;
						break;
					}
				}
			}
		}
	}

	return -1;
}

inline void DFSTraverseFrom(Graph* graph, int root) {
	printf("______________Now traversing the subgraph with node at index %d as root______________\n", root);
	Node* nodes = graph->nodes;
	register int n = graph->size;
	IntStack stack = graph->stack;

	intStackPush(&stack, root);
	nodes[root].visited = true;

	while (stack.size > 0) {
		Node node = nodes[intStackPop(&stack)];
		printf("%d ", node.value);

		for (register int i = 0; i < node.numOfChildren; ++i) {
			int childIndex = node.children[i];

			if (!nodes[childIndex].visited) {
				intStackPush(&stack, childIndex);
				nodes[childIndex].visited = true;
			}
		}
	}
	printf("\n____________________________\n");
}

inline void DFSTraverse(Graph* graph) {
	printf("______________Now traversing the whole graph______________\n");

	Node* nodes = graph->nodes;
	register int n = graph->size;
	IntStack stack = graph->stack;

	register int lastNodeChecked = 0;
	intStackPush(&stack, 0);
	nodes[0].visited = true;

	while (stack.size > 0) {
		Node node = nodes[intStackPop(&stack)];
		printf("%d ", node.value);

		for (register int i = 0; i < node.numOfChildren; ++i) {
			int childIndex = node.children[i];

			if (!nodes[childIndex].visited) {
				intStackPush(&stack, childIndex);
				nodes[childIndex].visited = true;
			}
		}

		if (stack.size == 0) {
			for (; lastNodeChecked < n; ++lastNodeChecked) {
				if (!nodes[lastNodeChecked].visited) {
					intStackPush(&stack, lastNodeChecked);
					nodes[lastNodeChecked].visited = true;
					printf("\n-----\n");
					break;
				}
			}
		}
	}
	printf("\n____________________________\n");
}

// 8 3 1 1 1 3 0 2 4 -10 2 2 4 7 1 4 12 1 0 3 1 6 6 2 5 6 1 0