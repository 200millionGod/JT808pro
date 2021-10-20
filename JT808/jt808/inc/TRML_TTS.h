/*************************************************************
 * Copyright			: 
 * FileName				: util.h
 * Description			: util.h implement
 * Author		        : chas
 * Created on			: Jan 9, 2019
 *************************************************************/

#ifndef INC_TRML_TTS_H_
#define INC_TRML_TTS_H_


int tts_switch_wav(TTSInfo *info, char *mp3_path, char *wav_path, char *text_info);
int load_tts_param(void);
void text_info_cb(void *Info);
int GBK2UTF8(char *gbk, char *utf8, int buf_len);

#endif

