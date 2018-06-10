/**
 * GridColouringThickMazeGenerator.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include <algorithm>
#include <cassert>
#include <optional>
#include <tuple>
#include <vector>

#include "types/CommonMazeAttributes.h"
#include "GridColouring.h"
#include "math/RNG.h"
#include "ThickMaze.h"
#include "ThickMazeAttributes.h"

#include "GridColouringThickMazeGenerator.h"

#ifdef DEBUG
#include <iostream>
using namespace std;
#endif

namespace spelunker::thickmaze {
    GridColouringThickMazeGenerator::GridColouringThickMazeGenerator(int w, int h, const GridColouring &gc,
                                                                     const GridColouring::CandidateConfiguration &cfg)
    : ThickMazeGenerator(w, h), gridColouring(gc), configuration(cfg) {}


    const ThickMaze GridColouringThickMazeGenerator::generate() {
#ifdef DEBUG
        cout << "Using following colouring:" << endl;
        for (auto i=0; i < gridColouring.numRows(); ++i) {
            cout << "\t";
            for (auto j = 0; j < gridColouring.numCols(); ++j)
                cout << gridColouring.cellColour(j, i) << " ";
            cout << endl;
        }
        cout << "Room colour: " << configuration.roomColour << endl;
        cout << "Walls: ";
        for (const auto &xyz: configuration.walls) {
            cout << '[';
            for (int i=0; i < xyz.size(); ++i) {
                cout << xyz[i];
                if (i < xyz.size() - 1)
                    cout << ",";
            }
            cout << "] ";
        }
        cout << endl;
#endif
        // Create a layout consisting completely of walls.
        auto contents = thickmaze::createThickMazeCellContents(width, height, WALL);

        // Find all the rooms as defined by the colouring and the configuration.
        std::vector<std::pair<int, int>> rooms;
        for (auto x = 0; x < width; ++x)
            for (auto y = 0; y < height; ++y)
                if (gridColouring.cellColour(x, y) == configuration.roomColour)
                    rooms.emplace_back(std::make_pair(x, y));

        // Turn walls into offsets, i.e. coordinates in relation to a given coordinate.
        GridColouring::OffsetMap offsetMap = gridColouring.mapWallsToOffsets(configuration);

#ifdef DEBUG
        cout << "OffsetMap:" << endl;
        for (auto om: offsetMap) {
            const auto &[p,q] = om;
            cout << p << ": ";
            for (auto c: q) {
                cout << "(" << c.first << "," << c.second << ") ";
            }
        }
        cout << endl;
        cout.flush();
#endif
        // Maintain a collection of walls to process.
        std::vector<AggregateWall> walls;

        // Pick a cell at random, mark it as floor, and add its walls to the vector.
        auto [sx,sy] = math::RNG::randomElement(rooms);
#ifdef DEBUG
        cout << "Starting at cell " << sx << "," << sy << endl;
#endif

        contents[sx][sy] = FLOOR;
        auto swalls = adjacentWalls(types::cell(sx, sy), offsetMap);
#ifdef DEBUG
        cout << "\tAdjacent walls:" << endl;
        for (auto swall: swalls) {
            cout << "\t[";
            for (int i=0; i < swall.size(); ++i) {
                cout << "(" << swall[i].first << "," << swall[i].second << ")";
                if (i < swall.size() - 1)
                    cout << ",";
            }
            cout << "]" << endl;
        }
#endif
        for (auto sw: swalls)
            walls.emplace_back(sw);

        while (!walls.empty()) {
#ifdef DEBUG
            cout << "Walls has size: " << walls.size() << endl;
#endif
            // Pick a random wall from the list.
            // We swap it with the end element and remove that for efficiency.
            const auto wallIdx = math::RNG::randomRange(walls.size());
            std::swap(walls[wallIdx], walls.back());
            const auto wall = walls.back();
            walls.pop_back();
#ifdef DEBUG
            cout << "Processing wall: ";
            for (auto xyz: wall) {
                cout << "(" << xyz.first << "," << xyz.second << ") ";
            }
            cout << endl;
#endif
            // Check if one of the rooms adjacent to this wall is uncovered.
            // If so, remove the wall, mark the room as visited by making it a floor, and add its
            // walls to the vector.
            auto arooms = adjacentRooms(wall);
#ifdef DEBUG
            cout << "\tAdjacent rooms: ";
            for (auto xyz: arooms) {
                cout << "(" << xyz.first << "," << xyz.second << ")=" << (contents[xyz.first][xyz.second] == WALL ? "uncovered" : "covered") << "   ";
            }
            cout << endl;
#endif
            bool uncovered = false;
            for (auto r: arooms) {
                auto [rx, ry] = r;
                if (contents[rx][ry] == WALL) {
#ifdef DEBUG
                    cout << "\tSetting room " << rx << "," << ry << " to FLOOR" << endl;
#endif
                    contents[rx][ry] = FLOOR;
                    for (auto rw: wall) {
                        auto [rwx, rwy] = rw;
#ifdef DEBUG
                        cout << "\tSetting wall " << rwx << ","  << rwy << " to FLOOR" << endl;
#endif
                        contents[rwx][rwy] = FLOOR;
                    }

                    auto rWalls = adjacentWalls(r, offsetMap);
                    for (auto rWall: rWalls) {
#ifdef DEBUG
                        cout << "\tAdding wall: ";
                        for (auto xyz: rWall) {
                            cout << xyz.first << "," << xyz.second << " ";
                        }
                        cout << endl;
#endif
                        walls.emplace_back(rWall);
                    }
                }
            }
        }

        return ThickMaze(width, height, contents);
    }

    std::optional<GridColouringThickMazeGenerator::AggregateWall>
            GridColouringThickMazeGenerator::offsetToAggregate(const types::Cell &c, const GridColouring::Offsets &offsets) {
        AggregateWall wall;

        auto [x, y] = c;
        for (auto offset: offsets) {
            auto [dx, dy] = offset;
            int cx = x + dx;
            int cy = y + dy;
            if (cx < 0 || cx >= width || cy < 0 || cy >= height)
                return {};
            wall.emplace_back(std::make_pair(cx, cy));
        }
        return wall;
    }

    types::CellCollection GridColouringThickMazeGenerator::adjacentRooms(const AggregateWall &wall) {
        types::CellCollection rooms;
        for (auto w: wall) {
            // Check the colours of all of the cells around w.
            auto nbrs = neighbours(w);
            for (auto n: nbrs) {
                const auto [nx, ny] = n;
                if (gridColouring.cellColour(nx,ny) == configuration.roomColour)
                    rooms.emplace_back(n);
            }
        }

        //assert(rooms.size() > 0 && rooms.size() <= 2);
        return rooms;
    }

    GridColouringThickMazeGenerator::AggregateWallCollection
        GridColouringThickMazeGenerator::adjacentWalls(const types::Cell &c, const GridColouring::OffsetMap &offsetMap) {

        AggregateWallCollection walls;
        for (const auto &offsetData: offsetMap) {
            const auto &[dir, offsets] = offsetData;
            auto wall = offsetToAggregate(c, offsets);
            if (wall.has_value())
                walls.emplace_back(wall.value());
        }

        return walls;
    }
}