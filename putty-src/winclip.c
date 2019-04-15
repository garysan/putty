#include <stdio.h>
#include <string.h>
#include <conio.h>
#include <errno.h>
#include <windows.h>
#include <malloc.h>
#include <stdlib.h>
#include <fcntl.h>
#include <io.h>
#include <wchar.h>
#define WIN32_LEAN_AND_MEAN

typedef struct tagOPTIONS {
    char szName[MAX_PATH];
} OPTIONS;

static  BOOL ParseCommandLine(char *file, OPTIONS *opt) {
    memset(opt, 0, sizeof(OPTIONS));
    if (!*(opt->szName))
		strcpy(opt->szName, file);

    return TRUE;
}
    
static  BOOL    ClipCopy(OPTIONS *opt) {
    HANDLE hd;
    LPSTR lp;
    FILE *fp;
    BOOL rv = FALSE;
    if (!OpenClipboard(NULL)) {
        return FALSE;
    }

    hd = GetClipboardData(CF_TEXT);
    if (!hd) {
        CloseClipboard();
        return FALSE;
    }

    if (*opt->szName) {
    	fp = fopen(opt->szName, "wb");
        if (!fp) {
            CloseClipboard();
            return FALSE;
        }
    } else
        fp = NULL;

    lp = (LPSTR)GlobalLock(hd);
    if (lp) {
        if (fp)
            fputs(lp, fp);
            
         GlobalUnlock(hd);
         rv = TRUE;			// flag success
    } 
    if (fp)
        fclose(fp);
	EmptyClipboard();
    CloseClipboard();

    return rv;
}

int garysan(char *file) {
	
    OPTIONS opt;

    return ParseCommandLine(file, &opt) && ClipCopy(&opt) ? 1 : 0;
}