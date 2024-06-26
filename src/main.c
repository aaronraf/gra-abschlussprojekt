#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <getopt.h>
#include <string.h>
#include <stdbool.h>
#include <sys/stat.h>

// const char* usage_msg = 
//     "Usage: %s [options] <Dateiname>\n"
//     "Options:\n"
//     "-c, --cycles <Zahl>         Die Anzahl der Zyklen, die simuliert werden sollen.\n"
//     "--directmapped              Simuliert einen direkt assoziativen Cache.\n"
//     "--fourway                   Simuliert einen vierfach assoziativen Cache.\n"
//     "--cacheline-size <Zahl>     Die Größe einer Cachezeile in Byte.\n"
//     "--cachelines <Zahl>         Die Anzahl der Cachezeilen.\n"
//     "--cache-latency <Zahl>      Die Latenzzeit eines Caches in Zyklen.\n"
//     "--memory-latency <Zahl>     Die Latenzzeit des Hauptspeichers in Zyklen.\n"
//     "--tf=<Dateiname>            Ausgabedatei für ein Tracefile mit allen Signalen.\n"
//     "-h, --help                  Eine Beschreibung aller Optionen des Programms.\n";
        
// const char* help_msg = "";

// void print_usage(const char* progname) {
//     fprintf(stderr, usage_msg, progname);
// }

// void print_help(const char* progname) {
//     print_usage(progname);
//     fprintf(stderr, "\n%s", help_msg);
// }

// int fetch_num() {
//     char* endptr;
//     int temp = strtol(optarg, &endptr, 10);

//     // if endptr is not null-byte, then the optarg value is invalid
//     if (endptr[0] != '\0') {
//         fprintf(stderr, "Invalid value for cycle!");
//         exit(EXIT_FAILURE);
//     }
//     return temp;
// }

char* read_csv(const char* csv_path) {
    FILE* csv_file = fopen(csv_path, 'r');
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
        fclose(csv_file);
        return NULL;
    }

    // null terminate
    content[file_info.st_size] = '\0';
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
    const char* csv_filename = "examples/inputs.csv";
    char* csv_content;

    // struct option long_options[] = {
    //     {"cycles", required_argument, 0, 'c'},
    //     {"directmapped", no_argument, 0, 0},
    //     {"fourway", no_argument, 0, 0},
    //     {"cacheline-size", required_argument, 0, 0},
    //     {"cachelines", required_argument, 0, 0},
    //     {"cache-latency", required_argument, 0, 0},
    //     {"memory-latency", required_argument, 0, 0},
    //     {"tf", required_argument, 0, 0},
    //     {"help", no_argument, 0, 'h'},
    //     {0, 0, 0, 0}
    // };
    
    // while ((opt = getopt_long(argc, argv, "c:h", long_options, &option_index)) != -1) {
    //     switch (opt) {
    //     case 'c':
    //         cycles = fetch_num;
    //         break;
    //     case 'h':
    //         print_help(progname);
    //         exit(EXIT_SUCCESS);
    //     case 0:
    //         if (strcmp(long_options[option_index].name, "directmapped") == 0) {
    //             direct_mapped = true;
    //         } else if (strcmp(long_options[option_index].name, "fourway") == 0) {
    //             fourway = true;
    //         } else if (strcmp(long_options[option_index].name, "cacheline-size") == 0) {
    //             cacheline_size = fetch_num;
    //         } else if (strcmp(long_options[option_index].name, "cachelines") == 0) {
    //             cachelines = fetch_num;
    //         } else if (strcmp(long_options[option_index].name, "cache-latency") == 0) {
    //             cache_latency = fetch_num;
    //         } else if (strcmp(long_options[option_index].name, "memory-latency") == 0) {
    //             memory_latency = fetch_num;
    //         } else if (strcmp(long_options[option_index].name, "tf") == 0) {
    //             tf_filename = optarg;
    //         }
    //         break;
    //     default:
    //         print_usage(progname);
    //         exit(EXIT_FAILURE);
    //     }
    // }

    // if (optind < argc) {
    //     input_filename = argv[optind];
    // }

    if (csv_filename) {
        if(!(csv_content = read_csv(csv_filename))) {
            return 1;
        }
    } else {
        csv_content = read_csv(csv_filename);
        printf("%s.*", csv_content);
    }

    return 0;
}