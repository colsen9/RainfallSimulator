#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "data.h"

int is_valid_id(const char *s) {
    int i = 0;
    if (s[0] == '-') i = 1;
    if (s[i] == '\0') return 0;
    for (; s[i]; i++) {
        if (!isdigit((unsigned char)s[i])) return 0;
    }
    return 1;
}

int validate() {
    int line_number = 1;
    int numPOI, numRoads;

    if (scanf("%d", &numPOI) != 1 || numPOI <= 0) {
        return line_number;
    }

    line_number++;

    for (int i = 0; i < numPOI; i++, line_number++) {
        char id_str[32];
        char name[128];
        double lat, lon;

        if (scanf("%31s", id_str) != 1)
            return line_number;

        if (scanf("%127[^\t\n]", name) != 1)
            return line_number;

        int j = 0;
        while (isspace(name[j])) j++;
        memmove(name, name + j, strlen(name + j) + 1);

        if (scanf("%lf\t%lf", &lat, &lon) != 2)
            return line_number;

        if (!is_valid_id(id_str)) return line_number;
        if (strlen(name) == 0) return line_number;
        if (lat < -90 || lat > 90 || lon < -180 || lon > 180)
            return line_number;
    }

    if (scanf("%d", &numRoads) != 1 || numRoads <= 0) {
        return line_number;
    }

    line_number++;

    for (int i = 0; i < numRoads; i++, line_number++) {
        char from_id[32], to_id[32];
        char dist_str[32];
        double lat, lon;
        char road_name[128];
        double distance = 0.0;

        if (scanf("%31s\t%31s\t%31s\t%lf\t%lf\t%127[^\n]",
                  from_id, to_id, dist_str, &lat, &lon, road_name) != 6)
            return line_number;

        if (!is_valid_id(from_id) || !is_valid_id(to_id))
            return line_number;

        if (strcmp(dist_str, "NaN") != 0) {
            if (sscanf(dist_str, "%lf", &distance) != 1)
                return line_number;
        }

        if (lat < -90 || lat > 90 || lon < -180 || lon > 180)
            return line_number;

        if (strlen(road_name) == 0)
            return line_number;
    }

    return 0;
}



















