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


#include <media_content.h>
#include <media_info_private.h>
#include <media_content_internal.h>

static void __media_storage_get_detail(sqlite3_stmt* stmt, media_storage_h storage)
{
	media_storage_s *_storage = (media_storage_s*)storage;

	if(STRING_VALID((const char *)sqlite3_column_text(stmt, 0)))
		_storage->storage_id = strdup((const char *)sqlite3_column_text(stmt, 0));

	if(STRING_VALID((const char *)sqlite3_column_text(stmt, 1)))
		_storage->storage_name = strdup((const char *)sqlite3_column_text(stmt, 1));

	if(STRING_VALID((const char *)sqlite3_column_text(stmt, 2)))
		_storage->storage_path = strdup((const char *)sqlite3_column_text(stmt, 2));

	if(STRING_VALID((const char *)sqlite3_column_text(stmt, 3)))
		_storage->storage_account = strdup((const char *)sqlite3_column_text(stmt, 3));

	_storage->storage_type = (int)sqlite3_column_int(stmt, 4);

	return;
}

int media_storage_insert_to_db(const char *storage_name, const char *storage_path, const char *storage_account, media_content_storage_e storage_type, media_storage_h *storage)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	char *storage_uuid = NULL;
	media_storage_s *_storage = NULL;

	media_content_retvm_if(!STRING_VALID(storage_name), MEDIA_CONTENT_ERROR_INVALID_PARAMETER, "invalid storage_name");
	media_content_retvm_if(!STRING_VALID(storage_path), MEDIA_CONTENT_ERROR_INVALID_PARAMETER, "invalid storage_path");

	ret = media_svc_generate_uuid(&storage_uuid);
	media_content_retvm_if(ret != MS_MEDIA_ERR_NONE, MEDIA_CONTENT_ERROR_INVALID_OPERATION, "Fail to get storage_id");
	media_content_retvm_if(storage_uuid == NULL, MEDIA_CONTENT_ERROR_INVALID_OPERATION, "Invalid storage_id");

	ret = media_svc_insert_storage(_content_get_db_handle(), storage_uuid, storage_name, storage_path, storage_account, storage_type);
	if (ret != MS_MEDIA_ERR_NONE)
	{
		ret = _content_error_capi(MEDIA_CONTENT_TYPE, ret);
		SAFE_FREE(storage_uuid);
		return ret;
	}

	_storage = (media_storage_s*)calloc(1, sizeof(media_storage_s));
	media_content_retvm_if(_storage == NULL, MEDIA_CONTENT_ERROR_OUT_OF_MEMORY, "OUT_OF_MEMORY");

	_storage->storage_id = strdup(storage_uuid);
	_storage->storage_path = strdup(storage_path);
	_storage->storage_name = strdup(storage_name);
	_storage->storage_type = storage_type;

	*storage = (media_storage_h)_storage;

	SAFE_FREE(storage_uuid);

	return ret;
}

int media_storage_delete_from_db(const char *storage_id)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;

	if(!STRING_VALID(storage_id))
	{
		media_content_error("Invalid Storage ID");
		return MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
	}

	ret = media_svc_delete_storage(_content_get_db_handle(), storage_id, NULL);

	return _content_error_capi(MEDIA_CONTENT_TYPE, ret);
}

int media_storage_get_storage_info_from_db(const char *storage_id, media_storage_h *storage)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	char select_query[DEFAULT_QUERY_SIZE];
	sqlite3_stmt *stmt = NULL;

	if(!STRING_VALID(storage_id) || (storage == NULL))
	{
		media_content_error("INVALID_PARAMETER(0x%08x)", MEDIA_CONTENT_ERROR_INVALID_PARAMETER);
		return MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
	}

	memset(select_query, 0x00, sizeof(select_query));
	snprintf(select_query, sizeof(select_query), SELECT_STORAGE_INFO_FROM_STORAGE, storage_id);

	ret = _content_query_prepare(&stmt, select_query, NULL, NULL);
	media_content_retv_if(ret != MEDIA_CONTENT_ERROR_NONE, ret);

	while(sqlite3_step(stmt) == SQLITE_ROW)
	{
		media_storage_s *_storage = (media_storage_s*)calloc(1, sizeof(media_storage_s));

		if(_storage == NULL)
		{
			media_content_error("OUT_OF_MEMORY(0x%08x)", MEDIA_CONTENT_ERROR_OUT_OF_MEMORY);
			SQLITE3_FINALIZE(stmt);
			return MEDIA_CONTENT_ERROR_OUT_OF_MEMORY;
		}

		__media_storage_get_detail(stmt, (media_storage_h)_storage);

		*storage = (media_storage_h)_storage;
	}

	SQLITE3_FINALIZE(stmt);

	return ret;
}

int media_storage_get_storage_count_from_db(filter_h filter, int *storage_count)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;

	if(storage_count)
	{
		ret = _media_db_get_group_count(filter, MEDIA_GROUP_STORAGE, storage_count);
	}
	else
	{
		media_content_error("INVALID_PARAMETER(0x%08x)", MEDIA_CONTENT_ERROR_INVALID_PARAMETER);
		return MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
	}

	return ret;
}

int media_storage_foreach_storage_from_db(filter_h filter, media_storage_cb callback, void *user_data)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;

	if(callback != NULL)
	{
		ret = _media_db_get_storage(filter, callback, user_data);
	}
	else
	{
		media_content_error("INVALID_PARAMETER(0x%08x)", MEDIA_CONTENT_ERROR_INVALID_PARAMETER);
		return MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
	}

	return ret;
}

int media_storage_get_media_count_from_db(const char *storage_id, filter_h filter, int *media_count)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;

	if(STRING_VALID(storage_id) && media_count)
	{
		ret = _media_db_get_group_item_count(storage_id, filter, MEDIA_GROUP_STORAGE, media_count);
	}
	else
	{
		media_content_error("INVALID_PARAMETER(0x%08x)", MEDIA_CONTENT_ERROR_INVALID_PARAMETER);
		return MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
	}

	return ret;
}

int media_storage_foreach_media_from_db(const char *storage_id, filter_h filter, media_info_cb callback, void *user_data)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;

	if((callback != NULL) && STRING_VALID(storage_id))
	{
		ret = _media_db_get_group_item(storage_id, filter, callback, user_data, MEDIA_GROUP_STORAGE);
	}
	else
	{
		media_content_error("INVALID_PARAMETER(0x%08x)", MEDIA_CONTENT_ERROR_INVALID_PARAMETER);
		return MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
	}

	return ret;
}

int media_storage_destroy(media_storage_h storage)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	media_storage_s *_storage = (media_storage_s*)storage;
	if(_storage)
	{
		SAFE_FREE(_storage->storage_id);
		SAFE_FREE(_storage->storage_path);
		SAFE_FREE(_storage->storage_name);
		SAFE_FREE(_storage->storage_account);
		SAFE_FREE(_storage);

		ret = MEDIA_CONTENT_ERROR_NONE;
	}
	else
	{
		media_content_error("INVALID_PARAMETER(0x%08x)", MEDIA_CONTENT_ERROR_INVALID_PARAMETER);
		ret = MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
	}

	return ret;
}

int media_storage_clone(media_storage_h *dst, media_storage_h src)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	media_storage_s *_src = (media_storage_s*)src;

	if(_src != NULL)
	{
		media_storage_s *_dst = (media_storage_s*)calloc(1, sizeof(media_storage_s));
		media_content_retvm_if(_dst == NULL, MEDIA_CONTENT_ERROR_OUT_OF_MEMORY, "OUT_OF_MEMORY");

		if(STRING_VALID(_src->storage_id))
		{
			_dst->storage_id = strdup(_src->storage_id);
			if(_dst->storage_id == NULL)
			{
				media_storage_destroy((media_storage_h)_dst);
				media_content_error("OUT_OF_MEMORY(0x%08x)", MEDIA_CONTENT_ERROR_OUT_OF_MEMORY);
				return MEDIA_CONTENT_ERROR_OUT_OF_MEMORY;
			}
		}

		if(STRING_VALID(_src->storage_name))
		{
			_dst->storage_name = strdup(_src->storage_name);
			if(_dst->storage_name == NULL)
			{
				media_content_error("OUT_OF_MEMORY(0x%08x)", MEDIA_CONTENT_ERROR_OUT_OF_MEMORY);
				media_storage_destroy((media_storage_h)_dst);
				return MEDIA_CONTENT_ERROR_OUT_OF_MEMORY;
			}
		}

		if(STRING_VALID(_src->storage_path))
		{
			_dst->storage_path = strdup(_src->storage_path);
			if(_dst->storage_path == NULL)
			{
				media_content_error("OUT_OF_MEMORY(0x%08x)", MEDIA_CONTENT_ERROR_OUT_OF_MEMORY);
				media_storage_destroy((media_storage_h)_dst);
				return MEDIA_CONTENT_ERROR_OUT_OF_MEMORY;
			}
		}

		if(STRING_VALID(_src->storage_account))
		{
			_dst->storage_account = strdup(_src->storage_account);
			if(_dst->storage_account == NULL)
			{
				media_content_error("OUT_OF_MEMORY(0x%08x)", MEDIA_CONTENT_ERROR_OUT_OF_MEMORY);
				media_storage_destroy((media_storage_h)_dst);
				return MEDIA_CONTENT_ERROR_OUT_OF_MEMORY;
			}
		}

		_dst->storage_type = _src->storage_type;

		*dst = (media_storage_h)_dst;

		ret = MEDIA_CONTENT_ERROR_NONE;
	}
	else
	{
		media_content_error("INVALID_PARAMETER(0x%08x)", MEDIA_CONTENT_ERROR_INVALID_PARAMETER);
		ret = MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
	}

	return ret;
}

int media_storage_get_id(media_storage_h storage, char **storage_id)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	media_storage_s *_storage = (media_storage_s*)storage;

	if(_storage && storage_id)
	{
		if(STRING_VALID(_storage->storage_id))
		{
			*storage_id = strdup(_storage->storage_id);
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

int media_storage_get_name(media_storage_h storage, char **storage_name)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	media_storage_s *_storage = (media_storage_s*)storage;

	if(_storage && storage_name)
	{
		if(STRING_VALID(_storage->storage_name))
		{
			*storage_name = strdup(_storage->storage_name);
			media_content_retvm_if(*storage_name == NULL, MEDIA_CONTENT_ERROR_OUT_OF_MEMORY, "OUT_OF_MEMORY");
		}
		else
		{
			*storage_name = NULL;
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

int media_storage_get_path(media_storage_h storage, char **storage_path)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	media_storage_s *_storage = (media_storage_s*)storage;

	if(_storage && storage_path)
	{
		if(STRING_VALID(_storage->storage_path))
		{
			*storage_path = strdup(_storage->storage_path);
			media_content_retvm_if(*storage_path == NULL, MEDIA_CONTENT_ERROR_OUT_OF_MEMORY, "OUT_OF_MEMORY");
		}
		else
		{
			*storage_path = NULL;
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

int media_storage_get_storage_account(media_storage_h storage, char **storage_account)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	media_storage_s *_storage = (media_storage_s*)storage;

	if(_storage && storage_account)
	{
		if(STRING_VALID(_storage->storage_account))
		{
			*storage_account = strdup(_storage->storage_account);
			media_content_retvm_if(*storage_account == NULL, MEDIA_CONTENT_ERROR_OUT_OF_MEMORY, "OUT_OF_MEMORY");
		}
		else
		{
			*storage_account = NULL;
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

int media_storage_get_type(media_storage_h storage, media_content_storage_e *storage_type)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	media_storage_s *_storage = (media_storage_s*)storage;

	if(_storage && storage_type)
	{
		*storage_type = _storage->storage_type;
		ret = MEDIA_CONTENT_ERROR_NONE;
	}
	else
	{
		media_content_error("INVALID_PARAMETER(0x%08x)", MEDIA_CONTENT_ERROR_INVALID_PARAMETER);
		ret = MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
	}

	return ret;
}

int media_storage_get_scan_status(const char *storage_uuid, media_storage_scan_status_e *scan_status)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	media_svc_scan_status_type_e status = MEDIA_STORAGE_SCAN_NONE;

	if(STRING_VALID(storage_uuid))
	{
		ret = media_svc_get_storage_scan_status(_content_get_db_handle(), storage_uuid, &status);
		if (ret != MS_MEDIA_ERR_NONE) {
			media_content_error("media_svc_get_storage_scan_status failed");
			ret = _content_error_capi(MEDIA_CONTENT_TYPE, ret);
		} else {
			*scan_status = status;
		}
	}
	else
	{
		media_content_error("INVALID_PARAMETER(0x%08x)", MEDIA_CONTENT_ERROR_INVALID_PARAMETER);
		ret = MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
	}

	return ret;
}