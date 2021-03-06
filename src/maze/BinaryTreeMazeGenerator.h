/**
 * BinaryTreeMazeGenerator.h
 *
 * By Sebastian Raaphorst, 2018.
 *
 * Uses the simplistic binary tree approach to generating a maze.
 */

#pragma once

#include <types/Dimensions2D.h>

#include "MazeGenerator.h"

namespace spelunker::maze {
    class Maze;

    /// Maze generator using the binary tree approach.
    /**
     * The BinaryTreeMazeGenerator proceeds as follows:
     *
     * Iterate over the cells of the maze, starting in the NW corner and proceeding row-by-row.
     *
     * For each cell, if possible to carve an east wall and a south wall, carve an east wall with probability p,
     * and a south wall with probability 1-p.
     *
     * Along the easternmost column, it will only be possible to carve southward.
     * Along the southmost column, it will only be possible to carve eastward.
     * In the SE corner, it will not be possible to cave at all.
     *
     * This algorithm has significant bias, as paths flow to the SE, but is one of the simplest algorithms.
     *
     * The probability factor p, for high p, can result in long horizontal passages, and, for low p,
     * long vertical passages.
     */
    class BinaryTreeMazeGenerator final : public MazeGenerator {
    public:
        BinaryTreeMazeGenerator(const types::Dimensions2D &d, double p);
        BinaryTreeMazeGenerator(int w, int h, double p);
        BinaryTreeMazeGenerator(const types::Dimensions2D &d);
        BinaryTreeMazeGenerator(int w, int h);
        ~BinaryTreeMazeGenerator() final = default;

        const Maze generate() const noexcept final;

        static constexpr double defaultEastProbability = 0.5;

    private:
        const double eastProbability;
    };
}

