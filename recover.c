#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    //check to see if the user entered input
    if (argc < 2)
    {
        printf("Usage: ./recover image\n");
        return 1;
    }

    //open memory card
    FILE *card = fopen(argv[1], "r");

    //a counter that will count how many jpeg files we have foun so far
    int counter = 0;
    //create a file with a global varuable 
    FILE *img = NULL;
    
    //a buffer to keep the 512 bits of memory for now
    unsigned char temparray[512];
    //as long as fread returns the amounts of bytes we want keep looping
    while (fread(temparray, sizeof(unsigned char), 512, card) == 512)
    {
        
        
        //see if this byte of memory is the begining of a jpeg
        if (temparray[0] == 0xff && temparray[1] == 0xd8 && temparray[2] == 0xff && (temparray[3] & 0xf0) == 0xe0)
        {
            //if it's not the first jpeg we have fount then close the last file
            if (counter > 0)
            {
                fclose(img);
            }
                
            char tempfilename[8];
            sprintf(tempfilename, "%03i.jpg", counter);
            img = fopen(tempfilename, "w");
            //copy the information into the new file
            fwrite(temparray, 512, 1, img);
            
            //add one to the counter to indecate we found a new jpeg
            counter++;
                


        }
        //if this byte of information is part of the last jpeg then keep copying
        else if (counter > 0)
        {
            fwrite(temparray, 512, 1, img);
        }

    }
    fclose(img);
    fclose(card);
}