#include<stdio.h>
#include<string.h>
#include"common.h"
#include"types.h"
#include"decode.h"
/*Function definations*/
//------------------------------------------------------//
/*read and validate the arguments*/
Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo)
{
    if( strcmp(strstr(argv[2],"."),".bmp") == 0)
    {
        //print argv is .bmp file
        printf("argv[2] is a .bmp file");
        decInfo->stego_image_fname=argv[2];
        if(argv[3]!=NULL)
        {
            strcpy(decInfo->output_fname , argv[3]);
        }
        else
            decInfo->output_fname[0]='\0';
        //return e_success
        return e_success;
    }
    else
    {
        printf("argv[2] is not a .bmp file");
    //return e_failure
        return e_failure;
    }
}
//----------------------------------------------------------------//
/*start the encoding*/
Status do_decoding(DecodeInfo *decInfo)
{
    if(open_files_decode(decInfo) == e_success)
    {
        //print the open the files is sucess
        printf("Info : Open file operation is success \n");
        if(skip_header(decInfo) == e_success)
        {
        //to skip the header data is done
            printf("Info : Skip header data is done\n");
            if(decode_magic_string(decInfo) == e_success)
            {
            //to magic string is decodeing
                printf("Info : magic string is decoded\n");
                if(decode_secret_file_extn_size(decInfo) == e_success)
                {
            //the decode the stego file extentaon size
                    printf("Info : Decoded the stego file extension size\n");
                    if(decode_secret_file_extn(decInfo,decInfo->extn_size) == e_success)
                    {
                //ifb the decodeing the stego  file extension part
                        printf("Info : Decoded the stego file extension\n");
                        if(decode_secret_file_data_size(decInfo) == e_success)
                        {
                        //to the decode part secret file data size
            printf("Info : Decoded the secret file data size\n");
                            if(decode_secret_file_data(decInfo,decInfo->data_size) == e_success)
                            {
                    //to the decode the part sevcret file data is done to goto next step
                                printf("Info : Decoding the secret file data is done\n");
                            }
                        }
                    }
                }
            }
        }
    }
}
//------------------------------------------------------------------------------------//

/*Open the file of stego.bmp*/
Status open_files_decode(DecodeInfo *decInfo)
{
    decInfo->fptr_stego_image=fopen(decInfo->stego_image_fname,"r");
    if(decInfo->fptr_stego_image == NULL)
    {
        perror("fopen");
        fprintf(stderr,"ERROR: Unable to open file %s\n",decInfo->stego_image_fname);
    //return e_failure
        return e_failure;
    }
    //return e_success
    return e_success;
}

//---------------------------------------------------------------------------------------//
/*to the decode skip the header file*/
Status skip_header(DecodeInfo *decInfo)
{
    fseek(decInfo->fptr_stego_image,54,SEEK_SET);
    //return e_success
    return e_success;
}
//------------------------------------------------------------------------------------//
/*decode the magic string from the stego.bmp*/
Status decode_magic_string(DecodeInfo *decInfo)
{
    int i;
    char magic_str[3];
    char buffer[8];
    for(i=0;i<2;i++)
    {
        fread(buffer,8,1,decInfo->fptr_stego_image);
        magic_str[i]=decode_byte_to_lsb(buffer);
    }
    //to magic string 2 is not nul chacaters
    magic_str[2]='\0';
    if(strcmp(magic_str,"#*") == 0)
    {
        //print the string is matching string
        printf("Info : Magic string is matching\n");
    //return e_success
        return e_success;
    }
    else
    {
        //print the magic is not matching string
        printf("Magic is not matching\n");
    //return e_failure
        return e_failure;
    }
}
//------------------------------------------------------------------//
/*decode byte to lsb*/
char decode_byte_to_lsb(char *image_buffer)
{
    int i;
    char ch=0;
    for(i=0;i<8;i++)
    {
        ch = ((image_buffer[i] & 1)<<i) | ch;
    }
    //return ch
    return ch;
}
//----------------------------------------------------------------------//
/*decode the secrete file extension size*/
Status decode_secret_file_extn_size(DecodeInfo *decInfo)
{
    char buffer[32];
    //fread from the decode secret file extension
    fread(buffer,32,1,decInfo->fptr_stego_image);
    //functon the call decode size to lsb
    decInfo->extn_size = decode_size_to_lsb(buffer);
//return e_success
    return e_success;
}
/*decode size to lsb*/
Status decode_size_to_lsb(char * image_buffer)
{
    int i;
    char ch=0;
    for(i=0;i<32;i++)
    {
        ch = ((image_buffer[i] & 1) << i) | ch;
    }
    //return ch
    return ch;
}
//-----------------------------------------------------------------------//
/*decode the secret file extension*/
Status decode_secret_file_extn(DecodeInfo *decInfo, int size)
{
    char buffer[8];
    int i;
    for(i=0;i<size;i++)
    {
        fread(buffer,8,1,decInfo->fptr_stego_image);
    //function the call the decode byte to lsb
        decInfo->extn[i] = decode_byte_to_lsb(buffer);
    }
    if(decInfo->output_fname[0] != '\0')
    {
        if(strcmp(decInfo->extn,strtok(decInfo->output_fname,".")) == 0)
        {
            decInfo->fptr_output = fopen(decInfo->output_fname,"w");
            //return e_success
            return e_success;
        }
    }
    else
    {
        strcpy(decInfo->output_fname, "output");
        strcat(decInfo->output_fname, decInfo->extn);
        decInfo->output_fname[6+size]='\0';
        decInfo->fptr_output = fopen(decInfo->output_fname,"w");
        //return e_success
    return e_success;
    }
}
//--------------------------------------------------------------------//
/* decode secret file data size */
Status decode_secret_file_data_size(DecodeInfo *decInfo)
{
    char buffer[32];
    //fread from secret file size
    fread(buffer,32,1,decInfo->fptr_stego_image);
    decInfo->data_size = decode_size_to_lsb(buffer);
    //return e_success
    return e_success;
}
//----------------------------------------------------------------//
/*decode the secret file data*/
Status decode_secret_file_data(DecodeInfo *decInfo,int size)
{
    char buffer[8];
    int i;
    for(i=0;i<size;i++)
    {
        fread(&buffer,8,1,decInfo->fptr_stego_image);
        decInfo->data[i]=decode_byte_to_lsb(buffer);
    }

    fwrite(decInfo->data,decInfo->data_size,1,decInfo->fptr_output);
    fclose(decInfo->fptr_output);
    //return e_success
    return e_success;
}
          
                                                                       