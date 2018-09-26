/******************************************************************************
**
**								Copyright (C) 2015  
**				Smartauto(China) Automotive Information System Co.,Ltd.
**								All rights reserved.
**
**----------------------------------File Info----------------------------------
**
**	Filename:		list.h
**	Abstract:		head of list buffer
**	Creat By:		Zeno Liu
**	Creat Time:		2015.03.05
**	Version:		v1.0
**
******************************************************************************/

#ifndef	_LIST_H_
#define	_LIST_H_

/* \brief
	include
*/
#include "type.h"

/* \brief
	list return status
*/
typedef enum
{
	LIST_OK,
	LIST_ERR,
	LIST_OVERFLOW_ERR,
}ListStatus;

/* \brief
	list struct
*/
typedef struct
{
	BYTE *base;			//user malloc buffer
	WORD front;			//head pointer
	WORD rear;			//tail pointer
	WORD itor;			//iterator
	WORD size;			//max element count
	BYTE elem_length;	//element length
}List;

/* \brief
	function declaration
*/
ListStatus xListCreate(List* pxList);
ListStatus xListDestroy(List* pxList);
ListStatus xListPushBack(List* pxList, const BYTE* pbElem);
ListStatus xListPushFront(List* pxList, const BYTE* pbElem);
ListStatus xListPopFront(List* pxList, BYTE* pbElem);
ListStatus xListFront(List xList, BYTE* pbElem);
ListStatus xListClear(List* pxList);
BOOL isListEmpty(List xList);
WORD wListLength(List xList);

//interface for random length frame(elem_length = 1)
WORD wListBegin(List xList);
WORD wListEnd(List xList);
ListStatus xListFind(List xList, WORD index, BYTE* pbElem);
ListStatus xListErase(List* pxList, BYTE length);
ListStatus xListSetIterator(List* pxList, WORD wItor);
ListStatus xListIteratorIncrement(List* pxList, WORD wIncrementData);

ListStatus xListPushBackByte(List* pxList, const BYTE Elem);

#endif
