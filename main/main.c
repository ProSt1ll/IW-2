#include "recommendation.h"
#define WORLD_PATH "../files/words.txt"

int main(){
    data_source base = create_random_data(100, 16000);
    //print_users(&base);
    //print_obj(&base);
    recommend(&base,3);
    refresh_rec(&base);
    delete_data(&base);
    return 0;
}
