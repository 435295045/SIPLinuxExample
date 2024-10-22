#ifndef __SIP_SDK_MEDIA_H__
#define __SIP_SDK_MEDIA_H__

#include "sip_sdk.h"

#ifdef __cplusplus
extern "C"
{
#endif

    typedef struct video_media_config
    {
        int fps;             // 帧率
        int width;           // 宽
        int height;          // 高
        int min_block_datas; // 数分块最小数量
    } video_media_config;

    typedef struct audio_media_frame
    {
        void *buf;       // 媒体数据
        sdk_size_t size; // buf size
    } audio_media_frame;

    typedef struct sip_video_op
    {
        sdk_status_t (*codec_init)(sdk_uuid_t call_uuid,
                                   video_media_config *vid_media_config,
                                   void **user_data);  // 初始化编解码器
        sdk_status_t (*codec_deinit)(void *user_data); // 销毁编解码器
        sdk_status_t (*codec_encode)(void *user_data,
                                     sdk_timestamp_t timestamp,
                                     void *buf,
                                     unsigned *buf_size,
                                     sdk_bool_t *is_keyframe,
                                     sdk_bool_t required_keyframe); // 回调编码帧
        sdk_status_t (*codec_decode)(void *user_data,
                                     sdk_timestamp_t timestamp,
                                     unsigned char *data,
                                     unsigned data_size); // 回调h.264数据
    } sip_video_op;

    typedef struct sip_audio_op
    {
        sdk_status_t (*on_call_audio_media_stream)(audio_media_frame media_frame); // 回调音频数据
        audio_media_frame *(*get_audio_frame_from_stream)();                       // 获取音频数据
    } sip_audio_op;

    typedef struct sip_sdk_media_h264_fmtp
    {
        char profile_level_id[20];
        char packetization_mode[10];
    } sip_sdk_media_h264_fmtp;
    typedef struct sip_sdk_media_config
    {
        int audio_clock_rate;              // 音频时钟速率（默认 16000)
        float mic_gain;                    // mic增益（默认 1)
        float speaker_gain;                // speaker增益（默认 1)
        sdk_bool_t ns_enable;              // 是否启用噪声抑制（默认 SDK_TRUE）
        sdk_bool_t agc_enable;             // 是否启用自动增益（默认 SDK_TRUE）
        sdk_bool_t aec_enable;             // 是否启用回音消除（默认 SDK_TRUE）
        sip_video_op video_op;             // 视频操作
        sip_audio_op audio_op;             // 音频操作
        sdk_bool_t not_enable_encode;      // 不启用编码（关闭可以省下一些内存）（默认 SDK_FALSE)
        sdk_bool_t not_enable_decode;      // 不启用解码（关闭可以省下一些内存）（默认 SDK_FALSE)
        sip_sdk_media_h264_fmtp h264_fmtp; // H264 fmtp
    } sip_sdk_media_config;

    extern sip_sdk_media_config sip_media_config;

    audio_media_frame *alloc_audio_media_frame(sdk_size_t size);

    sdk_status_t free_audio_media_frame(audio_media_frame *media_frame);

#ifdef __cplusplus
}
#endif

#endif