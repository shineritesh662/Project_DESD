#include <stdio.h>
#include <unistd.h>

int main()
{
    FILE *file = fopen("/home/puneeth/Downloads/CDAC_Project/adxl_data.txt", "w");
    fclose(file);
    int i = 1;
    while(1)
    {
        FILE *file = fopen("/home/puneeth/Downloads/CDAC_Project/adxl_data.txt", "a");
        if (file != NULL)
        {
            fprintf(file, "i = %d\n", i);
            i++;
            sleep(1);
            //fprintf(file, "X: %d Y: %d Z: %d\n", acc_x, acc_y, acc_z);
            fclose(file);
        }
    }
}