#ifndef __SIP_SDK_H__
#define __SIP_SDK_H__

// 最大自定义消息头数量
#define SDK_MAX_CUSTOM_HEADERS 5

#ifdef __cplusplus
extern "C"
{
#endif

    typedef int sdk_bool_t;
    typedef int sdk_call_t;
    typedef int sdk_status_t;
    typedef int sdk_message_t;
    typedef int sdk_dtmf_info_t;
    typedef int sdk_input_video_t;
    typedef long unsigned int sdk_size_t;
    typedef long unsigned int sdk_uuid_t;
    typedef unsigned long long sdk_timestamp_t;

    enum sdk_constants_
    {
        /** Status is OK. */
        SDK_SUCCESS = 0,
        /** Status Common Error. */
        SDK_ERROR_COMMON = -1,

        /** True value. */
        SDK_TRUE = 1,

        /** False value. */
        SDK_FALSE = 0,

        /* IP 呼叫 */
        SDK_CALL_TYPE_IP = 0,

        /* 服务器呼叫 */
        SDK_CALL_TYPE_SERVER = 1,

        /* IP 消息 */
        SDK_MESSAGE_TYPE_IP = 0,

        /* 服务器消息 */
        SDK_MESSAGE_TYPE_SERVER = 1,

        /* 普通 info 消息 */
        SDK_DTMF_INFO_TYPE = 0,

        /* 自定义 info 消息 */
        SDK_DTMF_INFO_TYPE_CUSTOM = 1,

        /* 呼叫状态 */
        CALL_STATE_NULL = 0,
        /* 呼叫状态 正在呼叫 */
        CALL_STATE_CALLING = CALL_STATE_NULL + 1,
        /* 呼叫状态 正在被叫叫 */
        CALL_STATE_INCOMING = CALL_STATE_CALLING + 1,
        /* 呼叫状态 回铃音 */
        CALL_STATE_EARLY = CALL_STATE_INCOMING + 1,
        /* 呼叫状态 正在连接 */
        CALL_STATE_CONNECTING = CALL_STATE_EARLY + 1,
        /* 呼叫状态 连接成功 */
        CALL_STATE_CONFIRMED = CALL_STATE_CONNECTING + 1,
        /* 呼叫状态 断开连接 */
        CALL_STATE_DISCONNECTED = CALL_STATE_CONFIRMED + 1,

        /* 通知对方发送关键帧 */
        SDK_MEDIA_NOTIFICATION_SEND_KEYFRAME = 10000,
    };

    typedef enum
    {
        /** 亿智 arm 系列 */
        YI_ZHI_ARM = 0,
        /** 君正 MIPS ad 系列 */
        JUN_ZHENG_MIPS_AD = YI_ZHI_ARM + 1,
    } device_type;

    typedef enum
    {
        /** license 测试使用 */
        SDK_LICENSE_TYPE_TEST = 0,
        /** license 时间授权 */
        SDK_LICENSE_TYPE_AUTHORIZATION_TIME = SDK_LICENSE_TYPE_TEST + 1,
        /** license 永久授权 */
        SDK_LICENSE_TYPE_AUTHORIZATION_ALWAYS = SDK_LICENSE_TYPE_AUTHORIZATION_TIME + 1,
        /** license 内部使用 */
        SDK_LICENSE_TYPE_AUTHORIZATION_INTERNAL = SDK_LICENSE_TYPE_AUTHORIZATION_ALWAYS + 1,
    } license_type;

    typedef struct sip_header
    {
        char *key;
        char *value;
    } sip_header;

    typedef struct sip_sdk_local_config
    {
        char *username;                   // 用户名
        char *proxy;                      // 代理地址
        unsigned proxy_port;              // 代理端口
        sdk_bool_t enable_stream_control; // 流发送控制
        int stream_elapsed;               // 流经过时间
        unsigned start_keyframe_count;    // 开始关键帧数量
        unsigned start_keyframe_interval; // 开始关键帧时间间隔
    } sip_sdk_local_config;

    typedef struct sip_sdk_registrar_config
    {
        char *domain;                               // 域
        char *username;                             // 用户名
        char *password;                             // 密码
        char *transport;                            // transport
        char *server_addr;                          // 服务器地址
        unsigned server_port;                       // 服务器端口
        sip_header headers[SDK_MAX_CUSTOM_HEADERS]; // sip header
        char *proxy;                                // 代理地址
        unsigned proxy_port;                        // 代理端口
        sdk_bool_t enable_stream_control;           // 流发送控制
        int stream_elapsed;                         // 流经过时间
        unsigned start_keyframe_count;              // 开始关键帧数量
        unsigned start_keyframe_interval;           // 开始关键帧时间间隔
    } sip_sdk_registrar_config;

    typedef struct sip_sdk_call_param
    {
        sdk_call_t call_type;                       // 呼叫类型
        char *username;                             // 呼叫账号
        char *remote_ip;                            // 远程IP
        sip_header headers[SDK_MAX_CUSTOM_HEADERS]; // sip header
        sdk_uuid_t call_uuid;                       // 呼叫ID
        sdk_bool_t transmit_video;                  // 是否传输视频
        sdk_bool_t transmit_sound;                  // 是否传输声音
    } sip_sdk_call_param;

    typedef struct sip_sdk_answer_param
    {
        unsigned code;             // 接听状态码
        sdk_uuid_t call_uuid;      // 呼叫ID
        sdk_bool_t transmit_video; // 是否传输视频
        sdk_bool_t transmit_sound; // 是否传输声音
    } sip_sdk_answer_param;

    typedef struct sip_sdk_hangup_param
    {
        unsigned code;        // 挂断状态码
        sdk_uuid_t call_uuid; // 呼叫ID
    } sip_sdk_hangup_param;

    typedef struct sip_sdk_message_param
    {
        sdk_message_t message_type; // 消息类型
        char *username;             // 对方账号
        char *remote_ip;            // 远程IP
        char *content;              // 消息内容
    } sip_sdk_message_param;

    typedef struct sip_sdk_dtmf_info_param
    {
        sdk_dtmf_info_t dtmf_info_type; // info消息类型
        sdk_uuid_t call_uuid;           // 呼叫ID
        char *content_type;             // 内容类型
        char *content;                  // 消息内容
    } sip_sdk_dtmf_info_param;

    typedef struct sip_sdk_observer
    {
        void (*on_init_completed)(sdk_status_t state, const char *msg);
        void (*on_stop_completed)();
        void (*on_registrar_state)(sdk_status_t state);
        void (*on_incoming_call)(sip_sdk_call_param call_param);
        void (*on_dtmf_info)(sip_sdk_dtmf_info_param dtmf_info_param);
        void (*on_message)(sip_sdk_message_param message_param);
        void (*on_message_state)(sdk_status_t state, sip_sdk_message_param message_param);
        void (*on_call_state)(sdk_uuid_t call_uuid, sdk_status_t state);
    } sip_sdk_observer;

    typedef struct sip_sdk_common_config
    {
        unsigned port;                           // 端口
        int log_level;                           // 日志等级
        sdk_bool_t sdk_run;                      // 是否运行SDK
        char *user_agent;                        // user agent
        sdk_bool_t video_enable;                 // 是否启用视频
        sdk_bool_t video_out_auto_transmit;      // 视频输出自动传输
        sip_sdk_observer *sdk_observer;          // 状态回调
        sdk_bool_t allow_multiple_connections;   // 允许多个连接
        sdk_bool_t domain_name_direct_registrar; // 域名注册
        sdk_bool_t does_it_support_broadcast;    // 是否支持广播
    } sip_sdk_common_config;

    extern sip_sdk_common_config sip_sdk_config;

    sdk_status_t sip_sdk_register(const char *client_id,
                                  const char *client_secret,
                                  const char *device_uuid,
                                  const char *auth_file_path,
                                  const device_type dev_type,
                                  const license_type lic_type);

    sdk_status_t sip_sdk_init();

    sdk_status_t sip_sdk_destroy();

    sdk_status_t sip_sdk_handle_ip_change();

    sdk_status_t local_account(const sip_sdk_local_config local_config);

    sdk_status_t registrar_account(const sip_sdk_registrar_config registrar_config);

    sdk_status_t unregistrar();

    sdk_status_t make_call(sip_sdk_call_param *call_param);

    sdk_status_t answer_call(const sip_sdk_answer_param answer_param);

    sdk_status_t hangup_call(const sip_sdk_hangup_param hangup_param);

    sdk_status_t send_message(const sip_sdk_message_param message_param);

    sdk_status_t send_dtmf_info(const sip_sdk_dtmf_info_param dtmf_info_param);

    sdk_status_t dump_info();

#ifdef __cplusplus
}
#endif

#endif