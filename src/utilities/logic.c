#include "logic.h"

// rotates array passed to points around center by rotation
// degrees with nCorners amount of corner
void rotate (float *points, int rotation, int nCorners) {

    float theta = rotation / (180/M_PI);  // convert from degrees to radians

    // rotates the list of points with a matrix
    for (uint8_t i = 0; i < nCorners*2; i += 2) {
        float x = -1*(points[i])*cos(theta) - (points[i+1])*sin(theta);
        float y = -1*(points[i])*sin(theta) + (points[i+1])*cos(theta);

        points[i] = x;
        points[i+1] = y;
    }

}

// collision detection for a point and a polygon
// checks if any of the "points" are inseide of the "polygon"
uint8_t rayCastingCollision (int *points, uint8_t corners_points, int *polygon, uint8_t corners_polygon) {
    uint8_t intersects = 0;

    for (uint8_t i = 0; i < corners_points*2; i += 2) {  //
    	int16_t px = points[i];
        int16_t py = points[i+1];
        for (uint8_t j = 0; j < corners_polygon*2; j += 2) {
            int16_t x1 = polygon[j];
            int16_t y1 = polygon[j + 1];
            // go to first list entry if at the end of list
            int16_t x2 = polygon[j == 0 ? (corners_polygon*2)-2 : j - 2];
            int16_t y2 = polygon[j == 0 ? (corners_polygon*2)-1 : j - 1];

            // idk gave up and copied it, the math for the raycasting algorithm
            if (((y1 >= py) != (y2 >= py)) && (px <= (x2 - x1) * (py - y1) / (y2 - y1) + x1))
                intersects += 1;

        }
        // if intersects is odd the ray started inside the polygon
        // this means the point is inside the polygon
        if (intersects % 2 == 1) return 1;
        else intersects = 0; // check next set of points
    }
    return 0;
}

float square (float val) {  // return the square of val

    return val*val;
}

// check if two points are within a said radius of each other
uint8_t nearby (vector2 p1, vector2 p2, float r) {

    return ((square(p1.x - p2.x) + square(p1.y - p2.y) <= square(r)) ? 1 : 0);
    // (statement ? true : false)
}
