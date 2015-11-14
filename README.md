# Blocks_World

## Breadth first search
Simple BFS, searching all moves that it can make excluding those where it hits a wall.
I have used this function to find solutions at around 900,000 nodes it takes a very long time!

## Depth first search
Simple DFS, searching all moves that it can make exclusing those where it hits a wall.
This function can find a solution with a depth of around 18 but this also takes a while!

## Checksum functionality
I have created a function that assigns a unique value to each unique board configuration.
There is a vector holding all previous checksums to ensure that paths are cut when a board has already been checked.
This saves us checking the boards' children nodes and thus saves a large amount of computing.

## Breadth first search (with checksum)
This version of the breadth first search uses the vector of checksums to cut paths of nodes already checked.
Give it a value upwards of 10,000 to ensure it will find a solution. This will likely be a lot sooner than 10,000 nodes.

## Depth first search (with checksum)
This version of the depth first search uses the vector of checksums to cut paths of nodes already checked.
Give this function a depth of 30 to 50 to finish in a reasonable time and return a solution.

## Iterative deepening
This incorporates a second checksum vector denoting nodes it has discovered in previous iterations.
It will not check these nodes but still expand the children nodes.
It still cuts paths to nodes whose checksums exist in the global checksum vector.

## A* search


