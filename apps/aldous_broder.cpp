/**
 * aldous_brodler.cpp
 *
 * By Sebastian Raaphorst, 2018.
  *
 * Generate a maze using the Aldous-Brodler algorithm.
 * Note that this is an extremely inefficient algorithm.
 */

#include <maze/AldousBroderMazeGenerator.h>

#include "Executor.h"

int main(int argc, char *argv[]) {
    return Executor<spelunker::maze::AldousBroderMazeGenerator>::generateAndDisplayMaze(argc, argv);
}
