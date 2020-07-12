/*
 * CDE - Common Desktop Environment
 *
 * Copyright (c) 1993-2012, The Open Group. All rights reserved.
 *
 * These libraries and programs are free software; you can
 * redistribute them and/or modify them under the terms of the GNU
 * Lesser General Public License as published by the Free Software
 * Foundation; either version 2 of the License, or (at your option)
 * any later version.
 *
 * These libraries and programs are distributed in the hope that
 * they will be useful, but WITHOUT ANY WARRANTY; without even the
 * implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 * PURPOSE. See the GNU Lesser General Public License for more
 * details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with these libraries and programs; if not, write
 * to the Free Software Foundation, Inc., 51 Franklin Street, Fifth
 * Floor, Boston, MA 02110-1301 USA
 */
/* $TOG: include.c /main/21 1998/02/06 11:10:06 kaleb $ */
/*

Copyright (c) 1993, 1994, 1998 The Open Group

All Rights Reserved.

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
OPEN GROUP BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN
AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

Except as contained in this notice, the name of The Open Group shall not be
used in advertising or otherwise to promote the sale, use or other dealings
in this Software without prior written authorization from The Open Group.

*/


#include "def.h"

extern struct	inclist	inclist[ MAXFILES ],
			*inclistp;
extern char	*includedirs[ ];
extern char	*notdotdot[ ];
extern boolean show_where_not;
extern boolean warn_multiple;

boolean
isdot(p)
	char	*p;
{
	if(p && *p++ == '.' && *p++ == '\0')
		return(TRUE);
	return(FALSE);
}

boolean
isdotdot(p)
	char	*p;
{
	if(p && *p++ == '.' && *p++ == '.' && *p++ == '\0')
		return(TRUE);
	return(FALSE);
}

boolean
issymbolic(dir, component)
	char	*dir, *component;
{
#ifdef S_IFLNK
	struct stat	st;
	char	buf[ BUFSIZ ], **pp;

	snprintf(buf, BUFSIZ, "%s%s%s", dir, *dir ? "/" : "", component);
	for (pp=notdotdot; *pp; pp++)
		if (strcmp(*pp, buf) == 0)
			return (TRUE);
	if (lstat(buf, &st) == 0
	&& (st.st_mode & S_IFMT) == S_IFLNK) {
		*pp++ = copy(buf);
		if (pp >= &notdotdot[ MAXDIRS ])
			fatalerr("out of .. dirs, increase MAXDIRS\n");
		return(TRUE);
	}
#endif
	return(FALSE);
}

/*
 * Occasionally, pathnames are created that look like .../x/../y
 * Any of the 'x/..' sequences within the name can be eliminated.
 * (but only if 'x' is not a symbolic link!!)
 */
void
remove_dotdot(path)
	char	*path;
{
	char	*end, *from, *to, **cp;
	char		*components[ MAXFILES ],
			newpath[ BUFSIZ ];
	boolean		component_copied;

	/*
	 * slice path up into components.
	 */
	to = newpath;
	if (*path == '/')
		*to++ = '/';
	*to = '\0';
	cp = components;
	for (from=end=path; *end; end++)
		if (*end == '/') {
			while (*end == '/')
				*end++ = '\0';
			if (*from)
				*cp++ = from;
			from = end;
		}
	*cp++ = from;
	*cp = NULL;

	/*
	 * Recursively remove all 'x/..' component pairs.
	 */
	cp = components;
	while(*cp) {
		if (!isdot(*cp) && !isdotdot(*cp) && isdotdot(*(cp+1))
		    && !issymbolic(newpath, *cp))
		{
		    char **fp = cp + 2;
		    char **tp = cp;

		    do 
			*tp++ = *fp; /* move all the pointers down */
		    while (*fp++);
		    if (cp != components)
			cp--;	/* go back and check for nested ".." */
		} else {
		    cp++;
		}
	}
	/*
	 * Concatenate the remaining path elements.
	 */
	cp = components;
	component_copied = FALSE;
	while(*cp) {
		if (component_copied)
			*to++ = '/';
		component_copied = TRUE;
		for (from = *cp; *from; )
			*to++ = *from++;
		*to = '\0';
		cp++;
	}
	*to++ = '\0';

	/*
	 * copy the reconstituted path back to our pointer.
	 */
	strncpy(path, newpath, BUFSIZ);
}

/*
 * Add an include file to the list of those included by 'file'.
 */
struct inclist *newinclude(newfile, incstring)
	char	*newfile, *incstring;
{
	struct inclist	*ip;

	/*
	 * First, put this file on the global list of include files.
	 */
	ip = inclistp++;
	if (inclistp == inclist + MAXFILES - 1)
		fatalerr("out of space: increase MAXFILES\n");
	ip->i_file = copy(newfile);

	if (incstring == NULL)
		ip->i_incstring = ip->i_file;
	else
		ip->i_incstring = copy(incstring);

	return(ip);
}

void
included_by(ip, newfile)
	struct inclist	*ip, *newfile;
{
	int i;

	if (ip == NULL)
		return;
	/*
	 * Put this include file (newfile) on the list of files included
	 * by 'file'.  If 'file' is NULL, then it is not an include
	 * file itself (i.e. was probably mentioned on the command line).
	 * If it is already on the list, don't stick it on again.
	 */
	if (ip->i_list == NULL) {
		ip->i_list = (struct inclist **)
			malloc(sizeof(struct inclist *) * ++ip->i_listlen);
		ip->i_merged = (boolean *)
		    malloc(sizeof(boolean) * ip->i_listlen);
	} else {
		for (i=0; i<ip->i_listlen; i++)
			if (ip->i_list[ i ] == newfile) {
			    i = strlen(newfile->i_file);
			    if (!(ip->i_flags & INCLUDED_SYM) &&
				!(i > 2 &&
				  newfile->i_file[i-1] == 'c' &&
				  newfile->i_file[i-2] == '.'))
			    {
				/* only bitch if ip has */
				/* no #include SYMBOL lines  */
				/* and is not a .c file */
				if (warn_multiple)
				{
					warning("%s includes %s more than once!\n",
						ip->i_file, newfile->i_file);
					warning1("Already have\n");
					for (i=0; i<ip->i_listlen; i++)
						warning1("\t%s\n", ip->i_list[i]->i_file);
				}
			    }
			    return;
			}
		ip->i_list = (struct inclist **) realloc(ip->i_list,
			sizeof(struct inclist *) * ++ip->i_listlen);
		ip->i_merged = (boolean *)
		    realloc(ip->i_merged, sizeof(boolean) * ip->i_listlen);
	}
	ip->i_list[ ip->i_listlen-1 ] = newfile;
	ip->i_merged[ ip->i_listlen-1 ] = FALSE;
}

void
inc_clean ()
{
	struct inclist *ip;

	for (ip = inclist; ip < inclistp; ip++) {
		ip->i_flags &= ~MARKED;
	}
}

struct inclist *inc_path(file, include, dot)
	char	*file,
			*include;
	boolean	dot;
{
	static char	path[ BUFSIZ ];
	char		**pp, *p;
	struct inclist	*ip;
	struct stat	st;
	boolean	found = FALSE;

	/*
	 * Check all previously found include files for a path that
	 * has already been expanded.
	 */
	for (ip = inclist; ip->i_file; ip++)
	    if ((strcmp(ip->i_incstring, include) == 0) &&
		!(ip->i_flags & INCLUDED_SYM))
	    {
		found = TRUE;
		break;
	    }

	/*
	 * If the path was surrounded by "" or is an absolute path,
	 * then check the exact path provided.
	 */
	if (!found && (dot || *include == '/')) {
		if (stat(include, &st) == 0) {
			ip = newinclude(include, include);
			found = TRUE;
		}
		else if (show_where_not)
			warning1("\tnot in %s\n", include);
	}

	/*
	 * If the path was surrounded by "" see if this include file is in the
	 * directory of the file being parsed.
	 */
	if (!found && dot) {
		for (p=file+strlen(file); p>file; p--)
			if (*p == '/')
				break;
		if (p == file)
			strncpy(path, include, BUFSIZ);
		else {
			strncpy(path, file, (p-file) + 1);
			path[ (p-file) + 1 ] = '\0';
			strncpy(path + (p-file) + 1, include,
						BUFSIZ - (p-file) - 1);
		}
		remove_dotdot(path);
		if (stat(path, &st) == 0) {
			ip = newinclude(path, include);
			found = TRUE;
		}
		else if (show_where_not)
			warning1("\tnot in %s\n", path);
	}

	/*
	 * Check the include directories specified. (standard include dir
	 * should be at the end.)
	 */
	if (!found)
		for (pp = includedirs; *pp; pp++) {
			snprintf(path, BUFSIZ, "%s/%s", *pp, include);
			remove_dotdot(path);
			if (stat(path, &st) == 0) {
				ip = newinclude(path, include);
				found = TRUE;
				break;
			}
			else if (show_where_not)
				warning1("\tnot in %s\n", path);
		}

	if (!found)
		ip = NULL;
	return(ip);
}
