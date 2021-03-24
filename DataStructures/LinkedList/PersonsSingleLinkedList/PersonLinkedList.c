#include <stdio.h>
#include <stdlib.h>
#include "PersonLinkedList.h"


Person* ListInsertHead(Person* _head, Person* _p)
{	
	if(!_p)
	{
		return _head;
	}
	
	_p->m_next = _head; /* Would work even if _head == NULL */
	
	return _p;
}


Person* ListRemoveHead(Person* _head, Person** _p)
{
	Person* tempNextPtr = NULL;
	
	if(!_p || !_head)
	{
		return _head;
	}
	
	tempNextPtr = _head->m_next;
	_head->m_next = NULL;
	*_p = _head;
		
	return tempNextPtr; /* The new head of the linked list */
}


Person* ListInsertByKey(Person* _head, int _key, Person* _p)
{
	Person* curr = _head;
	Person* prev = curr;
	
	if(!_p)
	{
		return _head;
	}
	
	if(!_head || _head->m_id > _key) /* _p should be the new head of linked list */
	{
		_p->m_next = _head; /* Would work even if _head == NULL */
		
		return _p;
	}
	
	while(curr)
	{
		if(curr->m_id == _key) /* _p should not be inserted */
		{
			break;
		}
		
		prev = curr;
		curr = curr->m_next;
		
		if(!curr || curr->m_id > _key)
		{
			prev->m_next = _p;
			_p->m_next = curr; /* Will be NULL if curr is NULL */
			break;
		}
	}
	
	return _head;
}


Person* ListInsertByKeyRec(Person* _head, int _key, Person* _p)
{
	if(!_p)
	{
		return _head;
	}
	
	if(!_head || _head->m_id > _key)
	{
		_p->m_next = _head; /* Will be NULL if _head is NULL */
		
		return _p;
	}
	
	if(_head->m_id == _key)
	{
		return _head; /* No change */
	}
	
	_head->m_next = ListInsertByKeyRec(_head->m_next, _key, _p);
	
	return _head;
}


Person* ListRemoveByKey(Person* _head, int _key, Person** _p)
{
	Person* tempNextPtr = NULL;
	Person* curr = _head;
	Person* prev = curr;
	
	if(!_p)
	{
		return _head;
	}
	
	if(!_head)
	{
		*_p = NULL;
		return _head; /* returns NULL */
	}
	
	if(_head->m_id == _key)
	{
		tempNextPtr = _head->m_next;
		_head->m_next = NULL;
		*_p = _head;
		
		return tempNextPtr;
	}
	
	while(curr)
	{
		prev = curr;
		curr = curr->m_next;
		
		if(!curr)
		{
			*_p = NULL;
			break;
		}
		
		if(curr->m_id == _key)
		{
			prev->m_next = curr->m_next;
			curr->m_next = NULL;
			*_p = curr;
			break;
		}	
	}
	
	return _head;
}


Person* ListRemoveByKeyRec(Person* _head, int _key, Person** _p)
{
	Person* tempNextPtr = NULL;
	
	if(!_p)
	{
		return _head;
	}
	
	if(!_head)
	{
		/* The key not found */
		*_p = NULL;
		return _head; /* returns NULL */
	}
	
	if(_head->m_id == _key)
	{
		tempNextPtr = _head->m_next;
		_head->m_next = NULL;
		*_p = _head;
		
		return tempNextPtr; /* The head will be removed because the current head's next pointer will be the next of the previous */
	}
	
	_head->m_next = ListRemoveByKeyRec(_head->m_next, _key, _p);

	return _head;
}


void PrintList(Person* _head)
{
	while(_head)
	{
		if(!_head->m_next)
		{
			printf("[{%d, %s, %d}] ---> [NULL]\n", _head->m_id, _head->m_name, _head->m_age);
		}
		else
		{
			printf("[{%d, %s, %d}] ---> ", _head->m_id, _head->m_name, _head->m_age);
		}
		
		_head = _head->m_next;	
	}
}


