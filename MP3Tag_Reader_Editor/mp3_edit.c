#include "mp3_header.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include <stdint.h>


//mp3_tags tags;


/*
-----------------------------------------------
--> Opens the original MP3 file and creates a new one.

-->Copies the first 10 bytes (ID3 header) as they are.

-->Reads each frame (name, size, flags, and data).

-->When it finds the frame to edit (like TIT2 for title)
   it writes the new text instead of the old one.

-->All other frames are copied unchanged.

-->At the end, it copies the remaining MP3 audio part.

--> The new file now has only the selected tag updated.
----------------------------------------------- */


void LittleToBigEndianInPlace(int *num) 
{
    unsigned char *ptr = (unsigned char *)num;
    unsigned char temp;

    temp = ptr[0];
    ptr[0] = ptr[3];
    ptr[3] = temp;

    temp = ptr[1];
    ptr[1] = ptr[2];
    ptr[2] = temp;
}


void edit_mp3(char** argv)
{
    

    FILE *fp = fopen("sample.mp3", "rb");
    FILE *temp = fopen("temp.mp3", "wb");

    if(fp == NULL || temp == NULL)
    {
        printf("\nERROR --> While opeing file\n");
        exit(0);
    }

    // Move to beginning
    fseek(fp, 0, SEEK_SET);

   
    if(!strcmp(argv[2], "-t"))
    {
        edit_tag(fp, temp, "TIT2", argv[3]);  // Title
        printf("\nNew Title Tag : %s\n",argv[3]);
    }
    else if(!strcmp(argv[2], "-a"))
    {
        edit_tag(fp, temp, "TPE1", argv[3]);  // Artist
        printf("\nNew Artist Tag : %s\n",argv[3]);
    }
    else if(!strcmp(argv[2], "-A"))
    {
        edit_tag(fp, temp, "TALB", argv[3]);  // Album
        printf("\nNew Album Tag : %s\n",argv[3]);
    }
    else if(!strcmp(argv[2], "-y"))
    {
        edit_tag(fp, temp, "TDRC", argv[3]);  // Year
        printf("\nNew Year Tag : %s\n",argv[3]);
    }
    else if(!strcmp(argv[2], "-g"))
    {
        edit_tag(fp, temp, "TCON", argv[3]);  // Genre
        printf("\nNew Genre Tag : %s\n",argv[3]);
    }
    else if(!strcmp(argv[2], "-c"))
    {
        edit_tag(fp, temp, "TCOM", argv[3]);  // Composer
        printf("\nNew Composer Tag : %s\n",argv[3]);
    }
    else{
        printf("Option not supported.\n");
    //printf("");
    }

    fclose(fp);
    fclose(temp);    
    
    // // rename(temp,fp);
    // // remove(fp);
    remove("sample.mp3"); // removes the file after edit
    rename("temp.mp3","sample.mp3"); // rename to temp.mp3 --> sample.mp3
   
        
}

void edit_tag(FILE* fp, FILE* temp, char* tag_options,char* new_value)
{
    char tag_name[5]; // to store frame name (4 bytes + null)
    char ch;  // temporary variable to copy one byte

    // Copy ID3 header --> first 10 bytes
    for (int i = 0; i < 10; i++)
    {
        fread(&ch, 1, 1, fp);
        fwrite(&ch, 1, 1, temp);
    }

    // Read frames until end of tag
    while (fread(tag_name, 1, 4, fp) == 4)  // read 4 bytes for frame name
    {
        tag_name[4] = '\0';

        unsigned char flags[2];
        int size;

        // Read size and flags
        if (fread(&size, 4, 1, fp) != 1) // read 4-byte size into int
            break;

        LittleToBigEndianInPlace(&size);  // Convert to big-endian
        if (fread(flags, 1, 2, fp) != 2) // 2-byte flags
            break;

        
        if (size <= 0 || size > 1000000) // skip frame if size is invalid
            break;

        // Read frame data
        unsigned char* frame_data = (unsigned char*)malloc(size);
        if (!frame_data) 
        { 
            printf("Memory allocation failed\n"); 
            exit(0); 
        }
        if (fread(frame_data, 1, size, fp) != size) 
        { 
            free(frame_data); 
            break;
        }

        // if (!strcmp(tag_name, tag_options) && new_value != NULL)
        // {
        //     // Write frame name
        //     fwrite(tag_name, 1, 4, temp);

        //     // New size and conversion
        //     int new_size = strlen(new_value);  //new title size and convert endian
        //     LittleToBigEndianInPlace(&new_size);

        //     fwrite(&new_size, 4, 1, temp); // write new size in big-endian
        //     fwrite(flags, 1, 2, temp);     // write original flags
        //     fwrite(new_value, 1, strlen(new_value), temp); // write new title
        // }
        if (!strcmp(tag_name, tag_options) && new_value != NULL)
        {
            fwrite(tag_name, 1, 4, temp);

            // Add +1 for the text encoding byte
            // int new_size = strlen(new_value) + 1;
            // LittleToBigEndianInPlace(&new_size);

            // fwrite(&new_size, 4, 1, temp);

            int new_size = strlen(new_value) + 1;
            int be_size = new_size;                // keep host-order value intact
            LittleToBigEndianInPlace(&be_size);    // convert copy to big-endian
            fwrite(&be_size, 4, 1, temp);

            fwrite(flags, 1, 2, temp);

            unsigned char enc = 0x00; //1-byte encoding flag before the string data
            fwrite(&enc, 1, 1, temp);  // write encoding byte
            fwrite(new_value, 1, strlen(new_value), temp); // then the string
        }
        else
        {
            // Write other frames as-is
            fwrite(tag_name, 1, 4, temp);
            int temp_size = size;
            LittleToBigEndianInPlace(&temp_size);  // convert size to big-endian before writing
            fwrite(&temp_size, 4, 1, temp);  // write frame size
            fwrite(flags, 1, 2, temp);
            fwrite(frame_data, 1, size, temp);//frame content
        }

        free(frame_data);
    }

    // Copy remaining data after ID3 frames -->Audio frames — the actual music data.
    while (fread(&ch, 1, 1, fp) > 0)
        fwrite(&ch, 1, 1, temp);
}







