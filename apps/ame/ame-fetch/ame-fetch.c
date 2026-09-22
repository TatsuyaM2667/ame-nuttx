/****************************************************************************
 * apps/ame/ame-fetch/ame-fetch.c
 *
 * Ame NuttX — fastfetch-style system info with logo
 *
 ****************************************************************************/

#include <nuttx/config.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/utsname.h>
#include <malloc.h>

/* ─── Logo (top to bottom, pale → deep → pale) ─── */

static const char *g_logo[] = {
    "      /\\",
    "     /++\\",
    "    /++++\\",
    "   /++++++\\",
    "  /++++++++\\",
    " /++++++++++\\",
    "/++++++++++++\\",
    " \\++++++++++/",
    "  \\++++++++/",
    "   \\++++++/",
    "    \\++++/",
    "     \\++/",
    "      \\/",
};

static const char *g_colors[] = {
    "195", "159", "153", "117", "111", "81", "75",
    "81", "111", "117", "153", "159", "195",
};

#define LOGO_N (sizeof(g_logo) / sizeof(g_logo[0]))
#define LOGO_W 16
#define INFO_N 16

int main(int argc, char *argv[])
{
    struct utsname un;
    static char hostname[64] = "ame";
    static char info[INFO_N][160];
    int n = 0;
    int i;
    int total;

    uname(&un);
#ifdef CONFIG_NET
    if (gethostname(hostname, sizeof(hostname)) < 0)
      {
        strncpy(hostname, "ame", sizeof(hostname) - 1);
      }
#endif

    /* ─── Build info lines ─── */

    snprintf(info[n++], sizeof(info[0]),
             "\x1b[1;36m%s@%s\x1b[0m", "root", hostname);
    snprintf(info[n++], sizeof(info[0]),
             "\x1b[36m-----------------------\x1b[0m");

    snprintf(info[n++], sizeof(info[0]),
             "\x1b[1;33mOS:\x1b[0m       Ame NuttX");

    snprintf(info[n++], sizeof(info[0]),
             "\x1b[1;33mKernel:\x1b[0m   %s %s",
             un.sysname, un.release);

    snprintf(info[n++], sizeof(info[0]),
             "\x1b[1;33mHost:\x1b[0m     %s", CONFIG_ARCH_BOARD);

    snprintf(info[n++], sizeof(info[0]),
             "\x1b[1;33mChip:\x1b[0m     %s", CONFIG_ARCH_CHIP);

    snprintf(info[n++], sizeof(info[0]),
             "\x1b[1;33mMachine:\x1b[0m  %s", un.machine);

    snprintf(info[n++], sizeof(info[0]),
             "\x1b[1;33mBuild:\x1b[0m    %s", un.version);

    /* Uptime */
    {
        long ticks = (long)clock();
        long secs = ticks / CLOCKS_PER_SEC;
        int hh = (int)(secs / 3600);
        int mm = (int)((secs % 3600) / 60);
        int ss = (int)(secs % 60);
        snprintf(info[n++], sizeof(info[0]),
                 "\x1b[1;33mUptime:\x1b[0m   %dh %dm %ds", hh, mm, ss);
    }

    /* Heap */
    {
        struct mallinfo mi = mallinfo();
        snprintf(info[n++], sizeof(info[0]),
                 "\x1b[1;33mHeap:\x1b[0m     %d KiB / %d KiB",
                 mi.uordblks / 1024, mi.arena / 1024);
    }

    /* CPU clock (if available) */
#ifdef CONFIG_SCHED_CPULOAD
    snprintf(info[n++], sizeof(info[0]),
             "\x1b[1;33mBoard:\x1b[0m    %s", CONFIG_ARCH_BOARD);
#endif

    /* ─── Print side by side ─── */

    total = LOGO_N > n ? LOGO_N : n;

    for (i = 0; i < total; i++)
      {
        if (i < LOGO_N)
          {
            printf("\x1b[38;5;%sm%-*s\x1b[0m  ",
                   g_colors[i], LOGO_W, g_logo[i]);
          }
        else
          {
            printf("%-*s  ", LOGO_W, "");
          }

        if (i < n)
          {
            printf("%s", info[i]);
          }

        printf("\n");
      }

    return 0;
}
