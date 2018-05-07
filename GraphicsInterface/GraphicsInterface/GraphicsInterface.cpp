// GraphicsInterface.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"


#include "stdafx.h"
using namespace Gdiplus;



// Global Variables:
GdiplusStartupInput gdiplusStartupInput;
ULONG_PTR gdiplusToken;


// Forward declarations of functions included in this code module:
extern "C" __declspec(dllexport) void draw(HDC hdc);
extern "C" __declspec(dllexport) void print(void);
extern "C" __declspec(dllexport) HDC getPrinterDC(TCHAR *printerName);
extern "C" __declspec(dllexport) ULONG test(void);

extern "C" __declspec(dllexport) ULONG test(void) {
	return 123456;
}



extern "C" __declspec(dllexport) void draw(HDC hdc)
{
	Graphics* graphics = new Graphics(hdc);
	Pen* pen = new Pen(Color(255, 0, 0, 0));
	graphics->DrawLine(pen, 50, 50, 350, 550);
	graphics->DrawRectangle(pen, 50, 50, 300, 500);
	graphics->DrawEllipse(pen, 50, 50, 300, 500);
	delete pen;
	delete graphics;
}

extern "C" __declspec(dllexport) void print(void) {
	// Get a device context for the printer.
	HDC hdc = getPrinterDC(NULL);
	DOCINFO docInfo;
	ZeroMemory(&docInfo, sizeof(docInfo));
	docInfo.cbSize = sizeof(docInfo);
	docInfo.lpszDocName = TEXT("GdiplusPrint");

	StartDoc(hdc, &docInfo);
	StartPage(hdc);

	draw(hdc);

	EndPage(hdc);
	EndDoc(hdc);
	DeleteDC(hdc);

}

extern "C" __declspec(dllexport) HDC getPrinterDC(TCHAR *printerName) {
	DWORD numprinters;
	DWORD defprinter = 0;
	DWORD               dwSizeNeeded = 0;
	DWORD               dwItem;
	LPPRINTER_INFO_2    printerinfo = NULL;

	// Get buffer size

	EnumPrinters(PRINTER_ENUM_LOCAL | PRINTER_ENUM_CONNECTIONS, NULL, 2, NULL, 0, &dwSizeNeeded, &numprinters);

	// allocate memory
	//printerinfo = (LPPRINTER_INFO_2)HeapAlloc ( GetProcessHeap (), HEAP_ZERO_MEMORY, dwSizeNeeded );
	printerinfo = (LPPRINTER_INFO_2)new char[dwSizeNeeded];

	if (EnumPrinters(PRINTER_ENUM_LOCAL | PRINTER_ENUM_CONNECTIONS,      // what to enumerate
		NULL,           // printer name (NULL for all)
		2,              // level
		(LPBYTE)printerinfo,        // buffer
		dwSizeNeeded,       // size of buffer
		&dwSizeNeeded,      // returns size
		&numprinters            // return num. items
	) == 0)
	{
		numprinters = 0;
	}

	{
		DWORD size = 0;

		// Get the size of the default printer name.
		GetDefaultPrinter(NULL, &size);
		if (size)
		{
			// Allocate a buffer large enough to hold the printer name.
			TCHAR* buffer = new TCHAR[size];

			// Get the printer name.
			GetDefaultPrinter(buffer, &size);

			for (dwItem = 0; dwItem < numprinters; dwItem++)
			{
				if (!wcscmp(buffer, printerinfo[dwItem].pPrinterName))
					defprinter = dwItem;
			}
			delete buffer;
		}
	}

	return CreateDC(NULL, printerName == NULL ? printerinfo[defprinter].pPrinterName : printerName, NULL, NULL);
}




