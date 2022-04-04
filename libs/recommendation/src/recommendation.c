#include "recommendation.h"

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
    fclose(ptr);
    return i;
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
    char *array = (char *) calloc(START_NICKNAME_LENGHT, sizeof(char));

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
    int nick_len = START_NICKNAME_LENGHT;
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

object *recommends_list(data_source *source) {
    if (source == NULL)
        return NULL;
    object *all_objects = (object *)
            calloc(source->objects_amount, sizeof(object));
    if (all_objects == NULL)
        return NULL;
    for (int i = 0; i < source->objects_amount; i++) {
        all_objects[i].object_id = i;
        all_objects[i].object_mark = 0;
        int cnt = 0;
        for (int j = 0; j < source->users_amount; j++) {
            if (source->users[j].marks[i] > 0) {
                all_objects[i].object_mark += source->users[j].marks[i];
                cnt++;
            }
        }
        all_objects[i].object_mark = all_objects[i].object_mark / cnt;
        // printf("ready for %d\n",i*100/source->objects_amount);
    }
    // quick_sort_obj(all_objects,source->objects_amount);
    return all_objects;
}

data_source create_data(const char *file) {
    data_source data;
    data.users_amount = find_size_users(file);
    if (data.users_amount == 0) {

        delete_data(&data);
        perror("Ошибка создания 2");
        exit(EXIT_FAILURE);
    }
    data.capacity = data.users_amount;
    data.objects_amount = find_size_obj(file, 0);
    if (data.objects_amount == 0) {
        delete_data(&data);
        perror("Ошибка создания 3");
        exit(EXIT_FAILURE);
    }
    data.users = (user *) calloc(data.users_amount, sizeof(user));
    if (data.users == NULL) {
        delete_data(&data);
        perror("Ошибка создания 4");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < data.users_amount; i++) {
        data.users[i].nickname = read_nickname(file, i + 1);
        if (data.users[i].nickname == NULL) {
            delete_data(&data);
            perror("Ошибка создания 5");
            exit(EXIT_FAILURE);
        }
        data.users[i].marks = read_obj(file, i + 1, data.objects_amount);
        if (data.users[i].nickname == NULL) {
            delete_data(&data);
            perror("Ошибка создания 6");
            exit(EXIT_FAILURE);
        }
    }

    data.objects = recommends_list(&data);
    if (data.objects == NULL) {
        delete_data(&data);
        perror("Ошибка создания 7");
        exit(EXIT_FAILURE);
    }
    return data;
}

int random_int(int min, int max) {
    return min + rand() % (max + 1 - min);
}

char *create_nickname() {
    int length = random_int(MIN_RANDOM_NICKNAME_LENGHT, MAX_RANDOM_NICKNAME_LENGHT);
    char *array = (char *) calloc(length + 1, sizeof(char));
    for (int i = 0; i < length; i++)
        array[i] = random_int(65, 122);
    array[length] = '\0';
    return array;
}

int *create_marks(int obj) {
    int *array = (int *) calloc(obj, sizeof(int));
    for (int i = 0; i < obj; i++)
        array[i] = random_int(MIN_RANDOM_MARK, MAX_RANDOM_MARK);
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
    data.objects = recommends_list(&data);
    return data;
}

int refresh_rec(data_source *source) {
    if (source == NULL)
        return -1;
    if (source->objects == NULL)
        return -1;
    source->objects = recommends_list(source);
    return 0;
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

void print_recomm_obj(object *tmp, int size) {
    if (tmp == NULL) {
        perror("Ошибка вывода рекомендованных объектов");
        exit(EXIT_FAILURE);
    }
    printf("Рекомендуемые Вам : \n");
    for (int i = 0; i < size; i++) {
        if (tmp[i].object_mark < 0.1)
            continue;
        printf("---------------------------------"
               "--------------------------------- \n");
        printf("№%d\n", i + 1);
        printf("id : %d \n", tmp[i].object_id);
        printf("Сумма оценок : %f \n", tmp[i].object_mark);
    }
    printf("---------------------------------"
           "--------------------------------- \n");
}

object *recommend(data_source *source, int id) {
    if (source == NULL)
        return NULL;
    if (id > source->users_amount || id < -1)
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
    free(array);
    if (cnt > NUMBER_OF_RECOMMENDED)
        cnt = NUMBER_OF_RECOMMENDED;
    print_recomm_obj(recommend_obj, cnt);
    return recommend_obj;
}


void print_users(data_source *source) {
    if (source == NULL) {
        perror("Ошибка вывода пользователей");
        exit(EXIT_FAILURE);
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
        perror("Ошибка вывода всех объектов");
        exit(EXIT_FAILURE);
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

int quick_sort_obj(object *mas, int size) {
    if (mas == NULL)
        return -1;
    if (size < 1)
        return -1;
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
    return 0;
}
