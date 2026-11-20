#include "mind.h"

#include <ctype.h>
#include <string.h>

static Reply read_reply(FILE *input)
{
    char line[64];
    size_t length = 0;
    int invalid = 0;
    int ch;

    /* Consume the whole line even when it is too long for the buffer. */
    while ((ch = fgetc(input)) != EOF && ch != '\n') {
        if (length == sizeof line - 1 || ch == '\0') {
            invalid = 1;
        } else {
            line[length++] = (char)tolower(ch);
        }
    }

    if (ch == EOF && (ferror(input) || (length == 0 && !invalid))) {
        return REPLY_END;
    }
    if (invalid) {
        return REPLY_INVALID;
    }

    char *word = line;
    while (length > 0 && isspace((unsigned char)line[length - 1])) {
        --length;
    }
    line[length] = '\0';
    while (isspace((unsigned char)*word)) {
        ++word;
    }

    if (strcmp(word, "higher") == 0 || strcmp(word, "h") == 0) {
        return REPLY_HIGHER;
    }
    if (strcmp(word, "lower") == 0 || strcmp(word, "l") == 0) {
        return REPLY_LOWER;
    }
    if (strcmp(word, "correct") == 0 || strcmp(word, "c") == 0) {
        return REPLY_CORRECT;
    }
    return REPLY_INVALID;
}

Result mind_play(FILE *input, FILE *output, Game *game)
{
    *game = (Game){ .remaining = { MIND_MIN, MIND_MAX } };
    Range *range = &game->remaining;

    while (range->low <= range->high && game->count < MIND_MAX_GUESSES) {
        int guess = range->low + (range->high - range->low) / 2;
        unsigned int turn = game->count + 1;
        fprintf(output, "Guess %u/%d: %d\n", turn, MIND_MAX_GUESSES, guess);

        Reply reply;
        do {
            fputs("Higher, lower, or correct? ", output);
            fflush(output);
            reply = read_reply(input);
            if (reply == REPLY_INVALID) {
                fputs("Please enter higher, lower, or correct (h/l/c).\n", output);
            }
        } while (reply == REPLY_INVALID);

        if (reply == REPLY_END) {
            return (Result){ RESULT_ABORTED, { .remaining = *range } };
        }

        game->history[game->count++] = (Turn){ guess, reply };
        switch (reply) {
        case REPLY_HIGHER:
            range->low = guess + 1;
            break;
        case REPLY_LOWER:
            range->high = guess - 1;
            break;
        case REPLY_CORRECT:
            return (Result){ RESULT_FOUND, { .secret = guess } };
        default:
            break;
        }
    }

    return (Result){ RESULT_INCONSISTENT, { .remaining = *range } };
}

int mind_append_log(const char *path, const Game *game, Result result)
{
    static const char *const reply_names[] = {
        [REPLY_HIGHER] = "higher",
        [REPLY_LOWER] = "lower",
        [REPLY_CORRECT] = "correct"
    };

    FILE *log = fopen(path, "a");
    if (log == NULL) {
        return -1;
    }

    fprintf(log, "Mind Reader round (%d-%d)\n", MIND_MIN, MIND_MAX);
    for (unsigned int i = 0; i < game->count; ++i) {
        const Turn *turn = &game->history[i];
        fprintf(log, "  %u. %d -> %s\n", i + 1, turn->guess,
                reply_names[turn->reply]);
    }

    switch (result.kind) {
    case RESULT_FOUND:
        fprintf(log, "Result: found %d in %u guess(es)\n",
                result.value.secret, game->count);
        break;
    case RESULT_INCONSISTENT:
        fprintf(log, "Result: inconsistent hints (low %d, high %d)\n",
                result.value.remaining.low, result.value.remaining.high);
        break;
    case RESULT_ABORTED:
        fprintf(log, "Result: input ended (range %d-%d)\n",
                result.value.remaining.low, result.value.remaining.high);
        break;
    }
    fputc('\n', log);

    int write_failed = ferror(log);
    int close_failed = fclose(log);
    return write_failed || close_failed ? -1 : 0;
}
