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
Program Behavior
------------------------------------------------------------
If the file passes all validation checks:
    Output: "VALID"

If the file is invalid:
    Output: <line number> (of the first invalid line)

------------------------------------------------------------
Execution
------------------------------------------------------------
To build the program:
    make

To run validation on a file (e.g., Ames.csv):
    ./mapper < Ames.csv

To clean compiled files:
    make clean

------------------------------------------------------------
Test Files
------------------------------------------------------------
The repository includes three small test files:
    test1.csv  - Minimal valid POI and road entries
    test2.csv  - Contains an invalid latitude
    test3.csv  - Contains missing or malformed data

------------------------------------------------------------
Known Issues / Notes
------------------------------------------------------------
- The program assumes well-formed tab-separated input.
- No dynamic memory allocation is used; reading is line-based via scanf.
- Large data files may take a few seconds to validate.


