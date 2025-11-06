/**************************************************************************

           Copyright(C), tao.jing All rights reserved

 **************************************************************************
   File   : TLog.cpp
   Author : tao.jing
   Date   : 2023/12/6
   Brief  : 
**************************************************************************/
#include "TLog.h"
#include "TException.h"
#include "TSysUtils.h"
#include <iostream>
#include "loguru.cpp"


namespace TBase {


    void initDateLog(int argc, char *argv[]) {

        std::cout << "Init app log..." << std::endl;
        loguru::init(argc, argv);

        // Create Log directory
        std::string cur_work_dir = TBase::getCurWorkDir();
        std::string log_dir = TBase::joinPath(
                std::vector<std::string>{
                        cur_work_dir,
                        "Log"
                });

        if (!TBase::dirExists(log_dir)) {
            TBase::createDir(log_dir);
        }

        // Create log file
        std::string cur_date = TBase::getDate('_');
        std::string log_file_name = cur_date + std::string(".log");
        std::string log_file_path = TBase::joinPath(log_dir, log_file_name);

        if (!loguru::add_file(log_file_path.c_str(), loguru::Append, loguru::Verbosity_INFO)) {
            throw TBaseException("Init app log failed. (AppMonitor::initAppLog)");
        } else {
            LOG_F(INFO, "Init app log %s.", log_file_path.c_str());
        }
    }


};





