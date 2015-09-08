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

static char *__media_folder_get_update_folder_sql(media_folder_h folder);

static char *__media_folder_get_update_folder_sql(media_folder_h folder)
{
	media_folder_s *_folder = (media_folder_s*)folder;
	char *return_sql = NULL;
	char *name_pinyin = NULL;
	bool pinyin_support = FALSE;

	/*Update Pinyin If Support Pinyin*/
	media_svc_check_pinyin_support(&pinyin_support);
	if(pinyin_support)
		media_svc_get_pinyin(_content_get_db_handle(), _folder->name, &name_pinyin);

	return_sql = sqlite3_mprintf("%q='%q', %q='%q', %q=%d, %q='%q'",
											DB_FIELD_FOLDER_PATH, _folder->path,
											DB_FIELD_FOLDER_NAME, _folder->name,
											DB_FIELD_FOLDER_MODIFIED_TIME, _folder->modified_time,
											DB_FIELD_FOLDER_NAME_PINYIN, name_pinyin);

	SAFE_FREE(name_pinyin);
	return return_sql;
}

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
		if(_dst == NULL)
		{
			media_content_error("OUT_OF_MEMORY(0x%08x)", MEDIA_CONTENT_ERROR_OUT_OF_MEMORY);
			return MEDIA_CONTENT_ERROR_OUT_OF_MEMORY;
		}

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

		_dst->storage_type = _src->storage_type;

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
			if(*folder_id == NULL)
			{
				media_content_error("OUT_OF_MEMORY(0x%08x)", MEDIA_CONTENT_ERROR_OUT_OF_MEMORY);
				return MEDIA_CONTENT_ERROR_OUT_OF_MEMORY;
			}
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

int media_folder_get_path(media_folder_h folder, char **path)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	media_folder_s *_folder = (media_folder_s*)folder;
	if(_folder)
	{
		if(STRING_VALID(_folder->path))
		{
			*path = strdup(_folder->path);
			if(*path == NULL)
			{
				media_content_error("OUT_OF_MEMORY(0x%08x)", MEDIA_CONTENT_ERROR_OUT_OF_MEMORY);
				return MEDIA_CONTENT_ERROR_OUT_OF_MEMORY;
			}
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
			if(*name == NULL)
			{
				media_content_error("OUT_OF_MEMORY(0x%08x)", MEDIA_CONTENT_ERROR_OUT_OF_MEMORY);
				return MEDIA_CONTENT_ERROR_OUT_OF_MEMORY;
			}
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

	while(sqlite3_step(stmt) == SQLITE_ROW)
	{
		media_folder_s *_folder = (media_folder_s*)calloc(1, sizeof(media_folder_s));

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
	char *set_sql = NULL;
	char *sql = NULL;

	if((_folder != NULL) && STRING_VALID(_folder->folder_id) && g_src_path)
	{
		/* Set modified time */
		time_t date;
		time(&date);
		_folder->modified_time = date;

		set_sql = __media_folder_get_update_folder_sql((media_folder_h)_folder);
		if(set_sql == NULL)
		{
			media_content_error("OUT_OF_MEMORY(0x%08x)", MEDIA_CONTENT_ERROR_OUT_OF_MEMORY);
			return MEDIA_CONTENT_ERROR_OUT_OF_MEMORY;
		}

		sql = sqlite3_mprintf("UPDATE %Q SET %s WHERE folder_uuid=%Q", DB_TABLE_FOLDER, set_sql, _folder->folder_id);

		ret = _content_query_sql(sql);

		sqlite3_free(set_sql);
		sqlite3_free(sql);
		if (ret != MEDIA_CONTENT_ERROR_NONE)
			return ret;

		/* Do folder rename operation using libmedia-service */
		ret = media_svc_rename_folder(_content_get_db_handle(), g_src_path, _folder->path);
		return _content_error_capi(MEDIA_CONTENT_TYPE, ret);
	}
	else
	{
		media_content_error("INVALID_PARAMETER(0x%08x)", MEDIA_CONTENT_ERROR_INVALID_PARAMETER);
		ret = MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
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

			folder_path = g_path_get_dirname(_folder->path);
			media_content_sec_debug("Existed Folder Path : %s", _folder->path);
			snprintf(new_folder_path, sizeof(new_folder_path), "%s/%s", folder_path, name); 
			media_content_sec_debug("New Path : %s", new_folder_path);

			SAFE_FREE(g_src_path);
			g_src_path = strdup(_folder->path);

			SAFE_FREE(_folder->path);
			SAFE_FREE(_folder->name);
			SAFE_FREE(folder_path);
			_folder->path = strdup(new_folder_path);

			if(_folder->path == NULL)
			{
				media_content_error("OUT_OF_MEMORY(0x%08x)", MEDIA_CONTENT_ERROR_OUT_OF_MEMORY);
				return MEDIA_CONTENT_ERROR_OUT_OF_MEMORY;
			}
		}

		_folder->name = strdup(name);
		if(_folder->name == NULL)
		{
			media_content_error("OUT_OF_MEMORY(0x%08x)", MEDIA_CONTENT_ERROR_OUT_OF_MEMORY);
			return MEDIA_CONTENT_ERROR_OUT_OF_MEMORY;
		}
	}
	else
	{
		media_content_error("INVALID_PARAMETER(0x%08x)", MEDIA_CONTENT_ERROR_INVALID_PARAMETER);
		ret = MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
	}

	return ret;
}
