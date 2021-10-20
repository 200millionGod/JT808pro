#ifndef _SUBJECT_H_
#define _SUBJECT_H_

#include "observer.h"

class CSubject
{
public:
	CSubject();
	~CSubject();
	void Attach(CObserver* observer);
	void Detach(CObserver* observer);
protected:
	void Notify(void* pData);
private:
	void* m_lpParam;
};
#endif

