============================================================
Project 1 - Rainfall Flood Simulation
============================================================

Author: Cayden Olsen
Course: COM S 3270
Repository: coms3270p1
Date: 10/31/2025

------------------------------------------------------------
Overview
------------------------------------------------------------

This program validates a tab-separated data file (Ames.csv)
that contains Points of Interest (POIs) and Road segments
for a given map. It checks the correctness of the data file
and reports whether it is valid or the line number of the
first invalid entry.

Create a graph data structure that stores POIs as nodes 
and roads as directed edges. Each node and edge holds 
additional data (e.g., name, coordinates, distance). 
The program can read the data, store it in the graph, and 
print a readable representation of the graph.

Use the citydata program to perform spatial and route-based
computations on the POI network. It loads validated road data
into the graph and supports command-line operations to find 
specific locations, compute straight-line and road distances 
between POIs, and determine the map’s diameter using the 
Haversine distance formula and Dijkstra’s shortest-path 
algorithm.

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
Validation Rules
------------------------------------------------------------

A file is INVALID if any of the following are true:
  - Latitude < -90 or > 90
  - Longitude < -180 or > 180
  - POI or Road ID contains non-numeric characters (except optional leading '-')
  - Empty POI name or road name
  - Missing or unreadable numeric values
  - The number of POIs or Roads is zero or negative
  - "NaN" distances are allowed but count as 0.0 for validation

------------------------------------------------------------
Graph Construction
------------------------------------------------------------

- Nodes represent Points of Interest (POIs)
- Edges represent Roads between POIs
- Edge weights are the distances from the data file
- Node `data` includes POI name, latitude, and longitude
- Edge `data` includes the road name

------------------------------------------------------------
City Data Operations
------------------------------------------------------------

The citydata program performs data queries and computations 
using the graph structure.

Supported operations:
  - `-f <filename>`  
    Loads a city dataset (TSV file). Required for all other operations.

  - `-location <name>`  
    Finds the latitude and longitude of a specific POI.

  - `-diameter`  
    Finds the largest straight-line (great-circle) distance between 
    any two POIs. Outputs both coordinates and the distance in meters.

  - `-distance <name1> <name2>`  
    Finds the “as the crow flies” distance between two POIs.

  - `-roaddist <name1> <name2>`  
    Computes the shortest path distance in meters between two POIs 
    using Dijkstra’s algorithm on the road network.

------------------------------------------------------------
Command Rules
------------------------------------------------------------

- Parameters may appear in any order.  
- Multiple operations can be combined in one command.  
  (Outputs appear in the order parameters are listed.)  
- Running with no parameters displays a usage statement 
  listing all supported flags.  
- Quoted strings (e.g., `"Atanasoff Hall"`) are parsed as a 
  single argument.

------------------------------------------------------------
Execution
------------------------------------------------------------

To build everything:
    make

To run the data validator:
    ./mapper < Ames.csv

To run the graph builder and printer:
    ./testgraph < Ames.csv

To run city data operations:
    ./citydata < Ames.csv -location "Atanasoff Hall"
    ./citydata < Ames.csv -distance "Atanasoff Hall" "Kildee Hall"
    ./citydata < Ames.csv -roaddist "Ames Highschool" "Coffee Place"
    ./citydata < Ames.csv -diameter

To clean compiled files:
    make clean

------------------------------------------------------------
Testing
------------------------------------------------------------

Run the validator:
    ./mapper < test_valid.csv
    ./mapper < test_invalid_latitude.csv
    ./mapper < test_invalid_id.csv

Run the graph builder:
    ./testgraph < test_graph.csv

Run the city analyzer:
    ./citydata -f test_citydata.csv -location "Starbucks"
    ./citydata -f test_citydata.csv -distance "Starbucks" "POI"
    ./citydata -f test_citydata.csv -diameter
    ./citydata -f test_citydata.csv -roaddist "Starbucks" "Home2 Suites by Hilton Ames"

------------------------------------------------------------
Expected Outputs
------------------------------------------------------------

Validator:
  test_valid.csv [VALID]
  test_invalid_latitude.csv [2]  
  test_invalid_id.csv [5]  

Graph Printer:
  Prints a formatted adjacency list representation of the graph:
    Node -1436: Starbucks (42.0119488, -93.6098681)
        Node -1435 (weight: 1.2, data: Duff Ave)
        Node -1434 (weight: 3.1, data: University Blvd)

    Node -1435: Home2 Suites by Hilton Ames (42.0336743, -93.5846869)
        Node -1434 (weight: 2.5, data: Campus Rd)

    Node -1434: POI (42.0413742, -93.6425578)
        (no outgoing edges)

City Data Analyzer:
  ./citydata -f test_citydata.csv -location "Starbucks"
      42.0119488 -93.6098681
  ./citydata -f test_citydata.csv -distance "Starbucks" "Home2 Suites by Hilton Ames"
      2321.93
  ./citydata -f test_citydata.csv -diameter
      42.0119 -93.6098 42.0413 -93.6425 4121.12
  ./citydata -f test_citydata.csv -roaddist "Starbucks" "POI"
      3300.45

------------------------------------------------------------
Known Issues / Notes
------------------------------------------------------------

- The program assumes well-formed tab-separated input.
- No dynamic memory allocation is used; reading is line-based via scanf.
- Large data files may take a few seconds to validate.
- Graph resizing doubles array size when capacity exceeded.
- testgraph output format may differ slightly depending on data spacing.
- Citydata requires -f <filename> for all operations.
- Great-circle calculations use the haversine formula.
- Dijkstra’s algorithm runs efficiently for moderately sized datasets.
- Edge cases: duplicate names, missing coordinates, or zero-distance roads.
- Graph doubles capacity automatically when full.
- Citydata output order matches command-line option order.

============================================================