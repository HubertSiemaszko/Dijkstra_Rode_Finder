# Mountain Route Finder

A simple C++ program that finds the fastest route in the mountains using **Dijkstra's algorithm**.

## How it works
The program calculates the minimum time needed to get from point A to point B on a 2D map.
* **Walking:**
  * Going uphill takes more time (calculated based on height difference).
  * Going flat or downhill takes 1 minute per square.
* **Ski Lifts:**
  * You can use ski lifts to move quickly between points.
  * The program accounts for **travel time** and **waiting time** (lifts depart at specific intervals).

## Input Format
The program reads from standard input:
1. Map dimensions (Width, Height).
2. Start and End coordinates.
3. Number of ski lifts.
4. List of lifts (Start X/Y, End X/Y, Travel Time, Departure Interval).
5. The map grid (heights).
