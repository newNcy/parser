#include <stdio.h>
#include "parser.h"

int main(int argc, char *argv[])
{
    Source * test = sourceFromFile("test.c");
    if (!test) return -1;

    while (!eos(test)) {
        Node * ast = translation_unit(test);
    }
    return 0;
}
