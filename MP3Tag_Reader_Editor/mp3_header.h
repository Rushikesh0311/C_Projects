#ifndef MP3_HEADER_H
#define MP3_HEADER_H

#include <stdio.h>

typedef struct MP3_TAGS
{
    char title[100];
    char artist_name[30];
    char album[30];
    char year[11];
    char content_type[50];
    char compose[50];
}mp3_tags;

int validate_inputs(int argc,char* argv[]);


void view_mp3(char** argv);
int convert_BigToLittle_endian(int* size);

void read_mp3_title(char* data); 
void read_mp3_artist(char* data);
void read_mp3_album(char* data);
void read_mp3_year(char* data);
void read_mp3_content_type(char* data);
void read_mp3_composor(char* data);

void edit_mp3(char** argv);
void LittleToBigEndianInPlace(int *num);
void edit_tag(FILE* fp,FILE* temp,char* tag_options,char* new_title);



#endif
