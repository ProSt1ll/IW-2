#include "recommendation_forked.h"
#include <string.h>


data_source create_data(const char *file) {
    if (file == NULL) {
        perror("Ошибка создания");
        exit(EXIT_FAILURE);
    }
    data_source data;
    data.users_amount = find_size_users(file);
    data.capacity = data.users_amount;
    data.objects_amount = find_size_obj(file, 0);
    data.users = (user *) calloc(data.users_amount, sizeof(user));
    for (int i = 0; i < data.users_amount; i++) {
        data.users[i].nickname = read_nickname(file, i + 1);
        data.users[i].marks = read_obj(file, i + 1, data.objects_amount);
    }
    data.objects = recommends_list_forked(&data);
    return data;
}

void refresh_rec(data_source *source) {
    source->objects = recommends_list_forked(source);
}

int random_int(int min, int max) {
    return min + rand() % (max + 1 - min);
}

char *create_nickname() {
    int length = random_int(3, 10);
    char *array = (char *) calloc(length + 1, sizeof(char));
    for (int i = 0; i < length; i++)
        array[i] = random_int(65, 122);
    array[length + 1] = '\0';
    return array;
}

int *create_marks(int obj) {
    int *array = (int *) calloc(obj, sizeof(int));
    for (int i = 0; i < obj; i++)
        array[i] = random_int(0, 5);
    return array;
}

data_source create_random_data(int users, int obj) {
    data_source data;
    data.users_amount = users;
    data.capacity = users;
    data.objects_amount = obj;
    data.users = (user *) calloc(data.users_amount, sizeof(user));
    for (int i = 0; i < data.users_amount; i++) {
        data.users[i].nickname = create_nickname();
        data.users[i].marks = create_marks(obj);
    }
    data.objects = recommends_list_forked(&data);
    return data;
}

int delete_data(data_source *source) {
    if (source == NULL)
        return -1;
    if (source->users != NULL) {
        for (int i = 0; i < source->users_amount; i++) {
            free(source->users[i].nickname);
            free(source->users[i].marks);
        }
    }
    free(source->objects);
    free(source->users);
    return 1;
}

char *read_nickname(const char *file, int number) {
    if ((file == NULL) || (number < 1)) {
        return NULL;
    }
    FILE *ptr = fopen(file, "r");

    if (ptr == NULL) {
        return NULL;
    }

    size_t i;
    char *array = (char *) calloc(15, sizeof(char));

    if (array == NULL) {
        fclose(ptr);
        return NULL;
    }
    char c = 0;
    for (i = 0; i < number - 1; i++) {
        while ((fscanf(ptr, "%c", &c) == 1) && (c != '\n')) {
        }
    }
    int cnt = 0;
    int nick_len = 15;
    while (1) {
        if (fscanf(ptr, "%c", &c) != 1) {
            return NULL;
        }
        if (c == ';' || c == '\n') {
            break;
        }
        array[cnt] = c;
        cnt++;
        if (cnt == nick_len - 1) {
            array = (char *) realloc(array, nick_len * 2);
            nick_len *= 2;
        }
    }

    fclose(ptr);
    return array;
}


int *read_obj(const char *file, int number, int size) {
    if ((file == NULL) || (number < 1)) {
        return NULL;
    }
    FILE *ptr = fopen(file, "r");

    if (ptr == NULL) {
        return NULL;
    }

    int i;
    int *array = (int *) calloc(size, sizeof(int));

    if (array == NULL) {
        fclose(ptr);
        return NULL;
    }
    char c = 0;
    for (i = 0; i < number - 1; i++) {
        while ((fscanf(ptr, "%c", &c) == 1) && (c != '\n')) {
        }
    }
    while ((fscanf(ptr, "%c", &c) == 1) && (c != ';')) {
    }
    int cur;
    if (fscanf(ptr, "%d", &cur) != 1) {
        return NULL;
    }
    int n = 1;
    for (i = size - 1; i > -1; i--) {
        array[i] = (cur / n) % 10;
        n = n * 10;
    }
    fclose(ptr);
    return array;
}

object *recommend(data_source *source, int id) {
    if (source == NULL)
        return NULL;
    int *array = (int *) calloc(source->objects_amount, sizeof(int));
    if (array == NULL)
        return NULL;
    int cnt = 0;
    for (int i = 0; i < source->objects_amount; i++) {
        if (source->users[id].marks[i] == 0) {
            array[cnt] = i;
            cnt++;
        }
    }
    object *recommend_obj = (object *) calloc(cnt, sizeof(object));
    for (int i = 0; i < cnt; i++)
        recommend_obj[i] = source->objects[array[i]];
    quick_sort_obj(recommend_obj, cnt);
    if (cnt > 10)
        cnt = 10;
    object *recommend_objj = (object *) calloc(cnt, sizeof(object));
    for (int i = 0; i < cnt; i++) {
        recommend_objj[i] = recommend_obj[i];
    }
    free(array);
    free(recommend_obj);
    // print_recomm_obj(recommend_objj, cnt);
    return recommend_objj;
}

object *recommends_list_forked(data_source *source) {
    object *all_objects = (object *) mmap(NULL,
                                          sizeof(object) * source->objects_amount,
                                          PROT_READ | PROT_WRITE,
                                          MAP_SHARED | MAP_ANONYMOUS,
                                          0, 0);
    struct mymsgbuf {
        size_t mtype;
        size_t numb;
    };
    size_t max_forks = sysconf(_SC_NPROCESSORS_ONLN);
    struct mymsgbuf qbuf = {1, 0};
    pid_t q_id = msgget(IPC_PRIVATE, IPC_CREAT | 0660);
    pid_t pid = fork();
    if (pid == 0) {
        for (size_t i = 0; i < source->objects_amount; ++i) {
            qbuf.numb = i;
            msgsnd(q_id, &qbuf, sizeof(size_t), 0);
        }
        exit(1);
    }

    pid_t pids[max_forks];

    for (size_t k = 0; k < max_forks; ++k) {
        pids[k] = fork();
        if (pids[k] == 0) {
            while (msgrcv(q_id, &qbuf, sizeof(size_t), qbuf.mtype,
                          IPC_NOWAIT) == sizeof(size_t)) {
                all_objects[qbuf.numb].object_id = qbuf.numb;
                all_objects[qbuf.numb].object_mark = 0;
                int cnt = 0;
                for (int j = 0; j < source->users_amount; j++) {
                    if (source->users[j].marks[qbuf.numb] > 0) {
                        all_objects[qbuf.numb].object_mark += source->users[j].marks[qbuf.numb];
                        cnt++;
                    }
                }
                all_objects[qbuf.numb].object_mark = all_objects[qbuf.numb].object_mark / cnt;
            }
            exit(1);
        }
    }
    waitpid(pid, 0, 0);
    for (size_t k = 0; k < max_forks; ++k) {
        waitpid(pids[k], 0, 0);
    }
    msgctl(q_id, IPC_RMID, NULL);

    object *ptr = (object *) calloc(source->objects_amount, sizeof(object));

    for (size_t i = 0; i < source->objects_amount; i++) {
        ptr[i] = all_objects[i];
    }
    munmap(all_objects, sizeof(object) * source->objects_amount);
    return ptr;
}

void print_recomm_obj(object *tmp, int size) {
    if (tmp == NULL) {
        return;
    }
    printf("Рекомендуемые Вам : \n");
    for (int i = 0; i < size; i++) {
        printf("---------------------------------"
               "--------------------------------- \n");
        printf("№%d\n", i + 1);
        printf("id : %d \n", tmp[i].object_id);
        printf("Сумма оценок : %f \n", tmp[i].object_mark);
    }
    printf("---------------------------------"
           "--------------------------------- \n");
}

void print_users(data_source *source) {
    if (source == NULL) {
        return;
    }
    for (int i = 0; i < source->users_amount; i++) {
        printf("---------------------------------"
               "--------------------------------- \n");
        printf("id : %d \n", i);

        printf("Nick : %s \n", source->users[i].nickname);
        printf("Marks :");
        for (int j = 0; j < source->objects_amount; j++) {
            printf("%d", source->users[i].marks[j]);
        }
        printf("\n");
        printf("---------------------------------"
               "--------------------------------- \n");
    }
}

void print_obj(data_source *source) {
    if (source == NULL) {
        return;
    }
    for (int i = 0; i < source->objects_amount; i++) {
        printf("---------------------------------"
               "--------------------------------- \n");
        printf("id : %d \n", source->objects[i].object_id);
        printf("Сумма оценок : %f \n", source->objects[i].object_mark);
    }
    printf("---------------------------------"
           "--------------------------------- \n");
}

int find_size_users(const char *file_path) {
    if (file_path == NULL) {
        return 0;
    }
    FILE *ptr = fopen(file_path, "r");

    if (ptr == NULL) {
        return 0;
    }
    int i = 0;
    char c = 1;
    while (fscanf(ptr, "%c", &c) == 1) {
        if (c == '\n') {
            i++;
        }
    }
    i++;
    fclose(ptr);
    return i;
}

int find_size_obj(const char *file_path, int num) {
    FILE *ptr = fopen(file_path, "r");

    if ((ptr == NULL) || (num < 0)) {
        return 0;
    }
    char c = 1;

    int i;

    for (i = 0; i < num - 1; i++) {
        while ((fscanf(ptr, "%c", &c) == 1) && (c != '\n')) {
        }
    }
    i = 0;
    while ((fscanf(ptr, "%c", &c) == 1) && (c != ';')) {
    }
    while ((fscanf(ptr, "%c", &c) == 1) && (c != ';')) {
        i++;
    }
    return i;
}

void quick_sort_obj(object *mas, int size) {
    int i = 0;
    int j = size - 1;

    float mid = mas[size / 2].object_mark;

    do {
        while (mas[i].object_mark > mid) {
            i++;
        }
        while (mas[j].object_mark < mid) {
            j--;
        }

        if (i <= j) {
            object tmp = mas[i];
            mas[i] = mas[j];
            mas[j] = tmp;
            i++;
            j--;
        }
    } while (i <= j);


    if (j > 0) {
        quick_sort_obj(mas, j + 1);
    }
    if (i < size) {
        quick_sort_obj(&mas[i], size - i);
    }
}

