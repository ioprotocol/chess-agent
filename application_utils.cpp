//
// Created by xushy on 18-7-2.
//
#include "application_utils.h"

#include <glibmm.h>
#include <opencv2/opencv.hpp>
#include <opencv2/imgcodecs.hpp>

std::string Hub::get_application_path() {
    std::string base_path = Glib::getenv(PWD);

    if(base_path.empty()) {
        base_path = Glib::get_current_dir();
    }
    return base_path;
}

std::string Hub::get_resources_img_path()
{
    return Glib::build_filename(get_application_path(), "resources", "img");
}

std::string Hub::get_resources_path()
{
    return Glib::build_filename(get_application_path(), "resources");
}

std::string Hub::get_resource_glade_file()
{
    return Glib::build_filename(get_application_path(), "resources", "app-main.glade");
}

cv::Mat Hub::pixbuffer_to_mat(GdkPixbuf *gdkPixbuf) {
    GOutputStream *outputStream;
    gpointer pixBuffer;
    gsize size;
    uchar *data;

    std::vector<int> params;
    params.push_back(cv::IMWRITE_JPEG_QUALITY);
    params.push_back(100);

    outputStream = g_memory_output_stream_new_resizable();

    gdk_pixbuf_save_to_stream(gdkPixbuf, outputStream, "jpeg", NULL, NULL, "quality", "100", NULL);

    pixBuffer = g_memory_output_stream_get_data(G_MEMORY_OUTPUT_STREAM(outputStream));
    size = g_memory_output_stream_get_size(G_MEMORY_OUTPUT_STREAM(outputStream));

    data = (uchar *) pixBuffer;

    std::vector<uchar> vec_data(&data[0], &data[0] + size);

    cv::Mat mat = cv::imdecode(vec_data, cv::IMREAD_COLOR);

    g_object_unref(gdkPixbuf);
    g_object_unref(outputStream);

    return mat;
}

