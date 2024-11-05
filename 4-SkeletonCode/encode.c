#include <stdio.h>
#include "encode.h"
#include "types.h"

/* Function Definitions */

/* Get image size
 * Input: Image file ptr
 * Output: width * height * bytes per pixel (3 in our case)
 * Description: In BMP Image, width is stored in offset 18,
 * and height after that. size is 4 bytes
 */
uint get_image_size_for_bmp(FILE *fptr_image)
{
    uint width, height;
    // Seek to 18th byte
    fseek(fptr_image, 18, SEEK_SET);

    // Read the width (an int)
    fread(&width, sizeof(int), 1, fptr_image);
    printf("width = %u\n", width);

    // Read the height (an int)
    fread(&height, sizeof(int), 1, fptr_image);
    printf("height = %u\n", height);

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
//__________________________________________________________________________
/* fuction for open files */
Status open_files(EncodeInfo *encInfo)
{
    // Src Image file
    encInfo->fptr_src_image = fopen(encInfo->src_image_fname, "r");
    // Do Error handling
    if (encInfo->fptr_src_image == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->src_image_fname);

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

//________________________________________________________________________________
/* DO ENCODING FUNCTION CALLING */
Status do_encoding(EncodeInfo *encInfo);

 if(open_files(encInfo) == e_success)
    {
        //to print the open fils operation is success
	printf("-Open files operation is success \n");
    return e_failure;

	if( check_capacity(encInfo) == e_success)
	{
        //to print the check the capacity is done
	    printf("-Check capacity is done\n");
        return e_failure;
        
	    if( (copy_bmp_header(encInfo->fptr_src_image,encInfo->fptr_stego_image)) == e_success )
	    {
            //to copying of header file is done 
		printf("-Copying of header file is done\n");

		if( (encode_magic_string( MAGIC_STRING,encInfo)) == e_success)
		{
            //to magic string encoding is done 
		    printf("-Magic string encoding is done\n");
		    strcpy(encInfo->extn_secret_file , strstr(encInfo->secret_fname, "."));
		    if( (encode_secret_file_extn_size( strlen(encInfo->extn_secret_file),encInfo->fptr_src_image,encInfo->fptr_stego_image))==e_success)
		    {
                //To encoding of secret file exctension size is done
			printf("-Encoding of secret file extension  size is done\n");
			if( (encode_secret_file_extn( encInfo->extn_secret_file, encInfo) )== e_success)
			{
                //to encoding of secret file extension is done 
			    printf("-Encoding of secret file extension is done\n");
			    if( (encode_secret_file_size( encInfo->size_secret_file, encInfo)) == e_success)
			    {
                    //to encoding of secret file size is done 
				printf("Info : Encoding of secret file size is done\n");
                
				if ( (encode_secret_file_data(encInfo)) == e_success)
				{
                    //to encoding of secret fiile data is done 
				    printf("Info : Encoding of secret file data is done\n");
				    
				}
			    }
			}
		    }
		}
	    }
	}
    }

//_____________________________________________________________________________________
/* check capacity function call ,that is checking the capacity weather it is suffiecient or not ,if its suffiecient return e_success*/
Status check_capacity(EncodeInfo *encInfo)
{
    /*finding image size */
    encInfo->image_capacity = get_image_size_for_bmp( encInfo->fptr_src_image);
    /* get the size of secrete file */
    encInfo->size_secret_file = get_file_size( encInfo->fptr_secret);
    /* header+(magic_string + secrete file_ext + _secrete file_ext + secrete file_ext_size)*/
    /* after that compare with image size */
    if(encInfo->image_capacity > 54+((2+4+4+4+25)*8))
    {
    //return e_success
	return e_success;
    }
    else
    {
    //return e_failure
    return e_failure;
    }

}
/*it will be the get file size*/
uint get_file_size(FILE *fptr)
{
    fseek(fptr,0,SEEK_END);
    //return ftell
    return ftell(fptr);
}

//-_______________________________________________________________________________________
/* copy bmp header we should not change the header ,if we dont get copy the header than data will get lost and
 image will get changed*/
 /* it will copy the data of header of the bmp file */
Status copy_bmp_header(FILE *fptr_src_image, FILE *fptr_dest_image)
{
    /* data is stored in stego ,and stored data will get copy from source to stego */

    

    /* rewind the both src and dest file pointers */
    char buffer[54];
fseek(fptr_src_image,0,SEEK_SET);
//src_image pointer
fread(buffer,1,54,fptr_src_image);
//dest_image pointer
    fwrite(buffer,1,54,fptr_dest_image);
    //return e_success
    return e_success;


    /* read 54 bytes dat from file */
    /* after reading write 54 bytes to the destination file */
    /* after that return e_success */
}

//______________________________________________________________________________________
/* it will take the data byte by byte */

Status encode_byte_to_lsb(char data, char *image_buffer)
{
    char ch;
    /*clear the lsb of image buffer */
    for(int i=0; i<8; i++)
    {
    /* get the 1 bit from the MSB of data */

        image_buffer[i]=(image_buffer[i] & 0xFE);
         
        ch=(data & (1<<(7-i))>> (7-i));

        /*merge the 1 bit MSB of data ,for merging we need to do the OR operation  */
        image_buffer[i]=image_buffer[i] | ch;

    }
}
//_________________________________________________________________

/*  calling magic string function first we ned to fing the lenght of magic string then need to do encoding */
 /* it will send the secret msg from magic string to image */
Status encode_magic_string(const char *magic_string, EncodeInfo *encInfo)
{
    
 encode_data_to_image(magic_string,strlen(MAGIC_STRING), encInfo->fptr_src_image,encInfo->fptr_stego_image);
   //return e_success
    return e_success;
}


//____________________________________________________________________
/* Encode secret file extenstion  it will tae the extention from secret file */
Status encode_secret_file_extn(const char *file_extn, EncodeInfo *encInfo)
{
 if((encode_data_to_image(encInfo->extn_secret_file,strlen(encInfo->extn_secret_file),encInfo->fptr_src_image,encInfo->fptr_stego_image)) == e_success)
    {
        //return  e_success
	return e_success;
    }
    else
    //return e_failure
	return e_failure;
}

//__________________________________________________________________________________________
/* Encode secret file size ,,it will take the size from secrete file */
Status encode_secret_file_size(long file_size, EncodeInfo *encInfo)
{
    if((encode_data_to_image(encInfo->extn_secret_file,strlen(encInfo->extn_secret_file),encInfo->fptr_src_image,encInfo->fptr_stego_image)) == e_success)
//return e_success
	return e_success;

    else
    {
    //return e_failure
	return e_failure;
    }
}
//________________________________________________________________________________
/*it will taken the secret file data encode*/
Status encode_secret_file_data(EncodeInfo *encInfo)
{
    int file_size=ftell(encInfo->fptr_secret);
    char buffer[file_size];
    fseek(encInfo->fptr_secret,0,SEEK_SET);
    fread(&buffer, file_size, 1, encInfo->fptr_secret);
    buffer[file_size]='\0';
    if( (encode_data_to_image(buffer,ftell(encInfo->fptr_secret),encInfo->fptr_src_image,encInfo->fptr_stego_image) ) == e_success)
	//return e_sucess
    return e_success;
}

//____________________________________________________________________________

//___________________________________________________________________
/*it will copy all the remaining data*/
Status copy_remaining_img_data(FILE *fptr_src, FILE *fptr_dest)
{
    char ch;
    while(fread(&ch,1,1,fptr_src))
    {
	fwrite(&ch,1,1,fptr_dest);
    }
}