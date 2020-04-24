'\"	TET Overall Specification Document
'\"
'\"	Note that the Constant Width font identifiers may be different
'\"	in your version of troff.  In this distribution they are specified
'\" 	(in this file) as CW and CB.  Change these as appropriate.
.fp 7 CW	\" Courier - Change to C if your troff needs that
.fp 8 CB	\" Courier Bold
'\"
'\"	Process with pic ch*.mm | tbl | troff -mm
'\"
'\"	SCCS: @(#) TET SPECIFICATION		Rel 4.5 (06/23/92)
'\"
'\"
'\"     Make the appropriate replacements in this section!
'\"
'\"     Set the ND date to the current date.
'\"     tT is the formal document title
'\"     tP is the name of the Project (if appropriate)
'\"     tD is the short document title
'\"     tE is the work group name (may be the same as the project name)
.ds tT Test Environment Toolkit
.ds tP Architectural, Functional, and Interface Specification
'\"             Document name (i.e., without project name)
.ds tD TET Specification
.ds tE Project Phoenix
'\"
'\"     Define headers and footers macro
'\"
.ds fA Revision: 4.5
'\"
'\"     fB null to remove page numbers on cover page
.ds fB
.ds fC Date: 92/02/18
.ds fE
.rm )k
.PH "''''"
.PF "''\s10\*(fE\s0''"
.tr ~
.SA 1
.S 12
.nr Cl 7
.nr Ej 1
.nr Hs 4
.nr Hu 1
.nr Hb 3
.ds HP +2 +2 +1 +0 +0 +0 +0
.ds HF 3 3 3 3 1 1 1
.if n .ds HF 1 1 1 1 1 1 1 1
'\"
'\"     First page, print title and authors
'\"
.S +4
.DS C






\fB\*(tT

\s-2\*(tP\s+2

.DE
.S
.sp 5i
\*(tE
.br
\*(fA (\*(fC)
.SK
.SP 6
Copyright 1992 Open Software Foundation (OSF)
.br
Copyright 1992 Unix International (UI)
.br
Copyright 1992 X/Open Company Limited (X/Open)
.br
.SP
.R
Permission to use, copy, modify, and distribute this software and its
documentation for any purpose and without fee is hereby granted, provided
that the above copyright notice appear in all copies and that both that
copyright notice and this permission notice appear in supporting
documentation, and that the name of OSF, UI or X/Open not be used in 
advertising or publicity pertaining to distribution of the software 
without specific, written prior permission.  OSF, UI and X/Open make 
no representations about the suitability of this software for any purpose.  
It is provided "as is" without express or implied warranty.
.SP
OSF, UI and X/Open DISCLAIM ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, 
INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO 
EVENT SHALL OSF, UI or X/Open BE LIABLE FOR ANY SPECIAL, INDIRECT OR 
CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF 
USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR 
OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR 
PERFORMANCE OF THIS SOFTWARE.
.P
.B Trademarks:
.P
UNIX\(rg is a registered trademark of UNIX System Laboratories, Inc.
in the United States and other countries.
.if n .fp 2 R
.if n .fp 3 R
.\"	These macros stolen shamelessly from IEEE TCOS (thanks Hal!)
.\"
.\"     a command name
.de Cm
.ie \\n(.$-2 \&\\$1\&\f7\s-1\\$2\s+1\fP\&\\$3
.el \&\f7\s-1\\$1\s+1\fP\&\\$2
.fF
..
.\"     a constant width expression
.de Cw
.ie \\n(.$-2 \&\\$1\&\f7\s-1\\$2\s+1\fP\&\\$3
.el \&\f7\s-1\\$1\s+1\fP\&\\$2
.fF
..
.\"     a constant width bold expression
.de Cb
.ie \\n(.$-2 \&\\$1\&\f8\s-1\\$2\s+1\fP\&\\$3
.el \&\f8\s-1\\$1\s+1\fP\&\\$2
.fF
..
.\"     an environment variable
.de Ev
.ie \\n(.$-2 \&\\$1\&\f3\\$2\fP\\$3
.el \&\f3\\$1\fP\\$2
.fF
..
.\"     a file name
.de Fi
.ie \\n(.$-2 \&\\$1\&\f7\s-1\\$2\s+1\fP\&\\$3
.el \&\f7\s-1\\$1\s+1\fP\&\\$2
.fF
..
.\"     a function name
.de Fn
.ie \\n(.$-2 \&\\$1\&\f2\\$2\^\fP(\^)\\$3
.el \&\f2\\$1\^\fP(\^)\\$2
..
.\"     a metavariable
.de Mv
.ie \\n(.$-2 \&\\$1\&\f2<\\$2>\fP\\$3
.el \&\f2<\\$1>\fP\\$2
.fF
..
.ds To \f2To be completed\ ...\fP
.\"
.\" nonprinting blank character for printf output
.ds pS \s-2\(*D\s+2
.ds Xp \&\f2\s-1XPG\s+1\fP
.\"     an option used as a list item
.de Lo
.br
.ne 4v
.LI "\&\f7\\-\\$1\fP"
.fF
..
.\"     a variable used as a list item
.de Lv
.br
.ne 4v
.LI "\&\f3\\$1\fP"
.fF
..
.\"
.\"	a printf-like format
.de Pf
\&\f7\s-1"\\$1"\s+1\fP, \c
.fF
..
.\"     a command option
.de Op
.ie \\n(.$-2 \&\s-1\\$1\s+1\&\f7\-\\$2\fP\\$3
.el \&\f7\-\s-1\\$1\s+1\fP\\$2
.fF
..
.\" Start of Synopsis macro
.de Sy
.nr sY 1        \" set "in-Synopsis" flag
.na
.ds Sz \&\f7\s-1\\$1\s+1\fP\0
.ll -6n
.in \w'\\*(Sz 'u+6n
.ti -\w'\\*(Sz 'u
\\*(Sz
..
.\" End of Synopsis macro
.de Se
.nr sY 0        \" clear "in-Synopsis" flag
.R \" I'm not sure why this is required, but what the heck!
.in
.ll +6n
.ad
..
.\" Option without dash or brackets
.de Or
.ie \\n(.$-2 \&\\$1\&\f7\\$2\fP\\$3
.el \&\f7\\$1\fP\\$2
.fF
..
.\" Argument (unbracketed)
.de Ar
.ie \\n(sY \{\
.   ie \\n(.$-2 \&\\$1\&\f2\\$2\fP\\$3\|
.   el \&\f2\\$1\fP\\$2\|\}
.el \{\
.   ie \\n(.$-2 \&\\$1\&\f2\\$2\fP\\$3
.   el \&\f2\\$1\fP\\$2\}
.fF
..
.\" Argument (bracketed)
.de Ab
\&\f8[\fP\^\f2\\$1\fP\^\f8]\fP\\$2
.fF
..
.\" option and arg (with no whitespace)
.de O1
.ie \\n(.$-1 \&\f8[\fP\f7\^\-\s-1\\$1\s+1\fP\^\f2\\$2\fP\^\f8]\fP
.el \&\f8[\fP\f7\^\-\s-1\\$1\s+1\fP\^\f2\^\fP\f8]\fP
.fF
..
.\" option and arg (with whitespace)
.de O2
\&\f8[\fP\f7\^\-\s-1\\$1\s+1\0\fP\f2\\$2\fP\^\f8]\fP
.fF
..
.\" Option and arg (with no whitespace)
.de A1
.ie \\n(.$-1 \&\f7\-\\$1\fP\^\f2\\$2\^\fP
.el \&\f7\-\\$1\fP\^\f2\^\fP\\$2
.\" \\$2 added on preceding--hlj
.fF
..
.\" Option and arg (with whitespace)
.de A2
\&\f7\-\\$1\fP\0\f2\\$2\^\fP
.fF
..
.\" List-Item option (with no whitespace)
.de L1
.br
.ne 4v
.ie \\n(.$-1 .LI \&\f7\-\s-1\\$1\s+1\fP\^\f2\\$2\fP
.el .LI \&\f7\-\s-1\\$1\s+1\fP\^\f2\^\fP
.fF
..
.\" List-Item option (with whitespace)
.de L2
.br
.ne 4v
.ie \\n(.$-1 .LI \&\f7\-\s-1\\$1\s+1\fP\0\f2\\$2\fP
.el .LI \&\f7\-\s-1\\$1\s+1\fP\0
.fF
..
.\" List-Item non-bracketed option w/out a dash (with no whitespace)
.de N1
.br
.ne 4v
.ie \\n(.$-1 .LI \&\f7\\$1\fP\^\f2\\$2\fP
.el .LI \&\f7\\$1\fP\^\f2\\$2\fP
.fF
..
.\" List-Item non-bracketed option w/out a dash (with whitespace)
.de N2
.br
.ne 4v
.LI \&\f7\\$1\fP\0\f2\\$2\fP
.fF
..
.\" List-Item argument without bracket
.de La
.br
.ne 4v
.LI "\&\f2\\$1\fP"
.fF
..
.\" List-Item bracketed built-in (with whitespace)
.de Lb
.br
.ne 4v
.ie \\n(.$-1 .LI \&\f7\\$1\fP\0\f8[\fP\^\f2\\$2\fP\f8]\f1
.el .LI \&\f7\\$1\f1
.fF
..
.\" List-Item bracketed address (with whitespace)
.\" for sed utility description
\." NO - this clashes with mm's Lx (List of Exhibits) number register!
\." Just as well it isn't used in this document!
.\" .de Lx
.\" .br
.\" .ne 4v
.\" .ie \\n(.$-1 .LI \&\f8[\fP\^\f2\\$1\fP\f8]\fP\f7\\$2\fP\f2\\$3\fP
.\" .el .LI \&\f8[\fP\^\f2\\$1\fP\f8]\fP
.\" .fF
.\" ..
.\" List-Item Parameter
.de Lp
.br
.ne 4v
.LI \&\f7${\^\f2\\$1\^\f7\\$2\f2\\$3\^\f7}\f1
.fF
..
.\"
.\"	a list item for commands or other constant width items
.de Lc
.br
.if \\n% .ne 4v
.LI "\&\f7\s-1\\$1\s+1\fP"
.fF
..
.\" Open bracket
.de Bo
\&\f8[\fP\^\\$1
..
.\" Close bracket
.de Bc
\&\f8]\fP\\$1
..
.\" Open curly bracket (or brace, if you prefer)
.de Co
\&\f8{\fP\^\\$1
..
.\" Close curly bracket (or brace, if you prefer)
.de Cc
\&\f8}\fP\\$1
..
.\" Pipe symbol
.de Pi
\&\f8|\fP\\$1
..
.rs
.nr % 1
.nr P 1
.OH "'''\s10\\\\*(tE\s0'"
.EH "'\s10\\\\*(tD\s0'''"
.SK
.VM 0 2
.PF "''\s10\\\\*(fE\s0''"
.OF "'\s10\\\\*(fA'\\\\*(fB'\\\\*(fC\s0'"
.EF "'\s10\\\\*(fA'\\\\*(fB'\\\\*(fC\s0'"
'\" -----------------------------------------------------------------------
'\".
'\"     Reset page numbers
'\"
.nr P 1
.nr % 1
'\"
'\"     Define headers and footers
'\"
.FH
'\"     Turn on the page numbering in the footers
.ds fB Page %
'\"
'\"     MACROEND
'\"
