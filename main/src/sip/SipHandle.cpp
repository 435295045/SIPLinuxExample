#include "SipHandle.hpp"
namespace sip
{
    

    /**
     * 初始化sdk
     */
    void init()
    {
        sip_sdk_config.sdk_run = SDK_TRUE;
        sip_sdk_config.port = 58581;
        sip_sdk_config.log_level = 4;
        sip_sdk_config.video_enable = SDK_TRUE;
        sip_sdk_config.video_out_auto_transmit = SDK_TRUE;
        sip_sdk_config.allow_multiple_connections = SDK_TRUE;
        static char *user_agent = "JHCloud-linux-1.0";
        sip_sdk_config.user_agent = user_agent;
        static char *data_dir = "/data";
        sip_sdk_config.data_dir = data_dir;
        sip_sdk_config.sdk_observer = &sdk_observer;
        sip_sdk_config.does_it_support_broadcast = SDK_TRUE;
        // 初始化媒体
        sipmedia::init();
        // 初始化sdk
        sip_sdk_init();
    }

    /**
     * 销毁sdk
     */
    void destroy()
    {
        sip_sdk_destroy();
    }

    /**
     * IP发生改变需要回调，或者重启sdk
     */
    void handleIpChange()
    {
        sip_sdk_handle_ip_change();
    }

    /**
     * 注册本地账号，注册到服务器
     */
    void registrar()
    {
        // 初始化本地账号（这里初始化username，会将本地sip消息体中from由原有的sip:+ip改为sip:+username)
        sip_sdk_local_config local_config = {
            .username = "test_username",
            .proxy = NULL,
            .proxy_port = 0,
            .enable_rate_control = SDK_FALSE,
            .start_keyframe_count = 120,
            .start_keyframe_interval = 1000,
        };
        local_account(local_config);
        // 注册到服务器
        // sip_header sh = {"jwt", "需要服务器获取"};
        // sip_sdk_registrar_config registrar_config = {
        //     .domain = "jhws.top",
        //     .username = (char *)username.c_str(),
        //     .password = "123456",
        //     .transport = "tcp",
        //     .server_addr = "test.jhws.top",
        //     .server_port = 58583,
        //     .headers = {sh},
        //     .proxy = "test.jhws.top",
        //     .proxy_port = 58583,
        //     .enable_rate_control = SDK_TRUE,
        //     .start_keyframe_count = 120,
        //     .start_keyframe_interval = 1000,
        // };
        // registrar_account(registrar_config);
    }

    /**
     * 解除注册到服务器
     */
    void unRegistrar()
    {
        unregistrar();
    }

    /**
     * 通过服务器呼叫
     * username: 对方用户名
     * headers: 自定义头信息
     */
    void call(std::string username, const std::map<std::string, std::string> &headers)
    {
        if (username.empty())
        {
            return;
        }
        /* 服务器呼叫 返回call_uuid */
        sip_sdk_call_param sscp = {
            SDK_CALL_TYPE_SERVER,
            (char *)username.c_str(),
            NULL,
        };
        size_t i = 0;
        for (const auto &header : headers)
        {
            sscp.headers[i].key = const_cast<char *>(header.first.c_str());
            sscp.headers[i].value = const_cast<char *>(header.second.c_str());
            i++;
        }
        make_call(&sscp);
        // 这里拿到呼叫uuid
    }

    /**
     * 通过服IP呼叫
     * ip: 对方IP
     * headers: 自定义头信息
     */
    void callIP(std::string ip, const std::map<std::string, std::string> &headers)
    {
        if (ip.empty())
        {
            return;
        }
        /* IP呼叫 返回call_uuid */
        sip_sdk_call_param sscp = {
            SDK_CALL_TYPE_IP,
            NULL,
            (char *)ip.c_str(),
        };
        size_t i = 0;
        for (const auto &header : headers)
        {
            sscp.headers[i].key = const_cast<char *>(header.first.c_str());
            sscp.headers[i].value = const_cast<char *>(header.second.c_str());
            i++;
        }
        make_call(&sscp);
        // 这里拿到呼叫uuid
    }

    /**
     * 接听呼叫
     * code: 接听状态码，正常接听200，先通媒体183
     */
    void answer(unsigned code)
    {
        /* 接听呼叫 */
        sip_sdk_answer_param answer_param = {
            code, // 接听状态码
            0,    // 呼叫uuid，为0表示接听所有被叫
        };
        answer_call(answer_param);
    }

    /**
     * 发送info消息
     * type: 消息类型
     * contentType: 内容类型
     * content: 内容（除自定义类型外、其他的类型内容只能是一个字节）
     */
    void sendDtmfInfo(int type, const std::string &contentType, const std::string &content)
    {
        sip_sdk_dtmf_info_param dtmf_info_param = {
            type,
            0,
            (char *)contentType.c_str(),
            (char *)content.c_str(),
        };
        send_dtmf_info(dtmf_info_param);
    }

    /**
     * 通过服务器发送sip message消息
     * username: 对方账号
     * content: 内容
     */
    void sendMessage(const std::string &username, const std::string &content)
    {
        sip_sdk_message_param message_param = {
            SDK_MESSAGE_TYPE_SERVER,
            (char *)username.c_str(),
            NULL,
            (char *)content.c_str(),
        };
        send_message(message_param);
    }

    /**
     * 通过IP发送sip message消息
     * username: 对方账号
     * content: 内容
     */
    void sendMessageIP(const std::string &ip, const std::string &content)
    {
        sip_sdk_message_param message_param = {
            SDK_MESSAGE_TYPE_IP,
            NULL,
            (char *)ip.c_str(),
            (char *)content.c_str(),
        };
        send_message(message_param);
    }

    /**
     * 挂断所有呼叫
     * code: 挂断状态码，正常挂断200
     */
    void hangup(unsigned code)
    {
        hangup(code, 0);
    }

    /**
     * 挂断 call_uuid 对应的呼叫
     * code: 挂断状态码，正常挂断200
     * uuid: call_uuid
     */
    void hangup(unsigned code, sdk_uuid_t uuid)
    {
        sip_sdk_hangup_param hangup_param = {code, uuid};
        hangup_call(hangup_param);
    }

    /**
     *  打印SDK信息，包括所有内存使用信息
     */
    void dump()
    {
        dump_info();
    }
}