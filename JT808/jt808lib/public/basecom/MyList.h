#ifndef _MY_LIST_H_
#define _MY_LIST_H_
typedef void* POS;

template <class T>
class CMyList  
{
typedef struct _node
{
	T  node;
	_node *pNext;
	_node *pPrev;
}
NODE;
public:
	POS			AddTail(const T &node);
	POS			AddHead(const T &node);
	T			RemoveTail();
	T			RemoveHead();
	T			GetTail() const;
	T&			GetTail();
	T			GetHead() const;
	T&			GetHead();
	T			GetNext(POS &pos) const;
	T&			GetNext(POS &pos);
	T			GetPrev(POS &pos) const;
	T&			GetPrev(POS &pos);
	POS			GetHeadPosition() const;
	POS			GetTailPosition() const;
	T			GetAt(POS pos) const;
	T&			GetAt(POS pos);
	POS			InsertBefore(POS pos, const T &node);
	POS			InsertAfter(POS pos,const T &node);
	void			SetAt(POS pos,T newnode);
	POS			Find(T node,POS startAfter);
	void			RemoveAll();
	int			GetCount() const;
	bool			IsEmpty() const;
	void			RemoveAt(POS pos);
	CMyList();
	virtual ~CMyList();
private:
	NODE *m_pHead;
	NODE *m_pTail;
	int	m_Count;
};

#ifndef NULL
#define NULL 0
#endif
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
template <class T>
CMyList<T>::CMyList()
{
	m_pHead=NULL;
	m_pTail=NULL;
	m_Count=0;
}

template <class T>
CMyList<T>::~CMyList()
{
	RemoveAll();
}

template <class T>
int CMyList<T>::GetCount() const
{
	return m_Count;
}

template <class T>
bool CMyList<T>::IsEmpty() const
{
	return (m_Count == 0);
}

template <class T>
T CMyList<T>::GetHead() const
{
	return m_pHead->node;	
}

template <class T>
T& CMyList<T>::GetHead()
{
	return m_pHead->node;	
}

template <class T>
T CMyList<T>::GetTail() const
{
	return m_pTail->node;
}

template <class T>
T& CMyList<T>::GetTail()
{
	return m_pTail->node;
}

template <class T>
POS CMyList<T>::GetHeadPosition() const
{
	return (POS) m_pHead;
}

template <class T>
POS CMyList<T>::GetTailPosition() const
{
	return (POS) m_pTail;
}

template <class T>
T CMyList<T>::GetNext(POS &pos) const
{
	NODE* pNode = (NODE*) pos;
	pos = (POS) pNode->pNext;
	return pNode->node; 
}

template <class T>
T& CMyList<T>::GetNext(POS &pos)
{
	NODE* pNode = (NODE*) pos;
	pos = (POS) pNode->pNext;
	return pNode->node; 
}

template <class T>
T CMyList<T>::GetPrev(POS &pos) const
{
	NODE* pNode = (NODE*) pos;
	pos = (POS) pNode->pPrev;
	return pNode->node; 
}

template <class T>
T& CMyList<T>::GetPrev(POS &pos)
{
	NODE* pNode = (NODE*) pos;
	pos = (POS) pNode->pPrev;
	return pNode->node; 
}

template <class T>
T CMyList<T>::GetAt(POS pos) const
{
	NODE* pNode = (NODE*) pos;
	return pNode->node; 
}

template <class T>
T& CMyList<T>::GetAt(POS pos)
{
	NODE* pNode = (NODE*) pos;
	return pNode->node; 
}

template <class T>
void CMyList<T>::SetAt(POS pos,T newnode)
{
	NODE* pNode = (NODE*) pos;
	pNode->node=newnode;
}

template <class T>
void CMyList<T>::RemoveAll()
{
	NODE* pNode= m_pHead;
	while(pNode!=NULL)
	{
		NODE* temp=pNode;
		pNode=pNode->pNext;
		delete temp;
		temp = NULL;
	}

	m_pHead=NULL;
	m_pTail=NULL;
	m_Count=0;
}

template <class T>
POS CMyList<T>::AddHead(const T &node)
{
	NODE *pNewNode=new NODE;
	pNewNode->pPrev=NULL;
	pNewNode->pNext=m_pHead;
	pNewNode->node = node;
	if (m_pHead != NULL)
		m_pHead->pPrev = pNewNode;
	else
		m_pTail = pNewNode;
	m_pHead = pNewNode;
	m_Count++;
	return (POS) pNewNode;

}

template <class T>
POS CMyList<T>::AddTail(const T &node)
{
	NODE *pNewNode=new NODE;
	pNewNode->pPrev=m_pTail;
	pNewNode->pNext=NULL;
	pNewNode->node = node;
	if (m_pTail != NULL)
		m_pTail->pNext = pNewNode;
	else
		m_pHead = pNewNode;
	m_pTail = pNewNode;
	m_Count++;
	return (POS) pNewNode;
}

template <class T>
T CMyList<T>::RemoveTail()
{
	NODE* pOldNode = m_pTail;
	T returnValue = pOldNode->node;

	m_pTail = pOldNode->pPrev;
	if (m_pTail != NULL)
		m_pTail->pNext = NULL;
	else
		m_pHead = NULL;

	delete pOldNode;
	pOldNode = NULL;

	m_Count -- ;
	return returnValue;}

template <class T>
T CMyList<T>::RemoveHead()
{
	NODE* pOldNode = m_pHead;
	T returnValue = pOldNode->node;

	m_pHead = pOldNode->pNext;
	if (m_pHead != NULL)
		m_pHead->pPrev = NULL;
	else
		m_pTail = NULL;
	delete pOldNode;
	pOldNode = NULL;

	m_Count -- ;
	return returnValue;
}

template <class T>
void CMyList<T>::RemoveAt(POS pos)
{
	NODE* pOldNode = (NODE*) pos;

	// remove pOldNode from list
	if (pOldNode == m_pHead)
	{
		m_pHead = pOldNode->pNext;
	}
	else
	{
		pOldNode->pPrev->pNext = pOldNode->pNext;
	}
	if (pOldNode == m_pTail)
	{
		m_pTail = pOldNode->pPrev;
	}
	else
	{
		pOldNode->pNext->pPrev = pOldNode->pPrev;
	}
	delete pOldNode;
	pOldNode = NULL;

	m_Count--;
}

template <class T>
POS CMyList<T>::InsertBefore(POS pos, const T &node)
{
	if (pos == NULL)
		return AddHead(node); // insert before nothing -> head of the list

	// Insert it before position
	NODE* pOldNode = (NODE*) pos;
	NODE* pNewNode = new NODE;
	pNewNode->pPrev=pOldNode->pPrev;
	pNewNode->pNext=pOldNode;
	pNewNode->node = node;

	if (pOldNode->pPrev != NULL)
	{
		pOldNode->pPrev->pNext = pNewNode;
	}
	else
	{
		m_pHead = pNewNode;
	}
	pOldNode->pPrev = pNewNode;
	m_Count++;
	return (POS) pNewNode;
}

template <class T>
POS CMyList<T>::InsertAfter(POS pos, const T &node)
{
	if (pos == NULL)
		return AddTail(node); // insert after nothing -> tail of the list

	// Insert it before position
	NODE* pOldNode = (NODE*) pos;
	NODE* pNewNode =new NODE;
	pNewNode->pPrev=pOldNode;
	pNewNode->pNext=pOldNode->pNext;
	pNewNode->node = node;

	if (pOldNode->pNext != NULL)
	{
		pOldNode->pNext->pPrev = pNewNode;
	}
	else
	{
		m_pTail = pNewNode;
	}
	pOldNode->pNext = pNewNode;
	m_Count++;
	return (POS) pNewNode;
}

template <class T>
POS CMyList<T>::Find(T node,POS startAfter)
{
	NODE* pNode = (NODE*) startAfter;
	if (pNode == NULL)
	{
		pNode = m_pHead;  // start at head
	}
	else
	{
		pNode = pNode->pNext;  // start after the one specified
	}

	for (; pNode != NULL; pNode = pNode->pNext)
	{
		if (pNode->node==node)
			return (POS)pNode;
	}
	return NULL;
}

#endif//_MY_LIST_H_
