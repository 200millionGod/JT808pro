#ifndef _OBSERVER_H_
#define _OBSERVER_H_

class CObserver
{
public:
	virtual void Update(void *pData) = 0;
	virtual ~CObserver(){};
};

#endif

