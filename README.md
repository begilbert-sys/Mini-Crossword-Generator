# Mini Crossword Generator
<img src="https://i.imgur.com/one4z3k.gif">
*Randomly generating a 7x7 in 0.4 seconds*
The [Mini Crossword](https://www.nytimes.com/crosswords/game/mini) is a crossword variant where the boards are much smaller, around 5x5. This project randomly generates unique mini crossword boards.

Generated boards can be customized, pre-filled with letters, and are guaranteed to have no repeated words. 

## How to run
As of right now, this program only works in the CLI, but I'm working on an [in-browser](https://github.com/begilbert-sys/make-your-own-crossy) version using TypeScript. 

The program only generates alphabetical words between lengths 3 and 8. Anything else will be ignored. 

It requires two files:
1. `data/wordbank.txt` - a list of all the words you'd like to use for generation. I've provided one by default. 

2. `data/board.txt` - what the board should look like. Blacked-out squares are `.`, blank squares are `_`, and letters must be lowercase. Take the following `board.txt` file:
```
. . _ _ _
_ _ _ _ _
m a r i o
_ _ _ _ _
_ _ _ _ _
_ _ _ . .
```
This would correspond to the follwing board:

<img src="https://i.imgur.com/9xDWliu.png" height=250>


## How it works (general overview)
The project relies on [tries](https://en.wikipedia.org/wiki/Trie) to procedurally generate words on the board, and [backtracks](https://en.wikipedia.org/wiki/Backtracking) if a generated board is invalid. The board is generated letter-by-letter using the following process:

1. Each filled square keeps two sets of possible "next letters": one for the next letter in the **across** direction and one for the next letter in the **down** direction. 
    ```
          s
          e
          e
    c o r *
    ```
    In this case, the possibilities might be the following:

    **Down**: SEE**D**, SEE**K**, SEE**M**, SEE**N**, SEE**P**

    **ACROSS**: COR**D**, COR**E**, COR**K**, COR**N**, COR**P**,

    Note that if there is no set to choose from because the square is adjacent to a border, then the set would consist of every letter in the alphabet. 


2. The square takes the intersection of these two sets:

    <img src="https://latex.codecogs.com/svg.image?\large&space;\left\{D,K,M,N,P\right\}\cap\left\{D,E,K,N,P\right\}\rightarrow\left\{D,K,N,P\right\}"><br>
    An element is arbitrarily chosen from the resulting set and assigned to the square:
```
      s
      e
      e
c o r D
```
3. The algorithm defers to the next square and waits to hear back. If a board is successfully generated after this point, great. However, if board generation fails, simply try the next letter in the set:
```
      s
      e
      e
c o r K
```
and defer again onto the next square. 

4. If all letters in the set fail to generate boards, then the function returns `false` and backtracks to the previous square.


This is just the gist, and there are quite a few things done to optimize generation, which are explained in the source code itself. 

## Limitations 
Once the algorithm reaches the 7x7 and 8x8 range, it begins to get very slow. Generating a 7x7 board with a good wordbank can take up to a minute. 
