/****************************************************************************
 * apps/ame/ame-logo/ame-logo.c
 *
 * Ame NuttX — ASCII logo with ANSI 256-color gradient
 *
 ****************************************************************************/

#include <nuttx/config.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

/* ANSI 256-color foreground */
#define C(n) "\x1b[38;5;" n "m"
#define R   "\x1b[0m"

/* Fastfetch variant (11 lines) */
static const char *g_logo_ff[] = {
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

/* Banner variant (17 lines) */
static const char *g_logo_banner[] = {
    "        /\\",
    "       /++\\",
    "      /++++\\",
    "     /++++++\\",
    "    /++++++++\\",
    "   /++++++++++\\",
    "  /++++++++++++\\",
    " /++++++++++++++\\",
    "/++++++++++++++++\\",
    " \\++++++++++++++/",
    "  \\++++++++++++/",
    "   \\++++++++++/",
    "    \\++++++++/",
    "     \\++++++/",
    "      \\++++/",
    "       \\++/",
    "        \\/",
};

/* Top-to-bottom color gradient (pale -> deep -> pale) */
static const char *g_colors_ff[] = {
    "195", "159", "153", "117", "111", "81", "75",
    "81", "111", "117", "153", "159", "195",
};

static const char *g_colors_banner[] = {
    "195", "159", "153", "117", "111", "81", "75", "69", "63",
    "69", "75", "81", "111", "117", "153", "159", "195",
};

static void print_logo(const char **lines, const char **colors, int n)
{
    int i;
    for (i = 0; i < n; i++) {
        printf("\x1b[38;5;%sm%s\x1b[0m\n", colors[i], lines[i]);
    }
}

int main(int argc, char *argv[])
{
    int banner = 0;
    if (argc > 1 && strcmp(argv[1], "--banner") == 0) {
        banner = 1;
    }

    if (banner) {
        print_logo(g_logo_banner, g_colors_banner, 17);
    } else {
        print_logo(g_logo_ff, g_colors_ff, 13);
    }
    return 0;
}
