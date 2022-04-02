#include <gtest/gtest.h>
#include "recommendation.h"

#define WORD_PATH "../files/words.txt"
#define ERROR_FILE_PATH "../files/error.txt"

TEST(test_data, test_data1) {
    data_source base = create_data(WORD_PATH);
    EXPECT_EQ(base.users[0].marks[0], 1);

    delete_data(&base);
}

TEST(test_eror_data, test_eror_data1) {

}

TEST(test_eror_data, test_eror_data2) {

}

TEST(test_eror_data, test_eror_data3) {

}

TEST(test_eror_data, test_eror_data4) {

}


int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
