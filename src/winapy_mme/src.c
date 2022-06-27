/*
    WinAPY - Windows API wrapper in C developed for Python.
    Copyright (c) 2022 Itzsten
*/

#include "Python.h"
#include <Windows.h>
#include <stdio.h>
#include <mmeapi.h>

#pragma comment(lib, "Winmm.lib")

#if PY_MAJOR_VERSION >= 3
#define PyString_ToCharArr PyUnicode_AsUTF8
#else
#define PyString_ToCharArr PyString_AsString
#endif

BOOL WINAPI RaiseExceptionCheck(BOOL bSuccess) {
    if (GetLastError() && (!bSuccess)) {
        PyErr_SetFromWindowsErr(GetLastError());
        return TRUE;
    }
    return FALSE;
}

BOOL WINAPI ErrorCheckMME(MMRESULT ret) {
    if (!ret) {
        return FALSE;
    }
    if (ret == MMSYSERR_ALLOCATED) PyErr_SetString(PyExc_MemoryError, "Specified resource is already allocated.");
    if (ret == MMSYSERR_BADDEVICEID) PyErr_SetString(PyExc_ValueError, "Specified device identifier is out of range.");
    if (ret == MMSYSERR_NODRIVER) PyErr_SetString(PyExc_SystemError, "No device driver is present.");
    if (ret == MMSYSERR_NOMEM) PyErr_SetString(PyExc_MemoryError, "Unable to allocate or lock memory.");
    if (ret == MMSYSERR_NOMEM) PyErr_SetString(PyExc_ValueError, "Attempted to open with an unsupported waveform-audio format.");
    if (ret == WAVERR_SYNC) PyErr_SetString(PyExc_StopAsyncIteration, "The device is synchronous but waveOutOpen was called without using the WAVE_ALLOWSYNC flag.");
    if (ret == MMSYSERR_INVALHANDLE) PyErr_SetString(PyExc_TypeError, "Specified device handle is invalid.");
    if (ret == WAVERR_UNPREPARED) PyErr_SetString(PyExc_ValueError, "The data block pointed to by the pwh parameter hasn't been prepared.");
    if (ret == WAVERR_STILLPLAYING) PyErr_SetString(PyExc_TimeoutError, "There are still buffers in the queue.");
    return TRUE;
}

static PyObject* PywaveOutOpen(PyObject* self, PyObject* args) {
    PyObject* obOut;
    UINT deviceId;
    WAVEFORMATEX wFormat;
    DWORD_PTR dwCallback, dwInstance;
    DWORD fdwOpen;
    HWAVEOUT hOut = 0;
    
    if (!PyArg_ParseTuple(args, "Ok(HHkkHH)KKk",
        &obOut,
        &deviceId,
        &wFormat.wFormatTag, &wFormat.nChannels, &wFormat.nSamplesPerSec,
        &wFormat.nAvgBytesPerSec, &wFormat.nBlockAlign, &wFormat.wBitsPerSample,
        &dwCallback,
        &dwInstance,
        &fdwOpen
    )) return NULL;

    if (obOut != Py_None) hOut = PyLong_AsLong(obOut);
    
    MMRESULT res = waveOutOpen(&hOut, deviceId, &wFormat, dwCallback, dwInstance, fdwOpen);
    
    if (ErrorCheckMME(res)) return NULL;
    return Py_BuildValue("L", hOut);
}

static PyObject* PywaveOutPrepareHeader(PyObject* self, PyObject* args) {
    HWAVEOUT hwo;
    WAVEHDR hdr;
    UINT cbwh = sizeof(WAVEHDR);
    PyObject* obData;

    if (!PyArg_ParseTuple(args, "L(OkKkk)",
        &hwo,
        &obData,
        &hdr.dwBytesRecorded,
        &hdr.dwUser,
        &hdr.dwFlags,
        &hdr.dwLoops
    )) return NULL;

    if (!PyByteArray_Check(obData)) {
        PyErr_SetString(PyExc_TypeError, "argument 2: index 1: Excepted bytes type object");
        return NULL;
    }
    
    LONGLONG length = PyByteArray_Size(obData);

    hdr.dwBufferLength = length;
    hdr.lpData = PyByteArray_AsString(obData);
    hdr.lpNext = 0;
    hdr.reserved = 0;

    if (hdr.lpData == NULL) {
        PyErr_SetString(PyExc_TypeError, "Could not convert bytearray to char pointer.");
        return NULL;
    }

    MMRESULT res = waveOutPrepareHeader(hwo, &hdr, cbwh);

    if (ErrorCheckMME(res)) return NULL;
    return Py_BuildValue("(OkKkk)", 
        PyByteArray_FromStringAndSize(hdr.lpData, length),
        hdr.dwBytesRecorded,
        hdr.dwUser,
        hdr.dwFlags,
        hdr.dwLoops);
}
static PyObject* PywaveOutClose(PyObject* self, PyObject* args) {
    HWAVEOUT hwo;
    if (!PyArg_ParseTuple(args, "L", &hwo)) return NULL;
    if (ErrorCheckMME(waveOutClose(hwo)))   return NULL;
    return Py_True;
}
static PyObject* PywaveOutUnprepareHeader(PyObject* self, PyObject* args) {
    HWAVEOUT hwo;
    WAVEHDR hdr;
    UINT cbwh = sizeof(WAVEHDR);
    PyObject* obData;

    if (!PyArg_ParseTuple(args, "L(OkKkk)",
        &hwo,
        &obData,
        &hdr.dwBytesRecorded,
        &hdr.dwUser,
        &hdr.dwFlags,
        &hdr.dwLoops
    )) return NULL;

    if (!PyByteArray_Check(obData)) {
        PyErr_SetString(PyExc_TypeError, "argument 2: index 1: Excepted bytes type object");
        return NULL;
    }

    LONGLONG length = PyByteArray_Size(obData);

    hdr.dwBufferLength = length;
    hdr.lpData = PyByteArray_AsString(obData);
    hdr.lpNext = 0;
    hdr.reserved = 0;

    if (hdr.lpData == NULL) {
        PyErr_SetString(PyExc_TypeError, "Could not convert bytearray to char pointer.");
        return NULL;
    }
    MMRESULT res = waveOutUnprepareHeader(hwo, &hdr, cbwh);

    if (ErrorCheckMME(res)) return NULL;
    return Py_True;
}
static PyObject* PywaveOutWrite(PyObject* self, PyObject* args) {
    HWAVEOUT hwo;
    WAVEHDR hdr;
    UINT cbwh = sizeof(WAVEHDR);
    PyObject* obData;

    if (!PyArg_ParseTuple(args, "L(OkKkk)",
        &hwo,
        &obData,
        &hdr.dwBytesRecorded,
        &hdr.dwUser,
        &hdr.dwFlags,
        &hdr.dwLoops
    )) return NULL;

    if (!PyByteArray_Check(obData)) {
        PyErr_SetString(PyExc_TypeError, "argument 2: index 1: Excepted bytes type object");
        return NULL;
    }

    LONGLONG length = PyByteArray_Size(obData);

    hdr.dwBufferLength = length;
    hdr.lpData = PyByteArray_AsString(obData);
    hdr.lpNext = 0;
    hdr.reserved = 0;

    if (hdr.lpData == NULL) {
        PyErr_SetString(PyExc_TypeError, "Could not convert bytearray to char pointer.");
        return NULL;
    }

    MMRESULT res = waveOutWrite(hwo, &hdr, cbwh);

    if (ErrorCheckMME(res)) return NULL;
    return Py_True;
}

static PyMethodDef module_methods[] = {
    { "waveOutOpen", PywaveOutOpen, METH_VARARGS },
    { "waveOutPrepareHeader", PywaveOutPrepareHeader, METH_VARARGS },
    { "waveOutClose", PywaveOutClose, METH_VARARGS },
    { "waveOutUnprepareHeader", PywaveOutUnprepareHeader, METH_VARARGS },
    { "waveOutWrite", PywaveOutWrite, METH_VARARGS },

    /* sentinel */
    { 0 }
};

static struct PyModuleDef ModuleCombinations =
{
    PyModuleDef_HEAD_INIT,
    "WinAPY_mme", /* name of module */
    NULL,
    -1,   /* size of per-interpreter state of the module, or -1 if the module keeps state in global variables. */
    module_methods
};


void PyInit_winapy_mme(void) {
    PyModule_Create(&ModuleCombinations);
}
