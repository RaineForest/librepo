/* librepo - A library providing (libcURL like) API to downloading repository
 * Copyright (C) 2012  Tomas Mlcoch
 *
 * Licensed under the GNU Lesser General Public License Version 2.1
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

#ifndef __LR_RESULT_INTERNAL_H__
#define __LR_RESULT_INTERNAL_H__

#include <glib.h>

#include "repomd.h"
#include "yum.h"
#include "deb.h"

G_BEGIN_DECLS

struct _LrResult {
    char            *destdir; /*!<
        Directory with repo */

    union {
        struct {
            LrYumRepoMd    *yum_repomd; /*!<
                Pointer to struct representingrepomd.xml */

            LrYumRepo      *yum_repo; /*!<
                Pointer to struct with info about yum repo */
        } yum;
        struct {
            LrDebRepo      *deb_repo;
        } deb;
    } repo;

    LrRepotype repotype;
};

G_END_DECLS

#endif
