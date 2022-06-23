/*
    WinAPY - Windows API wrapper in C developed for Python.
    Copyright (c) 2022 Itzsten
*/

// Mainly developed for my YouTube channel, however functions asked to be added probably will.
// Constant definitions? Meh.

// dang everything in a single file? :savage:

#include <Python.h>
#include <Windows.h>
#include <stdio.h>

#pragma comment(lib, "user32.lib")
#pragma comment(lib, "gdi32.lib")
#pragma comment(lib, "advapi32.lib")

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

typedef struct tagPYENUMCHILDWINDOWPARAMS {
    PyObject* func;
    PyObject* lParam;
} PYENUMCHILDWINDOWPARAMS, *PPYENUMCHILDWINDOWPARAMS, *LPPYENUMCHILDWINDOWPARAMS;

static PyObject* PyGetDC(PyObject* self, PyObject* args) {
    LONG hWnd;
    PyObject* pyhWnd;

    if (!PyArg_ParseTuple(args, "O", &pyhWnd)) {
        return NULL;
    }
    if (pyhWnd == Py_None) {
        hWnd = NULL;
    } else {
        hWnd = PyLong_AsLong(pyhWnd);
    }

    HDC res = GetDC(hWnd ? (HANDLE)&hWnd : 0);

    if (RaiseExceptionCheck(res)) return NULL;

    return Py_BuildValue("l", (LONG)res);
}
static PyObject* PyPatBlt(PyObject* self, PyObject* args) {
    LONG hdc;
    INT x1;
    INT y1;
    INT x2;
    INT y2;
    LONG rasterOperation;

    if (!PyArg_ParseTuple(args, "liiiil", &hdc, &x1, &y1, &x2, &y2, &rasterOperation)) {
        return NULL;
    }
    if (RaiseExceptionCheck(PatBlt((HANDLE)hdc, x1, y1, x2, y2, rasterOperation))) return NULL;
    return Py_BuildValue("b", TRUE);
}
static PyObject* PyBitBlt(PyObject* self, PyObject* args) {
    LONG hdc;
    LONG hdcSrc;
    INT x1;
    INT y1;
    INT x2;
    INT y2;
    INT r1;
    INT r2;
    LONG rasterOperation;

    if (!PyArg_ParseTuple(args, "liiiiliil", &hdc, &x1, &y1, &x2, &y2, &hdcSrc, &r1, &r2, &rasterOperation)) {
        return NULL;
    }
    if (RaiseExceptionCheck(BitBlt((HANDLE)hdc, x1, y1, x2, y2, (HANDLE)hdcSrc, r1, r2, rasterOperation))) return NULL;
    return Py_BuildValue("b", TRUE);
}
static PyObject* PyDeleteDC(PyObject* self, PyObject* args) {
    LONG hdc;

    if (!PyArg_ParseTuple(args, "l", &hdc)) {
        return NULL;
    }
    if (RaiseExceptionCheck(DeleteDC((HANDLE)hdc))) { return NULL; }
    return Py_BuildValue("b", TRUE);
}
static PyObject* PyGetSystemMetrics(PyObject* self, PyObject* args) {
    INT metric;
    if (!PyArg_ParseTuple(args, "i", &metric)) {
        return NULL;
    }
    INT a = GetSystemMetrics(metric);
    if (RaiseExceptionCheck(a)) { return NULL; }
    return Py_BuildValue("i", a);
}
static PyObject* PyGetDesktopWindow(PyObject* self, PyObject* args) {
    LONG window = (LONG)GetDesktopWindow();
    if (RaiseExceptionCheck(window)) { return NULL; }
    return Py_BuildValue("l", window);
}
static PyObject* PyGetLastError(PyObject* self, PyObject* args) { 
    return Py_BuildValue("i", GetLastError()); 
}
static PyObject* PyRectangle(PyObject* self, PyObject* args) {
    LONG hdc;
    INT x1;
    INT y1;
    INT x2;
    INT y2;

    if (!PyArg_ParseTuple(args, "liiii", &hdc, &x1, &y1, &x2, &y2)) {
        return NULL;
    }
    if (RaiseExceptionCheck(Rectangle((HANDLE)hdc, x1, y1, x2, y2))) return NULL;
    return Py_BuildValue("b", TRUE);
}
static PyObject* PyEllipse(PyObject* self, PyObject* args) {
    LONG hdc;
    INT x1;
    INT y1;
    INT x2;
    INT y2;

    if (!PyArg_ParseTuple(args, "liiii", &hdc, &x1, &y1, &x2, &y2)) {
        return NULL;
    }
    if (RaiseExceptionCheck(Ellipse((HANDLE)hdc, x1, y1, x2, y2))) return NULL;
    return Py_BuildValue("b", TRUE);
}
static PyObject* PyCreateSolidBrush(PyObject* self, PyObject* args) {
    LONG color;
    if (!PyArg_ParseTuple(args, "l", &color)) return NULL;
    HBRUSH brush = CreateSolidBrush(color);
    if (RaiseExceptionCheck(brush)) return NULL;
    return Py_BuildValue("l", (LONG)brush);
}
static PyObject* PyDeleteObject(PyObject* self, PyObject* args) {
    LONG object;
    if (!PyArg_ParseTuple(args, "l", &object)) return NULL;
    if (RaiseExceptionCheck(DeleteObject((HANDLE)object))) return NULL;
    return Py_BuildValue("b", TRUE);
}
static PyObject* PyRGB(PyObject* self, PyObject* args) {
    INT r, g, b;
    if (!PyArg_ParseTuple(args, "iii", &r, &g, &b)) return NULL;
    return Py_BuildValue("l", RGB(r, g, b));
}
static PyObject* PySelectObject(PyObject* self, PyObject* args) {
    HGDIOBJ longObj;
    LONG longHdc;
    PyObject* obObj;
    if (!PyArg_ParseTuple(args, "lO", &longHdc, &obObj)) return NULL;

    if (obObj == Py_None) {
        longObj = NULL;
    } else {
        longObj = PyLong_AsLong(obObj);
    }

    HGDIOBJ objOld = SelectObject((HANDLE)longHdc, longObj);
    if (RaiseExceptionCheck(objOld)) return NULL;
    return Py_BuildValue("l", objOld);
}
static PyObject* PyStretchBlt(PyObject* self, PyObject* args) {
    
    INT xDest, yDest, wDest, hDest, xSrc, ySrc, wSrc, hSrc;
    LONG hdcDst, hdcSrc, rasterOperation;

    if (!PyArg_ParseTuple(args, "liiiiliiiil", &hdcDst, &xDest, &yDest, &wDest, &hDest, &hdcSrc, &xSrc, &ySrc, &wSrc, &hSrc, &rasterOperation)) return NULL;
    if (RaiseExceptionCheck(StretchBlt( (HANDLE)hdcDst, xDest, yDest, wDest, hDest, (HANDLE)hdcSrc, xSrc, ySrc, wSrc, hSrc, rasterOperation ))) return NULL;
    return Py_BuildValue("O", PyBool_FromLong(TRUE));
}
static PyObject* PySetStretchBltMode(PyObject* self, PyObject* args) {
    INT nMode;
    LONG hdcDst;

    if (!PyArg_ParseTuple(args, "li", &hdcDst, &nMode)) return NULL;
    INT val = SetStretchBltMode(hdcDst, nMode);
    if (RaiseExceptionCheck(val)) return NULL;
    return Py_BuildValue("i", val);
}
static PyObject* PySetPixel(PyObject* self, PyObject* args) {
    INT x, y;
    LONG hdcDst, color;

    if (!PyArg_ParseTuple(args, "liil", &hdcDst, &x, &y, &color)) return NULL;
    LONG val = SetPixel((HANDLE)hdcDst, x, y, color);
    if (val == -1) { PyErr_SetFromWindowsErr(87); return NULL; }
    return Py_BuildValue("l", val);
}
static PyObject* PyGetStretchBltMode(PyObject* self, PyObject* args) {
    LONG h;
    if (!PyArg_ParseTuple(args, "l", &h)) return NULL;
    INT res = GetStretchBltMode((HANDLE)h);
    if (RaiseExceptionCheck(res)) return NULL;
    return Py_BuildValue("i", res);
}
static PyObject* PyGetPixel(PyObject* self, PyObject* args) {
    LONG h;
    INT x, y;
    if (!PyArg_ParseTuple(args, "lii", &h, &x, &y)) return NULL;
    LONG res = GetPixel(h, x, y);
    if (res == CLR_INVALID) { PyErr_SetFromWindowsErr(87); return NULL; }
    return Py_BuildValue("l", res);
}
static PyObject* PyGetRValue(PyObject* self, PyObject* args) {
    LONG h;
    if (!PyArg_ParseTuple(args, "l", &h)) return NULL;
    return Py_BuildValue("i", GetRValue(h));
}
static PyObject* PyGetGValue(PyObject* self, PyObject* args) {
    LONG h;
    if (!PyArg_ParseTuple(args, "l", &h)) return NULL;
    return Py_BuildValue("i", GetGValue(h));
}
static PyObject* PyGetBValue(PyObject* self, PyObject* args) {
    LONG h;
    if (!PyArg_ParseTuple(args, "l", &h)) return NULL;
    return Py_BuildValue("i", GetBValue(h));
}
static PyObject* PyGetRGBValues(PyObject* self, PyObject* args) {
    LONG h;
    if (!PyArg_ParseTuple(args, "l", &h)) return NULL;
    return Py_BuildValue("(iii)", GetRValue(h), GetGValue(h), GetBValue(h));
}
static PyObject* PyGetDCPenColor(PyObject* self, PyObject* args) {
    LONG h;
    if (!PyArg_ParseTuple(args, "l", &h)) return NULL;
    LONG res = GetDCPenColor(h);
    if (res == CLR_INVALID) { PyErr_SetFromWindowsErr(87); return NULL; }
    return Py_BuildValue("l", res);
}
static PyObject* PyGetDCBrushColor(PyObject* self, PyObject* args) {
    LONG h;
    if (!PyArg_ParseTuple(args, "l", &h)) return NULL;
    LONG res = GetDCBrushColor(h);
    if (res == CLR_INVALID) { PyErr_SetFromWindowsErr(87); return NULL; }
    return Py_BuildValue("l", res);
}
static PyObject* PyGetDCOrgEx(PyObject* self, PyObject* args) {
    LONG h;
    if (!PyArg_ParseTuple(args, "l", &h)) return NULL;
    POINT p;
    GetDCOrgEx(h, &p);
    return Py_BuildValue("(ll)", p.x, p.y);
}
static PyObject* PyGetDeviceCaps(PyObject* self, PyObject* args) {
    LONG h;
    INT index;
    if (!PyArg_ParseTuple(args, "li", &h, &index)) return NULL;
    INT val = GetDeviceCaps(h, index);
    return Py_BuildValue("i", val); 
}
static PyObject* PyGetObjectType(PyObject* self, PyObject* args) {
    LONG h;
    if (!PyArg_ParseTuple(args, "l", &h)) return NULL;
    LONG val = GetObjectType(h);
    if (RaiseExceptionCheck(val)) return NULL;
    return Py_BuildValue("l", val);
}
static PyObject* PyChord(PyObject* self, PyObject* args) {
    LONG hdc;
    int x1, y1, x2, y2, x3, y3, x4, y4;
    if (!PyArg_ParseTuple(args, "liiiiiiii", &hdc, &x1, &y1, &x2, &y2, &x3, &y3, &x4, &y4)) return NULL;
    if (!Chord(hdc, x1, y1, x2, y2, x3, y3, x4, y4)) {
        if (RaiseExceptionCheck(0)) return NULL;
    }
    return Py_BuildValue("O", PyBool_FromLong(TRUE)); 
}
static PyObject* PyPie(PyObject* self, PyObject* args) {
    LONG hdc;
    int x1, y1, x2, y2, x3, y3, x4, y4;
    if (!PyArg_ParseTuple(args, "liiiiiiii", &hdc, &x1, &y1, &x2, &y2, &x3, &y3, &x4, &y4)) return NULL;
    if (!Pie(hdc, x1, y1, x2, y2, x3, y3, x4, y4)) {
        if (RaiseExceptionCheck(0)) return NULL;
    }
    return Py_BuildValue("O", PyBool_FromLong(TRUE)); 
}
static PyObject* Pyrand(PyObject* self, PyObject* args) {
    return Py_BuildValue("i", rand());
}
static PyObject* PySleep(PyObject* self, PyObject* args) {
    LONG amn;
    if (!PyArg_ParseTuple(args, "l", &amn)) return NULL;
    Sleep(amn);
    return Py_BuildValue("O", Py_None);;
}
static PyObject* PySaveDC(PyObject* self, PyObject* args) {
    LONG hdc;
    if (!PyArg_ParseTuple(args, "l", &hdc)) return NULL;
    INT val = SaveDC(hdc);
    if (RaiseExceptionCheck(val)) return NULL;
    return Py_BuildValue("i", val);
}
static PyObject* PyRestoreDC(PyObject* self, PyObject* args) {
    LONG hdc;
    INT saved;
    if (!PyArg_ParseTuple(args, "li", &hdc, &saved)) return NULL;
    RestoreDC(hdc, saved);
    return Py_BuildValue("O", PyBool_FromLong(TRUE));
}
static PyObject* PyArc(PyObject* self, PyObject* args) {
    LONG hdc;
    int x1, y1, x2, y2, x3, y3, x4, y4;
    if (!PyArg_ParseTuple(args, "liiiiiiii", &hdc, &x1, &y1, &x2, &y2, &x3, &y3, &x4, &y4)) return NULL;
    if (!Arc(hdc, x1, y1, x2, y2, x3, y3, x4, y4)) {
        if (RaiseExceptionCheck(0)) return NULL;
    }
    return Py_BuildValue("O", PyBool_FromLong(TRUE));
}
static PyObject* PyArcTo(PyObject* self, PyObject* args) {
    LONG hdc;
    int x1, y1, x2, y2, x3, y3, x4, y4;
    if (!PyArg_ParseTuple(args, "liiiiiiii", &hdc, &x1, &y1, &x2, &y2, &x3, &y3, &x4, &y4)) return NULL;
    if (!ArcTo(hdc, x1, y1, x2, y2, x3, y3, x4, y4)) {
        if (RaiseExceptionCheck(0)) return NULL;
    }
    return Py_BuildValue("O", PyBool_FromLong(TRUE));
}
static PyObject* PyAngleArc(PyObject* self, PyObject* args) {
    LONG hdc;
    INT x, y;
    DWORD r;
    FLOAT start, sweep;

    if (!PyArg_ParseTuple(args, "liikff", &hdc, &x, &y, &r, &start, &sweep)) return NULL;

    if (!AngleArc(hdc, x, y, r, start, sweep)) {
        if (RaiseExceptionCheck(0)) return NULL;
    }

    return Py_BuildValue("O", PyBool_FromLong(TRUE));
}
static PyObject* PySetArcDirection(PyObject* self, PyObject* args) {
    LONG hdc;
    INT dir;

    if (!PyArg_ParseTuple(args, "li", &hdc, &dir)) return NULL;

    INT val = SetArcDirection(hdc, dir);
    if (RaiseExceptionCheck(val)) return NULL;

    return Py_BuildValue("i", val);
}
static PyObject* PyGetStockObject(PyObject* self, PyObject* args) {
    INT index;

    if (!PyArg_ParseTuple(args, "i", &index)) return NULL;

    INT val = GetStockObject(index);
    if (RaiseExceptionCheck(val)) return NULL;

    return Py_BuildValue("l", val);
}
static PyObject* PyRedrawWindow(PyObject* self, PyObject* args) {
    INT flags = RDW_ALLCHILDREN | RDW_ERASE | RDW_INVALIDATE;

    HWND hwnd;
    HRGN hrgn;

    PyObject* obRect = Py_None;
    PyObject* obRgn = Py_None;
    PyObject* obWnd = Py_None;
    if (!PyArg_ParseTuple(args, "OOO|i", &obWnd, &obRect, &obRgn, &flags)) return NULL;

    RECT rect;
    RECT* pRect;
    if (obRect == Py_None) {
        pRect = NULL;
    } else {
        if (!PyArg_ParseTuple(args, "O(iiii)O|i", &obWnd, &rect.left, &rect.top, &rect.right, &rect.bottom, &obRgn, &flags))
            return NULL;
        pRect = &rect;
    }
    if (obWnd == Py_None) {
        hwnd = NULL;
    } else {
        hwnd = PyLong_AsLong(obWnd);
    }
    if (obRgn == Py_None) {
        hrgn = NULL;
    } else {
        hrgn = PyLong_AsLong(obRgn);
    }

    return Py_BuildValue("O", PyBool_FromLong(RedrawWindow(hwnd, pRect, hrgn, flags)));
}
static PyObject* PyInvalidateRect(PyObject* self, PyObject* args) {
    HWND hwnd;
    BOOL erase;

    PyObject* obRect = Py_None;
    PyObject* obWnd = Py_None;
    PyObject* obBool;

    if (!PyArg_ParseTuple(args, "OOO", &obWnd, &obRect, &obBool)) return NULL;

    erase = PyObject_IsTrue(obBool);

    RECT rect;
    RECT* pRect;
    if (obRect == Py_None) {
        pRect = NULL;
    } else {
        if (!PyArg_ParseTuple(args, "O(iiii)O", &obWnd, &rect.left, &rect.top, &rect.right, &rect.bottom, &obBool))
            return NULL;
        pRect = &rect;
    }
    if (obWnd == Py_None) {
        hwnd = NULL;
    } else {
        hwnd = PyLong_AsLong(obWnd);
    }

    return Py_BuildValue("O", PyBool_FromLong(InvalidateRect(hwnd, pRect, erase)));
}
static PyObject* PyCreateCompatibleDC(PyObject *self, PyObject *args) {
    PyObject* obWnd = Py_None;
    HDC wnd;
    if (!PyArg_ParseTuple(args, "O", &obWnd)) return NULL;
    if (obWnd == Py_None) {
        wnd = NULL;
    } else {
        wnd = PyLong_AsLong(obWnd);
    }
    HDC val = CreateCompatibleDC(wnd);
    if (val == NULL) {
        PyErr_SetFromWindowsErr(87);
        return NULL;
    }
    return Py_BuildValue("l", val);
}
static PyObject* PyCreateCompatibleBitmap(PyObject* self, PyObject* args) {
    HDC wnd;
    INT w, h;
    LONG wndLong;
    if (!PyArg_ParseTuple(args, "lii", &wndLong, &w, &h)) return NULL;
    wnd = (HDC)wndLong;

    HBITMAP val = CreateCompatibleBitmap(wnd, w, h);
    if (val == NULL) {
        PyErr_SetFromWindowsErr(87);
        return NULL;
    }
    return Py_BuildValue("l", val);
}
static PyObject* PyCancelDC(PyObject* self, PyObject* args) {
    HDC wnd;
    INT w, h;
    LONG wndLong;
    if (!PyArg_ParseTuple(args, "l", &wndLong)) return NULL;
    wnd = (HDC)wndLong;

    HBITMAP val = CancelDC(wnd);
    if (val == FALSE) {
        PyErr_SetFromWindowsErr(87);
        return NULL;
    }
    return Py_BuildValue("l", PyBool_FromLong(val));
}
static PyObject* PySelectClipRgn(PyObject* self, PyObject* args) {
    HRGN longObj;
    LONG longHdc;
    PyObject* hObj;
    if (!PyArg_ParseTuple(args, "lO", &longHdc, &hObj)) return NULL;

    if (hObj == Py_None) {
        longObj = NULL;
    } else {
        longObj = (HANDLE)PyLong_AsLong(hObj);
    }

    INT objOld = SelectClipRgn((HANDLE)longHdc, longObj);
    if (RaiseExceptionCheck(objOld)) return NULL;
    return Py_BuildValue("i", objOld);
}
static PyObject* PyCreateEllipticRgn(PyObject* self, PyObject* args) {
    INT x1, y1, x2, y2;

    if (!PyArg_ParseTuple(args, "iiii", &x1, &y1, &x2, &y2)) return NULL;
    HRGN rgn = CreateEllipticRgn(x1, y1, x2, y2);
    if (RaiseExceptionCheck(rgn)) return NULL;
    return Py_BuildValue("l", rgn);
}
static PyObject* PyCreateEllipticRgnIndirect(PyObject* self, PyObject* args) {
    INT x1, y1, x2, y2;

    if (!PyArg_ParseTuple(args, "(iiii)", &x1, &y1, &x2, &y2)) return NULL;
    RECT rect;
    rect.left = x1;
    rect.bottom = y1;
    rect.right = x2;
    rect.top = y2;

    HRGN rgn = CreateEllipticRgnIndirect(&rect);
    if (RaiseExceptionCheck(rgn)) return NULL;
    return Py_BuildValue("l", rgn);
}
static PyObject* PyCreateRectRgn(PyObject* self, PyObject* args) {
    INT x1, y1, x2, y2;

    if (!PyArg_ParseTuple(args, "iiii", &x1, &y1, &x2, &y2)) return NULL;
    HRGN rgn = CreateRectRgn(x1, y1, x2, y2);
    if (RaiseExceptionCheck(rgn)) return NULL;
    return Py_BuildValue("l", rgn);
}
static PyObject* PyCreateRectRgnIndirect(PyObject* self, PyObject* args) {
    INT x1, y1, x2, y2;

    if (!PyArg_ParseTuple(args, "(iiii)", &x1, &y1, &x2, &y2)) return NULL;
    RECT rect;
    rect.left = x1;
    rect.bottom = y1;
    rect.right = x2;
    rect.top = y2;

    HRGN rgn = CreateRectRgnIndirect(&rect);
    if (RaiseExceptionCheck(rgn)) return NULL;
    return Py_BuildValue("l", rgn);
}
static PyObject* PyCreatePolygonRgn(PyObject* self, PyObject* args) {
    PyObject* obPoints;
    INT iMode;

    if (!PyArg_ParseTuple(args, "Oi", &obPoints, &iMode)) return NULL;

    if (!PyTuple_Check(obPoints)) {
        PyErr_SetString(PyExc_TypeError, "arg 1: tuple of points (2 integer tuple) excepted");
        return NULL;
    }
    INT nSize = PyTuple_GET_SIZE(obPoints);
    POINT ppm[512]; // please don't have more than 512 vertices :)
    for (INT i = 0; i < nSize; i++) {
        PyObject* item = PyTuple_GET_ITEM(obPoints, i);
        if ((!PyTuple_Check(item)) || (PyTuple_GET_SIZE(item) != 2)) {
            PyErr_SetString(PyExc_TypeError, "arg 1: tuple of points (2 integer tuple) excepted");
            return NULL;
        }
        ppm[i].x = (INT)PyLong_AsLong(PyTuple_GET_ITEM(item, 0));
        ppm[i].y = (INT)PyLong_AsLong(PyTuple_GET_ITEM(item, 1));
    }
    HRGN res = CreatePolygonRgn(ppm, nSize, iMode);

    if (res == NULL) {
        PyErr_SetFromWindowsErr(87);
        return NULL;
    }

    return Py_BuildValue("l", res);
}
static PyObject* PyEqualRgn(PyObject* self, PyObject* args) {
    LONG rgn1, rgn2;

    if (!PyArg_ParseTuple(args, "ll", &rgn1, &rgn2)) return NULL;
    BOOL retval = EqualRgn(rgn1, rgn2);

    return Py_BuildValue("O", PyBool_FromLong((LONG)retval));
}
static PyObject* PyCreateRoundRectRgn(PyObject* self, PyObject* args) {
    INT x1, y1, x2, y2, w, h;

    if (!PyArg_ParseTuple(args, "iiiiii", &x1, &y1, &x2, &y2, &w, &h)) return NULL;
    HRGN retval = CreateRoundRectRgn(x1, y1, x2, y2, w, h);

    if (retval == NULL) {
        PyErr_SetFromWindowsErr(87);
        return NULL;
    }
    return Py_BuildValue("l", retval);
}
static PyObject* PyGdiGetBatchLimit(PyObject* self, PyObject* args) {
    return Py_BuildValue("k", GdiGetBatchLimit());
}
static PyObject* PyGdiSetBatchLimit(PyObject* self, PyObject* args) {
    DWORD val;

    if (!PyArg_ParseTuple(args, "k", &val)) return NULL;
    HRGN retval = GdiSetBatchLimit(val);

    if (retval == NULL) {
        PyErr_SetFromWindowsErr(87);
        return NULL;
    }
    return Py_BuildValue("l", retval);
}
static PyObject* PyGetArcDirection(PyObject* self, PyObject* args) {
    LONG wnd;
    if (!PyArg_ParseTuple(args, "l", &wnd)) return NULL;
    return Py_BuildValue("i", GetArcDirection(wnd));
}
static PyObject* PyGetWorldTransform(PyObject* self, PyObject* args) {
    LONG wnd;
    if (!PyArg_ParseTuple(args, "l", &wnd)) return NULL;
    XFORM retval;
    BOOL res = GetWorldTransform(wnd, &retval);
    if (!res) {
        PyErr_SetFromWindowsErr(87);
        return NULL;
    }
    return Py_BuildValue("(ffffff)", retval.eDx, retval.eDy, retval.eM11, retval.eM12, retval.eM21, retval.eM22);
}
static PyObject* PyUpdateColors(PyObject* self, PyObject* args) {
    LONG wnd;
    if (!PyArg_ParseTuple(args, "l", &wnd)) return NULL;
    BOOL res = UpdateColors(wnd);
    if (!res) {
        PyErr_SetFromWindowsErr(87);
        return NULL;
    }
    return Py_BuildValue("O", PyBool_FromLong(res));
}
static PyObject* PyUnrealizeObject(PyObject* self, PyObject* args) {
    LONG obj;
    if (!PyArg_ParseTuple(args, "l", &obj)) return NULL;
    BOOL res = UnrealizeObject(obj);
    if (!res) {
        PyErr_SetFromWindowsErr(87);
        return NULL;
    }
    return Py_BuildValue("O", PyBool_FromLong(res));
}
static PyObject* PyTextOutA(PyObject* self, PyObject* args) {
    CHAR* text;
    INT x, y;
    LONG dc;

    if (!PyArg_ParseTuple(args, "liis", &dc, &x, &y, &text)) return NULL;

    INT c = strlen(text);
    BOOL res = TextOutA(dc, x, y, text, c);
    if (!res) {
        PyErr_SetFromWindowsErr(87);
        return NULL;
    }
    return Py_BuildValue("O", PyBool_FromLong(res));
}
static PyObject* PyTextOutW(PyObject* self, PyObject* args) {
    WCHAR* text;
    INT x, y;
    LONG dc;

    if (!PyArg_ParseTuple(args, "liiu", &dc, &x, &y, &text)) return NULL;

    INT c = wcslen(text);
    BOOL res = TextOutW(dc, x, y, text, c);
    if (!res) {
        PyErr_SetFromWindowsErr(87);
        return NULL;
    }
    return Py_BuildValue("O", PyBool_FromLong(res));
}
static PyObject* PyGetTextAlign(PyObject* self, PyObject* args) {
    LONG wnd;
    if (!PyArg_ParseTuple(args, "l", &wnd)) return NULL;
    UINT res = GetTextAlign(wnd);
    if (res == GDI_ERROR) {
        PyErr_SetFromWindowsErr(87);
        return NULL;
    }
    return Py_BuildValue("I", res);
}
static PyObject* PySetTextAlign(PyObject* self, PyObject* args) {
    LONG wnd;
    UINT val;
    if (!PyArg_ParseTuple(args, "lI", &wnd, &val)) return NULL;
    UINT res = SetTextAlign(wnd, val);
    if (res == GDI_ERROR) {
        PyErr_SetFromWindowsErr(87);
        return NULL;
    }
    return Py_BuildValue("I", res);
}
static PyObject* PyGetClientRect(PyObject* self, PyObject* args) {
    LONG wnd;
    if (!PyArg_ParseTuple(args, "l", &wnd)) return NULL;
    RECT rect;
    BOOL res = GetClientRect(wnd, &rect);
    if (!res) {
        PyErr_SetFromWindowsErr(87);
        return NULL;
    }
    return Py_BuildValue("(iiii)", rect.top, rect.left, rect.right, rect.bottom);
}
static PyObject* PyBeginPath(PyObject* self, PyObject* args) {
    LONG wnd;
    if (!PyArg_ParseTuple(args, "l", &wnd)) return NULL;
    BOOL res = BeginPath(wnd);
    if (!res) {
        PyErr_SetFromWindowsErr(87);
        return NULL;
    }
    return Py_BuildValue("O", PyBool_FromLong(res));
}
static PyObject* PyEndPath(PyObject* self, PyObject* args) {
    LONG wnd;
    if (!PyArg_ParseTuple(args, "l", &wnd)) return NULL;
    BOOL res = EndPath(wnd);
    if (!res) {
        PyErr_SetFromWindowsErr(87);
        return NULL;
    }
    return Py_BuildValue("O", PyBool_FromLong(res));
}
static PyObject* PySelectClipPath(PyObject* self, PyObject* args) {
    LONG wnd;
    INT mode;
    if (!PyArg_ParseTuple(args, "li", &wnd, &mode)) return NULL;
    BOOL res = SelectClipPath(wnd, mode);
    if (!res) {
        PyErr_SetFromWindowsErr(87);
        return NULL;
    }
    return Py_BuildValue("O", PyBool_FromLong(res));
}
static PyObject* PyPathToRegion(PyObject* self, PyObject* args) {
    LONG wnd;
    if (!PyArg_ParseTuple(args, "l", &wnd)) return NULL;
    HRGN res = PathToRegion(wnd);
    if (!res) {
        PyErr_SetFromWindowsErr(87);
        return NULL;
    }
    return Py_BuildValue("l", res);
}
static PyObject* PyWindowFromDC(PyObject* self, PyObject* args) {
    LONG wnd;
    if (!PyArg_ParseTuple(args, "l", &wnd)) return NULL;
    HWND res = WindowFromDC(wnd);
    if (!res) {
        PyErr_SetFromWindowsErr(87);
        return NULL;
    }
    return Py_BuildValue("l", res);
}
static PyObject* PyFillRect(PyObject* self, PyObject* args) {
    LONG wnd;
    LONG brush;
    INT x, y, dx, dy;
    if (!PyArg_ParseTuple(args, "l(iiii)l", &wnd, &x, &y, &dx, &dy, &brush)) return NULL;
    RECT r = { x, y, dx, dy };
    BOOL res = FillRect(wnd, &r, brush);
    if (!res) {
        PyErr_SetFromWindowsErr(87);
        return NULL;
    }
    return Py_BuildValue("O", PyBool_FromLong(res));
}
static PyObject* PySetBkMode(PyObject* self, PyObject* args) {
    LONG wnd;
    INT mode;
    if (!PyArg_ParseTuple(args, "li", &wnd, &mode)) return NULL;
    BOOL res = SetBkMode(wnd, mode);
    if (!res) {
        PyErr_SetFromWindowsErr(87);
        return NULL;
    }
    return Py_BuildValue("O", PyBool_FromLong(res));
}
static PyObject* PyGetBkMode(PyObject* self, PyObject* args) {
    LONG wnd;
    if (!PyArg_ParseTuple(args, "l", &wnd)) return NULL;
    INT res = GetBkMode(wnd);
    if (!res) {
        PyErr_SetFromWindowsErr(87);
        return NULL;
    }
    return Py_BuildValue("i", res);
}
static PyObject* PySetBkColor(PyObject* self, PyObject* args) {
    LONG wnd;
    LONG colr;
    if (!PyArg_ParseTuple(args, "ll", &wnd, &colr)) return NULL;
    BOOL res = SetBkColor(wnd, colr);
    if (!res) {
        PyErr_SetFromWindowsErr(87);
        return NULL;
    }
    return Py_BuildValue("O", PyBool_FromLong(res));
}
static PyObject* PyGetBkColor(PyObject* self, PyObject* args) {
    LONG wnd;
    if (!PyArg_ParseTuple(args, "l", &wnd)) return NULL;
    LONG res = GetBkColor(wnd);
    if (res == CLR_INVALID) {
        PyErr_SetFromWindowsErr(87);
        return NULL;
    }
    return Py_BuildValue("l", res);
}
static PyObject* PySetTextColor(PyObject* self, PyObject* args) {
    LONG wnd;
    LONG clr;
    if (!PyArg_ParseTuple(args, "ll", &wnd, &clr)) return NULL;
    LONG res = SetTextColor(wnd, clr);
    if (res == CLR_INVALID) {
        PyErr_SetFromWindowsErr(87);
        return NULL;
    }
    return Py_BuildValue("l", res);
}
static PyObject* PyGetTextColor(PyObject* self, PyObject* args) {
    LONG wnd;
    if (!PyArg_ParseTuple(args, "l", &wnd)) return NULL;
    LONG res = GetTextColor(wnd);
    if (res == CLR_INVALID) {
        PyErr_SetFromWindowsErr(87);
        return NULL;
    }
    return Py_BuildValue("l", res);
}
static PyObject* PySetTextJustification(PyObject* self, PyObject* args) {
    LONG wnd;
    INT a, b;
    if (!PyArg_ParseTuple(args, "lii", &wnd, &a, &b)) return NULL;
    BOOL res = SetTextJustification(wnd, a, b);
    if (!res) {
        PyErr_SetFromWindowsErr(87);
        return NULL;
    }
    return Py_BuildValue("O", PyBool_FromLong(res));
}
static PyObject* PySetTextCharacterExtra(PyObject* self, PyObject* args) {
    LONG wnd;
    INT a;
    if (!PyArg_ParseTuple(args, "li", &wnd, &a)) return NULL;
    INT res = SetTextCharacterExtra(wnd, a);
    if (res == 0x80000000) {
        PyErr_SetFromWindowsErr(87);
        return NULL;
    }
    return Py_BuildValue("i", res);
}
static PyObject* PySetROP2(PyObject* self, PyObject* args) {
    LONG wnd;
    INT a;
    if (!PyArg_ParseTuple(args, "li", &wnd, &a)) return NULL;
    INT res = SetROP2(wnd, a);
    if (!res) {
        PyErr_SetFromWindowsErr(87);
        return NULL;
    }
    return Py_BuildValue("i", res);
}
static PyObject* PySetPixelV(PyObject* self, PyObject* args) {
    INT x, y;
    LONG hdcDst, color;

    if (!PyArg_ParseTuple(args, "liil", &hdcDst, &x, &y, &color)) return NULL;
    LONG val = SetPixelV((HANDLE)hdcDst, x, y, color);
    if (val == -1) { PyErr_SetFromWindowsErr(87); return NULL; }
    return Py_BuildValue("l", val);
}
static PyObject* PySetPolyFillMode(PyObject* self, PyObject* args) {
    LONG wnd;
    INT a;
    if (!PyArg_ParseTuple(args, "li", &wnd, &a)) return NULL;
    INT res = SetPolyFillMode(wnd, a);
    if (!res) {
        PyErr_SetFromWindowsErr(87);
        return NULL;
    }
    return Py_BuildValue("i", res);
}
static PyObject* PySelectPalette(PyObject* self, PyObject* args) {
    LONG wnd;
    INT a;
    PyObject* bForceBkgd = Py_True;
    BOOL b;
    if (!PyArg_ParseTuple(args, "li|O", &wnd, &a, &bForceBkgd)) return NULL;
    if (!PyBool_Check(bForceBkgd)) {
        PyErr_SetString(PyExc_TypeError, "argument 3: excepted boolean");
        return NULL;
    }
    b = PyObject_IsTrue(bForceBkgd);
    INT res = SelectPalette(wnd, a, b);
    if (!res) {
        PyErr_SetFromWindowsErr(87);
        return NULL;
    }
    return Py_BuildValue("i", res);
}
static PyObject* PyRoundRect(PyObject* self, PyObject* args) {
    LONG wnd;
    INT x, y, dx, dy, w, h;
    if (!PyArg_ParseTuple(args, "liiiiii", &wnd, &x, &y, &dx, &dy, &w, &h)) return NULL;
    if (!RoundRect(wnd, x, y, dx, dy, w, h)) {
        PyErr_SetFromWindowsErr(87);
        return NULL;
    }
    return Py_BuildValue("O", PyBool_FromLong(TRUE));
}
static PyObject* PyCreatePen(PyObject* self, PyObject* args) {
    LONG color;
    INT a, b;
    if (!PyArg_ParseTuple(args, "iil", &a, &b, &color)) return NULL;
    HPEN ben = CreatePen(a, b, color);
    if (ben == NULL) {
        PyErr_SetFromWindowsErr(87);
        return NULL;
    }
    return Py_BuildValue("l", ben);
}
static PyObject* PyCreatePenIndirect(PyObject* self, PyObject* args) {
    LOGPEN lp;
    COLORREF clr;
    PyObject* pyPnt;
    UINT style;

    if (!PyArg_ParseTuple(args, "(IOk)", &style, &pyPnt, &clr)) return NULL;

    if ( (!PyTuple_Check(pyPnt)) || (PyTuple_GET_SIZE(pyPnt) != 2) ) {
        PyErr_SetString(PyExc_TypeError, "argument 2: tuple of two ints excepted");
        return NULL;
    }
    LONG x = PyLong_AsLong(PyTuple_GET_ITEM(pyPnt, 0));
    LONG y = PyLong_AsLong(PyTuple_GET_ITEM(pyPnt, 1));

    POINT point;
    lp.lopnColor = clr;
    lp.lopnStyle = style;

    point.x = x;
    point.y = y;

    lp.lopnWidth = point;

    HPEN ben = CreatePenIndirect(&lp);

    if (ben == NULL) {
        PyErr_SetFromWindowsErr(87);
        return NULL;
    }
    return Py_BuildValue("l", ben);
}
static PyObject* PyExtCreatePen(PyObject* self, PyObject* args) {
    DWORD style, width;
    PyObject* pyLb;
    PyObject* pstyle;
    DWORD pstyleR[16];
    LOGBRUSH lb;
    BOOL isPstyleNULL = FALSE;
    INT len = 0;

    if (!PyArg_ParseTuple(args, "kkOO", &style, &width, &pyLb, &pstyle)) return NULL;

    if ((!PyTuple_Check(pyLb)) || (PyTuple_GET_SIZE(pyLb) != 3)) {
        PyErr_SetString(PyExc_TypeError, "argument 3: tuple of 3 ints excepted (style, color, hatch)");
        return NULL;
    }

    lb.lbStyle = PyLong_AsLong(PyTuple_GET_ITEM(pyLb, 0));
    lb.lbColor = PyLong_AsLong(PyTuple_GET_ITEM(pyLb, 1));
    lb.lbHatch = PyLong_AsLong(PyTuple_GET_ITEM(pyLb, 2));

    if (pstyle == Py_None) {
        isPstyleNULL = TRUE;
    } else {
        len = PyTuple_GET_SIZE(pstyle);
        if (len > 16) {
            PyErr_SetString(PyExc_TypeError, "argument 4: tuple of ints with maximum length 16 excepted");
            return NULL;
        }
        for (INT i = 0; i < len; i++) {
            PyObject* item = PyTuple_GetItem(pstyle, i);
            if (!PyLong_Check(item)) {
                PyErr_SetString(PyExc_TypeError, "argument 4: tuple of ints excepted");
                return NULL;
            }
            pstyleR[i] = PyLong_AsLong(item);
        }
    }

    HPEN pen = ExtCreatePen(style, width, &lb, len, isPstyleNULL ? NULL : pstyleR);

    if ( !pen ) {
        PyErr_SetFromWindowsErr(87);
        return NULL;
    }

    return Py_BuildValue("l", pen);
}
static PyObject* PyFillRgn(PyObject* self, PyObject* args) {
    LONG hdc, hrgn, brush;
    if (!PyArg_ParseTuple(args, "lll", &hdc, &hrgn, &brush)) {
        return NULL;
    }
    if (!FillRgn(hdc, hrgn, brush)) {
        PyErr_SetFromWindowsErr(87);
        return NULL;
    }
    return Py_BuildValue("O", PyBool_FromLong(TRUE));
}
static PyObject* PyFlattenPath(PyObject* self, PyObject* args) {
    LONG hdc;
    if (!PyArg_ParseTuple(args, "l", &hdc)) {
        return NULL;
    }
    if (!FlattenPath(hdc)) {
        PyErr_SetFromWindowsErr(87);
        return NULL;
    }
    return Py_BuildValue("O", PyBool_FromLong(TRUE));
}
static PyObject* PyFrameRgn(PyObject* self, PyObject* args) {
    LONG hdc, hrgn, brush;
    INT w, h;
    if (!PyArg_ParseTuple(args, "lllii", &hdc, &hrgn, &brush, &w, &h)) {
        return NULL;
    }
    if (!FrameRgn(hdc, hrgn, brush, w, h)) {
        PyErr_SetFromWindowsErr(87);
        return NULL;
    }
    return Py_BuildValue("O", PyBool_FromLong(TRUE));
}
static PyObject* PyInvertRgn(PyObject* self, PyObject* args) {
    LONG hdc, hrgn;
    if (!PyArg_ParseTuple(args, "ll", &hdc, &hrgn)) {
        return NULL;
    }
    if (!InvertRgn(hdc, hrgn)) {
        PyErr_SetFromWindowsErr(87);
        return NULL;
    }
    return Py_BuildValue("O", PyBool_FromLong(TRUE));
}
static PyObject* PyLineTo(PyObject* self, PyObject* args) {
    LONG hdc;
    INT x, y;
    if (!PyArg_ParseTuple(args, "lii", &hdc, &x, &y)) {
        return NULL;
    }
    if (!LineTo(hdc, x, y)) {
        PyErr_SetFromWindowsErr(87);
        return NULL;
    }
    return Py_BuildValue("O", PyBool_FromLong(TRUE));
}
static PyObject* PyPaintRgn(PyObject* self, PyObject* args) {
    LONG hdc, hrgn;
    if (!PyArg_ParseTuple(args, "ll", &hdc, &hrgn)) {
        return NULL;
    }
    if (!PaintRgn(hdc, hrgn)) {
        PyErr_SetFromWindowsErr(87);
        return NULL;
    }
    return Py_BuildValue("O", PyBool_FromLong(TRUE));
}
static PyObject* PyPlgBlt(PyObject* self, PyObject* args) {
    LONG hdcDst, hdcSrc;
    PyObject* pointTuple;
    PyObject* hBitmap;
    POINT ppm[3];
    HBITMAP bmp;
    INT x, y, w, h, dx, dy;
    if (!PyArg_ParseTuple(args, "lOliiiiOii", &hdcDst, &pointTuple, &hdcSrc, &x, &y, &w, &h, &hBitmap, &dx, &dy)) return NULL;

    if (hBitmap == Py_None) {
        bmp = NULL;
    } else {
        bmp = PyLong_AsLong(hBitmap);
    }
    if ((!PyTuple_Check(pointTuple)) || (PyTuple_GET_SIZE(pointTuple) != 3)) {
        PyErr_SetString(PyExc_TypeError, "argument 2: excepted tuple of 3 points (x, y)");
        return NULL;
    }
    for (INT i = 0; i < 3; i++) {
        PyObject* item = PyTuple_GET_ITEM(pointTuple, i);
        if ((!PyTuple_Check(item)) || (PyTuple_GET_SIZE(item) != 2)) {
            PyErr_SetString(PyExc_TypeError, "argument 2: excepted tuple of 3 points (x, y)");
            return NULL;
        }
        ppm[i].x = PyLong_AsLong(PyTuple_GET_ITEM(item, 0));
        ppm[i].y = PyLong_AsLong(PyTuple_GET_ITEM(item, 1));
    }
    BOOL bSuccess = PlgBlt(hdcDst, ppm, hdcSrc, x, y, w, h, bmp, dx, dy);
    if (!bSuccess) {
        PyErr_SetFromWindowsErr(87);
        return NULL;
    }
    return Py_BuildValue("O", PyBool_FromLong(bSuccess));
}
static PyObject* PyMaskBlt(PyObject* self, PyObject* args) {
    LONG hdcDest;
    int xDest, yDest, width, height;
    LONG hdcSrc;
    int xSrc, ySrc;
    LONG hbmMask;
    int xMask, yMask;
    DWORD rop;
    if (!PyArg_ParseTuple(args, "liiiiliiliik", &hdcDest, &xDest, &yDest, &width, &height, &hdcSrc,
                                                &xSrc, &ySrc, &hbmMask, &xMask, &yMask, &rop)) return NULL;
    BOOL bSuccess = MaskBlt(hdcDest, xDest, yDest, width, height, hdcSrc, xSrc, ySrc, hbmMask, xMask, yMask, rop);
    if (!bSuccess) {
        PyErr_SetFromWindowsErr(87);
        return NULL;
    }
    return Py_BuildValue("O", PyBool_FromLong(bSuccess));

}
PyObject* PyCreateHatchBrush(PyObject* self, PyObject* args) {
    int hatch;
    LONG color;
    if (!PyArg_ParseTuple(args, "il", &hatch, &color)) return NULL;
    HBRUSH hatchBrush = CreateHatchBrush(hatch, color);
    if (hatchBrush == NULL) {
        PyErr_SetFromWindowsErr(87);
        return NULL;
    }
    return Py_BuildValue("l", hatchBrush);
}
BOOL WINAPI PyEnumChildWindowsProcHandler(HWND hwnd, LPARAM lparam) {
    LPPYENUMCHILDWINDOWPARAMS pParams = (LPPYENUMCHILDWINDOWPARAMS)lparam;
    if (PyObject_CallFunction(pParams->func, "lO", (LONG)hwnd, pParams->lParam) == NULL) {
        return FALSE;
    }

    return TRUE;
}
PyObject* PyEnumChildWindows(PyObject* self, PyObject* args) {
    PyObject* func, * hwnd;
    PyObject* lParam = Py_None;
    HWND aHwnd;

    if (!PyArg_ParseTuple(args, "OO|O", &hwnd, &func, &lParam)) return NULL;
    if (!PyCallable_Check(func)) {
        PyErr_SetString(PyExc_TypeError, "argument 2: must be a callable function");
        return NULL;
    }

    if (hwnd == Py_None) {
        aHwnd = NULL;
    } else {
        aHwnd = PyLong_AsLong(hwnd);
    }

    // allocate memory in heap to prevent corruption when larger data is transfered
    HANDLE hHeap = GetProcessHeap();
    LPPYENUMCHILDWINDOWPARAMS params = (LPPYENUMCHILDWINDOWPARAMS)HeapAlloc(hHeap, 0, sizeof(PYENUMCHILDWINDOWPARAMS));

    params->func = func;
    params->lParam = lParam;

    EnumChildWindows(aHwnd, PyEnumChildWindowsProcHandler, (LPARAM)params);

    HeapFree(hHeap, 0, params);
    return Py_None;
}
static PyObject* PyGetWindowTextW(PyObject* self, PyObject* args) {
    LONG hwnd;
    if (!PyArg_ParseTuple(args, "l", &hwnd)) return NULL;

    INT len = GetWindowTextLengthW(hwnd) + 1;
    if (RaiseExceptionCheck(len - 1)) return NULL;

    HANDLE hHeap = GetProcessHeap();
    WCHAR out[256];

    BOOL res = GetWindowTextW(hwnd, out, len);

    if (RaiseExceptionCheck(res)) {
        return NULL;
    }
    return Py_BuildValue("u", out);
}
static PyObject* PyGetWindowTextA(PyObject* self, PyObject* args) {
    LONG hwnd;
    if (!PyArg_ParseTuple(args, "l", &hwnd)) return NULL;

    INT len = GetWindowTextLengthA(hwnd) + 1;
    if (RaiseExceptionCheck(len - 1)) return NULL;

    HANDLE hHeap = GetProcessHeap();
    CHAR out[256];

    BOOL res = GetWindowTextA(hwnd, out, len);

    if (RaiseExceptionCheck(res)) {
        return NULL;
    }
    return Py_BuildValue("s", out);
}
static PyObject* PyGetWindowTextLengthA(PyObject* self, PyObject* args) {
    LONG hwnd;
    if (!PyArg_ParseTuple(args, "l", &hwnd)) return NULL;

    INT res = GetWindowTextLengthA(hwnd);

    if (RaiseExceptionCheck(res)) {
        return NULL;
    }
    return Py_BuildValue("i", res);
}
static PyObject* PyGetWindowTextLengthW(PyObject* self, PyObject* args) {
    LONG hwnd;
    if (!PyArg_ParseTuple(args, "l", &hwnd)) return NULL;

    INT res = GetWindowTextLengthW(hwnd);

    if (RaiseExceptionCheck(res)) {
        return NULL;
    }
    return Py_BuildValue("i", res);
}
static PyObject* PyGetWindowRect(PyObject* self, PyObject* args) {
    LONG hwnd;
    if (!PyArg_ParseTuple(args, "l", &hwnd)) return NULL;
    RECT rect;

    BOOL res = GetWindowRect(hwnd, &rect);

    if (RaiseExceptionCheck(res)) {
        return NULL;
    }
    return Py_BuildValue("(iiii)", rect.left, rect.top, rect.right, rect.bottom);
}
static PyObject* PyGetWindowPlacement(PyObject* self, PyObject* args) {
    LONG hwnd;
    if (!PyArg_ParseTuple(args, "l", &hwnd)) return NULL;
    WINDOWPLACEMENT wp;
    wp.length = sizeof(WINDOWPLACEMENT);
    if (RaiseExceptionCheck(GetWindowPlacement(hwnd, &wp))) return NULL;
    return Py_BuildValue("(IIOOO)", wp.flags, wp.showCmd,
        Py_BuildValue("(ii)", wp.ptMinPosition.x, wp.ptMinPosition.y),
        Py_BuildValue("(ii)", wp.ptMaxPosition.x, wp.ptMaxPosition.y),
        Py_BuildValue("(iiii)", wp.rcNormalPosition.left, wp.rcNormalPosition.top,
                                wp.rcNormalPosition.right, wp.rcNormalPosition.bottom)
    );
}
static PyObject* PyWindowFromPoint(PyObject* self, PyObject* args) {
    INT x, y;
    if (!PyArg_ParseTuple(args, "(ii)", &x, &y)) return NULL;

    POINT p;
    p.x = x;
    p.y = y;
    HWND res = WindowFromPoint(p);
    if (res == NULL) {
        return Py_None;
    }
    return Py_BuildValue("l", (LONG)res);
}
static PyObject* PyWindowFromPhysicalPoint(PyObject* self, PyObject* args) {
    INT x, y;
    if (!PyArg_ParseTuple(args, "(ii)", &x, &y)) return NULL;

    POINT p;
    p.x = x;
    p.y = y;
    HWND res = WindowFromPhysicalPoint(p);
    if (res == NULL) {
        return Py_None;
    }
    return Py_BuildValue("l", (LONG)res);
}
static PyObject* PySwitchToThisWindow(PyObject* self, PyObject* args) {
    LONG wnd;
    PyObject* bUnk;
    if (!PyArg_ParseTuple(args, "lO", &wnd, &bUnk)) return NULL;
    if (!PyBool_Check(bUnk)) {
        PyErr_SetString(PyExc_TypeError, "argument 2: excepted bool");
        return NULL;
    }
    SwitchToThisWindow(wnd, PyObject_IsTrue(bUnk));
    return Py_None;
}
static PyObject* PyShowWindow(PyObject* self, PyObject* args) {
    LONG wnd;
    INT nShow;
    if (!PyArg_ParseTuple(args, "li", &wnd, &nShow)) return NULL;

    return PyBool_FromLong(ShowWindow(wnd, nShow));
}
static PyObject* PyReleaseDC(PyObject* self, PyObject* args) {
    LONG wnd, hdc;
    if (!PyArg_ParseTuple(args, "ll", &wnd, &hdc)) return NULL;
    return PyBool_FromLong(ReleaseDC(wnd, hdc));
}
static PyObject* PyGetCursorPos(PyObject* self, PyObject* args) {
    POINT p;
    if (RaiseExceptionCheck(GetCursorPos(&p))) return NULL;
    return Py_BuildValue("(ii)", p.x, p.y);
}
static PyObject* PySetCursorPos(PyObject* self, PyObject* args) {
    INT x, y;
    if (!PyArg_ParseTuple(args, "ii", &x, &y)) return NULL;
    if (RaiseExceptionCheck(SetCursorPos(x, y))) return NULL;
    return Py_True;
}
static PyObject* PyLoadIconA(PyObject* self, PyObject* args) {
    PyObject* hinstance;
    INT icon;
    HINSTANCE hinst = NULL;
    if (!PyArg_ParseTuple(args, "Oi", &hinstance, &icon)) return NULL;
    if (hinstance != Py_None) hinst = PyLong_AsLong(hinstance);

    HICON iconic = LoadIconA(hinst, MAKEINTRESOURCEA(icon));
    if (RaiseExceptionCheck(iconic == NULL)) return NULL;
    return Py_BuildValue("l", iconic);
}
static PyObject* PyLoadIconW(PyObject* self, PyObject* args) {
    PyObject* hinstance;
    INT icon;
    HINSTANCE hinst = NULL;
    if (!PyArg_ParseTuple(args, "Oi", &hinstance, &icon)) return NULL;
    if (hinstance != Py_None) hinst = PyLong_AsLong(hinstance);

    HICON iconic = LoadIconW(hinst, MAKEINTRESOURCEW(icon));
    if (RaiseExceptionCheck(iconic == NULL)) return NULL;
    return Py_BuildValue("l", iconic);
}
static PyObject* PyDrawIcon(PyObject* self, PyObject* args) {
    LONG hdc, icon;
    INT x, y;
    if (!PyArg_ParseTuple(args, "liil", &hdc, &x, &y, &icon)) return NULL;
    if (RaiseExceptionCheck(DrawIcon(hdc, x, y, icon))) return NULL;
    return Py_True;
}
static PyObject* PyDrawIconEx(PyObject* self, PyObject* args) {
    LONG hdc, icon;
    PyObject* obBrush;
    HBRUSH brush = NULL;
    INT x, y, w, h;
    UINT istepIfAniCur, diFlags;
    if (!PyArg_ParseTuple(args, "liiliiIOI", &hdc, &x, &y, &icon, &w, &h, &istepIfAniCur, &obBrush, &diFlags)) return NULL;
    if (obBrush != Py_None)
        brush = PyLong_AsLong(obBrush);
    BOOL res = DrawIconEx(hdc, x, y, icon, w, h, istepIfAniCur, brush, diFlags);
    if (RaiseExceptionCheck(res)) return NULL;
    return Py_True;
}
static PyObject* PyLoadImageA(PyObject* self, PyObject* args) {
    PyObject* obHinstance;
    LONG ico;
    UINT type;
    INT cx, cy;
    UINT fuLoad;
    HINSTANCE inst = NULL;

    if (!PyArg_ParseTuple(args, "OlIiiI", &obHinstance, &ico, &type, &cx, &cy, &fuLoad)) return NULL;

    if (obHinstance != Py_None)
        inst = PyLong_AsLong(obHinstance);

    HANDLE res = LoadImageA(inst, MAKEINTRESOURCEA(ico), type, cx, cy, fuLoad);
    
    if (RaiseExceptionCheck(res == NULL)) return NULL;

    return Py_BuildValue("l", (LONG)res);
}
static PyObject* PyLoadImageW(PyObject* self, PyObject* args) {
    PyObject* obHinstance;
    LONG ico;
    UINT type;
    INT cx, cy;
    UINT fuLoad;
    HINSTANCE inst = NULL;

    if (!PyArg_ParseTuple(args, "OlIiiI", &obHinstance, &ico, &type, &cx, &cy, &fuLoad)) return NULL;

    if (obHinstance != Py_None)
        inst = PyLong_AsLong(obHinstance);

    HANDLE res = LoadImageW(inst, MAKEINTRESOURCEW(ico), type, cx, cy, fuLoad);

    if (RaiseExceptionCheck(res == NULL)) return NULL;

    return Py_BuildValue("l", (LONG)res);
}
static PyObject* PyLoadCursorFromFileA(PyObject* self, PyObject* args) {
    LPCSTR str;
    if (!PyArg_ParseTuple(args, "s", &str)) return NULL;
    HCURSOR cur = LoadCursorFromFileA(str);
    if (RaiseExceptionCheck(cur == NULL)) return NULL;
    return Py_BuildValue("l", cur);
}
static PyObject* PyLoadCursorFromFileW(PyObject* self, PyObject* args) {
    LPCWSTR str;
    if (!PyArg_ParseTuple(args, "u", &str)) return NULL;
    HCURSOR cur = LoadCursorFromFileW(str);
    if (RaiseExceptionCheck(cur == NULL)) return NULL;
    return Py_BuildValue("l", cur);
}
static PyObject* PyLoadCursorA(PyObject* self, PyObject* args) {
    PyObject* obHinst;
    HINSTANCE hinstance = NULL;
    LONG cursor;

    if (!PyArg_ParseTuple(args, "Ol", &obHinst, &cursor)) return NULL;

    if (obHinst != Py_None)
        hinstance = PyLong_AsLong(obHinst);

    HCURSOR cur = LoadCursorA(hinstance, MAKEINTRESOURCEA(cursor));
    if (RaiseExceptionCheck(cur == NULL)) return NULL;
    return Py_BuildValue("l", cur);
}
static PyObject* PyLoadCursorW(PyObject* self, PyObject* args) {
    PyObject* obHinst;
    HINSTANCE hinstance = NULL;
    LONG cursor;

    if (!PyArg_ParseTuple(args, "Ol", &obHinst, &cursor)) return NULL;

    if (obHinst != Py_None)
        hinstance = PyLong_AsLong(obHinst);

    HCURSOR cur = LoadCursorW(hinstance, MAKEINTRESOURCEW(cursor));
    if (RaiseExceptionCheck(cur == NULL)) return NULL;
    return Py_BuildValue("l", cur);
}
static PyObject* PyGetCursorInfo(PyObject* self, PyObject* args) {
    CURSORINFO leFisheAuChocolat;
    leFisheAuChocolat.cbSize = sizeof(CURSORINFO);
    GetCursorInfo(&leFisheAuChocolat);
    return Py_BuildValue("(klO)", leFisheAuChocolat.flags, leFisheAuChocolat.hCursor,
            Py_BuildValue("(ii)", leFisheAuChocolat.ptScreenPos.x, leFisheAuChocolat.ptScreenPos.y)
        );
}
static PyObject* PyMessageBoxA(PyObject* self, PyObject* args) {
    CHAR* title, * description;
    UINT uType;
    PyObject* obHwnd;

    if (!PyArg_ParseTuple(args, "OssI", &obHwnd, &description, &title, &uType)) return NULL;

    HWND hwnd = obHwnd == Py_None ? NULL : PyLong_AsLong(obHwnd);
    INT res = MessageBoxA(hwnd, title, description, uType);
    if (RaiseExceptionCheck(res)) return NULL;
    return Py_BuildValue("i", res);
}
static PyObject* PyMessageBoxW(PyObject* self, PyObject* args) {
    WCHAR* title, * description;
    UINT uType;
    PyObject* obHwnd;

    if (!PyArg_ParseTuple(args, "OuuI", &obHwnd, &description, &title, &uType)) return NULL;

    HWND hwnd = obHwnd == Py_None ? NULL : PyLong_AsLong(obHwnd);
    INT res = MessageBoxW(hwnd, title, description, uType);
    if (RaiseExceptionCheck(res)) return NULL;
    return Py_BuildValue("i", res);
}
static PyObject* PyMessageBeep(PyObject* self, PyObject* args) {
    UINT uType;

    if (!PyArg_ParseTuple(args, "I", &uType)) return NULL;

    BOOL res = MessageBeep(uType);
    if (RaiseExceptionCheck(res)) return NULL;
    return Py_True;
}
static PyObject* PyBeep(PyObject* self, PyObject* args) {
    DWORD dwFreq, dwDur;

    if (!PyArg_ParseTuple(args, "kk", &dwFreq, &dwDur)) return NULL;

    BOOL res = Beep(dwFreq, dwDur);
    if (RaiseExceptionCheck(res)) return NULL;
    return Py_True;
}
static PyObject* PyGetTopWindow(PyObject* self, PyObject* args) {
    PyObject* obWnd = Py_None;

    if (!PyArg_ParseTuple(args, "|O", &obWnd)) return NULL;

    HWND hwnd = obWnd == Py_None ? NULL : PyLong_AsLong(obWnd);

    HWND res = GetTopWindow(hwnd);
    if (RaiseExceptionCheck(res == NULL)) return NULL;
    return Py_True;
}
static PyObject* PyExitWindowsEx(PyObject* self, PyObject* args) {
    UINT flags;
    DWORD reason;
    if (!PyArg_ParseTuple(args, "Ik", &flags, &reason)) return NULL;
    BOOL res = ExitWindowsEx(flags, reason);
    if (RaiseExceptionCheck(res)) return NULL;
    return Py_True;
}
static PyObject* PyExitWindows(PyObject* self, PyObject* args) {
    BOOL res = ExitWindows(0, 0);
    if (RaiseExceptionCheck(res)) return NULL;
    return Py_True;
}
static PyObject* PyInitiateSystemShutdownA(PyObject* self, PyObject* args) {
    PyObject* computer, * message;
    DWORD timeout;
    PyObject* forceClose, * reboot;
    LPSTR computerName, messageName;
    BOOL forceName, rebootName;

    if (!PyArg_ParseTuple(args, "OOkOO", &computer, &message, &timeout, &forceClose, &reboot)) return NULL;
    if (computer == Py_None) {
        computerName = NULL;
    } else {
        computerName = PyString_ToCharArr(computer);
    }
    if (message == Py_None) {
        messageName = NULL;
    } else {
        messageName = PyString_ToCharArr(message);
    }

    if (!PyBool_Check(forceClose)) {
        PyErr_SetString(PyExc_TypeError, "argument 4: excepted bool");
        return NULL;
    }
    if (!PyBool_Check(reboot)) {
        PyErr_SetString(PyExc_TypeError, "argument 5: excepted bool");
        return NULL;
    }
    forceName = PyObject_IsTrue(forceClose);
    rebootName = PyObject_IsTrue(reboot);

    BOOL res = InitiateSystemShutdownA(computerName, messageName, timeout, forceName, rebootName);
    if (RaiseExceptionCheck(res)) return NULL;
    return Py_True;
}
static PyObject* PyInitiateSystemShutdownW(PyObject* self, PyObject* args) {
    PyObject* computer, * message;
    DWORD timeout;
    PyObject* forceClose, * reboot;
    LPWSTR computerName, messageName;
    BOOL forceName, rebootName;

    if (!PyArg_ParseTuple(args, "OOkOO", &computer, &message, &timeout, &forceClose, &reboot)) return NULL;
    if (computer == Py_None) {
        computerName = NULL;
    }
    else {
        computerName = PyUnicode_AsWideCharString(computer, 1024);
    }
    if (message == Py_None) {
        messageName = NULL;
    }
    else {
        messageName = PyUnicode_AsWideCharString(message, 1024);
    }

    if (!PyBool_Check(forceClose)) {
        PyErr_SetString(PyExc_TypeError, "argument 4: excepted bool");
        return NULL;
    }
    if (!PyBool_Check(reboot)) {
        PyErr_SetString(PyExc_TypeError, "argument 5: excepted bool");
        return NULL;
    }
    forceName = PyObject_IsTrue(forceClose);
    rebootName = PyObject_IsTrue(reboot);

    BOOL res = InitiateSystemShutdownW(computerName, messageName, timeout, forceName, rebootName);
    if (RaiseExceptionCheck(res)) return NULL;
    return Py_True;
}

static PyMethodDef module_methods[] = {
    {"GetDeviceCaps", PyGetDeviceCaps, METH_VARARGS},
    {"GetDCBrushColor", PyGetDCBrushColor, METH_VARARGS},
    {"GetDC", (PyCFunction)PyGetDC, METH_VARARGS},
    {"PatBlt", (PyCFunction)PyPatBlt, METH_VARARGS},
    {"GetSystemMetrics", (PyCFunction)PyGetSystemMetrics, METH_VARARGS},
    {"DeleteDC", (PyCFunction)PyDeleteDC, METH_VARARGS},
    {"BitBlt", (PyCFunction)PyBitBlt, METH_VARARGS},
    {"GetDesktopWindow", (PyCFunction)PyGetDesktopWindow,  METH_NOARGS},
    {"GetLastError", (PyCFunction)PyGetLastError, METH_NOARGS},
    {"Rectangle", (PyCFunction)PyRectangle, METH_VARARGS},
    {"Ellipse", (PyCFunction)PyEllipse, METH_VARARGS},
    {"CreateSolidBrush", (PyCFunction)PyCreateSolidBrush, METH_VARARGS},
    {"DeleteObject", (PyCFunction)PyDeleteObject, METH_VARARGS},
    {"RGB", (PyCFunction)PyRGB, METH_VARARGS},
    {"SelectObject", (PyCFunction)PySelectObject, METH_VARARGS},
    {"StretchBlt", (PyCFunction)PyStretchBlt, METH_VARARGS},
    {"SetStretchBltMode", (PyCFunction)PySetStretchBltMode, METH_VARARGS},
    {"GetStretchBltMode", (PyCFunction)PyGetStretchBltMode, METH_VARARGS},
    {"GetPixel", (PyCFunction)PyGetPixel, METH_VARARGS},
    {"SetPixel", (PyCFunction)PySetPixel, METH_VARARGS},
    {"SetPixelV", (PyCFunction)PySetPixelV, METH_VARARGS},
    {"GetRValue", (PyCFunction)PyGetRValue, METH_VARARGS},
    {"GetGValue", (PyCFunction)PyGetGValue, METH_VARARGS},
    {"GetBValue", (PyCFunction)PyGetBValue, METH_VARARGS},
    { "GetRGBValues", (PyCFunction)PyGetRGBValues, METH_VARARGS },
    { "GetDCPenColor", PyGetDCPenColor, METH_VARARGS },
    { "GetDCOrgEx", PyGetDCOrgEx, METH_VARARGS },
    { "GetObjectType", PyGetObjectType, METH_VARARGS },
    { "Chord", PyChord, METH_VARARGS },
    { "Pie", PyPie, METH_VARARGS },
    { "rand", Pyrand, METH_NOARGS },
    { "Sleep", PySleep, METH_VARARGS },
    { "SaveDC", PySaveDC, METH_VARARGS },
    { "RestoreDC", PyRestoreDC, METH_VARARGS },
    { "CancelDC", PyCancelDC, METH_VARARGS },
    { "Arc", PyArc, METH_VARARGS },
    { "ArcTo", PyArcTo, METH_VARARGS },
    { "AngleArc", PyAngleArc, METH_VARARGS },
    { "SetArcDirection", PySetArcDirection, METH_VARARGS },
    { "GetStockObject", PyGetStockObject, METH_VARARGS },
    { "RedrawWindow", PyRedrawWindow, METH_VARARGS },
    { "InvalidateRect", PyInvalidateRect, METH_VARARGS },
    { "CreateCompatibleDC", PyCreateCompatibleDC, METH_VARARGS },
    { "CreateCompatibleBitmap", PyCreateCompatibleBitmap, METH_VARARGS },
    { "SelectClipRgn", PySelectClipRgn, METH_VARARGS },
    { "CreateEllipticRgn", PyCreateEllipticRgn, METH_VARARGS },
    { "CreateEllipticRgnIndirect", PyCreateEllipticRgnIndirect, METH_VARARGS },
    { "CreateRectRgn", PyCreateRectRgn, METH_VARARGS },
    { "CreateRectRgnIndirect", PyCreateRectRgnIndirect, METH_VARARGS },
    { "CreatePolygonRgn", PyCreatePolygonRgn, METH_VARARGS },
    { "EqualRgn", PyEqualRgn, METH_VARARGS },
    { "CreateRoundRectRgn", PyCreateRoundRectRgn, METH_VARARGS },
    { "GdiGetBatchLimit", PyGdiGetBatchLimit, METH_NOARGS },
    { "GdiSetBatchLimit", PyGdiSetBatchLimit, METH_VARARGS },
    { "GetArcDirection", PyGetArcDirection, METH_VARARGS },
    { "GetWorldTransform", PyGetWorldTransform, METH_VARARGS },
    { "UpdateColors", PyUpdateColors, METH_VARARGS },
    { "UnrealizeObject", PyUnrealizeObject, METH_VARARGS },
    { "TextOutA", PyTextOutA, METH_VARARGS },
    { "GetTextAlign", PyGetTextAlign, METH_VARARGS },
    { "SetTextAlign", PySetTextAlign, METH_VARARGS },
    { "GetClientRect", PyGetClientRect, METH_VARARGS },
    { "BeginPath", PyBeginPath, METH_VARARGS },
    { "EndPath", PyEndPath, METH_VARARGS },
    { "SelectClipPath", PySelectClipPath, METH_VARARGS },
    { "PathToRegion", PyPathToRegion, METH_VARARGS },
    { "WindowFromDC", PyWindowFromDC, METH_VARARGS },
    { "FillRect", PyFillRect, METH_VARARGS },
    { "SetBkMode", PySetBkMode, METH_VARARGS },
    { "GetBkMode", PyGetBkMode, METH_VARARGS },
    { "SetBkColor", PySetBkMode, METH_VARARGS },
    { "GetBkColor", PyGetBkMode, METH_VARARGS },
    { "SetTextColor", PySetTextColor, METH_VARARGS },
    { "GetTextColor", PyGetTextColor, METH_VARARGS },
    { "SetTextJustification", PySetTextJustification, METH_VARARGS },
    { "SetTextCharacterExtra", PySetTextCharacterExtra, METH_VARARGS },
    { "SetROP2", PySetROP2, METH_VARARGS },
    { "SetPolyFillMode", PySetPolyFillMode, METH_VARARGS },
    { "SelectPalette", PySelectPalette, METH_VARARGS },
    { "RoundRect", PyRoundRect, METH_VARARGS },
    { "CreatePen", PyCreatePen, METH_VARARGS },
    { "CreatePenIndirect", PyCreatePenIndirect, METH_VARARGS },
    { "ExtCreatePen", PyExtCreatePen, METH_VARARGS },
    { "TextOutW", PyTextOutW, METH_VARARGS },
    { "TextOut", PyTextOutW, METH_VARARGS }, // Unicode is better.... most of the time.
    { "FillRgn", PyFillRgn, METH_VARARGS },
    { "FlattenPath", PyFlattenPath, METH_VARARGS },
    { "FrameRgn", PyFrameRgn, METH_VARARGS },
    { "InvertRgn", PyInvertRgn, METH_VARARGS },
    { "LineTo", PyLineTo, METH_VARARGS },
    { "PaintRgn", PyPaintRgn, METH_VARARGS },
    { "PlgBlt", PyPlgBlt, METH_VARARGS },
    { "MaskBlt", PyMaskBlt, METH_VARARGS },
    { "CreateHatchBrush", PyCreateHatchBrush, METH_VARARGS },
    { "EnumChildWindows", PyEnumChildWindows, METH_VARARGS },
    { "GetWindowTextA", PyGetWindowTextA, METH_VARARGS },
    { "GetWindowTextW", PyGetWindowTextW, METH_VARARGS },
    { "GetWindowText",  PyGetWindowTextW, METH_VARARGS },
    { "GetWindowTextLengthA", PyGetWindowTextLengthA, METH_VARARGS },
    { "GetWindowTextLengthW", PyGetWindowTextLengthW, METH_VARARGS },
    { "GetWindowTextLength" , PyGetWindowTextLengthW, METH_VARARGS },
    { "GetWindowRect", PyGetWindowRect, METH_VARARGS },
    { "GetWindowPlacement", PyGetWindowPlacement, METH_VARARGS },
    { "WindowFromPoint", PyWindowFromPoint, METH_VARARGS },
    { "WindowFromPhysicalPoint", PyWindowFromPhysicalPoint, METH_VARARGS },
    { "SwitchToThisWindow", PySwitchToThisWindow, METH_VARARGS },
    { "ShowWindow", PyShowWindow, METH_VARARGS },
    { "ReleaseDC", PyReleaseDC, METH_VARARGS },
    { "GetCursorPos", PyGetCursorPos, METH_NOARGS },
    { "SetCursorPos", PySetCursorPos, METH_VARARGS },
    { "LoadIconA", PyLoadIconA, METH_VARARGS },
    { "DrawIcon", PyDrawIcon, METH_VARARGS },
    { "LoadIconW", PyLoadIconW, METH_VARARGS },
    { "LoadIcon",  PyLoadIconW, METH_VARARGS },
    { "DrawIconEx", PyDrawIconEx, METH_VARARGS },
    { "LoadImageA", PyLoadImageA, METH_VARARGS },
    { "LoadImageW", PyLoadImageW, METH_VARARGS },
    { "LoadImage",  PyLoadImageW, METH_VARARGS },
    { "LoadCursorFromFileA", PyLoadCursorFromFileA, METH_VARARGS },
    { "LoadCursorFromFileW", PyLoadCursorFromFileW, METH_VARARGS },
    { "LoadCursorFromFile",  PyLoadCursorFromFileW, METH_VARARGS },
    { "LoadCursorA", PyLoadCursorA, METH_VARARGS },
    { "LoadCursorW", PyLoadCursorW, METH_VARARGS },
    { "LoadCursor",  PyLoadCursorW, METH_VARARGS },
    { "GetCursorInfo", PyGetCursorInfo, METH_NOARGS },
    { "MessageBoxA", PyMessageBoxA, METH_VARARGS },
    { "MessageBoxW", PyMessageBoxW, METH_VARARGS },
    { "MessageBox",  PyMessageBoxW, METH_VARARGS },
    { "GetTopWindow", PyGetTopWindow, METH_VARARGS },
    { "MessageBeep", PyMessageBeep , METH_VARARGS },
    { "Beep", PyBeep, METH_VARARGS },
    { "ExitWindowsEx", PyExitWindowsEx , METH_VARARGS },
    { "ExitWindows", PyExitWindows, METH_NOARGS },
    { "InitiateSystemShutdownA", PyInitiateSystemShutdownA, METH_VARARGS },
    { "InitiateSystemShutdownW", PyInitiateSystemShutdownW, METH_VARARGS },
    { "InitiateSystemShutdown",  PyInitiateSystemShutdownW, METH_VARARGS },


    /* sentinel */
    { 0 }
};

static struct PyModuleDef ModuleCombinations =
{
    PyModuleDef_HEAD_INIT,
    "WinApy", /* name of module */
    NULL,
    -1,   /* size of per-interpreter state of the module, or -1 if the module keeps state in global variables. */
    module_methods
};


void PyInit_winapy(void) {
    PyModule_Create(&ModuleCombinations);
}
