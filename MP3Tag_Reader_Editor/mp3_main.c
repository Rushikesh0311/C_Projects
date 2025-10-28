#include <stdio.h>
#include "mp3_header.h"
#include <string.h>
#include <unistd.h>

/*
================================================================================
                             MP3 Tag Editor Project
================================================================================
Name : Rushikesh Rajendra Gore
Date : 25-10-2025
Batch Id: 25021E - 22
Student_ID: 25022_008
language : C programming

Project Overview:
-----------------
This project is a simple MP3 metadata editor. It allows the user to view
and edit the ID3v2 tags of MP3 files, such as Title, Artist, Album, Year,
Genre, and Composer. The audio content of the MP3 remains unchanged.

Purpose:
--------
- Learn how MP3 ID3 tags are structured and stored.
- Practice file handling in C.
- Practice working with binary data, endian conversion, and dynamic memory.
- Create a functional tool to edit MP3 metadata safely.

Key Points:
-----------
- Only the selected tag is updated; all other frames and audio are unchanged.
- Uses dynamic memory allocation for frame data.
- Converts frame sizes to proper endian format before writing.
- Safe handling of invalid frame sizes to avoid corruption.
*/


int main(int argc,char* argv[])
{
    if(argc < 2)   // protect program from running with no arguments
    {
        printf("-------------------------------------------------------------------\n");
        printf("Error : ./mp3_tag_reader: Invalid Arguments\n");
        printf("Usage: ./mp3_tag_reader -h for help\n");
        printf("-------------------------------------------------------------------\n");
        return 1; // exit immediately — avoids seg-faults
    }

    int valid_args = validate_inputs(argc,argv);

    if(!strcmp(argv[1],"-v"))
    {    
        if(valid_args)
        {
           
            printf("\n-----------------------------------------------------\n");
            printf("            MP3 Tag Reader and Editor                    ");
            printf("\n-----------------------------------------------------\n");
            view_mp3(argv); // call for view mp3 --> only if args are valid
            printf("-----------------------------------------------------\n");
        }
    }


    if(!strcmp(argv[1],"-e"))
    {
        if(valid_args)
        {
            printf("\n-----------------------------------------------------\n");
            printf("            MP3 Tag Reader and Editor");
            printf("\n-----------------------------------------------------\n");
            edit_mp3(argv); // call for edit mp3 --> only id args are valid
            printf("\n-----------------------------------------------------\n");
            printf("Tag Edited Successfully\n");
            
        }
    }
    

    


}