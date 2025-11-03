#include "mp3_header.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int args_count = 0; // count of argcs
int len = 0; // argv[2] for sample.mp3

FILE* fp;
char buffer[4];  // extra var to store the fetched data from file 
char mp3[4] = "ID3"; // for comparing with buffer

int validate_inputs(int argc,char *argv[])
{
    // for(int i=0;i<argc;i++)
    // {
    //     args_count++;

    // }
    //printf("%d\n",no_of_args);

    if(args_count == 1) // if there is only one cmd arg
    {
        printf("-------------------------------------------------------------------\n");   
        printf("\nError : ./mp3_tag_reader: Invalid Arguements\nUsage: ./mp3_tag_reader -h for help\n");
        printf("-------------------------------------------------------------------\n");
        exit(1);
        //return 0;
    }

    if(!strcmp(argv[1],"-h")) // for --help menu 
    {
       printf("Help menu for Mp3 Tag Reader and Editor:\n\nFor viewing the tags- ./mp3_tag_reader -v \n\nFor editing the tags- ./mp3_tag_reader -e\n\nModifier  Function\n  -t   Modifies Title tag\n  -T   Modifies Track tag\n  -a   Modifies Artist tag\n  -A   Modifies Album tag\n  -y   Modifies Year tag\n  -c   Modifies Comment tag\n  -g   Modifies Genre tag\n");
       return 0;
    }

    if ((!strcmp(argv[1], "-v") && argc < 3) || (!strcmp(argv[1], "-e") && argc < 5)) 
    {
        printf("-------------------------------------------------------------------\n");
        printf("Error: Invalid arguments for %s option.\n", argv[1]);
        printf("Usage:\n  ./a.out -v <file.mp3>\n  ./a.out -e <tag> <new_value>\n");
        printf("-------------------------------------------------------------------\n");
        return 0;
        
    }


    if((strcmp(argv[1],"-v") != 0) && (strcmp(argv[1],"-e") != 0) && (strcmp(argv[1],"-h") != 0)) //
    {
        //printf("Invalid Tags\n");
        printf("-------------------------------------------------------------------\n");
        printf("Error: Missing arguments for %s option.\n", argv[1]);
        printf("Usage:\n  ./a.out -v <file.mp3>\n  ./a.out -e <tag> <new_value>\n");
        printf("-------------------------------------------------------------------\n");
        return 0;
    } 


    // if(!(strcmp(argv[1],"-v"))) // valid args for -v 
    // {
    //     if(args_count < 2 || args_count > 3)
    //     {
    //         printf("Invalid number of arguments for -v option\n");
    //         return 0;
    //     }
    // } 

    // if(!(strcmp(argv[1],"-e"))) // valid argc for -e
    // {
    //     if(args_count < 4 || args_count > 5)
    //     {
    //         printf("Invalid number of arguments for -e option\n"); 
    //         return 0;  
    //     }
    // }

    
    len = strlen(argv[2]); // len of sample.mp3

    if(!strcmp(argv[1],"-v"))
    {

    
        if((strcmp((argv[2] + (len - 4 )),".mp3")) != 0)
        {
            printf("-----------------------------------\n");
            printf("Invalid File Format");
            printf("-----------------------------------\n");
            exit(0);
        }
    }

    if(!strcmp(argv[1],"-v"))
    {
        fp = fopen(argv[2],"rb");
        fread(buffer, 3, 1, fp); // reading 3 bytes from file
        //buffer[3] = '\0'; // null-terminate buffer for safe string use

        for(int i=0;i<3;i++) // first 3 bytes
        {
            if(buffer[i] != mp3[i])
            {
                printf("-------------------------------------------------------------------\n");
                //printf("\nIt's mp3 file\n");
                // flag = 1;
                printf("THE FILE CODE DOESN'T MATCH\n");
                printf("-------------------------------------------------------------------\n");
                exit(1);
            }
        }
        fclose(fp);
    }

    if(!strcmp(argv[1],"-e"))
    {
        if((strcmp(argv[2],"-t")) != 0 && (strcmp(argv[2],"-T")) != 0 && (strcmp(argv[2],"-a")) != 0 && (strcmp(argv[2],"-A")) != 0 && (strcmp(argv[2],"-y")) != 0
        && (strcmp(argv[2],"-c")) != 0 && (strcmp(argv[2],"-g")) != 0)
        {
            printf("\n./a.out : INVALID ARGUMENTS\n");
            printf("USAGE : ");

            printf("\nTo edit please pass like: ./a.out -e -t/-a/-A/-m/-y/-c(TAG Options)\n");
        }
    }

    if (!strcmp(argv[1], "-e")) 
    {
       
        int len = strlen(argv[4]);
        if(len < 4 || strcmp(argv[4] + (len - 4), ".mp3") != 0)
        {
            printf("-----------------------------------\n");
            printf("Invalid File Format (expected .mp3)\n");
            printf("-----------------------------------\n");
            return 0;
        }
    }


    return 1;
}
 