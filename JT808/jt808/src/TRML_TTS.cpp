#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string.h>
#include <sys/stat.h>

#include "../inc/tts/json.h"
//#include "speech.h"
#include "../inc/minimp3.h"
#include "../inc/TRML_TTS.h"
#include "../inc/TRML_parse.h"
#include "../inc/debug_log.h"

using namespace std;
//using namespace aip;
using namespace Json;

#define TTS_PARAMS_PATH "/usr/local/etc/tts.xml"

#define MINIMP3_IMPLEMENTATION
#define MINIMP3_MAX_SAMPLES_PER_FRAME (1152*2)

//写wav文件
void wavWrite_int16(char* filename, sint16* buffer, int sampleRate, uint32_t totalSampleCount, int channels = 1) {

    FILE* fp = fopen(filename, "wb");
    if (fp == NULL) {
        DBG_LEVEL_6("文件打开失败.");
        return;
    }
    //修正写入的buffer长度
    totalSampleCount *= sizeof(sint16)*channels;
    int nbit = 16;
    int FORMAT_PCM = 1;
    int nbyte = nbit / 8;
    char text[4] = { 'R', 'I', 'F', 'F' };
    uint32_t long_number = 36 + totalSampleCount;
    fwrite(text, 1, 4, fp);
    fwrite(&long_number, 4, 1, fp);
    text[0] = 'W';
    text[1] = 'A';
    text[2] = 'V';
    text[3] = 'E';
    fwrite(text, 1, 4, fp);
    text[0] = 'f';
    text[1] = 'm';
    text[2] = 't';
    text[3] = ' ';
    fwrite(text, 1, 4, fp);

    long_number = 16;
    fwrite(&long_number, 4, 1, fp);
    sint16 short_number = FORMAT_PCM;//默认音频格式
    fwrite(&short_number, 2, 1, fp);
    short_number = channels; // 音频通道数
    fwrite(&short_number, 2, 1, fp);
    long_number = sampleRate; // 采样率
    fwrite(&long_number, 4, 1, fp);
    long_number = sampleRate * nbyte; // 比特率
    fwrite(&long_number, 4, 1, fp);
    short_number = nbyte; // 块对齐
    fwrite(&short_number, 2, 1, fp);
    short_number = nbit; // 采样精度
    fwrite(&short_number, 2, 1, fp);
    char data[4] = { 'd', 'a', 't', 'a' };
    fwrite(data, 1, 4, fp);
    long_number = totalSampleCount;
    fwrite(&long_number, 4, 1, fp);
    fwrite(buffer, totalSampleCount, 1, fp);
    fclose(fp);
}

//读取文件buffer
char *getFileBuffer(const char *fname, int *size)
{
    FILE * fd = fopen(fname, "rb");
    if (fd == 0)
        return 0;
    struct stat st;
    char *file_buf = 0;
    if (fstat(fileno(fd), &st) < 0)
        goto doexit;
    file_buf = (char *)malloc(st.st_size + 1);
    if (file_buf != NULL)
    {
        if (fread(file_buf, st.st_size, 1, fd) < 1)
        {
            fclose(fd);
            return 0;
        }
        file_buf[st.st_size] = 0;
    }

    if (size)
        *size = st.st_size;
doexit:
    fclose(fd);
    return file_buf;
}

sint16* DecodeMp3ToBuffer(char* filename, uint32_t *sampleRate, uint32_t *totalSampleCount, unsigned int *channels)
{
    int music_size = 0;
    int alloc_samples = 1024 * 1024, num_samples = 0;
    sint16 *music_buf = (sint16 *)malloc(alloc_samples * 2 * 2);
    unsigned char *file_buf = (unsigned char *)getFileBuffer(filename, &music_size);
    if (file_buf != NULL)
    {
        unsigned char *buf = file_buf;
        mp3dec_frame_info_t info;
        mp3dec_t dec;

        mp3dec_init(&dec);
        for (;;)
        {
            sint16 frame_buf[2 * 1152];
            int samples = mp3dec_decode_frame(&dec, buf, music_size, frame_buf, &info);
            if (alloc_samples < (num_samples + samples))
            {
                alloc_samples *= 2;
                sint16* tmp = (sint16 *)realloc(music_buf, alloc_samples * 2 * info.channels);
                if (tmp)
                    music_buf = tmp;
            }
            if (music_buf)
                memcpy(music_buf + num_samples*info.channels, frame_buf, samples*info.channels * 2);
            num_samples += samples;
            if (info.frame_bytes <= 0 || music_size <= (info.frame_bytes + 4))
                break;
            buf += info.frame_bytes;
            music_size -= info.frame_bytes;
        }
        if (alloc_samples > num_samples)
        {
            sint16* tmp = (sint16 *)realloc(music_buf, num_samples * 2 * info.channels);
            if (tmp)
                music_buf = tmp;
        }

        if (sampleRate)
            *sampleRate = info.hz;
        if (channels)
            *channels = info.channels;
        if (num_samples)
            *totalSampleCount = num_samples;

        free(file_buf);
        return music_buf;
    }
    if (music_buf)
        free(music_buf);
    return 0;
}

void splitpath(const char* path, char* drv, char* dir, char* name, char* ext)
{
    const char* end;
    const char* p;
    const char* s;
    if (path[0] && path[1] == ':') {
        if (drv) {
            *drv++ = *path++;
            *drv++ = *path++;
            *drv = '\0';
        }
    }
    else if (drv)
        *drv = '\0';
    for (end = path; *end && *end != ':';)
        end++;
    for (p = end; p > path && *--p != '\\' && *p != '/';)
        if (*p == '.') {
            end = p;
            break;
        }
    if (ext)
        for (s = end; (*ext = *s++);)
            ext++;
    for (p = end; p > path;)
        if (*--p == '\\' || *p == '/') {
            p++;
            break;
        }
    if (name) {
        for (s = p; s < end;)
            *name++ = *s++;
        *name = '\0';
    }
    if (dir) {
        for (s = path; s < p;)
            *dir++ = *s++;
        *dir = '\0';
    }
}

int tts_switch_wav(TTSInfo *info, char *mp3_path, char *wav_path, char *text_info)
{
	int ret = 0;
	
	string app_id(info->app_id);
    string api_key(info->api_key);
    string secret_key(info->secrect_key);
	string spd(info->spd);
	string pit(info->pit);
	string vol(info->vol);
	string per(info->per);

	DBG_LEVEL_6("[app_id:%s]", app_id.c_str());
	DBG_LEVEL_6("[api_key:%s]", api_key.c_str());
	DBG_LEVEL_6("[secret_key:%s]", secret_key.c_str());
	DBG_LEVEL_6("[spd:%s]", spd.c_str());
	DBG_LEVEL_6("[pit:%s]", pit.c_str());
	DBG_LEVEL_6("[vol:%s]", vol.c_str());
	DBG_LEVEL_6("[per:%s]", per.c_str());

//    aip::Speech client(app_id, api_key, secret_key);

	ofstream ofile;
	string file_ret;
    map<std::string, std::string> options;

	options["spd"] = spd;
	options["pit"] = pit;
	options["vol"] = vol;
	options["per"] = per;

	
    // 合成成功的二进制数据写入文件中
    ofile.open(mp3_path, std::ios::out | std::ios::binary);

    // 不带可选参数调用 UTF-8
//    Json::Value result = client.text2audio("径卫视觉深圳研发中心", options, file_ret);
//    Json::Value result = client.text2audio(text_info, options, file_ret);

    // Json::Value result = client.text2audio("语音转换测试123", options, file_ret);
    if (!file_ret.empty())
    {
        ofile << file_ret;
		DBG_LEVEL_6("Success!");
		uint32_t totalSampleCount = 0;
		uint32_t sampleRate = 0;
		unsigned int channels = 0;
		sint16* wavBuffer = NULL;
        wavBuffer = DecodeMp3ToBuffer(mp3_path, &sampleRate, &totalSampleCount, &channels);
		if(wavBuffer){
			wavWrite_int16(wav_path, wavBuffer, sampleRate, totalSampleCount, channels);
			free(wavBuffer);
			wavBuffer = NULL;

//			gdbus_play_audio(wav_path);
		}
    } else {
        // 服务端合成错误
		DBG_LEVEL_6("Fail!");
//        cout << result.toStyledString();
		ret = -1;
    }
	
	return ret;
}


int load_tts_param(void)
{
	XMLParse xml_parse;

	if(!access(TTS_PARAMS_PATH, F_OK))
	{
		DBG_LEVEL_6("tts.xml exist");
		if(!xml_parse.Load_tts_param(TTS_PARAMS_PATH, &g_mgr.tts_info))
		{
			DBG_LEVEL_6("get XML file info error!");
			return -1;
		}
	}
	else
	{
		DBG_LEVEL_6("configure file not exist,file Path[%s]", TTS_PARAMS_PATH);
		strncpy(g_mgr.tts_info.app_id, "16746859", strlen("16746859"));
		strncpy(g_mgr.tts_info.api_key, "LXySS64YmDagGCyKzcFyoqta", strlen("LXySS64YmDagGCyKzcFyoqta"));
		strncpy(g_mgr.tts_info.secrect_key, "rqxsE9h1HD506iGGpY9dUk7WSS4qVRCr", strlen("rqxsE9h1HD506iGGpY9dUk7WSS4qVRCr"));
		strncpy(g_mgr.tts_info.spd, "5", strlen("5"));
		strncpy(g_mgr.tts_info.pit, "14", strlen("14"));
		strncpy(g_mgr.tts_info.vol, "15", strlen("15"));
		strncpy(g_mgr.tts_info.per, "0", strlen("0"));
	}

	return 0;
}

int GBK2UTF8(char *gbk, char *utf8, int buf_len)
{
	
	DBG_LEVEL_6(" ");
	size_t srclen = strlen(gbk), outlen = buf_len;
	char outbuf[outlen] ;
	memset(outbuf, 0, outlen);
	
	iconv_t cd = iconv_open("UTF-8", "GBK");
	if (cd == (iconv_t)-1)
	{
		perror ("iconv_open");
	}

	char *srcstart = (char *)gbk;
	char *tempoutbuf = (char *)outbuf;

	size_t ret = iconv (cd, &srcstart, &srclen, &tempoutbuf, &outlen);
	if ((int)ret == -1)
	{
	  DBG_LEVEL_6("iconv faile:%d", (int)ret);
	}

	strncpy(utf8, outbuf, sizeof(outbuf));

	DBG_LEVEL_6("utf8:%s", outbuf);

	return 0;

}

void text_info_cb(void *Info)
{
	int ret;
	DBG_LEVEL_6(" ");
	TextInfo *tts_info = (TextInfo *)Info;
	char text[1024] = {0};
	char mp3_path[32] = "/tmp/tts.mp3";
	char wav_path[32] = "/tmp/tts.wav";
	
	if (tts_info->flag & 0x2){//show on the cvbs

	}

	if (tts_info->flag & 0x8){//TTS Broadcast Reading
	
		DBG_LEVEL_6(" ");
		GBK2UTF8(tts_info->text, text, 1024);
	
		DBG_LEVEL_6(" ");
		ret = tts_switch_wav(&g_mgr.tts_info, mp3_path,wav_path, text);
		if (ret ){
			ret = tts_switch_wav(&g_mgr.tts_info, mp3_path,wav_path, text);
			if (ret){
				ret = tts_switch_wav(&g_mgr.tts_info, mp3_path,wav_path, text);
			}
		}
	}
    
}


