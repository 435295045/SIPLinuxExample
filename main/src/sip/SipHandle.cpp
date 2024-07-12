#include "SipHandle.hpp"
namespace sip
{

    /**
     * 初始化结束回调
     */
    void on_init_completed(sdk_status_t state, const char *msg)
    {
        // 初始化成功可以去注册一下
        registrar();
    }

    /**
     * 初始化失败
     */
    void on_stop_completed()
    {
    }

    /**
     * 注册服务器状态回调
     *
     * 具体查阅sip状态码
     */
    void on_registrar_state(sdk_status_t state)
    {
    }

    /**
     * 收到呼叫
     */
    void on_incoming_call(sip_sdk_call_param call_param)
    {
        printf("on_incoming_call call_type: %d\n", call_param.call_type);
        printf("on_incoming_call username: %s\n", call_param.username);
        printf("on_incoming_call remote_ip: %s\n", call_param.remote_ip);
        for (int i = 0; i < SDK_MAX_CUSTOM_HEADERS; i++)
        {
            if (call_param.headers[i].key && call_param.headers[i].value)
            {
                printf("on_incoming_call header key: |%s|   value: |%s|\n", call_param.headers[i].key, call_param.headers[i].value);
            }
        }
        printf(" call_uuid: %lu\n", call_param.call_uuid);
    }

    /**
     * 收到dtmf info消息
     */
    void on_dtmf_info(sip_sdk_dtmf_info_param dtmf_info_param)
    {
        printf("dtmf_info_param: %lu content: %s\n", dtmf_info_param.call_uuid, dtmf_info_param.content);
    }

    /**
     * 收到sip message消息
     */
    void on_message(sip_sdk_message_param message_param)
    {
        printf("on_message message_type: %d\n", message_param.message_type);
        printf("on_message username: %s\n", message_param.username);
        printf("on_message remote_ip: %s\n", message_param.remote_ip);
        printf("on_message content: %s\n", message_param.content);
    }

    /**
     * 发送sip message消息状态回调
     */
    void on_message_state(sdk_status_t state, sip_sdk_message_param message_param)
    {
        printf("on_message_state state: %d\n", state);
        printf("on_message_state message_type: %d\n", message_param.message_type);
        printf("on_message_state username: %s\n", message_param.username);
        printf("on_message_state remote_ip: %s\n", message_param.remote_ip);
        printf("on_message_state content: %s\n", message_param.content);
    }

    /**
     * 呼叫状态回调
     *
     * 默认状态
     * CALL_STATE_NULL
     *
     * 呼叫状态 正在呼叫
     * CALL_STATE_CALLING
     *
     * 呼叫状态 正在被叫叫
     * CALL_STATE_INCOMING
     *
     * 呼叫状态 回铃音
     * CALL_STATE_EARLY
     *
     * 呼叫状态 正在连接
     * CALL_STATE_CONNECTING
     *
     * 呼叫状态 连接成功
     * CALL_STATE_CONFIRMED
     *
     * 呼叫状态 断开连接
     * CALL_STATE_DISCONNECTED
     */
    void on_call_state(sdk_uuid_t call_uuid, sdk_status_t state)
    {
    }

    /**
     * 注册回调函数
     */
    static sip_sdk_observer sdk_observer = {
        &on_init_completed,
        &on_stop_completed,
        &on_registrar_state,
        &on_incoming_call,
        &on_dtmf_info,
        &on_message,
        &on_message_state,
        &on_call_state,
    };

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
            .enable_stream_control = SDK_FALSE,
            .stream_elapsed = 2,
            .start_keyframe_count = 120,
            .start_keyframe_interval = 1000,
        };
        local_account(local_config);
        // 注册到服务器
        // sip_header sh = {"jwt", "12345"};
        // sip_sdk_registrar_config registrar_config = {
        //     .domain = "jhws.top",
        //     .username = "OD-1-1-1-1-0-3",
        //     .password = "123456",
        //     .transport = "tcp",
        //     .server_addr = "test.jhws.top",
        //     .server_port = 58583,
        //     .headers = {sh},
        //     .proxy = "test.jhws.top",
        //     .proxy_port = 58583,
        //     .enable_stream_control = SDK_FALSE,
        //     .stream_elapsed = 5,
        //     .start_keyframe_count = 10,
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
        sscp.transmit_sound = SDK_TRUE;
        sscp.transmit_video = SDK_TRUE;
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
        sscp.transmit_sound = SDK_TRUE;
        sscp.transmit_video = SDK_TRUE;
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
            SDK_TRUE,
            SDK_TRUE,
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