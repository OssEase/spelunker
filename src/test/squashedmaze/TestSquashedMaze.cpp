/**
 * TestSquashedMaze.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include <catch.hpp>

#include <maze/DFSMazeGenerator.h>
#include <maze/Maze.h>
#include <squashedmaze/SquashedMazeAttributes.h>
#include <squashedmaze/SquashedMaze.h>

using namespace spelunker;

TEST_CASE("SquashedMaze should process a Maze", "[squashedmaze][maze]") {
    constexpr auto width = 20;
    constexpr auto height = 20;
    const maze::DFSMazeGenerator dfs{width, height};
    const auto m = dfs.generate();

    const auto sm = squashedmaze::SquashedMaze(m);

    const auto &mp = sm.getEdgeMap();
    const auto &g = sm.getGraph();
    REQUIRE(true == true);
}