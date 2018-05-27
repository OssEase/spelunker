# spelunker

A C++17 library for generating / solving mazes. (It could be easily adapted for C++14, but this is a learning exercise for me in C++11, 14, and 17.)

# Maze Generation Algorithms

Right now, the library can generate perfect mazes using the following 12 algorithms:

1. Aldous-Broder with random walks.

2. Random binary trees (with branching probability).

3. Randomized BFS.

4. Randomized DFS.

5. Eller's algorithm, allowing probability for horizontal wall creation and density per contiguous block of cells in a row for vertical wall creation.

6. Hunt-and-kill.

7. Randomized Kruskal's algorithm.

8. Randomized Prim, iterating over walls.

9. Randomized Prim, iterating over cells.

10. Recursive division, allowing lower bound density and upper bound density for splits, probability to split vertically, and a flag to force alternating vertical / horizontal splits.

11. Sidewinder algorithm.

12. Wilson's algorithm.

# Unicursal Mazes / Labyrinths

Furthermore, mazes can be used to construct _unicursal mazes_ - more commonly known as labyrinths - which are mazes without branches, i.e. one long, twisting passageway.

# "Thick" Mazes and Generation Algorithms

Previously, the library only supported mazes with walls as cell dividers instead of walls as actual cells. Now I have implemented `ThickMaze`, a class where each cell in the maze is either `FLOOR` or `WALL`. This will allow some algorithmic implementations that are unavailable to mazes where walls are cell dividers. Most notably:

1. Cellular automata algorithms: these are offered with Moore and von Neumann extended neighbourhoods, and Maze / Mazecetric / Vote45 / Vote behavioural choosers. These are algorithms that generate a random grid of floor and walls, and then for a time t, use certain rules based on the contents of t-1 to determine the layout. They are known to converge to relatively stable configurations quite quickly, but seldom produce connected (and thus, not perfect) mazes. They are still interesting to study. The implementation allows users to supply their own neighbourhood counting functions and behavioural choosers, as well as max # generations, # generations to store to check for convergence, and probability that a cell is a wall during initialization.

# Typeclasses

Spelunker also defines and contains some typeclasses:

1. `Show`, that can be used to easily create string representations of mazes, thick mazes, coordinates, positions, etc. for text output; and

2. `Homomorphism`, that can morph one structure into another. In this case, there is a homomorphism from `Maze` (mazes with cell divider walls) to `ThickMaze` (mazes where walls occupy whole cells). This map is one-to-one or _injective_, meaning that no two `Maze`s map to the same `ThickMaze`. There is no reverse homomorphism, though, from general `ThickMaze` to `Maze`.

This means that any algorithm used to generate `Maze`s can also be used to generate `ThickMaze`s.

# What will Likely Not be Offered

Generation for non-perfect maze algorithms (apart from cellular automata), braid mazes, etc.

# Requirements

- A C++17 capable compiler (unless you want to go through, nest the namespaces, and remove the variable binding, e.g. `auto [a,b] = <some pair expression>`; however, I suggest you upgrade your GCC to the 7.x branch instead.

- Boost 1.31 or higher (for `disjoint_sets.hpp`); and

- CMake 3.0 or higher.

# Further work

1. ~~Include more maze generation algorithms.~~ I'm not sure there are any more well-known algorithms. Suggestions are welcome.

2. Add the ability for listeners to subcribe to `MazeGenerator` instances to receive events when a maze is extended. This will allow, say, drawing of a maze as it is being generated to show how the algorithms work.

3. Add testing with: https://github.com/catchorg/Catch2

4. CI: configure Travis.

5. Add various maze solvers, and also make them subscribable so that it is possible to draw, step-by-step, the path taken through the maze.

5. Write a Qt UI (as a binary independent of the library) to display all these features (maze step-by-step generation, maze step-by-step solving, etc) in a visually pleasant way.

6. Be able to somehow serialize mazes, preferably in JSON?

# Example

Here is an example of a width 50, height 40 maze generated by the randomized DFS algorithm:

```
┌───────┬───────────┬───────────┬───────────┬───┬───────────┬─────┬─────────┬─────────┬───────┬─────┐ 
│ ╶─┐ ╷ ╵ ┌───┬─╴ ┌─┘ ╷ ╶─┬─────┘ ┌───┐ ┌─┐ ╵ ╷ │ ╶─┐ ┌───┐ ╵ ┌───┘ ╷ ┌───╴ │ ╷ ╶─┬─╴ │ ┌─┬─╴ │ ┌─┐ │ 
├─┐ │ └─┬─┘ ┌─┘ ┌─┘ ┌─┼─╴ │ ╶───┬─┘ ╷ │ │ └───┘ └───┘ │ ╷ ├───┤ ╶───┤ └─────┘ ├─╴ │ ╶─┘ │ │ ╶─┤ │ ╵ │ 
│ ╵ └─┐ │ ╷ │ ╶─┤ ┌─┘ │ ╶─┴───┐ ╵ ┌─┤ │ └───┐ ┌─────┐ │ │ ╵ ╷ └─╴ ┌─┴─────┐ ╶─┤ ┌─┴───┬─┘ └─┐ │ ├───┤ 
│ ┌─┬─┘ │ ├─┴─┐ │ ╵ ╷ └─┬─┐ ╶─┴───┘ │ ├───┐ └─┤ ┌─┐ ├─┘ └─┬─┴─────┘ ┌───┐ │ ┌─┘ │ ╶─┐ │ ┌─╴ │ │ ╵ ╷ │ 
│ │ │ ┌─┤ ╵ ╷ ╵ ├─┬─┴─┐ │ └───┬─╴ ┌─┘ ├─╴ └─┐ ╵ ╵ │ ╵ ┌───┘ ┌───────┘ ╷ ╵ └─┘ ┌─┴─╴ │ │ ├───┤ └─┐ │ │ 
│ │ │ │ │ ╶─┴───┘ │ ╷ │ └─╴ ┌─┘ ┌─┘ ┌─┘ ┌─┐ └───┬─┴─┬─┘ ╷ ┌─┴─┐ ┌───┬─┴─┬─╴ ┌─┘ ┌───┤ │ ╵ ╷ └─╴ │ │ │ 
│ ╵ │ │ ╵ ┌───────┘ │ └─┬─╴ │ ╶─┘ ┌─┘ ┌─┘ └───┐ │ ╷ ╵ ╷ ├─┘ ╷ ├─┘ ╷ ╵ ╷ └─┐ │ ┌─┘ ╷ │ ├───┴───┐ │ │ │ 
│ ┌─┘ │ ╶─┤ ┌───────┤ ╷ │ ╶─┼─────┘ ┌─┘ ╷ ┌─╴ │ │ └─┬─┘ │ ┌─┤ ╵ ╶─┼───┤ ╷ │ │ └───┤ │ │ ┌───╴ │ └─┘ │ 
│ │ ┌─┴───┤ └─┐ ╷ ┌─┘ ├─┴─╴ │ ╷ ┌───┴─┐ │ └─┐ │ └─┐ └─┐ │ │ └─┬───┘ ╷ └─┘ ├─┴───╴ ╵ │ ╵ │ ╶───┼───╴ │ 
│ │ ╵ ┌─╴ ├─┐ └─┤ │ ╶─┤ ╶───┴─┤ ╵ ┌─╴ ├─┴─┐ └─┴─╴ ├─╴ │ │ │ ╶─┤ ╶─┬─┴─┬───┘ ╶─┬─────┴─┐ ├───┐ │ ╶───┤ 
│ ├───┘ ╷ ╵ ├─┐ ╵ ├─╴ ├─────┐ │ ╶─┤ ╷ │ ╷ │ ╶─────┤ ┌─┘ │ └─╴ └─┐ ╵ ╷ ╵ ╶─┬───┴─────╴ │ └─┐ │ └─┬─╴ │ 
│ │ ┌───┴─┐ │ └─┐ │ ╶─┤ ┌─┐ │ ├─╴ │ └─┘ │ └─┬───┐ │ └─┐ └───┐ ┌─┴───┤ ┌─┬─┘ ┌───────┬─┴─┐ ╵ ├─╴ ├───┤ 
├─┘ │ ╶───┘ ╵ ╷ │ └─┐ │ │ │ │ └───┴─┐ ╶─┴─┐ │ ╶─┘ └─┐ │ ┌───┘ │ ┌─┐ │ ╵ │ ┌─┘ ┌─┐ ┌─┘ ╷ └─┐ │ ╶─┘ ╷ │ 
│ ┌─┘ ┌───────┴─┴─┐ │ ╵ │ │ └───┬─╴ ├───┐ │ └───────┤ └─┘ ┌───┘ │ │ └───┤ │ ╶─┘ │ ╵ ┌─┤ ╶─┘ └─┬───┤ │ 
│ │ ╶─┤ ╶───┬─┬─╴ │ └─┬─┘ │ ╶─┐ │ ╶─┘ ╷ │ ├─────┬─┐ └─┬───┘ ┌───┤ └───┐ ╵ ├───┐ ├───┘ └─────┬─┘ ╷ ╵ │ 
│ └───┴───┐ │ │ ╷ ├─╴ ╵ ╷ ├─╴ │ └─┬───┤ │ ╵ ┌─╴ ╵ └─┐ │ ╶─┬─┘ ╷ └───╴ └─┬─┤ ╷ │ ├─────╴ ╷ ┌─┘ ┌─┼─╴ │ 
│ ┌─────┐ ╵ │ │ └─┤ ╶─┬─┴─┘ ┌─┴─╴ └─╴ │ └─┬─┘ ┌─────┘ ├─╴ │ ┌─┼───────┐ │ ╵ │ ╵ │ ┌─────┤ │ ┌─┘ │ ╶─┤ 
│ └─╴ ╷ └───┤ ├─╴ ├─╴ │ ╶───┴───┬─┬─╴ ├─╴ ├───┘ ┌───┐ │ ╶─┘ │ ╵ ╶─┐ ╶─┤ └─┐ └───┘ │ ┌─┐ ╵ │ │ ╶─┴─╴ │ 
├───┬─┴───┐ ╵ │ ╶─┤ ┌─┴───┬─┬─╴ │ ╵ ╶─┤ ┌─┘ ╶───┤ ╶─┤ ├───┬─┴───┐ └─┐ └─┐ └─┬─────┤ │ └───┤ └─┬─────┤ 
│ ╷ │ ╶─┐ └─╴ ├─┐ ╵ │ ╶─┐ ╵ │ ╶─┴─┬───┤ └───┬─╴ │ ╷ ╵ │ ╶─┘ ╷ ╷ └─┬─┘ ╷ └─┐ └─┐ ╷ │ └─┐ ╷ ├─╴ │ ╶─┐ │ 
│ │ └─╴ ├─────┘ └───┴─┐ └─┐ ├───┐ ╵ ╷ ├───┐ ╵ ┌─┤ └───┴─────┘ ├─┐ │ ┌─┴───┴─┐ │ └─┴─╴ ╵ │ │ ╷ ├─╴ │ │ 
│ ├─────┴───╴ ┌─────┐ │ ╷ └─┤ ╷ └───┤ ╵ ╷ └───┘ ├─────────────┤ ╵ │ ╵ ┌─╴ ╷ │ └───────┐ ├─┘ ├─┘ ┌─┘ │ 
│ └─┐ ┌───────┤ ╶─┐ ╵ │ └─┐ ╵ ├───┐ ├───┴───┐ ╶─┘ ╷ ┌───┬───╴ │ ┌─┴───┤ ┌─┘ └───────┐ ├─┘ ╷ │ ┌─┤ ╶─┤ 
├─┐ ╵ │ ┌───┐ └─┐ └─┬─┘ ┌─┴─┐ ╵ ╷ │ ╵ ╷ ╶─┐ ├─────┤ │ ╶─┤ ╶───┤ ╵ ┌─┐ ╵ ├───────┬───┘ │ ╶─┴─┘ ╵ ├─╴ │ 
│ └───┘ │ ╷ └─╴ ├─╴ │ ╶─┤ ╷ ├───┤ └───┴─┐ │ ├─╴ ╷ │ │ ╷ └─╴ ┌─┴───┘ ╵ ┌─┘ ┌───┐ ╵ ┌───┴─────────┘ ┌─┤ 
│ ┌─┐ ╶─┤ └─┬───┘ ┌─┘ ┌─┘ │ │ ╶─┴─╴ ┌───┘ │ │ ╶─┴─┤ │ └─┬───┘ ┌───────┘ ┌─┘ ╷ └───┘ ┌─────────┬───┘ │ 
│ │ ├─╴ │ ┌─┘ ┌───┴───┘ ┌─┤ └───┬───┤ ┌───┤ └───╴ │ ├─╴ │ ┌───┤ ┌───────┴─┐ └───┐ ╶─┤ ╷ ┌───┐ │ ╶─┐ │ 
│ │ │ ╶─┤ │ ╶─┴───╴ ┌───┘ └───┐ ╵ ╷ ╵ │ ╷ └─────┐ │ │ ┌─┘ ├─╴ │ ├─────╴ ╷ ├───╴ ├─╴ │ │ │ ╷ │ ├─╴ │ │ 
│ │ └─┐ └─┴─────┬───┘ ╶───┬─┐ ├───┴───┴─┘ ╷ ┌───┤ │ │ │ ┌─┘ ╷ │ ╵ ┌─┬───┤ └─┬───┤ ┌─┘ ├─┘ │ └─┤ ┌─┘ │ 
│ ╵ ╷ ├───┐ ┌─╴ │ ╶─┬───┐ │ ╵ └─╴ ┌───────┴─┘ ╷ ╵ │ └─┘ │ ╷ │ └─┬─┘ ╵ ╷ └─┐ │ ╷ │ └─┐ ╵ ┌─┴─┐ ╵ │ ╶─┤ 
├───┤ └─╴ │ └─┐ ├─╴ ├─╴ │ └───────┘ ┌─────────┼───┴─────┴─┘ ├─┐ │ ╶───┼─╴ │ ╵ │ └─┐ │ ╶─┴─╴ └─┬─┴─┐ │ 
├─╴ ├───┐ └─┐ │ │ ┌─┘ ╷ └───┬─────┬─┴─────┐ ╷ └───┐ ┌───┬───┘ │ └───┐ │ ╷ └───┼─╴ │ └─┬─┬───╴ └─╴ │ │ 
│ ┌─┘ ╷ ╵ ┌─┘ │ │ ╵ ┌─┴───┐ │ ╶───┘ ╷ ┌─┐ ╵ ├───┐ ╵ │ ┌─┘ ╷ ╶─┴───┐ │ │ │ ┌───┘ ┌─┴─╴ │ ╵ ┌───────┘ │ 
│ ╵ ┌─┴───┘ ┌─┴─┴───┘ ┌───┤ │ ┌─────┘ │ └───┘ ┌─┴───┤ ╵ ┌─┴───┐ ┌─┘ │ │ └─┤ ┌───┤ ╶───┴───┤ ┌─────┐ │ 
│ ╶─┤ ╷ ┌───┘ ┌───┐ ╶─┤ ╷ │ └─┤ ┌─────┘ ╶─────┤ ╷ ╶─┘ ┌─┘ ┌─╴ │ │ ╶─┴─┤ ╷ │ │ ╶─┘ ┌─┬───╴ │ │ ┌─┐ ╵ │ 
├───┘ │ │ ╶───┘ ╷ ├─┐ ╵ │ └─╴ │ └───┐ ╶───┬─┐ │ ├─────┴─╴ │ ╶─┤ └─┬─┐ └─┤ │ │ ╶───┘ │ ┌───┘ │ │ └───┤ 
│ ╶───┴─┴───┬───┤ ╵ └───┴─┬───┘ ┌─╴ ├───┐ │ │ ╵ │ ╶─────┬─┴─┐ │ ╷ ╵ └─┐ ╵ │ └─┬───┬─┘ │ ╶─┐ │ ├───╴ │ 
│ ┌───┐ ┌─╴ │ ╷ ├───────┐ └─────┤ ┌─┘ ╷ │ ╵ ├───┴───┬─╴ │ ╶─┘ │ │ ┌───┴─┐ └─┐ ╵ ╷ ╵ ┌─┴─╴ │ │ ╵ ┌─┐ │ 
│ ╵ ╷ │ └───┘ │ ╵ ╶─┐ ╶─┴─────╴ ╵ │ ╷ └─┴─╴ └─╴ ┌─╴ ╵ ┌─┘ ╶───┤ └─┘ ╷ ╶─┴─╴ └───┴───┴─────┘ ├───┘ ╵ │ 
└───┴─┴───────┴─────┴─────────────┴─┴───────────┴─────┴───────┴─────┴───────────────────────┴───────┘ 
```

# References

1. http://www.astrolog.org/labyrnth/algrithm.htm

2. https://en.wikipedia.org/wiki/Maze_generation_algorithm

3. https://en.wikipedia.org/wiki/Maze_solving_algorithm

4. http://weblog.jamisbuck.org/2010/12/29/maze-generation-eller-s-algorithm

5. Jamis Buck. Mazes for Programmers. The Pragmatic Programmers, July 2015. http://mazesforprogrammers.com
