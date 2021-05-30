#include <stdio.h>
#include <stdlib.h>
#include "graph.h"
#define INF 10000
int last_n;

Vertex* CreatVertex(int Name, int x, int y) {
	Vertex* newVertex = (Vertex*)calloc(1, sizeof(Vertex));
	newVertex->name = Name;
	newVertex->x = x;
	newVertex->y = y;
	newVertex->adj = NULL;

	return newVertex;
}

void CreatAdj(Vertex* vertex_from, Vertex* vertex_to, int weight) {
	if (vertex_from->name == vertex_to->name) {
		//puts("You can't use loops");
		return;
	}
	Adj* newAdj1 = (Adj*)calloc(1, sizeof(Adj));
	newAdj1->vertex = vertex_to;
	newAdj1->started = vertex_from;
	newAdj1->weight = weight;

	if (vertex_from->adj == NULL) {
		vertex_from->adj = newAdj1;
	}
	else {
		Adj* tmp1 = vertex_from->adj;
		vertex_from->adj = newAdj1;
		newAdj1->next = tmp1;
	}

	if (vertex_from->name == vertex_to->name) {
		return;
	}

	Adj* newAdj2 = (Adj*)calloc(1, sizeof(Adj));
	newAdj2->vertex = vertex_from;
	newAdj2->started = vertex_to;
	newAdj2->weight = weight;

	if (vertex_to->adj == NULL) {
		vertex_to->adj = newAdj2;
	}
	else {
		Adj* tmp2 = vertex_to->adj;
		vertex_to->adj = newAdj2;
		newAdj2->next = tmp2;
	}
}

void AddGraph(Graph* graph, Vertex* vertex) {
	graph->adjList = (Vertex*)realloc(graph->adjList, (graph->num_vertices + 1) * sizeof(Vertex));
	graph->adjList[graph->num_vertices] = *vertex;
	graph->num_vertices++;

	free(vertex);
}

int DeleteVertex(Graph* graph, int name) {
	int index = FindVertex(graph, name);
	if (index == -1) {
		return -1; //Вершины не найдено
	}
	Vertex vertex_delete = graph->adjList[index];

	if (vertex_delete.adj == NULL) {
		graph->adjList[index] = graph->adjList[graph->num_vertices - 1];
		graph->adjList = (Vertex*)realloc(graph->adjList, (graph->num_vertices - 1) * sizeof(Vertex));
		graph->num_vertices--;
		return 1; // Удалена изолированная вершина
	}

	Adj* tmp = vertex_delete.adj;
	while (tmp != NULL) {
		int i = FindVertex(graph, tmp->vertex->name);
//		graph->adjList[i].adj = DeleteFromList(graph->adjList[i].adj, name);
		Adj* ptr = tmp->next;
		int error = DeleteAdj(graph, graph->adjList[i].name, name);
		if (error < 0) {
			puts("There are some problems");
			return -1;
		}
		tmp = ptr;
	}
	Vertex* tmp1 = &(graph->adjList[index]);
	graph->adjList[index] = graph->adjList[graph->num_vertices - 1];
	Adj* fix = graph->adjList[index].adj;
	while (fix != NULL) {
		int i = FindVertex(graph, fix->vertex->name);
		Adj* fix1 = graph->adjList[i].adj;
		while (fix1 != NULL) {
			if (fix1->vertex->name == graph->adjList[index].name) {
				fix1->vertex = &graph->adjList[index];
			}
			fix1 = fix1->next;
		}
		fix = fix->next;
	}
	graph->adjList[graph->num_vertices - 1] = *(tmp1);
	graph->adjList = (Vertex*)realloc(graph->adjList, (graph->num_vertices - 1) * sizeof(Vertex));
	graph->num_vertices -= 1;
	return 1; // Вершина и инцидентные ей ребра удалены
}

//Поиск индекса искомой вершины в массиве из списков смежности
int FindVertex(Graph* graph, int name) { 
	for (int i = 0; i < graph->num_vertices; i++) {
		if (graph->adjList[i].name == name) {
			return i;
		}
	}
	return -1;
}

int DeleteAdj(Graph* graph, int name1, int name2) {
	int index1 = FindVertex(graph, name1);
	if (index1 < 0) {
		return -1; // Вершина не найдена
	}
	graph->adjList[index1].adj = DeleteFromList(graph->adjList[index1].adj, name2);

	int index2 = FindVertex(graph, name2);
	if (index2 < 0) {
		return -1; // Вершина не найдена
	}
	graph->adjList[index2].adj = DeleteFromList(graph->adjList[index2].adj, name1);

	return 1;
}

Adj* DeleteFromList(Adj* head, int name) { 
	Adj* ptr = head;

	if (head == NULL) {
		return NULL;
	}
	else if (ptr->vertex->name == name) {
		Adj* tmp = ptr;
		head = tmp->next;
		free(tmp);
		return head;
	}

	while (ptr && ptr->next->vertex->name != name) {
		ptr = ptr->next;
	}
	if (ptr->next->next == NULL) {
		free(ptr->next);
		ptr->next = NULL;
		return head;
	}
	
	Adj* tmp = ptr->next;
	ptr->next = tmp->next;
	free(tmp);

	return head;
} 
 
void DFS(Graph* graph, Vertex from, int* list) {
	int i = FindVertex(graph, from.name);
	list[i] = 1;
	while (from.adj != NULL) {
		int j = FindVertex(graph, from.adj->vertex->name);
		if (list[j] == 0) {
			DFS(graph, *from.adj->vertex, list);
		}
		else {
			from.adj = from.adj->next;
		}
	}
	list[i] = -1;
}

int* BellmanFord(Graph* graph, int name_from, int name_to) {
	int V = graph->num_vertices;
	int* dist = (int*)calloc(V, sizeof(int));

	int index_from = FindVertex(graph, name_from);
	int index_to = FindVertex(graph, name_to);

	int* Pre_List = (int*)calloc(V, sizeof(int));

	if (index_from == -1 || index_to == -1) {
		return NULL;
	}

	for (int i = 0; i < V; i++) {
		dist[i] = INF;
	}
	dist[index_from] = 0;

	Vertex vertex_from = graph->adjList[index_from];
	Vertex vertex_to = graph->adjList[index_to];
	for (int i = 0; i < V - 1; i++) {

		for (int j = 0; j < V; j++) {
		
			Vertex ptr = graph->adjList[j];
			while (ptr.adj != NULL) {
				int iterator1 = FindVertex(graph, ptr.name);
				int iterator2 = FindVertex(graph, ptr.adj->vertex->name);
		
				if (dist[iterator2] > (dist[iterator1] + ptr.adj->weight)) {
					dist[iterator2] = dist[iterator1] + ptr.adj->weight;
		
					Pre_List[iterator2] = graph->adjList[iterator1].name;
				}
				ptr.adj = ptr.adj->next;
			}
		}
	}
	return Pre_List;
}

int NumAdj(Graph* graph) {
	int num = 0;

	for (int i = 0; i < graph->num_vertices; i++) {
		Adj* tmp = graph->adjList[i].adj;
		while (tmp != NULL) {
			//tmp = tmp->adj->next->vertex;
			num++;
			tmp = tmp->next;
		}
	}
	return num / 2;
}

Adj** VectorAdj(Graph* graph) {
	Adj** vectorAdj = (Adj**)calloc(2*NumAdj(graph), sizeof(Adj*));
	int index = 0;

	for (int i = 0; i < graph->num_vertices; i++) {
		Adj* tmp = graph->adjList[i].adj;
		while (tmp != NULL) {
			vectorAdj[index] = tmp;
			index++;
			tmp = tmp->next;
		} 
	}

	Adj* tmp;
	for (int i = 2 * NumAdj(graph); i > 0; i--) {
		for (int j = i; j < i; j++) {
			tmp = vectorAdj[j];
			if (vectorAdj[j]->weight > vectorAdj[j + 1]->weight) {
				vectorAdj[j] = vectorAdj[j + 1];
				vectorAdj[j + 1] = tmp;
			}
		}
	}
	return vectorAdj;
}

Graph* Skeleton_Algorithm_Kruskal(Graph* graph) {
	
	Graph* skeleton = (Graph*)calloc(1, sizeof(Graph));
	skeleton->num_vertices = 0;
	//skeleton->adjList = (Vertex*)calloc(1, sizeof(Vertex));
	int numAdj = 2*NumAdj(graph);
	Adj** PriorityListAdj = VectorAdj(graph);
	int couner = 0;

	int* variety = (int*)calloc(graph->num_vertices, sizeof(int)); //Массив, показывающий принадлежность к множеству
	for (int i = 0; i < graph->num_vertices; i++) {
		variety[i] = i;
	}

	for (int i = 0; i < numAdj; i++) {
		int a = FindVertex(graph, PriorityListAdj[i]->started->name);
		int b = FindVertex(graph, PriorityListAdj[i]->vertex->name);

		if (variety[a] != variety[b]) {
			Vertex* new_vertex1;
			Vertex* new_vertex2;
			if (FindVertex(skeleton, PriorityListAdj[i]->started->name) > 0) {
				new_vertex1 = &(skeleton->adjList[FindVertex(skeleton, PriorityListAdj[i]->started->name)]);
			}
			else {
				new_vertex1 = CreatVertex(PriorityListAdj[i]->started->name, PriorityListAdj[i]->started->x, PriorityListAdj[i]->started->y);
			}

			if (FindVertex(skeleton, PriorityListAdj[i]->vertex->name) > 0) {
				new_vertex2 = &(skeleton->adjList[FindVertex(skeleton, PriorityListAdj[i]->vertex->name)]);
			}
			else {
				new_vertex2 = CreatVertex(PriorityListAdj[i]->vertex->name, PriorityListAdj[i]->vertex->x, PriorityListAdj[i]->vertex->y);
			}
			
			if ((FindVertex(skeleton, new_vertex2->name) > 0) && (FindVertex(skeleton, new_vertex1->name) > 0)) {
				int l = FindVertex(skeleton, new_vertex1->name);
				int k = FindVertex(skeleton, new_vertex2->name);
				CreatAdj(&skeleton->adjList[l], &skeleton->adjList[k], PriorityListAdj[i]->weight);
			}
			else if ((FindVertex(skeleton, new_vertex2->name) < 0) && (FindVertex(skeleton, new_vertex1->name) < 0)) {
				AddGraph(skeleton, new_vertex2);
				AddGraph(skeleton, new_vertex1);
				int l = FindVertex(skeleton, PriorityListAdj[i]->started->name);
				int k = FindVertex(skeleton, PriorityListAdj[i]->vertex->name);
				CreatAdj(&skeleton->adjList[l], &skeleton->adjList[k], PriorityListAdj[i]->weight);
			}
			else if ((FindVertex(skeleton, new_vertex2->name) < 0) && (FindVertex(skeleton, new_vertex1->name) > 0)) {
				AddGraph(skeleton, new_vertex2);
				int l = FindVertex(skeleton, PriorityListAdj[i]->started->name);
				int k = FindVertex(skeleton, PriorityListAdj[i]->vertex->name);
				CreatAdj(&skeleton->adjList[l], &skeleton->adjList[k], PriorityListAdj[i]->weight);
			}
			else if ((FindVertex(skeleton, new_vertex1->name) < 0) && (FindVertex(skeleton, new_vertex2->name) > 0)) {
				AddGraph(skeleton, new_vertex1);
				int l = FindVertex(skeleton, PriorityListAdj[i]->started->name);
				int k = FindVertex(skeleton, PriorityListAdj[i]->vertex->name);
				CreatAdj(&skeleton->adjList[l], &skeleton->adjList[k], PriorityListAdj[i]->weight);
			}

			int old_variety = variety[b], new_variety = variety[a];
			for (int j = 0; j < graph->num_vertices; j++) {
				if (variety[j] == old_variety) {
					variety[j] = new_variety;
				}
			}
		}
	}
	free(PriorityListAdj);
	free(variety);
	return skeleton;
}

void freeList(Adj* head) {
	Adj* tmp = head;
	while (tmp != NULL) {
		tmp = tmp->next;
		free(head);
		head = tmp;
	}
}

int loading(Graph* gr) {
	FILE* input;
	int index, name1, name2, weigh, coin, i = 0;
	int x, y;
	input = fopen("input_5lab.txt", "r");
	if (input == NULL)
		return -1;
	else {
		fscanf(input, "%d\n", &coin);
		while (i < coin) {
			fscanf(input, "%d\n", &index);
			fscanf(input, "%d\n", &x);
			fscanf(input, "%d\n", &y);
			Vertex* new_vertex = CreatVertex(index, x, y);
			AddGraph(gr, new_vertex);
			i += 1;
		}
		while (!feof(input)) {
			fscanf(input, "%d\n", &name1);
			fscanf(input, "%d\n", &name2);
			fscanf(input, "%d\n", &weigh);

			int index_from = FindVertex(gr, name1);

			int index_to = FindVertex(gr, name2);

			CreatAdj(&gr->adjList[index_from], &gr->adjList[index_to], weigh);
		}
	}
	fclose(input);
	return 0;
}

int output_to_file(Graph* graph) {
	FILE* output;
	Graph* ptr = graph;

	output = fopen("output_5lab.txt", "w");
	if (output == NULL) {
		return -1;
	}
	else if (graph != NULL) {
		fprintf(output, "%d\n", graph->num_vertices);
		for (int i = 0; i < graph->num_vertices; i++) {
			fprintf(output, "%d %d %d\n", ptr->adjList[i].name, ptr->adjList[i].x, ptr->adjList[i].y);
		}
		for (int i = 0; i < graph->num_vertices; i++) {
			Adj* tmp = graph->adjList[i].adj;
			while (tmp != NULL) {
				if (FindVertex(graph, tmp->vertex->name) > i) {
					fprintf(output, "%d %d %d\n", tmp->started->name, tmp->vertex->name, tmp->weight);
				}
				tmp = tmp->next;
			}
		}
		fclose(output);
		return 0;
	}
}

void Graphviz(Graph* graph) {
	puts("graph foo {");
	for (int i = 0; i < graph->num_vertices; i++) {
		printf("\"%d\";\n", graph->adjList[i]);
	}

	for (int i = 0; i < graph->num_vertices; i++) {
		Adj* tmp = graph->adjList[i].adj;
		while (tmp != NULL) {

			if (FindVertex(graph, tmp->vertex->name) > i) {
				printf("\"%d\" -- \"%d\" [ label=\" %d\"];\n", tmp->started->name, tmp->vertex->name, tmp->weight);
			}
			tmp = tmp->next;
		}
	}
	puts("}");
}

void RandomGeneration(Graph* graph, int size) {

	for (int i = 0; i < size; i++) {
		Vertex* new_vertex = CreatVertex(i, i, i);
		AddGraph(graph, new_vertex);
	}

	srand(time(NULL));


	for (int i = 0; i < rand() % size*2; i++) {
		CreatAdj(&graph->adjList[FindVertex(graph, rand() % size)], &graph->adjList[FindVertex(graph, rand() % size)], rand() % 15);
	}

	D_print_lists(graph);

	return;
}