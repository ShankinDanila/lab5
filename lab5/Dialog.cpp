#include <stdio.h>
#include <stdlib.h>
#include "graph.h"
#include "Dialog.h"

void D_print_lists(Graph* graph) {
	Adj* ptr = NULL;

	for (int i = 0; i < graph->num_vertices; i++)
	{
		printf("%d(%d|%d) -> ", graph->adjList[i].name, graph->adjList[i].x, graph->adjList[i].y);

		ptr = graph->adjList[i].adj;

		while (ptr != NULL)
		{
			printf("|%d|%d| -> ", ptr->vertex->name, ptr->weight);

			ptr = ptr->next;
		}

		printf("NULL\n");
	}
}

int dialog(const char* msgs[], int n) {
    const char* error = "";
    int choice;
    do {
        puts(error);
        error = "You're wrong. Try again!";
        for (int i = 0; i < n; ++i) {
            puts(msgs[i]);
        }
        printf("Make your choice: ");

        scanf_s("%d", &choice);

    } while ((choice < 0) || (choice > n - 1));
    return choice;
}

int getInt(int* a) {
    int n;
    do {
        n = scanf_s("%d", a);
        if (n == 0) {
            printf("Error");
            scanf_s("%*c", 0);
        }
    } while (n == 0);
    return n < 0 ? 0 : 1;
}

int D_DFS(Graph* graph, int name_from, int name_to) {
    
    int index_from_DFS = FindVertex(graph, name_from);
    int index_to_DFS = FindVertex(graph, name_to);
    if (index_from_DFS == -1 || index_to_DFS == -1) {
        return -2;
        
    }
    int* list = (int*)calloc(graph->num_vertices, sizeof(int));

    DFS(graph, graph->adjList[index_from_DFS], list);

    if (list[index_to_DFS] == 0) {
        free(list);
        return -1;
    }
    else {
        free(list);
        return 0;
    }

}