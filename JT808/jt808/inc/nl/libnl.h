#ifndef __LIBNL__
#define __LIBNL__

#ifdef __cplusplus
extern "C" {
#endif

#define NL_SPREADWIN	25

#define NL_MAX_PAYLOAD 1024 
#define NL_MAX_CMD_LEN 128
#define NL_MAX_EXTRA_LEN 512
/*
#define NLG_NONE	0
#define NLG_ALL		1
#define NLG_MAIN	2
#define NLG_FG		4
#define NLG_BG		8
#define NLG_VOICE	16
#define NLG_VIDEO   32
#define NLG_TEST	256

#define NLU_KERNEL	0
#define NLU_MAINAPP	1
#define NLU_UIAPP	2
#define NLU_CAMERA	3
#define NLU_WEIXIN	4
#define NLU_VOICE	5
#define NLU_TPMS	6
#define NLU_EDOG	7
#define NLU_OTA		8
#define NLU_CMCAR	9
#define NLU_SCCAR	10
#define NLU_PHONE	11
#define NLU_ETC 	12
#define NLU_MISC 	13
#define NLU_BANMA 	14
#define NLU_YISHITENG 	15
#define NLU_EDOG_SOCOL_DAEMON 	16
#define NLU_EDOG_SOCOL_APP 	17
#define NLU_RECORD 	18
#define NLU_BT  19
#define NLU_FMT  20
#define NLU_LED 21
#define NLU_WANJI_DAEMON 	101
#define NLU_WANJI_APP       102
#define NLU_MAIGU_DAEMON    103
#define NLU_MAIGU_DAEMON2   104
#define NLU_YX_APP0         105
#define NLU_YX_APP1         106
#define NLU_YX_APP2         107
#define NLU_YX_APP3         108
#define NLU_YX_APP4         109
#define NLU_YX_APP5         110
#define NLU_YX_APP6         111
#define NLU_YX_APP7         112
#define NLU_SSBB_DAEMON     113
#define NLU_SSBB_APP        114
#define NLU_TEST	255
*/

enum dgid
{
	NLG_NONE	= 0,
	NLG_ALL		= 1,
	NLG_MAIN	= 2,
	NLG_FG		= 4,
	NLG_BG		= 8,
	NLG_VOICE	= 16,
	NLG_VIDEO   = 32,
	NLG_TEST	= 256,
};

enum duid
{
	NLU_KERNEL	= 0,
	NLU_MAINAPP	= 1,
	NLU_UIAPP	= 2,
	NLU_CAMERA	= 3,
	NLU_WEIXIN	= 4,
	NLU_VOICE	= 5,
	NLU_TPMS	= 6,
	NLU_EDOG	= 7,
	NLU_OTA		= 8,
	NLU_CMCAR	= 9,
	NLU_SCCAR	= 10,
	NLU_PHONE	= 11,
	NLU_ETC 	= 12,
	NLU_MISC 	= 13,
	NLU_BANMA 	= 14,
	NLU_YISHITENG	= 15,
	NLU_EDOG_SOCOL_DAEMON	= 16,
	NLU_EDOG_SOCOL_APP	= 17,
	NLU_RECORD 	= 18,
	NLU_BT	= 19,
	NLU_FMT	= 20,
	NLU_LED = 21,
	NLU_WANJI_DAEMON 	= 101,
	NLU_WANJI_APP       = 102,
	NLU_MAIGU_DAEMON    = 103,
	NLU_MAIGU_DAEMON2   = 104,
	NLU_YX_APP0         = 105,
	NLU_YX_APP1         = 106,
	NLU_YX_APP2         = 107,
	NLU_YX_APP3         = 108,
	NLU_YX_APP4         = 109,
	NLU_YX_APP5         = 110,
	NLU_YX_APP6         = 111,
	NLU_YX_APP7         = 112,
	NLU_SSBB_DAEMON     = 113,
	NLU_SSBB_APP        = 114,
	NLU_TEST	= 255,
};

typedef struct _NLMSG {
	char *cmd;
	char *extra;
	int cmd_len;
	int extra_len;
	int suid;
	int sgid;
	int duid;
	int dgid;
} NLMSG;

int initNL(int uid, int gid);
void deinitNL();
int sendNLMsg(int duid, int dgid, char *cmd, char *extra);
int recvNLMsg(NLMSG * nlmsg); 

#ifdef __cplusplus
}
#endif

#endif
