===============
Build Instructions
===============

- play/pause at any time using the `p` key.

- quit at any time using `q` key or `ctl+c`.

- Use `1`, `2`, `3`, `4` keys to adjust speed in an inceasing fashion.

- To save currently displayed pattern, press `s` key. Then enter a filename in the prompt that will appear on the top of the scrreen & press `enter`. (Use `-` key for backspace).

- To load a pattern from a file, invoke the program with filename as a command line argument. ex `$ ./gol patterns/glider_gun`.

- To edit the current pattern:
  
  - pause the simulation (using `p` key) & a cursor will appear (marked by symbol `O`). 
  
  - You can move the cursor up, down, left, and right using `i`, `k`, `j`, `l` keys respectively. 
  
  - To toggle the state of a cell (from alive to dead or vice-versa), move the cursor to that cell and use `space` key.
