# Mind Reader in C

Think of a whole number from **1 to 100** and keep it secret. The computer
will find it in **at most seven guesses** if you keep the same number and
answer accurately.

- **higher** or **h** means your number is greater than the current guess.
- **lower** or **l** means your number is less than the current guess.
- **correct** or **c** confirms the guess.

The program accepts uppercase and lowercase replies. Invalid input prompts you
again on the same guess. If all candidates are ruled out, the program reports
that the hints are inconsistent. Launch the game again for another round.

## Windows

Clone this repository and open PowerShell in its folder:

~~~powershell
git clone https://github.com/shaswatpandeyofficial-art/mindreader.git
cd mindreader
~~~

Build with GCC from MinGW, then start the game:

~~~powershell
gcc -std=c11 -O2 -Wall -Wextra -Wpedantic main.c mind.c -o mind.exe
.\mind.exe
~~~

If GCC is installed in `C:\MinGW` and is absent from `PATH`, replace `gcc` in
that command with `& 'C:\MinGW\bin\gcc.exe'`.

## Linux or WSL

Clone this repository and build with GCC and Make:

~~~sh
git clone https://github.com/shaswatpandeyofficial-art/mindreader.git
cd mindreader
make
./mind
~~~

For a clean rebuild:

~~~sh
make clean
make
./mind
~~~

The `Makefile` compiles `mind.c` as the shared library `libmind.so` and links
`main.c` to it with `$ORIGIN` so the loader looks next to the executable.
The Windows GCC command compiles both C files into one executable.

## What the C code demonstrates

| Concept | Where it appears |
| --- | --- |
| Macros and a header guard | `mind.h` defines the bounds, guess limit, and public interface. |
| Structures | `Range`, `Turn`, and `Game` group related state. |
| Arrays and strings | `history` keeps the accepted turns. `line` holds input text. |
| Pointers | `Game *` lets `mind_play` update the caller's game. `Range *` points to its bounds. |
| Enums and a union | `ResultKind` tags `ResultValue`, which holds the correct number or the remaining range. |
| Loops, branching, and functions | The search loop computes a midpoint and `switch` updates the range. |
| File I/O | The round uses `FILE *` streams. `fopen`, `fprintf`, and `fclose` append the result. |
| Separate compilation | `main.c` supplies the terminal interface and `mind.c` implements the game and logging. |

## Why seven guesses suffice

Each wrong guess leaves at most half the candidates, rounded down. Starting
with 100 numbers, the worst-case counts after six misses are 50, 25, 12, 6,
3, and 1. Guess seven checks that final candidate.

Every round appends its accepted turns and outcome to `log.txt` in the
directory where you launched the game. See `sample-log.txt` for a round in
which the secret is 100.
