#ifndef LINKS_H_
#define LINKS_H_

#include "Base.h"
#include "Ref.h"


class CLinkObject : public gameplay::Ref
{
public:
	// Служебная переменная
	void * m_pContainer;
	// Служебная переменная
	void * m_pLastAccessContainer;
	// Проверить присоединен ли объект к списку
	virtual bool											__stdcall Linked();
	// Удалить объект из всех списков
	virtual int										__stdcall UnLink();
	// Создать новый объект
	static	CLinkObject*							Create();
	
	virtual ~CLinkObject();
protected:
	CLinkObject();	
};

template <class T>
class CObjectContainer
{
public:
	// Указатель на объект
	virtual CLinkObject*		__stdcall object(){ return NULL; };
	virtual CObjectContainer*	__stdcall next(){ return NULL; };
	virtual CObjectContainer*	__stdcall prev(){ return NULL; };
	operator T*()
	{
		return (T*)object();
	}
	T* operator->()
	{
		return (T*)object();
	}
};

template <class T>
class Itr
{
public:
	operator T*()
	{
		return m_pContainer ? (T*)m_pContainer->object() : NULL;
	}
	T* operator->()
	{
		return m_pContainer ? (T*)m_pContainer->object() : NULL;
	}
	Itr& operator++()
	{
		if (m_pContainer)
			m_pContainer = m_pContainer->next();
		return *this;
	}
	Itr operator++(int)
	{
		Itr tmp = *this;
		if (m_pContainer)
			m_pContainer = m_pContainer->next();
		return tmp;
	}
	Itr& operator--()
	{
		if (m_pContainer)
			m_pContainer = m_pContainer->prev();
		return *this;
	}
	Itr operator--(int)
	{
		Itr tmp = *this;
		if (m_pContainer)
			m_pContainer = m_pContainer->prev();
		return tmp;
	}
	bool operator== (Itr<T>& itr)
	{
		return m_pContainer == itr.m_pContainer;
	}
	bool operator!= (Itr<T>& itr)
	{
		return m_pContainer != itr.m_pContainer;
	}
	Itr& operator= (Itr<T>& other)
	{
		m_pContainer = other.m_pContainer;
		return *this;
	}
	Itr& operator= (int null)
	{
		m_pContainer = NULL;
		return *this;
	}
	Itr(CObjectContainer<T>& pContainer)
		: m_pContainer(&pContainer)
	{}
	Itr(const int pContainer)
		: m_pContainer(NULL)
	{}
protected:
	CObjectContainer<T>* m_pContainer;
};




// to do need boolean param in function Add to determinate adding to last object
//to do change adding order by default
// to do need copy function or operator=
class CLinkList : public gameplay::Ref
{
public:
	// Добавить объект к списку
	virtual int												Add(CLinkObject * pObject) = 0;
	// Вставить объект в список
	virtual int												Ins(CLinkObject * pPrevObject, CLinkObject * pObject) = 0;
	// Удалить объект из списка
	virtual int												Remove(CLinkObject * pObject) = 0;
	// Удалить контейнер для объекта из списка
	virtual CLinkObject *										RemoveContainer(CObjectContainer<CLinkObject>& pContainer) = 0;
	// Обменять элементы местами( производит обмен объектов во всех их контейнерах )
	virtual void												Swap(CLinkObject* pObject1, CLinkObject* pObject2) = 0;
	// Поменять порядок элементов( производит смену порядка контейнеров только в текущем листе )
	virtual void												SwapOrder(CLinkObject* pObject1, CLinkObject* pObject2) = 0;
	// Получить первый объект в списке
	virtual CObjectContainer<CLinkObject>&	GetFirst() = 0;
	// Получить последний объект в списке
	virtual CObjectContainer<CLinkObject>&	GetLast() = 0;
	//
	virtual int													GetLinkObjectsCount() = 0;
	// Отчистка списка контейнеров
	virtual void												Clear() = 0;
	// Создать новый список объектов
	static	CLinkList*											Create();
	// Удалить список объектов
	virtual ~CLinkList(){};
};

template <class T>
class CTemplateList
{
public:
	// Добавить объект к списку
	int								Add(T* pObject){ return m_pLinkList->Add((CLinkObject*)pObject); };
	// Вставить объект в список
	int								Ins(T* pPrevObject, T* pObject) { return m_pLinkList->Ins((CLinkObject*)pPrevObject, (CLinkObject*)pObject); };
	// Удалить объект из списка
	int								Remove(T* pObject){ return m_pLinkList->Remove((CLinkObject*)pObject); };
	// Удалить контейнер для объекта из списка
	T*									RemoveContainer(CObjectContainer<T>& Container){ return (T*)m_pLinkList->RemoveContainer(*(CObjectContainer<CLinkObject>*)&Container); };
	// Обменять элементы местами( производит обмен объектов во всех их контейнерах )
	void								Swap(T* pObject1, T* pObject2) { m_pLinkList->Swap((CLinkObject*)pObject1, (CLinkObject*)pObject2); };;
	// Поменять порядок элементов( производит смену порядка контейнеров только в текущем листе )
	void								SwapOrder(CLinkObject* pObject1, CLinkObject* pObject2){ m_pLinkList->SwapOrder((CLinkObject*)pObject1, (CLinkObject*)pObject2); };
	// Получить первый объект в списке
	Itr<T>	GetFirst(){ return *(CObjectContainer<T>*)&m_pLinkList->GetFirst(); };
	// Получить последний объект в списке
	Itr<T>	GetLast(){ return *(CObjectContainer<T>*)&m_pLinkList->GetLast(); };
	//
	int									GetLinkObjectsCount(){ return m_pLinkList->GetLinkObjectsCount(); };
	// Отчистка списка контейнеров
	void								Clear(){ m_pLinkList->Clear(); };

	CTemplateList() : m_pLinkList(CLinkList::Create()){};
	~CTemplateList(){ SAFE_DELETE(m_pLinkList); };
protected:
	CLinkList*							m_pLinkList;
};

template <class T>
class COwnerList : public CTemplateList<T>
{
public:
	virtual void Clear()
	{
		T* pObject;
		while (pObject = GetFirst())
			SAFE_DELETE(pObject);
		m_pLinkList->Clear();
	};
	virtual ~COwnerList(){ Clear(); };
};

template <class T>
class CLink
{
public:
	operator T*() { return m_Object.GetFirst(); };
	T* operator->() { return m_Object.GetFirst(); };
	CLink& operator= (T* other) { if (m_Object.GetFirst() != other) { m_Object.Clear(); m_Object.Add(other); } return *this; };
	CLink& operator= (CLink& other) { if (*this != other) { m_Object.Clear(); m_Object.Add(other.m_Object.GetFirst()); } return *this; };
	bool operator== (T* other) { return m_Object.GetFirst() == other; }
	CLink(CLink& src) { m_Object.Clear(); m_Object.Add(src.m_Object.GetFirst()); };
	CLink(T* other) { m_Object.Clear(); m_Object.Add(other); };
	CLink() : m_Object() {};
	~CLink() {};
protected:
	CTemplateList<T> m_Object;
};

template <class T>
class AutoRef
{
public:
	operator T*(){ return _object; };
	T* operator->(){ return _object; };
	T* operator= (T* other){ if (_object != other) { SAFE_RELEASE(_object); _object = other; other->addRef(); } return _object; };
	AutoRef(T* other)	{ _object = other; other->addRef(); };
	AutoRef() : _object(NULL){};
	~AutoRef(){ SAFE_RELEASE(_object); };
	void newRef(T* other){ if (_object != other) { SAFE_RELEASE(_object); _object = other; }};
private:
	T* _object;
};

class MethodSubscribingBase : public Ref
{
	friend class Parameter;
public:
	virtual void update(){}
protected:
	MethodSubscribingBase(){};
	virtual ~MethodSubscribingBase(){}
};

template <class ClassType, class ParameterType>
class SimpleMethodSubscribing : public MethodSubscribingBase
{
	typedef ParameterType(ClassType::*ValueMethod)();
public:
	SimpleMethodSubscribing(ClassType* instance, ValueMethod valueMethod)
		: _instance(instance)
		, _valueMethod(valueMethod){}
	virtual void update(){ (_instance->*_valueMethod)(); }
private:
	ClassType* _instance;
	ValueMethod _valueMethod;
};

template <class T>
class Subscribable : public CLink<T>
{
public:
	Subscribable()
		: _method(NULL)
	{}

	template <class ClassType, class ParameterType>
	void subscribe(ClassType* classInstance, ParameterType(ClassType::*valueMethod)()){ _method = new SimpleMethodSubscribing<ClassType, ParameterType>(classInstance, valueMethod); }
	Subscribable& operator= (T* other) { if (m_Object.GetFirst() != other) { m_Object.Clear(); m_Object.Add(other); if (_method) _method->update(); } return *this; };
	Subscribable& operator= (Subscribable& other) { if (*this != other) { m_Object.Clear(); m_Object.Add(other.m_Object.GetFirst()); if (_method) _method->update(); } return *this; };
	Subscribable(Subscribable& src)
		: CLink(src)
		, _method(NULL)
	{};
	Subscribable(T* other)
		: CLink(other)
		, _method(NULL)
	{};

private:
	MethodSubscribingBase* _method;
};

class CLinkListImpl;

class CTerraObjectContainer : public CObjectContainer<CLinkObject>
{
public:
	virtual CLinkObject*			__stdcall object();
	virtual CObjectContainer*	__stdcall next();
	virtual CObjectContainer*	__stdcall prev();

	CLinkObject*							m_pObject;
	CLinkListImpl&						m_pLinkList;
	CTerraObjectContainer*		m_pPrevContainer;
	CTerraObjectContainer*		m_pNextContainer;
	CTerraObjectContainer*		m_pNextContainerInObject;
	CTerraObjectContainer*		m_pPrevContainerInObject;

	CTerraObjectContainer(CLinkListImpl& LinkList);
	//CTerraObjectContainer();
};

class CLinkListImpl : public CLinkList
{
	friend class CTerraObjectContainer;
public:
	virtual int													Add(CLinkObject * pObject);
	virtual int													Ins(CLinkObject * pPrevObject, CLinkObject * pObject);
	virtual int													Remove(CLinkObject * pObject);
	virtual CLinkObject*										RemoveContainer(CObjectContainer<CLinkObject>& pContainer);
	virtual void														Swap(CLinkObject * pObject1, CLinkObject * pObject2);
	virtual void														SwapOrder(CLinkObject* pObject1, CLinkObject* pObject2);
	virtual CObjectContainer<CLinkObject>&	GetFirst();
	virtual CObjectContainer<CLinkObject>&	GetLast();
	virtual int															GetLinkObjectsCount();
	virtual void														Clear();
	CLinkListImpl();
	virtual ~CLinkListImpl();
private:
	CTerraObjectContainer									m_Empty;
	int																		m_nCount;
	CTerraObjectContainer*	 							m_pContainer;
	CTerraObjectContainer*	 							m_pFirstContainer;
	CTerraObjectContainer*	 							m_pContainerFree;
	CTerraObjectContainer*	 							GetContanerFree();
	void																	RemoveContanerToFree(CTerraObjectContainer * pContainer);
	CTerraObjectContainer&								GetContainer(CLinkObject* pObject);
};

#endif /* LINKS_H_ */