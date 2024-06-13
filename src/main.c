#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <getopt.h>

int main(int argc, char const* argv[]) {
    const char* progname = argv[0];
    
    if (argc == 1) {
        print_usage(progname);
        return EXIT_FAILURE;
    }

    int opt;
    int option_index = 0;
    struct option long_options[] = {
        {"cycles", required_argument, 0, 'c'},
        {"directmapped", no_argument, 0, 0},
        {"fourway", no_argument, 0, 0},
        {"cacheline-size", required_argument, 0, 0},
        {"cachelines", required_argument, 0, 0},
        {"cache-latency", required_argument, 0, 0},
        {"memory-latency", required_argument, 0, 0},
        {"tf", required_argument, 0, 0},
        {"help", no_argument, 0, 'h'},
        {0, 0, 0, 0}
    };
    
    while ((opt = getopt_long(argc, argv, "", long_options, &option_index)) != -1) {
        switch (opt) {
        case 'h':
            
            exit(EXIT_SUCCESS);
        
        default:
            print_usage(progname);
            return EXIT_FAILURE;
        }
    }

    return 0;
}