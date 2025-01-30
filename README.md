# Dungeon Generation

Project to generate dungeon using Delaunay Triangulation and Prim's algorithm for creating a minimum spanning tree.

![image](https://hackmd.io/_uploads/HkEAY_FdJl.png)

![image](https://hackmd.io/_uploads/B1nf5OF_1g.png)

## Config

In MyGenerator there is multiple variable exposed to change the generation:

- RoomClasses: List of classes used for generation
- MinimumRooms: Number of rooms used for triangulation (Not the final number of rooms)
- MaximumRooms: Number of rooms used for generation (Not the final number of rooms)

-EllipseHeight/EllipseWidth: Range for initial spawn

## Improvement Points:

- Use real assets
- Add true hallway generation(maybe with pathfinding)
- Handle room layouts better (currently it works better with square room of same size)

## Sources:

[Circum Circle formulas](https://en.wikipedia.org/wiki/Circumcircle#:~:text=In%20geometry%2C%20the%20circumscribed%20circle,radius%20is%20called%20the%20circumradius.)

[Prim's algorithm understandings](https://www.youtube.com/watch?v=Uj47dxYPow8)

[Delaunay Triangulation](https://www.gorillasun.de/blog/bowyer-watson-algorithm-for-delaunay-triangulation/)

[Procedural Dungeon Generation](https://web.archive.org/web/20160412103212/http://www.gamasutra.com/blogs/AAdonaac/20150903/252889/Procedural_Dungeon_Generation_Algorithm.php)
