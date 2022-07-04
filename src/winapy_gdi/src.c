/*
    WinAPY - Windows API wrapper in C developed for Python.
    Copyright (c) 2022 Itzsten
*/

// Mainly developed for my YouTube channel, however functions asked to be added probably will.
// Constant definitions? Meh.

#include "utils.h"

#pragma comment(lib, "gdi32.lib")

static PyObject* PyPatBlt(PyObject* self, PyObject* args) {
    //@description@ Paints a rectangle on the specified device context by mixing the current brush with existing colors according to the rop argument.@@bool
    //@args@ HDC|hdc|Handle to the device context@@int|x|Rectangles left offset@@int|y|Rectangles top offset@@int|w|Rectangles right offset@@int|h|Rectangles bottom offset@@int|rop|The raster operation code
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
    //@description@ Bit-block transfer of the color data from a rectangle of pixels from hdcSrc into hdcDst using the specified raster operation.@@bool
    //@args@ HDC|hdcDst|Handle to the destination device context@@int|x|Rectangles left offset@@int|y|Rectangles top offset@@int|cx|Rectangles right offset@@int|cy|Rectangles bottom offset@@HDC|hdcSrc|Handle to the source device context@@int|x1|Source device context's rectangles left@@int|y1|Source device context's rectangles top@@int|rop|The raster operation code
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
    //@description@ Frees up memory allocated by the specified device context and closes its handle.@@bool
    //@args@ HDC|hdc|Handle to a device context to deallocate and close.
    LONG hdc;

    if (!PyArg_ParseTuple(args, "l", &hdc)) {
        return NULL;
    }
    if (RaiseExceptionCheck(DeleteDC((HANDLE)hdc))) { return NULL; }
    return Py_BuildValue("b", TRUE);
}
static PyObject* PyGetLastError(PyObject* self, PyObject* args) { 
    //@description@ Retrieves the last error code caused by a WinAPY function.@@int
    //@args@ None
    return Py_BuildValue("i", GetLastError()); 
}
static PyObject* PyRectangle(PyObject* self, PyObject* args) {
    //@description@ Draws a rectangle using the current brush on the specified device context.@@bool
    //@args@ left|int|Left offset of rectangle@@top|int|Top offset of rectangle@@right|int|Right offset of rectangle@@bottom|int|Bottom offset of rectangle
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
    //@description@ Draws an ellipse using the current brush on the specified device context.@@bool
    //@args@ left|int|Left offset of ellipse@@top|int|Top offset of ellipse@@right|int|Right offset of ellipse@@bottom|int|Bottom offset of ellipse
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
    //@description@ Creates a new HBRUSH with the specified color.@@HBRUSH
    //@args@ color|int|The color of the brush
    LONG color;
    if (!PyArg_ParseTuple(args, "l", &color)) return NULL;
    HBRUSH brush = CreateSolidBrush(color);
    if (RaiseExceptionCheck(brush)) return NULL;
    return Py_BuildValue("l", (LONG)brush);
}
static PyObject* PyDeleteObject(PyObject* self, PyObject* args) {
    //@description@ Deletes the object and frees its memory.@@bool
    //@args@ object|HGDIOBJ|The object to delete, can be a brush, a pen etc.
    LONG object;
    if (!PyArg_ParseTuple(args, "l", &object)) return NULL;
    if (RaiseExceptionCheck(DeleteObject((HANDLE)object))) return NULL;
    return Py_BuildValue("b", TRUE);
}
static PyObject* PyRGB(PyObject* self, PyObject* args) {
    //@description@ Converts the specified RGB color to an integer passable to WinAPY functions.@@int
    //@args@ r|int|The R Value@@g|int|The G value@@b|int|The B value
    INT r, g, b;
    if (!PyArg_ParseTuple(args, "iii", &r, &g, &b)) return NULL;
    return Py_BuildValue("l", RGB(r, g, b));
}
static PyObject* PySelectObject(PyObject* self, PyObject* args) {
    //@description@ Sets the specified device context's current object of that objects type the specified object.@@int
    //@args@ object|int|Handle to the object to select
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
    //@description@ Copies a bitmap from a source rectangle into a destination rectangle, stretching or compressing the bitmap to fit the dimensions of the destination rectangle.@@bool
    //@args@ hdcDest|HDC|Handle to destinations device context@@xDest|int|Left offset of the destinations rectangle@@yDest|int|Top offset of the destinations rectangle@@wDest|int|Right offset of the destinations rectangle@@hDest|int|Bottom offset of the destinations rectangle@@hdcSrc|HDC|Handle to sources device context@@xSrc|int|Left offset of the sources rectangle@@ySrc|int|Top offset of the sources rectangle@@wSrc|int|Right offset of the sources rectangle@@hSrc|int|Bottom offset of the sources rectangle@@rop|int|The raster operation
    INT xDest, yDest, wDest, hDest, xSrc, ySrc, wSrc, hSrc;
    LONG hdcDst, hdcSrc, rasterOperation;

    if (!PyArg_ParseTuple(args, "liiiiliiiil", &hdcDst, &xDest, &yDest, &wDest, &hDest, &hdcSrc, &xSrc, &ySrc, &wSrc, &hSrc, &rasterOperation)) return NULL;
    if (RaiseExceptionCheck(StretchBlt( (HANDLE)hdcDst, xDest, yDest, wDest, hDest, (HANDLE)hdcSrc, xSrc, ySrc, wSrc, hSrc, rasterOperation ))) return NULL;
    return Py_BuildValue("O", PyBool_FromLong(TRUE));
}
static PyObject* PySetStretchBltMode(PyObject* self, PyObject* args) {
    //@description@ Sets the bitmap stretching mode in the specified device context.@@int
    //@args@ hdc|HDC|The device context to change the stretching mode of@@iMode|int|The new stretching mode
    INT nMode;
    LONG hdcDst;

    if (!PyArg_ParseTuple(args, "li", &hdcDst, &nMode)) return NULL;
    INT val = SetStretchBltMode(hdcDst, nMode);
    if (RaiseExceptionCheck(val)) return NULL;
    return Py_BuildValue("i", val);
}
static PyObject* PySetPixel(PyObject* self, PyObject* args) {
    //@description@ Sets the specified pixel on a device context.@@int
    //@args@ hdc|HDC|The device context to change the pixel of.@@x|int|The x coordinate of the pixel@@y|int|The y coordinate of the pixel@@color|int|The new color of the pixel
    INT x, y;
    LONG hdcDst, color;

    if (!PyArg_ParseTuple(args, "liil", &hdcDst, &x, &y, &color)) return NULL;
    LONG val = SetPixel((HANDLE)hdcDst, x, y, color);
    if (val == -1) { PyErr_SetFromWindowsErr(87); return NULL; }
    return Py_BuildValue("l", val);
}
static PyObject* PyGetStretchBltMode(PyObject* self, PyObject* args) {
    //@description@ Gets the current stretching mode of the specified device context.@@int
    //@args@ hdc|HDC|The device context to retrieve the stretching mode of
    LONG h;
    if (!PyArg_ParseTuple(args, "l", &h)) return NULL;
    INT res = GetStretchBltMode((HANDLE)h);
    if (RaiseExceptionCheck(res)) return NULL;
    return Py_BuildValue("i", res);
}
static PyObject* PyGetPixel(PyObject* self, PyObject* args) {
    //@description@ Gets the specified pixel's color on a device context.@@int
    //@args@ hdc|HDC|The device context to retrieve the pixel of.@@x|int|The x coordinate of the pixel@@y|int|The y coordinate of the pixel
    LONG h;
    INT x, y;
    if (!PyArg_ParseTuple(args, "lii", &h, &x, &y)) return NULL;
    LONG res = GetPixel(h, x, y);
    if (res == CLR_INVALID) { PyErr_SetFromWindowsErr(87); return NULL; }
    return Py_BuildValue("l", res);
}
static PyObject* PyGetRValue(PyObject* self, PyObject* args) {
    //@description@ Get the R value from the specified color.@@int
    //@args@ color|int|Color to retrieve the R value from
    LONG h;
    if (!PyArg_ParseTuple(args, "l", &h)) return NULL;
    return Py_BuildValue("i", GetRValue(h));
}
static PyObject* PyGetGValue(PyObject* self, PyObject* args) {
    //@description@ Get the G value from the specified color.@@int
    //@args@ color|int|Color to retrieve the G value from
    LONG h;
    if (!PyArg_ParseTuple(args, "l", &h)) return NULL;
    return Py_BuildValue("i", GetGValue(h));
}
static PyObject* PyGetBValue(PyObject* self, PyObject* args) {
    //@description@ Get the B value from the specified color.@@int
    //@args@ color|int|Color to retrieve the B value from
    LONG h;
    if (!PyArg_ParseTuple(args, "l", &h)) return NULL;
    return Py_BuildValue("i", GetBValue(h));
}
static PyObject* PyGetRGBValues(PyObject* self, PyObject* args) {
    //@description@ Returns the RGB values of a color, as a tuple of three integers.@@tuple
    //@args@ color|int|Color to retrieve the RGB values from
    LONG h;
    if (!PyArg_ParseTuple(args, "l", &h)) return NULL;
    return Py_BuildValue("(iii)", GetRValue(h), GetGValue(h), GetBValue(h));
}
static PyObject* PyGetDCPenColor(PyObject* self, PyObject* args) {
    //@description@ Returns the current pen color for the specified device context.@@int
    //@args@ hdc|HDC|Handle to the device context
    LONG h;
    if (!PyArg_ParseTuple(args, "l", &h)) return NULL;
    LONG res = GetDCPenColor(h);
    if (res == CLR_INVALID) { PyErr_SetFromWindowsErr(87); return NULL; }
    return Py_BuildValue("l", res);
}
static PyObject* PyGetDCBrushColor(PyObject* self, PyObject* args) {
    //@description@ Returns the current brush color for the specified device context.@@int
    //@args@ hdc|HDC|Handle to the device context
    LONG h;
    if (!PyArg_ParseTuple(args, "l", &h)) return NULL;
    LONG res = GetDCBrushColor(h);
    if (res == CLR_INVALID) { PyErr_SetFromWindowsErr(87); return NULL; }
    return Py_BuildValue("l", res);
}
static PyObject* PyGetDCOrgEx(PyObject* self, PyObject* args) {
    //@description@ Returns the final translation origin for a specified device context as a point.@@(int, int)
    //@args@ hdc|HDC|Handle to the device context
    LONG h;
    if (!PyArg_ParseTuple(args, "l", &h)) return NULL;
    POINT p;
    GetDCOrgEx(h, &p);
    return Py_BuildValue("(ll)", p.x, p.y);
}
static PyObject* PyGetDeviceCaps(PyObject* self, PyObject* args) {
    //@description@ Returns device-specific information for the specified device context.@@int
    //@args@ hdc|HDC|Handle to the device context@@index|int|The item to be returned
    LONG h;
    INT index;
    if (!PyArg_ParseTuple(args, "li", &h, &index)) return NULL;
    INT val = GetDeviceCaps(h, index);
    return Py_BuildValue("i", val); 
}
static PyObject* PyGetObjectType(PyObject* self, PyObject* args) {
    //@description@ Retrieves the type of the specified object.@@int
    //@args@ h|int|Handle to the object
    LONG h;
    if (!PyArg_ParseTuple(args, "l", &h)) return NULL;
    LONG val = GetObjectType(h);
    if (RaiseExceptionCheck(val)) return NULL;
    return Py_BuildValue("l", val);
}
static PyObject* PyChord(PyObject* self, PyObject* args) {
    //@description@ Draws a chord outlined by using the current pen and filled by using the current brush.@@bool
    //@args@ hdc|HDC|Handle to the device context@@x1|int|X coordinate of the upper-left corner of the bounding rectangle@@y1|int|Y coordinate of the upper-left corner of the bounding rectangle@@x2|int|X coordinate of the lower-right corner of the bounding rectangle@@y2|int|Y coordinate of the lower-right corner of the bounding rectangle@@x3|int|X coordinate of the endpoint of the radial defining the beginning of the chord@@y3|int|Y coordinate of the endpoint of the radial defining the beginning of the chord@@x4|int|X coordinate of the endpoint of the radial defining the end of the chord@@y4|int|Y coordinate of the endpoint of the radial defining the end of the chord
    LONG hdc;
    int x1, y1, x2, y2, x3, y3, x4, y4;
    if (!PyArg_ParseTuple(args, "liiiiiiii", &hdc, &x1, &y1, &x2, &y2, &x3, &y3, &x4, &y4)) return NULL;
    if (!Chord(hdc, x1, y1, x2, y2, x3, y3, x4, y4)) {
        if (RaiseExceptionCheck(0)) return NULL;
    }
    return Py_BuildValue("O", PyBool_FromLong(TRUE)); 
}
static PyObject* PyPie(PyObject* self, PyObject* args) {
    //@description@ Draws a pie-shaped wedge bounded by the intersection of an ellipse and two radials. The pie is outlined by using the current pen and filled by using the current brush.@@bool
    //@args@ hdc|HDC|Handle to the device context@@left|int|X coordinate of the upper-left corner of the bounding rectangle@@top|int|Y coordinate of the upper-left corner of the bounding rectangle@@right|int|X coordinate of the lower-right corner of the bounding rectangle@@bottom|int|Y coordinate of the lower-right corner of the bounding rectangle@@xr1|int|X-coordinate of the endpoint of the first radial@@yr1|int|Y-coordinate of the endpoint of the first radial@@xr2|int|X-coordinate of the endpoint of the second radial@@yr2|int|Y-coordinate of the endpoint of the second radial
    LONG hdc;
    int x1, y1, x2, y2, x3, y3, x4, y4;
    if (!PyArg_ParseTuple(args, "liiiiiiii", &hdc, &x1, &y1, &x2, &y2, &x3, &y3, &x4, &y4)) return NULL;
    if (!Pie(hdc, x1, y1, x2, y2, x3, y3, x4, y4)) {
        if (RaiseExceptionCheck(0)) return NULL;
    }
    return Py_BuildValue("O", PyBool_FromLong(TRUE)); 
}
static PyObject* PySaveDC(PyObject* self, PyObject* args) {
    //@description@ Saves the current state of the specified device context.@@int
    //@args@ hdc|HDC|Handle to the device context
    LONG hdc;
    if (!PyArg_ParseTuple(args, "l", &hdc)) return NULL;
    INT val = SaveDC(hdc);
    if (RaiseExceptionCheck(val)) return NULL;
    return Py_BuildValue("i", val);
}
static PyObject* PyRestoreDC(PyObject* self, PyObject* args) {
    //@description@ Restores a device context to the specified state returned from the SaveDC function.@@bool
    //@args@ hdc|HDC|Handle to the device context@@nSavedDC|int|The state to be restored
    LONG hdc;
    INT saved;
    if (!PyArg_ParseTuple(args, "li", &hdc, &saved)) return NULL;
    RestoreDC(hdc, saved);
    return Py_BuildValue("O", PyBool_FromLong(TRUE));
}
static PyObject* PyArc(PyObject* self, PyObject* args) {
    //@description@ Draws an elliptical arc.@@bool
    //@args@ hdc|HDC|Handle to the device context@@x1|int|X coordinate of the upper-left corner of the bounding rectangle@@y1|int|Y coordinate of the upper-left corner of the bounding rectangle@@x2|int|X coordinate of the lower-right corner of the bounding rectangle@@y2|int|Y coordinate of the lower-right corner of the bounding rectangle@@x3|int|X coordinate of the ending point of the radial line defining the starting point of the arc.@@y3|int|Y coordinate of the ending point of the radial line defining the starting point of the arc.@@x4|int|X coordinate of the ending point of the radial line defining the ending point of the arc.@@y4|int|Y coordinate of the ending point of the radial line defining the ending point of the arc.
    LONG hdc;
    int x1, y1, x2, y2, x3, y3, x4, y4;
    if (!PyArg_ParseTuple(args, "liiiiiiii", &hdc, &x1, &y1, &x2, &y2, &x3, &y3, &x4, &y4)) return NULL;
    if (!Arc(hdc, x1, y1, x2, y2, x3, y3, x4, y4)) {
        if (RaiseExceptionCheck(0)) return NULL;
    }
    return Py_BuildValue("O", PyBool_FromLong(TRUE));
}
static PyObject* PyArcTo(PyObject* self, PyObject* args) {
    //@description@ Draws an elliptical arc.@@bool
    //@args@ hdc|HDC|Handle to the device context@@left|int|X coordinate of the upper-left corner of the bounding rectangle@@top|int|Y coordinate of the upper-left corner of the bounding rectangle@@right|int|X coordinate of the lower-right corner of the bounding rectangle@@bottom|int|Y coordinate of the lower-right corner of the bounding rectangle@@xr1|int|X coordinate of the endpoint of the radial defining the starting point of the arc@@yr1|int|Y coordinate of the endpoint of the radial defining the starting point of the arc@@xr2|int|X coordinate of the endpoint of the radial defining the ending point of the arc@@yr2|int|Y coordinate of the endpoint of the radial defining the ending point of the arc
    LONG hdc;
    int x1, y1, x2, y2, x3, y3, x4, y4;
    if (!PyArg_ParseTuple(args, "liiiiiiii", &hdc, &x1, &y1, &x2, &y2, &x3, &y3, &x4, &y4)) return NULL;
    if (!ArcTo(hdc, x1, y1, x2, y2, x3, y3, x4, y4)) {
        if (RaiseExceptionCheck(0)) return NULL;
    }
    return Py_BuildValue("O", PyBool_FromLong(TRUE));
}
static PyObject* PyAngleArc(PyObject* self, PyObject* args) {
    //@description@ Draws a line segment and an arc.@@bool
    //@args@ hdc|HDC|Handle to the device context@@x|int|X coordinate of the center of the circle@@y|int|Y coordinate of the center of the circle@@r|int|The radius of the circle@@StartAngle|float|The start angle in degrees, relative to the x-axis@@SweepAngle|float|The sweep angle in degrees, relative to the starting angle
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
    //@description@ Sets the drawing direction to be used for arc and rectangle functions.@@int
    //@args@ hdc|HDC|Handle to the device context@@dir|int|AD_COUNTERCLOCKWISE (Figures drawn counterclockwise) or AD_CLOCKWISE (Figures drawn clockwise)
    LONG hdc;
    INT dir;

    if (!PyArg_ParseTuple(args, "li", &hdc, &dir)) return NULL;

    INT val = SetArcDirection(hdc, dir);
    if (RaiseExceptionCheck(val)) return NULL;

    return Py_BuildValue("i", val);
}
static PyObject* PyGetStockObject(PyObject* self, PyObject* args) {
    //@description@ Returns a handle to one of the stock pens, brushes, fonts, or palettes.@@HGDIOBJ
    //@args@ i|int|The type of stock object to be retrieved.
    INT index;

    if (!PyArg_ParseTuple(args, "i", &index)) return NULL;

    INT val = GetStockObject(index);
    if (RaiseExceptionCheck(val)) return NULL;

    return Py_BuildValue("l", val);
}
static PyObject* PyCreateCompatibleDC(PyObject* self, PyObject* args) {
    //@description@ Creates a memory device context compatible with the specified device.@@HDC
    //@args@ hdc|HDC|Handle to the device context, or None
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
    //@description@ Returns a bitmap compatible with the device that is associated with the specified device context.@@HBITMAP
    //@args@ hdc|HDC|A handle to a device context@@cx|int|The bitmap width, in pixels@@cy|int|The bitmap height, in pixels
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
    //@description@ Cancels any pending operation on the specified device context.@@bool
    //@args@ hdc|HDC|A handle to a device context
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
    //@description@ Selects a region as the current clipping region for the specified device context.@@int
    //@args@ hdc|HDC|A handle to a device context@@hrgn|HRGN|A handle to a region
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
    //@description@ Creates an elliptical region.@@HRGN
    //@args@ x1|int|X coordinate of the upper-left corner of the bounding rectangle of the ellipse@@y1|int|Y coordinate of the upper-left corner of the bounding rectangle of the ellipse@@x2|int|X coordinate of the lower-right corner of the bounding rectangle of the ellipse@@y2|int|Y coordinate of the lower-right corner of the bounding rectangle of the ellipse
    INT x1, y1, x2, y2;

    if (!PyArg_ParseTuple(args, "iiii", &x1, &y1, &x2, &y2)) return NULL;
    HRGN rgn = CreateEllipticRgn(x1, y1, x2, y2);
    if (RaiseExceptionCheck(rgn)) return NULL;
    return Py_BuildValue("l", rgn);
}
static PyObject* PyCreateEllipticRgnIndirect(PyObject* self, PyObject* args) {
    //@description@ Creates an elliptical region from a tuple.@@HRGN
    //@args@ rgnTuple|tuple|A tuple of 4 integers containing the X coordinate of the upper-left corner of the bounding rectangle of the ellipse, Y coordinate of the upper-left corner of the bounding rectangle of the ellipse, X coordinate of the lower-right corner of the bounding rectangle of the ellipse and Y coordinate of the lower-right corner of the bounding rectangle of the ellipse.
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
    //@description@ Creates a rectangular region.@@HRGN
    //@args@ x1|int|X coordinate of the upper-left corner of the bounding rectangle of the rectangle@@y1|int|Y coordinate of the upper-left corner of the bounding rectangle of the rectangle@@x2|int|X coordinate of the lower-right corner of the bounding rectangle of the rectangle@@y2|int|Y coordinate of the lower-right corner of the bounding rectangle of the rectangle.
    INT x1, y1, x2, y2;

    if (!PyArg_ParseTuple(args, "iiii", &x1, &y1, &x2, &y2)) return NULL;
    HRGN rgn = CreateRectRgn(x1, y1, x2, y2);
    if (RaiseExceptionCheck(rgn)) return NULL;
    return Py_BuildValue("l", rgn);
}
static PyObject* PyCreateRectRgnIndirect(PyObject* self, PyObject* args) {
    //@description@ Creates a rectangular region from a tuple.@@HRGN
    //@args@ rgnTuple|tuple|A tuple of 4 integers containing the X coordinate of the upper-left corner of the bounding rectangle of the rectangle, Y coordinate of the upper-left corner of the bounding rectangle of the rectangle, X coordinate of the lower-right corner of the bounding rectangle of the rectangle and Y coordinate of the lower-right corner of the bounding rectangle of the rectangle.
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
    //@description@ Creates a polygon region using the specified vertices.@@HRGN
    //@args@ points|tuple|Tuple of coordinates to be made into a polygon, example ((100, 100), (200, 100), (150, 200)) for a triangle@@mode|int|ALTERNATE or WINDING
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
    //@description@ Checks whether two regions are equal to each other.@@bool
    //@args@ rgn1|HRGN|The first region to be compared@@rgn2|HRGN|The second region to be compared
    LONG rgn1, rgn2;

    if (!PyArg_ParseTuple(args, "ll", &rgn1, &rgn2)) return NULL;
    BOOL retval = EqualRgn(rgn1, rgn2);

    return Py_BuildValue("O", PyBool_FromLong((LONG)retval));
}
static PyObject* PyCreateRoundRectRgn(PyObject* self, PyObject* args) {
    //@description@ Creates a rounded rectangular region.@@HRGN
    //@args@ x1|int|X coordinate of the upper-left rectangle.@@y1|int|Y coordinate of the upper-left rectangle.@@x2|int|X coordinate of the down-right rectangle.@@y2|int|Y coordinate of the down-right rectangle.@@w|int|Horizontal strength of the roundness.@@h|int|Vertical strength of the roundness.
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
    //@description@ Returns the maximum number of function calls that can be accumulated in the calling thread's current batch.@@int
    //@args@ None
    return Py_BuildValue("k", GdiGetBatchLimit());
}
static PyObject* PyGdiSetBatchLimit(PyObject* self, PyObject* args) {
    //@description@ Sets the maximum number of function calls that can be accumulated in the calling thread's current batch.@@int
    //@args@ val|int|The number of maxiumum number of function calls that can be accumulated in the calling thread's current batch
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
    //@description@ Returns the current arc direction for the specified device context.@@int
    //@args@ hdc|HDC|Handle to a device context
    LONG wnd;
    if (!PyArg_ParseTuple(args, "l", &wnd)) return NULL;
    return Py_BuildValue("i", GetArcDirection(wnd));
}
static PyObject* PyGetWorldTransform(PyObject* self, PyObject* args) {
    //@description@ Returns the current world-space to page-space transformation.@@(float, float, float, float, float, float)
    //@args@ hdc|HDC|Handle to a device context
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
    //@description@ Updates the client area of the specified device context by remapping the current colors in the client area to the currently realized logical palette.@@bool
    //@args@ hdc|HDC|Handle to a device context
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
    //@description@ Resets the origin of a brush or resets a palette.@@bool
    //@args@ h|HGDIOBJ|Handle to an object to be reset
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
    //@description@ Writes a string at the specified location, using the currently selected font, background color, and text color.@@bool
    //@args@ hdc|HDC|Handle to a device context@@x|int|The x-position where the text should be drawn at@@y|int|The y-position where the text should be drawn at@@string|str|The text that should be drawn, in ASCII
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
    //@description@ Writes a string at the specified location, using the currently selected font, background color, and text color.@@bool
    //@args@ hdc|HDC|Handle to a device context@@x|int|The x-position where the text should be drawn at@@y|int|The y-position where the text should be drawn at@@string|str|The text that should be drawn, in UTF-16
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
    //@description@ Returns the text-alignment setting for the specified device context.@@int
    //@args@ hdc|HDC|Handle to a device context
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
    //@description@ Sets the text-alignment flags for the specified device context and returns the previous value.@@int
    //@args@ hdc|HDC|Handle to a device context@@align|int|The text alignment by using a mask.
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
static PyObject* PyBeginPath(PyObject* self, PyObject* args) {
    //@description@ Opens a path bracket in the specified device context.@@bool
    //@args@ hdc|HDC|Handle to a device context
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
    //@description@ Closes a path bracket and selects the path defined by the bracket into the specified device context.@@bool
    //@args@ hdc|HDC|Handle to a device context
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
    //@description@ Selects the current path as a clipping region for a device context, merging the new region with any existing clipping region using the specified mode.@@bool
    //@args@ hdc|HDC|Handle to a device context@@mode|int|RGN_AND, RGN_COPY, RGN_DIFF, RGN_OR or RGN_XOR
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
    //@description@ Creates a region from the path that is selected into the specified device context.@@HRGN
    //@args@ hdc|HDC|Handle to a device context
    LONG wnd;
    if (!PyArg_ParseTuple(args, "l", &wnd)) return NULL;
    HRGN res = PathToRegion(wnd);
    if (!res) {
        PyErr_SetFromWindowsErr(87);
        return NULL;
    }
    return Py_BuildValue("l", res);
}
static PyObject* PySetBkMode(PyObject* self, PyObject* args) {
    //@description@ Sets the background mix mode of the specified device context.@@bool
    //@args@ hdc|HDC|Handle to a device context@@mode|int|OPAQUE or TRANSPARENT
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
    //@description@ Returns the current background mix mode for a specified device context.@@int
    //@args@ hdc|HDC|Handle to a device context
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
    //@description@ Sets the current background color to the specified color value.@@int
    //@args@ hdc|HDC|Handle to a device context@@color|int|Background color, commonly returned from the RGB function
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
    //@description@ Returns current background color of the specified device context.@@int
    //@args@ hdc|HDC|Handle to a device context
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
    //@description@ Sets the text color for the specified device context to the specified color.@@int
    //@args@ hdc|HDC|Handle to a device context@@color|int|Text color
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
    //@description@ Returns the text color for the specified device context.@@int
    //@args@ hdc|HDC|Handle to a device context
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
    //@description@ Sets the amount of space the system should add to the break characters in a string of text.@@bool
    //@args@ hdc|HDC|Handle to a device context@@extra|int|The extra space@@count|int|The number of break characters in the line
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
    //@description@ Sets the intercharacter spacing for the specified device context. Returns the previous intercharacter spacing.@@int
    //@args@ hdc|HDC|Handle to a device context@@extra|int|The amount of extra space
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
    //@description@ Sets the current foreground mix mode and returns the previous value.@@int
    //@args@ hdc|HDC|Handle to a device context@@rop2|int|The mixing mode
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
    //@description@ Sets the pixel at the specified coordinate to the specified color.@@int
    //@args@ hdc|HDC|Handle to a device context@@x|int|The X coordinate of the pixel@@y|int|The Y coordinate of the pixel@@color|int|The new color of the pixel
    INT x, y;
    LONG hdcDst, color;

    if (!PyArg_ParseTuple(args, "liil", &hdcDst, &x, &y, &color)) return NULL;
    LONG val = SetPixelV((HANDLE)hdcDst, x, y, color);
    if (val == -1) { PyErr_SetFromWindowsErr(87); return NULL; }
    return Py_BuildValue("l", val);
}
static PyObject* PySetPolyFillMode(PyObject* self, PyObject* args) {
    //@description@ Sets the polygon fill mode for functions that fill polygons@@int
    //@args@ hdc|HDC|Handle to a device context@@mode|int|ALTERNATE or WINDING
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
    //@description@ Selects the specified logical palette into a device context.@@HPALLETE
    //@args@ hdc|HDC|Handle to a device context@@hPal|HPALLETE|Handle to the logical palette to be selected@@forceBkgd|bool|[Optional] Specifies whether the logical palette is forced to be a background palette
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
    //@description@ Draws a rectangle with rounded corners.@@bool
    //@args@ hdc|HDC|Handle to a device context.@@x1|int|X coordinate of the upper-left rectangle.@@y1|int|Y coordinate of the upper-left rectangle.@@x2|int|X coordinate of the down-right rectangle.@@y2|int|Y coordinate of the down-right rectangle.@@w|int|Horizontal strength of the roundness.@@h|int|Vertical strength of the roundness.
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
    //@description@ Creates a logical pen that has the specified style, width, and color.@@HPEN
    //@args@ iStyle|int|The pen style. It can be any one of the following values; ``PS_SOLID, PS_DASH, PS_DOT, PS_DASHDOT, PS_DASHDOTDOT, PS_NULL or PS_INSIDEFRAME``@@cWidth|int|The width of the pen@@color|int|The color of the pen, commonly returned from the RGB function
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
    //@description@ Creates a logical pen that has the specified style, width, and color from a tuple.@@HPEN
    //@args@ penProperties|tuple|The touple should contain the pen style, the width of the pen as a tuple of two integers, and the color of the pen.
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
    //@description@ Creates a logical cosmetic or geometric pen that has the specified style, width, and brush attributes.@@HPEN
    //@args@ style|int|The type can be ``PS_GEOMETRIC or PS_COSMETIC``, combine it with the OR operator with the style that can be any of the following values; ``PS_ALTERNATE, PS_SOLID, PS_DASH, PS_DOT, PS_DASHDOT, PS_DASHDOTDOT, PS_NULL, PS_USERSTYLE, PS_INSIDEFRAME``, combine it with the OR operator if the pen type is geometric with any of the following values (end cap); ``PS_ENDCAP_ROUND, PS_ENDCAP_SQUARE, PS_ENDCAP_FLAT``, if it is geometrical you should also combine it with the OR operator with any of the following values (join); ``PS_JOIN_BEVEL, PS_JOIN_MITER, PS_JOIN_ROUND``.@@cWidth|int|The width of the pen. If the pen type is ``PS_COSMETIC`` it should be set to 1.@@logBrush|tuple|Tuple containing 3 elements; style, color and a hatch brush.@@pstyle|tuple|Tuple of maximum 16 ints that specifies an user-defined style, or None.
    DWORD style; DWORD width;
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
    //@description@ Fills the specified region using the given brush.@@bool
    //@args@ hdc|HDC|Handle to a device context@@rgn|HRGN|Region to be filled@@brush|HBRUSH|Brush to paint the region with
    LONG hdc; LONG hrgn; LONG brush;
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
    //@description@ Transforms any curves in the path that is selected into the current device context, turning curves into sequences of lines.@@bool
    //@args@ hdc|HDC|Handle to a device context
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
    //@description@ Draws a border around the specified region by using the specified brush.@@bool
    //@args@ hdc|HDC|Handle to a device context@@rgn|HRGN|Handle to a region to be drawn a border around it@@hbr|HBRUSH|Brush to paint the border with@@w|int|Width of vertical brush strokes.@@h|int|Height of horizontal brush strokes.
    LONG hdc;
    LONG hrgn; LONG brush;
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
    LONG hdc;
    LONG hrgn;
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
    //@description@ Draws a line from the current position up to, but not including, the specified point.@@bool
    //@args@ hdc|HDC|Handle to a device context@@x|int|X-coordinate of the line@@y|int|Y-coordinate of the line
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
    //@description@ Paints the specified region by using the brush currently selected into the device context.@@bool
    //@args@ hdc|HDC|Handle to a device context@@rgn|HRGN|Region to be painted
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
    //@description@ Performs a bit-block transfer of the bits of color data from the specified rectangle in the source device context to the specified parallelogram in the destination device context. If the given bitmask handle identifies a valid monochrome bitmap, the function uses this bitmap to mask the bits of color data from the source rectangle.@@bool
    //@args@ hdcDst|hdc|Handle to the destination device context.@@points|tuple|Tuple of three tuples of two ints that identify the parallelogram. Example ``((-10, 10), (20, 0), (10, -10))``.@@hdcSrc|hdc|Handle to the source device context.@@xSrc|int|The x-coordinate, in logical units, of the upper-left corner of the source rectangle.@@ySrc|int|The y-coordinate, in logical units, of the upper-left corner of the source rectangle.@@width|int|The width, in logical units, of the source rectangle.@@height|int|The height, in logical units, of the source rectangle.@@bitmap|HBITMAP|None or a handle to a monochrome bitmap that is used to mask the colors of the source rectangle.@@xMask|int|The x-coordinate, in logical units, of the upper-left corner of the monochrome bitmap.@@yMask|int|The y-coordinate, in logical units, of the upper-left corner of the monochrome bitmap.
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
    //@description@ Combines the color data for the source and destination bitmaps using the specified mask and raster operation.@@bool
    //@args@ hdcDest|hdc|Handle to the destination device context.@@xDest|int|The x-coordinate, in logical units, of the upper-left corner of the source rectangle.@@yDest|int|The y-coordinate, in logical units, of the upper-left corner of the source rectangle.@@width|int|The width, in logical units, of the source rectangle.@@height|int|The height, in logical units, of the source rectangle.@@hdcSrc|hdc|Handle to the source device context.@@xSrc|int|The x-coordinate, in logical units, of the upper-left corner of the source bitmap.@@ySrc|int|The y-coordinate, in logical units, of the upper-left corner of the source bitmap.@@hbmMask|HBITMAP|A handle to a monochrome bitmap that is used to mask the colors of the source rectangle.@@xMask|int|The x-coordinate, in logical units, of the upper-left corner of the monochrome bitmap.@@yMask|int|The y-coordinate, in logical units, of the upper-left corner of the monochrome bitmap.
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
static PyObject* PyCreateHatchBrush(PyObject* self, PyObject* args) {
    //@description@ Creates a logical brush that has the specified hatch pattern and color.@@HBRUSH
    //@args@ iHatch|int|The hatch style of the brush.@@color|int|The foreground color of the brush that is used for the hatches. To create a color value, use the RGB function.
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

typedef struct _PyDIBSection {
    LPRGBQUAD lpRgb;
    LONG nSize;
    LONG nWidth;
    LONG nHeight;
} PyDIBSection, *PPyDIBSection, *LPPyDibSection;

static PyObject* PyCreateDIBSection(PyObject*_, PyObject* args) {
    HANDLE hHeap = GetProcessHeap( );
    PBITMAPINFO bmp = (PBITMAPINFO)HeapAlloc( hHeap, 0, sizeof(BITMAPINFO) );
    if (bmp == NULL) { PyErr_SetString(PyExc_MemoryError, "Out of memory!"); return NULL; }
    HDC hdc;
    if (!PyArg_ParseTuple(args, "l(llHkkllkk)",
        &hdc,
        &bmp->bmiHeader.biWidth,
        &bmp->bmiHeader.biHeight,
        &bmp->bmiHeader.biBitCount,
        &bmp->bmiHeader.biCompression,
        &bmp->bmiHeader.biSizeImage,
        &bmp->bmiHeader.biXPelsPerMeter,
        &bmp->bmiHeader.biYPelsPerMeter,
        &bmp->bmiHeader.biClrUsed,
        &bmp->bmiHeader.biClrImportant
    )) return NULL;
    
    LONG nSize = bmp->bmiHeader.biWidth * bmp->bmiHeader.biHeight * sizeof(COLORREF);
    bmp->bmiHeader.biSize = sizeof(BITMAPINFO);
    bmp->bmiHeader.biPlanes = 1;

    INT usage = DIB_RGB_COLORS;
    LPRGBQUAD prgbDst = (LPRGBQUAD)HeapAlloc( hHeap, 0, nSize );
    HBITMAP hbm = CreateDIBSection(hdc, &bmp, 0, &prgbDst, NULL, 0);

    LPPyDibSection res = (LPPyDibSection)HeapAlloc( hHeap, 0, sizeof(PyDIBSection) );

    if (res == NULL || prgbDst == NULL) {
        HeapFree(hHeap, 0, bmp);
        PyErr_SetString(PyExc_MemoryError, "Out of memory!");
        return NULL;
    }
    
    res->lpRgb = prgbDst;
    res->nWidth = bmp->bmiHeader.biWidth;
    res->nHeight = bmp->bmiHeader.biHeight;
    res->nSize = nSize;

    HeapFree(hHeap, 0, bmp);

    return Py_BuildValue("(LL)", (LONGLONG)hbm, (LONGLONG)res);
}
static PyObject* PyFreeDIBSection(PyObject*_, PyObject* args) {
    LPPyDibSection rgb;
    if (!PyArg_ParseTuple(args, "L", &rgb)) return NULL;
    
    free(rgb);

    return Py_BuildValue("i", 0);
}

typedef struct {
    PyObject_HEAD
        /* Your internal 'loc' data. */
        LPPyDibSection loc;
} PY_DIBDATA;

static void PyDib_dealloc(PY_DIBDATA* self)
{
    HeapFree(GetProcessHeap( ), 0, self->loc->lpRgb);
    HeapFree(GetProcessHeap( ), 0, self->loc);
    self->ob_base.ob_type->tp_free((PyObject*)self);
}

static int PyDib_init(PY_DIBDATA* self, PyObject* args, PyObject* kwds)
{
    if (!PyArg_ParseTuple(args, "L", &self->loc))
        return NULL;

    return 0;
}

static PyObject* PyDib_SetSectionData(PY_DIBDATA* self, PyObject* args) {
    LPPyDibSection rgb = self->loc;
    CHAR r, g, b;
    PyObject* bArr;
    if (!PyArg_ParseTuple(args, "O", &bArr)) return NULL;

    if (!PyByteArray_Check(bArr)) {
        PyErr_SetString(PyExc_TypeError, "argument 2: excepted bytearray object");
        return NULL;
    }

    LONGLONG length = PyByteArray_Size(bArr);
    CHAR* arr = PyByteArray_AsString(bArr);

    for (INT i = 0; i < length; i += 3) {
        rgb->lpRgb[i].rgbRed = arr[i + 0];
        rgb->lpRgb[i].rgbGreen = arr[i + 1];
        rgb->lpRgb[i].rgbBlue = arr[i + 2];
    }

    return Py_BuildValue("i", 0);
}
static PyObject* PyDib_GetSectionData(PY_DIBDATA* self, PyObject* args) {
    HANDLE hHeap = GetProcessHeap( );
    LPPyDibSection rgb = self->loc->lpRgb;
    CHAR r, g, b;
    INT ra;

    LONG len = rgb->nWidth * rgb->nHeight;

    CHAR* result = HeapAlloc(hHeap, 0, sizeof(CHAR) * len * 3);

    printf("%ld x %ld (%ld)\n", rgb->nWidth, rgb->nHeight, rgb->nSize);

    for (INT i = 0; i < len; i+= 3) {
        printf("%d", i);

        ra = i != 0 ? i / 3 : 0;

        r = rgb->lpRgb[ra].rgbRed;
        g = rgb->lpRgb[ra].rgbGreen;
        b = rgb->lpRgb[ra].rgbBlue;
        result[i] = r;
        result[i + 1] = g;
        result[i + 2] = b;
    }

    PyObject* bArr = PyByteArray_FromStringAndSize(result, len);

    HeapFree(hHeap, 0, result);

    return Py_BuildValue("O", bArr);
}


static PyMethodDef PyDib_methods[] = {
    { "SetDIBSectionData", PyDib_SetSectionData, METH_VARARGS },
    { "GetDIBSectionData", PyDib_GetSectionData, METH_NOARGS },
    {NULL}
};

static PyMemberDef PyDib_members[] = {
    {"loc", T_LONGLONG, offsetof(PY_DIBDATA, loc), READONLY, "Pointer address to structure defining the properties of the DIB." },
    {NULL}  /* Sentinel */
};

static PyObject* PyDib_new(PyTypeObject* type, PyObject* args, PyObject* kwds) {
    PY_DIBDATA* self;

    self = (PY_DIBDATA*)type->tp_alloc(type, 0);
    self->loc = 0;

    return (PyObject*)self;
}

static PyTypeObject PyDib_Types = {
    PyObject_HEAD_INIT(NULL)
    "WinAPY_gdi.DIBSection",   /*tp_name*/
    sizeof(PY_DIBDATA),        /*tp_basicsize*/
    0,                         /*tp_itemsize*/
    (destructor)PyDib_dealloc, /*tp_dealloc*/
    0,                         /*tp_print*/
    0,                         /*tp_getattr*/
    0,                         /*tp_setattr*/
    0,                         /*tp_compare*/
    0,                         /*tp_repr*/
    0,                         /*tp_as_number*/
    0,                         /*tp_as_sequence*/
    0,                         /*tp_as_mapping*/
    0,                         /*tp_hash */
    0,                         /*tp_call*/
    0,                         /*tp_str*/
    0,                         /*tp_getattro*/
    0,                         /*tp_setattro*/
    0,                         /*tp_as_buffer*/
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,/*tp_flags*/
    "DIB section object",          /* tp_doc */
    0,                         /* tp_traverse */
    0,                         /* tp_clear */
    0,                         /* tp_richcompare */
    0,                         /* tp_weaklistoffset */
    0,                         /* tp_iter */
    0,                         /* tp_iternext */
    PyDib_methods,      /* tp_methods, error here! */
    PyDib_members,      /* tp_members */
    0,                         /* tp_getset */
    0,                         /* tp_base */
    0,                         /* tp_dict */
    0,                         /* tp_descr_get */
    0,                         /* tp_descr_set */
    0,                         /* tp_dictoffset */
    (initproc)PyDib_init,/* tp_init */
    0,                         /* tp_alloc */
    PyDib_new,                 /* tp_new */
};

static PyMethodDef module_methods[] = {
    {"CreateDIBSection", (PyCFunction)PyCreateDIBSection, METH_VARARGS},
    {"PatBlt", (PyCFunction)PyPatBlt, METH_VARARGS},
    {"DeleteDC", (PyCFunction)PyDeleteDC, METH_VARARGS},
    {"BitBlt", (PyCFunction)PyBitBlt, METH_VARARGS},
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
    { "SaveDC", PySaveDC, METH_VARARGS },
    { "RestoreDC", PyRestoreDC, METH_VARARGS },
    { "CancelDC", PyCancelDC, METH_VARARGS },
    { "Arc", PyArc, METH_VARARGS },
    { "ArcTo", PyArcTo, METH_VARARGS },
    { "AngleArc", PyAngleArc, METH_VARARGS },
    { "SetArcDirection", PySetArcDirection, METH_VARARGS },
    { "GetStockObject", PyGetStockObject, METH_VARARGS },
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
    { "BeginPath", PyBeginPath, METH_VARARGS },
    { "EndPath", PyEndPath, METH_VARARGS },
    { "SelectClipPath", PySelectClipPath, METH_VARARGS },
    { "PathToRegion", PyPathToRegion, METH_VARARGS },
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
    { "GetDeviceCaps", PyGetDeviceCaps, METH_VARARGS },
    { "GetDCBrushColor", PyGetDCBrushColor, METH_VARARGS },

    /* sentinel */
    { 0 }
};

static struct PyModuleDef ModuleCombinations =
{
    PyModuleDef_HEAD_INIT,
    "winapy_gdi", /* name of module */
    NULL,
    -1,
    module_methods
};


PyMODINIT_FUNC PyInit_winapy_gdi(void) {
    PyObject* m;

    if (PyType_Ready(&PyDib_Types) < 0)
        return NULL;

    m = PyModule_Create(&ModuleCombinations);

    Py_INCREF(&PyDib_Types);
    PyModule_AddObject(m, "DIBSection", (PyObject*)&PyDib_Types);

    return Py_BuildValue("O", m);
}
