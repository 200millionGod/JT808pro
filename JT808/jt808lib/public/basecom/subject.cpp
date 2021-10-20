#include "subject.h"
#include "MyList.h"

CSubject::CSubject()
{
	m_lpParam = new CMyList<CObserver*>();
}

CSubject::~CSubject()
{
	delete ((CMyList<CObserver*>*)m_lpParam);
}

void CSubject::Attach(CObserver* observer)
{
	((CMyList<CObserver*>*)m_lpParam)->AddTail(observer);
}

void CSubject::Detach(CObserver* observer)
{
	CObserver *pObserver;
	POS pos = ((CMyList<CObserver*>*)m_lpParam)->GetHeadPosition();
	while(pos != NULL)
	{
		pObserver = ((CMyList<CObserver*>*)m_lpParam)->GetAt(pos);
		if(pObserver == observer)
		{
			((CMyList<CObserver*>*)m_lpParam)->RemoveAt(pos);
			break;
		}
		((CMyList<CObserver*>*)m_lpParam)->GetNext(pos);
	}
}

void CSubject::Notify(void* pData)
{
	CObserver *pObserver;
	POS pos = ((CMyList<CObserver*>*)m_lpParam)->GetHeadPosition();
	while(pos != NULL)
	{
		pObserver = ((CMyList<CObserver*>*)m_lpParam)->GetNext(pos);
		if(pObserver != NULL)
			pObserver->Update(pData);
	}
}

