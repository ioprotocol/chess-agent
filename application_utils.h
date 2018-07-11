//
// Created by xushy on 18-7-2.
//

#ifndef CHESSAGENT_APPLICATION_UTILS_H
#define CHESSAGENT_APPLICATION_UTILS_H

#include <iostream>

#define PWD "EPWD"

namespace Hub {

    std::string get_application_path();

    std::string get_resources_path();

    std::string get_resources_img_path();

    std::string get_resource_glade_file();
}

namespace Chess {
    enum Type {
        B_CHE = 10, B_MA = 11, B_XIANG = 12, B_SHI = 13, B_JIANG = 14, B_PAO = 15, B_ZU = 16,
        R_CHE = 0, R_MA = 1, R_XIANG = 2, R_SHI = 3, R_JIANG = 4, R_PAO = 5, R_ZU = 6
    };
}
#endif //CHESSAGENT_APPLICATION_UTILS_H
