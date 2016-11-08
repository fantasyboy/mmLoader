// mmLoader-shellcode-generator.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <windows.h>
#include "..\src\mmLoader\mmLoader.h"
#include <Strsafe.h>

#pragma comment(linker, "/NODEFAULTLIB")
#pragma comment(linker, "/ENTRY:CodeBag")

LPSTR head =
"//\r\n"
"// This file is generated by the mmLoaderShellCode Generator\r\n"
"// https://github.com/tishion\r\n"
"//\r\n"
"#ifndef __MMLOADERSHELLCODE_H_INCLUDED_\r\n"
"#define __MMLOADERSHELLCODE_H_INCLUDED_\r\n"
"#pragma once\r\n"
"#include <windows.h>\r\n"
"\r\n"
"/// <summary>\r\n"
"/// Function table. These function will be used in the mmLoader.\r\n"
"/// </summary>\r\n"
"typedef struct __NTFUNCPTRS\r\n"
"{\r\n"
"	LPVOID pfnCreateFileW;			// CreateFileW\r\n"
"	LPVOID pfnGetFileSize;			// GetFileSize\r\n"
"	LPVOID pfnCreateFileMappingW;	// CreateFileMappingW\r\n"
"	LPVOID pfnMapViewOfFile;		// MapViewOfFile\r\n"
"	LPVOID pfnUnmapViewOfFile;		// UnmapViewOfFile\r\n"
"	LPVOID pfnCloseHandle;			// CloseHandle\r\n"
"	LPVOID pfnGetModuleHandleA;		// GetModuleHandleA\r\n"
"	LPVOID pfnLoadLibraryA;			// LoadLibraryA\r\n"
"	LPVOID pfnGetProcAddress;		// GetProcAddress\r\n"
"	LPVOID pfnVirtualAlloc;			// VirtualAlloc\r\n"
"	LPVOID pfnVirtualFree;			// VirtualFree\r\n"
"	LPVOID pfnVirtualProtect;		// VirtualProtect\r\n"
"	LPVOID pfnReversed_0;\r\n"
"	LPVOID pfnReversed_1;\r\n"
"	LPVOID pfnReversed_2;\r\n"
"	LPVOID pfnReversed_3;\r\n"
"	LPVOID pfnReversed_4;\r\n"
"} NTFUNCPTRSTABLE, *PNTFUNCPTRSTABLE;\r\n"
"\r\n"
"/// <summary>\r\n"
"/// Represents the memory module instance.\r\n"
"/// </summary>\r\n"
"typedef struct __MEMMODULE\r\n"
"{\r\n"
"	union								// MemModule base\r\n"
"	{\r\n"
"		DWORD	dwBase;\r\n"
"		HMODULE	hModule;\r\n"
"		LPVOID	lpBase;\r\n"
"		PIMAGE_DOS_HEADER pImageDosHeader;\r\n"
"	};\r\n"
"	DWORD dwSizeOfImage;				// MemModule size\r\n"
"	DWORD dwCrc;						// MemModule crc32\r\n"
"	DWORD dwPageSize;					// System page size\r\n"
"\r\n"
"	BOOL  bLoadOk;						// MemModule is load ok?\r\n"
"\r\n"
"	PNTFUNCPTRSTABLE pNtFuncptrsTable;	// Pointer to NT function pointers table \r\n"
"\r\n"
"	struct								// Raw file resource data\r\n"
"	{\r\n"
"		HANDLE	h;\r\n"
"		HANDLE	hMapping;\r\n"
"		LPVOID	pBuffer;\r\n"
"	} RawFile;\r\n"
"\r\n"
"	TCHAR tszModuleName[MAX_PATH];		// MemModule Name (or full file path name)\r\n"
"   __MEMMODULE()\r\n"
"	{\r\n"
"		dwBase = 0;\r\n"
"		dwSizeOfImage = 0;\r\n"
"		dwCrc = 0;\r\n"
"		bLoadOk = 0;\r\n"
"		pNtFuncptrsTable = 0;\r\n"
"		RawFile.h = 0;\r\n"
"\r\n"
"		SYSTEM_INFO sysInfo;\r\n"
"		::GetNativeSystemInfo(&sysInfo);\r\n"
"		dwPageSize = sysInfo.dwPageSize;\r\n"
"		for (int i = 0; i < MAX_PATH; i++) tszModuleName[i] = 0;\r\n"
"	}\r\n"
"} MEM_MODULE, *PMEM_MODULE;\r\n"
"\r\n"
"/// <summary>\r\n"
"/// Enums for MemModuleHelper.\r\n"
"/// </summary>\r\n"
"typedef enum _MMHELPER_METHOD\r\n"
"{\r\n"
"	MHM_BOOL_LOAD,\r\n"
"	MHM_VOID_FREE,\r\n"
"	MHM_FARPROC_GETPROC,\r\n"
"} MMHELPER_METHOD;\r\n"
"\r\n"
"/// <summary>\r\n"
"/// Type of the MemModuleHlper function.\r\n"
"/// </summary>\r\n"
"typedef int(__stdcall * Type_MemModuleHelper)(PMEM_MODULE, MMHELPER_METHOD, LPCWSTR, LPCSTR, BOOL);\r\n"
"\r\n"
"/************************************************************************\\\r\n"
" *\r\n"
" * Auxiliary Function:\r\n"
" *		use the mmLoader through this function after it is loaded from shell code.\r\n"
" *\r\n"
" * Parameters:\r\n"
" *		pMmeModule:\r\n"
" *\r\n"
" *		method:\r\n"
" *			Function to be used\r\n"
" *\r\n"
" *		lpModuleName:\r\n"
" *			name of the module to be loaded, only valid when method == MHM_BOOL_LOAD\r\n"
" *			\r\n"
" *		lpProcName:\r\n"
" *			name of the proc to be retrieved, only valid when MHM_FARPROC_GETPROC\r\n"
" *			\r\n"
" *		bCallEntry:\r\n"
" *			need to call the module entry point?\r\n"
" *\r\n"
" *	return value:\r\n"
" *		when method == MHM_BOOL_LOAD\r\n"
" *			return the resulT of loading, TRUE or FALSE\r\n"
" *\r\n"
" *		when method MHM_VOID_FREE:\r\n"
" *			no return value\r\n"
" *\r\n"
" *		when method == MHM_FARPROC_GETPROC\r\n"
" *			return the address of the target proc, return NULL when failed to get the address\r\n"
" *\r\n"
" *\r\n"
" *\r\n"
"\\************************************************************************/\r\n"
"\r\n"
"/// <summary>\r\n"
"/// The byte array of the mmLoader shell code.\r\n"
"/// </summary>\r\n"
"unsigned char mmLoaderShellCode[] =\r\n"
"{\r\n";

LPSTR tail =
"\r\n};\r\n"
"\r\n"
"/************************************************************************\r\n"
"#include \"stdafx.h\"\r\n"
"#include <windows.h>\r\n"
"#include <tchar.h>\r\n"
"#include <strsafe.h>\r\n"
"#include \"mmLoaderShellCode.h\"\r\n"
"\r\n"
"int main()\r\n"
"{\r\n"
"	// Function table\r\n"
"	NTFUNCPTRSTABLE sNtFuncPtrsTable;\r\n"
"	sNtFuncPtrsTable.pfnCreateFileW = CreateFileW;\r\n"
"	sNtFuncPtrsTable.pfnGetFileSize = GetFileSize;\r\n"
"	sNtFuncPtrsTable.pfnCreateFileMappingW = CreateFileMappingW;\r\n"
"	sNtFuncPtrsTable.pfnMapViewOfFile = MapViewOfFile;\r\n"
"	sNtFuncPtrsTable.pfnUnmapViewOfFile = UnmapViewOfFile;\r\n"
"	sNtFuncPtrsTable.pfnCloseHandle = CloseHandle;\r\n"
"	sNtFuncPtrsTable.pfnGetModuleHandleA = GetModuleHandleA;\r\n"
"	sNtFuncPtrsTable.pfnLoadLibraryA = LoadLibraryA;\r\n"
"	sNtFuncPtrsTable.pfnGetProcAddress = GetProcAddress;\r\n"
"	sNtFuncPtrsTable.pfnVirtualAlloc = VirtualAlloc;\r\n"
"	sNtFuncPtrsTable.pfnVirtualFree = VirtualFree;\r\n"
"	sNtFuncPtrsTable.pfnVirtualProtect = VirtualProtect;\r\n"
"\r\n"
"	// Memory module\r\n"
"	MEM_MODULE sMemModule;\r\n"
"	::RtlZeroMemory(&sMemModule, sizeof(sMemModule));\r\n"
"	sMemModule.RawFile.h = INVALID_HANDLE_VALUE;\r\n"
"	sMemModule.pNtFuncptrsTable = &sNtFuncPtrsTable;\r\n"
"\r\n"
"	// Allocate memory buffer for shell code with EXECUTE privilege\r\n"
"	LPVOID  lpShellCodeBase = ::VirtualAlloc(\r\n"
"		NULL,\r\n"
"		sizeof(mmLoaderShellCode),\r\n"
"		MEM_RESERVE | MEM_COMMIT,\r\n"
"		PAGE_EXECUTE_READWRITE);\r\n"
"\r\n"
"	if (NULL == lpShellCodeBase)\r\n"
"	{\r\n"
"		::_tprintf(_T(\"Failed to allocate space for ShellCode!\\r\\n\"));\r\n"
"		return FALSE;\r\n"
"	}\r\n"
"\r\n"
"	// Copy shell code to the executable memory buffer\r\n"
"	::RtlCopyMemory(lpShellCodeBase, mmLoaderShellCode, sizeof(mmLoaderShellCode));\r\n"
"\r\n"
"	// Get the helper function \r\n"
"	Type_MemModuleHelper pfnMemModuleHelper = (Type_MemModuleHelper)lpShellCodeBase;\r\n"
"\r\n"
"	// Load the module\r\n"
"	WCHAR wszDllPath[] = L\"demo-module.dll\";\r\n"
"	if (pfnMemModuleHelper(&sMemModule, MHM_BOOL_LOAD, wszDllPath, NULL, FALSE))\r\n"
"	{\r\n"
"		_tprintf(_T(\"Module was load user32.dll successfully.Module Base : 0x%08X!\\r\\n\"), sMemModule.dwBase);\r\n"
"\r\n"
"		// Get address of function demoFunction\r\n"
"		LPVOID lpAddr = (LPVOID)pfnMemModuleHelper(&sMemModule, MHM_FARPROC_GETPROC, NULL, \"demoFunction\", FALSE);\\r\\n"
"		if (lpAddr)\r\n"
"		{\r\n"
"			_tprintf(_T(\"Get address of demoFunction successfully.Address: 0x%p!\\r\\n\"), lpAddr);\r\n"
"\r\n"
"			// Function pointer type of demoFunction\r\n"
"			typedef int (WINAPI * Type_TargetFunction)(HWND, LPCSTR, LPCSTR, UINT);\r\n"
"\r\n"
"			// Call the demoFunction\r\n"
"			Type_TargetFunction pfnFunction = (Type_TargetFunction)lpAddr;\r\n"
"			pfnFunction();\r\n"
"		}\r\n"
"		else\r\n"
"			_tprintf(_T(\"Failed to get address of demoFunction from memory module\\r\\n.\"));\r\n"
"\r\n"
"		// Free the module\r\n"
"		pfnMemModuleHelper(&sMemModule, MHM_VOID_FREE, NULL, NULL, FALSE);\r\n"
"	}\r\n"
"	else\r\n"
"		_tprintf(_T(\"Failed to load the module!\\r\\n\"));\r\n"
"\r\n"
"	// Free the memory buffer of the shell code\r\n"
"	::VirtualFree(lpShellCodeBase, 0, MEM_RELEASE);\r\n"
"\r\n"
"	return 0;\r\n"
"}\r\n"
"************************************************************************/\r\n"
"\r\n"
"#endif // __MMLOADERSHELLCODE_H_INCLUDED_";

/// <summary>
/// 
/// </summary>
void InitializeConsole()
{
	// Create a console
	if (!::AllocConsole())
		::MessageBox(NULL, _T("Failed to allocate console."), _T("Warning"), MB_OK);
}

/// <summary>
/// 
/// </summary>
/// <param name="message"></param>
void ConsoleWrite(LPCSTR message)
{
	if (!message)
		return;

	DWORD bytesWritten = 0;
	HANDLE h = ::GetStdHandle(STD_OUTPUT_HANDLE);
	if (!h || !::WriteConsoleA(h, message, lstrlenA(message), &bytesWritten, NULL))
		::MessageBoxA(NULL, message, "Warning", MB_OK);
}

#ifdef _DEBUG
#define SHELLCODE_HEADER_FILE_NAME "mmLoaderShellCode-debug.h"
#else
#define SHELLCODE_HEADER_FILE_NAME "mmLoaderShellCode-release.h"
#endif

/// <summary>
/// 
/// </summary>
#define mml_ZeroMemory(p, len) for (int i=0; i < len; i++) *(unsigned char*)p = 0

/// <summary>
/// 
/// </summary>
#define mml_hextoascii(hex) 

/// <summary>
/// main function.
/// </summary>
/// <returns></returns>
int CodeBag()
{
	// Initialize the console
	InitializeConsole();

	// Get code start and end address
	unsigned char* pStart = (unsigned char*)&mmLoaderSCStart;
	unsigned char* pEnd = (unsigned char*)&mmLoaderSCEnd;

	// Get code length
	int codeLength = (pEnd - pStart);

	// Get the buffer length
	size_t textLength = 512 * 1024;

	// Allocate the heap buffer for the file content
	ConsoleWrite("Allocating buffer..\r\n");
	LPSTR pBuffer = (LPSTR)::HeapAlloc(::GetProcessHeap(), HEAP_ZERO_MEMORY, textLength);
	if (!pBuffer)
		ConsoleWrite("Failed to allocate memory buffer.\r\n");

	// Start to build the file content
	ConsoleWrite("Building the file content..\r\n");\
	::StringCchCatA(pBuffer, textLength, head);
	unsigned char charTable[] = { "0123456789ABCDEF" };
	unsigned char buf[3] = { 0, 0, 0 };
	int n = 0;
	for (unsigned char* p = pStart; p < pEnd; p++)
	{
		// Start one row
		if (0 == n++) ::StringCchCatA(pBuffer, textLength, "\t");
		{
			// Hex to string
			::StringCchCatA(pBuffer, textLength, "0x");
			buf[0] = charTable[*p >> 4];
			buf[1] = charTable[*p & 0x0f];
			::StringCchCatA(pBuffer, textLength, (char*)buf);
			::StringCchCatA(pBuffer, textLength, ", ");
		}
		// End one row
		if (n == 16 && !(n = 0)) ::StringCchCatA(pBuffer, textLength, "\r\n");
	}
	::StringCchCatA(pBuffer, textLength, tail);

	// Display the file content
	ConsoleWrite("File content building done:\r\n\r\n");
	ConsoleWrite(pBuffer);

	// Get the valid string length
	::StringCchLengthA(pBuffer, textLength, &textLength);

	ConsoleWrite("Create file mmLoaderShellCode.h\r\n");
	// Create file to save the content
	HANDLE h = 	::CreateFile(_T("mmLoaderShellCode.h"), FILE_WRITE_ACCESS, FILE_SHARE_READ, NULL, CREATE_ALWAYS, NULL, NULL);
	DWORD dwBytesWritten = 0;
	if (INVALID_HANDLE_VALUE == h || NULL == h)
		ConsoleWrite("Failed to create file \"mmLoaderShellCode.h\".\r\n");
	else
	{
		// Write the string content to the disk file
		if (!::WriteFile(h, pBuffer, textLength, &dwBytesWritten, NULL))
			ConsoleWrite("Failed to write content to file \"mmLoaderShellCode.h\".\r\n");

		::FlushFileBuffers(h);
		::CloseHandle(h);
	}

	// Free the content buffer
	::HeapFree(::GetProcessHeap(), NULL, pBuffer);

	// Wait for the return key
	ConsoleWrite("\r\nShell code generated done.\r\n");
	::WaitForSingleObject(::GetCurrentProcess(), INFINITE);

	return 0;
}