#ifndef _PUBLIC_H_
#define _PUBLIC_H_

const int LOG_LEVLE_EMERG = 0;	/* system is unusable */
const int LOG_LEVLE_ALTER = 1;	/* action must be taken immediately */
const int LOG_LEVLE_CRIT = 2;	/* critical conditions */
const int LOG_LEVLE_ERR = 3;	/* error conditions */
const int LOG_LEVLE_WARNING = 4;/* warning conditions */
const int LOG_LEVLE_NOTICE = 5;	/* normal but significant condition */
const int LOG_LEVLE_INFO = 6;	/* informational */
const int LOG_LEVLE_DEBUG = 7;	/* debug-level messages */


unsigned int GetTickCount();
bool IsTimeOut(unsigned int dwTime, unsigned int dwTimeout);//单位都为毫秒




#endif


