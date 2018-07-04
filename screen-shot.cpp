//
// Created by xsy on 18-7-4.
//

#include "screen-shot.h"

#include <unistd.h>
#include <time.h>

#include <gtk/gtk.h>

#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>

#include <iostream>

#include "screenshot-utils.h"
#include "screenshot-config.h"

ScreenShot::ScreenShot() {
    init_screen_config();
}

ScreenShot::~ScreenShot() {
    g_free(screenshot_config);
}

void ScreenShot::run() {
    GOutputStream *outputStream;
    GdkPixbuf *gdkPixbuf;
    gpointer pixBuffer;
    gsize size;
    uchar *data;
    std::vector<int> params;

    params.push_back(cv::IMWRITE_JPEG_QUALITY);
    params.push_back(100);

    outputStream = g_memory_output_stream_new_resizable();
    gdkPixbuf = screenshot_fallback_get_pixbuf(NULL);

    gdk_pixbuf_save_to_stream(gdkPixbuf, outputStream, "jpeg", NULL, NULL, "quality", "100", NULL);

    pixBuffer = g_memory_output_stream_get_data(G_MEMORY_OUTPUT_STREAM(outputStream));
    size = g_memory_output_stream_get_size(G_MEMORY_OUTPUT_STREAM(outputStream));

    data = (uchar *) pixBuffer;

    std::vector<uchar> vec_data(&data[0], &data[0] + size);

    cv::Mat mat = cv::imdecode(vec_data, cv::IMREAD_COLOR);

    mat.release();
    g_object_unref(gdkPixbuf);
    g_object_unref(outputStream);

    time_t timep;
    time(&timep);
    std::cout << "screen shot" << asctime(gmtime(&timep)) << std::endl;
}