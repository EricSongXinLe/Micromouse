#include "solver.h"
#include "API.h"
#include <math.h>
#include "queue.h"
#include <stdio.h>

Action solver() {
    return floodFill();
}

// This is an example of a simple left wall following algorithm.
Action leftWallFollower() {
    if(API_wallFront()) {
        if(API_wallLeft()){
            return RIGHT;
        }
        return LEFT;
    }
    return FORWARD;
}


// Put your implementation of floodfill here!
Action floodFill() {
    struct Queue* queue = createQueue();
    int manhattanDist[16][16];
    int horizWalls[16][16];
    int vertWalls[16][16];
    for(int i = 0; i < 16; i++) {
        for(int j = 0; j < 16; j++) {
            manhattanDist[i][j] = abs(i - 7) + abs(j - 7);
            printf("%d\n", manhattanDist[i][j]);
            horizWalls[i][j] = 0;
            vertWalls[i][j] = 0;
        }
    }
    return IDLE;
}