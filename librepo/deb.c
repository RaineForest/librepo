
#define _POSIX_SOURCE
#define _DEFAULT_SOURCE

#include <assert.h>
#include <string.h>

#include "deb.h"
#include "deb_internal.h"
#include "util.h"

LrDebRepo *
lr_deb_repo_init(void)
{
    return lr_malloc0(sizeof(LrDebRepo));
}

void
lr_deb_repo_free(LrDebRepo *repo)
{
    if (!repo) {
        return;
    }

    lr_free(repo->baseurl);
    lr_free(repo);
}

void
lr_deb_download_release(LrHandle *handle, LrDebRepo *repo, GError **err)
{
    assert(repo);

    g_debug("%s: Downloading InRelease", __func__);

    char* path = lr_pathconcat(handle->destdir, "InRelease", NULL);
    int fd = open(path, O_CREAT|O_TRUNC|O_RDWR, 0666);
    if (fd == -1) {
        g_set_error(err, LR_DEB_ERROR, LRE_IO,
                    "Cannot open %s: %s", *path, g_strerror(errno));
        lr_free(path);
        return;
    }

    LrDownloadTarget* targetInRelease = lr_downloadtarget_new(handle,
                                                              "InRelease",
                                                              repo->baseurl,
                                                              fd,
                                                              NULL,
                                                              NULL,
                                                              0,
                                                              FALSE,
                                                              NULL,
                                                              NULL,
                                                              NULL,
                                                              NULL,
                                                              NULL,
                                                              0,
                                                              0,
                                                              NULL,
                                                              TRUE,
                                                              FALSE);

    GError* tmp_err = NULL;
    int ret = lr_download_target(targetInRelease, &tmp_err);
    if (tmp_err) {
        // fallback on the deprecated "Release" file
        LrDownloadTarget* targetRelease = lr_downloadtarget_new(handle,
                                                                "Release",
                                                                repo->baseurl,
                                                                fd,
                                                                NULL,
                                                                NULL,
                                                                0,
                                                                FALSE,
                                                                NULL,
                                                                NULL,
                                                                NULL,
                                                                NULL,
                                                                NULL,
                                                                0,
                                                                0,
                                                                NULL,
                                                                TRUE,
                                                                FALSE);
        g_err_free(tmp_err);
        tmp_err = NULL;
        ret = lr_download_target(targetInRelease, &tmp_err);
        if (tmp_err) {
            // Actual error time
        }
    }

    lr_downloadtarget_free(targetInRelease);

    close(fd);
    lr_free(path);
}

int
lr_deb_download_remote(LrHandle *handle, LrResult *result, GError **err)
{

}

gboolean
lr_deb_perform(LrHandle *handle, LrResult *result, GError **err)
{
    int ret = TRUE;
    assert(handle);
    assert(!err || *err == NULL);

    if (!result) {
        g_set_error(err, LR_DEB_ERROR, LRE_BADFUNCARG,
                    "Missing result parameter");
        return FALSE;
    }

    if (!handle->urls) {
        g_set_error(err, LR_DEB_ERROR, LRE_NOURL,
                "No LRO_URLS specified");
        return FALSE;
    }

    if (handle->local && (!handle->urls || !handle->urls[0])) {
        g_set_error(err, LR_DEB_ERROR, LRE_NOURL,
                    "Localrepo specified, but no LRO_URLS set");
        return FALSE;
    }

    if (handle->update) {
        // Download/Locate only specified files
        if (!result->repo.deb.deb_repo) {
            g_set_error(err, LR_DEB_ERROR, LRE_INCOMPLETERESULT,
                    "Incomplete result object - "
                    "Cannot update on this result object");
            return FALSE;
        }
    } else {
        // Download/Locate from scratch
        if (result->repo.deb.deb_repo) {
            g_set_error(err, LR_DEB_ERROR, LRE_ALREADYUSEDRESULT,
                        "This result object is not clear - "
                        "Already used result object");
            return FALSE;
        }
        result->repo.deb.deb_repo = lr_deb_repo_init();
    }

    if (handle->local) {
        // Do not duplicate repository, just use the existing local one

    } else {
        // Download remote/Duplicate local repository
        // Note: All checksums are checked while downloading

        ret = lr_deb_download_remote(handle, result, err);
    }

    return ret;
}
