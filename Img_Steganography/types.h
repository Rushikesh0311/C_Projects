#ifndef TYPES_H
#define TYPES_H

/* User defined types */
typedef unsigned int uint;

/* Status will be used in fn. return type */
typedef enum
{
    e_success,
    e_failure
} Status;

typedef enum
{
    e_encode, // for opt -e argv 
    e_decode, // -d
    e_unsupported // noting two of them
} OperationType;

#endif
