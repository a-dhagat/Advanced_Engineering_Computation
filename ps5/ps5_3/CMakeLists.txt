enable_testing()

add_executable(ps5_3 main.cpp bintree.h)
add_test(NAME TEST_BINTREE_01 COMMAND ps5_3)
