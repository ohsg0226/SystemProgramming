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

int CmdProcessing(void);
TCHAR* StrLower(TCHAR*);

int main(int agrc, TCHAR *argv[])
{
    _tsetlocale(LC_ALL, _T("Korean"));

    DWORD isExit;
    while (1) {
        isExit = CmdProcessing();
        if (isExit == TRUE) {
            _fputts(_T("명령어 처리를 종료합니다. \n"), stdout);
            break;
        }
    }
    return 0;
}

TCHAR cmdString[STR_LEN];
TCHAR cmdTokenList[CMD_TOKEN_NUM][STR_LEN];
TCHAR seps[] = _T(" ,\t\n");
TCHAR* next_token = NULL;

int CmdProcessing(void) {
    _fputts(_T("Best command prompt>>"), stdout);
    _getts_s(cmdString);

    TCHAR* token = _tcstok_s(cmdString, seps, &next_token);
    int tokenNum = 0;
    while (token != NULL) {
        _tcscpy_s(cmdTokenList[tokenNum++], StrLower(token));
        token = _tcstok_s(NULL, seps, &next_token);;
    }

    if (!_tcscmp(cmdTokenList[0], _T("exit"))) {
        return TRUE;
    }
    else if (!_tcscmp(cmdTokenList[0], _T("instruction1"))) {

    }
    else if (!_tcscmp(cmdTokenList[0], _T("instruction2"))) {
    }
    else {
        STARTUPINFO si = { 0, };
        PROCESS_INFORMATION pi;
        
        si.cb = sizeof(si);
        BOOL isRun = CreateProcess(NULL, cmdTokenList[0], NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi);
        
        if (isRun == FALSE) {
            _tprintf(ERROR_CMD, cmdTokenList[0]);
        }
    }
    return 0;
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
