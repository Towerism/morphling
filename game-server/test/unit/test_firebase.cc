#include <gtest/gtest.h>
#include <database/firebase.h>

TEST(FIREBASETest, WriteCheck) {
    firebase f("morphling-50028");
    fire_err fe = f.write_json("test.json",json::parse("{\"Test\":15}"));
    ASSERT_EQ(fe.res_code,CURLE_OK);
    ASSERT_EQ(fe.res_json["Test"],15);
}

TEST(FIREBASETest, MultiWriteCheck) {
    firebase f("morphling-50028");
    fire_err fe = f.write_json("test.json",json::parse("{\"Test\":15}"));
    ASSERT_EQ(fe.res_code,CURLE_OK);
    ASSERT_EQ(fe.res_json["Test"],15);

    fe = f.write_json("test.json",json::parse("{\"Test\":1}"));
    ASSERT_EQ(fe.res_code,CURLE_OK);
    ASSERT_EQ(fe.res_json["Test"],1);
}

