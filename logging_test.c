#include "logging.h"
#include "dmalloc.h"

void main()
{
    L_init("logging_test.log");
    L_once("#1 should be printed once\n");
    L_once("#2 should be printed once\n");
    L_once("#3 should be printed once\n");
    L_once("#1 should be printed once\n");
    L_once("#1 should be printed once\n");
    L_once("#1 should be printed once\n");
    L_once("#2 should be printed once\n");
    L_once("#2 should be printed once\n");
    L_once("#3 should be printed once\n");
    L_deinit();

    dump_unfreed_blocks();
};
