//
// Algoritmos e Estruturas de Dados --- 2023/2024
//
// Topological Sorting
//

#include "GraphTopologicalSorting.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "Graph.h"
#include "IntegersQueue.h"
#include "instrumentation.h"

#define ITS InstrCount[0]
#define ACESSMEM InstrCount[1]

struct _GraphTopoSort {
  int* marked;                     // Aux array
  unsigned int* numIncomingEdges;  // Aux array
  unsigned int* vertexSequence;    // The result
  int validResult;                 // 0 or 1
  unsigned int numVertices;        // From the graph
  Graph* graph;
};

// AUXILIARY FUNCTION
// Allocate memory for the struct
// And for its array fields
// Initialize all struct fields
//
static GraphTopoSort* _create(Graph* g) {
  assert(g != NULL);

  GraphTopoSort* p = NULL;

  // TO BE COMPLETED
  p = (GraphTopoSort*)malloc(sizeof(GraphTopoSort));
  ACESSMEM += 2;
  if (p == NULL) {
    printf("Error allocating memory for GraphTopoSort struct\n");
    exit(0);
  }

  // initialize struct fields
  unsigned int numVertices = GraphGetNumVertices(g);
  p->validResult = 0; // Not computed yet
  p->numVertices = numVertices;
  p->graph = g;
  ACESSMEM += 3;
  
  // allocate initialize arrays
  p->marked = (int*)calloc(numVertices, sizeof(int));
  ACESSMEM += 2;
  if (p->marked == NULL) {
    printf("Error allocating memory for marked array\n");
    exit(0);
  }

  p->numIncomingEdges = (unsigned int*)malloc(numVertices * sizeof(unsigned int));
  ACESSMEM += 2;
  if (p->numIncomingEdges == NULL) {
    printf("Error allocating memory for numIncomingEdges array\n");
    free(p->marked);
    ACESSMEM++;
    exit(0);
  }

  p->vertexSequence = (unsigned int*)malloc(numVertices * sizeof(unsigned int));
  ACESSMEM += 3;
  if (p->vertexSequence == NULL) {
    printf("Error allocating memory for vertexSequence array\n");
    free(p->marked);
    free(p->numIncomingEdges);
    ACESSMEM += 2;
    exit(0);
  }

  for (unsigned int i = 0; i < numVertices; i++) {
    ITS++;
    p->numIncomingEdges[i] = GraphGetVertexInDegree(g, i);
    p->vertexSequence[i] = 0; // placeholder
    ACESSMEM += 2;
  }

  return p;
}

//
// Computing the topological sorting, if any, using the 1st algorithm:
// 1 - Create a copy of the graph (done)
// 2 - Successively identify vertices without incoming edges and remove their
//     outgoing edges
// Check if a valid sorting was computed and set the isValid field
// For instance, by checking if the number of elements in the vertexSequence is
// the number of graph vertices
//
GraphTopoSort* GraphTopoSortComputeV1(Graph* g) {
  assert(g != NULL && GraphIsDigraph(g) == 1);

  // Create and initialize the struct
  GraphTopoSort* p = _create(g);

  // Build the topological sorting

  // TO BE COMPLETED

  // copia o grafo
  Graph* g_copy = GraphCopy(g);
  if (g_copy == NULL) {
    printf("Error copying graph\n");
    exit(0);
  }

  // contador de vertices processados
  unsigned int processedVertices = 0;

  while (processedVertices < p->numVertices) {
    ITS++;
    int verticeWithNoIncomingEdges = -1;

    // procura um vertice sem arestas incidentes
    ACESSMEM++;
    for (unsigned int i = 0; i < p->numVertices; i++) {
      ITS++;
      //printf("Checking vertex %d whit inDegree: %d\n", i, GraphGetVertexInDegree(g_copy, i)); DEBUG
      ACESSMEM++;
      if (p->marked[i] == 0 && GraphGetVertexInDegree(g_copy, i) == 0) {
        verticeWithNoIncomingEdges = i;
        //printf("%d\n", i); DEBUG
        p->vertexSequence[processedVertices++] = i;
        ACESSMEM++;
        
        // marca o vertice como processado e print do ID
        p->marked[i] = 1;
        ACESSMEM++;

        unsigned int* adjacentsVertex = GraphGetAdjacentsTo(g_copy, i);
        if (adjacentsVertex == NULL) {
          printf("Error getting adjacents vertex\n");
          exit(0);
        }
        // remover o vertice no grafo copia
        for (unsigned int j = 1; j <= adjacentsVertex[0]; j++) {
          ITS++;
          //printf("Removing edge %d -> %d\n", verticeWithNoIncomingEdges, adjacentsVertex[j]); DEBUG
          GraphRemoveEdge(g_copy, verticeWithNoIncomingEdges, adjacentsVertex[j]);
        } 
        free(adjacentsVertex); 
        ACESSMEM++;
        break;   
      }
      
    }
    if (verticeWithNoIncomingEdges == -1) {
      break;
    }
  }
  GraphDestroy(&g_copy);
  ACESSMEM++;
  if (processedVertices != p->numVertices) {
    p->validResult = 0;  
  }
  else {
    p->validResult = 1;
  }
  ACESSMEM++;
  
  return p;
}

//
// Computing the topological sorting, if any, using the 2nd algorithm
// Check if a valid sorting was computed and set the isValid field
// For instance, by checking if the number of elements in the vertexSequence is
// the number of graph vertices
//
GraphTopoSort* GraphTopoSortComputeV2(Graph* g) {
    assert(g != NULL && GraphIsDigraph(g) == 1);

    GraphTopoSort* topoSort = _create(g);

    unsigned int idx = 0; // índice para o array vertexSequence

    while (idx < topoSort->numVertices) {
        ITS++;
        int found = 0;
        unsigned int v;

        // procurar um vértice com in-degree = 0 e não marcado
        ACESSMEM++;
        for (v = 0; v < topoSort->numVertices; v++) {
            ITS++;
            ACESSMEM += 2;
            if (topoSort->numIncomingEdges[v] == 0 && topoSort->marked[v] == 0) {
                found = 1;
                break;
            }
        }

        // se nenhum vértice for encontrado, implica um ciclo ou um erro
        if (!found) {
            topoSort->validResult = 0;
            ACESSMEM++;
            break;
        }

        // adicionar o vértice à ordem topológica
        topoSort->vertexSequence[idx++] = v;
        topoSort->marked[v] = 1;
        ACESSMEM += 2;

        // diminuir o in-degree dos vértices adjacentes
        unsigned int* adjacents = GraphGetAdjacentsTo(g, v);
        for (unsigned int i = 1; i <= adjacents[0]; i++) {
            ITS++;
            unsigned int adjacentVertex = adjacents[i];
            ACESSMEM++;
            if (topoSort->numIncomingEdges[adjacentVertex] > 0) {
                topoSort->numIncomingEdges[adjacentVertex]--;
                ACESSMEM++;
            }
        }
        free(adjacents);
        ACESSMEM++;
    }

    // validar o resultado
    ACESSMEM++;
    if (idx == topoSort->numVertices) {
        topoSort->validResult = 1;
    } else {
        topoSort->validResult = 0;
    }

    return topoSort;
}


//
// Computing the topological sorting, if any, using the 3rd algorithm
// Check if a valid sorting was computed and set the isValid field
// For instance, by checking if the number of elements in the vertexSequence is
// the number of graph vertices
//
GraphTopoSort* GraphTopoSortComputeV3(Graph* g) {
  assert(g != NULL && GraphIsDigraph(g) == 1);

  // Create and initialize the struct

  GraphTopoSort* topoSort = _create(g);

  // Build the topological sorting

  // TO BE COMPLETED
  // inicializar array auxiliar
  unsigned int* numEdgesPerVertex = (unsigned int*)malloc(topoSort->numVertices * sizeof(unsigned int));
  ACESSMEM++;
  if (numEdgesPerVertex == NULL) {
    printf("Error allocating memory for numEdgesPerVertex array\n");
    exit(0);
  }

  for( unsigned int i = 0; i < topoSort->numVertices; i++) {
    ITS++;
    numEdgesPerVertex[i] = GraphGetVertexInDegree(g, i);
    ACESSMEM++;
  }

  // inicializar fila
  Queue* queue = QueueCreate(topoSort->numVertices);
  ACESSMEM++;
  for (unsigned int i = 0; i < topoSort->numVertices; i++) {
    ITS++;
    if (numEdgesPerVertex[i] == 0) {
      QueueEnqueue(queue, i);
    }
  }

  unsigned int processedVertices = 0;

  while (!QueueIsEmpty(queue)) {
    ITS++;
    unsigned int v = QueueDequeue(queue); // retirar próximo vértice da fila
    topoSort->vertexSequence[processedVertices++] = v; // adicionar vértice à sequência topológica
    ACESSMEM++;

    unsigned int* adjacentsVertex = GraphGetAdjacentsTo(g, v);
    ACESSMEM++;
    if (adjacentsVertex == NULL) {
      printf("Error getting adjacents vertex\n");
      exit(0);
    }
    for (unsigned int i = 1; i <= adjacentsVertex[0]; i++) {
      ITS++;
      unsigned int w = adjacentsVertex[i];
      ACESSMEM++;
      numEdgesPerVertex[w]--;
      if (numEdgesPerVertex[w] == 0) {
        QueueEnqueue(queue, w);
        ACESSMEM++;
      }  
    }  
    free(adjacentsVertex);
  }

  ACESSMEM++;
  if (processedVertices != topoSort->numVertices) {
    topoSort->validResult = 0;  
  }
  else {
    topoSort->validResult = 1;
  }

  free(numEdgesPerVertex);
  QueueDestroy(&queue);
  ACESSMEM += 2;

  return topoSort;
}

void GraphTopoSortDestroy(GraphTopoSort** p) {
  assert(*p != NULL);

  GraphTopoSort* aux = *p;

  free(aux->marked);
  free(aux->numIncomingEdges);
  free(aux->vertexSequence);

  free(*p);
  *p = NULL;
  ACESSMEM += 4;
}

//
// A valid sorting was computed?
//
int GraphTopoSortIsValid(const GraphTopoSort* p) { return p->validResult; }

//
// Getting an array containing the topological sequence of vertex indices
// Or NULL, if no sequence could be computed
// MEMORY IS ALLOCATED FOR THE RESULTING ARRAY
//
unsigned int* GraphTopoSortGetSequence(const GraphTopoSort* p) {
  assert(p != NULL);
  // TO BE COMPLETED
  ACESSMEM++;
  if (p->validResult == 1) {
    unsigned int* vertexSequence = (unsigned int*)malloc(p->numVertices * sizeof(unsigned int));
    ACESSMEM++;
    if (vertexSequence == NULL) {
      return NULL;
    }
    ACESSMEM++; 
    for (unsigned int i = 0; i < p->numVertices; i++) {
      ITS++;
      ACESSMEM++;
      vertexSequence[i] = p->vertexSequence[i];
    }
    return vertexSequence;
  }
  return NULL;
}

// DISPLAYING on the console

//
// The toplogical sequence of vertex indices, if it could be computed
//
void GraphTopoSortDisplaySequence(const GraphTopoSort* p) {
  assert(p != NULL);

  ACESSMEM++;
  if (p->validResult == 0) {
    printf(" *** The topological sorting could not be computed!! *** \n");
    return;
  }

  printf("Topological Sorting - Vertex indices:\n");
  for (unsigned int i = 0; i < GraphGetNumVertices(p->graph); i++) {
    ITS++;
    ACESSMEM++;
    printf("%d ", p->vertexSequence[i]);
  }
  printf("\n");
}

//
// The toplogical sequence of vertex indices, if it could be computed
// Followed by the digraph displayed using the adjecency lists
// Adjacency lists are presented in topologic sorted order
//
void GraphTopoSortDisplay(const GraphTopoSort* p) {
  assert(p != NULL);

  // The topological order
  GraphTopoSortDisplaySequence(p);

  ACESSMEM++;
  if (p->validResult == 0) {
    return;
  }

  // The Digraph
  for (unsigned int i = 0; i < GraphGetNumVertices(p->graph); i++) {
    ITS++;
    ACESSMEM++;
    GraphListAdjacents(p->graph, p->vertexSequence[i]);
  }
  printf("\n");
}
