# Console Adventure Game

## Project Overview

In this project, you will progressively develop a small **turn-based adventure game in C++**, running in a console using **ncurses** (bonus).

The game world is represented by at least one array and several struct.

The player controls a hero who explores the world, encounters enemies, takes damage, casts spells, and progresses through several levels.

The project is divided into eight steps. Each step introduces a new gameplay or engine concept and builds upon the previous one.

# 1 — Represent and display the map

Create the first version of the game world.

The map must contain exactly:

* **20 columns**
* **10 rows**

Each cell represents a type of terrain:

* `.` — Basic ground
* `~` — Water
* `A` — Mountain

At this stage, the map a content can be hard-coded directly in the C++.

### Example

The following is a valid **20-column × 10-row** map:

```text
....................
.....~~~~...........
.....~~~~...AAA.....
....................
....................
....................
..AAA...........~~..
....................
....................
....................
```

Every line contains **20 characters**, and the map contains **10 lines**.

The program must display the complete map in the terminal.

### Requirements

* Create a data structure representing the map.
* Store the 20 × 10 cells.
* Display every cell.
* Initialize and use ncurses (bonus).
* Keep the map representation separate from the rendering code.


# 2 — Add the Hero and the movements

Add the hero, represented by:

```text
H
```

The hero must have a position on the map.

The player must be able to move the hero using the keyboard.


* `Z` — move up
* `S` — move down
* `Q` — move left
* `D` — move right

The hero must remain inside the **20 × 10** map.

### Example

Initial state:

```text
....................
.....~~~~...........
.....~~~~...AAA.....
....................
H...................
....................
..AAA...........~~..
....................
....................
....................
```

After moving the hero one cell to the right:

```text
....................
.....~~~~...........
.....~~~~...AAA.....
....................
.H..................
....................
..AAA...........~~..
....................
....................
....................
```

The terrain underneath the hero must remain part of the map.

The `H` should represent the hero's current position, rather than permanently replacing a map cell.

The screen must be refreshed after each player action (create a new one if you don't clear the console).

### Game Loop

The game should now follow the basic structure:

```text
        INPUT
          ↓
        UPDATE
          ↓
        RENDER
```


# 3 — Add Enemies and a Life / Damage System

Add enemies represented by:

```text
X
```

The map must contain **5 enemies**.

Enemies have their own positions.

The hero now has one life point and die if he reaches a cell occupied by an enemy.

Add a *Game Over* "screen".

### Example

A possible game state:

```text
....................
.....~~~~...........
..X..~~~~...AAA.....
....................
H..........X.........
....................
..AAA......X....~~..
....................
........X.......X....
....................
```

There are five enemies on the map.

When the hero moves onto an enemy's position, a collision occurs.

The game must detect that the hero and an enemy occupy the same cell.

The result is:

```text
Hero + Enemy
     ↓
  Collision
     ↓
   Damage
     ↓
   0 lives
     ↓
  Game Over
```


### Requirements

Implement:

* enemy positions.
* hero life.
* "collision detection".
* damage.
* Game Over.

Collision detection must use the actual positions stored by the game.


# 4 — Make the enemies move

Enemies must now be able to move.

The game becomes **turn-based**.

After the player performs an action, the enemies perform their actions.

For the first implementation, an enemy can randomly choose one of the four directions:

```text
        UP
         ↑
         |
LEFT ←  X  → RIGHT
         |
         ↓
       DOWN
```

The movement must be validated before being applied.

Enemies cannot:

* leave the map.
* move onto invalid cells.
* move through terrain that is not walkable.


### Game Loop

The game loop now becomes:

```text
Player Input
     ↓
Hero Update
     ↓
Collision Check
     ↓
Enemy Update
     ↓
Collision Check
     ↓
Render
```

# 5 — Add a level exit and multiple levels

Add a special cell represented by:

```text
E
```

When the hero reaches this cell, the current level is completed.

The game must then load a new map (hard coded) without restarting the program.

Each level should be able to define its own:

* terrain.
* hero starting position.
* enemies.
* exit.

# 6 — Create a map file parser

The maps should no longer be hard-coded in the C++ source code.

Create a simple text-file format for storing levels.

### Example

`level01.txt`:

```text
....................
.....~~~~...........
.....~~~~...AAA.....
....................
H...................
....................
..AAA...........~~..
....................
....................
.................E..
```

The parser must read the file and interpret each character.

```text
. → Ground
~ → Water
A → Mountain
H → Hero starting position
X → Enemy
E → Level exit
```

The parser must extract the positions of the different entities from the file.

The project can contain several levels:

```text
maps/
├── level01.txt
├── level02.txt
└── level03.txt
```

The game should be able to load different files without modifying the C++ source code.

### Validation

The parser must verify that:

* the file contains exactly 10 rows.
* every row contains exactly 20 characters.
* every character is a valid map character.
* the map contains a valid hero starting position.
* the map contains a valid level exit.

The parser must detect and display errors, not silently creating an incorrect map.

# 7 — Hero state machine and spell casting

The hero can now perform a more complex action: **casting a spell**.

Implement a **simple finite state machine** to control the hero.

When the player chose to cast the spell, the hero enters a **targeting state**.

The spell has a limited range.

The game must display which cells can be targeted and the one being currently targeted.

### Example

A complete 20 × 10 map with the hero:

```text
....................
.....~~~~...........
.....~~~~...AAA.....
....................
H...................
....................
..AAA...........~~..
....................
....................
.................E..
```

After activating the spell, the valid cells around the hero could be represented conceptually as:

```text
....................
.....~~~~...........
.....~~~~...AAA.....
...***...............
...*H*...............
...***...............
..AAA...........~~..
....................
....................
.................E..
```

The player can then move a targeting cursor across valid cells.

For example:

```text
....................
.....~~~~...........
.....~~~~...AAA.....
...***...............
...*H*...............
...*^*...............
..AAA...........~~..
....................
....................
.................E..
```

The cursor represents the currently selected target.

The spell can only be cast if the selected cell is inside the spell's range.

### Requirements

The targeting system must:

* enter a dedicated targeting state.
* display the spell range.
* allow the player to select a target.
* prevent invalid targets from being selected.
* confirm the target.
* execute the spell.
* return to the appropriate state.

The same input can have different meanings depending on the current state.

# 8 — Enemy detection and pathfinding

The final step introduces basic enemy AI.

Enemies must be able to **detect the hero when he is within a certain distance**.

Define a detection range.

For the hero is outside this range, the enemy continues its normal behaviour.

If the hero enters the detection range, the enemy detects him and attempts to move toward him.


---

## Pathfinding

The enemy must take obstacles into account.

For example:

```text
....................
....................
.......AAA..........
..H....AAA.....X....
.......AAA..........
.......AAA..........
....................
....................
....................
.................E..
```

The mountains block the direct path between the enemy and the hero.

The enemy therefore needs to find a route around them.

A possible path could conceptually look like:

```text
....................
....................
.......AAA..........
..H....AAA.....X....
..↑....AAA.....↓....
..↑............↓....
..←←←←←←←←←←←←←←....
....................
....................
.................E..
```

The exact path depends on the pathfinding implementation.

You should implement a pathfinding algorithm such as **A***.

### Enemy Behaviour

The final enemy behaviour can be represented as:

```text
             Enemy
                │
                ↓
        Is the hero nearby?
           /           \
         NO             YES
         │               │
         ↓               ↓
 Normal movement     Find next cell (A*)
                         │
                         ↓
                    Move Toward
                      the Hero
```


# — Rating and remarks

The final mark is mostly 50/50 between functionalities and code/architecture quality.

The last 2 steps are more complex and represent 1/3 of the points.

Optimizing any step can lead to a few bonus points.

Using ncurses also gives some bonus points.

Messy git repository can lead to negative points.