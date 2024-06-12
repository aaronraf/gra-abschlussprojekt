#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char const* argv[]) {
    const char* progname = argv[0];
    
    if (argc == 1) {
        print_usage(progname);
        return EXIT_FAILURE;
    }

    int opt;
    while ((opt = getopt(argc, argv, "c:")) != -1) {
        switch (opt) {
        case 'c':
            
            break;
        
        default:
            print_usage(progname);
            return EXIT_FAILURE;
        }
    }

    return 0;
}