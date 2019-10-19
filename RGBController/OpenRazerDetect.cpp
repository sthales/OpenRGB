#include "RGBController.h"
#include "RGBController_OpenRazer.h"
#include <vector>
#include <stdio.h>
#include <stdlib.h>

#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>

/******************************************************************************************\
*                                                                                          *
*   DetectOpenRazerControllers                                                             *
*                                                                                          *
*       Detect devices supported by the OpenRazer kernel drivers                           *
*                                                                                          *                                                                                          *
\******************************************************************************************/

void DetectOpenRazerControllers(std::vector<RGBController*> &rgb_controllers)
{
    char driver_path[512];
    DIR *dir;
    struct dirent *ent;
    int test_fd;
    bool done = false;
    int driver_to_read = 0;

    while(driver_to_read < 8)
    {
        switch(driver_to_read)
        {
        case 0:
            strcpy(driver_path, "/sys/bus/hid/drivers/razerkbd/");
            break;

        case 1:
            strcpy(driver_path, "/sys/bus/hid/drivers/razermouse/");
            break;

        case 2:
            strcpy(driver_path, "/sys/bus/hid/drivers/razerfirefly/");
            break;

        case 3:
            strcpy(driver_path, "/sys/bus/hid/drivers/razermug/");
            break;

        case 4:
            strcpy(driver_path, "/sys/bus/hid/drivers/razercore/");
            break;

        case 5:
            strcpy(driver_path, "/sys/bus/hid/drivers/razerkraken/");
            break;
        
        case 6:
            strcpy(driver_path, "/sys/bus/hid/drivers/razermousemat/");
            break;
        
        case 7:
            strcpy(driver_path, "/sys/bus/hid/drivers/razerchromahdk/");
            break;
        }

        done = false;

        dir = opendir(driver_path);

        if(dir == NULL)
        {
            closedir(dir);

            driver_to_read++;
            continue;
        }

        ent = readdir(dir);

        while(ent != NULL)
        {
            if(ent->d_type == DT_DIR || ent->d_type == DT_LNK)
            {
                if(!strcmp(ent->d_name, "."))
                {
                    if(done == false)
                    {
                        done = true;
                    }
                    else
                    {
                        break;
                    }
                }
                else if(!strcmp(ent->d_name, "..") || !strcmp(ent->d_name, "module"))
                {
                }
                else
                {
                    int device_type;
                    char device_string[1024];
                    strcpy(device_string, driver_path);
                    strcat(device_string, ent->d_name);

                    RGBController_OpenRazer * razer_rgb = new RGBController_OpenRazer(device_string);

                    if(razer_rgb->device != RGBController_OpenRazer::RAZER_NO_DEVICE)
                    {
                        rgb_controllers.push_back(razer_rgb);
                    }
                    else
                    {
                        delete razer_rgb;
                    }
                    
                }
            }

            ent = readdir(dir);
        }

        closedir(dir);

        driver_to_read++;
    }

}   /* DetectOpenRazerControllers() */