#include <math.h>
#include <stdio.h>

#define STB_IMAGE_IMPLEMENTATION
#include "./headers/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "./headers/stb_image_write.h"


unsigned char * uc_arrayNew_1d ( int _size )
{
    return ( unsigned char *) calloc (_size , sizeof ( unsigned char ));
}

unsigned char *subtract_image ( unsigned char *bg1, unsigned char* bg2, unsigned char* observed_image , int width , int height ,
        int channel , float accuracy )
{
    unsigned char * extracted_image = uc_arrayNew_1d ( width * height * channel );
    for (int i = 0; i < height ; ++i)
    {   
        for (int j = 0; j < width ; ++j)
        {
            float diff = 0;
            for ( int k = 0; k < channel ; k++)
            {
                int index = (i * width + j) * channel + k;
                diff += abs(bg1[index] - observed_image[index]);
            }

            if (diff > accuracy * 3) {
                for ( int k = 0; k < channel ; k++)
                {
                    int index = (i * width + j) * channel + k;
                    extracted_image[index] = observed_image[index];
                }
            }
        }
    }

    unsigned char * new_image = uc_arrayNew_1d ( width * height * channel );
    for (int i = 0; i < height ; ++i)
    {   
        for (int j = 0; j < width ; ++j)
        {
            int pixel_index = (i * width + j) * channel;
            if (extracted_image[pixel_index] == 0)
            {
                for (int k = 0; k < channel; k++)
                {
                    new_image[pixel_index + k] = bg2[pixel_index + k];
                }
            }
            else
            {
                for (int k = 0; k < channel; k++)
                {
                    new_image[pixel_index + k] = extracted_image[pixel_index + k];
                }
            }
        }
    }

    return new_image;
}


int main ()
{

    int width , height , channel;
    char bg1_path[] = "./images1/background.jpg";
    char bg2_path[] = "./images1/new_background.jpg";
    char observed_image_path[] = "./images1/foreground.jpg";
    char result_path[] = "./images1/new_image.jpg";
    
    unsigned char * bg1 = stbi_load ( bg1_path , &width , &height , & channel , 0);
    unsigned char * bg2 = stbi_load ( bg2_path , &width , &height , & channel , 0);
    unsigned char * observe_image = stbi_load ( observed_image_path , &width , &height , & channel , 0);
    printf ("Width = %d \nHeight = %d\nChannel = %d\n", width , height , channel );
    
    unsigned char * new_image = subtract_image(bg1, bg2, observe_image, width, height, channel, 50);

    stbi_write_png ( result_path , width , height , channel , new_image , width * channel );
    printf (" New image saved to %s\n", result_path );

    return 0;
}
