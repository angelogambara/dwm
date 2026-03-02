/* See LICENSE file for copyright and license details. */

#define SESSION_FILE "/tmp/dwm-session"

/* appearance */
static const unsigned int borderpx  = 5;        /* border pixel of windows */
static const unsigned int gappx     = 0;        /* gaps between windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const int horizpadbar        = 5;        /* horizontal padding for statusbar */
static const int vertpadbar         = 10;       /* vertical padding for statusbar */
static const char *fonts[]          = {
	"Hack:pixelsize=14:antialias=true:autohint=false",
	"Font Awesome 7 Free:pixelsize=14:antialias=true",
};
static const char dmenufont[]       = "Hack:pixelsize=14:antialias=true:autohint=false";
static const char gray1[]           = "#1e2122";
static const char gray2[]           = "#928374";
static const char gray3[]           = "#A89984";
static const char gray4[]           = "#EBDBB2";
static const char yellow[]          = "#D79921";
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { gray3, gray1,  gray2  },
	[SchemeSel]  = { gray4, yellow, yellow },
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

/* Lockfile */
static char lockfile[] = "/tmp/dwm.lock";

static const unsigned int ulinepad      = 5;  /* horizontal padding between the underline and tag */
static const unsigned int ulinestroke   = 2;  /* thickness / height of the underline */
static const unsigned int ulinevoffset  = 0;  /* how far above the bottom of the bar the line should appear */
static const int ulineall               = 0;  /* 1 to show underline on all tags, 0 for just the active ones */

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   monitor */
	{ "Thunar",   NULL,       "Rename",   0,            1,           -1 },
	{ "Xarchiver", NULL,      NULL,       0,            1,           -1 },
	{ "mpv",      NULL,       NULL,       0,            1,           -1 },
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 0;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */
static const int refreshrate = 120;  /* refresh rate (per second) for client move/resize */

#include "horizgrid.c"
static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
	{ "###",      horizgrid },
};

/* key definitions */
#define MODKEY Mod4Mask
#define ALTKEY Mod1Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} }, \
	{ ALTKEY|ShiftMask,             KEY,      swaptags,       {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

#define STATUSBAR "dwmblocks"

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", gray1, "-nf", gray3, "-sb", yellow, "-sf", gray4, NULL };
static const char *termcmd[] = { "alacritty", NULL };
static const char *sxivcmd[] = { "sh", "-c", "sxiv -ro ~/gallery-dl", NULL };
static const char *benqcmd[] = { "sh", "-c", "sxiv -ro ~/gallery-dl -P ~/.local/share/color/icc/benq.icc", NULL };
static const char *flameshotcmd[] = { "flameshot",  "gui",  NULL };
static const char *notifexeccmd[] = { "dunstctl", "action", NULL };
static const char *notifkillcmd[] = { "dunstctl", "close-all", NULL };
static const char *togglemonitorcmd[] = { "toggle-monitor.sh", NULL };
static const char *suspendcmd[] = { "zzz", NULL };
static const char *micmutecmd[] = { "sh", "-c", "pactl set-source-mute @DEFAULT_SINK@ toggle; pkill -RTMIN+6 dwmblocks", NULL };
static const char *volupcmd[]   = { "sh", "-c", "pactl set-sink-volume @DEFAULT_SINK@ +5%; pkill -RTMIN+8 dwmblocks", NULL };
static const char *voldowncmd[] = { "sh", "-c", "pactl set-sink-volume @DEFAULT_SINK@ -5%; pkill -RTMIN+8 dwmblocks", NULL };
static const char *volmutecmd[] = { "sh", "-c", "pactl set-sink-mute @DEFAULT_SINK@ toggle; pkill -RTMIN+8 dwmblocks", NULL };
static const char *brightupcmd[]   = { "sh", "-c", "brightnessctl set 5%+; pkill -RTMIN+5 dwmblocks", NULL };
static const char *brightdowncmd[] = { "sh", "-c", "brightnessctl set 5%-; pkill -RTMIN+5 dwmblocks", NULL };
static const char *wallpapercmd[] = { "sh", "-c", "~/.local/bin/setbg  ~/.cache/wallpaper", NULL };
static const char *dwmblockscmd[] = { "sh", "-c", "killall dwmblocks; setsid -f dwmblocks", NULL };
static const char *browsercmd[] = { "firefox", NULL };
static const char *filemanagercmd[] = { "thunar", NULL };
static const char *kritacmd[] = { "krita", NULL };
static const char *prismlaunchercmd[] = { "prismlauncher", NULL };
static const char *steamcmd[] = { "steam", NULL };
static const char *virtmanagercmd[] = { "virt-manager", NULL };
static const char *xencelabscmd[] = { "gtk-launch", "xencelabs", NULL };

#include <X11/XF86keysym.h>
static const Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY|ShiftMask,             XK_Return, spawn,          {.v = dmenucmd } },
	{ MODKEY,                       XK_Return, spawn,          {.v = termcmd } },
	{ MODKEY,                       XK_b,      spawn,          {.v = browsercmd } },
	{ MODKEY,                       XK_e,      spawn,          {.v = filemanagercmd } },
	{ MODKEY,                       XK_s,      spawn,          {.v = sxivcmd } },
	{ MODKEY|ShiftMask,             XK_s,      spawn,          {.v = benqcmd } },
	{ MODKEY,                       XK_n,      spawn,          {.v = notifexeccmd } },
	{ MODKEY|ShiftMask,             XK_n,      spawn,          {.v = notifkillcmd } },
	{ MODKEY|ShiftMask,             XK_m,      spawn,          {.v = togglemonitorcmd } },
	{ MODKEY,                       XK_p,      spawn,          {.v = prismlaunchercmd } },
	{ MODKEY|ShiftMask,             XK_t,      spawn,          {.v = steamcmd } },
	{ MODKEY,                       XK_v,      spawn,          {.v = virtmanagercmd } },
	{ MODKEY|ShiftMask,             XK_z,      spawn,          {.v = suspendcmd } },
	{ MODKEY|ControlMask,           XK_k,      spawn,          {.v = kritacmd } },
	{ MODKEY|ControlMask,           XK_x,      spawn,          {.v = xencelabscmd } },
	{ MODKEY,                       XK_Print,  spawn,          {.v = flameshotcmd } },
	{ MODKEY|ControlMask,           XK_b,      spawn,          {.v = wallpapercmd } },
	{ MODKEY|ControlMask|ShiftMask, XK_b,      spawn,          {.v = dwmblockscmd } },
	{ MODKEY|ShiftMask,             XK_b,      togglebar,      {0} },
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_j,      movestack,      {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_k,      movestack,      {.i = -1 } },
	{ MODKEY|ControlMask|ShiftMask, XK_j,      rotatestack,    {.i = +1 } },
	{ MODKEY|ControlMask|ShiftMask, XK_k,      rotatestack,    {.i = -1 } },
	{ MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_d,      incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,       {.f = +0.05} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = -0.05} },
	{ MODKEY|ControlMask,           XK_Return, zoom,           {0} },
	{ MODKEY|ControlMask|ShiftMask, XK_Return, view,           {0} },
	{ MODKEY,                       XK_Tab,    shiftviewclients, { .i = +1 } },
	{ MODKEY|ShiftMask,             XK_Tab,    shiftviewclients, { .i = -1 } },
	{ MODKEY|ShiftMask,             XK_c,      killclient,     {0} },
	{ MODKEY|ControlMask,           XK_c,      killclient,     {.ui = 1} },  // kill unselect
	{ MODKEY|ControlMask|ShiftMask, XK_c,      killclient,     {.ui = 2} },  // killall
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_f,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_m,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_z,      setlayout,      {.v = &layouts[3]} },
	{ MODKEY|ControlMask,           XK_space,  focusmaster,    {0} },
	{ MODKEY,                       XK_space,  setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
	{ MODKEY|ShiftMask,             XK_f,      togglefullscr,  {0} },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_apostrophe,  swapmon,   {0} },
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
	{ MODKEY|ShiftMask,             XK_q,      quit,           {0} },
	{ MODKEY|ControlMask|ShiftMask, XK_q,      quit,           {1} },
	{ 0,                            XK_F1,     spawn,          {.v = volmutecmd } },
	{ 0,                            XK_F2,     spawn,          {.v = voldowncmd } },
	{ 0,                            XK_F3,     spawn,          {.v = volupcmd } },
	{ 0,                            XK_F4,     spawn,          {.v = micmutecmd } },
	{ 0,                            XF86XK_AudioMute,            spawn,          {.v = volmutecmd } },
	{ 0,                            XF86XK_AudioLowerVolume,     spawn,          {.v = voldowncmd } },
	{ 0,                            XF86XK_AudioRaiseVolume,     spawn,          {.v = volupcmd } },
	{ 0,                            XF86XK_AudioMicMute,         spawn,          {.v = micmutecmd } },
	{ 0,                            XF86XK_MonBrightnessDown,    spawn,          {.v = brightdowncmd } },
	{ 0,                            XF86XK_MonBrightnessUp,      spawn,          {.v = brightupcmd } },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static const Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button2,        setlayout,      {.v = &layouts[3]} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[5]} },
	{ ClkStatusText,        0,              Button1,        sigstatusbar,   {.i = 1} },
	{ ClkStatusText,        0,              Button2,        sigstatusbar,   {.i = 2} },
	{ ClkStatusText,        0,              Button3,        sigstatusbar,   {.i = 3} },
	{ ClkStatusText,        0,              Button4,        sigstatusbar,   {.i = 4} },
	{ ClkStatusText,        0,              Button5,        sigstatusbar,   {.i = 5} },
	{ ClkStatusText,        MODKEY,         Button4,        focusstack,     {-1} },
	{ ClkStatusText,        MODKEY,         Button5,        focusstack,     {+1} },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            0,              Button4,        shiftviewclients,   {-1} },
	{ ClkTagBar,            0,              Button5,        shiftviewclients,   {+1} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button2,        swaptags,       {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

/* vim: set ts=4 sw=4 noet : */
