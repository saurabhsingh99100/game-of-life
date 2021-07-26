=====================
prerequisites
=====================

1. install git & c/c++ compiler

```bash
$ sudo apt install git build-essential
```

2. install ncurses

```bash
$ sudo apt install libncurses5-dev libncursesw5-dev
```

3. clone the repo

```bash
$ git clone https://github.com/saurabhsingh99100/game-of-life.git
```

4. cd to the cloned repo & compile the program using the povided shell script

```bash
$ cd game-of-life
$ ./compile.sh
```

**Note:** You *may* first need to make compile script executable, this can be done by:

```bash
$ chmod +x compile.sh
```