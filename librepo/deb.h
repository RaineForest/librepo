#ifndef __LR_DEBREPO_H__
#define __LR_DEBREPO_H__

#include <glib.h>

G_BEGIN_DECLS

/** \defgroup   deb     Deb repo manipulation
 *  \addtogroup deb
 *  @{
 */

typedef struct {
    char *baseurl;      /*!< Repo location */
} LrDebRepo;

/** Allocate a new deb repo object.
 * @return              New deb repo object.
 */
LrDebRepo *
lr_deb_repo_init(void);

/** Free a deb repo object.
 * @param repo          Deb repo object.
 */
void
lr_deb_repo_free(LrDebRepo *repo);

/** @} */

G_END_DECLS

#endif
