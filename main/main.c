#include "recommendation.h"
#define WORLD_PATH "../files/words.txt"

int main(){
    data_source base = create_random_data(3, 16000);
    //print_users(&base);
    //print_obj(&base);
    object *a = recommend(&base,2);
   // refresh_rec(&base);
    free(a);
    delete_data(&base);
    return 0;
}
