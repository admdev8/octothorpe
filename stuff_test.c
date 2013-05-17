#include "datatypes.h"
#include "dmalloc.h"
#include "stuff.h"
#include "logging.h"

int main()
{
    byte *buf=DMALLOC (byte, 1024, "buf");
    L_init ("tmp");
    
    memset (buf, 0, 1024);
    fill_by_tetrabytes(buf, 128, 0x0badf00d);
    L_print_buf (buf, 256);
    memset (buf, 0, 1024);
    fill_by_tetrabytes(buf, 127, 0x0badf00d);
    L_print_buf (buf, 256);
    memset (buf, 0, 1024);
    fill_by_tetrabytes(buf, 126, 0x0badf00d);
    L_print_buf (buf, 256);
    memset (buf, 0, 1024);
    fill_by_tetrabytes(buf, 125, 0x0badf00d);
    L_print_buf (buf, 256);

};
