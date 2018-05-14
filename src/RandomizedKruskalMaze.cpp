/**
 * RandomizedKruskalMaze.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include <algorithm>
#include <random>
#include <tuple>
#include <vector>
#include <boost/pending/disjoint_sets.hpp>
#include "DisjointSetHelper.h"
#include "Maze.h"
#include "RandomizedKruskalMaze.h"

namespace vorpal::maze {
    using namespace std;

    RandomizedKruskalMaze::RandomizedKruskalMaze(const int w,
                                                 const int h,
                                                 const vorpal::maze::Cell &s,
                                                 const vorpal::maze::CellCollection &ends)
            : Maze(w, h, s, ends) {}

    const WallIncidence RandomizedKruskalMaze::generate() {
        // We start with all walls, and then remove them iteratively.
        auto wi = initializeEmptyLayout(true);

        // Create a collection of all possible walls.
        std::vector<int> walls;
        for (auto w = 0; w < numWalls; ++w)
            walls.emplace_back(w);

        // Get the map of wall ranks to adjacent cells.
        auto unrank = createUnrankWallMap();

        // Given a wall, find its adjacent cell.
        // We need disjoint sets to represent the connected sets of cells.
        // To do so efficiently, we use Boost's disjoint_sets with some helper classes and methods.

        // Rank / unrank cells:
        const auto ranker = [this](int x, int y) { return x * width + y; };
        const auto unranker = [this](int rk) { return std::make_pair(rk / width, rk % width); };

        // Create a vector of all elements.
        std::vector<Element> elements;
        elements.reserve(width * height);
        for (auto x = 0; x < width; ++x)
            for (auto y = 0; y < width; ++y)
                elements.emplace_back(Element(ranker(x, y)));

        // Create disjoint singleton sets.
        Rank rank(elements);
        Parent parent(elements);
        boost::disjoint_sets<Rank *, Parent *> dsets(&rank, &parent);
        std::for_each(elements.begin(), elements.end(), [&dsets](auto e) { dsets.make_set(e); });

        // Shuffle the vector of walls and then iterate over them.
        std::random_device rd;
        std::mt19937 g(rd());
        std::shuffle(walls.begin(), walls.end(), g);

        for (auto w : walls) {
            const auto &pp = unrank[w];

            const auto &c1 = pp.first.first;
            const auto &cx1 = c1.first;
            const auto &cy1 = c1.second;
            const auto cr1 = ranker(cx1, cy1);

            const auto &c2 = pp.second.first;
            const auto &cx2 = c2.first;
            const auto &cy2 = c2.second;
            const auto cr2 = ranker(cx2, cy2);

            // If the cells belong to separate partitions, remove the wall and join them.
            const auto set1 = dsets.find_set(Element(cr1));
            const auto set2 = dsets.find_set(Element(cr2));
            if (set1 != set2) {
                wi[w] = false;
                dsets.link(set1, set2);
            }
        }

        return wi;
    }
}