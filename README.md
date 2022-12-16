How it was made data oriented:
Data members of game objects were separated into arrays, tried to get rid of as many classes as possible while still trying to have any sense of possible design on top left and avoiding the supposed monolithic design of data orientation.
The game loop updates game objects en-masse while only loading in relevant information for the operation.
Game objects are allocated in chunks to avoid multiple expansions of containers.

Collision check is partially commented out due to running out of time to fix a bug that cropped up last second. A week was definitively not enough time for this assignment, but that's what we got after being introduced to data orientation..