#include<stdio.h>
#include<string.h>
#include"common.h"
#include"types.h"
#define MAX_FILE_SUFFIX 4

typedef struct _DecodeInfo
{
    /* Source Image info
       char *src_image_fname;
       FILE *fptr_src_image;
       uint image_capacity;
       uint bits_per_pixel;
       char image_data[MAX_IMAGE_BUF_SIZE];

       Secret File Info
       char *secret_fname;
       FILE *fptr_secret;
       char extn_secret_file[MAX_FILE_SUFFIX];
       char secret_data[MAX_SECRET_BUF_SIZE];
       long size_secret_file;

       Stego Image Info*/
    char output_fname[10];
    char extn_size;
    char data_size;
    char data[25];
    char extn[MAX_FILE_SUFFIX];
    char *stego_image_fname;
    FILE *fptr_stego_image;
    FILE *fptr_output;
}DecodeInfo;

/*Decodeing function prototype*/

/*Read and validate decode args from argv*/
OperationType check_operation_type(char *argv[]);

/*Read and validate decoding args*/
Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo);
/*Read and validate decoding args*/
Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo);

/*to perform the do decoding*/
Status do_decoding(DecodeInfo *decInfo);

/*get file pointers for i/p and o/p filers*/
Status open_files_decode(DecodeInfo *decInfo);

/* to skip the header file */
Status skip_header(DecodeInfo *decInfo);

/*Decode magic string*/
Status decode_magic_string( DecodeInfo *decInfo);

/*Deccode byte to lsb*/
Status decode_byte_to_lsb(char *image_buffer);

/*Decode secrete file extn size */
Status decode_secret_file_extn_size(DecodeInfo *decInfo);

/*Decode secrete file extn*/
Status decode_secret_file_extn(DecodeInfo *decInfo,int size);

/*Decode size to lsb*/
Status decode_size_to_lsb(char *image_buffer);

/* Decode secrete file data size*/
Status decode_secret_file_data_size(DecodeInfo *decInfo);

/*Decode secrete file data */
Status decode_secret_file_data(DecodeInfo *decInfo,int size);

                     