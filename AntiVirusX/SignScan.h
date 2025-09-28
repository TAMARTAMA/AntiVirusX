#pragma once
#ifndef SIGN_SCAN_H
#define SIGN_SCAN_H
#include <iostream>
#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#undef byte
#include <softpub.h>
#include <wintrust.h>
#include <tchar.h>
#include <vector>


enum StateVerifySignature
{
	NotVerifySignature = 0,
	VerifySignature_ = 1,
	UNSUCCESSFULVerifySignature = 2
};
TCHAR* wstringToTCHAR(const std::wstring& wstr);

StateVerifySignature VerifySignature(const std::wstring& );
#endif //SIGN_SCAN_H