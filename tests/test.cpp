
#include "../src/main.h"  

int main() {
    // WE WILL BE CALLING THE MAIN FUNCTION WITH DIFFERENT PARAMS 

    // ./ipk-l4-scan -i eth0 --pt 21,22,143 --pu 53,67 localhost
    char arg1[] = "./main";
    char arg2[] = "--pu";
    char arg3[] = "53,67";
    char arg4[] = "--pt";
    char arg5[] = "21-24";

    char* argv[] = {arg1, arg2, arg3, arg4, arg5};
    int argc = 5;

    // TEST CASES 
    mainn(argc, argv);

    return 0;
}

