============================================================
DEVELOPER DOCUMENTATION Project 1 - Rainfall Flood Simulation
============================================================

Author: Cayden Olsen
Course: COM S 3270
Repository: coms3270p1
Date: 10/31/2025

------------------------------------------------------------
Repository Structure
------------------------------------------------------------

coms3270P1/
│
├── mapper.c          # Main validator driver
├── data.c            # Validation logic
├── data.h            # validate() prototype
│
├── graph.c           # Core graph implementation
├── graph.h           # Graph type definitions and prototypes
├── testgraph.c       # Main program for graph building
├── testgraph.h       # Node and edge data struct definitions
│
├── citydata.c        # Command-line tool for city graph queries
│
├── Makefile          # Build configuration for mapper, testgraph, and city data
│
├── Ames.csv          # Large test dataset
├── test_valid.csv    # Valid small test file
├── test_invalid_id.csv # Invalid ID test
├── test_invalid_latitude.csv # Invalid coordinate test
├── test_graph.csv    # Graph structure test
├── test_citydata.csv # Small dataset for citydata testing
│
├── README            # Description and usage guide
└── DEVELOPER         # Developer documentation

------------------------------------------------------------
Source Files Overview
------------------------------------------------------------

1. data.c
   - Implements:
       int validate(void);
   - Reads a tab-separated dataset from stdin.
   - Performs data validation for POIs and roads.
   - Returns 0 if valid, or the line number of the first invalid entry.
   - Includes a helper function:
       int is_valid_id(const char *s);
     that allows numeric IDs with an optional leading '-'.

2. data.h
   - Header file that declares the prototype for validate().
   - Uses include guards to prevent multiple inclusions.

3. mapper.c
   - Contains main() which calls validate().
   - Prints "VALID" if the file is valid, otherwise prints
     the line number of the first invalid entry.

4. graph.h
   - Defines node_t, edge_t, graph_t.
   - Declares all graph manipulation functions.

5. graph.c
   - Implements:
        graph_t* createGraph();
        void freeGraph(graph_t* graph);
        node_t* addNode(graph_t* graph, int id, void* data);
        edge_t* addEdge(graph_t* graph, int fromId, int toId, float weight, void* data);
        node_t* getNode(graph_t* graph, int id);
        edge_t* getEdge(graph_t* graph, int fromId, int toId);
        int removeNode(graph_t* graph, int id);
        int removeEdge(graph_t* graph, int fromId, int toId);
        void printGraph(graph_t* graph);

   - Dynamic memory allocation for all nodes and edges.
   - Automatic resizing of node array when capacity exceeded.

6. testgraph.h
   - Defines:
        typedef struct {
            char name[128];
            double lat;
            double lon;
        } poi_data_t;

7. testgraph.c
   - Reads tab-separated data from stdin.
   - Creates a graph and populates it using addNode() and addEdge().
   - Calls printGraph() to display adjacency list format.

8. citydata.c
   - Implements multiple command-line utilities to analyze a city road graph:
       - `-f <filename>`: Specifies the dataset to load (required).
       - `-location <name>`: Finds latitude/longitude of a POI.
       - `-diameter`: Finds farthest two POIs using great-circle distance.
       - `-distance <A> <B>`: Computes straight-line (Haversine) distance between two POIs.
       - `-roaddist <A> <B>`: Computes shortest path between two POIs via roads (Dijkstra).
   - When executed without parameters, prints a detailed usage statement.
   - Parses argv in any order; executes parameters sequentially as they appear.
   - Uses the existing graph structure to load data efficiently.
   - Distance calculations assume Earth radius of 6371000 meters.

8. Makefile
   - Defines the build process without macros or variables.
   - Targets:
       mapper  - Builds the mapper
       testgraph   - Builds the graph builder
       citydata   - Build the citydata analyzer
       clean   - Removes all object and executable files

------------------------------------------------------------
Key Data Structures
------------------------------------------------------------

graph_t:
    - Contains an expandable array of node pointers.
    - Fields:
        node_t** nodes
        int nodeCount
        int edgeCount
        int nodeSpace

node_t:
    - Represents a point of interest.
    - Fields:
        int id
        void* data
        edge_t* edges

edge_t:
    - Represents a road segment.
    - Fields:
        node_t* toNode
        float weight
        void* data
        edge_t* next

poi_data_t (in testgraph.h):
    - Stores information about each POI (Point of Interest).
    - Fields:
        int id
        char name[128]
        double lat
        double lon

path_t (used in citydata.c for Dijkstra and distance queries):
    - Represents a computed shortest path between POIs.
    - Fields:
        node_t** nodes
        int count
        float totalDistance

------------------------------------------------------------
Function Summary
------------------------------------------------------------

int is_valid_id(const char *s);
    - Checks that a string represents an integer.
    - Allows an optional leading '-' for negative IDs.
    - Returns 1 if valid, 0 otherwise.

int validate(void);
    - Reads data from stdin.
    - Validates POIs and roads according to specification.
    - Returns 0 if valid, or the line number of the first invalid line.

int main(void);
    - Calls validate().
    - Prints "VALID" or the error line number.

graph_t* createGraph(void);
void freeGraph(graph_t* graph);
node_t* addNode(graph_t* graph, int id, void* data);
edge_t* addEdge(graph_t* graph, int fromId, int toId, float weight, void* data);
node_t* getNode(graph_t* graph, int id);
edge_t* getEdge(graph_t* graph, int fromId, int toId);
int removeNode(graph_t* graph, int id);
int removeEdge(graph_t* graph, int fromId, int toId);
void printGraph(graph_t* graph);
int getNodeIndex(graph_t* graph, int id);
float getEdgeWeight(graph_t* graph, int fromId, int toId);
void resetGraphVisits(graph_t* graph);

float haversine(double lat1, double lon1, double lat2, double lon2);
    - Computes the great-circle distance between two points on Earth.

node_t* findNodeByName(graph_t* graph, const char* name);
    - Searches the graph for a POI matching the given name.

void findCityDiameter(graph_t* graph);
    - Determines and prints the two POIs that are farthest apart by great-circle distance.

float computeShortestRoadDistance(graph_t* graph, node_t* start, node_t* end);
    - Uses Dijkstra’s algorithm to compute the minimum road distance between two POIs.

void handleCommand(graph_t* graph, int argc, char** argv);
    - Processes command-line arguments for citydata.c and dispatches the correct function.

------------------------------------------------------------
Compilation Instructions
------------------------------------------------------------

Run validator:
    make

Produces mapper:
    ./mapper

Run testgraph:
    make testgraph

Produces testgraph:
    ./testgraph

Run citydata analysis:
    make citydata

Produces citydata:
    ./citydata

Clean up:
    make clean

------------------------------------------------------------
Testing
------------------------------------------------------------

Run the validator:
    ./mapper < test_valid.csv
    ./mapper < test_invalid_latitude.csv
    ./mapper < test_invalid_id.csv

Expected outputs:
    test_valid.csv: VALID
    test_invalid_latitude.csv: 2
    test_invalid_id: 5

Run the graph builder:
    ./testgraph < test_graph.csv

Expected output:
    Node -1436: Starbucks (42.0119488, -93.6098681)
        Node -1435 (weight: 1.2, data: Duff Ave)
        Node -1434 (weight: 3.1, data: University Blvd)

    Node -1435: Home2 Suites by Hilton Ames (42.0336743, -93.5846869)
        Node -1434 (weight: 2.5, data: Campus Rd)

    Node -1434: POI (42.0413742, -93.6425578)
        (no outgoing edges)

Run the city data analyzer:
    ./citydata -f test_citydata.csv -location "Library"
        EXPECTED: 42.030781 -93.631913
    ./citydata -f test_citydata.csv -distance "Library" "Mall"
        EXPECTED: 742.9
    ./citydata -f test_citydata.csv -diameter
        EXPECTED: 42.025000 -93.635000 42.033000 -93.640000 925.4
    ./citydata -f test_citydata.csv -roaddist "Library" "Stadium"
        EXPECTED: 1100.0

------------------------------------------------------------
Notes
------------------------------------------------------------

- File input is read directly from stdin.
- The validator uses tab-based field separation (`\t`).
- The code handles "NaN" as 0 for distance.
- Graph uses singly linked adjacency lists.
- Node array doubles in size automatically when full.
- All heap-allocated memory is freed via freeGraph().
- IDs are matched exactly; edge creation fails if nodes missing.
- Output formatting is designed for clarity over compactness.
- File input is read directly from stdin or a filename (-f).
- Tab-separated input is required.
- All coordinates use degrees; conversions done via Haversine formula.
- The Dijkstra algorithm uses edge weights (distance) as meters.
- When no arguments are provided, citydata prints a usage guide.
- Graph memory is fully released using freeGraph().

------------------------------------------------------------
LLM/AI Usage
------------------------------------------------------------

The only LLM/AI usage during this project was to debug VsCode
complications. Otherwise, completion of the project itself
is independent.

============================================================