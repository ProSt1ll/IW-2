#ifndef LIBS_RECOMMENDATION_INCLUDE_RECOMMENDATION_H_
#define LIBS_RECOMMENDATION_INCLUDE_RECOMMENDATION_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <sys/msg.h>
#include <string.h>
#include <time.h>
#define NUMBER_OF_RECOMMENDED 10
#define START_NICKNAME_LENGHT 15
#define MIN_RANDOM_NICKNAME_LENGHT 3
#define MAX_RANDOM_NICKNAME_LENGHT 10
#define MIN_RANDOM_MARK 0
#define MAX_RANDOM_MARK 5

    typedef struct {
    char *nickname;
    int *marks;
} user;

typedef struct {
    int object_id;
    float object_mark;
} object;

typedef struct {
    user *users;
    object *objects;
    int objects_amount;
    int users_amount;
    int capacity;
} data_source;


int refresh_rec(data_source *source);

data_source create_data(const char *file);

object *recommend(data_source *source, int id);

int quick_sort_obj(object *numbers, int size);

void print_obj(data_source *source);

void print_users(data_source *source);


int delete_data(data_source *source);

data_source create_random_data(int users, int obj);

#if defined(__cplusplus)
}
#endif

#endif  // LIBS_RECOMMENDATION_INCLUDE_RECOMMENDATION_H_
