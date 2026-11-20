#include "mind.h"

int main(void)
{
    Game game;
    puts("Mind Reader");
    printf("Think of a whole number from %d to %d. Keep it secret!\n",
           MIND_MIN, MIND_MAX);
    puts("Tell me whether your number is higher, lower, or correct (h/l/c).\n");

    Result result = mind_play(stdin, stdout, &game);
    putchar('\n');

    switch (result.kind) {
    case RESULT_FOUND:
        printf("I found %d in %u %s!\n", result.value.secret, game.count,
               game.count == 1 ? "guess" : "guesses");
        break;
    case RESULT_INCONSISTENT:
        puts("The hints are inconsistent. No number fits all of them.");
        break;
    case RESULT_ABORTED:
        puts("Input ended. The round was stopped.");
        break;
    }

    if (mind_append_log("log.txt", &game, result) == 0) {
        puts("Round saved to log.txt.");
    } else {
        fputs("Could not save the round to log.txt.\n", stderr);
    }

    return 0;
}
