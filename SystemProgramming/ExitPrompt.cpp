// ExitPrompt.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <stdio.h>
#include <stdlib.h>
#include <tchar.h>
#include <locale.h>
#include <Windows.h>

#define STR_LEN 256
#define CMD_TOKEN_NUM 10

TCHAR ERROR_CMD[] = _T("'%s' is not executable program. \n");

int CmdProcessing(int);
int CmdReadTokenize(void);
TCHAR* StrLower(TCHAR*);
TCHAR cmdString[STR_LEN];
TCHAR cmdTokenList[CMD_TOKEN_NUM][STR_LEN];
TCHAR seps[] = _T(" ,\t\n");
TCHAR* next_token = NULL;

int _tmain(int argc, TCHAR* argv[])
{
    _tsetlocale(LC_ALL, _T("Korean"));

    if (argc > 2) {
        for (int i = 1; i < argc; i++)
            _tcscpy_s(cmdTokenList[i - 1], argv[i]);

        CmdProcessing(argc - 1);
    }

    DWORD isExit = NULL;
    while (1) {
        int tokenNum = CmdReadTokenize();
        if (tokenNum == 0)
            continue;

        isExit = CmdProcessing(tokenNum);
        if (isExit == TRUE)
        {
            _fputts(_T("명령어 처리를 종료합니다. \n"), stdout);
            break;
        }
    }
    return 0;
}

int CmdProcessing(int tokenNum)
{
    BOOL isRun;
    STARTUPINFO si = { 0, };
    PROCESS_INFORMATION pi;

    TCHAR cmdStringWithOptions[STR_LEN] = { 0, };
    TCHAR optString[STR_LEN] = { 0, };

    si.cb = sizeof(si);
    if (!_tcscmp(cmdTokenList[0], _T("exit")))
    {
        return TRUE;
    }
    else if (!_tcscmp(cmdTokenList[0], _T("start"))) {
        if (tokenNum > 1) {
            for (int i = 1; i < tokenNum; i++)
                _stprintf_s(optString, _T("%s %s"), optString, cmdTokenList[i]);

            _stprintf_s(cmdStringWithOptions, _T("%s %s"), _T("Test.exe"), optString);
        }
        else
            _stprintf_s(cmdStringWithOptions, _T("%s"), _T("Test.exe"));

        isRun = CreateProcess(NULL, cmdStringWithOptions, NULL, NULL, TRUE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi);

        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
    }
    else if (!_tcscmp(cmdTokenList[0], _T("echo")))
    {
        for (int i = 1; i < tokenNum; i++)
            _stprintf_s(optString, _T("%s %s"), optString, cmdTokenList[i]);

        _tprintf(_T("echo message:%s \n"), optString);
    }
    else
    {
        _tcscpy_s(cmdStringWithOptions, cmdTokenList[0]);

        for (int i = 1; i < tokenNum; i++)
            _stprintf_s(cmdStringWithOptions, _T("%s %s"), cmdStringWithOptions, cmdTokenList[i]);

        isRun = CreateProcess(NULL, cmdStringWithOptions, NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi);

        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);

        if (isRun == FALSE)
            _tprintf(ERROR_CMD, cmdTokenList[0]);
    }

    return 0;
}
  

int CmdReadTokenize(void)
{
    
    _fputts(_T("Best command prompt>> "), stdout);
    _getts_s(cmdString);

    TCHAR* token = _tcstok_s(cmdString, seps, &next_token);

    int tokenNum = 0;

    while (token != NULL)
    {
        _tcscpy_s(cmdTokenList[tokenNum++], StrLower(token));
        token = _tcstok_s(NULL, seps, &next_token);
    }

    return tokenNum;
}

TCHAR* StrLower(TCHAR* pStr) {
    TCHAR* ret = pStr;

    while (*pStr) {
        if (_istupper(*pStr)) {
            *pStr = _totlower(*pStr);
        }
        pStr++;
    }
    return ret;
}
