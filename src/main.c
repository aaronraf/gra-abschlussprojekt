#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <getopt.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <sys/stat.h>

struct Request {
    unsigned addr;
    unsigned data;
    int we;         // 0 for read, 1 for write
};

struct Result {
    size_t cycles;
    size_t misses;
    size_t hits;
    size_t primitiveGateCount;
};

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

char* read_csv(const char* csv_path) {
    FILE* csv_file = fopen(csv_path, "r");
    if (csv_file == NULL) {
        fprintf(stderr, "Error, can't open file!\n");
        return NULL;
    }

    struct stat file_info;

    // edge case read file informations
    if (fstat(fileno(csv_file), &file_info)) {
        fprintf(stderr, "Error retrieving file stat\n");
        fclose(csv_file);
        return NULL;
    }

    // edge case invalid file
    if (!S_ISREG(file_info.st_mode) || file_info.st_size <= 0) {
        fprintf(stderr, "Error processing file\n");
        fclose(csv_file);
        return NULL;
    }

    // allocate memory in heap
    char* content = (char*) malloc(file_info.st_size + 1);
    if (!content) {
        fprintf(stderr, "Error reading file, cannot allocate enough memory\n");
        fclose(csv_file);
        return NULL;
    }

    // read file content and save it in "content"
    if (fread(content, 1, file_info.st_size, csv_file) != (size_t) file_info.st_size) {
        fprintf(stderr, "Error reading file!");
        free(content);
        content = NULL;
        fclose(csv_file);
        return NULL;
    }

    // null terminate
    content[file_info.st_size] = '\0';
    return content;
}

void parse_data(const char* content, struct Request request[], int number_of_requests) {
    char* content_copy = strdup(content);   // copy of the content to avoid modifying the original
    if (content_copy == NULL) {
        fprintf(stderr, "Error, .csv file does not have any content.");
        exit(EXIT_FAILURE);
    }
    
    char* rest;
    char* line = strtok_r(content_copy, "\n", &rest);

    for (int i = 0; i < number_of_requests && line != NULL; i++) {
        char temp[2];     // Allocate space for "W" or "R" and null-byte terminator
        unsigned int addr;
        int data;

        sscanf(line, "%1s,%x,%d", temp, &addr, &data);
        data = temp[0] == 'W' ? data : 0;

        // printf("Temp: %s\n", temp);
        // printf("Addr: %x\n", addr);
        // printf("Data: %d\n", data);

        // request[i].we = temp;
        // request[i].addr = addr;
        // request[i].data = data;

        line = strtok_r(NULL, "\n", &rest);
    }

    free(content_copy);
}

int main(int argc, char const* argv[]) {
    const char* progname = argv[0];
    
    // if (argc == 1) {
    //     print_usage(progname);
    //     return EXIT_FAILURE;
    // }

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
    const char* csv_path = "../examples/inputs.csv";
    char* csv_content;

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
            cycles = fetch_num();
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
                cacheline_size = fetch_num();
            } else if (strcmp(long_options[option_index].name, "cachelines") == 0) {
                cachelines = fetch_num();
            } else if (strcmp(long_options[option_index].name, "cache-latency") == 0) {
                cache_latency = fetch_num();
            } else if (strcmp(long_options[option_index].name, "memory-latency") == 0) {
                memory_latency = fetch_num();
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

    if (csv_path) {
        if(!(csv_content = read_csv(csv_path))) {
            return 1;
        }
    } else {
        csv_content = read_csv(csv_path);
        if (csv_content) {
            printf("%s", csv_content);
            free(csv_content);
        }
    }
    printf("%s\n", csv_content);
    
    // TODO: replace 3 with request array's actual size
    parse_data(csv_content, 3);

    return 0;
}