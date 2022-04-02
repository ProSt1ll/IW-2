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


void refresh_rec(data_source *source);


int find_size_users(const char *file_path);

int find_size_obj(const char *file_path, int num);

char *read_nickname(const char *file, int number);

int *read_obj(const char *file, int number, int size);

data_source create_data(const char *file);

object *recommend(data_source *source, int id);

void quick_sort_obj(object *numbers, int size);

void print_obj(data_source *source);

void print_users(data_source *source);

void print_recomm_obj(object *source, int size);

int delete_data(data_source *source);

data_source create_random_data(int users, int obj);

#if defined(__cplusplus)
}
#endif

#endif  // LIBS_RECOMMENDATION_INCLUDE_RECOMMENDATION_H_
