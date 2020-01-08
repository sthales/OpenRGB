/*-----------------------------------------*\
|  RGBController_Hue2.cpp                   |
|                                           |
|  Generic RGB Interface for NZXT Hue 2     |
|                                           |
|  Adam Honse (CalcProgrammer1) 12/29/2019  |
\*-----------------------------------------*/

#include "RGBController_Hue2.h"


RGBController_Hue2::RGBController_Hue2(Hue2Controller* hue2_ptr)
{
    hue2 = hue2_ptr;

    name = "NZXT Hue 2";

    type = DEVICE_TYPE_LEDSTRIP;
    
    mode led_mode;
    led_mode.name = "Custom";
    modes.push_back(led_mode);

    /*-------------------------------------------------*\
    | Set size of colors array                          |
    \*-------------------------------------------------*/
    unsigned int led_count = 0;
    for (unsigned int channel_idx = 0; channel_idx < HUE_2_NUM_CHANNELS; channel_idx++)
    {
        led_count += hue2->channel_leds[channel_idx];
    }
    colors.resize(led_count);

    /*-------------------------------------------------*\
    | Set zones and leds                                |
    \*-------------------------------------------------*/
    unsigned int led_idx = 0;
    for (unsigned int channel_idx = 0; channel_idx < HUE_2_NUM_CHANNELS; channel_idx++)
    {
        if(hue2->channel_leds[channel_idx] > 0)
        {
            zone* new_zone = new zone;

            char ch_idx_string[2];
            sprintf(ch_idx_string, "%d", channel_idx + 1);

            new_zone->name = "Hue 2 Channel ";
            new_zone->name.append(ch_idx_string);
            new_zone->type = ZONE_TYPE_LINEAR;

            std::vector<int> *new_zone_map = new std::vector<int>();

            for (unsigned int led_ch_idx = 0; led_ch_idx < hue2->channel_leds[channel_idx]; led_ch_idx++)
            {
                char led_idx_string[3];
                sprintf(led_idx_string, "%d", led_ch_idx + 1);

                led new_led;
                new_led.name = "Hue 2 Channel ";
                new_led.name.append(ch_idx_string);
                new_led.name.append(", LED ");
                new_led.name.append(led_idx_string);

                leds.push_back(new_led);
                leds_channel.push_back(channel_idx);

                new_zone_map->push_back(led_idx);
                led_idx++;
            }

            new_zone->map.push_back(*new_zone_map);
            zones.push_back(*new_zone);
            zones_channel.push_back(channel_idx);
        }
    }
}

int RGBController_Hue2::GetMode()
{
    return 0;
}

void RGBController_Hue2::SetMode(int /*mode*/)
{

}

void RGBController_Hue2::SetCustomMode()
{

}

void RGBController_Hue2::UpdateLEDs()
{
    for(std::size_t zone_idx = 0; zone_idx <= zones.size(); zone_idx++)
    {
        unsigned int channel = zones_channel[zone_idx];

        std::vector<RGBColor> channel_colors;

        for(std::size_t color = 0; color < colors.size(); color++)
        {
            if(leds_channel[color] == channel)
            {
                channel_colors.push_back(colors[color]);
            }
        }

        if(channel_colors.size() > 0)
        {
            hue2->SetChannelLEDs(channel, channel_colors);
        }
    }
}

void RGBController_Hue2::UpdateZoneLEDs(int zone)
{
    unsigned int channel = zones_channel[zone];

    std::vector<RGBColor> channel_colors;

    for(std::size_t color = 0; color < colors.size(); color++)
    {
        if(leds_channel[color] == channel)
        {
            channel_colors.push_back(colors[color]);
        }
    }

    if(channel_colors.size() > 0)
    {
        hue2->SetChannelLEDs(channel, channel_colors);
    }
}

void RGBController_Hue2::UpdateSingleLED(int led)
{
    unsigned int channel = leds_channel[led];

    std::vector<RGBColor> channel_colors;

    for(std::size_t color = 0; color < colors.size(); color++)
    {
        if(leds_channel[color] == channel)
        {
            channel_colors.push_back(colors[color]);
        }
    }

    if(channel_colors.size() > 0)
    {
        hue2->SetChannelLEDs(channel, channel_colors);
    }
}