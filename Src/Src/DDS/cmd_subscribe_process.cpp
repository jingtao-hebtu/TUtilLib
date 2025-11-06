/**************************************************************************

           Copyright(C), 2016-2026, tao.jing All rights reserved

 **************************************************************************
   File   : cmd_subscribe_process.cpp
   Author : tao.jing
   Date   : 2022/9/12
   Brief  : 
**************************************************************************/
#include "cmd_subscribe_process.h"
#include "mm_dds_manager.h"
#include "mm_data_manager.h"
#include "dds/dds.h"
#include "MMCMD.h"
#include "mm_object_ring_buffer.h"
#include "comm_sdk_api.h"
#include "mm_protocol.h"
#include "comm_log.h"
#include "mm_host.h"
#include <iostream>


#define MAX_SAMPLES 1

namespace mm_comm
{
    void sync_process_mm_cmd(const std::string& transfer_name,
                             MMCMD_MSG* mm_cmd_msg)
    {
        if (mm_cmd_msg->msg_type != 0)
        {
            return;
        }

        t_comm::stream_transfer* _transfer = mm_comm::mm_host::instance()->get_transfer();

        if (_transfer == nullptr)
        {
            log_e("[sync_process_mm_cmd] Invalid host transfer.");
            return;
        }

        t_comm::mm_pro_data_desc_t data_desc;
        data_desc.src_id = mm_cmd_msg->src_id;
        data_desc.dst_id = mm_cmd_msg->dst_id;
        data_desc.module_id = mm_cmd_msg->module_id;
        data_desc.func_id = mm_cmd_msg->func_id;
        data_desc.ack1 = mm_cmd_msg->ack1;
        data_desc.ack2 = mm_cmd_msg->ack2;

        _transfer->send_packet(reinterpret_cast<unsigned char *>(mm_cmd_msg->data),
                               mm_cmd_msg->data_len,
                               reinterpret_cast<uint8_t*>(&data_desc),
                               sizeof(data_desc));
    }

    void cmd_subscribe_process_func()
    {
        mm_object_ring_buffer* cmd_rbuf;
        while (mm_dds_manager::instance()->get_cmd_sub_switch())
        {
            cmd_rbuf = mm_data_manager::instance()->get_mmcmd_rbuf();
            if (cmd_rbuf == nullptr)
            {
                // No eeg ring buffer
                dds_sleepfor(DDS_MSECS(10));
                continue;
            }
            else
            {
                break;
            }
        }

        dds_return_t rc;
        void *samples[MAX_SAMPLES];
        dds_sample_info_t infos[MAX_SAMPLES];
        samples[0] = MMCMD_MSG__alloc();

        std::int32_t cmd_reader = mm_dds_manager::instance()->get_cmd_sub_reader();
        while (mm_dds_manager::instance()->get_cmd_sub_switch())
        {
            rc = dds_take(cmd_reader, samples,
                          infos, MAX_SAMPLES, MAX_SAMPLES);
            if (rc < 0)
                log_e("dds_read: %s", dds_strretcode(-rc));

            if ((rc > 0) && (infos[0].valid_data))
            {
                sync_process_mm_cmd("mm_terminal_1",
                                    reinterpret_cast<MMCMD_MSG*>(samples[0]));
            }
            else
            {
                dds_sleepfor(DDS_MSECS (20));
            }
        }
        MMCMD_MSG_free(samples[0], DDS_FREE_ALL);
    }
};
