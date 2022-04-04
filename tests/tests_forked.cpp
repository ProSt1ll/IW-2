#include <gtest/gtest.h>
#include "recommendation_forked.h"

#define ERROR_FILE_PATH "../files/error.txt"


TEST(test_data_random, test_data_random) {
    data_source base = create_random_data(20, 20);
    EXPECT_TRUE(base.users[0].marks[0] > -1);
    EXPECT_TRUE(base.users[0].nickname[0] > -1);
    EXPECT_TRUE(base.objects_amount == 20);
    EXPECT_TRUE(base.objects[0].object_id == 0);
    delete_data(&base);
}

TEST(test_recommend, test_recommend) {
    data_source base = create_random_data(20, 20);
    object *tmp = recommend(&base, 30);
    int a;
    if (tmp == NULL)
        a = 1;
    else
        a = 0;
    EXPECT_TRUE(a);
    tmp = recommend(&base, 10);
    EXPECT_TRUE(tmp[0].object_id > -1);
    free(tmp);
    delete_data(&base);
}

TEST(test_sort, test_sort) {
    data_source base = create_random_data(20, 20);
    EXPECT_EQ(quick_sort_obj(base.objects, -1), -1);
    EXPECT_EQ(quick_sort_obj(base.objects, 20), 0);
    delete_data(&base);
}

TEST(test_delete_data, test_delete_data) {
    data_source base = create_random_data(20, 20);
    EXPECT_TRUE(delete_data(&base));
    data_source *tmp = NULL;
    EXPECT_EQ(delete_data(tmp), -1);
}

TEST(test_print, test_print) {
    data_source base = create_random_data(20, 20);
    EXPECT_NO_THROW(print_users(&base));
    EXPECT_NO_THROW(print_obj(&base));
    delete_data(&base);
}

TEST(test_refresh, test_refresh) {
    data_source base = create_random_data(20, 20);
    EXPECT_EQ(refresh_rec(&base), 0);
    delete_data(&base);
    data_source *tmp = NULL;
    EXPECT_EQ(refresh_rec(tmp), -1);
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
