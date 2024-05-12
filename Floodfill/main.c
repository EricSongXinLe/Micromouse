#include <stdio.h>
#include "solver.h"
#include "API.h"
#include "queue.h"


// You do not need to edit this file.
// This program just runs your solver and passes the choices
// to the simulator.
/*
int main(int argc, char* argv[]) {
    debug_log("Running...");
    while (1) {
        Action nextMove = solver();
        switch(nextMove){
            case FORWARD:
                API_moveForward();
                break;
            case LEFT:
                API_turnLeft();
                break;
            case RIGHT:
                API_turnRight();
                break;
            case IDLE:
                break;
        }
    }
}
*/
int main(){
// Creating a queue
    struct Queue* queue = createQueue();

    // Enqueuing some Coord elements
    struct Coord point1 = {1, 2};
    struct Coord point2 = {3, 4};
    struct Coord point3 = {5, 6};

    push_back(queue, point1);
    push_back(queue, point2);
    push_back(queue, point3);

    // Dequeuing and printing elements
    printf("Dequeuing elements:\n");
    while (!isEmpty(queue)) {
        struct Coord coord = pop(queue);
        printf("(%d, %d)\n", coord.x, coord.y);
    }

    // Freeing memory
    free(queue);

    return 0;
}