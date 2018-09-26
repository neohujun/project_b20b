/******************************************************************************
**
**								Copyright (C) 2015  
**				Smartauto(China) Automotive Information System Co.,Ltd.
**								All rights reserved.
**
**----------------------------------File Info----------------------------------
**
**	Filename:		list.c
**	Abstract:		list buffer
**	Creat By:		Zeno Liu
**	Creat Time:		2015.03.05
**	Version:		v1.0
**
******************************************************************************/

/* \brief
	include
*/
#include "list.h"
#include <string.h>
#include "Error.h"

/* \brief
	include
*/
//#pragma MESSAGE DISABLE C1420

/* \brief
	Func Name:		xListStatus ListCreate(List* pxList)
	Parameters:		List* pxList
	Return Value:	ListStatus
	Abstract:		creat a list
*/
ListStatus xListCreate(List* pxList)
{
	if(NULL == pxList)
	{
		return LIST_ERR;
	}
	
//	if(pxList->base == NULL)
//	{
//		pxList->base = (BYTE*)malloc(pxList->elem_length * pxList->size);
//	}
//	else
//	{
//		pxList->base = (BYTE*)realloc(pxList->base, pxList->elem_length * pxList->size);
//	}
	if(NULL == pxList->base)
	{
		return LIST_ERR;
	}
	
	pxList->front =
	pxList->rear = 
	pxList->itor = 0;
	
	return LIST_OK;
}

/* \brief
	Func Name:		xListStatus ListPushBack(List* pxList, const BYTE* pbElem)
	Parameters:		List* pxList, BYTE* pbElem
	Return Value:	ListStatus
	Abstract:		add element to back of list
*/
ListStatus xListPushBack(List* pxList, const BYTE* pbElem)
{
	if((NULL == pxList) || (NULL == pbElem) || (NULL == pxList->base))
	{
		return LIST_ERR;
	}
	
	if((pxList->rear / pxList->elem_length + 1) % pxList->size == (pxList->front / pxList->elem_length))
	{//list full
		return LIST_OVERFLOW_ERR;
	}

//	DisableInterrupts;
	(void)memcpy(&pxList->base[pxList->rear], pbElem, pxList->elem_length);
	
	pxList->rear = (pxList->rear + pxList->elem_length) % (pxList->size * pxList->elem_length);
//	EnableInterrupts;
	
	return LIST_OK;
}

/* \brief
	Func Name:		xListStatus ListPushFront(List* pxList, const BYTE* pbElem)
	Parameters:		List* pxList, BYTE* pbElem
	Return Value:	ListStatus
	Abstract:		add element to front of list
*/
ListStatus xListPushFront(List* pxList, const BYTE* pbElem)
{
	if((NULL == pxList) || (NULL == pbElem) || (NULL == pxList->base))
	{
		return LIST_ERR;
	}
	
	if((pxList->rear / pxList->elem_length + 1) % pxList->size == (pxList->front / pxList->elem_length))
	{//list full
		return LIST_OVERFLOW_ERR;
	}

//	DisableInterrupts;
	pxList->front = ((pxList->front + pxList->size * pxList->elem_length) - pxList->elem_length) % (pxList->size * pxList->elem_length);
	
	(void)memcpy(&pxList->base[pxList->front], pbElem, pxList->elem_length);
//	EnableInterrupts;
	
	return LIST_OK;
}

/* \brief
	Func Name:		xListStatus ListPopFront(List* pxList, BYTE* pbElem)
	Parameters:		List* pxList, BYTE* pbElem
	Return Value:	ListStatus
	Abstract:		return front element pointer and delete it
*/
ListStatus xListPopFront(List* pxList, BYTE* pbElem)
{
	if((NULL == pxList) || (NULL == pbElem) || (NULL == pxList->base))
	{
		return LIST_ERR;
	}
	
	if(isListEmpty(*pxList))
	{
		return LIST_ERR;
	}

//	DisableInterrupts;
	(void)memcpy(pbElem, &pxList->base[pxList->front], pxList->elem_length);

	pxList->front = (pxList->front + pxList->elem_length) % (pxList->size * pxList->elem_length);
//	EnableInterrupts;
	
	return LIST_OK;
}

/* \brief
	Func Name:		xListStatus ListFront(List* pxList, BYTE* pbElem)
	Parameters:		List* pxList, BYTE* pbElem
	Return Value:	ListStatus
	Abstract:		return front element pointer, but not delete
*/
ListStatus xListFront(List xList, BYTE* pbElem)
{
	if((NULL == pbElem) || (NULL == xList.base))
	{
		return LIST_ERR;
	}
	
	if(isListEmpty(xList))
	{
		return LIST_ERR;
	}
	
	(void)memcpy(pbElem, &xList.base[xList.front], xList.elem_length);
	
	return LIST_OK;
}

/* \brief
	Func Name:		xListStatus ListClear(List* pxList)
	Parameters:		List* pxList
	Return Value:	ListStatus
	Abstract:		clear list
*/
ListStatus xListClear(List* pxList)
{
	if((NULL == pxList) || (NULL == pxList->base))
	{
		return LIST_ERR;
	}
	
	pxList->front =
	pxList->rear = 0;
	
	return LIST_OK;
}

/* \brief
	Func Name:		BOOL isListEmpty(List Q)
	Parameters:		List Q
	Return Value:	BOOL	TRUE:empty
	Abstract:		if list empty
*/
BOOL isListEmpty(List xList)
{
	if(xList.front == xList.rear)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

/* \brief
	Func Name:
	Parameters:
	Return Value:
	Abstract:
*/
WORD wListLength(List xList)
{
	if(NULL == xList.base)
	{
		return 0;
	}

	if(xList.rear >= xList.front)
	{
		return ((xList.rear - xList.front) / xList.elem_length);
	}
	else
	{
		return ((xList.rear + (xList.elem_length * xList.size) - xList.front) / xList.elem_length);
	}
}

/* \brief
	Func Name:
	Parameters:
	Return Value:
	Abstract:
*/
WORD wListBegin(List xList)
{
	if((1 != xList.elem_length) || (NULL == xList.base))
	{
		return 0;
	}
	
	return xList.front;
}

/* \brief
	Func Name:
	Parameters:
	Return Value:
	Abstract:
*/
WORD wListEnd(List xList)
{
	if((1 != xList.elem_length) || (NULL == xList.base))
	{
		return 0;
	}
	
	return xList.rear;
}

/* \brief
	Func Name:
	Parameters:
	Return Value:
	Abstract:
*/
ListStatus xListFind(List xList, WORD index, BYTE* pbElem)
{
	if((NULL == pbElem) || (NULL == xList.base) ||
		(1 != xList.elem_length) || isListEmpty(xList) ||
		//index out of range
		((xList.rear >= xList.front) &&(index < xList.front)) ||
		((xList.rear >= xList.front) &&(index >= xList.rear)) ||
		((xList.rear < xList.front) &&(index < xList.front) && (index >= xList.rear)))
	{
		return LIST_ERR;
	}

	*pbElem = xList.base[index];
	
	return LIST_OK;
}

/* \brief
	Func Name:
	Parameters:
	Return Value:
	Abstract:
*/
ListStatus xListErase(List* pxList, BYTE length)
{
	if((NULL == pxList) || (NULL == pxList->base)
		|| isListEmpty(*pxList) || (1 != pxList->elem_length))
	{
		return LIST_ERR;
	}

	if(length >= wListLength(*pxList))
	{
		pxList->front = pxList->rear;
	}
	else
	{
		pxList->front = ((pxList->front + length) % pxList->size);
	}
	
	return LIST_OK;
}

/* \brief
	Func Name:
	Parameters:
	Return Value:
	Abstract:
*/
ListStatus xListSetIterator(List* pxList, WORD wItor)
{
	if((NULL == pxList) || (NULL == pxList->base)
		|| (1 != pxList->elem_length) )
	{
		return LIST_ERR;
	}

	pxList->itor = wItor%pxList->size;
	
	return LIST_OK;
}

/* \brief
	Func Name:
	Parameters:
	Return Value:
	Abstract:
*/
ListStatus xListIteratorIncrement(List* pxList, WORD wIncrementData)
{
	if((NULL == pxList) || (NULL == pxList->base)
		|| (1 != pxList->elem_length) )
	{
		return LIST_ERR;
	}

	pxList->itor += wIncrementData;
	pxList->itor %= pxList->size;
	
	return LIST_OK;
}

/* \brief
*/
ListStatus xListPushBackByte(List* pxList, const BYTE Elem)
{
	if((pxList->rear + 1) % pxList->size == pxList->front)
	{//list full
		return LIST_OVERFLOW_ERR;
	}
	
	pxList->base[pxList->rear] = Elem;
	
	pxList->rear = (pxList->rear + 1) % pxList->size;
	
	return LIST_OK;
}

