/**
 * RecursiveDivisionMazeGenerator.h
 *
 * By Sebastian Raaphorst, 2018.
 *
 * A maze generator using the
 * <a href="http://weblog.jamisbuck.org/2011/1/12/maze-generation-recursive-division-algorithm"
 * recursive division algorithm.</a>
 */

#pragma once

#include <types/Dimensions2D.h>

#include "MazeGenerator.h"

namespace spelunker::maze {
    class Maze;

    class RecursiveDivisionMazeGenerator final : public MazeGenerator {
    public:
        RecursiveDivisionMazeGenerator(const types::Dimensions2D &d);
        RecursiveDivisionMazeGenerator(int w, int h);
        ~RecursiveDivisionMazeGenerator() final = default;

        const Maze generate() const noexcept final;
    private:
        /// A rectangle struct we use to represent sections of the maze to complete, to avoid recursion.
        struct rectangle {
            const int x, y, w, h;
            rectangle(int px, int py, int pw, int ph) : x{px}, y{py}, w{pw}, h{ph} {}
        };
    };
}
