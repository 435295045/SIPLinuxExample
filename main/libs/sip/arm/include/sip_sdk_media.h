#ifndef __SIP_SDK_MEDIA_H__
#define __SIP_SDK_MEDIA_H__

#include "sip_sdk.h"

#ifdef __cplusplus
extern "C"
{
#endif

    typedef struct video_media_frame
    {
        void *buf;       // 媒体数据
        sdk_size_t size; // buf size
        unsigned w;      // 宽
        unsigned h;      // 高
    } video_media_frame;

    typedef struct audio_media_frame
    {
        void *buf;       // 媒体数据
        sdk_size_t size; // buf size
    } audio_media_frame;

    typedef struct sip_video_op
    {
        sdk_status_t (*codec_init)(void **user_data);  // 初始化编解码器
        sdk_status_t (*codec_deinit)(void *user_data); // 销毁编解码器
        sdk_status_t (*codec_encode)(void *user_data,
                                     void *buf,
                                     unsigned *buf_size,
                                     sdk_bool_t *is_keyframe,
                                     sdk_bool_t required_keyframe); // 回调编码帧
        sdk_status_t (*codec_decode)(sdk_uuid_t call_uuid,
                                     void *user_data,
                                     unsigned char *data,
                                     unsigned data_size); // 回调h.264数据
    } sip_video_op;

    typedef struct sip_audio_op
    {
        sdk_status_t (*on_call_audio_media_stream)(audio_media_frame media_frame); // 回调音频数据
        audio_media_frame *(*get_audio_frame_from_stream)();                       // 获取音频数据
    } sip_audio_op;

    typedef struct sip_sdk_media_config
    {
        int clock_rate;               // 时钟速率
        int width;                    // 宽
        int height;                   // 高
        int fps;                      // 帧率
        sip_video_op video_op;        // 视频操作
        sip_audio_op audio_op;        // 音频操作
        sdk_bool_t not_enable_encode; // 不启用编码（关闭可以省下一些内存）
        sdk_bool_t not_enable_decode; // 不启用解码（关闭可以省下一些内存）
    } sip_sdk_media_config;

    extern sip_sdk_media_config sip_media_config;

    audio_media_frame *alloc_audio_media_frame(sdk_size_t size);

    sdk_status_t free_audio_media_frame(audio_media_frame *media_frame);

#ifdef __cplusplus
}
#endif

#endif