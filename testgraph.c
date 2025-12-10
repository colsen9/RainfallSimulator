#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "graph.h"
#include "testgraph.h"

int main() {
    graph_t* g = createGraph();
    if (!g) {
        fprintf(stderr, "Error: Could not create graph.\n");
        return 1;
    }

    int numPOI, numRoads;
    if (scanf("%d", &numPOI) != 1 || numPOI <= 0) {
        fprintf(stderr, "Invalid POI section.\n");
        freeGraph(g);
        return 1;
    }

    for (int i = 0; i < numPOI; i++) {
        int id;
        char name[128];
        double lat, lon;
        if (scanf("%d\t%127[^\t]\t%lf\t%lf", &id, name, &lat, &lon) != 4) {
            fprintf(stderr, "Invalid POI data.\n");
            freeGraph(g);
            return 1;
        }

        POIData* poi = malloc(sizeof(POIData));
        strcpy(poi->name, name);
        poi->lat = lat;
        poi->lon = lon;

        addNode(g, id, poi);
    }

    if (scanf("%d", &numRoads) != 1 || numRoads <= 0) {
        fprintf(stderr, "Invalid road section.\n");
        freeGraph(g);
        return 1;
    }

    for (int i = 0; i < numRoads; i++) {
        int fromId, toId;
        float dist;
        double lat, lon;
        char roadName[128];

        if (scanf("%d\t%d\t%f\t%lf\t%lf\t%127[^\n]", &fromId, &toId, &dist, &lat, &lon, roadName) != 6) {
            fprintf(stderr, "Invalid road data.\n");
            freeGraph(g);
            return 1;
        }

        RoadData* rd = malloc(sizeof(RoadData));
        strcpy(rd->roadName, roadName);
        addEdge(g, fromId, toId, dist, rd);
    }

    printGraph(g);
    freeGraph(g);
    return 0;
}











