//*
// Author James Meehan
// PSET4 Resize (more comfortable)
//* 

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "bmp.h"

int main(int argc, char *argv[])
{
    // ensure positive integer is entered
    float n = atof(argv[1]);

    if (n < 0.0 || n > 100.0)
    {
        fprintf(stderr, "Usage: Please enter a positive number bewteen 1 and 100\n");
        return 1;
    }

    // ensure proper usage
    if (argc != 4)
    {
        fprintf(stderr, "Usage: ./copy factor infile outfile\n");
        return 1;
    }

    // remember filenames
    char *infile = argv[2];
    char *outfile = argv[3];

    // open input file 
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 2;
    }

    // open output file
    FILE *outptr = fopen(outfile, "w");
    if (outptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr, "Could not create %s.\n", outfile);
        return 3;
    }

    // read infile's BITMAPFILEHEADER
    BITMAPFILEHEADER bf;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);
    

    // read infile's BITMAPINFOHEADER
    BITMAPINFOHEADER bi;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);
    
    // determine padding for scanlines
    int orig_padding = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    int orig_width = bi.biWidth;
    int orig_height = bi.biHeight;
    
    // ensure infile is (likely) a 24-bit uncompressed BMP 4.0
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 || 
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        fprintf(stderr, "Unsupported file format.\n");
        return 4;
    }
    
    // resize variables for outfile
     bi.biWidth *= n;
     bi.biHeight *= n;
    
     // determine padding for scanlines
    int padding = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
  
    //resize biSizeImage and bfSize
    bi.biSizeImage = ((sizeof(RGBTRIPLE) * bi.biWidth) + padding)*(abs(bi.biHeight));
    bf.bfSize = bi.biSizeImage + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);


    // write outfile's BITMAPFILEHEADER
    fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, outptr);

    // write outfile's BITMAPINFOHEADER
    fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, outptr);
    
    
    // if image is being downsized
    if (n < 1.0)
    {
        // iterate over infile's scanlines
        for (int i = 0; i < abs(bi.biHeight); i++ ) 
    {

            // iterate over pixels in scanline
            for (int j = 0; j < bi.biWidth; j++)
            {
                // temporary storage
                RGBTRIPLE triple;

                // read RGB triple from infile
                fread(&triple, sizeof(RGBTRIPLE), 1, inptr);
            
                // write RGB triple to outfile
                fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
                
                //Number of pixels to skip over
                fseek(inptr, (1/n-1)*sizeof(RGBTRIPLE), SEEK_CUR);
                
                } 
        
            // add correct padding for resized image
            for (int k = 0; k < padding; k++)
            {
                fputc(0x00, outptr);
            }
            
          // skip over remaining padding and next line  
            fseek(inptr, (orig_padding + (orig_width * 3) + orig_padding), SEEK_CUR);
            

    }
    
    // if image is being enlarged
    }
    else
    {
    for (int i = 0; i < abs(orig_height); i++)
    {
        float x = (sizeof(RGBTRIPLE)*orig_width);
        if (i !=0)
        {
            // move cursor to next line
            fseek(inptr, (x + orig_padding), SEEK_CUR);
        }
        
        for (int z = 0; z < n; z++)
        {
            
            
        
            // iterate over pixels in scanline
            for (int j = 0; j < orig_width; j++)
            {
                // temporary storage
                RGBTRIPLE triple;

                // read RGB triple from infile
                fread(&triple, sizeof(RGBTRIPLE), 1, inptr);
            
                for (int k = 0; k < n; k++)
                {
                // write RGB triple to outfile
                fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
                }
            
            } 
        
          // add proper padding for resized image
            for (int k = 0; k < padding; k++)
            {
                fputc(0x00, outptr);
            }
            
            
        // move cursor back to start of line    
        fseek(inptr, -x, SEEK_CUR);
        }    
    }
    }
    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // success
    return 0;
} 
