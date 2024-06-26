/* See LICENSE file for copyright and license details. */

#include <X11/XF86keysym.h>

/* appearance */
static const unsigned int borderpx = 3;     /* border pixel of windows */
static const unsigned int gappx = 10;       /* gaps between windows */
static const unsigned int snap = 32;        /* snap pixel */
static const int showbar = 1;               /* 0 means no bar */
static const int topbar = 1;                /* 0 means bottom bar */
static const int usealtbar = 1;             /* 1 means use non-dwm status bar */
static const char *altbarclass = "Polybar"; /* Alternate bar class name */
static const char *alttrayname = "tray";    /* Polybar tray instance name */
static const char *altbarcmd =
    "$HOME/.config/polybar/launch.sh"; /* Alternate bar launch command */
static const char *fonts[] = {"JetBrainsMonoNL Nerd Font:size=12"};
static const char dmenufont[] = "JetBrainsMonoNL Nerd Font:size=12";
static const char col_main[] = "#29251d";
static const char col_cl_border[] = "#303446";
static const char col_blocks_font[] = "#c6d0f5";
static const char col_tab_font[] = "#c6d0f5";
static const char col_selected[] = "#ca9ee6";
static const unsigned int baralpha = 0xC8;
static const unsigned int borderalpha = OPAQUE;

static const char *colors[][3] = {
    /*               fg         bg         border   */
    [SchemeNorm] = {col_blocks_font, col_main, col_cl_border},
    [SchemeSel] = {col_tab_font, col_selected, col_selected},
};

static const char *const autostart[] = {
    "picom",  "-b",        NULL, "sh",    "-c", "/home/myschkin/.fehbg",
    NULL,     "nm-applet", NULL, "dunst", NULL, "greenclip",
    "daemon", NULL,        NULL /* terminate */
};

static const unsigned int alphas[][3] = {
    /*               fg      bg        border*/
    [SchemeNorm] = {OPAQUE, baralpha, borderalpha},
    [SchemeSel] = {OPAQUE, baralpha, borderalpha},
};

/* tagging */
static const char *tags[] = {"1", "2", "3", "4"};

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
static const float mfact = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster = 1;    /* number of clients in master area */
static const int resizehints =
    1; /* 1 means respect size hints in tiled resizals */
static const int attachbelow =
    1; /* 1 means attach after the currently active window */
static const int lockfullscreen =
    1; /* 1 will force focus on the fullscreen window */

static const Layout layouts[] = {
    /* symbol     arrange function */
    {"[]=", tile}, /* first entry is default */
    {"><>", NULL}, /* no layout function means floating behavior */
    {"[M]", monocle},
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY, TAG)                                                      \
  {MODKEY, KEY, view, {.ui = 1 << TAG}},                                       \
      {MODKEY | ControlMask, KEY, toggleview, {.ui = 1 << TAG}},               \
      {MODKEY | ShiftMask, KEY, tag, {.ui = 1 << TAG}},                        \
      {MODKEY | ControlMask | ShiftMask, KEY, toggletag, {.ui = 1 << TAG}},

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd)                                                             \
  {                                                                            \
    .v = (const char *[]) { "/bin/sh", "-c", cmd, NULL }                       \
  }

#define STATUSBAR "dwmblocks"
/* Volume controls definition */
static const char *upvol[] = {"vol_notifs.sh", "5%+", NULL};
static const char *downvol[] = {"vol_notifs.sh", "5%-", NULL};
static const char *mutevol[] = {"vol_notifs.sh", "0", NULL};
static const char *upbright[] = {"bright_notifs", "+", NULL};
static const char *downbright[] = {"bright_notifs", "-", NULL};

#define SHCMD(cmd)                                                             \
  {                                                                            \
    .v = (const char *[]) { "/bin/sh", "-c", cmd, NULL }                       \
  }
/* commands */
static char dmenumon[2] =
    "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = {"rofi-launcher", NULL};
static const char *powermenucmd[] = {"rofi-powermenu", NULL};
static const char *clipmenucmd[] = {
    "rofi",    "-modi",     "\"clipboard:greenclip print\"",
    "-show",   "clipboard", "-run-command",
    "\'{cmd}\'", NULL};
static const char *emojimenucmd[] = {"rofi",  "-modi", "emoji",
                                     "-show", "emoji", NULL};
static const char *bwmenucmd[] = {"rofi-rbw", NULL};
static const char *networkmenucmd[] = {"networkmanager_dmenu", NULL};
static const char *termcmd[] = {"kitty", "tmux", NULL};
static const char *calccmd[] = {"rofi", "-show", "calc", "-modi", "calc", NULL};

static const Key keys[] = {
    /* modifier                     key        function        argument */
    {MODKEY, XK_p, spawn, {.v = dmenucmd}},
    {MODKEY, XK_x, spawn, {.v = powermenucmd}},
    {MODKEY, XK_v, spawn, {.v = clipmenucmd}},
    {MODKEY, XK_e, spawn, {.v = emojimenucmd}},
    {MODKEY, XK_w, spawn, {.v = bwmenucmd}},
    {MODKEY, XK_n, spawn, {.v = networkmenucmd}},
    {MODKEY, XK_c, spawn, {.v = calccmd}},
    {MODKEY | ShiftMask, XK_Return, spawn, {.v = termcmd}},
    {MODKEY, XK_b, togglebar, {0}},
    {MODKEY, XK_j, focusstack, {.i = +1}},
    {MODKEY, XK_k, focusstack, {.i = -1}},
    {MODKEY, XK_i, incnmaster, {.i = +1}},
    {MODKEY, XK_d, incnmaster, {.i = -1}},
    {MODKEY, XK_h, setmfact, {.f = -0.05}},
    {MODKEY, XK_l, setmfact, {.f = +0.05}},
    {MODKEY, XK_Return, zoom, {0}},
    {MODKEY, XK_Tab, view, {0}},
    {MODKEY | ShiftMask, XK_c, killclient, {0}},
    {MODKEY, XK_t, setlayout, {.v = &layouts[0]}},
    {MODKEY, XK_f, setlayout, {.v = &layouts[1]}},
    {MODKEY, XK_m, setlayout, {.v = &layouts[2]}},
    {MODKEY, XK_space, setlayout, {0}},
    {MODKEY | ShiftMask, XK_space, togglefloating, {0}},
    {MODKEY, XK_0, view, {.ui = ~0}},
    {MODKEY | ShiftMask, XK_0, tag, {.ui = ~0}},
    {MODKEY, XK_comma, focusmon, {.i = -1}},
    {MODKEY, XK_period, focusmon, {.i = +1}},
    {MODKEY | ShiftMask, XK_comma, tagmon, {.i = -1}},
    {MODKEY | ShiftMask, XK_period, tagmon, {.i = +1}},
    {MODKEY, XK_KP_Subtract, setgaps, {.i = -1}},
    {MODKEY, XK_KP_Add, setgaps, {.i = +1}},
    {MODKEY, XK_KP_Enter, setgaps, {.i = 0}},
    {MODKEY | ShiftMask, XK_q, quit, {0}},
    {MODKEY | ControlMask | ShiftMask, XK_q, quit, {1}},
    {0, XK_Print, spawn,
     SHCMD("maim -s | xclip -selection clipboard -t image/png")},
    {0, XF86XK_AudioLowerVolume, spawn, {.v = downvol}},
    {0, XF86XK_AudioMute, spawn, {.v = mutevol}},
    {0, XF86XK_AudioRaiseVolume, spawn, {.v = upvol}},
    {0, XF86XK_MonBrightnessUp, spawn, {.v = upbright}},
    {0, XF86XK_MonBrightnessDown, spawn, {.v = downbright}},
    TAGKEYS(XK_1, 0) TAGKEYS(XK_2, 1) TAGKEYS(XK_3, 2) TAGKEYS(XK_4, 3)
        TAGKEYS(XK_5, 4) TAGKEYS(XK_6, 5) TAGKEYS(XK_7, 6) TAGKEYS(XK_8, 7)
            TAGKEYS(XK_9, 8)};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle,
 * ClkClientWin, or ClkRootWin */
static const Button buttons[] = {
    /* click                event mask      button          function argument */
    {ClkLtSymbol, 0, Button1, setlayout, {0}},
    {ClkLtSymbol, 0, Button3, setlayout, {.v = &layouts[2]}},
    {ClkWinTitle, 0, Button2, zoom, {0}},
    {ClkStatusText, 0, Button1, sigstatusbar, {.i = 1}},
    {ClkStatusText, 0, Button2, sigstatusbar, {.i = 2}},
    {ClkStatusText, 0, Button3, sigstatusbar, {.i = 3}},
    {ClkClientWin, MODKEY, Button1, movemouse, {0}},
    {ClkClientWin, MODKEY, Button2, togglefloating, {0}},
    {ClkClientWin, MODKEY, Button3, resizemouse, {0}},
    {ClkTagBar, 0, Button1, view, {0}},
    {ClkTagBar, 0, Button3, toggleview, {0}},
    {ClkTagBar, MODKEY, Button1, tag, {0}},
    {ClkTagBar, MODKEY, Button3, toggletag, {0}},
};

static const char *ipcsockpath = "/tmp/dwm.sock";
static IPCCommand ipccommands[] = {
    IPCCOMMAND(view, 1, {ARG_TYPE_UINT}),
    IPCCOMMAND(toggleview, 1, {ARG_TYPE_UINT}),
    IPCCOMMAND(tag, 1, {ARG_TYPE_UINT}),
    IPCCOMMAND(toggletag, 1, {ARG_TYPE_UINT}),
    IPCCOMMAND(tagmon, 1, {ARG_TYPE_UINT}),
    IPCCOMMAND(focusmon, 1, {ARG_TYPE_SINT}),
    IPCCOMMAND(focusstack, 1, {ARG_TYPE_SINT}),
    IPCCOMMAND(zoom, 1, {ARG_TYPE_NONE}),
    IPCCOMMAND(incnmaster, 1, {ARG_TYPE_SINT}),
    IPCCOMMAND(killclient, 1, {ARG_TYPE_SINT}),
    IPCCOMMAND(togglefloating, 1, {ARG_TYPE_NONE}),
    IPCCOMMAND(setmfact, 1, {ARG_TYPE_FLOAT}),
    IPCCOMMAND(setlayoutsafe, 1, {ARG_TYPE_PTR}),
    IPCCOMMAND(quit, 1, {ARG_TYPE_NONE})};
