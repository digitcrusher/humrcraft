/*
 * karolslib.cpp
 * humrcraft Source Code
 * Available on Github
 *
 * Copyright (c) 2017 Karol "digitcrusher" Łacina
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#include <stdlib.h>
#include <src/utils/module.h>
#include <src/window/terminal.h>
#include <src/karolslib.h>

#if !defined(_WIN32)
#ifdef main
#undef main
#endif
#define KL_main main
#endif
int KL_main(int argc, char** argv) {
    return KL_user_main(argc, argv);
}
#if defined(_WIN32)
#ifdef WinMain
#undef WinMain
#endif
#define KL_WinMain WinMain
static void KL_unEscapeQuotes(char *arg) {
	char *last = NULL;
	while(*arg) {
		if(*arg == '"' && *last == '\\') {
			char *c_curr = arg;
			char *c_last = last;

			while(*c_curr) {
				*c_last = *c_curr;
				c_last = c_curr;
				c_curr++;
			}
			*c_last = '\0';
		}
		last = arg;
		arg++;
	}
}
static int KL_parseCommandLine(char *cmdline, char **argv) {
	char *bufp;
	char *lastp = NULL;
	int argc, last_argc;
	argc = last_argc = 0;
	for(bufp = cmdline; *bufp;) {
		/* Skip leading whitespace */
		while(*bufp == ' ') {
			++bufp;
		}
		/* Skip over argument */
		if(*bufp == '"') {
			++bufp;
			if(*bufp) {
				if(argv) {
					argv[argc] = bufp;
				}
				++argc;
			}
			/* Skip over word */
			while(*bufp && (*bufp != '"' || (lastp && *lastp == '\\'))) {
				lastp = bufp;
				++bufp;
			}
		}else {
			if(*bufp) {
				if(argv) {
					argv[argc] = bufp;
				}
				++argc;
			}
			/* Skip over word */
			while(*bufp && *bufp != ' ') {
				++bufp;
			}
		}
		if(*bufp) {
			if(argv) {
				*bufp = '\0';
			}
			++bufp;
		}
		/* Strip out \ from \" sequences */
		if(argv && last_argc != argc) {
			KL_unEscapeQuotes(argv[last_argc]);
		}
		last_argc = argc;
	}
	if(argv) {
		argv[argc] = NULL;
	}
	return argc;
}
HINSTANCE KL_hInstance;
HINSTANCE KL_hPrevInstance;
PSTR KL_szCmdLine;
int KL_iCmdShow;
int WINAPI KL_WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow) {
    KL_hInstance = hInstance;
    KL_hPrevInstance = hPrevInstance;
    KL_szCmdLine = szCmdLine;
    KL_iCmdShow = iCmdShow;
    char** argv;
    int argc;
	argc = KL_parseCommandLine(szCmdLine, NULL);
	argv = (char**)malloc(sizeof(char*)*(strlen(szCmdLine)+1));
	if(argv == NULL) {
		return 0;
	}
	KL_parseCommandLine(szCmdLine, argv);
    return KL_main(argc, argv);
}
#endif

bool KL_init() {
    //Set up stdmodbrd.
    KL_stdmodbrd = KL_createModBoard();
    //Set up stdterm.
    if(!(KL_stdterm = KL_createTerminal(TERMINAL_DEFAULT_BUFF_WIDTH, TERMINAL_DEFAULT_BUFF_HEIGHT, TERMINAL_DEFAULT_FLAGS, NULL, NULL))) {
        return 1;
    }
    //Create module main and add it to the global modboard modbrd.
    KL_addMod(*KL_createModule("main", NULL), KL_stdmodbrd);
    //Create routine main and add it to module main from modbrd.
    KL_addRtn(*KL_createRoutine("main", (void (*)())NULL), KL_getMod("main", KL_stdmodbrd));
    /*
    //Create init.
    KL_addRtn(*KL_createRoutine("init", (void (*)())NULL), KL_getMod("main", KL_stdmodbrd));
    //Create deinit.
    KL_addRtn(*KL_createRoutine("deinit", (void (*)())NULL), KL_getMod("main", KL_stdmodbrd));
    //Iterate over the modules from modbrd.
    for(int i=0; i<KL_stdmodbrd->size; i++) {
        KL_routine* rtn;
        //Rtn has init? If yes call it.
        if((rtn = KL_getRtn("init", KL_getMod(i, KL_stdmodbrd)))) {
            if(rtn->func != NULL) {
                rtn->func();
            }
        }
    }
    */
    return 0;
}
void KL_deinit() {
    /*
    //Iterate over the modules from modbrd.
    for(int i=0; i<KL_stdmodbrd->size; i++) {
        KL_routine* rtn;
        //Rtn has deinit? If yes call it.
        if((rtn = KL_getRtn("deinit", KL_getMod(i, KL_stdmodbrd)))) {
            if(rtn->func != NULL) {
                rtn->func();
            }
        }
    }
    */
    //Delete stdmodbrd.
    KL_destroyModBoard(KL_stdmodbrd);
    //Delete stdterm.
    KL_destroyTerminal(KL_stdterm);
}
