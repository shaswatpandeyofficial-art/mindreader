#ifndef MIND_H
#define MIND_H

#include <stdio.h>

#define MIND_MIN 1
#define MIND_MAX 100
#define MIND_MAX_GUESSES 7

typedef enum {
    REPLY_HIGHER,
    REPLY_LOWER,
    REPLY_CORRECT,
    REPLY_INVALID,
    REPLY_END
} Reply;

typedef struct {
    int low;
    int high;
} Range;

typedef struct {
    int guess;
    Reply reply;
} Turn;

typedef struct {
    Range remaining;
    unsigned int count;
    Turn history[MIND_MAX_GUESSES];
} Game;

typedef enum {
    RESULT_FOUND,
    RESULT_INCONSISTENT,
    RESULT_ABORTED
} ResultKind;

typedef union {
    int secret;
    Range remaining;
} ResultValue;

/* kind tells callers which member of value is active. */
typedef struct {
    ResultKind kind;
    ResultValue value;
} Result;

/* Starts a fresh round and records each valid reply in *game. */
Result mind_play(FILE *input, FILE *output, Game *game);

/* Returns 0 on success or -1 if opening, writing, or closing fails. */
int mind_append_log(const char *path, const Game *game, Result result);

#endif
