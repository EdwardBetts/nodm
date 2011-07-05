/*
 * test-xstart - test that we are able to start X
 *
 * Copyright 2011  Enrico Zini <enrico@enricozini.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include "log.h"
#include "xserver.h"
#include "common.h"
#include <stdio.h>

int main(int argc, char* argv[])
{
    struct log_config cfg = {
        .program_name = "test-xstart",
        .log_to_syslog = false,
        .log_to_stderr = true,
        .info_to_stderr = true,
    };
    log_start(&cfg);

    struct nodm_xserver srv;
    nodm_xserver_init(&srv);

    const char* server_argv[] = { "/usr/bin/Xnest", ":1", "-geometry", "1x1+0+0", NULL };
    srv.argv = server_argv;
    srv.name = ":1";

    int res = nodm_xserver_start(&srv);
    if (res != E_SUCCESS)
    {
        fprintf(stderr, "nodm_xserver_start return code: %d\n", res);
        return res;
    }

    res = nodm_xserver_stop(&srv);
    if (res != E_SUCCESS)
    {
        fprintf(stderr, "nodm_xserver_stop return code: %d\n", res);
        return res;
    }

    log_end();
    return E_SUCCESS;
}
