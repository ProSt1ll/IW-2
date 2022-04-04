#include "recommendation.h"
#define WORLD_PATH "../files/words.txt"
#define ERROR_FILE_PATH "../files/error.txt"

int main(){
    data_source base = create_data(WORLD_PATH);

    delete_data(&base);

    return 0;
}
