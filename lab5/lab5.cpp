#include "graph.h"
#include "Dialog.h"
#include <stdio.h>
#include <stdlib.h>

#define _CRTDBG_MAP_ALLOC 
#include <crtdbg.h>

const char* MSGS[] = { "0. Quit", "1. Add new vertex", "2. Add new adj", "3. Delete vertex",
                      "4. Delete adj", "5. Print graph", "6. Find DFS", "7. Find skeleton", 
                      "8. Bellman Ford", "9. File write", "10. File read", "11. Graph genetation", 
                      "12. Graphviz" };

const int MSGS_SIZE = sizeof(MSGS) / sizeof(MSGS[0]);

int main() {
    int c = 0;
    int name, name_to, name_from, i, size;
    int weigh;
    Graph* tree;
    Graph* graph = (Graph*)calloc(1, sizeof(Graph));
    
    do {
        c = dialog(MSGS, MSGS_SIZE);
        switch (c) {
        case 0:
            break;

        case 1:
            puts("Enter the name of new vertex: ");
            getInt(&name);

            puts("Enter coordinates of new vertex\n");
            puts("x, y = ");
            int x, y;
            getInt(&x);
            getInt(&y);

            Vertex* new_vertex = CreatVertex(name, x, y);
            AddGraph(graph, new_vertex);

            break;

        case 2:
            puts("Enter the name of vertex 1: ");
            getInt(&name_from);

            puts("Enter the name of vertex 2: ");;
            getInt(&name_to);

            puts("Enter the weigh: ");
            getInt(&weigh);

            int index_from = FindVertex(graph, name_to);
            int index_to = FindVertex(graph, name_from);
            if (index_from == -1 || index_to == -1) {
                puts("Some of the vertices were not found");
                break;
            }

            CreatAdj(&graph->adjList[index_from], &graph->adjList[index_to], weigh);
            
            break;

        case 3:
            puts("Enter the name of vertex: ");
            getInt(&name);

            i = DeleteVertex(graph, name);
            if (i < 0) {
                puts("Error in deleting");
            }
            else {
                puts("Vertex has been deleted");
            }
            
            break;

        case 4:
            puts("Enter the name of vertex 1: ");
            getInt(&name_from);

            puts("Enter the name of vertex 2: ");
            getInt(&name_to);

            i = DeleteAdj(graph, name_to, name_from);
            if (i < 0) {
                puts("Error in deleting");
            }
            else {
                puts("Vertex has been deleted");
            }
            
            break;

        case 5:
            
            D_print_lists(graph);

            break;

        case 6:
            puts("Enter the name of vertex 1: ");
            getInt(&name_from);

            puts("Enter the name of vertex 2: ");
            getInt(&name_to);

            if (D_DFS(graph, name_from, name_to) == -2) {
                puts("Some of the vertices were not found");
            }
            else if (D_DFS(graph, name_from, name_to) == -1) {
                puts("No path found");
            }
            else {
                puts("Path found");
            }
            break;

        case 7:
            
            tree = (Graph*)calloc(1, sizeof(Graph));
            tree = Skeleton_Algorithm_Kruskal(graph);
            
            D_print_lists(tree);

            for (int j = 0; j < tree->num_vertices; j++) {
                if (tree->adjList[j].adj) {
                    freeList(tree->adjList[j].adj);
                }
            }
            free(tree->adjList);
            free(tree);
            break;

        case 8:
            puts("Enter the name of vertex 1: ");
            getInt(&name_from);

            puts("Enter the name of vertex 2: ");
            getInt(&name_to);

            if (D_DFS(graph, name_from, name_to) == -1) {
                puts("Path found");
                break;
            }

            int n = FindVertex(graph, name_to);
            int* pre_list = BellmanFord(graph, name_from, name_to);

            printf("%d <-", name_to);
            while (pre_list[n] != 0) {
                printf(" %d <-", pre_list[n]);
                n = FindVertex(graph, pre_list[n]);
            }
            puts("The way");
            
            free(pre_list);
            break;

        case 9:

            i = output_to_file(graph);

            if (i < 0) {
                puts("First make a graph");
            }
            
            break;

        case 10:
          
            loading(graph);


            break;

        case 11:
            puts("Enter the number of vertexes: ");
            getInt(&size);

            RandomGeneration(graph, size);

            break;

        case 12:

            Graphviz(graph);

            break;

        }

    } while (c != 0);
   
    for (int j = 0; j < graph->num_vertices; j++) {
        if (graph->adjList[j].adj) {
            freeList(graph->adjList[j].adj);
        }
    }
    free(graph->adjList);
    free(graph);

    _CrtDumpMemoryLeaks();

    return 0;
}

//int F_read(char* name, Graph* graph) {
//    FILE* fd = fopen(name, "r");
//    int buf_key;
//    double buf_num;
//    char buf_str[80];
//
//    while (fscanf(fd, "%d %s %lf", &buf_key, buf_str, &buf_num) != EOF) {
//        InfoType* information = (InfoType*)calloc(1, sizeof(InfoType));
//
//        information->num = buf_num;
//        information->string = buf_str;
//        Insert(tree, buf_key, information);
//
//    }
//    print_Tree_as_tree(TreeSearch(tree->root, tree->root->key), 0);
//
//    return 0;
//}

//int Timing() {
//    int n = 10, key[10000], k, cnt = 1000, i, m;
//    clock_t first, last;
//    Tree* tree = (Tree*)calloc(1, sizeof(Tree));
//    srand(time(NULL));
//
//    InfoType* info = (InfoType*)calloc(1, sizeof(InfoType));
//    info->num = 0;
//    info->string = NULL;
//
//    while (n-- > 0) {
//        for (i = 0; i < 10000; ++i)
//            key[i] = rand() * rand();
//        for (i = 0; i < cnt; ) {
//            k = rand() * rand();
//            if (Insert(tree, k, info))
//                ++i;
//        }
//        m = 0;
//        first = clock();
//        for (i = 0; i < 10000; ++i)
//            if (TreeSearch_Time(tree->root, key[i]))
//                ++m;
//        last = clock();
//        printf("%d items was found\n", m);
//        printf("test #%d, number of nodes = %d, time = %d\n", 10 - n, (10 -
//            n) * cnt, last - first);
//    }
//    FreeElem(tree->root);
//    free(tree);
//    free(info);
//
//    return 1;
//}