#include "mp3_header.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
-----------------------------------------------
--> The program skips the first 10 bytes (ID3 header).

--> After that, it reads 4 bytes at a time — this gives
   the Frame ID (like TIT2, TPE1, TALB, etc.).

--> Once we identify the tag:
   - Next 4 bytes tell the Frame Size -->data length.
   - Then 2 bytes are Flags.
   - The next 'size' bytes are the actual text data.

-->The first byte inside the data usually tells encoding,
   so we skip that (data + 1) to print clean text.

-->After finishing one frame, the file pointer automatically
   points to the next frame's start, so the loop continues
   reading the next tag until no valid tag (blank bytes) is found.

-->This way, each tag like Title, Artist, Album, Year, etc.
   is read and displayed one by one.
-----------------------------------------------
*/


mp3_tags tags;

int convert_BigToLittle_endian(int* size)
{
    unsigned char *ptr = (unsigned char *)size;
    unsigned char temp;

    temp = ptr[0];
    ptr[0] = ptr[3];
    ptr[3] = temp;

    temp = ptr[1];
    ptr[1] = ptr[2];
    ptr[2] = temp;
}
void view_mp3(char** argv)
{
    FILE* fp = fopen(argv[2],"rb");
    
    if(fp == NULL)
    {
        printf("\nERROR: In file opening\n");
        exit(0);
    }

    char frame_id[5] = {0};    // To store 4-letter tag IDs 
    int frame_size;            // To store size of data in each frame -> fram content
    unsigned short flags;
    char data[256];

    fseek(fp, 10, SEEK_SET); // Skip ID3 header

    while (!feof(fp)) //keep reading frames until file ends or  blank frame is found
    {
        if (fread(frame_id, 1, 4, fp) != 4)  // If we not read 4 bytes -->file ended
            break; // EOF reached
        if (frame_id[0] == 0)
            break; // No more frames --> Exits

        fread(&frame_size, 4, 1, fp);  // Read frame size -->next 4 bytes
        convert_BigToLittle_endian(&frame_size); // Convert Big -> Little endian

        fread(&flags, 2, 1, fp); // read flags 

        // if (frame_size <= 0 || frame_size > sizeof(data))
        // {
        //     fseek(fp, frame_size, SEEK_CUR);
        //     continue;
        // }

        if (frame_size <= 0) //If the file reaches to end-->sometimes fread()  read garbage or zero size.
            continue;

        unsigned char* data; //   ----> ALLOCATE MEMORY
        if(frame_size > 255) 
            data = malloc(frame_size + 1);  // allocate exact size + null termination
        else
        {
            data = malloc(256); // fix size allocation
        }

        // ---- READ FRAME CONTENT ----
        fread(data, 1, frame_size, fp);   // read frame_size bytes from sample.mp3 file -->Read the actual tag content
        data[frame_size] = '\0'; // Null termination

        // Call for each specific TAG
        if (!strcmp(frame_id, "TIT2"))
            read_mp3_title(data);
        else if (!strcmp(frame_id, "TPE1"))
            read_mp3_artist(data);
        else if (!strcmp(frame_id, "TALB"))
            read_mp3_album(data);
        else if (!strcmp(frame_id, "TYER") || !strcmp(frame_id, "TDRC"))
            read_mp3_year(data);
        else if(!strcmp(frame_id,"TCON"))
            read_mp3_content_type(data); 
        else if(!strcmp(frame_id,"TCOM"))
            read_mp3_composor(data);     
   
    }
    fclose(fp);
}
// -------------------------------Functions for each tags-----------------------------------
void read_mp3_title(char* data)
{
   
    // char buffer[30] = {'\0'};
	// fseek(fp, 10, SEEK_SET); 

	// fread(buffer,4,1,fp);
	// if(!strcmp(buffer, "TIT2"))
	// {
    //     fseek(fp, 10 + 4 + 4 + 2 + 1, SEEK_SET); 
    //     // 10 (header) +4(ID) +4(size) +2(flags) +1('\0') = 21
    //     fread(buffer, 25, 1, fp);  // read title (size = 25)
    //     buffer[25] = '\0';
    //     strcpy(tags.title,buffer);
    //     printf("Title : %s\n", tags.title);
    //    // printf("%ld\n", ftell(fp)); // 46

	// }

    strcpy(tags.title, data + 1); // skip first byte ('\0')
    printf("%-15s : %s\n", "Title", tags.title);
}

void read_mp3_artist(char* data)
{
    // char artist[100] = {0};
    // fseek(fp, 57, SEEK_SET);       // jump to start of artist name 46 + 11 
    // fread(artist, 1, 35, fp);      // read 35 bytes 
    
    // artist[35] = '\0';             // null terminate

    // strcpy(tags.artist_name, artist);
    // printf("Artist: %s\n", tags.artist_name);
    // //printf("%ld\n", ftell(fp));

    strcpy(tags.artist_name, data + 1);
    printf("%-15s : %s\n", "Artist", tags.artist_name);
}

void read_mp3_album(char* data)
{
    // char album[100] = {0};
    // fseek(fp, 102, SEEK_SET);        // jump to start of album text 92 + 11
    // fread(album, 1, 21, fp);         // read 21 bytes 
    // album[21] = '\0';              
    // strcpy(tags.album, album);
    // printf("Album: %s\n", tags.album);

    strcpy(tags.album, data + 1);
    printf("%-15s : %s\n", "Album", tags.album);
}

void read_mp3_year(char* data)
{
    // char year[5] = {0};

    // fseek(fp,120,SEEK_SET);
    // fread(year,1,4,fp);
    // year[4] = '\0';

    // strcpy(tags.year,year);
    // printf("Year: %s\n",tags.year);

    strcpy(tags.year, data);
   printf("%-15s : %s\n", "Year", tags.year);
}

void read_mp3_content_type(char* data)
{
    strcpy(tags.content_type,data);
    printf("%-15s : %s\n", "Content type", tags.content_type);
}

void read_mp3_composor(char* data)
{
    strcpy(tags.compose,data);
    printf("%-15s : %s\n", "Composor", tags.compose);

}