#include "recommendation.h"
#define WORLD_PATH "../files/words.txt"
#define ERROR_FILE_PATH "../files/error.txt"

int main(){
    //data_source base = create_data(ERROR_FILE_PATH);
   data_source base = create_random_data(32, 20);
    //print_users(&base);
    //print_obj(&base);
    //object *a = recommend(&base,2);

    recommend(&base,2);
  //  free(a);
    delete_data(&base);
    delete_data(&base);

    return 0;
}
