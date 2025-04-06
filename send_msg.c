/// @file
/// @brief
/// @copyright
///   2025 Mitsutoshi Nakano <ItSANgo@gmail.com>
///   SPDX-License-Identifier: AGPL-3.0
/// @version 0.1.0

#include <err.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <sysexits.h>

#include "ini.h"

#if __has_c_attribute(noreturn)
#define NORETURN [[noreturn]]
#else
#define NORETURN
#endif // __has_c_attribute(noreturn)

#define MSG_SIZE 1024

NORETURN static int
usage(int status, const char *progname)
{
    fprintf(
        status ? stderr : stdout,
        "Usage: %s [OPTIONS] [MESSAGE]\n"
        "Options:\n"
        "  -c, --conf FILE  Specify the configuration file\n"
        "  -h, --help       Show this help message\n"
        "  -V, --version    Show version information\n",
        progname
    );
    exit(status);
}

NORETURN int
main(int argc, char *argv[])
{
    static struct option long_options[] = {
        {"conf", required_argument, NULL, 'c'},
        {"help", no_argument, NULL, 'h'},
        {"version", no_argument, NULL, 'V'},
        {NULL, 0, NULL, 0}
    };
    const char *conf_file = "wait_msg.ini";
    for (int c;
         (c = getopt_long(argc, argv, "c:hV", long_options, NULL)) != -1;) {
        switch (c) {
        case 'c':
            conf_file = optarg;
            break;
        case 'h':
            usage(EX_OK, argv[0]);
        case 'V':
            printf("Version: 0.1.0-SNAPSHOT\n");
            exit(EX_OK);
        default:
            fprintf(stderr, "Unknown option: %c\n", c);
            usage(EX_USAGE, argv[0]);
        }
    }

    FILE *fp = fopen(conf_file, "r");
    if (fp == NULL) {
        err(EX_CONFIG, "the configuration file %s:", conf_file);
    }
    struct list *ini_list = load_ini(fp);
    if (ini_list == NULL) {
        err(EX_IOERR, "failed to load the configuration file");
    }
    if (fclose(fp)) {
        warn("failed to fclose the configuration file");
    }
    const char *key_str = get_value(ini_list, KEY);
    if (key_str == NULL) {
        errx(EX_CONFIG,
            KEY " was not found in the configuration file %s:", conf_file);
    }

    key_t msg_key = strtoll(key_str, NULL, 16);
    int msgqid = msgget(msg_key, IPC_CREAT | 0600);
    if (msgqid == -1) {
        err(EX_OSERR, "failed to msgget");
    }

    struct msgbuf {
        long mtype;
        char mtext[MSG_SIZE];
    } msg = {1, {0}};

    if (argv[optind]) {
        strncpy(msg.mtext, argv[optind], MSG_SIZE - 1);
    }
    size_t len = strlen(msg.mtext);

    if (msgsnd(msgqid, &msg, len, 0)) {
        err(EX_OSERR, "failed to msgsnd");
    }
    exit(EX_OK);
}
