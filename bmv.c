/*
 * Copyright (c) 2014, Daniel Bolgheroni. All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 * 
 *   1. Redistributions of source code must retain the above copyright
 *      notice, this list of conditions and the following disclaimer.
 *
 *   2. Redistributions in binary form must reproduce the above copyright
 *      notice, this list of conditions and the following disclaimer in
 *      the documentation and/or other materials provided with the
 *      distribution.
 * 
 * THIS SOFTWARE IS PROVIDED BY DANIEL BOLGHERONI ''AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL DANIEL BOLGHERONI OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define MAXFNSIZE 256

void usage(void);

int main(int argc, char **argv) {
    int ch, opt_d, opt_o, opt_p, offset, padding;
    char *old, *pattern, *ext;
    char new[MAXFNSIZE];

    opt_d = opt_o = opt_p = 0;

    while ((ch = getopt(argc, argv, "do:p:")) != -1) {
        switch (ch) {
            case 'd':
                opt_d = 1;
                break;
            case 'o':
                opt_o = 1;
                offset = atoi(optarg);
                break;
            case 'p':
                opt_p = 1;
                padding = atoi(optarg);
                break;
            default:
                usage();
                /* NOTREACHED */
        }
    }
    argc -= optind;
    argv += optind;

    /* TODO: resolve when no file globbing succeeds */

    /* minimum requirements */
    if (argc < 3)
        usage();

    /* default value for -o */
    if (!opt_o || offset == 0)
        offset = 1;

    /* default value for -p */
    if (!opt_p)
        padding = 2;

    /* first arg after options is the pattern */
    if (*argv) {
        pattern = *argv;
        argv++;
    }

    while (*argv) {
        old = *argv;

        /* extract extension */
        while (**argv) {
            if (**argv == '.') {
                ext = &(**argv);
            }

            (*argv)++;
        }
        ++ext;

        /* padding */
        switch(padding) {
            case 1:
                snprintf(new, MAXFNSIZE, "%s%01d.%s", pattern, offset, ext);
                break;
            case 2:
                snprintf(new, MAXFNSIZE, "%s%02d.%s", pattern, offset, ext);
                break;
            case 3:
                snprintf(new, MAXFNSIZE, "%s%03d.%s", pattern, offset, ext);
                break;
            case 4:
                snprintf(new, MAXFNSIZE, "%s%04d.%s", pattern, offset, ext);
                break;
            case 5:
                snprintf(new, MAXFNSIZE, "%s%05d.%s", pattern, offset, ext);
                break;
            case 6:
                snprintf(new, MAXFNSIZE, "%s%06d.%s", pattern, offset, ext);
                break;
            case 7:
                snprintf(new, MAXFNSIZE, "%s%07d.%s", pattern, offset, ext);
                break;
            case 8:
                snprintf(new, MAXFNSIZE, "%s%08d.%s", pattern, offset, ext);
                break;
            default:
                snprintf(new, MAXFNSIZE, "%s%02d.%s", pattern, offset, ext);
                break;
        }

        /* -d dummy mode option */
        if (!opt_d) {
            fprintf(stdout, "%s -> %s\n", old, new);

            if (rename(old, new)) {
                fprintf(stderr, "error renaming %s\n", old);
                exit(1);
            }
        } else {
            printf("%s -> %s (dummy)\n", old, new);
        }

        argv++; offset++;
    }

    return 0;
}

void usage(void) {
    fprintf(stderr, "usage: bmv [-d] [-o offset] [-p padding] pattern file ...\n");
}
