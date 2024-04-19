# Copyright (c) 2002-2012 krzYszcz and others.
# For information on usage and redistribution, and for a DISCLAIMER OF ALL
# WARRANTIES, see the file, "LICENSE.txt," in this distribution.  */

proc ofelia_textwindow_open {name geometry title font} {
 if {[winfo exists $name]} {
        $name.text delete 1.0 end
    } else {
        toplevel $name
        wm title $name $title
        wm geometry $name $geometry
        wm protocol $name WM_DELETE_WINDOW \
            [concat ofelia_textwindow_close $name 1]
        bind $name <<Modified>> "ofelia_textwindow_dodirty $name"
        text $name.text -relief raised -highlightthickness 0 -bd 2 \
            -font [get_font_for_size $font] \
            -yscrollcommand "$name.scroll set" -background white
        scrollbar $name.scroll -command "$name.text yview"
        pack $name.scroll -side right -fill y
        pack $name.text -side left -fill both -expand 1
        bind $name.text <$::modifier-Key-s> "ofelia_textwindow_send $name"
        bind $name.text <$::modifier-Key-w> "ofelia_textwindow_close $name 1"
        focus $name.text
    }
}

proc ofelia_textwindow_dodirty {name} {
    if {[catch {$name.text edit modified} dirty]} {set dirty 1}
    set title [wm title $name]
    set dt [string equal -length 1 $title "*"]
    if {$dirty} {
        if {$dt == 0} {wm title $name *$title}
    } else {
        if {$dt} {wm title $name [string range $title 1 end]}
    }
}

proc ofelia_textwindow_setdirty {name flag} {
    if {[winfo exists $name]} {
        catch {$name.text edit modified $flag}
    }
}

proc ofelia_textwindow_doclose {name} {
    destroy $name
    pdsend [concat $name signoff]
}

proc ofelia_textwindow_append {name contents} {
    if {[winfo exists $name]} {
        $name.text insert end $contents
    }
}

proc ofelia_textwindow_clear {name} {
    if {[winfo exists $name]} {
        $name.text delete 1.0 end
    }
}

proc ofelia_textwindow_send {name} {
    if {[winfo exists $name]} {
        pdsend [concat $name __clear]
        for {set i 1} \
         {[$name.text compare $i.end < end]} \
              {incr i 1} {
            set lin [$name.text get $i.0 $i.end]
            if {$lin != ""} {
                set lin [string map {"," " \\, " ";" " \\; " "$" "\\$"} $lin]
                pdsend [concat $name __addline $lin]
            }
        }
        pdsend [concat $name __notify]
    }
    ofelia_textwindow_setdirty $name 0
}

proc ofelia_textwindow_close {name ask} {
    if {[winfo exists $name]} {
        if {[catch {$name.text edit modified} dirty]} {set dirty 1}
        if {$ask && $dirty} {
            set title [wm title $name]
            if {[string equal -length 1 $title "*"]} {
                set title [string range $title 1 end]
            }
            set answer [tk_messageBox \-type yesnocancel \
             \-icon question \
             \-message [concat Save changes to \"$title\"?]]
            if {$answer == "yes"} {ofelia_textwindow_send $name}
            if {$answer != "cancel"} {pdsend [concat $name __close]}
        } else {pdsend [concat $name __close]}
    }
}
