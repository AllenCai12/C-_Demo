//
// Created by alan on 18-6-21.
//

#ifndef HIMPP_VDEC_DEMO_VIN_FILE_H
#define HIMPP_VDEC_DEMO_VIN_FILE_H

#include "vin.h"






int vin_file_open();

int vin_file_get_channels(struct vin_channels *channels);

size_t vin_file_read(int id, void *buf);

void vin_file_close();

#endif //HIMPP_VDEC_DEMO_VIN_FILE_H
