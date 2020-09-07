/* Copyright (C) 2015-2021 Free Software Foundation, Inc.
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
   <https://www.gnu.org/licenses/>.  */

#include <sys/socket.h>
/* The kernel header with SO_* constants is used as default for _GNU_SOURCE,
   however the new constants that describe 64-bit time support were added
   only on v5.1.  */
#if !defined(SO_RCVTIMEO_NEW) || !defined(SO_RCVTIMEO_OLD)
# include <bits/socket-constants.h>
#endif
#include <time.h>
#include <sysdep.h>
#include <socketcall.h>

static int
getsockopt_syscall (int fd, int level, int optname, void *optval,
		    socklen_t *len)
{
#ifdef __ASSUME_GETSOCKOPT_SYSCALL
  return INLINE_SYSCALL (getsockopt, 5, fd, level, optname, optval, len);
#else
  return SOCKETCALL (getsockopt, fd, level, optname, optval, len);
#endif
}

#ifndef __ASSUME_TIME64_SYSCALLS
static int
getsockopt32 (int fd, int level, int optname, void *optval,
	      socklen_t *len)
{
  int r = -1;

  if (level != SOL_SOCKET)
    return r;

  switch (optname)
    {
    case SO_RCVTIMEO_NEW:
    case SO_SNDTIMEO_NEW:
      {
        if (*len < sizeof (struct __timeval64))
	  {
	    __set_errno (EINVAL);
	    break;
	  }

	if (optname == SO_RCVTIMEO_NEW)
	  optname = SO_RCVTIMEO_OLD;
	if (optname == SO_SNDTIMEO_NEW)
	  optname = SO_SNDTIMEO_OLD;

	struct __timeval32 tv32;
	r = getsockopt_syscall (fd, level, optname, &tv32,
				(socklen_t[]) { sizeof tv32 });
	if (r < 0)
	  break;
	struct __timeval64 *tv64 = (struct __timeval64 *) optval;
	*tv64 = valid_timeval32_to_timeval64 (tv32);
	*len = sizeof (*tv64);
      }
    }

  return r;
}
#endif

int
__getsockopt (int fd, int level, int optname, void *optval, socklen_t *len)
{
  int r = getsockopt_syscall (fd, level, optname, optval, len);

#ifndef __ASSUME_TIME64_SYSCALLS
  if (r == -1 && errno == ENOPROTOOPT)
    r = getsockopt32 (fd, level, optname, optval, len);
#endif

 return r;
}
weak_alias (__getsockopt, getsockopt)
