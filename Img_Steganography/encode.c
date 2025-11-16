#include <stdio.h>
#include "encode.h"
#include "types.h"
#include <string.h>

#define EXTN_FIELD_SIZE 32
#define EXT_IMG_BYTES 32
#define EXT_DATA_BYTES (EXT_IMG_BYTES / 8)   // 4 data bytes -> 32 image bytes
#define FILESIZE_FIELD_SIZE 32

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


Status read_and_validate_encode_args(int argc, char *argv[], EncodeInfo *encInfo)
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
            //strcpy(encInfo->extn_secret_file, ptr);   // copy extension part
            strncpy(encInfo->extn_secret_file, ptr, sizeof(encInfo->extn_secret_file) - 1);
            encInfo->extn_secret_file[sizeof(encInfo->extn_secret_file) - 1] = '\0';

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
    printf("POS after header = %ld\n", ftell(encInfo->fptr_stego_image));


    // 4  --> Encode MAGIC
    if (encode_magic_string(MAGIC_STRING, encInfo) != e_success)
        return e_failure;
    printf("Done: Magic String Encoded\n");
    printf("POS after magic  = %ld\n", ftell(encInfo->fptr_stego_image));

    // 5 --> Encode extension LENGTH 32 bits
    int extn_len = (int)strlen(encInfo->extn_secret_file);
    if (encode_secret_file_extn_size(extn_len, encInfo) != e_success)
        return e_failure;
    printf("Done: Extension Length Encoded\n");
    printf("POS after extnLen = %ld\n", ftell(encInfo->fptr_stego_image));

    // 6 -->Encode extension STRING
    if (encode_secret_file_extn(encInfo->extn_secret_file, encInfo) != e_success)
        return e_failure;
    printf("Done: Extension Encoded\n");
    printf("POS after extn = %ld bytes\n", ftell(encInfo->fptr_stego_image)); 

    // 7--> Encode secret FILE SIZE (32 bits)
    uint file_size = get_file_size(encInfo->fptr_secret);
    if (encode_secret_file_size(file_size, encInfo) != e_success)
        return e_failure;
    printf("Done: File Size Encoded\n");
    printf("POS after fileSz  = %ld\n", ftell(encInfo->fptr_stego_image));

    // 8 -->Encode secret FILE DATA
    if (encode_secret_file_data(encInfo) != e_success)
        return e_failure;
    printf("Done: Secret File Data Encoded\n");
    printf("POS after data  = %ld\n", ftell(encInfo->fptr_stego_image));

    // 9--> Copy remaining image bytes
    if (copy_remaining_img_data(encInfo->fptr_src_image, encInfo->fptr_stego_image) != e_success)
        return e_failure;

    return e_success;
}


Status check_capacity(EncodeInfo *encInfo)
{
    encInfo->image_capacity = get_image_size_for_bmp(encInfo->fptr_src_image);

    int magicString_len = strlen(MAGIC_STRING);
    int Extension_img_bytes = EXT_IMG_BYTES;                // <-- extension occupies 32 image bytes (fixed)
    int secret_file_size = get_file_size(encInfo->fptr_secret);

    // Count image bytes needed:
    // - SRC_IMGBUFF (header area already consumed)
    // - magicString_len data bytes -> magicString_len * 8 image bytes
    // - extnLen field takes 32 image bytes
    // - extension image bytes (Extension_img_bytes)
    // - file-size field takes 32 image bytes
    // - secret file data: secret_file_size data bytes -> *8 image bytes
    int encoding_image_bytes = SRC_IMGBUFF + (magicString_len * 8) + 32 /*extnLen field*/ + Extension_img_bytes + 32 /*file-size field*/ + (secret_file_size * 8);

    if (encoding_image_bytes < (int)encInfo->image_capacity)
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

Status encode_data_to_image(char *data, int size, FILE* fptr_src_image, FILE* fptr_stego_image)
{
    unsigned char buffer[8];

    for (int i = 0; i < size; ++i)
    {
        unsigned char ch = (unsigned char)data[i];

        /* READ exactly 8 carrier bytes (use fgetc to be explicit and safe) */
        for (int j = 0; j < 8; ++j)
        {
            int c = fgetc(fptr_src_image);
            if (c == EOF)               /* source exhausted unexpectedly */
                return e_failure;
            buffer[j] = (unsigned char)c;
        }

        /* encode this byte into the 8 LSBs */
        encode_byte_to_lsb(ch, buffer);

        /* WRITE the 8 modified bytes */
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
    while((ch =fgetc(encInfo->fptr_secret)) != EOF)
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

/* encode_secret_file_data: read secret file byte-by-byte and encode each byte
   into 8 carrier bytes from fptr_src_image and write to fptr_stego_image.
   This guarantees no carrier group is skipped.
*/

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
    // Encode extension so it occupies EXACTLY 32 image bytes (4 data bytes * 8 = 32)
    char extn_buf[EXT_DATA_BYTES];
    memset(extn_buf, '\0', EXT_DATA_BYTES);
    // copy at most 3 chars (leave one for NUL if needed), but for typical ".txt" it's fine
    strncpy(extn_buf, extn, EXT_DATA_BYTES);

    // encode EXT_DATA_BYTES data bytes -> will write EXT_IMG_BYTES (32) image bytes
    if (encode_data_to_image(extn_buf, EXT_DATA_BYTES, encInfo->fptr_src_image, encInfo->fptr_stego_image) == e_success)
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

Status encode_byte_to_lsb(char data, char *image_buffer)
{
    // Loop through 8 bits (MSB → LSB)
    for(int i = 0; i < 8; i++)
    {
        unsigned char bit = (data >> (7 - i)) & 1;   // extract one bit
        image_buffer[i] = (image_buffer[i] & ~1) | bit;  // clear LSB, set new bit
    }

    return e_success;
}









    



