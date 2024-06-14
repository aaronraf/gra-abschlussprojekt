#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <getopt.h>
#include <string.h>
#include <stdbool.h>

const char* usage_msg = 
    "Usage: %s [options] <Dateiname>\n"
    "Options:\n"
    "-c, --cycles <Zahl>         Die Anzahl der Zyklen, die simuliert werden sollen.\n"
    "--directmapped              Simuliert einen direkt assoziativen Cache.\n"
    "--fourway                   Simuliert einen vierfach assoziativen Cache.\n"
    "--cacheline-size <Zahl>     Die Größe einer Cachezeile in Byte.\n"
    "--cachelines <Zahl>         Die Anzahl der Cachezeilen.\n"
    "--cache-latency <Zahl>      Die Latenzzeit eines Caches in Zyklen.\n"
    "--memory-latency <Zahl>     Die Latenzzeit des Hauptspeichers in Zyklen.\n"
    "--tf=<Dateiname>            Ausgabedatei für ein Tracefile mit allen Signalen.\n"
    "-h, --help                  Eine Beschreibung aller Optionen des Programms.\n";
        
const char* help_msg = "";

void print_usage(const char* progname) {
    fprintf(stderr, usage_msg, progname);
}

void print_help(const char* progname) {
    print_usage(progname);
    fprintf(stderr, "\n%s", help_msg);
}

int fetch_num() {
    char* endptr;
    int temp = strtol(optarg, &endptr, 10);

    // if endptr is not null-byte, then the optarg value is invalid
    if (endptr[0] != '\0') {
        fprintf(stderr, "Invalid value for cycle!");
        exit(EXIT_FAILURE);
    }
    return temp;
}

int main(int argc, char const* argv[]) {
    const char* progname = argv[0];
    
    if (argc == 1) {
        print_usage(progname);
        return EXIT_FAILURE;
    }

    int opt;
    int option_index = 0;
    int cycles = 0;
    bool direct_mapped = false;
    bool fourway = false;
    int cacheline_size = 0;
    int cachelines = 0;
    int cache_latency = 0;
    int memory_latency = 0;
    char* tf_filename = NULL;
    char* input_filename = NULL;

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
    
    while ((opt = getopt_long(argc, argv, "c:h", long_options, &option_index)) != -1) {
        switch (opt) {
        case 'c':
            cycles = fetch_num;
            break;
        case 'h':
            print_help(progname);
            exit(EXIT_SUCCESS);
        case 0:
            if (strcmp(long_options[option_index].name, "directmapped") == 0) {
                direct_mapped = true;
            } else if (strcmp(long_options[option_index].name, "fourway") == 0) {
                fourway = true;
            } else if (strcmp(long_options[option_index].name, "cacheline-size") == 0) {
                cacheline_size = fetch_num;
            } else if (strcmp(long_options[option_index].name, "cachelines") == 0) {
                cachelines = fetch_num;
            } else if (strcmp(long_options[option_index].name, "cache-latency") == 0) {
                cache_latency = fetch_num;
            } else if (strcmp(long_options[option_index].name, "memory-latency") == 0) {
                memory_latency = fetch_num;
            } else if (strcmp(long_options[option_index].name, "tf") == 0) {
                tf_filename = optarg;
            }
            break;
        default:
            print_usage(progname);
            exit(EXIT_FAILURE);
        }
    }

    if (optind < argc) {
        input_filename = argv[optind];
    }

    return 0;
}