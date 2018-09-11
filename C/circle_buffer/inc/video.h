//
// Created by alan on 18-6-21.
//

#ifndef HIMPP_VDEC_DEMO_VIDEO_PROCESSING_H
#define HIMPP_VDEC_DEMO_VIDEO_PROCESSING_H

#ifdef __cplusplus
extern "C" {
#endif

struct rect {
    int x;
    int y;
    int width;
    int height;
};

struct sub_screen_spec {
    int id;
    struct rect area;
};


int video_open();

int video_set_screen_model(struct sub_screen_spec *screen);

int video_get_screen_attribute(struct rect *rect);

int video_start();

int video_close();

#ifdef __cplusplus
}
#endif


#endif //HIMPP_VDEC_DEMO_VIDEO_PROCESSING_H
