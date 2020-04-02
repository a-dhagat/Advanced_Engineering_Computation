#include <iostream>
#include <string.h>
#include "../simplebitmap/simplebitmap.h"
// #include "../simplebitap/simplebitmaptemplate.h"
// #include "simplebitmap.h"
#include <stdio.h>
#include <fstream>
#include <sstream>

int main(int argc, char* argv[])
{
    if (argc==1)
    {
        std::cout << "Usage: ps4_1 <pngFileName.png>" << std::endl;
        return 0;
    }

    FILE * fp;
    
    fp = fopen(argv[1], "rb");
    std::cout << fp << std::endl;
    if (fp==NULL)
        std::cout << "Error: Failed to read a .PNG file" << std::endl;
    
    SimpleBitmap sbmp, sbmp_clip;
    
    // Load the image and check if image loaded correctly
    bool var = sbmp.LoadPng(fp);
    std::cout << var << std::endl;

    // Get number of components per line (nx)
    int nx = sbmp.GetHeight();
    int ny = sbmp.GetWidth();
    std::cout << nx << std::endl;
    std::cout << ny << std::endl;
    // return 0;
    int count = 0;
    for (int thisX=0; thisX<nx; thisX+=40)
    {
        // printf("\n-----%d", thisX);
        for (int thisY=0; thisY<ny; thisY+=40)
        {
            if (count >= 200)
                break;
            // printf("\n\t-----%d", thisY);
            // printf("\n\nMain: %d\n\n", thisY);
            sbmp_clip = sbmp.CutOut(thisY, thisX, 40, 40);
            sbmp_clip.Invert();
            std::ostringstream ss;
            ss << count;
            std::string fname = ss.str() + ".png";
            const char *file_name = fname.c_str();
            FILE * fp_s;
            fp_s = fopen(file_name, "wb");

            // char file_name[30];
            // sprintf(file_name, "%d.png", count);
            // FILE* fp_s = fopen(file_name, "w");
            // count += 1;

            sbmp_clip.SavePng(fp_s);
            fclose(fp_s);
            count += 1;
            // break;
        }
        if (count>=200)
            break;
        
    }
    
    // sbmp.CutOut(sbmp.);
    fclose(fp);
    
    return 0;
}