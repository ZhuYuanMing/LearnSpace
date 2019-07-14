#include "./EditableVideo.h"
EditableVideo::~EditableVideo(){
    currentMatPointer.reset();
    av_free(buffer);
    av_free(currentFrameRGB);

    av_free(currentFrame);
    avcodec_close(pCodecContext_video);
    avformat_close_input(&pFormatContext);

}

EditableVideo::EditableVideo(std::string srcPath) {
    pFormatContext = avformat_alloc_context();
    if (!pFormatContext) {
        printf("ERROR could not allocate memory for Format Context");
        exit(1);
    }
    if (avformat_open_input(&pFormatContext, srcPath.c_str(), nullptr, nullptr) != 0) {
        printf("ERROR could not open the file");
        exit(1);
    }


    this->duration = pFormatContext->duration;
    this->playing = false;
    this->currentFrame = av_frame_alloc();
    this->currentFrameRGB = av_frame_alloc();

    //malloc a part of memory to store original data while converting


    printf("format %s, duration %lld us, bit_rate %lld", pFormatContext->iformat->name, pFormatContext->duration, pFormatContext->bit_rate);
    if (avformat_find_stream_info(pFormatContext,  nullptr) < 0) {
        printf("ERROR could not get the stream info");
        exit(1);
    }
    //find first video stream
    AVCodec *pCodec = nullptr;

    for (int i = 0; i < pFormatContext->nb_streams; i++){
        AVCodecParameters *pLocalCodecParameters =  nullptr;
        pLocalCodecParameters = pFormatContext->streams[i]->codecpar;
        AVCodec *pLocalCodec = nullptr;
        // finds the registered decoder for a codec ID
        pLocalCodec = avcodec_find_decoder(pLocalCodecParameters->codec_id);
        if (pLocalCodec==nullptr) {
            printf("ERROR unsupported codec!");
            exit(1);
        }
        // when the stream is a video we store its index, codec parameters and codec
        if(pLocalCodecParameters->codec_type == AVMEDIA_TYPE_VIDEO){
            if (this->video_stream_index == -1) {
                this->video_stream_index = i;
                pCodec = pLocalCodec;
                this->pCodecContext_video = pFormatContext->streams[i]->codec;
                if(avcodec_open2(pCodecContext_video, pCodec, 0) < 0){ //open video decoder
                    throw "error";
            }
                this->VideoFps = (int)av_q2d(pFormatContext->streams[i]->r_frame_rate);
                this->timeBase = (int64_t(pCodecContext_video->time_base.num) * AV_TIME_BASE) / int64_t(pCodecContext_video->time_base.den);
                printf("VideoFps is %d",this->VideoFps);
            }
        }

        //audio codec
        if (pLocalCodecParameters->codec_type == AVMEDIA_TYPE_AUDIO) {
            if (this->audio_stream_index == -1) {
                this->audio_stream_index = i;
            }
            this->pCodecContext_audio = pFormatContext->streams[i]->codec;
            printf("Audio Codec: %d channels, sample rate %d", pLocalCodecParameters->channels, pLocalCodecParameters->sample_rate);
        }

        // print its name, id and bitrate
        //printf("\tCodec %s ID %d bit_rate %lld", pLocalCodec->name, pLocalCodec->id, pCodecParameters->bit_rate);
    }

    numBytes = avpicture_get_size(AV_PIX_FMT_RGB24, pCodecContext_video->width, pCodecContext_video->height);
    buffer = (uint8_t *)av_malloc( numBytes*sizeof(uint8_t));
    avpicture_fill((AVPicture *)currentFrameRGB, buffer, AV_PIX_FMT_RGB24,pCodecContext_video->width, pCodecContext_video->height);
}

std::shared_ptr<cv::Mat> EditableVideo::getNextImage() {
    int ret = AVERROR(EAGAIN) ;
    while(ret == AVERROR(EAGAIN)){
        if(av_read_frame(this->pFormatContext, &this->currentPacket) >= 0){
            if(this->currentPacket.stream_index == this->video_stream_index){ //judge if video data
                ret = avcodec_send_packet(pCodecContext_video, &currentPacket);
                if(ret < 0){
                    fprintf(stderr, "Error sending a packet for decoding\n");
                    exit(1);
                }
                while(ret >= 0) {
                    ret = avcodec_receive_frame(pCodecContext_video, currentFrame);
                    if (ret == AVERROR(EAGAIN))// || ret == AVERROR_EOF
                        break;
                    static struct SwsContext *img_convert_ctx;
                    if (img_convert_ctx == NULL) {
                        int w = pCodecContext_video->width;
                        int h = pCodecContext_video->height;
                        img_convert_ctx = sws_getContext(w, h, pCodecContext_video->pix_fmt, w, h, AV_PIX_FMT_RGB24,
                                                         SWS_BICUBIC, NULL, NULL, NULL);
                        if (img_convert_ctx == NULL) {
                            fprintf(stderr, "Cannot initialize the conversion context!\n");
                            exit(1);
                        }
                    }
                    //convert YUV420p image to BRG24
                    sws_scale(img_convert_ctx, currentFrame->data, currentFrame->linesize, 0, pCodecContext_video->height,
                              currentFrameRGB->data, currentFrameRGB->linesize);
                    CopyDate(currentFrameRGB, pCodecContext_video->width, pCodecContext_video->height,
                             currentPacket.pts - prepts);
                    prepts = currentPacket.pts;
                    return currentMatPointer;
                }
                continue;
            }
            av_packet_unref(&currentPacket);
        }
        else{
            std::shared_ptr<cv::Mat> blackPhoto = std::make_shared<cv::Mat>(cv::Size(pCodecContext_video->width, pCodecContext_video->height), CV_8UC3, cv::Scalar(0));
            currentMatPointer = blackPhoto;
            return currentMatPointer;
        }
    }

}

void EditableVideo::CopyDate(AVFrame *pFrame, int width, int height, int64_t time) {
    if(time <=0 ) time = 1;

    int		nChannels;
    size_t		stepWidth;
    uchar  *	pData;
    std::shared_ptr<cv::Mat> frameImage = std::make_shared<cv::Mat>(cv::Size(width, height), CV_8UC3, cv::Scalar(0));
    stepWidth = frameImage->step;
    nChannels = frameImage->channels();
    pData = frameImage->data;

    for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++){
            pData[i * stepWidth + j * nChannels + 0] = pFrame->data[0][i * pFrame->linesize[0] + j * nChannels + 2];
            pData[i * stepWidth + j * nChannels + 1] = pFrame->data[0][i * pFrame->linesize[0] + j * nChannels + 1];
            pData[i * stepWidth + j * nChannels + 2] = pFrame->data[0][i * pFrame->linesize[0] + j * nChannels + 0];
        }
    }
    currentMatPointer = frameImage;

}
void EditableVideo::seekImage(int timeStamp) {
    int64_t seekTime_us = timeStamp * AV_TIME_BASE; //seekTime_us:us
    int64_t targetFrame = av_rescale_q(seekTime_us, AV_TIME_BASE_Q, pFormatContext->streams[video_stream_index]->time_base);
    if(av_seek_frame(this->pFormatContext, this->video_stream_index, targetFrame, AVSEEK_FLAG_BACKWARD) < 0)
        printf("av_seek_frame failed.");
}
