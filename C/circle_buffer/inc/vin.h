//
// Created by alan on 18-6-21.
//

#ifndef HIMPP_VDEC_DEMO_VIN_H
#define HIMPP_VDEC_DEMO_VIN_H


#include <type.h>
#define  MAX_CHANNEL_COUNT  256

struct vin_channel_attribute {
    int         channel;   //通道号
    int         width;
    int         height;
    int         format;
    SOURCE      source; //数据来源:[DISK NETWORK CAMERA]
};

struct vin_channel {
    struct vin_channel_attribute attr;
    void *data;
    size_t size;
};

struct vin_channels {
    int count;
    struct vin_channel channel[MAX_CHANNEL_COUNT];
};

int     vin_open();
int     vin_start();

struct vin_channels * vin_get_channels();

int     vin_channels_add(struct vin_channels *dest_channels, struct vin_channels *src_channels);

int vin_read(int id, void *buf);

int     vin_close();

#endif //HIMPP_VDEC_DEMO_VIN_H
