//
// Created by xushy on 18-7-2.
//
#include "application_utils.h"

#include <glibmm.h>
#include <giomm.h>
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

void Hub::mat_to_pixbuffer(cv::Mat &mat, GdkPixbuf *gdkPixbuf) {
    std::vector<uchar> data_encode;
    std::vector<int> params;
    params.push_back(cv::IMWRITE_JPEG_QUALITY);
    params.push_back(100);
    cv::imencode(".jpeg", mat, data_encode, params);

    GBytes* buffer = g_bytes_new(data_encode.data(), data_encode.size());

    GInputStream* inputStream = g_memory_input_stream_new_from_bytes(buffer);
    gdkPixbuf = gdk_pixbuf_new_from_stream(inputStream, NULL, NULL);

    g_bytes_unref(buffer);
    g_object_unref(inputStream);
}

void Hub::mat_to_pixbuffer(cv::Mat &mat, Glib::RefPtr<Gdk::Pixbuf> pixbuf) {
    std::vector<uchar> data_encode;
    std::vector<int> params;
    params.push_back(cv::IMWRITE_JPEG_QUALITY);
    params.push_back(100);
    cv::imencode(".jpeg", mat, data_encode, params);

    gconstpointer data = (gconstpointer) data_encode.data();
    Glib::RefPtr<Glib::Bytes> gbytes = Glib::Bytes::create(data, data_encode.size());
    Glib::RefPtr<Gio::MemoryInputStream> inputStream = Gio::MemoryInputStream::create();
    inputStream->add_bytes(gbytes);

    Glib::RefPtr<Gdk::Pixbuf> refPtr = Gdk::Pixbuf::create_from_stream(inputStream);
    refPtr->copy_area(0, 0, refPtr->get_width(), refPtr->get_height(), pixbuf, 0, 0);
}

cv::Point Chess::uint32_to_point(guint32 position) {
    return cv::Point(position % 10000, position / 10000);
}

guint32 Chess::point_to_uint32(cv::Point point) {
    return point_to_uint32(point.x, point.y);
}

guint32 Chess::point_to_uint32(gint x, gint y) {
    return y * 10000 + x;
}

gboolean Chess::is_same_position(guint32 point1, guint32 point2) {
    return get_distance_by_position(point1, point2) < 6;
}

gdouble Chess::get_distance_by_position(guint32 point1, guint32 point2) {
    gint x1 = point1 % 10000;
    gint y1 = point1 / 10000;
    gint x2 = point2 % 10000;
    gint y2 = point2 / 10000;
    double distance = sqrt(abs(x1 - x2) * abs(x1 - x2) + abs(y1 - y2) * abs(y1 - y2));
    return distance;
}

gdouble Chess::get_distance_by_position(cv::Point point1, cv::Point point2) {
    return get_distance_by_position(point_to_uint32(point1), point_to_uint32(point2));
}

std::string Chess::get_chess_name(gint type) {
    std::string prefix;
    std::string name;
    if (type >= 10) {
        prefix = "黑";
        type = type - 10;
    } else {
        prefix = "红";
    }
    switch (type) {
        case R_CHE:
            name = "车";
            break;
        case R_MA:
            name = "马";
            break;
        case R_XIANG:
            name = "相";
            break;
        case R_SHI:
            name = "士";
            break;
        case R_JIANG:
            name = "将";
            break;
        case R_PAO:
            name = "炮";
            break;
        case R_ZU:
            name = "卒";
            break;
    }
    return prefix + name;
}
