#include "graph.h"
#include <string.h>

#define INITIAL_NODE_CAPACITY 100

graph_t* createGraph() {
    graph_t* g = malloc(sizeof(graph_t));
    if (!g) return NULL;
    g->nodeSpace = INITIAL_NODE_CAPACITY;
    g->nodeCount = 0;
    g->edgeCount = 0;
    g->nodes = calloc(g->nodeSpace, sizeof(node_t*));
    if (!g->nodes) {
        free(g);
        return NULL;
    }
    return g;
}

void freeGraph(graph_t* graph) {
    if (!graph) return;

    for (int i = 0; i < graph->nodeCount; i++) {
        node_t* node = graph->nodes[i];
        if (!node) continue;

        edge_t* e = node->edges;
        while (e) {
            edge_t* next = e->next;
            free(e);
            e = next;
        }
        free(node->data);
        free(node);
    }
    free(graph->nodes);
    free(graph);
}

node_t* getNode(graph_t* graph, int id) {
    for (int i = 0; i < graph->nodeCount; i++) {
        if (graph->nodes[i]->id == id)
            return graph->nodes[i];
    }
    return NULL;
}

// Add node
node_t* addNode(graph_t* graph, int id, void* data) {
    if (getNode(graph, id)) return NULL;

    if (graph->nodeCount == graph->nodeSpace) {
        graph->nodeSpace *= 2;
        node_t** newArr = realloc(graph->nodes, graph->nodeSpace * sizeof(node_t*));
        if (!newArr) return NULL;
        graph->nodes = newArr;
    }

    node_t* n = malloc(sizeof(node_t));
    if (!n) return NULL;
    n->id = id;
    n->data = data;
    n->edges = NULL;

    graph->nodes[graph->nodeCount++] = n;
    return n;
}

edge_t* addEdge(graph_t* graph, int fromId, int toId, float weight, void* data) {
    node_t* fromNode = getNode(graph, fromId);
    node_t* toNode = getNode(graph, toId);
    if (!fromNode || !toNode) return NULL;

    edge_t* curr = fromNode->edges;
    while (curr) {
        if (curr->toNode->id == toId)
            return NULL;
        curr = curr->next;
    }

    edge_t* e = malloc(sizeof(edge_t));
    if (!e) return NULL;
    e->toNode = toNode;
    e->weight = weight;
    e->data = data;
    e->next = fromNode->edges;
    fromNode->edges = e;

    graph->edgeCount++;
    return e;
}

edge_t* getEdge(graph_t* graph, int fromId, int toId) {
    node_t* fromNode = getNode(graph, fromId);
    if (!fromNode) return NULL;

    edge_t* e = fromNode->edges;
    while (e) {
        if (e->toNode->id == toId)
            return e;
        e = e->next;
    }
    return NULL;
}

int removeEdge(graph_t* graph, int fromId, int toId) {
    node_t* fromNode = getNode(graph, fromId);
    if (!fromNode) return 0;

    edge_t* prev = NULL;
    edge_t* curr = fromNode->edges;

    while (curr) {
        if (curr->toNode->id == toId) {
            if (prev) prev->next = curr->next;
            else fromNode->edges = curr->next;
            free(curr);
            graph->edgeCount--;
            return 1;
        }
        prev = curr;
        curr = curr->next;
    }
    return 0;
}

int removeNode(graph_t* graph, int id) {
    int index = -1;
    for (int i = 0; i < graph->nodeCount; i++) {
        if (graph->nodes[i]->id == id) {
            index = i;
            break;
        }
    }
    if (index == -1) return 0;

    node_t* node = graph->nodes[index];
    edge_t* e = node->edges;
    while (e) {
        edge_t* next = e->next;
        free(e);
        e = next;
    }

    free(node->data);
    free(node);

    for (int i = index; i < graph->nodeCount - 1; i++)
        graph->nodes[i] = graph->nodes[i + 1];

    graph->nodeCount--;
    return 1;
}

void printGraph(graph_t* graph) {
    if (!graph) return;
    for (int i = 0; i < graph->nodeCount; i++) {
        node_t* node = graph->nodes[i];
        printf("Node %d:\n", node->id);
        edge_t* e = node->edges;
        while (e) {
            printf("  -> Node %d (weight: %.2f)\n", e->toNode->id, e->weight);
            e = e->next;
        }
        if (!node->edges)
            printf("  (no outgoing edges)\n");
    }
}

























