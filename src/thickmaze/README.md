# ThickMaze

A `ThickMaze` is a maze where the walls are not simply cell dividers, but actually take up a cell themselves, i.e. each entry in the `ThickMaze` is either `FLOOR` or `WALL`. This representation allows the `ThickMaze` library to use some unique algorithms that would not be possible for the [`Maze`](../maze/README.md) library:

1. [Cellular Automata](#cellular-automata)
2. [Grid Colourings](#grid-colourings)

Additionally, all algorithms that can be used to generate `Maze`s can also be used, via homomorphism, to generate `ThickMaze`s. See the [Typeclasses](#typeclasses) section below for more information.

## Cellular Automata

`ThickMaze`s allow us to implement maze-generating _cellular automata,_ as described here:

https://en.wikipedia.org/wiki/Cellular_automaton

These are algorithms that generate a random grid of floor and walls, and then for a time `t`, use certain rules based on the contents at time `t-1` to determine the layout. They are known to converge to relatively stable configurations quite quickly, but seldom produce connected (and thus, not perfect) mazes. They are still interesting to study, and algorithms such as [braiding](../../README.md#braiding) can post-modify such mazes to increase the likelihood that they be connected.

The state of a cell at time `t` is determined by three parameters, each of which will be described below:

1. Its state at time `t-1`;
2. The neighbourhood of the cell at time `t-1`; and
3. The cellular automaton rule chosen.

The state of a cell is a binary parameter, where `true` means the cell is alive (i.e. a wall), and `false` means that the cell is not alive (i.e. a floor space).

The neighbourhood of a cell comprises the state of the cells around a given cell. The library provides implementations the two most common neighbourhood types:

1. The Moore neighbourhood, which consists of the eight squares surrounding a cell; and
2. The von Neumann neighbourhood, which also has size eight but instead consists of two tiles in each of the four cardinal directions, i.e. north, south, east, and west.

Users may additionally plug in their own customized neighbourhood functions.

A cellular automaton ruleset determines the contents of a cell at time `t` based on its state and the state of its neighbours at time `t-1`. It is usually written <code>Bb<sub>1</sub>b<sub>2</sub>...b<sub>m</sub>/Ss<sub>1</sub>s<sub>2</sub>...s<sub>n</sub></code>, with the following significance:

1. If a cell's `t-1` state is not alive and the cell had <code>b<sub>i</sub></code> neighbours at time `t-1`, then the cell is _born,_ i.e. the state changes to alive at time `t`.

2. If a cell's `t-1` state is alive and the cell had <code>s<sub>j</sub></code> neighbours at time `t-1`, then the cell _survives_, i.e. the state remains alive at time `t`.

3. Otherwise, the cell _dies_ or remains not alive at time `t`.

The current automata ruleset offered by default include:

1. `B2/S123`, which seems to show the most promise and was proposed at [1](https://english.rejbrand.se/rejbrand/article.asp?ItemIndex=421);
2. Maze aka `B3/S12345`, which performs quite poorly;
3. Mazectric aka `B3/S1234`, which produces maze-like passages;
4. Vote aka `B5678/S45678`, which produces mazes with large, cavernous spaces; and
5. Vote45 aka `B4678/S35678`, which, at a small-scale, results in mostly empty space, but on larger scales, may result in cavernous spaces.

Users may define their own cellular automata rulesets.

The grid is initialized at random, with a user-supplied probabiity (default value 0.5) of each cell starting off alive. The algorithm runs until one of two conditions are met:

1. A user-supplied maximum number of generations have passed (default value 10,000); or
2. The grid reaches a _relatively stable_ state, i.e. for a user-supplied parameter `N`, the grid repeats one of the previously seen `N` states. This indicates that the cellular automaton has either stabilized to a single state, or has reached a convergence where it cycles between a small number of states.

Here is a width 100, height 50 `ThickMaze` generated by the cellular automaton with using the Moore neighbourhood and ruleset `B2/S123`, displayed as a screenshot to reduce row interspacing. You can see that the maze is neither perfect (i.e. it has loops), nor connected (i.e. there are parts of the maze that you can't reach from other parts).

![image here](../../examples/b2s123.png)


## Typeclasses

For the definition of homomorphisms and related concepts, see [Homomorphisms](../../README.md#homomorphisms) on the main page.

There is a homomorphism (specifically, a monomorphism) from regular `Maze`s to `ThickMaze`s. In other words, every `Maze` can be represented as a `ThickMaze` with the same structure, but not the converse, i.e. there are `ThickMaze`s that cannot be represented as `Maze`s.

This is useful, as it means that every algorithm that generates `Maze`s can also generate `ThickMaze`s via the homomorphism.

Here's an example of a 25 by 20 randomizd depth-first search `Maze`:

```
┌───────┬───────────────────┬─────────┬─────┬─────┐ 
│ ╶─┬─┐ └───────────╴ ┌───╴ │ ╶─────┐ ╵ ╶─┐ ╵ ╶─┐ │ 
│ ╷ │ └─╴ ┌─────┐ ┌───┤ ╶─┬─┘ ┌─────┤ ┌───┼───┬─┘ │ 
├─┘ ├─────┘ ┌─┐ └─┘ ╷ └─┐ └───┘ ┌─┐ └─┤ ╷ ╵ ╷ ╵ ╷ │ 
│ ╶─┤ ╶─────┘ └─┬───┴─┐ ├───┬───┘ └─┐ │ ├───┴─┬─┘ │ 
│ ╷ └─────┬───╴ │ ╷ ┌─┘ │ ╶─┘ ╶─┐ ╶─┘ ╵ │ ┌─╴ │ ╶─┤ 
│ │ ╶─┬─╴ │ ╶───┤ │ │ ╶─┴─┬─────┼───┬───┤ └─┬─┴─┐ │ 
│ ├─┐ │ ┌─┴───┐ │ │ └───┐ │ ╶─┐ ╵ ╷ └─┐ ╵ ╷ │ ╷ ╵ │ 
│ │ │ │ ╵ ┌─╴ │ ╵ ├─╴ ┌─┘ └─┐ ├───┴─┐ └───┤ ╵ ├───┤ 
│ ╵ │ ├─┬─┘ ┌─┴───┤ ┌─┤ ┌───┘ │ ┌───┴───╴ │ ┌─┴─┐ │ 
├───┘ │ ╵ ╶─┴───╴ │ ╵ │ │ ╶───┘ │ ┌─┬─────┤ ╵ ╷ ╵ │ 
│ ┌───┴───────┬─╴ │ ┌─┘ ├───┬─╴ │ ╵ ├───╴ ├───┴─╴ │ 
│ ╵ ╶───┬───╴ ├───┘ │ ╷ ╵ ┌─┘ ┌─┴─┬─┘ ╶───┘ ┌─────┤ 
├───┬───┘ ┌─╴ │ ┌───┤ ├─╴ │ ┌─┘ ┌─┘ ┌───┬───┴─╴ ╷ │ 
│ ╷ ╵ ┌───┤ ┌─┘ │ ╷ │ └───┘ │ ╶─┘ ┌─┘ ╷ ╵ ╷ ┌───┤ │ 
│ └─┬─┘ ╷ ╵ │ ┌─┘ │ └───────┤ ╶───┴───┤ ╶─┤ │ ╶─┤ │ 
│ ╷ └─┬─┴─┐ │ │ ┌─┴───────╴ ├───────┐ │ ┌─┘ ├─╴ │ │ 
│ ├─┐ │ ╷ └─┤ ├─┘ ┌─╴ ┌───┐ │ ┌─┐ ╶─┘ ├─┘ ┌─┘ ╷ │ │ 
│ ╵ │ ╵ ├─╴ │ │ ╷ ├───┘ ╷ └─┤ ╵ └─────┘ ┌─┴─╴ │ │ │ 
├─╴ └───┤ ╶─┘ │ └─┘ ╶───┴─┐ └───────────┘ ╷ ╶─┴─┘ │ 
└───────┴─────┴───────────┴───────────────┴───────┘ 
```

Here is the `ThickMaze` with the same structure as obtained by the homomorphism:

![image here](../../examples/thickify_example.png)

Additionally, there is `Show` typeclass for `ThickMaze`s that was used to generate the string representation as seen in the previous example.

## Grid Colourings

## Citations

1. [Cellular automaton maze generation - Andreas Rejbrand’s Website](https://english.rejbrand.se/rejbrand/article.asp?ItemIndex=421)

[Back to top](#thickmaze)