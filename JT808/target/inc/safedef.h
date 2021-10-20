#ifndef _SAFE_DEF_H_
#define _SAFE_DEF_H_

#define INVALID_SOCKET -1

#define SAFE_DELETE_OBJECT(p) if ((p) != NULL) {delete (p); (p) = NULL;}
#define SAFE_DELETE_ARRAY(p) if ((p) != NULL) {delete [] (p); (p) = NULL;}
#define SAFE_CLOSE_SOCKET(p) if ((p) != INVALID_SOCKET) {close((p)); (p) = INVALID_SOCKET;}

#ifdef SAFE_RELEASE
#undef SAFE_RELEASE
#endif
#define SAFE_RELEASE(x)		{ if (x) (x)->Release(); (x) = NULL; }

#endif

