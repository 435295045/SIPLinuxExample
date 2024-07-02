#include "sip_media_handle.h"

/**
 * 编解码器初始化
 */
static sdk_status_t codec_init(sdk_uuid_t call_uuid, void **user_data)
{
    printf("---------------------------------------------: %ld\n", call_uuid);
    /* 这里初始化编解码器 这里是文件中读取模拟编码器*/
    // SimulateCode *simulateCode = new SimulateCode("/data/test.h264");
    // // user_data 你的私有数据
    // *user_data = simulateCode;
    return SDK_SUCCESS;
}

/**
 * 编码回调
 * 这里放入H.264数据
 * user_data: 前面你的私有数据
 * buf: 回调buf，需要将 H.264 数据拷贝进来
 * buf_size: H.264 数据大小
 * is_keyframe: H.264 数据是否是关键帧，是：SDK_TRUE 不是: SDK_FLASE
 * required_keyframe: 需要强制返回关键帧
 */
// uint8_t *buffer = (uint8_t *)malloc(640 * 480);
// size_t buffer_size = 0;
// int frame_type = 0;
static sdk_status_t codec_encode(void *user_data,
                                 void *buf,
                                 unsigned *buf_size,
                                 sdk_bool_t *is_keyframe,
                                 sdk_bool_t required_keyframe)
{
    if (required_keyframe)
    {
        // 这里最好强制编码关键帧，这个时候对方客户端是需要关键帧的
    }
    // memset(buffer, 0, 640 * 480);
    // SimulateCode *simulateCode = (SimulateCode *)user_data;
    // // 从文件中读取帧
    // int ret = simulateCode->h264_readnalu(buffer, &buffer_size, &frame_type);
    // if (ret <= 0)
    // {
    //     printf("ReadNalu error 1\n");
    //     usleep(1000 * 1000);
    //     return SDK_ERROR_COMMON;
    // }
    // memcpy(buf, buffer, buffer_size);
    // *buf_size = buffer_size;
    // *is_keyframe = frame_type;
    return SDK_SUCCESS;
}

/**
 * 收到H.264数据
 * 这里将每路流的H.264数据保存到文件中
 * 可以安装 ffmpeg 使用ffplay播放
 *
 * call_uuid: 呼叫uuid,记录每一个呼叫
 * user_data: 前面你加入的私有数据
 * data: H.264 数据
 * data_size: H.264 数据大小
 */
sdk_status_t codec_decode(void *user_data, unsigned char *data, unsigned data_size)
{
    return 0; // 假设成功返回0，而不是-1
}

/**
 * 释放销毁编解码器
 * user_data: 你加入的私有数据
 **/
static sdk_status_t codec_deinit(void *user_data)
{
    // SimulateCode *simulateCode = (SimulateCode *)user_data;
    // delete simulateCode;
    return SDK_SUCCESS;
}

/**
 * 返回音频数据
 */
static audio_media_frame *audio_frame_from_stream()
{
    // 音频数据大小,需要根据实际大小填写
    // int size = 0;
    // 分配一个音频结构体
    // audio_media_frame *media_frame = alloc_audio_media_frame(size);
    // if (media_frame == NULL || media_frame->buf == NULL)
    // {
    //     goto error;
    // }

    // 加入数据
    // memcpy(media_frame->buf, buf, size);

    // 正常返回不需要释放，发生任何错误没有正常返回需要自己释放media_frame
    // return media_frame;
    return NULL;

error:
    // free_audio_media_frame(media_frame);
    return NULL;
}

/**
 * 接收音频数据并播放
 */
static sdk_status_t on_call_audio_media_stream(audio_media_frame media_frame)
{
    // media_frame.buf 数据
    // media_frame.size 数据大小
    // 播放成功返回SDK_SUCCESS，失败返回 SDK_ERROR_COMMON
    return SDK_ERROR_COMMON;
}

void media_init()
{
    /* 如果不启用编码，请使用以下配置（不启用编码可以省更多cpu与内存） */
    // sip_media_config.clock_rate = 90000;
    // sip_media_config.width = 10;
    // sip_media_config.height = 10;
    // sip_media_config.fps = 5;

    /* 正常使用编码，按照实际需求填写 */
    sip_media_config.clock_rate = 90000;
    sip_media_config.width = 640;
    sip_media_config.height = 480;
    sip_media_config.fps = 15;

    /* 音频时钟速率 */
    sip_media_config.audio_clock_rate = 16000;

    sip_media_config.video_op.codec_init = codec_init;
    sip_media_config.video_op.codec_deinit = codec_deinit;
    sip_media_config.audio_op.get_audio_frame_from_stream = audio_frame_from_stream;
    sip_media_config.audio_op.on_call_audio_media_stream = on_call_audio_media_stream;
    // 是否开启解码
    if (INIT_ENABLE_DECODE)
    {
        sip_media_config.not_enable_decode = SDK_FALSE;
        sip_media_config.video_op.codec_decode = codec_decode;
    }
    else
    {
        sip_media_config.not_enable_decode = SDK_TRUE;
    }
    // 是否开启编码
    if (INIT_ENABLE_ENCODE)
    {
        sip_media_config.not_enable_encode = SDK_FALSE;
        sip_media_config.video_op.codec_encode = codec_encode;
    }
    else
    {
        sip_media_config.not_enable_encode = SDK_TRUE;
    }
}
