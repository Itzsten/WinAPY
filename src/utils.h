#pragma once
#include <Windows.h>
#include <stdio.h>
#include "Python.h"
#include "utils.c" // this is all your fault visual studio 😐
#include "structmember.h"

#define MIDIHDR_TUPLE_TO_STRUCT_FAILED(hdr) (hdr.reserved == -69420)

#if PY_MAJOR_VERSION >= 3
#define PyString_ToCharArr PyUnicode_AsUTF8
#else
#define PyString_ToCharArr PyString_AsString
#endif

BOOL WINAPI ErrorCheckMME(MMRESULT ret);
BOOL WINAPI RaiseExceptionCheck(BOOL bSuccess);

typedef PyObject* PyOb;
