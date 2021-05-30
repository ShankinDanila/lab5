typedef struct Adj {
	struct Vertex* vertex;
	int weight;

	struct Adj* next;
	struct Vertex* started;
} Adj;

typedef struct Vertex {
	int name;
	int x;
	int y;

	struct Adj* adj;
} Vertex;

typedef struct Graph {
	int num_vertices;

	Vertex* adjList;
} Graph;

Vertex* CreatVertex(int Name, int x, int y);
void CreatAdj(Vertex* vertex_from, Vertex* vertex_to, int weight);
void AddGraph(Graph* grapf, Vertex* vertex);
int DeleteVertex(Graph* graph, int name);
int FindVertex(Graph* graph, int name);
int DeleteAdj(Graph* graph, int name1, int name2);
Adj* DeleteFromList(Adj* head, int name);
void DFS(Graph* graph, Vertex from, int* list);
int* BellmanFord(Graph* graph, int name_from, int name_to);
int NumAdj(Graph* graph);
Adj** VectorAdj(Graph* graph);
Graph* Skeleton_Algorithm_Kruskal(Graph* graph);
void freeList(Adj* head);
int loading(Graph* gr);
void Graphviz(Graph* graph);
void RandomGeneration(Graph* graph, int size);
int output_to_file(Graph* graph);