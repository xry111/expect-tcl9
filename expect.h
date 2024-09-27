/* expect.h - include file for using the expect library, libexpect.a
from C or C++ (i.e., without Tcl)

Written by: Don Libes, libes@cme.nist.gov, NIST, 12/3/90

Design and implementation of this program was paid for by U.S. tax
dollars.  Therefore it is public domain.  However, the author and NIST
would appreciate credit if this program or parts of it are used.
*/

#ifndef _EXPECT_H
#define _EXPECT_H

#include <stdio.h>
#include <stdarg.h>
#include <setjmp.h>
#include <tcl.h>

#undef _ANSI_ARGS_
#undef CONST
#undef CONST84
#undef TCL_VARARGS
#undef TCL_VARARGS_DEF
#undef TCL_VARARGS_START

#define _ANSI_ARGS_(x)	x
#define CONST const
#define CONST84 const
#define TCL_VARARGS(type, name) (type name, ...)
#define TCL_VARARGS_DEF(type, name) (type name, ...)
#define TCL_VARARGS_START(type, name, list) (va_start(list, name), name)

/*
 * regexp definitions - from tcl8.0/tclRegexp.h
 */

/*
 * Definitions etc. for regexp(3) routines.
 *
 * Caveat:  this is V8 regexp(3) [actually, a reimplementation thereof],
 * not the System V one.
 *
 * RCS: @(#) $Id: expect.h,v 5.32 2010/07/01 00:53:49 eee Exp $
 */

#ifndef _REGEXP
#define _REGEXP 1

#ifdef BUILD_tcl
# undef TCL_STORAGE_CLASS
# define TCL_STORAGE_CLASS DLLEXPORT
#endif

/*
 * NSUBEXP must be at least 10, and no greater than 117 or the parser
 * will not work properly.
 */

#define NSUBEXP  20

typedef struct regexp {
	char *startp[NSUBEXP];
	char *endp[NSUBEXP];
	char regstart;		/* Internal use only. */
	char reganch;		/* Internal use only. */
	char *regmust;		/* Internal use only. */
	int regmlen;		/* Internal use only. */
	char program[1];	/* Unwarranted chumminess with compiler. */
} regexp;

EXTERN regexp *TclRegComp _ANSI_ARGS_((char *exp));
EXTERN int TclRegExec _ANSI_ARGS_((regexp *prog, char *string, char *start));
EXTERN void TclRegSub _ANSI_ARGS_((regexp *prog, char *source, char *dest));
EXTERN void exp_TclRegError _ANSI_ARGS_((char *msg));
EXTERN char *TclGetRegError _ANSI_ARGS_((void));

# undef TCL_STORAGE_CLASS
# define TCL_STORAGE_CLASS DLLIMPORT

#endif /* REGEXP */


/*
 * end of regexp definitions
 */


/*
 * finally - expect-specific definitions
 */

#include "expect_comm.h"

enum exp_type {
	exp_end = 0,		/* placeholder - no more cases */
	exp_glob,		/* glob-style */
	exp_exact,		/* exact string */
	exp_regexp,		/* regexp-style, uncompiled */
	exp_compiled,		/* regexp-style, compiled */
	exp_null,		/* matches binary 0 */
	exp_bogus		/* aid in reporting compatibility problems */
};

struct exp_case {		/* case for expect command */
	char *pattern;
	regexp *re;
	enum exp_type type;
	int value;		/* value to be returned upon match */
};

EXTERN char *exp_buffer;		/* buffer of matchable chars */
EXTERN char *exp_buffer_end;		/* one beyond end of matchable chars */
EXTERN char *exp_match;			/* start of matched string */
EXTERN char *exp_match_end;		/* one beyond end of matched string */
EXTERN int exp_match_max;		/* bytes */
EXTERN int exp_timeout;			/* seconds */
EXTERN int exp_full_buffer;		/* if true, return on full buffer */
EXTERN int exp_remove_nulls;		/* if true, remove nulls */

EXTERN int exp_pty_timeout;		/* see Cray hooks in source */
EXTERN int exp_pid;			/* process-id of spawned process */
EXTERN int exp_autoallocpty;		/* if TRUE, we do allocation */
EXTERN int exp_pty[2];			/* master is [0], slave is [1] */
EXTERN char *exp_pty_slave_name;	/* name of pty slave device if we */
					/* do allocation */
EXTERN char *exp_stty_init;		/* initial stty args */
EXTERN int exp_ttycopy;			/* copy tty parms from /dev/tty */
EXTERN int exp_ttyinit;			/* set tty parms to sane state */
EXTERN int exp_console;			/* redirect console */

#ifdef HAVE_SIGLONGJMP
EXTERN sigjmp_buf exp_readenv;		/* for interruptable read() */
#else
EXTERN jmp_buf exp_readenv;		/* for interruptable read() */
#endif /* HAVE_SIGLONGJMP */

EXTERN int exp_reading;			/* whether we can longjmp or not */
#define EXP_ABORT	1		/* abort read */
#define EXP_RESTART	2		/* restart read */

EXTERN int exp_is_debugging;
EXTERN int exp_loguser;

EXTERN void (*exp_close_in_child)();	/* procedure to close files in child */
EXTERN void exp_slave_control _ANSI_ARGS_((int,int));
EXTERN int exp_logfile_all;
EXTERN FILE *exp_debugfile;
EXTERN FILE *exp_logfile;
extern void exp_debuglog _ANSI_ARGS_(TCL_VARARGS(char *,fmt));
extern void exp_errorlog _ANSI_ARGS_(TCL_VARARGS(char *,fmt));

EXTERN int exp_disconnect _ANSI_ARGS_((void));
EXTERN FILE *exp_popen	_ANSI_ARGS_((char *command));
EXTERN void (*exp_child_exec_prelude) _ANSI_ARGS_((void));

#ifndef EXP_DEFINE_FNS
EXTERN int exp_spawnl	_ANSI_ARGS_(TCL_VARARGS(char *,file));
EXTERN int exp_expectl	_ANSI_ARGS_(TCL_VARARGS(int,fd));
EXTERN int exp_fexpectl	_ANSI_ARGS_(TCL_VARARGS(FILE *,fp));
#endif

EXTERN int exp_spawnv	_ANSI_ARGS_((char *file, char *argv[]));
EXTERN int exp_expectv	_ANSI_ARGS_((int fd, struct exp_case *cases));
EXTERN int exp_fexpectv	_ANSI_ARGS_((FILE *fp, struct exp_case *cases));

EXTERN int exp_spawnfd	_ANSI_ARGS_((int fd));

#endif /* _EXPECT_H */
