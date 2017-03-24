#include <stdlib.h>
#include <stdio.h>
#include <argp.h>

#define DATABASE_PATH "./database.dat"

/* Program version */
const char *argp_program_version = "0.1";

/* Email addres in case of bugs */
const char *argp_program_bug_address = "eduardoquijano2@gmail.com";

/* Program documentation */
static char doc[] = "Toy program to handle brazilian ZIP code (CEP) storage like " 
                    "a database (SGBD) in binary files";

/* Description of the parameter */
static char args_doc[] = "CEP [LONG] - A numéric representation of CEP (without dashes or spaces)\n"
                         "\t" "Ex: 75800012\n"
                         "DESCRIPTION [STRING] - A text (up to 255 characters long) for all CEP's"
                         "text (street name and etc)";

/* Acceptable options */ 
typedef enum  
{
    ACTION_CREATE = 'c', 
    ACTION_READ   = 'r',
    ACTION_UPDATE = 'u',
    ACTION_DELETE = 'd',
    ACTION_LIST = 'l'
} selectable_action;

static struct argp_option options[] = { 
    { "create", ACTION_CREATE,  "CEP_NUM", 0, "Add a CEP to database."},
    { "read",   ACTION_READ,    "CEP_NUM", 0, "Read a CEP in database."},
    { "update", ACTION_UPDATE,  "CEP_NUM", 0, "Update a CEP in database."},
    { "delete", ACTION_DELETE,  "CEP_NUM", 0, "Delete a CEP from database."},
    { 0 } 
};

typedef unsigned long cep_num_t;
typedef unsigned char[50] house_desc_t;

/* CEP struct */
struct CEP 
{
    cep_num_t num;
    house_desc_t desc;
};

/* Used by main to communicate with parse_opt. */
struct arguments
{
    struct CEP cep;
    selectable_action action;
};


/* Parse a single option. */
static error_t parse_opt (int key, char *arg, struct argp_state *state)
{
    /* Get the input argument from argp_parse, which we
         know is a pointer to our arguments structure. */
    struct arguments *arguments = state->input;

    switch (key)
    {
        case ACTION_CREATE:
        case ACTION_READ:
        case ACTION_UPDATE:
        case ACTION_DELETE:
            arguments->action = key;
            arguments->cep.num = atol(arg);
            break;
        default:
            return ARGP_ERR_UNKNOWN;
    }
  return EXIT_SUCCESS;
}

/* Our argp parser. */
static struct argp argp = { options, parse_opt, args_doc, doc };


/* Save a CEP in database */
void create_cep(struct CEP cep){
    FILE *database = fopen(DATABASE_PATH, "ab");
    fwrite(&cep, sizeof(struct CEP), 1, database);
    fclose(database);
}

void update_cep(cep_num_t num, struct CEP cep) {
    FILE *database = fopen(DATABASE_PATH, "r+b");
    if(database == NULL)
    {
        printf("Could'nt open file '" DATABASE_PATH "'." "\n");
        return NULL;
    }
    
    struct CEP *cep = malloc(sizeof(struct CEP));
    
    while(!feof(database)){
        fread(cep, sizeof(struct CEP), 1, database);
        if(cep->num == num) {
            fseek(database, ftell(database) - sizeof(struct CEP), SEEK_SET)
            fwrite(&cep, sizeof(struct CEP), 1, database);
            return cep;
        }
    }
}

/* Read a cep from database */
struct CEP *read_cep(cep_num_t num) {
     FILE *database = fopen(DATABASE_PATH, "rb");
     if(database == NULL)
     {
         printf("Could'nt open file '" DATABASE_PATH "'." "\n");
         return NULL;
     }
     
     struct CEP *cep = malloc(sizeof(struct CEP));
     
     
     while(!feof(database)){
         fread(cep, sizeof(struct CEP), 1, database);
         if(cep->num == num) {
             fclose(database);
             return cep;
         }
     }

     fclose(database);
     free(cep);
     printf("CEP %ld wasn't in database" "\n", num);
     return NULL;
}

void delete_cep(cep_num_t num) {
     FILE *database = fopen(DATABASE_PATH, "rb");
 
     if(database == NULL)
     {
         printf("Could'nt open file '" DATABASE_PATH "'." "\n");
         return;
     }

    FILE *database_tmp = fopen(DATABASE_PATH ".tmp", "wb");
    struct CEP cep;
    bool found = false; 
    while(!feof(database))
    {
        fread(&cep, sizeof(struct CEP), 1, database);
        if(cep.num == num) found = true;
        else fwrite(&cep, sizeof(struct CEP), 1, database_tmp);
    }

    fclose(database);
    fclose(database_tmp);

    if(found) 
    {
        remove(DATABASE_PATH);
        rename(DATABASE_PATH ".tmp", DATABASE_PATH);
    }
    else 
    {
        printf("CEP %ld wasn't in database" "\n", num);
        remove(DATABASE_PATH ".tmp");
    }    
    return;
}

void print_cep(struct CEP cep) {
    printf( "\n"
            "Número do CEP: %ld"
            "\n", cep.num);
    return;
}

int main (int argc, char **argv) 
{
    struct arguments arguments;

    /* DEFAULTS */
    arguments.action = ACTION_LIST;

    /* Parse arguments */
    argp_parse (&argp, argc, argv, 0, 0, &arguments);

    struct CEP *cep;
    switch (arguments.action) {
        case ACTION_CREATE:
            create_cep(arguments.cep);
            break;
        case ACTION_READ:
            cep = read_cep(arguments.cep.num);
            if(cep != NULL)
            {
                print_cep(*cep);
                free(cep);
            }
            break;
        case ACTION_UPDATE:
            cep = read_cep(arguments.cep.num, *cep);
            break;
        case ACTION_DELETE:
            delete_cep(arguments.cep.num);
            break;
        case ACTION_LIST:
        default:
        

    }


    return EXIT_SUCCESS;
}

