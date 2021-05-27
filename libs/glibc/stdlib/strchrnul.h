/*
   strchrnul — Find and return a character in a string, or end of string

   Versions strchrnul() first appeared in glibc in version 2.1.1.

   Conforming to strchr() and strrchr() are in SVr4, 4.3BSD, C89, C99, 
   POSIX.1-2001, POSIX.1-2008. strchrnul() is a GNU extension.

   Copyright (C) 1991-2018 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.
  
   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.
  
   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see
   <http://www.gnu.org/licenses/>.
*/

#ifndef STRCHRNUL_H__
#define STRCHRNUL_H__

#ifdef __cplusplus
extern "C" {
#endif //__cplusplus

// TODO: document
char* __strchrnul(const char* s, int c);

#ifdef __cplusplus
}
#endif //__cplusplus

#endif // STRCHRNUL_H__
