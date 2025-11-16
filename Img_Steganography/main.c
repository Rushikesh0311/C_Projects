// #include <stdio.h>
// #include "encode.h"
// #include "types.h"
// #include "decode.h"


// int main(int argc,char *argv[])
// {
//     printf("\n======================================\n");
//     printf("   Steganography using LSB Technique    \n");
//     printf("========================================\n");
//     EncodeInfo encInfo; // structure declaration
    
//     /* arg validation: we always need at least 2 args: program and option */
//     if (argc < 2) 
//     {
//         printf("Error: Missing arguments.\n");
//         printf("Usage:\n  Encode: %s -e <src_image.bmp> <secret_file> [stego_image.bmp]\n  Decode: %s -d <stego_image.bmp> [output_file]\n", argv[0], argv[0]);
//         return e_failure;
//     }

//     if( check_operation_type(argv) == e_encode)
//     {
//         if(argc >= 4 && argc <= 5)
//         {
//             if((read_and_validate_encode_args(argc,argv,&encInfo)) == e_success)
//             {
//                 // Encoding Proccess
//                 do_encoding(&encInfo);
//             }
//         }
//         else
//         {
//             printf("Error: Unsupported option. Use -e for encode or -d for decode.\n");
//         }
//     }
//     else if(check_operation_type(argv) == e_decode)
//     {
//        DecodeInfo decInfo;

//         if(read_and_validate_decode_args(argc, argv, &decInfo) == e_success)
//             do_decoding(&decInfo);
//         else
//             printf("Error: Missing or wrong arguments.\nUsage: ./a.out -d <stego_image.bmp> [output_file]");
//     }
//     else
//     {
//         printf("Error: Unsupported option. Use -e for encode or -d for decode.\n");
//     }
    
// }



#include <stdio.h>
#include <string.h>
#include "encode.h"
#include "decode.h"
#include "types.h"

int main(int argc, char *argv[])
{
    printf("\n======================================\n");
    printf("   Steganography using LSB Technique\n");
    printf("======================================\n");

    if (argc < 2)
    {
        printf("Error: Missing arguments.\n");
        printf("Encode: %s -e <src_image.bmp> <secret_file> [stego.bmp]\n", argv[0]);
        printf("Decode: %s -d <stego.bmp> [output_file]\n", argv[0]);
        return 1;
    }

    /* ----------- ENCODING ----------- */
    if (check_operation_type(argv) == e_encode)
    {
        EncodeInfo encInfo;
        memset(&encInfo, 0, sizeof(encInfo));   // avoid garbage values

        if (argc >= 4 && argc <= 5)
        {
            if (read_and_validate_encode_args(argc, argv, &encInfo) == e_success)
            {
                do_encoding(&encInfo);
            }
            else
            {
                printf("Error: Invalid encode arguments.\n");
            }
        }
        else
        {
            printf("Usage: %s -e <src_image.bmp> <secret_file> [stego.bmp]\n", argv[0]);
        }
    }

    /* ----------- DECODING ----------- */
    else if (check_operation_type(argv) == e_decode)
    {
        DecodeInfo decInfo;
        memset(&decInfo, 0, sizeof(decInfo));    // avoid segfault

        if (read_and_validate_decode_args(argc, argv, &decInfo) == e_success)
        {
            do_decoding(&decInfo);
        }
        else
        {
            printf("Usage: %s -d <stego.bmp> [output_file]\n", argv[0]);
        }
    }

    /* ----------- INVALID OPTION ----------- */
    else
    {
        printf("Error: Unsupported option. Use -e or -d.\n");
    }

    return 0;
}




// /*
// 54   -> Header
// 70   -> Header + Magic
// 102  -> After extn size
// 134  -> After extn string
// 166  -> After file size
// 366  -> After secret file data
// */

