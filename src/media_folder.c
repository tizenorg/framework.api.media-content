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


#include <media_info_private.h>
#include <media_util_private.h>

static char *g_src_path = NULL;

int media_folder_get_folder_count_from_db(filter_h filter, int *folder_count)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;

	if(!folder_count)
	{
		media_content_error("INVALID_PARAMETER(0x%08x)", MEDIA_CONTENT_ERROR_INVALID_PARAMETER);
		return MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
	}

	ret = _media_db_get_group_count(filter, MEDIA_GROUP_FOLDER, folder_count);

	return ret;
}

int media_folder_foreach_folder_from_db(filter_h filter, media_folder_cb callback, void *user_data)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;

	if(callback == NULL)
	{
		media_content_error("INVALID_PARAMETER(0x%08x)", MEDIA_CONTENT_ERROR_INVALID_PARAMETER);
		return MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
	}

	ret = _media_db_get_folder(filter, callback, user_data);

	return ret;
}

int media_folder_get_media_count_from_db(const char *folder_id, filter_h filter, int *media_count)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;

	if(STRING_VALID(folder_id) && media_count)
	{
		ret = _media_db_get_group_item_count(folder_id, filter, MEDIA_GROUP_FOLDER, media_count);
	}
	else
	{
		media_content_error("INVALID_PARAMETER(0x%08x)", MEDIA_CONTENT_ERROR_INVALID_PARAMETER);
		return MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
	}

	return ret;
}

int media_folder_foreach_media_from_db(const char *folder_id, filter_h filter, media_info_cb callback, void *user_data)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;

	if((callback != NULL) && STRING_VALID(folder_id))
	{
		ret = _media_db_get_group_item(folder_id, filter, callback, user_data, MEDIA_GROUP_FOLDER);
	}
	else
	{
		media_content_error("INVALID_PARAMETER(0x%08x)", MEDIA_CONTENT_ERROR_INVALID_PARAMETER);
		return MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
	}

	return ret;
}

int media_folder_destroy(media_folder_h folder)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	media_folder_s *_folder = (media_folder_s*)folder;
	if(_folder)
	{
		SAFE_FREE(_folder->path);
		SAFE_FREE(_folder->name);
		SAFE_FREE(_folder->folder_id);
		SAFE_FREE(_folder->parent_folder_id);
		SAFE_FREE(_folder->storage_uuid);
		SAFE_FREE(_folder);
		ret = MEDIA_CONTENT_ERROR_NONE;
	}
	else
	{
		media_content_error("INVALID_PARAMETER(0x%08x)", MEDIA_CONTENT_ERROR_INVALID_PARAMETER);
		ret = MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
	}

	return ret;
}
int media_folder_clone(media_folder_h *dst, media_folder_h src)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	media_folder_s *_src = (media_folder_s*)src;

	if(_src != NULL)
	{
		media_folder_s *_dst = (media_folder_s*)calloc(1, sizeof(media_folder_s));
		media_content_retvm_if(_dst == NULL, MEDIA_CONTENT_ERROR_OUT_OF_MEMORY, "OUT_OF_MEMORY");

		if(STRING_VALID(_src->folder_id))
		{
			_dst->folder_id = strdup(_src->folder_id);
			if(_dst->folder_id == NULL)
			{
				SAFE_FREE(_dst);
				media_content_error("OUT_OF_MEMORY(0x%08x)", MEDIA_CONTENT_ERROR_OUT_OF_MEMORY);
				return MEDIA_CONTENT_ERROR_OUT_OF_MEMORY;
			}
		}

		if(STRING_VALID(_src->parent_folder_id))
		{
			_dst->parent_folder_id = strdup(_src->parent_folder_id);
			if(_dst->parent_folder_id == NULL)
			{
				media_folder_destroy((media_folder_h)_dst);
				media_content_error("OUT_OF_MEMORY(0x%08x)", MEDIA_CONTENT_ERROR_OUT_OF_MEMORY);
				return MEDIA_CONTENT_ERROR_OUT_OF_MEMORY;
			}
		}

		if(STRING_VALID(_src->name))
		{
			_dst->name = strdup(_src->name);
			if(_dst->name == NULL)
			{
				media_content_error("OUT_OF_MEMORY(0x%08x)", MEDIA_CONTENT_ERROR_OUT_OF_MEMORY);
				media_folder_destroy((media_folder_h)_dst);
				return MEDIA_CONTENT_ERROR_OUT_OF_MEMORY;
			}
		}

		if(STRING_VALID(_src->path))
		{
			_dst->path = strdup(_src->path);
			if(_dst->path == NULL)
			{
				media_content_error("OUT_OF_MEMORY(0x%08x)", MEDIA_CONTENT_ERROR_OUT_OF_MEMORY);
				media_folder_destroy((media_folder_h)_dst);
				return MEDIA_CONTENT_ERROR_OUT_OF_MEMORY;
			}
		}

		if(STRING_VALID(_src->storage_uuid))
		{
			_dst->storage_uuid = strdup(_src->storage_uuid);
			if(_dst->storage_uuid == NULL)
			{
				media_content_error("OUT_OF_MEMORY(0x%08x)", MEDIA_CONTENT_ERROR_OUT_OF_MEMORY);
				media_folder_destroy((media_folder_h)_dst);
				return MEDIA_CONTENT_ERROR_OUT_OF_MEMORY;
			}
		}

		_dst->storage_type = _src->storage_type;
		_dst->modified_time = _src->modified_time;
		_dst->folder_order= _src->folder_order;

		*dst = (media_folder_h)_dst;

		ret = MEDIA_CONTENT_ERROR_NONE;
	}
	else
	{
		media_content_error("INVALID_PARAMETER(0x%08x)", MEDIA_CONTENT_ERROR_INVALID_PARAMETER);
		ret = MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
	}

	return ret;
}

int media_folder_get_folder_id(media_folder_h folder, char **folder_id)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	media_folder_s *_folder = (media_folder_s*)folder;

	if(_folder)
	{
		if(STRING_VALID(_folder->folder_id))
		{
			*folder_id = strdup(_folder->folder_id);
			media_content_retvm_if(*folder_id == NULL, MEDIA_CONTENT_ERROR_OUT_OF_MEMORY, "OUT_OF_MEMORY");
		}
		else
		{
			*folder_id = NULL;
		}
		ret = MEDIA_CONTENT_ERROR_NONE;
	}
	else
	{
		media_content_error("INVALID_PARAMETER(0x%08x)", MEDIA_CONTENT_ERROR_INVALID_PARAMETER);
		ret = MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
	}

	return ret;
}

int media_folder_get_parent_folder_id(media_folder_h folder, char **parent_folder_id)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	media_folder_s *_folder = (media_folder_s*)folder;

	if(_folder)
	{
		if(STRING_VALID(_folder->parent_folder_id))
		{
			*parent_folder_id = strdup(_folder->parent_folder_id);
			media_content_retvm_if(*parent_folder_id == NULL, MEDIA_CONTENT_ERROR_OUT_OF_MEMORY, "OUT_OF_MEMORY");
		}
		else
		{
			*parent_folder_id = NULL;
		}
		ret = MEDIA_CONTENT_ERROR_NONE;
	}
	else
	{
		media_content_error("INVALID_PARAMETER(0x%08x)", MEDIA_CONTENT_ERROR_INVALID_PARAMETER);
		ret = MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
	}

	return ret;
}

int media_folder_get_path(media_folder_h folder, char **path)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	media_folder_s *_folder = (media_folder_s*)folder;

	if(_folder)
	{
		if(STRING_VALID(_folder->path))
		{
			*path = strdup(_folder->path);
			media_content_retvm_if(*path == NULL, MEDIA_CONTENT_ERROR_OUT_OF_MEMORY, "OUT_OF_MEMORY");
		}
		else
		{
			*path = NULL;
		}
		ret = MEDIA_CONTENT_ERROR_NONE;

	}
	else
	{
		media_content_error("INVALID_PARAMETER(0x%08x)", MEDIA_CONTENT_ERROR_INVALID_PARAMETER);
		ret = MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
	}

	return ret;

}

int media_folder_get_name(media_folder_h folder, char **name)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	media_folder_s *_folder = (media_folder_s*)folder;

	if(_folder)
	{
		if(STRING_VALID(_folder->name))
		{
			*name = strdup(_folder->name);
			media_content_retvm_if(*name == NULL, MEDIA_CONTENT_ERROR_OUT_OF_MEMORY, "OUT_OF_MEMORY");
		}
		else
		{
			*name = NULL;
		}
		ret = MEDIA_CONTENT_ERROR_NONE;

	}
	else
	{
		media_content_error("INVALID_PARAMETER(0x%08x)", MEDIA_CONTENT_ERROR_INVALID_PARAMETER);
		ret = MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
	}

	return ret;
}

int media_folder_get_modified_time(media_folder_h folder, time_t* time)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	media_folder_s *_folder = (media_folder_s*)folder;

	if(_folder)
	{
		*time = _folder->modified_time;
		ret = MEDIA_CONTENT_ERROR_NONE;
	}
	else
	{
		media_content_error("INVALID_PARAMETER(0x%08x)", MEDIA_CONTENT_ERROR_INVALID_PARAMETER);
		ret = MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
	}

	return ret;
}

int media_folder_get_storage_type(media_folder_h folder, media_content_storage_e* storage_type)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	media_folder_s *_folder = (media_folder_s*)folder;

	if(_folder)
	{
		*storage_type = _folder->storage_type;
		ret = MEDIA_CONTENT_ERROR_NONE;
	}
	else
	{
		media_content_error("INVALID_PARAMETER(0x%08x)", MEDIA_CONTENT_ERROR_INVALID_PARAMETER);
		ret = MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
	}

	return ret;
}

int media_folder_get_storage_id(media_folder_h folder, char **storage_id)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	media_folder_s *_folder = (media_folder_s*)folder;

	if(_folder)
	{
		if(STRING_VALID(_folder->storage_uuid))
		{
			*storage_id = strdup(_folder->storage_uuid);
			media_content_retvm_if(*storage_id == NULL, MEDIA_CONTENT_ERROR_OUT_OF_MEMORY, "OUT_OF_MEMORY");
		}
		else
		{
			*storage_id = NULL;
		}
		ret = MEDIA_CONTENT_ERROR_NONE;

	}
	else
	{
		media_content_error("INVALID_PARAMETER(0x%08x)", MEDIA_CONTENT_ERROR_INVALID_PARAMETER);
		ret = MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
	}

	return ret;
}

int media_folder_get_order(media_folder_h folder, int *order)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	media_folder_s *_folder = (media_folder_s*)folder;

	if(_folder)
	{
		*order = _folder->folder_order;
		ret = MEDIA_CONTENT_ERROR_NONE;
	}
	else
	{
		media_content_error("INVALID_PARAMETER(0x%08x)", MEDIA_CONTENT_ERROR_INVALID_PARAMETER);
		ret = MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
	}

	return ret;
}


int media_folder_get_folder_from_db(const char *folder_id, media_folder_h *folder)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	sqlite3_stmt *stmt = NULL;
	char select_query[DEFAULT_QUERY_SIZE];

	if(!STRING_VALID(folder_id))
	{
		media_content_error("INVALID_PARAMETER(0x%08x)", MEDIA_CONTENT_ERROR_INVALID_PARAMETER);
		return MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
	}

	memset(select_query, 0x00, sizeof(select_query));

	snprintf(select_query, sizeof(select_query), SELECT_FOLDER_FROM_FOLDER, folder_id);

	ret = _content_query_prepare(&stmt, select_query, NULL, NULL);
	media_content_retv_if(ret != MEDIA_CONTENT_ERROR_NONE, ret);

	media_folder_s *_folder = NULL;

	while(sqlite3_step(stmt) == SQLITE_ROW)
	{
		if(_folder)
			media_folder_destroy((media_folder_h)_folder);

		_folder = (media_folder_s*)calloc(1, sizeof(media_folder_s));
		if(_folder == NULL)
		{
			media_content_error("OUT_OF_MEMORY(0x%08x)", MEDIA_CONTENT_ERROR_OUT_OF_MEMORY);
			SQLITE3_FINALIZE(stmt);
			return MEDIA_CONTENT_ERROR_OUT_OF_MEMORY;
		}

		media_content_error("folder handle %x", _folder);

		if(STRING_VALID((const char *)sqlite3_column_text(stmt, 0)))
			_folder->folder_id = strdup((const char *)sqlite3_column_text(stmt, 0));

		if(STRING_VALID((const char *)sqlite3_column_text(stmt, 1)))
			_folder->path = strdup((const char *)sqlite3_column_text(stmt, 1));

		if(STRING_VALID((const char *)sqlite3_column_text(stmt, 2)))
			_folder->name = strdup((const char *)sqlite3_column_text(stmt, 2));

		*folder = (media_folder_h)_folder;
	}

	SQLITE3_FINALIZE(stmt);

	return ret;
}

int media_folder_update_to_db(media_folder_h folder)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	media_folder_s *_folder = (media_folder_s*)folder;
	char *sql = NULL;

	if((_folder == NULL) || (_folder->folder_id == NULL))
	{
		media_content_error("INVALID_PARAMETER(0x%08x)", MEDIA_CONTENT_ERROR_INVALID_PARAMETER);
		return MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
	}

	/* Update folder order */
	sql = sqlite3_mprintf(UPDATE_FOLDER_ORDER, _folder->folder_order, _folder->folder_id);

	ret = _content_query_sql(sql);
	SQLITE3_SAFE_FREE(sql);

	if(STRING_VALID(_folder->folder_id) && g_src_path)
	{
		char *name_pinyin = NULL;
		bool pinyin_support = FALSE;

		/* Set modified time */
		time_t date;
		time(&date);
		_folder->modified_time = date;

		/*Update Pinyin If Support Pinyin*/
		media_svc_check_pinyin_support(&pinyin_support);
		if(pinyin_support)
			media_svc_get_pinyin(_content_get_db_handle(), _folder->name, &name_pinyin);

		sql = sqlite3_mprintf("UPDATE %Q SET path='%q', name='%q', modified_time=%d, name_pinyin='%q' WHERE folder_uuid=%Q",
						DB_TABLE_FOLDER, _folder->path, _folder->name, _folder->modified_time, name_pinyin, _folder->folder_id);

		ret = _content_query_sql(sql);
		SQLITE3_SAFE_FREE(sql);
		if (ret != MEDIA_CONTENT_ERROR_NONE) {
			SAFE_FREE(g_src_path);
			return ret;
		}

		/* Update all folder record's path, which are matched by old parent path */
		char *update_folder_path_sql = NULL;
		char src_path_slash[MAX_QUERY_SIZE + 1] = {0, };
		char dst_path_slash[MAX_QUERY_SIZE + 1] = {0, };

		snprintf(src_path_slash, sizeof(src_path_slash), "%s/", g_src_path);
		snprintf(dst_path_slash, sizeof(dst_path_slash), "%s/", _folder->path);

		update_folder_path_sql = sqlite3_mprintf("UPDATE folder SET path = REPLACE( path, '%q', '%q');", src_path_slash, dst_path_slash);

		ret = _content_query_sql(update_folder_path_sql);
		SQLITE3_SAFE_FREE(update_folder_path_sql);
		if (ret != MEDIA_CONTENT_ERROR_NONE) {
			SAFE_FREE(g_src_path);
			return ret;
		}

		ret = media_svc_rename_folder(_content_get_db_handle(), _folder->storage_uuid, g_src_path, _folder->path);
		SAFE_FREE(g_src_path);

		return _content_error_capi(MEDIA_CONTENT_TYPE, ret);
	}

	return ret;
}

int media_folder_set_name(media_folder_h folder, const char *name)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	media_folder_s *_folder = (media_folder_s*)folder;

	if(_folder != NULL && STRING_VALID(name))
	{
		if (STRING_VALID(_folder->path) && STRING_VALID(_folder->name)) {
			char *folder_path = NULL;
			char new_folder_path[MAX_QUERY_SIZE] = {0,};
			bool ignore_dir = FALSE;

			folder_path = g_path_get_dirname(_folder->path);
			media_content_sec_debug("Existed Folder Path : %s", _folder->path);
			snprintf(new_folder_path, sizeof(new_folder_path), "%s/%s", folder_path, name); 
			media_content_sec_debug("New Path : %s", new_folder_path);

			ret = _media_util_check_ignore_dir(new_folder_path, &ignore_dir);
			if (ignore_dir == TRUE) {
				media_content_error("Invalid folder path");
				SAFE_FREE(folder_path);
				return ret;
			}

			SAFE_FREE(g_src_path);
			g_src_path = strdup(_folder->path);

			SAFE_FREE(_folder->path);
			SAFE_FREE(_folder->name);
			SAFE_FREE(folder_path);
			_folder->path = strdup(new_folder_path);
			media_content_retvm_if(_folder->path == NULL, MEDIA_CONTENT_ERROR_OUT_OF_MEMORY, "OUT_OF_MEMORY");
		}

		_folder->name = strdup(name);
		media_content_retvm_if(_folder->name == NULL, MEDIA_CONTENT_ERROR_OUT_OF_MEMORY, "OUT_OF_MEMORY");
	}
	else
	{
		media_content_error("INVALID_PARAMETER(0x%08x)", MEDIA_CONTENT_ERROR_INVALID_PARAMETER);
		ret = MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
	}

	return ret;
}

int media_folder_set_order(media_folder_h folder, int order)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	media_folder_s *_folder = (media_folder_s*)folder;

	if(_folder != NULL && order >= 0)
	{
		_folder->folder_order = order;
	}
	else
	{
		media_content_error("INVALID_PARAMETER(0x%08x)", MEDIA_CONTENT_ERROR_INVALID_PARAMETER);
		ret = MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
	}

	return ret;
}
