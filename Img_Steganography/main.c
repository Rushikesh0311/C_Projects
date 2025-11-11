#include <stdio.h>
#include "encode.h"
#include "types.h"

int main(int argc,char *argv[])
{
    EncodeInfo encInfo; // structure declaration
    
    if (argc < 4)
    {
        printf("Error: Missing or wrong arguments.\n");
        printf("Usage: %s -e <src_image> <secret_file>  optional--> <stego_image>\n", argv[0]);
        return 1;
    }

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








/*
54   -> Header
70   -> Header + Magic
102  -> After extn size
134  -> After extn string
166  -> After file size
366  -> After secret file data
*/
