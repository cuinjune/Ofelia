/*==============================================================================
	ofelia: OpenFrameworks as an External Library for Interactive Applications
 
	Copyright (c) 2018 Zack Lee <cuinjune@gmail.com>
 
	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.
 
	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
	GNU General Public License for more details.
 
	You should have received a copy of the GNU General Public License
	along with this program. If not, see <http://www.gnu.org/licenses/>.
	
	See https://github.com/cuinjune/ofxOfelia for documentation
 ==============================================================================*/

#include "ofeliaOverwrites.h"
#include "g_editor.c"
#include <math.h>
#if defined(_WIN32)
#include <io.h>
#else
#include <unistd.h>
#endif

/* borrowed from RMARGIN and BMARGIN in g_rtext.c */
#define ATOM_RMARGIN 2
#define ATOM_BMARGIN 4 /* 1 pixel smaller than object TMARGIN+BMARGIN */

#define MESSAGE_CLICK_WIDTH 5

bool bUsingWindow = false;
bool bThreadShouldWait = false;
bool bObjectCleared = true;
bool bPatchClosing = false;

/* wait until the thread gets to the beginning of update */
void waitForRenderingToFinish() {
    
    if (bUsingWindow && bObjectCleared) {
        
        sys_unlock();
        bThreadShouldWait = true;
        bObjectCleared = false;
        
        while (bUsingWindow && bThreadShouldWait) {
            
            const int millis = 1;
#if defined(_WIN32)
            Sleep(millis);
#else
            usleep(millis * 1000);
#endif
        }
    }
}

/* quit, after calling glist_finddirty() on all toplevels and verifying
 the user really wants to discard changes  */
void glob_verifyquit2(void *dummy, t_floatarg f)
{
    t_glist *g, *g2;
    /* find all root canvases */
    for (g = pd_getcanvaslist(); g; g = g->gl_next)
        if ((g2 = glist_finddirty(g)))
        {
            canvas_vis(g2, 1);
            sys_vgui("pdtk_canvas_menuclose .x%lx {.x%lx menuclose 3;\n}\n",
                     canvas_getrootfor(g2), g2);
            return;
        }
    if (f == 0 && sys_perf)
        sys_vgui("pdtk_check .pdwindow {really quit?} {pd quit} yes\n");
    else
    {
        bPatchClosing = true;
        waitForRenderingToFinish();
        glob_quit(0);
        bObjectCleared = true;
    }
}

/* close a window (or possibly quit Pd), checking for dirty flags.
 The "force" parameter is interpreted as follows:
 0 - request from GUI to close, verifying whether clean or dirty
 1 - request from GUI to close, no verification
 2 - verified - mark this one clean, then continue as in 1
 3 - verified - mark this one clean, then verify-and-quit
 */
void canvas_menuclose2(t_canvas *x, t_floatarg fforce)
{
    int force = fforce;
    t_glist *g;
    if ((x->gl_owner || x->gl_isclone) && (force == 0 || force == 1))
        canvas_vis(x, 0);   /* if subpatch, just invis it */
    else if (force == 0)
    {
        g = glist_finddirty(x);
        if (g)
        {
            vmess(&g->gl_pd, gensym("menu-open"), "");
            sys_vgui("pdtk_canvas_menuclose .x%lx {.x%lx menuclose 2;\n}\n",
                     canvas_getrootfor(g), g);
            return;
        }
        else if (sys_perf)
        {
            sys_vgui("pdtk_check .x%lx {Close this window?} {.x%lx menuclose 1;\n} yes\n",
                     canvas_getrootfor(x), x);
        }
        else
        {
            bPatchClosing = true;
            waitForRenderingToFinish();
            pd_free(&x->gl_pd);
            bObjectCleared = true;
        }
    }
    else if (force == 1)
    {
        bPatchClosing = true;
        waitForRenderingToFinish();
        pd_free(&x->gl_pd);
        bObjectCleared = true;
    }
    else if (force == 2)
    {
        canvas_dirty(x, 0);
        while (x->gl_owner)
            x = x->gl_owner;
        g = glist_finddirty(x);
        if (g)
        {
            vmess(&g->gl_pd, gensym("menu-open"), "");
            sys_vgui("pdtk_canvas_menuclose .x%lx {.x%lx menuclose 2;\n}\n",
                     canvas_getrootfor(x), g);
            return;
        }
        else
        {
            bPatchClosing = true;
            waitForRenderingToFinish();
            pd_free(&x->gl_pd);
            bObjectCleared = true;
        }
    }
    else if (force == 3)
    {
        canvas_dirty(x, 0);
        glob_verifyquit2(0, 1);
    }
}

/* call this for selected objects only */
void glist_deselect2(t_glist *x, t_gobj *y)
{
    int fixdsp = 0;
    if (x->gl_editor)
    {
        t_selection *sel, *sel2;
        t_rtext *z = 0;
        if (!glist_isselected(x, y)) bug("glist_deselect");
        if (x->gl_editor->e_textedfor)
        {
            t_rtext *fuddy = glist_findrtext(x, (t_text *)y);
            if (x->gl_editor->e_textedfor == fuddy)
            {
                if (x->gl_editor->e_textdirty)
                {
                    z = fuddy;
                    canvas_stowconnections(glist_getcanvas(x));
                    glist_checkanddeselectall(x, y);
                }
                gobj_activate(y, x, 0);
            }
            if (zgetfn(&y->g_pd, gensym("dsp")))
                fixdsp = canvas_suspend_dsp();
        }
        if ((sel = x->gl_editor->e_selection)->sel_what == y)
        {
            x->gl_editor->e_selection = x->gl_editor->e_selection->sel_next;
            gobj_select(sel->sel_what, x, 0);
            freebytes(sel, sizeof(*sel));
        }
        else
        {
            for (sel = x->gl_editor->e_selection; (sel2 = sel->sel_next);
                 sel = sel2)
            {
                if (sel2->sel_what == y)
                {
                    sel->sel_next = sel2->sel_next;
                    gobj_select(sel2->sel_what, x, 0);
                    freebytes(sel2, sizeof(*sel2));
                    break;
                }
            }
        }
        if (z)
        {
            char *buf;
            int bufsize;
            
            rtext_gettext(z, &buf, &bufsize);
            waitForRenderingToFinish();
            text_setto((t_text *)y, x, buf, bufsize);
            canvas_fixlinesfor(x, (t_text *)y);
            x->gl_editor->e_textedfor = 0;
            bObjectCleared = true;
        }
        if (fixdsp)
            canvas_resume_dsp(1);
    }
}

void glist_noselect2(t_glist *x)
{
    if (x->gl_editor)
    {
        while (x->gl_editor->e_selection)
            glist_deselect2(x, x->gl_editor->e_selection->sel_what);
        if (x->gl_editor->e_selectedline)
            glist_deselectline(x);
    }
}

void glist_selectline2(t_glist *x, t_outconnect *oc, int index1,
                       int outno, int index2, int inno)
{
    if (x->gl_editor)
    {
        glist_noselect2(x);
        x->gl_editor->e_selectedline = 1;
        x->gl_editor->e_selectline_index1 = index1;
        x->gl_editor->e_selectline_outno = outno;
        x->gl_editor->e_selectline_index2 = index2;
        x->gl_editor->e_selectline_inno = inno;
        x->gl_editor->e_selectline_tag = oc;
        sys_vgui(".x%lx.c itemconfigure l%lx -fill blue\n",
                 x, x->gl_editor->e_selectline_tag);
    }
}

/* mouse click */
void canvas_doclick2(t_canvas *x, int xpos, int ypos, int which,
                     int mod, int doit)
{
    t_gobj *y;
    int shiftmod, runmode, altmod, doublemod = 0, rightclick;
    int x1=0, y1=0, x2=0, y2=0, clickreturned = 0;
    
    if (!x->gl_editor)
    {
        bug("editor");
        return;
    }
    
    shiftmod = (mod & SHIFTMOD);
    runmode = ((mod & CTRLMOD) || (!x->gl_edit));
    altmod = (mod & ALTMOD);
    rightclick = (mod & RIGHTCLICK);
    
    EDITOR->canvas_undo_already_set_move = 0;
    
    /* if keyboard was grabbed, notify grabber and cancel the grab */
    if (doit && x->gl_editor->e_grab && x->gl_editor->e_keyfn)
    {
        (* x->gl_editor->e_keyfn) (x->gl_editor->e_grab, 0);
        glist_grab(x, 0, 0, 0, 0, 0);
    }
    
    if (doit && !runmode && xpos == EDITOR->canvas_upx &&
        ypos == EDITOR->canvas_upy &&
        sys_getrealtime() - EDITOR->canvas_upclicktime < DCLICKINTERVAL)
        doublemod = 1;
    x->gl_editor->e_lastmoved = 0;
    if (doit)
    {
        x->gl_editor->e_grab = 0;
        x->gl_editor->e_onmotion = MA_NONE;
    }
    /* post("click %d %d %d %d", xpos, ypos, which, mod); */
    
    if (x->gl_editor->e_onmotion != MA_NONE)
        return;
    
    x->gl_editor->e_xwas = xpos;
    x->gl_editor->e_ywas = ypos;
    
    if (runmode && !rightclick)
    {
        for (y = x->gl_list; y; y = y->g_next)
        {
            /* check if the object wants to be clicked */
            if (canvas_hitbox(x, y, xpos, ypos, &x1, &y1, &x2, &y2)
                && (clickreturned = gobj_click(y, x, xpos, ypos,
                                               shiftmod, ((mod & CTRLMOD) && (!x->gl_edit)) || altmod,
                                               0, doit)))
                break;
        }
        if (!doit)
        {
            if (y)
                canvas_setcursor(x, clickreturned);
            else canvas_setcursor(x, CURSOR_RUNMODE_NOTHING);
        }
        return;
    }
    /* if not a runmode left click, fall here. */
    if ((y = canvas_findhitbox(x, xpos, ypos, &x1, &y1, &x2, &y2)))
    {
        t_object *ob = pd_checkobject(&y->g_pd);
        /* check you're in the rectangle */
        ob = pd_checkobject(&y->g_pd);
        if (rightclick)
            canvas_rightclick(x, xpos, ypos, y);
        else if (shiftmod)
        {
            if (doit)
            {
                t_rtext *rt;
                if (ob && (rt = x->gl_editor->e_textedfor) &&
                    rt == glist_findrtext(x, ob))
                {
                    rtext_mouse(rt, xpos - x1, ypos - y1, RTEXT_SHIFT);
                    x->gl_editor->e_onmotion = MA_DRAGTEXT;
                    x->gl_editor->e_xwas = x1;
                    x->gl_editor->e_ywas = y1;
                }
                else
                {
                    if (glist_isselected(x, y))
                        glist_deselect2(x, y);
                    else glist_select(x, y);
                }
            }
        }
        else
        {
            int noutlet;
            /* resize?  only for "true" text boxes or canvases*/
            if (ob && !x->gl_editor->e_selection &&
                (ob->te_pd->c_wb == &text_widgetbehavior ||
                 pd_checkglist(&ob->te_pd)) &&
                xpos >= x2-4 && ypos < y2-4)
            {
                if (doit)
                {
                    if (!glist_isselected(x, y))
                    {
                        glist_noselect2(x);
                        glist_select(x, y);
                    }
                    x->gl_editor->e_onmotion = MA_RESIZE;
                    x->gl_editor->e_xwas = x1;
                    x->gl_editor->e_ywas = y1;
                    x->gl_editor->e_xnew = xpos;
                    x->gl_editor->e_ynew = ypos;
                }
                else canvas_setcursor(x, CURSOR_EDITMODE_RESIZE);
            }
            /* look for an outlet */
            else if (ob && (noutlet = obj_noutlets(ob)) &&
                     ypos >= y2 - (IOHEIGHT*x->gl_zoom) + x->gl_zoom)
            {
                int width = x2 - x1;
                int iow = IOWIDTH * x->gl_zoom;
                int nout1 = (noutlet > 1 ? noutlet - 1 : 1);
                int closest = ((xpos-x1) * (nout1) + width/2)/width;
                int hotspot = x1 +
                (width - iow) * closest / (nout1);
                if (closest < noutlet &&
                    xpos >= (hotspot - x->gl_zoom) &&
                    xpos <= hotspot + (iow + x->gl_zoom))
                {
                    if (doit)
                    {
                        int issignal = obj_issignaloutlet(ob, closest);
                        x->gl_editor->e_onmotion = MA_CONNECT;
                        x->gl_editor->e_xwas = xpos;
                        x->gl_editor->e_ywas = ypos;
                        sys_vgui(
                                 ".x%lx.c create line %d %d %d %d -width %d -tags x\n",
                                 x, xpos, ypos, xpos, ypos,
                                 (issignal ? 2 : 1) * x->gl_zoom);
                    }
                    else canvas_setcursor(x, CURSOR_EDITMODE_CONNECT);
                }
                else if (doit)
                    goto nooutletafterall;
                else canvas_setcursor(x, CURSOR_EDITMODE_NOTHING);
            }
            /* not in an outlet; select and move */
            else if (doit)
            {
                t_rtext *rt;
                /* check if the box is being text edited */
            nooutletafterall:
                if (ob && (rt = x->gl_editor->e_textedfor) &&
                    rt == glist_findrtext(x, ob))
                {
                    rtext_mouse(rt, xpos - x1, ypos - y1,
                                (doublemod ? RTEXT_DBL : RTEXT_DOWN));
                    x->gl_editor->e_onmotion = MA_DRAGTEXT;
                    x->gl_editor->e_xwas = x1;
                    x->gl_editor->e_ywas = y1;
                }
                else
                {
                    /* otherwise select and drag to displace */
                    if (!glist_isselected(x, y))
                    {
                        glist_noselect2(x);
                        glist_select(x, y);
                    }
                    x->gl_editor->e_onmotion = MA_MOVE;
                }
            }
            else canvas_setcursor(x, CURSOR_EDITMODE_NOTHING);
        }
        return;
    }
    /* if right click doesn't hit any boxes, call rightclick
     routine anyway */
    if (rightclick)
        canvas_rightclick(x, xpos, ypos, 0);
    
    /* if not an editing action, and if we didn't hit a
     box, set cursor and return */
    if (runmode || rightclick)
    {
        canvas_setcursor(x, CURSOR_RUNMODE_NOTHING);
        return;
    }
    /* having failed to find a box, we try lines now. */
    if (!runmode && !altmod && !shiftmod)
    {
        t_linetraverser t;
        t_outconnect *oc;
        t_float fx = xpos, fy = ypos;
        t_glist *glist2 = glist_getcanvas(x);
        linetraverser_start(&t, glist2);
        while ((oc = linetraverser_next(&t)))
        {
            t_float lx1 = t.tr_lx1, ly1 = t.tr_ly1,
            lx2 = t.tr_lx2, ly2 = t.tr_ly2;
            t_float area = (lx2 - lx1) * (fy - ly1) -
            (ly2 - ly1) * (fx - lx1);
            t_float dsquare = (lx2-lx1) * (lx2-lx1) + (ly2-ly1) * (ly2-ly1);
            if (area * area >= 50 * dsquare) continue;
            if ((lx2-lx1) * (fx-lx1) + (ly2-ly1) * (fy-ly1) < 0) continue;
            if ((lx2-lx1) * (lx2-fx) + (ly2-ly1) * (ly2-fy) < 0) continue;
            if (doit)
            {
                glist_selectline2(glist2, oc,
                                  canvas_getindex(glist2, &t.tr_ob->ob_g), t.tr_outno,
                                  canvas_getindex(glist2, &t.tr_ob2->ob_g), t.tr_inno);
            }
            canvas_setcursor(x, CURSOR_EDITMODE_DISCONNECT);
            return;
        }
    }
    canvas_setcursor(x, CURSOR_EDITMODE_NOTHING);
    if (doit)
    {
        if (!shiftmod) glist_noselect2(x);
        sys_vgui(".x%lx.c create rectangle %d %d %d %d -tags x\n",
                 x, xpos, ypos, xpos, ypos);
        x->gl_editor->e_xwas = xpos;
        x->gl_editor->e_ywas = ypos;
        x->gl_editor->e_onmotion = MA_REGION;
    }
}

void canvas_mouse2(t_canvas *x, t_floatarg xpos, t_floatarg ypos,
                   t_floatarg which, t_floatarg mod)
{
    canvas_doclick2(x, xpos, ypos, which, mod, 1);
}

static void canvas_undo_cut2(t_canvas *x, void *z, int action)
{
    t_undo_cut *buf = z;
    int mode = buf->u_mode;
    t_pd *boundx = s__X.s_thing;
    if (action == UNDO_UNDO)
    {
        if (mode == UCUT_CUT)
            canvas_dopaste(x, EDITOR->copy_binbuf);
        else if (mode == UCUT_CLEAR)
            canvas_dopaste(x, buf->u_objectbuf);
        else if (mode == UCUT_TEXT)
        {
            t_gobj *y1, *y2;
            waitForRenderingToFinish();
            glist_noselect(x);
            for (y1 = x->gl_list; (y2 = y1->g_next); y1 = y2)
                ;
            if (y1)
            {
                if (!buf->u_redotextbuf)
                {
                    glist_noselect(x);
                    glist_select(x, y1);
                    buf->u_redotextbuf = canvas_docopy(x);
                    glist_noselect(x);
                }
                glist_delete(x, y1);
            }
            canvas_dopaste(x, buf->u_objectbuf);
            bObjectCleared = true;
        }
        s__X.s_thing = &x->gl_pd;
        binbuf_eval(buf->u_reconnectbuf, 0, 0, 0);
        s__X.s_thing = boundx;
    }
    else if (action == UNDO_REDO)
    {
        if (mode == UCUT_CUT || mode == UCUT_CLEAR) {
            
            waitForRenderingToFinish();
            canvas_doclear(x);
            bObjectCleared = true;
        }
        else if (mode == UCUT_TEXT)
        {
            t_gobj *y1, *y2;
            for (y1 = x->gl_list; (y2 = y1->g_next); y1 = y2)
                ;
            waitForRenderingToFinish();
            
            if (y1)
                glist_delete(x, y1);
            canvas_dopaste(x, buf->u_redotextbuf);
            bObjectCleared = true;
            s__X.s_thing = &x->gl_pd;
            binbuf_eval(buf->u_reconnectbuf, 0, 0, 0);
            s__X.s_thing = boundx;
        }
    }
    else if (action == UNDO_FREE)
    {
        if (buf->u_objectbuf)
            binbuf_free(buf->u_objectbuf);
        if (buf->u_reconnectbuf)
            binbuf_free(buf->u_reconnectbuf);
        if (buf->u_redotextbuf)
            binbuf_free(buf->u_redotextbuf);
        t_freebytes(buf, sizeof(*buf));
    }
}

/* this routine is called whenever a key is pressed or released.  "x"
 may be zero if there's no current canvas.  The first argument is true or
 false for down/up; the second one is either a symbolic key name (e.g.,
 "Right" or an Ascii key number.  The third is the shift key. */
void canvas_key2(t_canvas *x, t_symbol *s, int ac, t_atom *av)
{
    int keynum, fflag;
    t_symbol *gotkeysym;
    
    int down, shift;
    
    if (ac < 3)
        return;
    
    EDITOR->canvas_undo_already_set_move = 0;
    down = (atom_getfloat(av) != 0);  /* nonzero if it's a key down */
    shift = (atom_getfloat(av+2) != 0);  /* nonzero if shift-ed */
    if (av[1].a_type == A_SYMBOL)
        gotkeysym = av[1].a_w.w_symbol;
    else if (av[1].a_type == A_FLOAT)
    {
        char buf[UTF8_MAXBYTES1];
        switch((int)(av[1].a_w.w_float))
        {
            case 8:  gotkeysym = gensym("BackSpace"); break;
            case 9:  gotkeysym = gensym("Tab"); break;
            case 10: gotkeysym = gensym("Return"); break;
            case 27: gotkeysym = gensym("Escape"); break;
            case 32: gotkeysym = gensym("Space"); break;
            case 127:gotkeysym = gensym("Delete"); break;
            default:
                /*-- moo: assume keynum is a Unicode codepoint; encode as UTF-8 --*/
                u8_wc_toutf8_nul(buf, (UCS4)(av[1].a_w.w_float));
                gotkeysym = gensym(buf);
        }
    }
    else gotkeysym = gensym("?");
    fflag = (av[0].a_type == A_FLOAT ? av[0].a_w.w_float : 0);
    keynum = (av[1].a_type == A_FLOAT ? av[1].a_w.w_float : 0);
    if (keynum == '\\' || keynum == '{' || keynum == '}')
    {
        post("keycode %d: dropped", (int)keynum);
        return;
    }
#if 0
    post("keynum %d, down %d", (int)keynum, down);
#endif
    if (keynum == '\r') keynum = '\n';
    if (av[1].a_type == A_SYMBOL &&
        !strcmp(av[1].a_w.w_symbol->s_name, "Return"))
        keynum = '\n';
    /* alias Apple key numbers to symbols.  This is done unconditionally,
     not just if we're on an Apple, just in case the GUI is remote. */
    if (keynum == 30 || keynum == 63232)
        keynum = 0, gotkeysym = gensym("Up");
    else if (keynum == 31 || keynum == 63233)
        keynum = 0, gotkeysym = gensym("Down");
    else if (keynum == 28 || keynum == 63234)
        keynum = 0, gotkeysym = gensym("Left");
    else if (keynum == 29 || keynum == 63235)
        keynum = 0, gotkeysym = gensym("Right");
    else if (keynum == 63273)
        keynum = 0, gotkeysym = gensym("Home");
    else if (keynum == 63275)
        keynum = 0, gotkeysym = gensym("End");
    else if (keynum == 63276)
        keynum = 0, gotkeysym = gensym("Prior");
    else if (keynum == 63277)
        keynum = 0, gotkeysym = gensym("Next");
    if (gensym("#key")->s_thing && down)
        pd_float(gensym("#key")->s_thing, (t_float)keynum);
    if (gensym("#keyup")->s_thing && !down)
        pd_float(gensym("#keyup")->s_thing, (t_float)keynum);
    if (gensym("#keyname")->s_thing)
    {
        t_atom at[2];
        at[0] = av[0];
        SETFLOAT(at, down);
        SETSYMBOL(at+1, gotkeysym);
        pd_list(gensym("#keyname")->s_thing, 0, 2, at);
    }
    if (!x || !x->gl_editor)  /* if that 'invis'ed the window,  stop. */
        return;
    if (x && down)
    {
        t_object *ob;
        /* cancel any dragging action */
        if (x->gl_editor->e_onmotion == MA_MOVE)
            x->gl_editor->e_onmotion = MA_NONE;
        /* if an object has "grabbed" keys just send them on */
        if (x->gl_editor->e_grab
            && x->gl_editor->e_keyfn && keynum)
            (* x->gl_editor->e_keyfn)
            (x->gl_editor->e_grab, (t_float)keynum);
        /* if a text editor is open send the key on, as long as
         it is either "real" (has a key number) or else is an arrow key. */
        else if (x->gl_editor->e_textedfor && (keynum
                                               || !strcmp(gotkeysym->s_name, "Up")
                                               || !strcmp(gotkeysym->s_name, "Down")
                                               || !strcmp(gotkeysym->s_name, "Left")
                                               || !strcmp(gotkeysym->s_name, "Right")))
        {
            /* send the key to the box's editor */
            if (!x->gl_editor->e_textdirty)
            {
                canvas_setundo(x, canvas_undo_cut2,
                               canvas_undo_set_cut(x, UCUT_TEXT), "typing");
            }
            rtext_key(x->gl_editor->e_textedfor,
                      (int)keynum, gotkeysym);
            if (x->gl_editor->e_textdirty)
                canvas_dirty(x, 1);
        }
        /* check for backspace or clear */
        else if (keynum == 8 || keynum == 127)
        {
            if (x->gl_editor->e_selectedline)
            {
                waitForRenderingToFinish();
                canvas_clearline(x);
                bObjectCleared = true;
            }
            else if (x->gl_editor->e_selection)
            {
                canvas_setundo(x, canvas_undo_cut2,
                               canvas_undo_set_cut(x, UCUT_CLEAR), "clear");
                waitForRenderingToFinish();
                canvas_doclear(x);
                bObjectCleared = true;
            }
        }
        /* check for arrow keys */
        else if (!strcmp(gotkeysym->s_name, "Up"))
            canvas_displaceselection(x, 0, shift ? -10 : -1);
        else if (!strcmp(gotkeysym->s_name, "Down"))
            canvas_displaceselection(x, 0, shift ? 10 : 1);
        else if (!strcmp(gotkeysym->s_name, "Left"))
            canvas_displaceselection(x, shift ? -10 : -1, 0);
        else if (!strcmp(gotkeysym->s_name, "Right"))
            canvas_displaceselection(x, shift ? 10 : 1, 0);
    }
    /* if control key goes up or down, and if we're in edit mode, change
     cursor to indicate how the click action changes */
    if (x && keynum == 0 && x->gl_edit &&
        !strncmp(gotkeysym->s_name, "Control", 7))
        canvas_setcursor(x, down ?
                         CURSOR_RUNMODE_NOTHING :CURSOR_EDITMODE_NOTHING);
}

static void canvas_cut2(t_canvas *x)
{
    if (!x->gl_editor)  /* ignore if invisible */
        return;
    if (x->gl_editor && x->gl_editor->e_selectedline)   /* delete line */
    {
        waitForRenderingToFinish();
        canvas_clearline(x);
        bObjectCleared = true;
    }
    else if (x->gl_editor->e_textedfor) /* delete selected text in a box */
    {
        char *buf;
        int bufsize;
        rtext_getseltext(x->gl_editor->e_textedfor, &buf, &bufsize);
        if (!bufsize && x->gl_editor->e_selection &&
            !x->gl_editor->e_selection->sel_next)
        {
            /* if the text is already empty, delete the box.  We
             first clear 'textedfor' so that canvas_doclear later will
             think the whole box was selected, not the text */
            x->gl_editor->e_textedfor = 0;
            goto deleteobj;
        }
        canvas_copy(x);
        rtext_key(x->gl_editor->e_textedfor, 127, &s_);
        canvas_dirty(x, 1);
    }
    else if (x->gl_editor && x->gl_editor->e_selection)
    {
    deleteobj:      /* delete one or more objects */
        canvas_setundo(x, canvas_undo_cut2,
                       canvas_undo_set_cut(x, UCUT_CUT), "cut");
        canvas_copy(x);
        waitForRenderingToFinish();
        canvas_doclear(x);
        bObjectCleared = true;
        sys_vgui("pdtk_canvas_getscroll .x%lx.c\n", x);
    }
}

static void canvas_objtext(t_glist *gl, int xpix, int ypix, int width,
                           int selected, t_binbuf *b)
{
    t_text *x;
    int argc;
    t_atom *argv;
    pd_this->pd_newest = 0;
    canvas_setcurrent((t_canvas *)gl);
    canvas_getargs(&argc, &argv);
    binbuf_eval(b, &pd_objectmaker, argc, argv);
    if (binbuf_getnatom(b))
    {
        if (!pd_this->pd_newest)
            x = 0;
        else if (!(x = pd_checkobject(pd_this->pd_newest)))
        {
            binbuf_print(b);
            error("... didn't return a patchable object");
        }
    }
    else x = 0;
    if (!x)
    {
        x = (t_text *)pd_new(text_class);
        if (binbuf_getnatom(b))
        {
            binbuf_print(b);
            pd_error(x, "... couldn't create");
        }
    }
    x->te_binbuf = b;
    x->te_xpix = xpix;
    x->te_ypix = ypix;
    x->te_width = width;
    x->te_type = T_OBJECT;
    glist_add(gl, &x->te_g);
    if (selected)
    {
        /* this is called if we've been created from the menu. */
        glist_select(gl, &x->te_g);
        gobj_activate(&x->te_g, gl, 1);
    }
    if (pd_class(&x->ob_pd) == vinlet_class)
        canvas_resortinlets(glist_getcanvas(gl));
    if (pd_class(&x->ob_pd) == voutlet_class)
        canvas_resortoutlets(glist_getcanvas(gl));
    canvas_unsetcurrent((t_canvas *)gl);
}

extern int sys_noautopatch;
/* utility routine to figure out where to put a new text box from menu
 and whether to connect to it automatically */
static void canvas_howputnew(t_canvas *x, int *connectp, int *xpixp, int *ypixp,
                             int *indexp, int *totalp)
{
    int xpix, ypix, indx = 0, nobj = 0, n2, x1, x2, y1, y2;
    int connectme = (x->gl_editor->e_selection &&
                     !x->gl_editor->e_selection->sel_next && !sys_noautopatch);
    if (connectme)
    {
        t_gobj *g, *selected = x->gl_editor->e_selection->sel_what;
        for (g = x->gl_list, nobj = 0; g; g = g->g_next, nobj++)
            if (g == selected)
            {
                gobj_getrect(g, x, &x1, &y1, &x2, &y2);
                indx = nobj;
                *xpixp = x1;
                *ypixp = y2 + 5;
            }
        glist_noselect2(x);
        /* search back for 'selected' and if it isn't on the list,
         plan just to connect from the last item on the list. */
        for (g = x->gl_list, n2 = 0; g; g = g->g_next, n2++)
        {
            if (g == selected)
            {
                indx = n2;
                break;
            }
            else if (!g->g_next)
                indx = nobj-1;
        }
    }
    else
    {
        glist_getnextxy(x, xpixp, ypixp);
        *xpixp -= 3;
        *ypixp -= 3;
        glist_noselect2(x);
    }
    *connectp = connectme;
    *indexp = indx;
    *totalp = nobj;
}

void canvas_obj(t_glist *gl, t_symbol *s, int argc, t_atom *argv)
{
    t_text *x;
    if (argc >= 2)
    {
        t_binbuf *b = binbuf_new();
        binbuf_restore(b, argc-2, argv+2);
        canvas_objtext(gl, atom_getfloatarg(0, argc, argv),
                       atom_getfloatarg(1, argc, argv), 0, 0, b);
    }
    /* JMZ: don't go into interactive mode in a closed canvas */
    else if (!glist_isvisible(gl))
        post("unable to create stub object in closed canvas!");
    else
    {
        /* interactively create new obect */
        t_binbuf *b = binbuf_new();
        int connectme, xpix, ypix, indx, nobj;
        canvas_howputnew(gl, &connectme, &xpix, &ypix, &indx, &nobj);
        pd_vmess(&gl->gl_pd, gensym("editmode"), "i", 1);
        canvas_objtext(gl, xpix, ypix, 0, 1, b);
        if (connectme)
            canvas_connect(gl, indx, 0, nobj, 0);
        else canvas_startmotion(glist_getcanvas(gl));
    }
}

/* make an object box for an object that's already there. */

/* iemlib */
void canvas_iemguis(t_glist *gl, t_symbol *guiobjname)
{
    t_atom at;
    t_binbuf *b = binbuf_new();
    int xpix, ypix;
    
    pd_vmess(&gl->gl_pd, gensym("editmode"), "i", 1);
    glist_noselect2(gl);
    SETSYMBOL(&at, guiobjname);
    binbuf_restore(b, 1, &at);
    glist_getnextxy(gl, &xpix, &ypix);
    canvas_objtext(gl, xpix, ypix, 0, 1, b);
    canvas_startmotion(glist_getcanvas(gl));
}

void canvas_bng(t_glist *gl, t_symbol *s, int argc, t_atom *argv)
{
    canvas_iemguis(gl, gensym("bng"));
}

void canvas_toggle(t_glist *gl, t_symbol *s, int argc, t_atom *argv)
{
    canvas_iemguis(gl, gensym("tgl"));
}

void canvas_vslider(t_glist *gl, t_symbol *s, int argc, t_atom *argv)
{
    canvas_iemguis(gl, gensym("vsl"));
}

void canvas_hslider(t_glist *gl, t_symbol *s, int argc, t_atom *argv)
{
    canvas_iemguis(gl, gensym("hsl"));
}

void canvas_hdial(t_glist *gl, t_symbol *s, int argc, t_atom *argv)
{
    canvas_iemguis(gl, gensym("hdl"));
}

void canvas_vdial(t_glist *gl, t_symbol *s, int argc, t_atom *argv)
{
    canvas_iemguis(gl, gensym("vdl"));
}

void canvas_hradio(t_glist *gl, t_symbol *s, int argc, t_atom *argv)
{
    canvas_iemguis(gl, gensym("hradio"));
}

void canvas_vradio(t_glist *gl, t_symbol *s, int argc, t_atom *argv)
{
    canvas_iemguis(gl, gensym("vradio"));
}

void canvas_vumeter(t_glist *gl, t_symbol *s, int argc, t_atom *argv)
{
    canvas_iemguis(gl, gensym("vu"));
}

void canvas_mycnv(t_glist *gl, t_symbol *s, int argc, t_atom *argv)
{
    canvas_iemguis(gl, gensym("cnv"));
}

void canvas_numbox(t_glist *gl, t_symbol *s, int argc, t_atom *argv)
{
    canvas_iemguis(gl, gensym("nbx"));
}

/* iemlib */

void canvas_objfor(t_glist *gl, t_text *x, int argc, t_atom *argv)
{
    x->te_width = 0;                            /* don't know it yet. */
    x->te_type = T_OBJECT;
    x->te_binbuf = binbuf_new();
    x->te_xpix = atom_getfloatarg(0, argc, argv);
    x->te_ypix = atom_getfloatarg(1, argc, argv);
    if (argc > 2) binbuf_restore(x->te_binbuf, argc-2, argv+2);
    glist_add(gl, &x->te_g);
}

/* ---------------------- the "message" text item ------------------------ */

typedef struct _messresponder
{
    t_pd mr_pd;
    t_outlet *mr_outlet;
} t_messresponder;

typedef struct _message
{
    t_text m_text;
    t_messresponder m_messresponder;
    t_glist *m_glist;
    t_clock *m_clock;
} t_message;

static t_class *message_class, *messresponder_class;

static void messresponder_bang(t_messresponder *x)
{
    outlet_bang(x->mr_outlet);
}

static void messresponder_float(t_messresponder *x, t_float f)
{
    outlet_float(x->mr_outlet, f);
}

static void messresponder_symbol(t_messresponder *x, t_symbol *s)
{
    outlet_symbol(x->mr_outlet, s);
}

static void messresponder_list(t_messresponder *x,
                               t_symbol *s, int argc, t_atom *argv)
{
    outlet_list(x->mr_outlet, s, argc, argv);
}

static void messresponder_anything(t_messresponder *x,
                                   t_symbol *s, int argc, t_atom *argv)
{
    outlet_anything(x->mr_outlet, s, argc, argv);
}

static void message_bang(t_message *x)
{
    binbuf_eval(x->m_text.te_binbuf, &x->m_messresponder.mr_pd, 0, 0);
}

static void message_float(t_message *x, t_float f)
{
    t_atom at;
    SETFLOAT(&at, f);
    binbuf_eval(x->m_text.te_binbuf, &x->m_messresponder.mr_pd, 1, &at);
}

static void message_symbol(t_message *x, t_symbol *s)
{
    t_atom at;
    SETSYMBOL(&at, s);
    binbuf_eval(x->m_text.te_binbuf, &x->m_messresponder.mr_pd, 1, &at);
}

static void message_list(t_message *x, t_symbol *s, int argc, t_atom *argv)
{
    binbuf_eval(x->m_text.te_binbuf, &x->m_messresponder.mr_pd, argc, argv);
}

static void message_set(t_message *x, t_symbol *s, int argc, t_atom *argv)
{
    binbuf_clear(x->m_text.te_binbuf);
    binbuf_add(x->m_text.te_binbuf, argc, argv);
    glist_retext(x->m_glist, &x->m_text);
}

static void message_add2(t_message *x, t_symbol *s, int argc, t_atom *argv)
{
    binbuf_add(x->m_text.te_binbuf, argc, argv);
    glist_retext(x->m_glist, &x->m_text);
}

static void message_add(t_message *x, t_symbol *s, int argc, t_atom *argv)
{
    binbuf_add(x->m_text.te_binbuf, argc, argv);
    binbuf_addsemi(x->m_text.te_binbuf);
    glist_retext(x->m_glist, &x->m_text);
}

static void message_addcomma(t_message *x)
{
    t_atom a;
    SETCOMMA(&a);
    binbuf_add(x->m_text.te_binbuf, 1, &a);
    glist_retext(x->m_glist, &x->m_text);
}

static void message_addsemi(t_message *x)
{
    message_add(x, 0, 0, 0);
}

static void message_adddollar(t_message *x, t_floatarg f)
{
    t_atom a;
    int n = f;
    if (n < 0)
        n = 0;
    SETDOLLAR(&a, n);
    binbuf_add(x->m_text.te_binbuf, 1, &a);
    glist_retext(x->m_glist, &x->m_text);
}

static void message_adddollsym(t_message *x, t_symbol *s)
{
    t_atom a;
    char buf[MAXPDSTRING];
    buf[0] = '$';
    strncpy(buf+1, s->s_name, MAXPDSTRING-2);
    buf[MAXPDSTRING-1] = 0;
    SETDOLLSYM(&a, gensym(buf));
    binbuf_add(x->m_text.te_binbuf, 1, &a);
    glist_retext(x->m_glist, &x->m_text);
}

static void message_click(t_message *x,
                          t_floatarg xpos, t_floatarg ypos, t_floatarg shift,
                          t_floatarg ctrl, t_floatarg alt)
{
    message_float(x, 0);
    if (glist_isvisible(x->m_glist))
    {
        /* not zooming click width for now as it gets too fat */
        t_rtext *y = glist_findrtext(x->m_glist, &x->m_text);
        sys_vgui(".x%lx.c itemconfigure %sR -width %d\n",
                 glist_getcanvas(x->m_glist), rtext_gettag(y), MESSAGE_CLICK_WIDTH);
        clock_delay(x->m_clock, 120);
    }
}

static void message_tick(t_message *x)
{
    if (glist_isvisible(x->m_glist))
    {
        t_rtext *y = glist_findrtext(x->m_glist, &x->m_text);
        sys_vgui(".x%lx.c itemconfigure %sR -width %d\n",
                 glist_getcanvas(x->m_glist), rtext_gettag(y),
                 glist_getzoom(x->m_glist));
    }
}

static void message_free(t_message *x)
{
    clock_free(x->m_clock);
}

void canvas_msg(t_glist *gl, t_symbol *s, int argc, t_atom *argv)
{
    t_message *x = (t_message *)pd_new(message_class);
    x->m_messresponder.mr_pd = messresponder_class;
    x->m_messresponder.mr_outlet = outlet_new(&x->m_text, &s_float);
    x->m_text.te_width = 0;                             /* don't know it yet. */
    x->m_text.te_type = T_MESSAGE;
    x->m_text.te_binbuf = binbuf_new();
    x->m_glist = gl;
    x->m_clock = clock_new(x, (t_method)message_tick);
    if (argc > 1)
    {
        x->m_text.te_xpix = atom_getfloatarg(0, argc, argv);
        x->m_text.te_ypix = atom_getfloatarg(1, argc, argv);
        if (argc > 2) binbuf_restore(x->m_text.te_binbuf, argc-2, argv+2);
        glist_add(gl, &x->m_text.te_g);
    }
    else if (!glist_isvisible(gl))
        post("unable to create stub message in closed canvas!");
    else
    {
        int connectme, xpix, ypix, indx, nobj;
        canvas_howputnew(gl, &connectme, &xpix, &ypix, &indx, &nobj);
        
        pd_vmess(&gl->gl_pd, gensym("editmode"), "i", 1);
        x->m_text.te_xpix = xpix;
        x->m_text.te_ypix = ypix;
        glist_add(gl, &x->m_text.te_g);
        glist_noselect2(gl);
        glist_select(gl, &x->m_text.te_g);
        gobj_activate(&x->m_text.te_g, gl, 1);
        if (connectme)
            canvas_connect(gl, indx, 0, nobj, 0);
        else canvas_startmotion(glist_getcanvas(gl));
    }
}

/* ---------------------- the "atom" text item ------------------------ */

#define ATOMBUFSIZE 40
#define ATOM_LABELLEFT 0
#define ATOM_LABELRIGHT 1
#define ATOM_LABELUP 2
#define ATOM_LABELDOWN 3

static t_class *gatom_class;

typedef struct _gatom
{
    t_text a_text;
    t_atom a_atom;          /* this holds the value and the type */
    t_glist *a_glist;       /* owning glist */
    t_float a_toggle;       /* value to toggle to */
    t_float a_draghi;       /* high end of drag range */
    t_float a_draglo;       /* low end of drag range */
    t_symbol *a_label;      /* symbol to show as label next to box */
    t_symbol *a_symfrom;    /* "receive" name -- bind ourselvs to this */
    t_symbol *a_symto;      /* "send" name -- send to this on output */
    char a_buf[ATOMBUFSIZE];/* string buffer for typing */
    char a_shift;           /* was shift key down when dragging started? */
    char a_wherelabel;      /* 0-3 for left, right, above, below */
    t_symbol *a_expanded_to; /* a_symto after $0, $1, ...  expansion */
} t_gatom;

/* prepend "-" as necessary to avoid empty strings, so we can
 use them in Pd messages.  A more complete solution would be
 to introduce some quoting mechanism; but then we'd be much more
 complicated. */
static t_symbol *gatom_escapit(t_symbol *s)
{
    if (!*s->s_name)
        return (gensym("-"));
    else if (*s->s_name == '-')
    {
        char shmo[100];
        shmo[0] = '-';
        strncpy(shmo+1, s->s_name, 99);
        shmo[99] = 0;
        return (gensym(shmo));
    }
    else return (iemgui_dollar2raute(s));
}

/* undo previous operation: strip leading "-" if found. */
static t_symbol *gatom_unescapit(t_symbol *s)
{
    if (*s->s_name == '-')
        return (gensym(s->s_name+1));
    else return (iemgui_raute2dollar(s));
}

static void gatom_redraw(t_gobj *client, t_glist *glist)
{
    t_gatom *x = (t_gatom *)client;
    glist_retext(x->a_glist, &x->a_text);
}

static void gatom_retext(t_gatom *x, int senditup)
{
    binbuf_clear(x->a_text.te_binbuf);
    binbuf_add(x->a_text.te_binbuf, 1, &x->a_atom);
    if (senditup && glist_isvisible(x->a_glist))
        sys_queuegui(x, x->a_glist, gatom_redraw);
}

#ifdef _MSC_VER
#include <float.h>
#define isnan _isnan
#endif

static void gatom_set(t_gatom *x, t_symbol *s, int argc, t_atom *argv)
{
    t_atom oldatom = x->a_atom;
    int changed = 0;
    if (!argc) return;
    if (x->a_atom.a_type == A_FLOAT)
    {
        x->a_atom.a_w.w_float = atom_getfloat(argv);
        changed = ((x->a_atom.a_w.w_float != oldatom.a_w.w_float));
        if (isnan(x->a_atom.a_w.w_float) != isnan(oldatom.a_w.w_float))
            changed = 1;
    }
    else if (x->a_atom.a_type == A_SYMBOL)
        x->a_atom.a_w.w_symbol = atom_getsymbol(argv),
        changed = (x->a_atom.a_w.w_symbol != oldatom.a_w.w_symbol);
    if (changed)
        gatom_retext(x, 1);
    x->a_buf[0] = 0;
}

static void gatom_bang(t_gatom *x)
{
    if (x->a_atom.a_type == A_FLOAT)
    {
        if (x->a_text.te_outlet)
            outlet_float(x->a_text.te_outlet, x->a_atom.a_w.w_float);
        if (*x->a_expanded_to->s_name && x->a_expanded_to->s_thing)
        {
            if (x->a_symto == x->a_symfrom)
                pd_error(x,
                         "%s: atom with same send/receive name (infinite loop)",
                         x->a_symto->s_name);
            else pd_float(x->a_expanded_to->s_thing, x->a_atom.a_w.w_float);
        }
    }
    else if (x->a_atom.a_type == A_SYMBOL)
    {
        if (x->a_text.te_outlet)
            outlet_symbol(x->a_text.te_outlet, x->a_atom.a_w.w_symbol);
        if (*x->a_symto->s_name && x->a_expanded_to->s_thing)
        {
            if (x->a_symto == x->a_symfrom)
                pd_error(x,
                         "%s: atom with same send/receive name (infinite loop)",
                         x->a_symto->s_name);
            else pd_symbol(x->a_expanded_to->s_thing, x->a_atom.a_w.w_symbol);
        }
    }
}

static void gatom_float(t_gatom *x, t_float f)
{
    t_atom at;
    SETFLOAT(&at, f);
    gatom_set(x, 0, 1, &at);
    gatom_bang(x);
}

static void gatom_clipfloat(t_gatom *x, t_float f)
{
    if (x->a_draglo != 0 || x->a_draghi != 0)
    {
        if (f < x->a_draglo)
            f = x->a_draglo;
        if (f > x->a_draghi)
            f = x->a_draghi;
    }
    gatom_float(x, f);
}

static void gatom_symbol(t_gatom *x, t_symbol *s)
{
    t_atom at;
    SETSYMBOL(&at, s);
    gatom_set(x, 0, 1, &at);
    gatom_bang(x);
}

/* We need a list method because, since there's both an "inlet" and a
 "nofirstin" flag, the standard list behavior gets confused. */
static void gatom_list(t_gatom *x, t_symbol *s, int argc, t_atom *argv)
{
    if (!argc)
        gatom_bang(x);
    else if (argv->a_type == A_FLOAT)
        gatom_float(x, argv->a_w.w_float);
    else if (argv->a_type == A_SYMBOL)
        gatom_symbol(x, argv->a_w.w_symbol);
    else pd_error(x, "gatom_list: need float or symbol");
}

static void gatom_motion(void *z, t_floatarg dx, t_floatarg dy)
{
    t_gatom *x = (t_gatom *)z;
    if (dy == 0) return;
    if (x->a_atom.a_type == A_FLOAT)
    {
        if (x->a_shift)
        {
            double nval = x->a_atom.a_w.w_float - 0.01 * dy;
            double trunc = 0.01 * (floor(100. * nval + 0.5));
            if (trunc < nval + 0.0001 && trunc > nval - 0.0001) nval = trunc;
            gatom_clipfloat(x, nval);
        }
        else
        {
            double nval = x->a_atom.a_w.w_float - dy;
            double trunc = 0.01 * (floor(100. * nval + 0.5));
            if (trunc < nval + 0.0001 && trunc > nval - 0.0001) nval = trunc;
            trunc = floor(nval + 0.5);
            if (trunc < nval + 0.001 && trunc > nval - 0.001) nval = trunc;
            gatom_clipfloat(x, nval);
        }
    }
}

static void gatom_key(void *z, t_floatarg f)
{
    t_gatom *x = (t_gatom *)z;
    int c = f;
    int len = (int)strlen(x->a_buf);
    t_atom at;
    char sbuf[ATOMBUFSIZE + 4];
    if (c == 0)
    {
        /* we're being notified that no more keys will come for this grab */
        if (x->a_buf[0])
            gatom_retext(x, 1);
        return;
    }
    else if (c == '\b')
    {
        if (len > 0)
            x->a_buf[len-1] = 0;
        goto redraw;
    }
    else if (c == '\n')
    {
        if (x->a_atom.a_type == A_FLOAT)
            x->a_atom.a_w.w_float = atof(x->a_buf);
        else if (x->a_atom.a_type == A_SYMBOL)
            x->a_atom.a_w.w_symbol = gensym(x->a_buf);
        else bug("gatom_key");
        gatom_bang(x);
        gatom_retext(x, 1);
        x->a_buf[0] = 0;
    }
    else if (len < (ATOMBUFSIZE-1))
    {
        /* for numbers, only let reasonable characters through */
        if ((x->a_atom.a_type == A_SYMBOL) ||
            ((c >= '0' && c <= '9') || c == '.' || c == '-'
             || c == 'e' || c == 'E'))
        {
            /* the wchar could expand to up to 4 bytes, which
             * which might overrun our a_buf;
             * therefore we first expand into a temporary buffer,
             * and only if the resulting utf8 string fits into a_buf
             * we apply it
             */
            char utf8[UTF8_MAXBYTES];
            int utf8len = u8_wc_toutf8(utf8, c);
            if((len+utf8len) < (ATOMBUFSIZE-1))
            {
                int j=0;
                for(j=0; j<utf8len; j++)
                    x->a_buf[len+j] = utf8[j];
                
                x->a_buf[len+utf8len] = 0;
            }
            goto redraw;
        }
    }
    return;
redraw:
    /* LATER figure out how to avoid creating all these symbols! */
    sprintf(sbuf, "%s...", x->a_buf);
    SETSYMBOL(&at, gensym(sbuf));
    binbuf_clear(x->a_text.te_binbuf);
    binbuf_add(x->a_text.te_binbuf, 1, &at);
    glist_retext(x->a_glist, &x->a_text);
}

static void gatom_click(t_gatom *x,
                        t_floatarg xpos, t_floatarg ypos, t_floatarg shift, t_floatarg ctrl,
                        t_floatarg alt)
{
    if (x->a_text.te_width == 1)
    {
        if (x->a_atom.a_type == A_FLOAT)
            gatom_float(x, (x->a_atom.a_w.w_float == 0));
    }
    else
    {
        if (alt)
        {
            if (x->a_atom.a_type != A_FLOAT) return;
            if (x->a_atom.a_w.w_float != 0)
            {
                x->a_toggle = x->a_atom.a_w.w_float;
                gatom_float(x, 0);
                return;
            }
            else gatom_float(x, x->a_toggle);
        }
        x->a_shift = shift;
        x->a_buf[0] = 0;
        glist_grab(x->a_glist, &x->a_text.te_g, gatom_motion, gatom_key,
                   xpos, ypos);
    }
}

/* message back from dialog window */
static void gatom_param(t_gatom *x, t_symbol *sel, int argc, t_atom *argv)
{
    t_float width = atom_getfloatarg(0, argc, argv);
    t_float draglo = atom_getfloatarg(1, argc, argv);
    t_float draghi = atom_getfloatarg(2, argc, argv);
    t_symbol *label = gatom_unescapit(atom_getsymbolarg(3, argc, argv));
    t_float wherelabel = atom_getfloatarg(4, argc, argv);
    t_symbol *symfrom = gatom_unescapit(atom_getsymbolarg(5, argc, argv));
    t_symbol *symto = gatom_unescapit(atom_getsymbolarg(6, argc, argv));
    
    gobj_vis(&x->a_text.te_g, x->a_glist, 0);
    if (!*symfrom->s_name && *x->a_symfrom->s_name)
        inlet_new(&x->a_text, &x->a_text.te_pd, 0, 0);
    else if (*symfrom->s_name && !*x->a_symfrom->s_name && x->a_text.te_inlet)
    {
        canvas_deletelinesforio(x->a_glist, &x->a_text,
                                x->a_text.te_inlet, 0);
        inlet_free(x->a_text.te_inlet);
    }
    if (!*symto->s_name && *x->a_symto->s_name)
        outlet_new(&x->a_text, 0);
    else if (*symto->s_name && !*x->a_symto->s_name && x->a_text.te_outlet)
    {
        canvas_deletelinesforio(x->a_glist, &x->a_text,
                                0, x->a_text.te_outlet);
        outlet_free(x->a_text.te_outlet);
    }
    if (draglo >= draghi)
        draglo = draghi = 0;
    x->a_draglo = draglo;
    x->a_draghi = draghi;
    if (width < 0)
        width = 4;
    else if (width > 80)
        width = 80;
    x->a_text.te_width = width;
    x->a_wherelabel = ((int)wherelabel & 3);
    x->a_label = label;
    if (*x->a_symfrom->s_name)
        pd_unbind(&x->a_text.te_pd,
                  canvas_realizedollar(x->a_glist, x->a_symfrom));
    x->a_symfrom = symfrom;
    if (*x->a_symfrom->s_name)
        pd_bind(&x->a_text.te_pd,
                canvas_realizedollar(x->a_glist, x->a_symfrom));
    x->a_symto = symto;
    x->a_expanded_to = canvas_realizedollar(x->a_glist, x->a_symto);
    gobj_vis(&x->a_text.te_g, x->a_glist, 1);
    canvas_dirty(x->a_glist, 1);
    
    /* glist_retext(x->a_glist, &x->a_text); */
}

/* -------------------- widget behavior for text objects ------------ */

static void text_getrect(t_gobj *z, t_glist *glist,
                         int *xp1, int *yp1, int *xp2, int *yp2)
{
    t_text *x = (t_text *)z;
    int width, height, iscomment = (x->te_type == T_TEXT);
    t_float x1, y1, x2, y2;
    
    /* for number boxes, we know width and height a priori, and should
     report them here so that graphs can get swelled to fit. */
    
    if (x->te_type == T_ATOM && x->te_width > 0)
    {
        width = (x->te_width > 0 ? x->te_width : 6) * glist_fontwidth(glist);
        height = glist_fontheight(glist);
        if (glist_getzoom(glist) > 1)
        {
            /* zoom margins */
            width += ATOM_RMARGIN * glist_getzoom(glist);
            height += ATOM_BMARGIN * glist_getzoom(glist);
        }
        else
        {
            width += ATOM_RMARGIN;
            height += ATOM_BMARGIN;
        }
    }
    /* if we're invisible we don't know our size so we just lie about
     it.  This is called on invisible boxes to establish order of inlets
     and possibly other reasons.
     To find out if the box is visible we can't just check the "vis"
     flag because we might be within the vis() routine and not have set
     that yet.  So we check directly whether the "rtext" list has been
     built.  LATER reconsider when "vis" flag should be on and off? */
    
    else if (glist->gl_editor && glist->gl_editor->e_rtext)
    {
        t_rtext *y = glist_findrtext(glist, x);
        width = rtext_width(y);
        height = rtext_height(y) - (iscomment << 1);
    }
    else width = height = 10;
    x1 = text_xpix(x, glist);
    y1 = text_ypix(x, glist);
    x2 = x1 + width;
    y2 = y1 + height;
    y1 += iscomment;
    *xp1 = x1;
    *yp1 = y1;
    *xp2 = x2;
    *yp2 = y2;
}

static void text_displace(t_gobj *z, t_glist *glist,
                          int dx, int dy)
{
    t_text *x = (t_text *)z;
    x->te_xpix += dx;
    x->te_ypix += dy;
    if (glist_isvisible(glist))
    {
        t_rtext *y = glist_findrtext(glist, x);
        rtext_displace(y, dx, dy);
        text_drawborder(x, glist, rtext_gettag(y),
                        rtext_width(y), rtext_height(y), 0);
        canvas_fixlinesfor(glist, x);
    }
}

static void text_select(t_gobj *z, t_glist *glist, int state)
{
    t_text *x = (t_text *)z;
    t_rtext *y = glist_findrtext(glist, x);
    rtext_select(y, state);
    if (glist_isvisible(glist) && gobj_shouldvis(&x->te_g, glist))
        sys_vgui(".x%lx.c itemconfigure %sR -fill %s\n", glist,
                 rtext_gettag(y), (state? "blue" : "black"));
}

static void text_activate(t_gobj *z, t_glist *glist, int state)
{
    t_text *x = (t_text *)z;
    t_rtext *y = glist_findrtext(glist, x);
    if (z->g_pd != gatom_class) rtext_activate(y, state);
}

static void text_delete(t_gobj *z, t_glist *glist)
{
    t_text *x = (t_text *)z;
    canvas_deletelinesfor(glist, x);
}

static void text_vis(t_gobj *z, t_glist *glist, int vis)
{
    t_text *x = (t_text *)z;
    if (vis)
    {
        if (gobj_shouldvis(&x->te_g, glist))
        {
            t_rtext *y = glist_findrtext(glist, x);
            if (x->te_type == T_ATOM)
                glist_retext(glist, x);
            text_drawborder(x, glist, rtext_gettag(y),
                            rtext_width(y), rtext_height(y), 1);
            rtext_draw(y);
        }
    }
    else
    {
        t_rtext *y = glist_findrtext(glist, x);
        if (gobj_shouldvis(&x->te_g, glist))
        {
            text_eraseborder(x, glist, rtext_gettag(y));
            rtext_erase(y);
        }
    }
}

static int text_click(t_gobj *z, struct _glist *glist,
                      int xpix, int ypix, int shift, int alt, int dbl, int doit)
{
    t_text *x = (t_text *)z;
    if (x->te_type == T_OBJECT)
    {
        t_symbol *clicksym = gensym("click");
        if (zgetfn(&x->te_pd, clicksym))
        {
            if (doit)
                pd_vmess(&x->te_pd, clicksym, "fffff",
                         (double)xpix, (double)ypix,
                         (double)shift, (double)0, (double)alt);
            return (1);
        }
        else return (0);
    }
    else if (x->te_type == T_ATOM)
    {
        if (doit)
            gatom_click((t_gatom *)x, (t_floatarg)xpix, (t_floatarg)ypix,
                        (t_floatarg)shift, (t_floatarg)0, (t_floatarg)alt);
        return (1);
    }
    else if (x->te_type == T_MESSAGE)
    {
        if (doit)
            message_click((t_message *)x, (t_floatarg)xpix, (t_floatarg)ypix,
                          (t_floatarg)shift, (t_floatarg)0, (t_floatarg)alt);
        return (1);
    }
    else return (0);
}

void text_save(t_gobj *z, t_binbuf *b)
{
    t_text *x = (t_text *)z;
    if (x->te_type == T_OBJECT)
    {
        /* if we have a "saveto" method, and if we don't happen to be
         a canvas that's an abstraction, the saveto method does the work */
        if (zgetfn(&x->te_pd, gensym("saveto")) &&
            !((pd_class(&x->te_pd) == canvas_class) &&
              (canvas_isabstraction((t_canvas *)x)
               || canvas_istable((t_canvas *)x))))
        {
            mess1(&x->te_pd, gensym("saveto"), b);
            binbuf_addv(b, "ssii", gensym("#X"), gensym("restore"),
                        (int)x->te_xpix, (int)x->te_ypix);
        }
        else    /* otherwise just save the text */
        {
            binbuf_addv(b, "ssii", gensym("#X"), gensym("obj"),
                        (int)x->te_xpix, (int)x->te_ypix);
        }
        binbuf_addbinbuf(b, x->te_binbuf);
    }
    else if (x->te_type == T_MESSAGE)
    {
        binbuf_addv(b, "ssii", gensym("#X"), gensym("msg"),
                    (int)x->te_xpix, (int)x->te_ypix);
        binbuf_addbinbuf(b, x->te_binbuf);
    }
    else if (x->te_type == T_ATOM)
    {
        t_atomtype t = ((t_gatom *)x)->a_atom.a_type;
        t_symbol *sel = (t == A_SYMBOL ? gensym("symbolatom") :
                         (t == A_FLOAT ? gensym("floatatom") : gensym("intatom")));
        t_symbol *label = gatom_escapit(((t_gatom *)x)->a_label);
        t_symbol *symfrom = gatom_escapit(((t_gatom *)x)->a_symfrom);
        t_symbol *symto = gatom_escapit(((t_gatom *)x)->a_symto);
        binbuf_addv(b, "ssiiifffsss", gensym("#X"), sel,
                    (int)x->te_xpix, (int)x->te_ypix, (int)x->te_width,
                    (double)((t_gatom *)x)->a_draglo,
                    (double)((t_gatom *)x)->a_draghi,
                    (double)((t_gatom *)x)->a_wherelabel,
                    label, symfrom, symto);
    }
    else
    {
        binbuf_addv(b, "ssii", gensym("#X"), gensym("text"),
                    (int)x->te_xpix, (int)x->te_ypix);
        binbuf_addbinbuf(b, x->te_binbuf);
    }
    if (x->te_width)
        binbuf_addv(b, ",si", gensym("f"), (int)x->te_width);
    binbuf_addv(b, ";");
}

/* ---------------- gatom-specific widget functions --------------- */
static void gatom_getwherelabel(t_gatom *x, t_glist *glist, int *xp, int *yp)
{
    int x1, y1, x2, y2, width, height;
    text_getrect(&x->a_text.te_g, glist, &x1, &y1, &x2, &y2);
    width = x2 - x1;
    height = y2 - y1;
    if (x->a_wherelabel == ATOM_LABELLEFT)
    {
        *xp = x1 - 3 -
        (int)strlen(canvas_realizedollar(x->a_glist, x->a_label)->s_name) *
        glist_fontwidth(glist);
        *yp = y1 + 2;
    }
    else if (x->a_wherelabel == ATOM_LABELRIGHT)
    {
        *xp = x2 + 2;
        *yp = y1 + 2;
    }
    else if (x->a_wherelabel == ATOM_LABELUP)
    {
        *xp = x1 - 1;
        *yp = y1 - 1 - glist_fontheight(glist);
    }
    else
    {
        *xp = x1 - 1;
        *yp = y2 + 3;
    }
}

static void gatom_displace(t_gobj *z, t_glist *glist,
                           int dx, int dy)
{
    t_gatom *x = (t_gatom*)z;
    text_displace(z, glist, dx, dy);
    sys_vgui(".x%lx.c move %lx.l %d %d\n", glist_getcanvas(glist),
             x, dx, dy);
}

static void gatom_vis(t_gobj *z, t_glist *glist, int vis)
{
    t_gatom *x = (t_gatom*)z;
    text_vis(z, glist, vis);
    if (*x->a_label->s_name)
    {
        if (vis)
        {
            int x1, y1;
            gatom_getwherelabel(x, glist, &x1, &y1);
            sys_vgui("pdtk_text_new .x%lx.c {%lx.l label text} %f %f {%s} %d %s\n",
                     glist_getcanvas(glist), x,
                     (double)x1, (double)y1,
                     canvas_realizedollar(x->a_glist, x->a_label)->s_name,
                     sys_hostfontsize(glist_getfont(glist), glist_getzoom(glist)),
                     "black");
        }
        else sys_vgui(".x%lx.c delete %lx.l\n", glist_getcanvas(glist), x);
    }
    if (!vis)
        sys_unqueuegui(x);
}

static const t_widgetbehavior gatom_widgetbehavior =
{
    text_getrect,
    gatom_displace,
    text_select,
    text_activate,
    text_delete,
    gatom_vis,
    text_click,
};

void canvas_atom(t_glist *gl, t_atomtype type,
                 t_symbol *s, int argc, t_atom *argv)
{
    t_gatom *x = (t_gatom *)pd_new(gatom_class);
    t_atom at;
    x->a_text.te_width = 0;                        /* don't know it yet. */
    x->a_text.te_type = T_ATOM;
    x->a_text.te_binbuf = binbuf_new();
    x->a_glist = gl;
    x->a_atom.a_type = type;
    x->a_toggle = 1;
    x->a_draglo = 0;
    x->a_draghi = 0;
    x->a_wherelabel = 0;
    x->a_label = &s_;
    x->a_symfrom = &s_;
    x->a_symto = x->a_expanded_to = &s_;
    if (type == A_FLOAT)
    {
        x->a_atom.a_w.w_float = 0;
        x->a_text.te_width = 5;
        SETFLOAT(&at, 0);
    }
    else
    {
        x->a_atom.a_w.w_symbol = &s_symbol;
        x->a_text.te_width = 10;
        SETSYMBOL(&at, &s_symbol);
    }
    binbuf_add(x->a_text.te_binbuf, 1, &at);
    if (argc > 1)
    /* create from file. x, y, width, low-range, high-range, flags,
     label, receive-name, send-name */
    {
        x->a_text.te_xpix = atom_getfloatarg(0, argc, argv);
        x->a_text.te_ypix = atom_getfloatarg(1, argc, argv);
        x->a_text.te_width = atom_getfloatarg(2, argc, argv);
        /* sanity check because some very old patches have trash in this
         field... remove this in 2003 or so: */
        if (x->a_text.te_width < 0 || x->a_text.te_width > 500)
            x->a_text.te_width = 4;
        x->a_draglo = atom_getfloatarg(3, argc, argv);
        x->a_draghi = atom_getfloatarg(4, argc, argv);
        x->a_wherelabel = (((int)atom_getfloatarg(5, argc, argv)) & 3);
        x->a_label = gatom_unescapit(atom_getsymbolarg(6, argc, argv));
        x->a_symfrom = gatom_unescapit(atom_getsymbolarg(7, argc, argv));
        if (*x->a_symfrom->s_name)
            pd_bind(&x->a_text.te_pd,
                    canvas_realizedollar(x->a_glist, x->a_symfrom));
        
        x->a_symto = gatom_unescapit(atom_getsymbolarg(8, argc, argv));
        x->a_expanded_to = canvas_realizedollar(x->a_glist, x->a_symto);
        if (x->a_symto == &s_)
            outlet_new(&x->a_text,
                       x->a_atom.a_type == A_FLOAT ? &s_float: &s_symbol);
        if (x->a_symfrom == &s_)
            inlet_new(&x->a_text, &x->a_text.te_pd, 0, 0);
        glist_add(gl, &x->a_text.te_g);
    }
    else
    {
        int connectme, xpix, ypix, indx, nobj;
        canvas_howputnew(gl, &connectme, &xpix, &ypix, &indx, &nobj);
        outlet_new(&x->a_text,
                   x->a_atom.a_type == A_FLOAT ? &s_float: &s_symbol);
        inlet_new(&x->a_text, &x->a_text.te_pd, 0, 0);
        pd_vmess(&gl->gl_pd, gensym("editmode"), "i", 1);
        x->a_text.te_xpix = xpix;
        x->a_text.te_ypix = ypix;
        glist_add(gl, &x->a_text.te_g);
        glist_noselect2(gl);
        glist_select(gl, &x->a_text.te_g);
        if (connectme)
            canvas_connect(gl, indx, 0, nobj, 0);
        else canvas_startmotion(glist_getcanvas(gl));
    }
}

void canvas_floatatom(t_glist *gl, t_symbol *s, int argc, t_atom *argv)
{
    canvas_atom(gl, A_FLOAT, s, argc, argv);
}

void canvas_symbolatom(t_glist *gl, t_symbol *s, int argc, t_atom *argv)
{
    canvas_atom(gl, A_SYMBOL, s, argc, argv);
}

static void gatom_free(t_gatom *x)
{
    if (*x->a_symfrom->s_name)
        pd_unbind(&x->a_text.te_pd,
                  canvas_realizedollar(x->a_glist, x->a_symfrom));
    gfxstub_deleteforkey(x);
}

static void gatom_properties(t_gobj *z, t_glist *owner)
{
    t_gatom *x = (t_gatom *)z;
    char buf[200];
    sprintf(buf, "pdtk_gatom_dialog %%s %d %g %g %d {%s} {%s} {%s}\n",
            x->a_text.te_width, x->a_draglo, x->a_draghi,
            x->a_wherelabel, gatom_escapit(x->a_label)->s_name,
            gatom_escapit(x->a_symfrom)->s_name,
            gatom_escapit(x->a_symto)->s_name);
    gfxstub_new(&x->a_text.te_pd, x, buf);
}

/* ----------------- the "text" object.  ------------------ */

/* add a "text" object (comment) to a glist.  While this one goes for any
 glist, the other 3 below are for canvases only.  (why?)  This is called
 without args if invoked from the GUI; otherwise at least x and y
 are provided.  */

void glist_text(t_glist *gl, t_symbol *s, int argc, t_atom *argv)
{
    t_text *x = (t_text *)pd_new(text_class);
    t_atom at;
    x->te_width = 0;                            /* don't know it yet. */
    x->te_type = T_TEXT;
    x->te_binbuf = binbuf_new();
    if (argc > 1)
    {
        x->te_xpix = atom_getfloatarg(0, argc, argv);
        x->te_ypix = atom_getfloatarg(1, argc, argv);
        if (argc > 2) binbuf_restore(x->te_binbuf, argc-2, argv+2);
        else
        {
            SETSYMBOL(&at, gensym("comment"));
            binbuf_restore(x->te_binbuf, 1, &at);
        }
        glist_add(gl, &x->te_g);
    }
    else
    {
        int xpix, ypix;
        pd_vmess((t_pd *)glist_getcanvas(gl), gensym("editmode"), "i", 1);
        SETSYMBOL(&at, gensym("comment"));
        glist_noselect2(gl);
        glist_getnextxy(gl, &xpix, &ypix);
        x->te_xpix = xpix-1;
        x->te_ypix = ypix-1;
        binbuf_restore(x->te_binbuf, 1, &at);
        glist_add(gl, &x->te_g);
        glist_noselect2(gl);
        glist_select(gl, &x->te_g);
        /* it would be nice to "activate" here, but then the second,
         "put-me-down" click changes the text selection, which is quite
         irritating, so I took this back out.  It's OK in messages
         and objects though since there's no text in them at menu
         creation. */
        /* gobj_activate(&x->te_g, gl, 1); */
        canvas_startmotion(glist_getcanvas(gl));
    }
}

/* Pd method to create a new scalar, add it to a glist, and initialize
 it from the message arguments. */
void glist_scalar(t_glist *glist,
                  t_symbol *classname, int argc, t_atom *argv)
{
    t_symbol *templatesym =
    canvas_makebindsym(atom_getsymbolarg(0, argc, argv));
    t_binbuf *b;
    int natoms, nextmsg = 0;
    t_atom *vec;
    if (!template_findbyname(templatesym))
    {
        pd_error(glist, "%s: no such template",
                 atom_getsymbolarg(0, argc, argv)->s_name);
        return;
    }
    
    b = binbuf_new();
    binbuf_restore(b, argc, argv);
    natoms = binbuf_getnatom(b);
    vec = binbuf_getvec(b);
    canvas_readscalar(glist, natoms, vec, &nextmsg, 0);
    binbuf_free(b);
}

/* remove every object from a glist.  Experimental. */
void glist_clear2(t_glist *x)
{
    t_gobj *y, *y2;
    int dspstate = 0, suspended = 0;
    t_symbol *dspsym = gensym("dsp");
    while ((y = x->gl_list))
    {
        /* to avoid unnecessary DSP resorting, we suspend DSP
         only if we hit a patchable object. */
        if (!suspended && pd_checkobject(&y->g_pd) && zgetfn(&y->g_pd, dspsym))
        {
            dspstate = canvas_suspend_dsp();
            suspended = 1;
        }
        /* here's the real deletion. */
        waitForRenderingToFinish();
        glist_delete(x, y);
        bObjectCleared = true;
    }
    if (suspended)
        canvas_resume_dsp(dspstate);
}

/* this gets called when a message gets sent to an object whose creation
 failed, presumably because of loading a patch with a missing extern or
 abstraction */
static void text_anything(t_text *x, t_symbol *s, int argc, t_atom *argv)
{
}

void ofeliaOverwrites_setup()
{
    text_class = class_new(gensym("text"), 0, 0, sizeof(t_text),
                           CLASS_NOINLET | CLASS_PATCHABLE, 0);
    class_addanything(text_class, text_anything);
    
    message_class = class_new(gensym("message"), 0, (t_method)message_free,
                              sizeof(t_message), CLASS_PATCHABLE, 0);
    class_addbang(message_class, message_bang);
    class_addfloat(message_class, message_float);
    class_addsymbol(message_class, message_symbol);
    class_addlist(message_class, message_list);
    class_addanything(message_class, message_list);
    
    class_addmethod(message_class, (t_method)message_click, gensym("click"),
                    A_FLOAT, A_FLOAT, A_FLOAT, A_FLOAT, A_FLOAT, 0);
    class_addmethod(message_class, (t_method)message_set, gensym("set"),
                    A_GIMME, 0);
    class_addmethod(message_class, (t_method)message_add, gensym("add"),
                    A_GIMME, 0);
    class_addmethod(message_class, (t_method)message_add2, gensym("add2"),
                    A_GIMME, 0);
    class_addmethod(message_class, (t_method)message_addcomma,
                    gensym("addcomma"), 0);
    class_addmethod(message_class, (t_method)message_addsemi,
                    gensym("addsemi"), 0);
    class_addmethod(message_class, (t_method)message_adddollar,
                    gensym("adddollar"), A_FLOAT, 0);
    class_addmethod(message_class, (t_method)message_adddollsym,
                    gensym("adddollsym"), A_SYMBOL, 0);
    
    messresponder_class = class_new(gensym("messresponder"), 0, 0,
                                    sizeof(t_text), CLASS_PD, 0);
    class_addbang(messresponder_class, messresponder_bang);
    class_addfloat(messresponder_class, (t_method) messresponder_float);
    class_addsymbol(messresponder_class, messresponder_symbol);
    class_addlist(messresponder_class, messresponder_list);
    class_addanything(messresponder_class, messresponder_anything);
    
    gatom_class = class_new(gensym("gatom"), 0, (t_method)gatom_free,
                            sizeof(t_gatom), CLASS_NOINLET | CLASS_PATCHABLE, 0);
    class_addbang(gatom_class, gatom_bang);
    class_addfloat(gatom_class, gatom_float);
    class_addsymbol(gatom_class, gatom_symbol);
    class_addlist(gatom_class, gatom_list);
    class_addmethod(gatom_class, (t_method)gatom_set, gensym("set"),
                    A_GIMME, 0);
    class_addmethod(gatom_class, (t_method)gatom_click, gensym("click"),
                    A_FLOAT, A_FLOAT, A_FLOAT, A_FLOAT, A_FLOAT, 0);
    class_addmethod(gatom_class, (t_method)gatom_param, gensym("param"),
                    A_GIMME, 0);
    class_setwidget(gatom_class, &gatom_widgetbehavior);
    class_setpropertiesfn(gatom_class, gatom_properties);
    
    /* -------------------------- objects ----------------------------- */
    class_addmethod(canvas_class, (t_method)canvas_obj,
                    gensym("obj"), A_GIMME, A_NULL);
    class_addmethod(canvas_class, (t_method)canvas_msg,
                    gensym("msg"), A_GIMME, A_NULL);
    class_addmethod(canvas_class, (t_method)canvas_floatatom,
                    gensym("floatatom"), A_GIMME, A_NULL);
    class_addmethod(canvas_class, (t_method)canvas_symbolatom,
                    gensym("symbolatom"), A_GIMME, A_NULL);
    class_addmethod(canvas_class, (t_method)glist_text,
                    gensym("text"), A_GIMME, A_NULL);
    class_addmethod(canvas_class, (t_method)glist_glist, gensym("graph"),
                    A_GIMME, A_NULL);
    class_addmethod(canvas_class, (t_method)glist_scalar,
                    gensym("scalar"), A_GIMME, A_NULL);
    
    /* ---------------------- list handling ------------------------ */
    class_addmethod(canvas_class, (t_method)glist_clear2, gensym("clear"),
                    A_NULL);

    /* -------------- IEMGUI: button, toggle, slider, etc.  ------------ */
    class_addmethod(canvas_class, (t_method)canvas_bng, gensym("bng"),
                    A_GIMME, A_NULL);
    class_addmethod(canvas_class, (t_method)canvas_toggle, gensym("toggle"),
                    A_GIMME, A_NULL);
    class_addmethod(canvas_class, (t_method)canvas_vslider, gensym("vslider"),
                    A_GIMME, A_NULL);
    class_addmethod(canvas_class, (t_method)canvas_hslider, gensym("hslider"),
                    A_GIMME, A_NULL);
    class_addmethod(canvas_class, (t_method)canvas_hdial, gensym("hdial"),
                    A_GIMME, A_NULL);
    class_addmethod(canvas_class, (t_method)canvas_vdial, gensym("vdial"),
                    A_GIMME, A_NULL);
    class_addmethod(canvas_class, (t_method)canvas_hradio, gensym("hradio"),
                    A_GIMME, A_NULL);
    class_addmethod(canvas_class, (t_method)canvas_vradio, gensym("vradio"),
                    A_GIMME, A_NULL);
    class_addmethod(canvas_class, (t_method)canvas_vumeter, gensym("vumeter"),
                    A_GIMME, A_NULL);
    class_addmethod(canvas_class, (t_method)canvas_mycnv, gensym("mycnv"),
                    A_GIMME, A_NULL);
    class_addmethod(canvas_class, (t_method)canvas_numbox, gensym("numbox"),
                    A_GIMME, A_NULL);

    /* ------------------------ events ---------------------------------- */
    class_addmethod(canvas_class, (t_method)canvas_mouse2, gensym("mouse"),
                    A_FLOAT, A_FLOAT, A_FLOAT, A_FLOAT, A_NULL);
    class_addmethod(canvas_class, (t_method)canvas_mouseup, gensym("mouseup"),
                    A_FLOAT, A_FLOAT, A_FLOAT, A_NULL);
    class_addmethod(canvas_class, (t_method)canvas_key2, gensym("key"),
                    A_GIMME, A_NULL);
    class_addmethod(canvas_class, (t_method)canvas_motion, gensym("motion"),
                    A_FLOAT, A_FLOAT, A_FLOAT, A_NULL);
    
    /* ------------------------ menu actions ---------------------------- */
    class_addmethod(glob_pdobject, (t_method)glob_verifyquit2,
                    gensym("verifyquit"), A_DEFFLOAT, 0);
    class_addmethod(canvas_class, (t_method)canvas_menuclose2,
                    gensym("menuclose"), A_DEFFLOAT, 0);
    class_addmethod(canvas_class, (t_method)canvas_cut2,
                    gensym("cut"), A_NULL);
    class_addmethod(canvas_class, (t_method)canvas_copy,
                    gensym("copy"), A_NULL);
    class_addmethod(canvas_class, (t_method)canvas_paste,
                    gensym("paste"), A_NULL);
    class_addmethod(canvas_class, (t_method)canvas_duplicate,
                    gensym("duplicate"), A_NULL);
    class_addmethod(canvas_class, (t_method)canvas_selectall,
                    gensym("selectall"), A_NULL);
    class_addmethod(canvas_class, (t_method)canvas_reselect,
                    gensym("reselect"), A_NULL);
    class_addmethod(canvas_class, (t_method)canvas_undo,
                    gensym("undo"), A_NULL);
    class_addmethod(canvas_class, (t_method)canvas_redo,
                    gensym("redo"), A_NULL);
    class_addmethod(canvas_class, (t_method)canvas_tidy,
                    gensym("tidy"), A_NULL);
    class_addmethod(canvas_class, (t_method)canvas_texteditor,
                    gensym("texteditor"), A_NULL);
    class_addmethod(canvas_class, (t_method)canvas_editmode,
                    gensym("editmode"), A_DEFFLOAT, A_NULL);
    class_addmethod(canvas_class, (t_method)canvas_print,
                    gensym("print"), A_SYMBOL, A_NULL);
    class_addmethod(canvas_class, (t_method)canvas_menufont,
                    gensym("menufont"), A_NULL);
    class_addmethod(canvas_class, (t_method)canvas_font,
                    gensym("font"), A_FLOAT, A_FLOAT, A_FLOAT, A_NULL);
    class_addmethod(canvas_class, (t_method)canvas_zoom,
                    gensym("zoom"), A_FLOAT, A_NULL);
    class_addmethod(canvas_class, (t_method)canvas_find,
                    gensym("find"), A_SYMBOL, A_FLOAT, A_NULL);
    class_addmethod(canvas_class, (t_method)canvas_find_again,
                    gensym("findagain"), A_NULL);
    class_addmethod(canvas_class, (t_method)canvas_find_parent,
                    gensym("findparent"), A_NULL);
    class_addmethod(canvas_class, (t_method)canvas_done_popup,
                    gensym("done-popup"), A_FLOAT, A_FLOAT, A_FLOAT, A_NULL);
    class_addmethod(canvas_class, (t_method)canvas_donecanvasdialog,
                    gensym("donecanvasdialog"), A_GIMME, A_NULL);
    class_addmethod(canvas_class, (t_method)glist_arraydialog,
                    gensym("arraydialog"), A_SYMBOL, A_FLOAT, A_FLOAT, A_FLOAT, A_NULL);
    
    /* -------------- connect method used in reading files ------------------ */
    class_addmethod(canvas_class, (t_method)canvas_connect,
                    gensym("connect"), A_FLOAT, A_FLOAT, A_FLOAT, A_FLOAT, A_NULL);
    
    class_addmethod(canvas_class, (t_method)canvas_disconnect,
                    gensym("disconnect"), A_FLOAT, A_FLOAT, A_FLOAT, A_FLOAT, A_NULL);
    
    /* -------------- copy buffer ------------------ */
    EDITOR->copy_binbuf = binbuf_new();
}

