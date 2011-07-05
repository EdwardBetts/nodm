/*
 * test-xsession - test that we are able to run a X session
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
#include "common.h"
#include "dm.h"
#include <stdio.h>
#include <stdlib.h>

int test_session(struct nodm_xsession_child* s)
{
    return E_SUCCESS;
}

int main(int argc, char* argv[])
{
    struct log_config cfg = {
        .program_name = "test-xsession",
        .log_to_syslog = false,
        .log_to_stderr = true,
        .info_to_stderr = true,
    };
    log_start(&cfg);

    //const char* xcmdline = "/usr/bin/Xnest :1";
    setenv("NODM_SESSION", "/bin/true", 1);
    setenv("NODM_USER", getenv_with_default("USER", "root"), 1);

    int res;

    struct nodm_display_manager dm;
    nodm_display_manager_init(&dm);

    // configure display manager for testing
    res = nodm_display_manager_parse_xcmdline(&dm, "/usr/bin/Xnest :1 -geometry 1x1+0+0");
    if (res != E_SUCCESS)
    {
        fprintf(stderr, "nodm_display_manager_parse_xcmdline return code: %d\n", res);
        goto cleanup;
    }
    dm.session.conf_use_pam = false;
    dm.session.conf_cleanup_xse = false;
    dm.session.conf_run_as[0] = 0;
    dm.session.child_body = test_session;

    res = nodm_display_manager_start(&dm);
    if (res != E_SUCCESS)
    {
        fprintf(stderr, "nodm_display_manager_start return code: %d\n", res);
        goto cleanup;
    }

    res = nodm_display_manager_wait(&dm);
    if (res != E_SUCCESS)
    {
        fprintf(stderr, "nodm_display_manager_wait return code: %d\n", res);
        goto cleanup;
    }

    res = nodm_display_manager_stop(&dm);
    if (res != E_SUCCESS)
    {
        fprintf(stderr, "nodm_display_manager_stop return code: %d\n", res);
        goto cleanup;
    }

    nodm_display_manager_cleanup(&dm);

cleanup:
    log_end();
    return res;
}
