/* Constants */
#define TERMINAL "st"
#define TERMCLASS "St"
#include <X11/XF86keysym.h>

/* appearance */
static unsigned int borderpx			= 0;		// border pixel of windows
static const int startwithgaps[]		= { 1 };	// 1 means gaps are used by default, this can be customized for each tag
static const unsigned int gappx[]		= { 10 };	// default gap between windows in pixels, this can be customized for each tag
static unsigned int snap				= 32;		// snap pixel
static int showbar						= 1;		// 0 means no bar
static int topbar						= 1;		// 0 means bottom bar
static char font[]						= "monospace:size=10";
static char dmenufont[]					= "monospace:size=10";
static const char *fonts[]				= { 
	"Hack:style=Regular:pixelsize=14",
};
static char normbgcolor[]				= "#222222";
static char normbordercolor[]			= "#444444";
static char normfgcolor[]				= "#bbbbbb";
static char selfgcolor[]				= "#eeeeee";
static char selbordercolor[]			= "#005577";
static char selbgcolor[]				= "#005577";
static const unsigned int baralpha		= 100;
static const unsigned int borderalpha	= OPAQUE;
static char *colors[][3] = {
//						fg				bg				border
	[SchemeNorm] = { 	normfgcolor,	normbgcolor,	normbordercolor	},
	[SchemeSel]  = {	selfgcolor,		selbgcolor,		selbordercolor	},
};
static const unsigned int alphas[][3] = {
//						fg				bg				border
	[SchemeNorm] = {	OPAQUE,			baralpha,		borderalpha		},
	[SchemeSel]  = {	OPAQUE,			baralpha,		borderalpha		},
};

static const char *upvol[]		= { "upvol",   NULL, NULL, NULL , NULL };
static const char *downvol[]	= { "downvol", NULL, NULL, NULL , NULL };
static const char *mutevol[]	= { "mutevol", NULL, NULL, NULL , NULL };

/* tagging */
static const char *tags[] = { 
	"\uf120", // Terminal
	"🌎", // Web
	"", // file browser 
	"\uf001", // Music
	"", // calendar
	"", // code 
	"", // rss 
	"", // settings 
	"" // overflow
};

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
//		class			instance	title		tags mask	isfloating	monitor
	{	"St",			NULL,		NULL,		0,			0,			-1	},
	{	"qutebrowser",	NULL,		NULL,		1 << 1,		0,			-1	},
	{	"St",			"st",		"lf",		1 << 2,		0,			-1	},
	{	"St",			"st",	"ncmpcpp",		1 << 3,		0,			-1	},
};

/* layout(s) */
static float mfact				= 0.55;	// factor of master area size [0.05..0.95]
static int nmaster     			= 1;	// number of clients in master area
static int resizehints 			= 0;	// 1 means respect size hints in tiled resizals
static const int lockfullscreen = 1; // 1 will force focus on the fullscreen window

static const Layout layouts[] = {
//	symbol		arrange function
	{ "[]=",	tile },	// first entry is default
	{ "><>",	NULL },	// no layout function means floating behavior
	{ "[M]",	monocle },
};

/* key definitions */
#define MODKEY Mod4Mask // Set modifier key (1=alt,4=super)
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

#define STATUSBAR "dwmblocks"

/* commands */
static char dmenumon[2] = "0"; // component of dmenucmd, manipulated in spawn()
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", normbgcolor, "-nf", normfgcolor, "-sb", selbordercolor, "-sf", normbordercolor, NULL };
static const char *termcmd[]  = { "st", NULL };

/* Xresources preferences to load at startup */
ResourcePref resources[] = {
	{ "font",			STRING,  &font				},
	{ "dmenufont",		STRING,  &dmenufont			},
	{ "color0",			STRING,  &normbgcolor		},
	{ "color0",			STRING,  &normbordercolor	},
	{ "color4",			STRING,  &normfgcolor		},
	{ "color4",			STRING,  &selbgcolor		},
	{ "color8",			STRING,  &selbordercolor	},
	{ "color0",			STRING,  &selfgcolor		},
	{ "borderpx",		INTEGER, &borderpx			},
	{ "snap",			INTEGER, &snap				},
	{ "showbar",		INTEGER, &showbar			},
	{ "topbar",			INTEGER, &topbar			},
	{ "nmaster",		INTEGER, &nmaster			},
	{ "resizehints",	INTEGER, &resizehints		},
	{ "mfact",			FLOAT,   &mfact				},
};

static Key keys[] = {
//	modifier			key					function		argument							~ BASICS ~
	{ MODKEY,			XK_d,				spawn,			{.v = dmenucmd }				},	// dmenu
	{ MODKEY,			XK_Return,			spawn,			{.v = termcmd }					}, 	//spawn terminal
	{ MODKEY,			XK_q,				killclient,		{0}								}, 	// close window
	{ MODKEY,			XK_b,				togglebar,		{0}								}, 	// show/hide statusbar
	{ MODKEY|ShiftMask,	XK_t,				spawn,			SHCMD("opacity-toggle")			}, 	// toggle transparency
																								/* NAVIGATION */
	{ MODKEY,			XK_j,				focusstack,		{.i = +1 }						},
	{ MODKEY,			XK_k,				focusstack,		{.i = -1 }						},
	{ MODKEY,			XK_i,				incnmaster,		{.i = +1 }						},
	{ MODKEY,			XK_u,				incnmaster,		{.i = -1 }						},
	{ MODKEY,			XK_h,				setmfact,		{.f = -0.05}					},
	{ MODKEY,			XK_l,				setmfact,		{.f = +0.05}					},
	{ MODKEY,			XK_space,			zoom,			{0}								},
	{ MODKEY,			XK_Tab,				view,			{0}								},
																								/* PROGRAM HOTKEYS */
	{ MODKEY,			XK_w,				spawn,			SHCMD("$BROWSER")				},	// browser
	{ MODKEY,			XK_asciitilde,		spawn,			SHCMD("icons")					},	// icon menu, copy icon	
	{ MODKEY|ShiftMask,	XK_asciitilde,		spawn,			SHCMD("icons -u")				},	// icon menu, copy unicode
	{ MODKEY,			XK_e,				spawn,			SHCMD(TERMINAL " -e $FILE")		},	// file browser
	{ MODKEY|ShiftMask,	XK_y,				spawn,			SHCMD("youtube")				},	// terminal youtube player
	{ MODKEY|ShiftMask,	XK_m,				spawn,			SHCMD(TERMINAL " -e ncmpcpp")	},	// music player
	{ MODKEY,			XK_s,				spawn,			SHCMD("import -window root cap_$(date '+%s').png")	},	// screencap fullscreen
	{ MODKEY|ShiftMask,	XK_s,				spawn,			SHCMD("import cap_$(date '+%s').png")	},	// screencap selection
																								/* MUSIC */
	{ MODKEY,			XK_p,				spawn,			SHCMD("mpc toggle")				},	// play/pause
	{ MODKEY,			XK_bracketright,	spawn,			SHCMD("mpc next")				},	// next track
	{ MODKEY,			XK_bracketleft,		spawn,			SHCMD("mpc prev")				},	// prev track
	{ MODKEY|ShiftMask,	XK_bracketleft,		spawn,			SHCMD("mpc seek +10")			},	// seek forward
	{ MODKEY|ShiftMask,	XK_bracketleft,		spawn,			SHCMD("mpc seek -10")			},	// seek backward
																								/* LAYOUTS */
	{ MODKEY,			XK_t,				setlayout,		{.v = &layouts[0]}				},	// tiling
	{ MODKEY|ShiftMask,	XK_f,				setlayout,		{.v = &layouts[1]}				},	// floating
	{ MODKEY,			XK_f,				fullscreen,		{0}								},	// fullscreen
	{ MODKEY|ShiftMask,	XK_space,			togglefloating,	{0}								},
	{ MODKEY,			XK_0,				view,			{.ui = ~0 }						},
	{ MODKEY|ShiftMask,	XK_0,				tag,			{.ui = ~0 }						},
	{ MODKEY,			XK_comma,			focusmon,		{.i = -1 }						},
	{ MODKEY,			XK_period,			focusmon,		{.i = +1 }						},
	{ MODKEY|ShiftMask,	XK_comma,			tagmon,			{.i = -1 }						},
	{ MODKEY|ShiftMask,	XK_period,			tagmon,			{.i = +1 }						},
	{ MODKEY,			XK_minus,			setgaps,		{.i = -5 }						},	// gaps
	{ MODKEY,			XK_equal,			setgaps,		{.i = +5 }						},
	{ MODKEY|ShiftMask,	XK_equal,			setgaps,		{.i = GAP_RESET }				},
	{ MODKEY,			XK_a,				setgaps,		{.i = GAP_TOGGLE}				},
	{ MODKEY,			XK_BackSpace,		spawn,			SHCMD("syscontrol")				},	// logout, reboot, etc.
	{ MODKEY|ShiftMask,	XK_q,				quit,			{0}								},	// refreshdwm
	{ 0,				XF86XK_AudioLowerVolume,	spawn,	{.v = downvol}					},
	{ 0,				XF86XK_AudioRaiseVolume,	spawn,	{.v = upvol}		  			},
//	{ 0,				XF86XK_AudioMute,			spawn,	{.v = mutevol}					},
	{ MODKEY,			XK_m,			spawn,	{.v = mutevol}					},
	{ 0,				XF86XK_AudioNext,			spawn,	SHCMD("mpc next")				},
	{ 0,				XF86XK_AudioPrev,			spawn,	SHCMD("mpc prev")				},
	{ 0,				XF86XK_AudioPlay,			spawn,	SHCMD("mpc toggle")				},
	{ 0,				XF86XK_AudioStop,			spawn,	SHCMD("mpc stop")				},
//																								~ Tags ~
	TAGKEYS(			XK_1,				0)
	TAGKEYS(			XK_2,				1)
	TAGKEYS(			XK_3,				2)
	TAGKEYS(			XK_4,				3)
	TAGKEYS(			XK_5,				4)
	TAGKEYS(			XK_6,				5)
	TAGKEYS(			XK_7,				6)
	TAGKEYS(			XK_8,				7)
	TAGKEYS(			XK_9,				8)
};

/* button definitions */
// click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin
static Button buttons[] = {
//	click						event mask	button		function		argument
	{ ClkLtSymbol,				0,			Button1,	setlayout,		{0}					},
	{ ClkLtSymbol,				0,			Button3,	setlayout,		{.v = &layouts[2]}	},
	{ ClkWinTitle,				0,			Button2,	zoom,			{0}					},
	{ ClkStatusText,	        0,			Button1,	sigstatusbar,	{.i = 1}			},
	{ ClkStatusText,	        0,			Button2,	sigstatusbar,	{.i = 2}			},
	{ ClkStatusText,	        0,			Button3,	sigstatusbar,	{.i = 3}			},
	{ ClkStatusText|ShiftMask,	0,			Button3,	sigstatusbar,	{.i = 4}			},
	{ ClkStatusText|ShiftMask,	0,			Button3,	sigstatusbar,	{.i = 5}			},
	{ ClkStatusText|ShiftMask,	0,			Button3,	sigstatusbar,	{.i = 6}			},
	{ ClkClientWin,				MODKEY,		Button1,	movemouse,		{0}					},
	{ ClkClientWin,				MODKEY,		Button2,	togglefloating,	{0}					},
	{ ClkClientWin,				MODKEY,		Button3,	resizemouse,	{0}					},
	{ ClkTagBar,				0,			Button1,	view,			{0}					},
	{ ClkTagBar,				0,			Button3,	toggleview,		{0}					},
	{ ClkTagBar,				MODKEY,		Button1,	tag,			{0}					},
	{ ClkTagBar,				MODKEY,		Button3,	toggletag,		{0}					},
};

