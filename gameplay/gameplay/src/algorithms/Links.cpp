#include "Links.h"
/*static CListIterator* Create( CLinkList* LinkList )
{
return (CListIterator*) new CTerraObjectContainer( (CLinkListImpl*)LinkList );
}*/

///////////////////////////////////////////////////////////////////////////////////
bool CLinkObject::Linked()
{
	return (m_pContainer) ? true : false;
}

int CLinkObject::UnLink()
{
	CTerraObjectContainer* pContainer = (CTerraObjectContainer*)m_pContainer;
	if (pContainer)
	{
		CLinkListImpl* pLinkList;
		CTerraObjectContainer* pNextContainer = pContainer->m_pNextContainerInObject;
		CTerraObjectContainer* pPrevContainer = pContainer->m_pPrevContainerInObject;

		pLinkList = &pContainer->m_pLinkList;
		if (pLinkList) pLinkList->RemoveContainer(*pContainer);

		while (pNextContainer)
		{
			pContainer = pNextContainer;
			pLinkList = &pContainer->m_pLinkList;
			pNextContainer = pNextContainer->m_pNextContainerInObject;
			if (pLinkList) pLinkList->RemoveContainer(*pContainer);
		}
		while (pPrevContainer)
		{
			pContainer = pPrevContainer;
			pLinkList = &pContainer->m_pLinkList;
			pPrevContainer = pPrevContainer->m_pPrevContainerInObject;
			if (pLinkList) pLinkList->RemoveContainer(*pContainer);
		}
	}
	return 0;
}

CLinkObject::CLinkObject()
{
	m_pContainer = NULL;
	m_pLastAccessContainer = NULL;
}

CLinkObject::~CLinkObject()
{
	UnLink();
}

CLinkObject* CLinkObject::Create()
{
	return new CLinkObject();
}
CLinkList* CLinkList::Create()
{
	return (CLinkList*)new CLinkListImpl();
}
////////////////////////////////////////////////////////////////////////////////////
CTerraObjectContainer * CLinkListImpl::GetContanerFree()
{
	if (m_pContainerFree)
	{
		CTerraObjectContainer * pContanerFree = m_pContainerFree;
		m_pContainerFree = m_pContainerFree->m_pPrevContainer;
		if (m_pContainerFree)
			m_pContainerFree->m_pNextContainer = NULL;
		return pContanerFree;
	}
	else
		return new CTerraObjectContainer(*this);
}
void CLinkListImpl::RemoveContanerToFree(CTerraObjectContainer * pContainer)
{
	pContainer->m_pNextContainer = NULL;
	pContainer->m_pPrevContainer = NULL;
	pContainer->m_pPrevContainerInObject = NULL;
	pContainer->m_pNextContainerInObject = NULL;
	if (m_pContainerFree)
	{
		pContainer->m_pPrevContainer = m_pContainerFree;
		m_pContainerFree->m_pNextContainer = pContainer;
		m_pContainerFree = pContainer;
	}
	else
	{
		m_pContainerFree = pContainer;
	}
}

CTerraObjectContainer& CLinkListImpl::GetContainer(CLinkObject * pObject)
{
	if (!pObject) return m_Empty;

	CTerraObjectContainer* pContainer = (CTerraObjectContainer*)pObject->m_pLastAccessContainer;
	if (pContainer && ((&pContainer->m_pLinkList) == this))
	{
		return *(CTerraObjectContainer*)pObject->m_pLastAccessContainer;
	}
	else
	{
		pContainer = (CTerraObjectContainer*)pObject->m_pContainer;
		if (&pContainer->m_pLinkList == this)
			return *pContainer;

		CTerraObjectContainer* pNextContainer = pContainer->m_pNextContainerInObject;
		CTerraObjectContainer* pPrevContainer = pContainer->m_pPrevContainerInObject;

		while (pNextContainer)
		{
			if (&pNextContainer->m_pLinkList == this)
				return *pNextContainer;
			pNextContainer = pNextContainer->m_pNextContainerInObject;
		}
		while (pPrevContainer)
		{
			if (&pPrevContainer->m_pLinkList == this)
				return *pPrevContainer;
			pPrevContainer = pPrevContainer->m_pPrevContainerInObject;
		}
		return m_Empty;
	}
}

int CLinkListImpl::Add(CLinkObject * pObject)
{
	if (!pObject) return 1;
	CTerraObjectContainer * pContainer = GetContanerFree();
	pContainer->m_pObject = pObject;
	pContainer->m_pObject->m_pLastAccessContainer = pContainer;
	//////////////////////////////////////////////////////////////////////////////////////
	if (m_pContainer)
	{
		pContainer->m_pPrevContainer = m_pContainer;
		pContainer->m_pNextContainer = NULL;
		m_pContainer->m_pNextContainer = pContainer;
		m_pContainer = pContainer;
	}
	else
	{
		m_pContainer = pContainer;
		m_pFirstContainer = pContainer;
		pContainer->m_pPrevContainer = NULL;
		pContainer->m_pNextContainer = NULL;
	}
	//////////////////////////////////////////////////////////////////////////////////////
	if (pObject->m_pContainer)
	{
		pContainer->m_pNextContainerInObject = (CTerraObjectContainer *)pObject->m_pContainer;
		pContainer->m_pPrevContainerInObject = NULL;
		((CTerraObjectContainer *)pObject->m_pContainer)->m_pPrevContainerInObject = pContainer;
		pObject->m_pContainer = pContainer;
	}
	else
	{
		pObject->m_pContainer = pContainer;
		pContainer->m_pNextContainerInObject = NULL;
		pContainer->m_pPrevContainerInObject = NULL;
	}
	m_nCount++;
	return 0;
}

int CLinkListImpl::Ins(CLinkObject * pPrevObject, CLinkObject * pObject)
{
	if (!pObject) return 1;
	if (!pPrevObject) return 1;

	CTerraObjectContainer * pContainer = GetContanerFree();
	pContainer->m_pObject = pObject;
	pContainer->m_pObject->m_pLastAccessContainer = pContainer;

	CTerraObjectContainer * pPrevContainer = &GetContainer(pPrevObject);
	CTerraObjectContainer * pNextContainer = pPrevContainer->m_pPrevContainer;

	//
	pPrevContainer->m_pPrevContainer = pContainer;
	pContainer->m_pNextContainer = pPrevContainer;
	pContainer->m_pPrevContainer = pNextContainer;
	if (pNextContainer) pNextContainer->m_pNextContainer = pContainer;

	if (pObject->m_pContainer)
	{
		pContainer->m_pNextContainerInObject = (CTerraObjectContainer *)pObject->m_pContainer;
		pContainer->m_pPrevContainerInObject = NULL;
		((CTerraObjectContainer *)pObject->m_pContainer)->m_pPrevContainerInObject = pContainer;
		pObject->m_pContainer = pContainer;
	}
	else
	{
		pObject->m_pContainer = pContainer;
		pContainer->m_pNextContainerInObject = NULL;
		pContainer->m_pPrevContainerInObject = NULL;
	}
	m_nCount++;

	return 0;
}

int CLinkListImpl::Remove(CLinkObject * pObject)
{
	RemoveContainer(GetContainer(pObject));
	return 0;
}

CLinkObject* CLinkListImpl::RemoveContainer(CObjectContainer<CLinkObject>& Container)
{
	CObjectContainer<CLinkObject>* pContainer = &Container;
	if (pContainer)
	{
		CLinkObject * pObject = ((CTerraObjectContainer*)pContainer)->m_pObject;

		if (m_pFirstContainer == pContainer)
		{
			m_pFirstContainer = ((CTerraObjectContainer*)pContainer)->m_pNextContainer;
		}

		if (m_pContainer == pContainer)
		{
			m_pContainer = ((CTerraObjectContainer*)pContainer)->m_pPrevContainer;
			if (m_pContainer)
				m_pContainer->m_pNextContainer = NULL;
		}
		else
		{
			if (((CTerraObjectContainer*)pContainer)->m_pNextContainer)
				((CTerraObjectContainer*)pContainer)->m_pNextContainer->m_pPrevContainer = ((CTerraObjectContainer*)pContainer)->m_pPrevContainer;
			if (((CTerraObjectContainer*)pContainer)->m_pPrevContainer)
				((CTerraObjectContainer*)pContainer)->m_pPrevContainer->m_pNextContainer = ((CTerraObjectContainer*)pContainer)->m_pNextContainer;
		}
		//////////////////////////////////////////////////////////////////////////////
		if (pObject && (pObject->m_pContainer == pContainer))
		{
			pObject->m_pContainer = ((CTerraObjectContainer*)pContainer)->m_pNextContainerInObject;
			if (pObject->m_pContainer)
				((CTerraObjectContainer*)pObject->m_pContainer)->m_pPrevContainerInObject = NULL;
		}
		else
		{
			if (((CTerraObjectContainer*)pContainer)->m_pPrevContainerInObject)
				((CTerraObjectContainer*)pContainer)->m_pPrevContainerInObject->m_pNextContainerInObject = ((CTerraObjectContainer*)pContainer)->m_pNextContainerInObject;
			if (((CTerraObjectContainer*)pContainer)->m_pNextContainerInObject)
				((CTerraObjectContainer*)pContainer)->m_pNextContainerInObject->m_pPrevContainerInObject = ((CTerraObjectContainer*)pContainer)->m_pPrevContainerInObject;
		}


		RemoveContanerToFree((CTerraObjectContainer*)pContainer);

		m_nCount--;

		return pObject;
	}
	else
	{
		return NULL;
	}
}

void CLinkListImpl::Swap(CLinkObject * pObject1, CLinkObject * pObject2)
{
	if (pObject1 == pObject2) return;

	void* pContainer = pObject1->m_pContainer;
	void* pLastAccessContainer = pObject1->m_pLastAccessContainer;
	pObject1->m_pContainer = pObject2->m_pContainer;
	pObject1->m_pLastAccessContainer = pObject2->m_pLastAccessContainer;
	pObject2->m_pContainer = pContainer;
	pObject2->m_pLastAccessContainer = pLastAccessContainer;

	CTerraObjectContainer* pContainer1 = (CTerraObjectContainer*)pObject1->m_pContainer;
	CTerraObjectContainer* pContainer2 = (CTerraObjectContainer*)pObject2->m_pContainer;
	while (pContainer1)
	{
		pContainer1->m_pObject = pObject1;
		pContainer1 = pContainer1->m_pNextContainerInObject;
	}
	while (pContainer2)
	{
		pContainer2->m_pObject = pObject2;
		pContainer2 = pContainer2->m_pNextContainerInObject;
	}
}

void CLinkListImpl::SwapOrder(CLinkObject* pObject1, CLinkObject* pObject2)
{
	if (pObject1 == pObject2) return;
	if (!pObject1->m_pLastAccessContainer) return;
	if (!pObject2->m_pLastAccessContainer) return;
	CTerraObjectContainer* pContainer1 = (CTerraObjectContainer*)pObject1->m_pLastAccessContainer;
	CTerraObjectContainer* pContainer2 = (CTerraObjectContainer*)pObject2->m_pLastAccessContainer;
	if (&pContainer1->m_pLinkList != &pContainer2->m_pLinkList) return;
}

CObjectContainer<CLinkObject>& CLinkListImpl::GetFirst()
{
	if (m_pFirstContainer)
		return *m_pFirstContainer;
	else
		return m_Empty;
}

CObjectContainer<CLinkObject>& CLinkListImpl::GetLast()
{
	if (m_pContainer)
		return *m_pContainer;
	else
		return m_Empty;
}

int CLinkListImpl::GetLinkObjectsCount()
{
	return m_nCount;
}

void CLinkListImpl::Clear()
{
	CTerraObjectContainer* pContainer = m_pContainer;
	CTerraObjectContainer* pNextContainer;
	while (pContainer)
	{
		pNextContainer = pContainer->m_pPrevContainer;
		RemoveContainer(*pContainer);
		pContainer = pNextContainer;
	}

	m_pContainer = NULL;
}

#pragma warning ( disable: 4355 )
CLinkListImpl::CLinkListImpl()
: m_Empty(*this)
, m_nCount(0)
, m_pContainer(NULL)
, m_pFirstContainer(NULL)
, m_pContainerFree(NULL)
{
}
#pragma warning ( default: 4355 )
CLinkListImpl::~CLinkListImpl()
{
	Clear();

	CTerraObjectContainer* pNextContainer;
	CTerraObjectContainer* pContainer = m_pContainerFree;
	while (pContainer)
	{
		pNextContainer = pContainer->m_pPrevContainer;
		SAFE_DELETE(pContainer);
		pContainer = pNextContainer;
	}
	m_pContainerFree = NULL;
}
//////////////////////////////////////////////////////////////////////////////////////////////////
CLinkObject* CTerraObjectContainer::object()
{
	return m_pObject;
}

CObjectContainer<CLinkObject>*	CTerraObjectContainer::next()
{
	if (m_pNextContainer)
		return m_pNextContainer;
	else
		return &m_pLinkList.m_Empty;
}
CObjectContainer<CLinkObject>*	CTerraObjectContainer::prev()
{
	if (m_pPrevContainer)
		return m_pPrevContainer;
	else
		return &m_pLinkList.m_Empty;
}

CTerraObjectContainer::CTerraObjectContainer(CLinkListImpl& pLinkList)
: m_pObject(NULL)
, m_pLinkList(pLinkList)
, m_pPrevContainer(NULL)
, m_pNextContainer(NULL)
, m_pNextContainerInObject(NULL)
, m_pPrevContainerInObject(NULL)
{
}