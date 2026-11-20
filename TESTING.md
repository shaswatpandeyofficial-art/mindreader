# Pre-publication checks

Checked on Windows with MinGW GCC 6.3.0 on 21 November 2026.

The source compiled without warnings using:

```sh
gcc -std=c11 -O2 -Wall -Wextra -Wpedantic -Werror main.c mind.c -o mind.exe
```

An external automated harness ran 124 cases against the freshly compiled executable. All passed:

- All 100 secret numbers from 1 through 100; the longest round used seven guesses.
- Short and full replies, mixed case, surrounding whitespace, and CRLF input.
- Empty, unknown, overlong, trailing-text, and embedded-NUL replies; invalid input did not consume a guess.
- Inconsistent higher/lower hints and end-of-input during a round.
- A final valid reply without a trailing newline.
- Appending round logs without overwriting previous rounds.
- Reporting a log-writing failure while still completing the game.

The Linux shared-library build is documented in the README. Linux runtime testing was not performed in this check.
