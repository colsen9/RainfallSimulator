#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "graph.h"
#include "testgraph.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

static void usage(const char *prog) {
    printf("Usage: %s -f <filename> [options]\n", prog);
    printf("Options (order may vary; multiple outputs follow order of args):\n");
    printf("  -f <filename>                : (required) tab-separated data file\n");
    printf("  -location <locationname>     : print latitude longitude\n");
    printf("  -diameter                    : print lat1 lon1 lat2 lon2 distance_m\n");
    printf("  -distance <name1> <name2>    : print great-circle distance (meters)\n");
    printf("  -roaddist <name1> <name2>    : print shortest road distance (meters)\n");
    printf("\nNotes:\n  - Names containing spaces must be passed quoted so they appear as single argv entries.\n");
}

static double deg2rad(double d) {
    return d * M_PI / 180.0;
}

static double haversine_m(double lat1, double lon1, double lat2, double lon2) {
    double R = 6371000.0;
    double phi1 = deg2rad(lat1);
    double phi2 = deg2rad(lat2);
    double dphi = deg2rad(lat2 - lat1);
    double dlambda = deg2rad(lon2 - lon1);

    double a = sin(dphi/2.0) * sin(dphi/2.0) +
               cos(phi1) * cos(phi2) *
               sin(dlambda/2.0) * sin(dlambda/2.0);
    double c = 2.0 * atan2(sqrt(a), sqrt(1 - a));
    return R * c;
}

static node_t* find_node_by_name(graph_t *g, const char *name) {
    if (!g) return NULL;
    for (int i = 0; i < g->nodeCount; ++i) {
        node_t *n = g->nodes[i];
        if (n && n->data) {
            POIData *p = (POIData*) n->data;
            if (strcmp(p->name, name) == 0) return n;
        }
    }
    return NULL;
}

static graph_t* build_graph_from_file(FILE *fp) {
    if (!fp) return NULL;
    graph_t *g = createGraph();
    if (!g) return NULL;

    int numPOI = 0;
    if (fscanf(fp, "%d", &numPOI) != 1 || numPOI <= 0) {
        freeGraph(g);
        return NULL;
    }

    int ch = fgetc(fp);
    if (ch != '\n' && ch != EOF) ungetc(ch, fp);

    for (int i = 0; i < numPOI; ++i) {
        char line[1024];
        if (!fgets(line, sizeof(line), fp)) { freeGraph(g); return NULL; }

        char name[256];
        int id;
        double lat, lon;

        char *p = line;
        if (sscanf(p, "%d", &id) != 1) { freeGraph(g); return NULL; }

        char *tab1 = strchr(p, '\t');
        if (!tab1) { freeGraph(g); return NULL; }
        tab1++;

        char *tab2 = strchr(tab1, '\t');
        if (!tab2) { freeGraph(g); return NULL; }

        size_t namelen = tab2 - tab1;
        if (namelen >= sizeof(name)) namelen = sizeof(name)-1;
        memcpy(name, tab1, namelen);
        name[namelen] = '\0';

        if (sscanf(tab2+1, "%lf\t%lf", &lat, &lon) != 2) { freeGraph(g); return NULL; }

        POIData *poi = malloc(sizeof(POIData));
        if (!poi) { freeGraph(g); return NULL; }
        strncpy(poi->name, name, sizeof(poi->name)-1);
        poi->name[sizeof(poi->name)-1] = '\0';
        poi->lat = lat;
        poi->lon = lon;

        if (!addNode(g, id, poi)) {
            free(poi);
            freeGraph(g);
            return NULL;
        }
    }

    int numRoads = 0;
    if (fscanf(fp, "%d", &numRoads) != 1 || numRoads <= 0) {
        freeGraph(g);
        return NULL;
    }

    ch = fgetc(fp);
    if (ch != '\n' && ch != EOF) ungetc(ch, fp);

    for (int i = 0; i < numRoads; ++i) {
        char line[1024];
        if (!fgets(line, sizeof(line), fp)) { freeGraph(g); return NULL; }

        int fromId, toId;
        char dist_token[64];
        double lat, lon;
        char roadName[256];

        char *p = line;
        char *tab1 = strchr(p, '\t');
        if (!tab1) { freeGraph(g); return NULL; }
        *tab1 = '\0';
        fromId = atoi(p);
        p = tab1 + 1;

        char *tab2 = strchr(p, '\t');
        if (!tab2) { freeGraph(g); return NULL; }
        *tab2 = '\0';
        toId = atoi(p);
        p = tab2 + 1;

        char *tab3 = strchr(p, '\t');
        if (!tab3) { freeGraph(g); return NULL; }
        *tab3 = '\0';
        strncpy(dist_token, p, sizeof(dist_token)-1);
        dist_token[sizeof(dist_token)-1] = '\0';
        p = tab3 + 1;

        char *tab4 = strchr(p, '\t');
        if (!tab4) { freeGraph(g); return NULL; }
        *tab4 = '\0';
        if (sscanf(p, "%lf", &lat) != 1) { freeGraph(g); return NULL; }
        p = tab4 + 1;

        char *tab5 = strchr(p, '\t');
        if (!tab5) { freeGraph(g); return NULL; }
        *tab5 = '\0';
        if (sscanf(p, "%lf", &lon) != 1) { freeGraph(g); return NULL; }
        p = tab5 + 1;

        char *newline = strchr(p, '\n');
        if (newline) *newline = '\0';
        strncpy(roadName, p, sizeof(roadName)-1);
        roadName[sizeof(roadName)-1] = '\0';

        float distVal = 0.0f;
        if (strcmp(dist_token, "NaN") != 0) {
            double tmp;
            if (sscanf(dist_token, "%lf", &tmp) == 1) distVal = (float)tmp;
            else { freeGraph(g); return NULL; }
        } else {
            distVal = 0.0f;
        }

        RoadData *rd = malloc(sizeof(RoadData));
        if (!rd) { freeGraph(g); return NULL; }
        strncpy(rd->roadName, roadName, sizeof(rd->roadName)-1);
        rd->roadName[sizeof(rd->roadName)-1] = '\0';

        if (!addEdge(g, fromId, toId, distVal, rd)) {
            free(rd);
            freeGraph(g);
            return NULL;
        }
    }

    return g;
}

typedef struct {
    int idx;
    double dist;
} HeapItem;

static void swap_heap(HeapItem *a, HeapItem *b) {
    HeapItem tmp = *a; *a = *b; *b = tmp;
}

static void heap_push(HeapItem heap[], int *heap_size, HeapItem item) {
    int i = (*heap_size)++;
    heap[i] = item;
    while (i > 0) {
        int parent = (i - 1) / 2;
        if (heap[parent].dist <= heap[i].dist) break;
        swap_heap(&heap[parent], &heap[i]);
        i = parent;
    }
}

static HeapItem heap_pop(HeapItem heap[], int *heap_size) {
    HeapItem result = heap[0];
    (*heap_size)--;
    heap[0] = heap[*heap_size];
    int i = 0;
    while (1) {
        int l = 2*i + 1, r = 2*i + 2, smallest = i;
        if (l < *heap_size && heap[l].dist < heap[smallest].dist) smallest = l;
        if (r < *heap_size && heap[r].dist < heap[smallest].dist) smallest = r;
        if (smallest == i) break;
        swap_heap(&heap[i], &heap[smallest]);
        i = smallest;
    }
    return result;
}

static double dijkstra_on_graph(graph_t *g, int sIndex, int tIndex) {
    if (!g) return INFINITY;
    int n = g->nodeCount;
    double *dist = malloc(sizeof(double) * n);
    int *visited = malloc(sizeof(int) * n);
    if (!dist || !visited) { free(dist); free(visited); return INFINITY; }
    for (int i = 0; i < n; ++i) { dist[i] = INFINITY; visited[i] = 0; }
    dist[sIndex] = 0.0;

    HeapItem *heap = malloc(sizeof(HeapItem) * n * 2);
    int heap_size = 0;
    heap_push(heap, &heap_size, (HeapItem){sIndex, 0.0});

    while (heap_size > 0) {
        HeapItem it = heap_pop(heap, &heap_size);
        int u = it.idx;
        if (visited[u]) continue;
        visited[u] = 1;
        if (u == tIndex) break;

        node_t *node = g->nodes[u];
        edge_t *e = node->edges;
        while (e) {
            int vIndex = -1;
            for (int k = 0; k < g->nodeCount; ++k) {
                if (g->nodes[k] == e->toNode) { vIndex = k; break; }
            }
            if (vIndex >= 0 && !visited[vIndex]) {
                double alt = dist[u] + (double) e->weight;
                if (alt < dist[vIndex]) {
                    dist[vIndex] = alt;
                    heap_push(heap, &heap_size, (HeapItem){vIndex, alt});
                }
            }
            e = e->next;
        }
    }

    double result = dist[tIndex];

    free(dist);
    free(visited);
    free(heap);
    return result;
}

int main(int argc, char **argv) {
    if (argc < 2) { usage(argv[0]); return 1; }

    char *filename = NULL;

    typedef enum { OP_LOCATION, OP_DIAMETER, OP_DISTANCE, OP_ROADDIST } OpType;
    typedef struct {
        OpType type;
        char *arg1;
        char *arg2;
    } Op;

    Op ops[64];
    int opcount = 0;

    for (int i = 1; i < argc; ++i) {
        if (strcmp(argv[i], "-f") == 0) {
            if (i + 1 >= argc) { fprintf(stderr, "Error: -f requires filename\n"); return 1; }
            filename = argv[++i];
        } else if (strcmp(argv[i], "-location") == 0) {
            if (i + 1 >= argc) { fprintf(stderr, "Error: -location requires name\n"); return 1; }
            ops[opcount++] = (Op){OP_LOCATION, argv[++i], NULL};
        } else if (strcmp(argv[i], "-diameter") == 0) {
            ops[opcount++] = (Op){OP_DIAMETER, NULL, NULL};
        } else if (strcmp(argv[i], "-distance") == 0) {
            if (i + 2 >= argc) { fprintf(stderr, "Error: -distance requires two names\n"); return 1; }
            ops[opcount++] = (Op){OP_DISTANCE, argv[++i], argv[++i]};
        } else if (strcmp(argv[i], "-roaddist") == 0) {
            if (i + 2 >= argc) { fprintf(stderr, "Error: -roaddist requires two names\n"); return 1; }
            ops[opcount++] = (Op){OP_ROADDIST, argv[++i], argv[++i]};
        } else if (strcmp(argv[i], "--help") == 0 || strcmp(argv[i], "-h") == 0) {
            usage(argv[0]);
            return 0;
        } else {
            fprintf(stderr, "Warning: unknown argument '%s' ignored\n", argv[i]);
        }
    }

    if (!filename) { fprintf(stderr, "Error: -f <filename> is required\n"); usage(argv[0]); return 1; }

    FILE *fp = fopen(filename, "r");
    if (!fp) { perror("fopen"); return 1; }

    graph_t *g = build_graph_from_file(fp);
    fclose(fp);
    if (!g) { fprintf(stderr, "Error: failed to load graph from '%s'\n", filename); return 1; }

    if (opcount == 0) { usage(argv[0]); freeGraph(g); return 0; }

    for (int oi = 0; oi < opcount; ++oi) {
        Op op = ops[oi];
        if (op.type == OP_LOCATION) {
            node_t *n = find_node_by_name(g, op.arg1);
            if (!n || !n->data) {
                printf("NOTFOUND\n");
            } else {
                POIData *p = (POIData*) n->data;
                printf("%.7f %.7f\n", p->lat, p->lon);
            }
        } else if (op.type == OP_DIAMETER) {
            double best = -1.0;
            POIData *pa = NULL, *pb = NULL;
            int ai=-1, bi=-1;
            for (int i = 0; i < g->nodeCount; ++i) {
                POIData *pi = (POIData*) g->nodes[i]->data;
                for (int j = i+1; j < g->nodeCount; ++j) {
                    POIData *pj = (POIData*) g->nodes[j]->data;
                    double d = haversine_m(pi->lat, pi->lon, pj->lat, pj->lon);
                    if (d > best) { best = d; pa = pi; pb = pj; ai=i; bi=j; }
                }
            }
            if (best < 0.0) {
                printf("0\n");
            } else {
                printf("%.7f %.7f %.7f %.7f %.2f\n", pa->lat, pa->lon, pb->lat, pb->lon, best);
            }
        } else if (op.type == OP_DISTANCE) {
            node_t *n1 = find_node_by_name(g, op.arg1);
            node_t *n2 = find_node_by_name(g, op.arg2);
            if (!n1 || !n2) {
                printf("NOTFOUND\n");
            } else {
                POIData *p1 = (POIData*) n1->data;
                POIData *p2 = (POIData*) n2->data;
                double d = haversine_m(p1->lat, p1->lon, p2->lat, p2->lon);
                printf("%.3f\n", d);
            }
        } else if (op.type == OP_ROADDIST) {
            node_t *n1 = find_node_by_name(g, op.arg1);
            node_t *n2 = find_node_by_name(g, op.arg2);
            if (!n1 || !n2) {
                printf("NOTFOUND\n");
            } else {
                int sIndex=-1, tIndex=-1;
                for (int i = 0; i < g->nodeCount; ++i) {
                    if (g->nodes[i] == n1) sIndex = i;
                    if (g->nodes[i] == n2) tIndex = i;
                }
                if (sIndex < 0 || tIndex < 0) { printf("NOTFOUND\n"); }
                else {
                    double dist = dijkstra_on_graph(g, sIndex, tIndex);
                    if (!isfinite(dist)) printf("UNREACHABLE\n");
                    else printf("%.3f\n", dist);
                }
            }
        }
    }

    freeGraph(g);
    return 0;
}













