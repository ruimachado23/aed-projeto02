//
// Algoritmos e Estruturas de Dados --- 2023/2024
//
// Joaquim Madeira, Joao Manuel Rodrigues - June 2021, Nov 2023
//
// Graph - Using a list of adjacency lists representation
//

#include "Graph.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "SortedList.h"
#include "instrumentation.h"

struct _Vertex {
  unsigned int id;
  unsigned int inDegree;
  unsigned int outDegree;
  List* edgesList;
};

struct _Edge {
  unsigned int adjVertex;
  double weight;
};

struct _GraphHeader {
  int isDigraph;
  int isComplete;
  int isWeighted;
  unsigned int numVertices;
  unsigned int numEdges;
  List* verticesList;
};

// The comparator for the VERTICES LIST

int graphVerticesComparator(const void* p1, const void* p2) {
  unsigned int v1 = ((struct _Vertex*)p1)->id;
  unsigned int v2 = ((struct _Vertex*)p2)->id;
  int d = v1 - v2;
  return (d > 0) - (d < 0);
}

// The comparator for the EDGES LISTS

int graphEdgesComparator(const void* p1, const void* p2) {
  unsigned int v1 = ((struct _Edge*)p1)->adjVertex;
  unsigned int v2 = ((struct _Edge*)p2)->adjVertex;
  int d = v1 - v2;
  return (d > 0) - (d < 0);
}

Graph* GraphCreate(unsigned int numVertices, int isDigraph, int isWeighted) {
  Graph* g = (Graph*)malloc(sizeof(struct _GraphHeader));
  if (g == NULL) abort();

  g->isDigraph = isDigraph;
  g->isComplete = 0;
  g->isWeighted = isWeighted;

  g->numVertices = numVertices;
  g->numEdges = 0;

  g->verticesList = ListCreate(graphVerticesComparator);

  for (unsigned int i = 0; i < numVertices; i++) {
    struct _Vertex* v = (struct _Vertex*)malloc(sizeof(struct _Vertex));
    if (v == NULL) abort();

    v->id = i;
    v->inDegree = 0;
    v->outDegree = 0;

    v->edgesList = ListCreate(graphEdgesComparator);

    ListInsert(g->verticesList, v);
  }

  assert(g->numVertices == ListGetSize(g->verticesList));

  return g;
}

Graph* GraphCreateComplete(unsigned int numVertices, int isDigraph) {
  Graph* g = GraphCreate(numVertices, isDigraph, 0);

  g->isComplete = 1;

  List* vertices = g->verticesList;
  ListMoveToHead(vertices);
  unsigned int i = 0;
  for (; i < g->numVertices; ListMoveToNext(vertices), i++) {
    struct _Vertex* v = ListGetCurrentItem(vertices);
    List* edges = v->edgesList;
    for (unsigned int j = 0; j < g->numVertices; j++) {
      if (i == j) {
        continue;
      }
      struct _Edge* new = (struct _Edge*)malloc(sizeof(struct _Edge));
      if (new == NULL) abort();
      new->adjVertex = j;
      new->weight = 1;

      ListInsert(edges, new);
    }
    if (g->isDigraph) {
      v->inDegree = g->numVertices - 1;
      v->outDegree = g->numVertices - 1;
    } else {
      v->outDegree = g->numVertices - 1;
    }
  }
  if (g->isDigraph) {
    g->numEdges = numVertices * (numVertices - 1);
  } else {
    g->numEdges = numVertices * (numVertices - 1) / 2;
  }

  return g;
}

void GraphDestroy(Graph** p) {
  assert(*p != NULL);
  Graph* g = *p;

  List* vertices = g->verticesList;
  if (ListIsEmpty(vertices) == 0) {
    ListMoveToHead(vertices);
    unsigned int i = 0;
    for (; i < g->numVertices; ListMoveToNext(vertices), i++) {
      struct _Vertex* v = ListGetCurrentItem(vertices);

      List* edges = v->edgesList;
      if (ListIsEmpty(edges) == 0) {
        unsigned int i = 0;
        ListMoveToHead(edges);
        for (; i < ListGetSize(edges); ListMoveToNext(edges), i++) {
          struct _Edge* e = ListGetCurrentItem(edges);
          free(e);
        }
      }
      ListDestroy(&(v->edgesList));
      free(v);
    }
  }

  ListDestroy(&(g->verticesList));
  free(g);

  *p = NULL;
}

Graph* GraphCopy(const Graph* g) {
  assert(g != NULL);

  // TO BE COMPLETED !!

  // criar um novo grafo, com as mesmas propriedades básicas
  Graph* newGraph = GraphCreate(g->numVertices, g->isDigraph, g->isWeighted);
  newGraph->numEdges = g->numEdges;
  newGraph->isComplete = g->isComplete;
  
  List* vertices = g->verticesList;     // lista de vértices do grafo original
  ListMoveToHead(vertices);             // colocar o cursor (ponteiro interno) no início da lista

  for (unsigned int i = 0; i < g->numVertices; i++){
    struct _Vertex* originalVertex = (struct _Vertex*)ListGetCurrentItem(vertices);           // obter o vértice atual 
    ListMove(newGraph->verticesList, originalVertex->id);                                     // colocar o cursor no vértice correspondente no novo grafo
    struct _Vertex* newVertex = (struct _Vertex*)ListGetCurrentItem(newGraph->verticesList);  // obter o vértice atual do novo grafo

    // copiar as propriedades do vértice original para o novo vértice
    newVertex->inDegree = originalVertex->inDegree;                      
    newVertex->outDegree = originalVertex->outDegree;    
    
    List* edges = originalVertex->edgesList;              // lista de arestas do vértice original
    ListMoveToHead(edges);                                // colocar o cursor no início da lista

    for (unsigned int j = 0; j < originalVertex->outDegree; j++){
      struct _Edge* originalEdge = (struct _Edge*)ListGetCurrentItem(edges);         // obter a aresta atual

      struct _Edge* newEdge = (struct _Edge*)malloc(sizeof(struct _Edge));           // criar uma nova aresta

      // verificar se a alocação foi bem sucedida
      if (newEdge == NULL) {
        GraphDestroy(&newGraph);
        return NULL;
      }                                                  

      newEdge->weight = originalEdge->weight;                                        // copiar o peso da aresta
      newEdge->adjVertex = originalEdge->adjVertex;                                  // copiar o vértice adjacente

      // adicionar a nova aresta ao novo vértice
      ListInsert(newVertex->edgesList, newEdge);  

      ListMoveToNext(edges);                          // avançar para a próxima aresta                                     
    }
    ListMoveToNext(vertices);                         // avançar para o próximo vértice
    
  }
  return newGraph;
}

Graph* GraphFromFile(FILE* f) {
  assert(f != NULL);

  // TO BE COMPLETED !!

  /*
  Formato do ficheiro:
    1ª linha: 0 ou 1 (não orientado ou orientado)
    2ª linha: 0 ou 1 (arestas sem peso ou com peso)
    3ª linha: número de vértices
    4ª linha: número de arestas
    5ª linha em diante: arestas (vértice de origem, vértice de destino, peso)
  */

  unsigned int numVertices, numEdges;
  int isDigraph, isWeighted;

  // ler e guardar as primeiras 4 linhas do ficheiro
  fscanf(f, "%d", &isDigraph);                
  fscanf(f, "%d", &isWeighted);               
  fscanf(f, "%u", &numVertices);             
  fscanf(f, "%u", &numEdges);                 

  // criar um novo grafo com as propriedades lidas
  Graph* newGraph = GraphCreate(numVertices, isDigraph, isWeighted);

  unsigned int v1, v2;
  double weight;

  // ler as restantes linhas do ficheiro
  for (unsigned int i = 0; i < numEdges; i++) {
    if (isWeighted) {
      fscanf(f, "%u %u %lf", &v1, &v2, &weight);
        GraphAddWeightedEdge(newGraph, v1, v2, weight);     // adicionar aresta com peso ao grafo
      } else {
        fscanf(f, "%u %u", &v1, &v2);
        if (v1 != v2) {
          GraphAddEdge(newGraph, v1, v2);     
        }
      }
    }
  return newGraph;
}

// Graph

int GraphIsDigraph(const Graph* g) { return g->isDigraph; }

int GraphIsComplete(const Graph* g) { return g->isComplete; }

int GraphIsWeighted(const Graph* g) { return g->isWeighted; }

unsigned int GraphGetNumVertices(const Graph* g) { return g->numVertices; }

unsigned int GraphGetNumEdges(const Graph* g) { return g->numEdges; }

//
// For a graph
//
double GraphGetAverageDegree(const Graph* g) {
  assert(g->isDigraph == 0);
  return 2.0 * (double)g->numEdges / (double)g->numVertices;
}

static unsigned int _GetMaxDegree(const Graph* g) {
  List* vertices = g->verticesList;
  if (ListIsEmpty(vertices)) return 0;

  unsigned int maxDegree = 0;
  ListMoveToHead(vertices);
  unsigned int i = 0;
  for (; i < g->numVertices; ListMoveToNext(vertices), i++) {
    struct _Vertex* v = ListGetCurrentItem(vertices);
    if (v->outDegree > maxDegree) {
      maxDegree = v->outDegree;
    }
  }
  return maxDegree;
}

//
// For a graph
//
unsigned int GraphGetMaxDegree(const Graph* g) {
  assert(g->isDigraph == 0);
  return _GetMaxDegree(g);
}

//
// For a digraph
//
unsigned int GraphGetMaxOutDegree(const Graph* g) {
  assert(g->isDigraph == 1);
  return _GetMaxDegree(g);
}

// Vertices

//
// returns an array of size (outDegree + 1)
// element 0, stores the number of adjacent vertices
// and is followed by indices of the adjacent vertices
//
unsigned int* GraphGetAdjacentsTo(const Graph* g, unsigned int v) {
  assert(v < g->numVertices);

  // Node in the list of vertices
  List* vertices = g->verticesList;
  ListMove(vertices, v);
  struct _Vertex* vPointer = ListGetCurrentItem(vertices);
  unsigned int numAdjVertices = vPointer->outDegree;

  unsigned int* adjacent =
      (unsigned int*)calloc(1 + numAdjVertices, sizeof(unsigned int));

  if (numAdjVertices > 0) {
    adjacent[0] = numAdjVertices;
    List* adjList = vPointer->edgesList;
    ListMoveToHead(adjList);
    for (unsigned int i = 0; i < numAdjVertices; ListMoveToNext(adjList), i++) {
      struct _Edge* ePointer = ListGetCurrentItem(adjList);
      adjacent[i + 1] = ePointer->adjVertex;
    }
  }

  return adjacent;
}

//
// returns an array of size (outDegree + 1)
// element 0, stores the number of adjacent vertices
// and is followed by the distances to the adjacent vertices
//
double* GraphGetDistancesToAdjacents(const Graph* g, unsigned int v) {
  assert(v < g->numVertices);

  // Node in the list of vertices
  List* vertices = g->verticesList;
  ListMove(vertices, v);
  struct _Vertex* vPointer = ListGetCurrentItem(vertices);
  unsigned int numAdjVertices = vPointer->outDegree;

  double* distance = (double*)calloc(1 + numAdjVertices, sizeof(double));

  if (numAdjVertices > 0) {
    distance[0] = numAdjVertices;
    List* adjList = vPointer->edgesList;
    ListMoveToHead(adjList);
    for (unsigned int i = 0; i < numAdjVertices; ListMoveToNext(adjList), i++) {
      struct _Edge* ePointer = ListGetCurrentItem(adjList);
      distance[i + 1] = ePointer->weight;
    }
  }

  return distance;
}

//
// For a graph
//
unsigned int GraphGetVertexDegree(Graph* g, unsigned int v) {
  assert(g->isDigraph == 0);
  assert(v < g->numVertices);

  ListMove(g->verticesList, v);
  struct _Vertex* p = ListGetCurrentItem(g->verticesList);

  return p->outDegree;
}

//
// For a digraph
//
unsigned int GraphGetVertexOutDegree(Graph* g, unsigned int v) {
  assert(g->isDigraph == 1);
  assert(v < g->numVertices);

  ListMove(g->verticesList, v);
  struct _Vertex* p = ListGetCurrentItem(g->verticesList);

  return p->outDegree;
}

//
// For a digraph
//
unsigned int GraphGetVertexInDegree(Graph* g, unsigned int v) {
  assert(g->isDigraph == 1);
  assert(v < g->numVertices);

  ListMove(g->verticesList, v);
  struct _Vertex* p = ListGetCurrentItem(g->verticesList);

  return p->inDegree;
}

// Edges

static int _addEdge(Graph* g, unsigned int v, unsigned int w, double weight) {
  struct _Edge* edge = (struct _Edge*)malloc(sizeof(struct _Edge));
  edge->adjVertex = w;
  edge->weight = weight;

  ListMove(g->verticesList, v);
  struct _Vertex* vertex = ListGetCurrentItem(g->verticesList);
  int result = ListInsert(vertex->edgesList, edge);

  if (result == -1) {
    return 0;
  } else {
    g->numEdges++;
    vertex->outDegree++;

    ListMove(g->verticesList, w);
    struct _Vertex* destVertex = ListGetCurrentItem(g->verticesList);
    destVertex->inDegree++;
  }

  if (g->isDigraph == 0) {
    // Bidirectional edge
    struct _Edge* edge = (struct _Edge*)malloc(sizeof(struct _Edge));
    edge->adjVertex = v;
    edge->weight = weight;

    ListMove(g->verticesList, w);
    struct _Vertex* vertex = ListGetCurrentItem(g->verticesList);
    result = ListInsert(vertex->edgesList, edge);

    if (result == -1) {
      return 0;
    } else {
      // g->numEdges++; // Do not count the same edge twice on a undirected
      // graph !!
      vertex->outDegree++;
    }
  }

  return 1;
}

int GraphAddEdge(Graph* g, unsigned int v, unsigned int w) {
  assert(g->isWeighted == 0);
  assert(v != w);
  assert(v < g->numVertices);
  assert(w < g->numVertices);

  return _addEdge(g, v, w, 1.0);
}

int GraphAddWeightedEdge(Graph* g, unsigned int v, unsigned int w,
                         double weight) {
  assert(g->isWeighted == 1);
  assert(v != w);
  assert(v < g->numVertices);
  assert(w < g->numVertices);

  return _addEdge(g, v, w, weight);
}

int GraphRemoveEdge(Graph* g, unsigned int v, unsigned int w) {
  assert(g != NULL);

  // TO BE COMPLETED !!
  
  // vértice v - origem
  // vértice w - destino

  // verificar se v e w existem
  assert(v < g->numVertices);
  assert(w < g->numVertices);

  // verificar se v e w sao diferentes
  assert(v != w);

  ListMove(g->verticesList, v);                                        // colocar o cursor no vértice v
  struct _Vertex* vertex = (struct _Vertex*)ListGetCurrentItem(g->verticesList);        // obter o vértice v (origem)

  List* edges = vertex->edgesList;               // lista de arestas do vértice v
  ListMoveToHead(edges);                         // colocar o cursor no início da lista

  // procurar a aresta que liga v a w
  while(ListIsEmpty(edges) == 0){
    struct _Edge* edge = (struct _Edge*)ListGetCurrentItem(edges);     // obter a aresta atual

    if (edge->adjVertex == w) {
            // aresta encontrada, remover e liberar memória
            free(edge);
            ListRemoveCurrent(edges);
            vertex->outDegree--;
            g->numEdges--;
            ListMove(g->verticesList, w);                                       // colocar o cursor no vértice w
            struct _Vertex* destVertex = (struct _Vertex*)ListGetCurrentItem(g->verticesList);   // obter o vértice w
            destVertex->inDegree--;
            // em grafos nao orientados, temos de remover duas arestas (v -> w e w -> v)
            if (g->isDigraph == 0) {
                // remover a aresta correspondente do vértice de destino
                ListMove(g->verticesList, w);                                       // colocar o cursor no vértice w
                struct _Vertex* destVertex = (struct _Vertex*)ListGetCurrentItem(g->verticesList);   // obter o vértice w 
                List* destEdges = destVertex->edgesList;                            // lista de arestas do vértice w 
                ListMoveToHead(destEdges);                                          // colocar o cursor no início da lista

                // procurar a aresta que liga w a v
                while (ListIsEmpty(destEdges) == 0) {
                    struct _Edge* destEdge = (struct _Edge*)ListGetCurrentItem(destEdges);
                    if (destEdge->adjVertex == v) {
                        free(destEdge);
                        ListRemoveCurrent(destEdges);
                        destVertex->outDegree--;
                        break;
                    }
                    ListMoveToNext(destEdges);
                }
            }

            return 1;       // aresta removida com sucesso
    }
    ListMoveToNext(edges);  // avançar para a próxima aresta
  }
  return 0;                 // aresta não encontrada
}

// CHECKING

int GraphCheckInvariants(const Graph* g) {
  assert(g != NULL);
  // TO BE COMPLETED !!

  /*
    Invariantes possíveis num grafo:
      1. número de vértices/arestas <= 0
      2. número de vértices/arestas inconsistente com a lista de vértices/arestas
      3. validade das listas de adjacências
  */

  // verificar se o número de vértices e arestas é maior que 0
  if (g->numVertices <= 0 || g->numEdges <= 0) {
    return 0;                                               // invariante detetado
  }

  // verificar se o número de vértices é consistente com a lista de vértices
  if (g->numVertices != ListGetSize(g->verticesList)) {
    return 0;                                               // número de vértices inconsistente
  }

  // verificar se o número de arestas é consistente com a lista de arestas
  unsigned int totalEdges = 0;

  // Iterar sobre cada vértice e verificar suas listas de adjacências
  List* vertices = g->verticesList;                                                // lista de vértices do grafo
  ListMoveToHead(vertices);                                                        // colocar o cursor no início da lista
  for (unsigned int i = 0; i < g->numVertices; i++) {
    struct _Vertex* vertex = (struct _Vertex*)ListGetCurrentItem(vertices);        // obter o vértice atual
        
    if (vertex->edgesList == NULL) {
      return 0;  // invariante detetado
    }
        totalEdges += vertex->outDegree;                                           // contar o número total de arestas
        ListMoveToNext(vertices);
    }

    // grafo for não orientado - total de arestas tem de ser dividido por 2
    if (!g->isDigraph) {
        totalEdges /= 2;
    }

    // verificar se o número total de arestas é consistente
    if (g->numEdges != totalEdges) {
        return 0;  // invariante detetado

    }
  return 1;         // todos os invariantes verificados
}

// DISPLAYING on the console

void GraphDisplay(const Graph* g) {
  printf("---\n");
  if (g->isWeighted) {
    printf("Weighted ");
  }
  if (g->isComplete) {
    printf("COMPLETE ");
  }
  if (g->isDigraph) {
    printf("Digraph\n");
    printf("Max Out-Degree = %d\n", GraphGetMaxOutDegree(g));
  } else {
    printf("Graph\n");
    printf("Max Degree = %d\n", GraphGetMaxDegree(g));
  }
  printf("Vertices = %2d | Edges = %2d\n", g->numVertices, g->numEdges);

  List* vertices = g->verticesList;
  ListMoveToHead(vertices);
  unsigned int i = 0;
  for (; i < g->numVertices; ListMoveToNext(vertices), i++) {
    printf("%2d ->", i);
    struct _Vertex* v = ListGetCurrentItem(vertices);
    if (ListIsEmpty(v->edgesList)) {
      printf("\n");
    } else {
      List* edges = v->edgesList;
      unsigned int i = 0;
      ListMoveToHead(edges);
      for (; i < ListGetSize(edges); ListMoveToNext(edges), i++) {
        struct _Edge* e = ListGetCurrentItem(edges);
        if (g->isWeighted) {
          printf("   %2d(%4.2f)", e->adjVertex, e->weight);
        } else {
          printf("   %2d", e->adjVertex);
        }
      }
      printf("\n");
    }
  }
  printf("---\n");
}

void GraphListAdjacents(const Graph* g, unsigned int v) {
  printf("---\n");

  unsigned int* array = GraphGetAdjacentsTo(g, v);

  printf("Vertex %d has %d adjacent vertices -> ", v, array[0]);

  for (unsigned int i = 1; i <= array[0]; i++) {
    printf("%d ", array[i]);
  }

  printf("\n");

  free(array);

  printf("---\n");
}
