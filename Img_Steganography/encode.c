#include <stdio.h>
#include "encode.h"
#include "types.h"
#include <string.h>
//#include "comman.h"

/* Function Definitions */

/* Get image size
 * Input: Image file ptr
 * Output: width * height * bytes per pixel (3 in our case)
 * Description: In BMP Image, width is stored in offset 18,
 * and height after that. size is 4 bytes
 */
uint get_image_size_for_bmp(FILE *fptr_image)
{
    uint width, height; //  uint -->unsigned int
    // Seek to 18th byte
    fseek(fptr_image, 18, SEEK_SET); //18 - 20 width 

    // Read the width (an int)
    fread(&width, sizeof(int), 1, fptr_image);

    //printf("width = %u\n", width);

    // Read the height (an int)
    fread(&height, sizeof(int), 1, fptr_image);

    //printf("height = %u\n", height);

    // Return image capacity
    return width * height * 3;
}

/* 
 * Get File pointers for i/p and o/p files
 * Inputs: Src Image file, Secret file and
 * Stego Image file
 * Output: FILE pointer for above files
 * Return Value: e_success or e_failure, on file errors
 */
Status open_files(EncodeInfo *encInfo)
{
    // Src Image file
    encInfo->fptr_src_image = fopen(encInfo->src_image_fname, "r");
    // Do Error handling
    if (encInfo->fptr_src_image == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->src_image_fname); // file name ->strcut member

    	return e_failure;
    }

    // Secret file
    encInfo->fptr_secret = fopen(encInfo->secret_fname, "r");
    // Do Error handling
    if (encInfo->fptr_secret == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->secret_fname);

    	return e_failure;
    }

    // Stego Image file
    encInfo->fptr_stego_image = fopen(encInfo->stego_image_fname, "w");
    // Do Error handling
    if (encInfo->fptr_stego_image == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->stego_image_fname);

    	return e_failure;
    }

    // No failure return e_success
    return e_success;
}

OperationType check_operation_type(char *argv[])
{
    if(!(strcmp(argv[1],"-e")))
        return e_encode;
    else if(!(strcmp(argv[1],"-d")))
        return e_decode;
    else
        e_unsupported;        
}

// OperationType extension_check(char** argv)
// {
//     int len = strlen(argv[2]);
// }



Status read_and_validate_encode_args(int argc, char **argv, EncodeInfo *encInfo)
{
    if(argc < 2)
    {
        printf("MIssing Agruments\n");
        return e_failure;
    }
    

    // ---------- Validate source image (argv[2]) ----------

    int len = strlen(argv[2]);                                   // length of Source img file
    if (len < 4 || strcmp(argv[2] + (len - 4), ".bmp") != 0)    // check if file name ends with ".bmp"
    {
        //Prevents invalid memory access when subtracting 4 from len.
        printf("Error: image %s format should be .bmp\n", argv[2]);
        return e_failure;
    }
    encInfo->src_image_fname = argv[2];      // Storing image name in structure

    // ---------- Validate secret file (argv[3]) ----------
    if (argc > 3)
    {
        char *ptr = strstr(argv[3], ".");    // finding '.' in filename to get extension
        if (ptr != NULL)
        {
            strcpy(encInfo->extn_secret_file, ptr);   // copy extension part
        }
        else
        {
            printf("Invalid file type: %s -->no extension\n", argv[3]);
            return e_failure;
        }

        //  valid extensions .txt || .c || .sh
        if (!(strcmp(encInfo->extn_secret_file, ".txt")) || !(strcmp(encInfo->extn_secret_file, ".c")) || !(strcmp(encInfo->extn_secret_file, ".sh")))
        {
            encInfo->secret_fname = argv[3]; // // valid file --> save name
        }
        else
        {
            printf("Invalid file Extension! Only .txt, .c, or .sh you can use!!.\n");
            return e_failure;
        }
    }
    else
    {
        // if secret file missing
        printf("Usage: ./a.out <option> <stego_file> <secret_file>\n");
        return e_failure;
    }

    // ---------- Optional output File (argv[4]) ----------
    if (argc > 4)
    {
        int len2 = strlen(argv[4]);         //Lenght of last option output file
        if (len2 < 4 || strcmp(argv[4] + (len2 - 4), ".bmp") != 0)
        {
            printf("Error: image %s format should be .bmp\n", argv[4]);
            return e_failure;
        }
        encInfo->stego_image_fname = argv[4];
    }
    else
    {
        encInfo->stego_image_fname = "stego.bmp"; //  // no output file --> NULL

    }

    return e_success;
}

// Status do_encoding(EncodeInfo *encInfo)
// {
//     if(open_files(encInfo) == e_success)
//     {
//         if(check_capacity(encInfo) == e_success)
//         {
//             if(encInfo->stego_image_fname == NULL)
//             {
//                 printf("\nOuutput File is not passed!\n");
//                 encInfo->stego_image_fname = "stego.bmp";
//                 if(open_files(encInfo) == e_failure)
//                 {
//                     printf("Error: %s function failed\n", "open_encode_files");
// 					return e_failure;	
//                 }
//             }
//             else
// 			{
// 				printf("INFO: Output File mentioned. ");
// 				if (open_files(encInfo) == e_failure)
// 				{
// 					printf("Error: %s function failed\n", "open_files");
// 					return e_failure;
// 				}
// 					printf("INFO: Opened %s\n",encInfo->stego_image_fname);
// 			}
//         }    


// Status do_encoding(EncodeInfo *encInfo)
// {
//     // 1 -->Open files
//     if (open_files(encInfo) != e_success) 
//         return e_failure;

//     // 2 -->Check capacity
//     if (check_capacity(encInfo) != e_success) 
//         return e_failure;

//     // 3 -->Copy 54-byte BMP header 
//     if (copy_bmp_header(encInfo->fptr_src_image, encInfo->fptr_stego_image) != e_success)
//         return e_failure;
//     printf("Done: Header Copy\n");

//     // 4 -->Encode MAGIC string
//     if (encode_magic_string(MAGIC_STRING, encInfo) != e_success) 
//         return e_failure;
//     printf("Done: Magic String Encoded\n");

//     // 5 -->Encode extension LENGTH (as 4 bytes -> 32 LSBs)
//     int extn_len = (int)strlen(encInfo->extn_secret_file);
//     if (encode_secret_file_extn_size(extn_len, encInfo) != e_success) 
//         return e_failure;
//     printf("Done: Extension Length Encoded\n");

//     // 6 -->Encode extension STRING itself (each char -> 8 LSBs)
//     if (encode_data_to_image(encInfo->extn_secret_file, extn_len,encInfo->fptr_src_image, encInfo->fptr_stego_image) != e_success)
//         return e_failure;
//     printf("Done: Extension Encoded\n");

//     // 7 -->Encode SECRET FILE SIZE (as 4 bytes -> 32 LSBs)
//     uint file_size = get_file_size(encInfo->fptr_secret);
//     if (encode_secret_file_extn_size((int)file_size, encInfo) != e_success) 
//         return e_failure;
//     printf("Done: File Size Encoded\n");
//     //printf(" Secret File Actual Size -%zu bytes\n", file_size);

    
//     // if(encode_secret_file_extn(encInfo->extn_secret_file,encInfo) != e_success)
//     //     return e_failure;
//     // printf("Done: Encoding secret file extension\n");

//     // if(encode_secret_file_size(file_size,encInfo) != e_success)
//     //     return e_failure;
//     // printf("Done: Encoding secret file size\n");

//     //8 -->Encoding Secret file data    
//     encode_secret_file_data(encInfo);
//     printf("Done: Secret File Data Encoded\n");

//     // 9 --> Copyed all remaining data 
//     if (copy_remaining_img_data(encInfo->fptr_src_image, encInfo->fptr_stego_image) != e_success)
//         return e_failure;


    
//     return e_success;
// }

Status do_encoding(EncodeInfo *encInfo)
{
    // 1--> Open files
    if (open_files(encInfo) != e_success) 
        return e_failure;

    // 2--> Check capacity
    if (check_capacity(encInfo) != e_success) 
        return e_failure;

    // 3 --> Copy 54-byte header
    if (copy_bmp_header(encInfo->fptr_src_image, encInfo->fptr_stego_image) != e_success)
        return e_failure;
    printf("Done: Header Copy\n");
    //printf("POS after header = %ld\n", ftell(encInfo->fptr_stego_image));


    // 4  --> Encode MAGIC
    if (encode_magic_string(MAGIC_STRING, encInfo) != e_success)
        return e_failure;
    printf("Done: Magic String Encoded\n");
    //printf("POS after magic  = %ld\n", ftell(encInfo->fptr_stego_image));

    // 5 --> Encode extension LENGTH 32 bits
    int extn_len = (int)strlen(encInfo->extn_secret_file);
    if (encode_secret_file_extn_size(extn_len, encInfo) != e_success)
        return e_failure;
    printf("Done: Extension Length Encoded\n");
    //printf("POS after extnLen = %ld\n", ftell(encInfo->fptr_stego_image));

    // 6 -->Encode extension STRING
    if (encode_secret_file_extn(encInfo->extn_secret_file, encInfo) != e_success)
        return e_failure;
    printf("Done: Extension Encoded\n");
    //printf("POS after extn = %ld bytes\n", ftell(encInfo->fptr_stego_image)); 

    // 7--> Encode secret FILE SIZE (32 bits)
    uint file_size = get_file_size(encInfo->fptr_secret);
    if (encode_secret_file_size(file_size, encInfo) != e_success)
        return e_failure;
    printf("Done: File Size Encoded\n");
    //printf("POS after fileSz  = %ld\n", ftell(encInfo->fptr_stego_image));

    // 8 -->Encode secret FILE DATA
    if (encode_secret_file_data(encInfo) != e_success)
        return e_failure;
    printf("Done: Secret File Data Encoded\n");
    //printf("POS after data  = %ld\n", ftell(encInfo->fptr_stego_image));

    // 9--> Copy remaining image bytes
    if (copy_remaining_img_data(encInfo->fptr_src_image, encInfo->fptr_stego_image) != e_success)
        return e_failure;

    return e_success;
}


Status check_capacity(EncodeInfo *encInfo)
{
    encInfo->image_capacity = get_image_size_for_bmp(encInfo->fptr_src_image);

    int magicString_len = strlen(MAGIC_STRING);
    int Extension_len = strlen(encInfo->extn_secret_file);
    int serect_FileSize = get_file_size(encInfo->fptr_secret);
    int Encoding_Things = SRC_IMGBUFF + ((magicString_len + sizeof(int) + Extension_len + 4 + serect_FileSize) * 8);

    if(Encoding_Things < encInfo->image_capacity)
        return e_success;
    else    
        return e_failure;
}  

uint get_file_size(FILE* fptr)
{
    //This moves the file pointer to the end of the file.
    uint size;
	fseek(fptr, 0, SEEK_END);
	size = (uint)ftell(fptr); 
	rewind(fptr);
	return size;
    /*we moved to the end, this value equals --> total file size in bytes.
    We store that value into size.*/
}

Status copy_bmp_header(FILE *fptr_src_image, FILE *fptr_dest_image)
{
    fseek(fptr_src_image,0,SEEK_SET);                   // Setting offset ot the 0th index

    char buffer[54];
    fread(buffer,sizeof(char),54,fptr_src_image);       // reading 54 header bytes from src img 

    fwrite(buffer,sizeof(char),54,fptr_dest_image);     // writing 54 header byes in dest file as it is

    return e_success;

}

Status encode_magic_string(const char* magic_string,EncodeInfo *encInfo)
{
    if(encode_data_to_image((char *)MAGIC_STRING,(int)strlen(MAGIC_STRING),encInfo->fptr_src_image,encInfo->fptr_stego_image) == e_success)
        return e_success;
    else
        return e_failure;    
}

Status encode_data_to_image(char *data, int size, FILE *fptr_src_image, FILE *fptr_stego_image)
{
    unsigned char buffer[8];

    for (int i = 0; i < size; i++)       // loop for each data byte
    {
        unsigned char ch = data[i];      // byte to encode

        // Read 8 bytes from source image
        for (int j = 0; j < 8; j++)
        {
            int code = fgetc(fptr_src_image);
            if (code == EOF)
                return e_failure;

            buffer[j] = (unsigned char)code;
        }

        // Encode 1 byte -> 8 bits into 8 LSBs
        for (int k = 0; k < 8; k++)
        {
            // GET bit from ch -->MSB to LSB
            unsigned char extract_bit = (ch >> (7 - k)) & 1;

            // clear LSB --> CLEAR LSB of buffer[k]
            buffer[k] = (buffer[k] & ~1);

            // SET the new bit in LSB
            buffer[k] = buffer[k] | extract_bit;
        }

        // Writing bytes to stego image
        if (fwrite(buffer, 1, 8, fptr_stego_image) != 8)
            return e_failure;
    }

    return e_success;
}

void encode_int_to_lsb(int val,unsigned char buffer[])
{
    for(int i=0;i<32;i++)
    {
        unsigned char extracting_bits = (val >> (31 -i) & 1);
        buffer[i] = (buffer[i] & ~1) | extracting_bits;
    }
}
Status encode_secret_file_extn_size(int size,EncodeInfo *encInfo)
{
    char Buffer[32];

    for(int i=0;i<32;i++)
    {
        int ch = fgetc(encInfo->fptr_src_image);
        if(ch == EOF)
            return e_failure;
        Buffer[i] = (unsigned char)ch;

    }

    encode_int_to_lsb(size,Buffer);     //seperate Function 

    //printf("Check Point--> Size encoded into image  %d\n", size);

    if(fwrite(Buffer,1,32,encInfo->fptr_stego_image) != 32)
        return e_failure;

    return e_success;    
}

Status encode_secret_file_data(EncodeInfo *encInfo)
{
    unsigned char Buffer[8];
    int ch;

    //Reading secret file data
    while(ch =fgetc(encInfo->fptr_secret) != EOF)
    {
        unsigned char bytes = (unsigned char)ch;

         // Reading 8 bytes from source image
        for(int i=0;i<8;i++)
        {
            int data = fgetc(encInfo->fptr_src_image);
            if(data == EOF)
                return e_failure;
            Buffer[i] = (unsigned char)data;    
        }

        for(int j=0;j<8;j++)
        {
            unsigned char extrating_bytes = bytes >> (7 -j) & 1;
            Buffer[j] = (Buffer[j] & ~1) | extrating_bytes;
        }

        fwrite(Buffer,1,8,encInfo->fptr_stego_image);
    }

    return e_success;

}

Status copy_remaining_img_data(FILE *fptr_src, FILE *fptr_dest)
{
    // Copying the rest of the image data after encoding is finished
    int ch;
    while ((ch = fgetc(fptr_src)) != EOF)
    {
        fputc(ch, fptr_dest); // writing same byte to output image
    }
    return e_success;
}

// Encode extension string (.txt) --> each char stored in 8 LSBs
Status encode_secret_file_extn(const char *extn, EncodeInfo *encInfo)
{
    if(encode_data_to_image((char *)extn,strlen(extn),encInfo->fptr_src_image, encInfo->fptr_stego_image) == e_success)
        return e_success;
    else
        return e_failure;    
}


// Encode secret file size (32 bits) --> stored in 32 image bytes
Status encode_secret_file_size(long file_size, EncodeInfo *encInfo)
{
    unsigned char buffer[32];

    // Read 32 bytes from the source image
    for (int i = 0; i < 32; i++)
    {
        int ch = fgetc(encInfo->fptr_src_image);
        if (ch == EOF) return e_failure;
        buffer[i] = (unsigned char)ch;
    }

    // Store file_size bits in the LSBs
    encode_int_to_lsb(file_size, buffer);

    // Write  32 modifed bytes to stego image
    fwrite(buffer, 1, 32, encInfo->fptr_stego_image);

    return e_success;
}









    



