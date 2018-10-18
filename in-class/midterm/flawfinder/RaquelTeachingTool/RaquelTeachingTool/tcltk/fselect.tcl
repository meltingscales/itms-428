#==============================================================
#  fselect.tcl
#
#  University of Northumbria
#  Sebastian Kuschel
#  May 1997
#  reorganisation of the header by Mesel Tesfaiesus, Nov. 2001
#===============================================================

# Dialog chapter

# fileselect returns the selected pathname, or {}
proc fileselect {why {default {}} } {
	global fileselect
    
	catch {destroy .fileselect}
	set t [toplevel .fileselect -bd 4]
        wm title .fileselect "RAQUEL - Fileselection"
	message $t.msg -aspect 1000 -text $why
	pack $t.msg -side top -fill x
    
	# Create a read-only entry for the durrent directory
	set fileselect(dirEnt) [entry $t.dir -width 15 \
		-relief flat -state disabled]
	pack $t.dir -side top -fill x
    
	# Create an entry for the pathname
	# The value is kept in fileselect(path)
	frame $t.top
	label $t.top.l -text "File:" -padx 0
	set e [entry $t.top.path -relief sunken \
		-textvariable fileselect(path)]
	pack $t.top -side top -fill x
	pack $t.top.l -side left
	pack $t.top.path -side right -fill x -expand true
	set fileselect(pathEnt) $e
    
	# Set up bindings to invoke OK and Cancel
	bind $e <Return> fileselectOK
	bind $e <Control-c> fileselectCancel
	bind $e <space> fileselectComplete
	focus $e
    
	# Create a listbox to hold the directory contents
	listbox $t.list -yscrollcommand [list $t.scroll set]
	scrollbar $t.scroll -command [list $t.list yview]

	# A single click copies the name into the entry
	# A double-click selects the name
	bind $t.list <Button-1> {fileselectClick %y}
	bind $t.list <Double-Button-1> {
		fileselectClick %y ; fileselectOK
	}
    
	# Warp focus to listbox so the user can use arrow keys
	bind $e <Tab> "focus $t.list ; $t.list select set 0"
	bind $t.list <Return> fileselectTake
	bind $t.list <space> {fileselectTake ; break}
	bind $t.list <Tab> "focus $e"
    
	# Create the OK and Cancel buttons
	# The OK button has a rim to indicate it is the default
	frame $t.buttons -bd 10
	frame $t.buttons.ok -bd 2 -relief sunken
	button $t.buttons.ok.b -text OK \
		-command fileselectOK
	button $t.buttons.cancel -text Cancel \
		-command fileselectCancel

	# Pack the list, scrollbar, and button box
	# in a horizontal stack below the upper widgets
	pack $t.list -side left -fill both -expand true
	pack $t.scroll -side left -fill y
	pack $t.buttons -side left -fill both
	pack $t.buttons.ok $t.buttons.cancel \
		-side top -padx 10 -pady 5
	pack $t.buttons.ok.b -padx 4 -pady 4
    
	# Initialize variables and list the directory
	if {[string length $default] == 0} {
		set fileselect(path) {}
		set dir [pwd]
	} else {
		set fileselect(path) [file tail $default]
		set dir [file dirname $default]
	}
	set fileselect(dir) {}
	set fileselect(done) 0

	# Wait for the listbox to be visible so
	# we can provide feedback during the listing 
	tkwait visibility .fileselect.list
	fileselectList $dir

	tkwait variable fileselect(done)
	destroy .fileselect
	return $fileselect(path)
}
proc fileselectList { dir {files {}} } {
	global fileselect

	# Update the directory, being careful
	# to view the tail end
	set e $fileselect(dirEnt)
	$e config -state normal
	$e delete 0 end
	$e insert 0 $dir
	$e config -state disabled
	$e xview moveto 1

	set fileselect(dir) $dir
	.fileselect.list delete 0 end
	.fileselect.list insert 0 Listing...
	update idletasks
	.fileselect.list delete 0
	if {[string length $files] == 0} {
		# List the directory and add an
		# entry for the parent directory
		set files [glob -nocomplain $fileselect(dir)/*]
	        #set files [glob -nocomplain $fileselect(dir)/*.raq]
		.fileselect.list insert end ../
	}
	set dirs {}
	set others {}
	foreach f [lsort $files] {
		if [file isdirectory $f] {
			lappend dirs [file tail $f]/
		} else {
			lappend others [file tail $f]
		}
	}
	foreach f [concat $dirs $others] {
	    .fileselect.list insert end $f
	}
}

proc fileselectOK {} {
	global fileselect
    
	# Handle the parent directory specially
	if {[regexp {\.\./?} $fileselect(path)]} {
		set fileselect(path) {}
		fileselectList [file dirname $fileselect(dir)]
		return
	}
    
	set path $fileselect(dir)/$fileselect(path)
    
	if [file isdirectory $path] {
		set fileselect(path) {}
		fileselectList $path
		return
	}
	set fileselect(path) $path
	set fileselect(done) 1
	set fileselect(ok) 1
	return
}

proc fileselectCancel {} {
	global fileselect
	set fileselect(done) 1
	set fileselect(path) {}
	set fileselect(ok) 0
}

proc fileselectClick { y } {
	# Take the item the user clicked on
	global fileselect
	set l .fileselect.list
	set fileselect(path) [$l get [$l nearest $y]]
	focus $fileselect(pathEnt)
}
proc fileselectTake {} {
	# Take the currently selected list item and
	# change focus back to the entry
	global fileselect
	set l .fileselect.list
	set fileselect(path) [$l get [$l curselection]]
	focus $fileselect(pathEnt)
}

proc fileselectComplete {} {
	global fileselect

	# Do file name completion
	# Nuke the space that triggered this call
	set fileselect(path) [string trim $fileselect(path) \t\ ]

	# Figure out what directory we are looking at
	# dir is the directory
	# tail is the partial name
	if {[string match /* $fileselect(path)]} {
		set dir [file dirname $fileselect(path)]
		set tail [file tail $fileselect(path)]
	} elseif [string match ~* $fileselect(path)] {
		if [catch {file dirname $fileselect(path)} dir] {
			return	;# Bad user
		}
		set tail [file tail $fileselect(path)]
	} else {
		set path $fileselect(dir)/$fileselect(path)
		set dir [file dirname $path]
		set tail [file tail $path]
	}
	# See what files are there
	set files [glob -nocomplain $dir/$tail*]
	if {[llength [split $files]] == 1} {
		# Matched a single file
		set fileselect(dir) $dir
		set fileselect(path) [file tail $files]
	} else {
		if {[llength [split $files]] > 1} {
			# Find the longest common prefix
			set l [expr [string length $tail]-1]
			set miss 0
			# Remember that files has absolute paths
			set file1 [file tail [lindex $files 0]]
			while {!$miss} {
				incr l
				if {$l == [string length $file1]} {
					# file1 is a prefix of all others
					break
				}
				set new [string range $file1 0 $l]
				foreach f $files {
					if ![string match $new* [file tail $f]] {
						set miss 1
						incr l -1
						break
					}
				}
			}
			set fileselect(path) [string range $file1 0 $l]
		}
		fileselectList $dir $files
	}
}
