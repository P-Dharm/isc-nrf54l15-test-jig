#ifndef __PROCESS_H_
#define __PROCESS_H_

typedef enum test_return_t{
    TEST_OK = 0,
    TEST_ERROR = 3,
}test_return;

void select_test_to_perform(void);

#endif 