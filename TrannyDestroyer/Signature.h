#pragma once

#include <time.h>
#include <Windows.h>

#define INRANGE(x,a,b)    (x >= a && x <= b) 
#define getBits( x )    (INRANGE((x&(~0x20)),'A','F') ? ((x&(~0x20)) - 'A' + 0xa) : (INRANGE(x,'0','9') ? x - '0' : 0))
#define getByte( x )    (getBits(x[0]) << 4 | getBits(x[1]))

class Signature
{
public:
	DWORD dwFindPattern(DWORD dwAddress, DWORD dwLength, const char* szPattern)
	{
		const char* pat = szPattern;
		DWORD firstMatch = NULL;
		for (DWORD pCur = dwAddress; pCur < dwLength; pCur++)
		{
			if (!*pat) return firstMatch;
			if (*(PBYTE)pat == '\?' || *(BYTE*)pCur == getByte(pat)) {
				if (!firstMatch) firstMatch = pCur;
				if (!pat[2]) return firstMatch;
				if (*(PWORD)pat == '\?\?' || *(PBYTE)pat != '\?') pat += 3;
				else pat += 2;
			}
			else {
				pat = szPattern;
				firstMatch = 0;
			}
		}
		return NULL;
	}

	DWORD Find(const char* module, const char* chPattern, int offset = 0)
	{
		static HMODULE hmModule = GetModuleHandle(module);
		static PIMAGE_DOS_HEADER pDOSHeader = (PIMAGE_DOS_HEADER)hmModule;
		static PIMAGE_NT_HEADERS pNTHeaders = (PIMAGE_NT_HEADERS)(((DWORD)hmModule) + pDOSHeader->e_lfanew);
		return dwFindPattern(((DWORD)hmModule) + pNTHeaders->OptionalHeader.BaseOfCode, ((DWORD)hmModule) + pNTHeaders->OptionalHeader.SizeOfCode, chPattern) + offset;
	}
};

extern Signature g_Sig;