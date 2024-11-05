#include <stdio.h>
#include "encode.h"
#include "types.h"
#include "common.h"

int main( int argc,char* argv[])
{
    EncodeInfo encInfo;
    uint img_size;

    // Fill with sample filenames
    encInfo.src_image_fname = argv[2];
    encInfo.secret_fname = argv[3];
    encInfo.stego_image_fname = "stego_img.bmp";

    // Test open_files
    if (open_files(&encInfo) == e_failure)
    {
    	printf("ERROR: %s function failed\n", "open_files" );
    	return 1;
    }
    else
    {
    	printf("SUCCESS: %s function completed\n", "open_files" );
    }

    // Test get_image_size_for_bmp
    img_size = get_image_size_for_bmp(encInfo.fptr_src_image);
    printf("INFO: Image size = %u\n", img_size);

    return 0;
}

/* calling function for check operation wheather encoding or decoding */


if(check_operation_type(argv[1])==e_encode)
{
   printf("encoding selected\n");
//encoding
if(do_encoding(&encInfo)==e_success)
{
        return e_success;

printf("encoding successfull\n");
}
}

else if(check_operation_type (argv[1])==e_decode)
{
        printf("decoding selected");
//decode//
if(do_decoding(&encInfo)==e_success)
{
        return e_success;

printf("decoding successfull\n");
}

}

else
{
        /*invalid input */
        printf("Invalid input \n");
}
return 0;



OperationType check_operation_type(char *argv[])
    {
            //checking encode or not/
            if(strcmp(argv[1],"-e")==0)
            {
                    return e_encode;
            }
            //checking decode or not
            else if(strcmp(argv[1],"-d")==0)
            {
                    return e_decode;
            }
            //invalid
            else
            {
                    return e_unsupported;
            }
    }
    