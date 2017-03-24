#include "cep.h"    
#include <stdlib.h>
#include <string.h>

#define CEP_DESC_SIZE 255

typedef long int cep_num_t;

typedef struct {
    cep_num_t num;
    char[CEP_DESC_SIZE] description;
}cep_t;

bool compare_cep_by_num(cep_num_t *num, cep_t *cep) 
{
    return *num == cep->num;
}

bool compare_cep_by_desc(char[] description, cep_t *cep)
{
    return str_cmp(description, cep->description) == 0;
}
