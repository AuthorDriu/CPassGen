#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <time.h>

//#define DEBUG;


const char *standard_characters = "qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM123456789";


void print_err(char *msg) {
    fprintf(stderr, "%s\n", msg);
}


void print_help() {
    fprintf(stderr, "usage:\n");
    fprintf(stderr, "    passgen [options]\n");
    fprintf(stderr, "\n");
    fprintf(stderr, " -h | --help        show this message\n");
    fprintf(stderr, " -l <unsigned int>  set password length\n");
    fprintf(stderr, " -q <unsigned int>  set passwords quantity\n");
    fprintf(stderr, " -a <string>        set allowed characters\n");
}


typedef struct {
    uint32_t len;
    char *chars;
} allowed_characters_t;


allowed_characters_t *get_allowed_characters(const char *chars) {
    allowed_characters_t *ac = malloc(sizeof(allowed_characters_t));
    ac->chars = chars;
    ac->len = strlen(chars);
}


char get_random_character(allowed_characters_t *ac) {
    return ac->chars[(int32_t)(rand() % ac->len)];
}

char *generate_password(allowed_characters_t *ac, uint32_t len) {
    char *password = malloc(len + 1);
    password[len] = '\0';
    for (uint32_t i = 0; i < len; ++i) {
        password[i] = get_random_character(ac);
    }
    return password;
}


typedef struct {
    int32_t len;
    int32_t pass_quantity;
    allowed_characters_t *ac;
} arguments_t;


arguments_t parse_arguments(uint32_t argc, char **argv) {
    arguments_t args;

    // Standard values
    args.len = 16;
    args.pass_quantity = 1;
    args.ac = NULL;

    for (uint32_t i = 1; i < argc; ++i) {
        if (!strcmp(argv[i], "-l")) {
            if (i + 1 < argc) {
                args.len = atoi(argv[++i]);
            } else {
                print_err("\"-l\" takes int value");
                exit(-4);
            }

        } else if (!strcmp(argv[i], "-q")) {
            if (i + 1 < argc) {
                args.pass_quantity = atoi(argv[++i]);
            } else {
                print_err("\"-q\" takes int value");
                exit(-4);
            }
        } else if (!strcmp(argv[i], "-a")) {
            if (i + 1 < argc) {
                args.ac = get_allowed_characters(argv[++i]);
            } else {
                print_err("\"-a\" takes string value");
                exit(-4);
            }
        } else if (!strcmp(argv[i], "-h") || !strcmp(argv[i], "--help")) {
            print_help();
            exit(0);
        } else {
            char msg[256];
            sprintf(msg, "Wrong parameter: \"%s\"", argv[i]);
            print_err(msg);
            print_err("Try to use \"passgen --help\" to see help message");
            exit(-3);
        }
    }

    if (!args.ac) {
        args.ac = get_allowed_characters(standard_characters);
    }

    return args;
}


int main(uint32_t argc, char **argv) {
    srand(time(NULL));
    arguments_t args = parse_arguments(argc, argv);

#ifdef DEBUG
    fprintf(stderr, "len:                %d\n", args.len);
    fprintf(stderr, "passwords quantity: %d\n", args.pass_quantity);
    fprintf(stderr, "allowed characters:   \n");
    fprintf(stderr, "    len:            %d\n", args.ac->len);
    fprintf(stderr, "    chars:          %s\n", args.ac->chars);
#endif

    for (uint32_t i = 0; i < args.pass_quantity; ++i) {
        char *password = generate_password(args.ac, args.len);
        printf("%s\n", password);
        free(password);
    }

    free(args.ac);
    return (0);
}
