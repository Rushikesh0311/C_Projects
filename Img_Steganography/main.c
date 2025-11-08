#include <stdio.h>
#include "encode.h"
#include "types.h"

int main(int argc,char *argv[])
{
    EncodeInfo encInfo; // structure declaration
    


    if( check_operation_type(argv) == e_encode)
    {
        if(argc >= 4 && argc <= 5)
        {
            if((read_and_validate_encode_args(argc,argv,&encInfo)) == e_success)
            {
                // Encoding Proccess
                do_encoding(&encInfo);
            }
        }
        else
        {
            printf("\nEROR: \n");
        }
    }
}
