/* See LICENSE file for copyright and license details. */

#include <X11/XF86keysym.h>

/* appearance */
static const unsigned int borderpx = 2; /* border pixel of windows */
static const unsigned int snap = 32;		/* snap pixel */
static const int showbar = 1;						/* 0 means no bar */
static const int topbar = 1;						/* 0 means bottom bar */
static const char *fonts[] = {"JetBrainsMono Nerd Font:size=11"};
static const char dmenufont[] = "JetBrainsMono Nerd Font:size=11";
static const char col_gray1[] = "#434c5e";
static const char col_gray2[] = "#3b4252";
static const char col_gray3[] = "#eceff4";
static const char col_gray4[] = "#eeeeee";
static const char col_purple[] = "#b48ead";
static const char *colors[][3] = {
		/*               fg         bg         border   */
		[SchemeNorm] = {col_gray3, col_gray1, col_gray2},
		[SchemeSel] = {col_gray3, col_gray1, col_purple},
};

/* tagging */
static const char *tags[] = {"1", "2", "3", "4", "5", "6", "7", "8", "9"};

static const char ptagf[] = "[%s|%s]"; /* format of a tag label */
static const char etagf[] = "[%s]";		 /* format of an empty tag */
static const int lcaselbl = 1;				 /* 1 means make tag label lowercase */
static const unsigned int ptagl = 20;	 /* max length of a tag */

static const unsigned int ulinepad = 5;			/* horizontal padding between the underline and tag */
static const unsigned int ulinestroke = 2;	/* thickness / height of the underline */
static const unsigned int ulinevoffset = 1; /* how far above the bottom of the bar the line should appear */
static const int ulineall = 0;							/* 1 to show underline on all tags, 0 for just the active ones */

static const Rule rules[] = {
		/* xprop(1):
		 *	WM_CLASS(STRING) = instance, class
		 *	WM_NAME(STRING) = title
		 */
		/* class      instance    title       tags mask     isfloating   monitor */
		{"Gimp", NULL, NULL, 0, 1, -1},
		{"Firefox", NULL, NULL, 1 << 8, 0, -1},
};

/* layout(s) */
static const float mfact = 0.55;		 /* factor of master area size [0.05..0.95] */
static const int nmaster = 1;				 /* number of clients in master area */
static const int resizehints = 1;		 /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

static const Layout layouts[] = {
		/* symbol     arrange function */
		{"[]=", tile}, /* first entry is default */
		{"><>", NULL}, /* no layout function means floating behavior */
		{"[M]", monocle},
};

/* key definitions */
#define MONKEY Mod4Mask
#define TAGKEYS(KEY, TAG)                                        \
	{MONKEY, KEY, view, {.ui = 1 << TAG}},                         \
			{MONKEY | ControlMask, KEY, toggleview, {.ui = 1 << TAG}}, \
			{MONKEY | ShiftMask, KEY, tag, {.ui = 1 << TAG}},          \
			{MONKEY | ControlMask | ShiftMask, KEY, toggletag, {.ui = 1 << TAG}},

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd)                                       \
	{                                                      \
		.v = (const char *[]) { "/bin/sh", "-c", cmd, NULL } \
	}

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = {"dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_purple, "-sf", col_gray4, NULL};
static const char *termcmd[] = {"st", NULL};

static Key keys[] = {
		/* modifier                     key        function        argument */
		{MONKEY, XK_p, spawn, {.v = dmenucmd}},
		{MONKEY | ShiftMask, XK_Return, spawn, {.v = termcmd}},
		{MONKEY, XK_b, togglebar, {0}},
		{MONKEY, XK_j, focusstack, {.i = +1}},
		{MONKEY, XK_k, focusstack, {.i = -1}},
		{MONKEY, XK_i, incnmaster, {.i = +1}},
		{MONKEY, XK_d, incnmaster, {.i = -1}},
		{MONKEY, XK_h, setmfact, {.f = -0.05}},
		{MONKEY, XK_l, setmfact, {.f = +0.05}},
		{MONKEY, XK_Return, zoom, {0}},
		{MONKEY, XK_Tab, view, {0}},
		{MONKEY | ShiftMask, XK_c, killclient, {0}},
		{MONKEY, XK_t, setlayout, {.v = &layouts[0]}},
		{MONKEY, XK_f, setlayout, {.v = &layouts[1]}},
		{MONKEY, XK_m, setlayout, {.v = &layouts[2]}},
		{MONKEY, XK_space, setlayout, {0}},
		{MONKEY | ShiftMask, XK_space, togglefloating, {0}},
		{MONKEY, XK_0, view, {.ui = ~0}},
		{MONKEY | ShiftMask, XK_0, tag, {.ui = ~0}},
		{MONKEY, XK_comma, focusmon, {.i = -1}},
		{MONKEY, XK_period, focusmon, {.i = +1}},
		{MONKEY | ShiftMask, XK_comma, tagmon, {.i = -1}},
		{MONKEY | ShiftMask, XK_period, tagmon, {.i = +1}},
		TAGKEYS(XK_1, 0)
				TAGKEYS(XK_2, 1)
						TAGKEYS(XK_3, 2)
								TAGKEYS(XK_4, 3)
										TAGKEYS(XK_5, 4)
												TAGKEYS(XK_6, 5)
														TAGKEYS(XK_7, 6)
																TAGKEYS(XK_8, 7)
																		TAGKEYS(XK_9, 8)
		//{ MONKEY|ShiftMask,             XK_q,      quit,           {0} },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
		/* click                event mask      button          function        argument */
		{ClkLtSymbol, 0, Button1, setlayout, {0}},
		{ClkLtSymbol, 0, Button3, setlayout, {.v = &layouts[2]}},
		{ClkWinTitle, 0, Button2, zoom, {0}},
		{ClkStatusText, 0, Button2, spawn, {.v = termcmd}},
		{ClkClientWin, MONKEY, Button1, movemouse, {0}},
		{ClkClientWin, MONKEY, Button2, togglefloating, {0}},
		{ClkClientWin, MONKEY, Button3, resizemouse, {0}},
		{ClkTagBar, 0, Button1, view, {0}},
		{ClkTagBar, 0, Button3, toggleview, {0}},
		{ClkTagBar, MONKEY, Button1, tag, {0}},
		{ClkTagBar, MONKEY, Button3, toggletag, {0}},
};
