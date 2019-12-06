
#ifndef __LR_DEB_INTERNAL_H__
#define __LR_DEB_INTERNAL_H__

#include <glib.h>

#include "rcodes.h"
#include "result.h"
#include "handle.h"

G_BEGIN_DECLS

gboolean
lr_deb_perform(LrHandle *handle, LrResult *result, GError **err);

G_END_DECLS

#endif
