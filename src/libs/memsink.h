/*****************************************************************************
#                                                                            #
#    uStreamer - Lightweight and fast MJPG-HTTP streamer.                    #
#                                                                            #
#    Copyright (C) 2018-2021  Maxim Devaev <mdevaev@gmail.com>               #
#                                                                            #
#    This program is free software: you can redistribute it and/or modify    #
#    it under the terms of the GNU General Public License as published by    #
#    the Free Software Foundation, either version 3 of the License, or       #
#    (at your option) any later version.                                     #
#                                                                            #
#    This program is distributed in the hope that it will be useful,         #
#    but WITHOUT ANY WARRANTY; without even the implied warranty of          #
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the           #
#    GNU General Public License for more details.                            #
#                                                                            #
#    You should have received a copy of the GNU General Public License       #
#    along with this program.  If not, see <https://www.gnu.org/licenses/>.  #
#                                                                            #
*****************************************************************************/


#pragma once

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <assert.h>

#include <sys/types.h>
#include <sys/file.h>
#include <sys/stat.h>
#include <sys/mman.h>

#include "tools.h"
#include "logging.h"
#include "frame.h"


#ifndef CFG_MEMSINK_MAX_DATA
#	define CFG_MEMSINK_MAX_DATA 33554432
#endif
#define MEMSINK_MAX_DATA ((size_t)(CFG_MEMSINK_MAX_DATA))


typedef struct {
	uint64_t	id;
	size_t		used;
	unsigned	width;
	unsigned	height;
	unsigned	format;
	unsigned	stride;
	bool		online;
	long double	grab_ts;
	long double	encode_begin_ts;
	long double	encode_end_ts;
	long double	last_client_ts;
	uint8_t		data[MEMSINK_MAX_DATA];
} memsink_shared_s;

typedef struct {
	const char	*name;
	const char	*obj;
	bool		server;
	bool		rm;
	unsigned	client_ttl; // Only for server
	unsigned	timeout;

	int					fd;
	memsink_shared_s	*mem;
	uint64_t			last_id;
	bool				has_clients; // Only for server
} memsink_s;


memsink_s *memsink_init(
	const char *name, const char *obj, bool server,
	mode_t mode, bool rm, unsigned client_ttl, unsigned timeout);

void memsink_destroy(memsink_s *sink);

int memsink_server_check_clients(memsink_s *sink);
int memsink_server_put(memsink_s *sink, const frame_s *frame);
int memsink_client_get(memsink_s *sink, frame_s *frame);
