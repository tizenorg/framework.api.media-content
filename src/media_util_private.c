/*
* Copyright (c) 2011 Samsung Electronics Co., Ltd All Rights Reserved
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/

#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <media_util_private.h>
#include <media_info_private.h>
#include <media_content_type.h>

int _media_util_check_file(const char *path)
{
	int exist;

	/* check the file exits actually */
	exist = open(path, O_RDONLY);
	if(exist < 0) {
		media_content_sec_debug("path [%s]", path);
		media_content_stderror("open file fail");
		if (errno == EACCES || errno == EPERM) {
			return MEDIA_CONTENT_ERROR_PERMISSION_DENIED;
		} else {
			return MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
		}
	}

	close(exist);

	return MEDIA_CONTENT_ERROR_NONE;
}

int _media_util_check_ignore_dir(const char *dir_path, bool *ignore)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	media_svc_storage_type_e storage_type = 0;
	char *scan_ignore = ".scan_ignore";
	bool find = false;

	media_content_sec_debug("dir_path : %s", dir_path);

	if(!STRING_VALID(dir_path))
	{
		media_content_error("INVALID_PARAMETER(0x%08x)", MEDIA_CONTENT_ERROR_INVALID_PARAMETER);
		return MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
	}

	*ignore = FALSE;
	/*1. Check Hidden Directory*/
	if(strstr(dir_path, "/."))
	{
		*ignore = TRUE;
		media_content_info("hidden path");
		return MEDIA_CONTENT_ERROR_NONE;
	}

	/*2. Check Scan Ignore Directory*/
	ret = media_svc_get_storage_type(dir_path, &storage_type);
	if(ret != MEDIA_INFO_ERROR_NONE)
	{
		media_content_error("media_svc_get_storage_type failed : %d", ret);
		return _content_error_capi(MEDIA_CONTENT_TYPE, ret);
	}

	DIR *dp = NULL;
	struct dirent entry;
	struct dirent *result = NULL;

	char *leaf_path = NULL;
	char search_path[4096] = {0, };

	strncpy(search_path, dir_path, strlen(dir_path));
	while(STRING_VALID(search_path))
	{
		dp = opendir(search_path);
		if(dp == NULL)
		{
			media_content_error("Fail Open Directory");
			return MEDIA_CONTENT_ERROR_INVALID_OPERATION;
		}

		while (!readdir_r(dp, &entry, &result))
		{
			if (result == NULL)
				break;

			if(STRING_VALID(entry.d_name) && (strcmp(entry.d_name, scan_ignore) == 0))
			{
				media_content_info("Find Ignore path");
				media_content_sec_debug("Ignore path[%s]", search_path);
				find = TRUE;
				break;
			}
			else
			{
				//media_content_sec_debug("entry.d_name[%s]", entry.d_name);
				continue;
			}
		}

		if (dp)	closedir(dp);
		dp = NULL;

		if(find)
		{
			*ignore = TRUE;
			break;
		}
		else
		{
			/*If root path, Stop Scanning*/
			if((storage_type == MEDIA_SVC_STORAGE_INTERNAL) && (strcmp(search_path, MEDIA_ROOT_PATH_INTERNAL) == 0))
			{
				//media_content_debug("Internal root path. Stop Scanning. Not found Ignore information");
				break;
			}
			else if((storage_type == MEDIA_SVC_STORAGE_EXTERNAL) && (strcmp(search_path, MEDIA_ROOT_PATH_SDCARD) == 0))
			{
				//media_content_debug("Enternal root path. Stop Scanning. Not found Ignore information");
				break;
			}

			leaf_path = strrchr(search_path, '/');
			if(leaf_path != NULL)
			{
				int seek_len = leaf_path -search_path;
				search_path[seek_len] = '\0';
				//media_content_sec_debug("go to other dir [%s]", search_path);
			}
			else
			{
				media_content_debug("Fail to find leaf path");
				break;
			}
		}
	}

	return MEDIA_CONTENT_ERROR_NONE;
}
