# Week 9 - Lists

## Lists.c

All automated tests complete. No mention of any memory leaks when ran.

## Dungeons.c

### Cellular Automata

A cellular automata (like Conway's Game of Life) is used, along with customisable birth, death, and starting condition rules, to generate a cave like dungeon. A recursive floodfill algorithm is then used to check the size of cave regions. If a cave region is smaller than 100 cells it is filled in. Any unconnected regions are connected to each other to ensure if the dungeon was used in a game that the player could visit every location.

### Rooms and Mazes

My rooms and mazes is based on Bob Nystrom's article found at http://journal.stuffwithstuff.com/2014/12/21/rooms-and-mazes/. One difference between the two ideas is that I generate the maze first and place the rooms on top of it, whereas the article proposes generating rooms and filling the space between them with mazes. I think Bob's way looks better, but it takes longer to generate and is more complicated. My way still provides a fully connected maze with winding corridors.

For generating my mazes I'm using an edited version of the extra work I submitted in week 6 to make it more suitable for this project.