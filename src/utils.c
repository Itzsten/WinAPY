#pragma once
#include <Windows.h>
#include "Python.h"
#pragma comment(lib, "winmm.lib")

BOOL WINAPI ErrorCheckMME(MMRESULT ret) {
    if (!ret) {
        return FALSE;
    }
    CHAR err[512];
    CHAR res[520];
    if (!waveOutGetErrorTextA(ret, err, 512)) {
        sprintf_s(res, 520, "[MMSYSERR %u] %s", ret, err);
        PyErr_SetString(PyExc_SystemError, res);
    }
    return TRUE;
}


BOOL WINAPI RaiseExceptionCheck(BOOL bSuccess) {
    if (GetLastError() && (!bSuccess)) {
        PyErr_SetFromWindowsErr(GetLastError());
        return TRUE;
    }
    return FALSE;
}
