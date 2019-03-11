#ifndef TGRAPHICS_ESC_DEFINES_HPP_
#define TGRAPHICS_ESC_DEFINES_HPP_

#include <stdio.h>

/* Foreground colors */

#define FGCL_BLACK (30)
#define FGCL_RED (31)
#define FGCL_GREEN (32)
#define FGCL_WHITE (37)

#define FGCL_RESET (39)

/* Background colors */

#define BGCL_BLACK (40)
#define BGCL_RED (41)
#define BGCL_GREEN (42)
#define BGCL_WHITE (47)

#define BGCL_RESET (49)

/* ESC macroses */

#define ESC "\033"

#define clear_screen() printf(ESC "[2J")
#define clear_line() printf(ESC "[2K")

#define set_cursor(x, y) printf(ESC "[%d;%dH", (y), (x))
#define reset_cursor() printf(ESC "[H")

#define set_color(color) printf(ESC "[%dm", (color))

#endif // TGRAPHICS_ESC_DEFINES_HPP_
