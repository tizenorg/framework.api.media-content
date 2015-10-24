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

static void __media_tag_item_add(media_tag_s *tag_s, media_tag_item_s *item_s);
static void __media_tag_item_release(media_tag_s *tag_s);
static int __media_tag_insert_item_to_tag(int tag_id, const char *media_id);
static int __media_tag_remove_item_from_tag(int tag_id, const char *media_id);
static int __media_tag_update_tag_name(int tag_id, const char *tag_name);
static int __media_tag_get_tag_info_from_db(const char *name, media_tag_h tag);

static void __media_tag_item_add(media_tag_s *tag_s, media_tag_item_s *item_s)
{
	tag_s->item_list = g_list_append(tag_s->item_list, item_s);
}

static void __media_tag_item_release(media_tag_s *tag_s)
{
	int idx = 0;
	int list_cnt = 0;
	media_tag_item_s *item = NULL;

	list_cnt = g_list_length(tag_s->item_list);

	media_content_debug("list_cnt : [%d]", list_cnt);

	for(idx = 0; idx < list_cnt; idx++)
	{
		item = (media_tag_item_s*)g_list_nth_data(tag_s->item_list, idx);
		if(item != NULL)
		{
			SAFE_FREE(item->media_id);
			SAFE_FREE(item->tag_name);
			SAFE_FREE(item);
		}
	}

	g_list_free(tag_s->item_list);
	tag_s->item_list = NULL;

}

static int __media_tag_insert_item_to_tag(int tag_id, const char *media_id)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	char *query_str = NULL;

	query_str = sqlite3_mprintf("INSERT INTO %q (tag_id, media_uuid) values (%d, '%q')",
			DB_TABLE_TAG_MAP, tag_id, media_id);
	ret = _content_query_sql(query_str);
	SQLITE3_SAFE_FREE(query_str);

	return ret;
}

static int __media_tag_remove_item_from_tag(int tag_id, const char *media_id)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	char *query_str = NULL;

	query_str = sqlite3_mprintf(REMOVE_TAG_ITEM_FROM_TAG_MAP, tag_id, media_id);

	ret = _content_query_sql(query_str);
	SQLITE3_SAFE_FREE(query_str);

	return ret;
}

static int __media_tag_update_tag_name(int tag_id, const char *tag_name)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	char *query_str = NULL;

	query_str = sqlite3_mprintf(UPDATE_TAG_NAME_FROM_TAG, tag_name, tag_id);

	ret = _content_query_sql(query_str);
	SQLITE3_SAFE_FREE(query_str);

	return ret;
}

static int __media_tag_get_tag_info_from_db(const char *name, media_tag_h tag)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	sqlite3_stmt *stmt = NULL;
	char *select_query = NULL;
	int id = 0;
	char *tag_name = NULL;
	media_tag_s *_tag = (media_tag_s*)tag;

	media_content_retvm_if(_tag == NULL, MEDIA_CONTENT_ERROR_INVALID_PARAMETER, "invalid tag");

	select_query = sqlite3_mprintf(SELECT_TAG_BY_NAME, name);

	ret = _content_query_prepare(&stmt, select_query, NULL, NULL);
	SQLITE3_SAFE_FREE(select_query);
	media_content_retv_if(ret != MEDIA_CONTENT_ERROR_NONE, ret);

	while(sqlite3_step(stmt) == SQLITE_ROW)
	{
		id = (int)sqlite3_column_int(stmt,0);

		if(STRING_VALID((const char *)sqlite3_column_text(stmt, 1)))
		{
			if(tag_name)
				SAFE_FREE(tag_name);

			tag_name = strdup((const char *)sqlite3_column_text(stmt, 1));
		}
	}

	_tag->tag_id = id;
	_tag->name = tag_name;

	SQLITE3_FINALIZE(stmt);

	return ret;
}

int media_tag_insert_to_db(const char *tag_name, media_tag_h *tag)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	char *query_str = NULL;

	if(!STRING_VALID(tag_name))
	{
		media_content_error("INVALID_PARAMETER(0x%08x)", MEDIA_CONTENT_ERROR_INVALID_PARAMETER);
		return MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
	}

	query_str = sqlite3_mprintf(INSERT_TAG_TO_TAG, tag_name);
	ret = _content_query_sql(query_str);
	SQLITE3_SAFE_FREE(query_str);

	if(ret == MEDIA_CONTENT_ERROR_NONE)
	{
		media_tag_s *_tag = (media_tag_s*)calloc(1, sizeof(media_tag_s));
		media_content_retvm_if(_tag == NULL, MEDIA_CONTENT_ERROR_OUT_OF_MEMORY, "OUT_OF_MEMORY");

		ret = __media_tag_get_tag_info_from_db(tag_name, (media_tag_h)_tag);
		*tag = (media_tag_h)_tag;
	}

	return ret;
}

int media_tag_delete_from_db(int tag_id)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	char *query_str = NULL;

	if(tag_id < 0)
	{
		media_content_error("INVALID_PARAMETER(0x%08x)", MEDIA_CONTENT_ERROR_INVALID_PARAMETER);
		return MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
	}

	query_str = sqlite3_mprintf(DELETE_TAG_FROM_TAG, tag_id);

	ret = _content_query_sql(query_str);

	SQLITE3_SAFE_FREE(query_str);

	return ret;
}

int media_tag_get_tag_count_from_db(filter_h filter, int *tag_count)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;

	if(tag_count != NULL)
	{
		ret = _media_db_get_group_count(filter, MEDIA_GROUP_TAG, tag_count);
	}
	else
	{
		media_content_error("INVALID_PARAMETER(0x%08x)", MEDIA_CONTENT_ERROR_INVALID_PARAMETER);
		ret = MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
	}

	return ret;
}

int media_tag_foreach_tag_from_db (filter_h filter, media_tag_cb callback, void *user_data)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;

	if(callback == NULL)
	{
		media_content_error("INVALID_PARAMETER(0x%08x)", MEDIA_CONTENT_ERROR_INVALID_PARAMETER);
		return MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
	}

	ret = _media_db_get_tag(NULL, filter, callback, user_data);

	return ret;
}

int media_tag_get_media_count_from_db(int tag_id, filter_h filter, int *media_count)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;

	if((tag_id > 0) && (media_count != NULL))
	{
		ret = _media_db_get_group_item_count_by_id(tag_id, filter, MEDIA_GROUP_TAG, media_count);
	}
	else
	{
		media_content_error("INVALID_PARAMETER(0x%08x)", MEDIA_CONTENT_ERROR_INVALID_PARAMETER);
		ret = MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
	}

	return ret;
}

int media_tag_foreach_media_from_db(int tag_id, filter_h filter, media_info_cb callback, void *user_data)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;

	if(callback == NULL)
	{
		media_content_error("INVALID_PARAMETER(0x%08x)", MEDIA_CONTENT_ERROR_INVALID_PARAMETER);
		return MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
	}

	ret = _media_db_get_group_item_by_id(tag_id, filter, callback, user_data, MEDIA_GROUP_TAG);

	return ret;
}

int media_tag_destroy(media_tag_h tag)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	media_tag_s *_tag = (media_tag_s*)tag;
	if(_tag)
	{
		SAFE_FREE(_tag->name);
		SAFE_FREE(_tag);
		ret = MEDIA_CONTENT_ERROR_NONE;
	}
	else
	{
		media_content_error("INVALID_PARAMETER(0x%08x)", MEDIA_CONTENT_ERROR_INVALID_PARAMETER);
		ret = MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
	}

	return ret;
}
int media_tag_clone(media_tag_h *dst, media_tag_h src)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	media_tag_s *_src = (media_tag_s*)src;
	media_tag_s *_dst = NULL;

	if((_src != NULL))
	{
		_dst = (media_tag_s*)calloc(1, sizeof(media_tag_s));
		media_content_retvm_if(_dst == NULL, MEDIA_CONTENT_ERROR_OUT_OF_MEMORY, "OUT_OF_MEMORY");

		_dst->tag_id = _src->tag_id;

		if(STRING_VALID(_src->name))
		{
			_dst->name = strdup(_src->name);
			if(_dst->name == NULL)
			{
				SAFE_FREE(_dst);
				media_content_error("OUT_OF_MEMORY(0x%08x)", MEDIA_CONTENT_ERROR_OUT_OF_MEMORY);
				return MEDIA_CONTENT_ERROR_OUT_OF_MEMORY;
			}
		}
		*dst = (media_tag_h)_dst;
		ret = MEDIA_CONTENT_ERROR_NONE;
	}
	else
	{
		media_content_error("INVALID_PARAMETER(0x%08x)", MEDIA_CONTENT_ERROR_INVALID_PARAMETER);
		ret = MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
	}

	return ret;
}

int media_tag_get_tag_id(media_tag_h tag, int *tag_id)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	media_tag_s *_tag = (media_tag_s*)tag;

	if((_tag != NULL) && (tag_id != NULL))
	{
		*tag_id = _tag->tag_id;
		ret = MEDIA_CONTENT_ERROR_NONE;
	}
	else
	{
		media_content_error("INVALID_PARAMETER(0x%08x)", MEDIA_CONTENT_ERROR_INVALID_PARAMETER);
		ret = MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
	}

	return ret;
}

int media_tag_get_name(media_tag_h tag, char **name)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	media_tag_s *_tag = (media_tag_s*)tag;
	if(_tag)
	{
		if(STRING_VALID(_tag->name))
		{
			*name = strdup(_tag->name);
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

int media_tag_get_tag_from_db(int tag_id, media_tag_h *tag)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	sqlite3_stmt *stmt = NULL;
	char select_query[DEFAULT_QUERY_SIZE];

	if(tag_id <= 0)
	{
		media_content_error("INVALID_PARAMETER(0x%08x)", MEDIA_CONTENT_ERROR_INVALID_PARAMETER);
		return MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
	}

	memset(select_query, 0x00, sizeof(select_query));

	snprintf(select_query, sizeof(select_query), SELECT_TAG_FROM_TAG, tag_id);

	ret = _content_query_prepare(&stmt, select_query, NULL, NULL);
	media_content_retv_if(ret != MEDIA_CONTENT_ERROR_NONE, ret);

	media_tag_s *_tag = NULL;

	while(sqlite3_step(stmt) == SQLITE_ROW)
	{
		if(_tag)
			media_tag_destroy((media_tag_h)_tag);

		_tag = (media_tag_s*)calloc(1, sizeof(media_tag_s));
		if(_tag == NULL)
		{
			SQLITE3_FINALIZE(stmt);
			media_content_error("OUT_OF_MEMORY(0x%08x)", MEDIA_CONTENT_ERROR_OUT_OF_MEMORY);
			return MEDIA_CONTENT_ERROR_OUT_OF_MEMORY;
		}

		_tag->tag_id = (int)sqlite3_column_int(stmt, 0);

		if(STRING_VALID((const char *)sqlite3_column_text(stmt, 1)))
		{
			_tag->name = strdup((const char *)sqlite3_column_text(stmt, 1));
		}

		*tag = (media_tag_h)_tag;
	}

	SQLITE3_FINALIZE(stmt);

	return ret;
}

int media_tag_add_media(media_tag_h tag, const char *media_id)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	media_tag_s *_tag = (media_tag_s*)tag;

	if((_tag != NULL) && STRING_VALID(media_id))
	{
		media_tag_item_s *_item = (media_tag_item_s*)calloc(1, sizeof(media_tag_item_s));
		media_content_retvm_if(_item == NULL, MEDIA_CONTENT_ERROR_OUT_OF_MEMORY, "OUT_OF_MEMORY");

		_item->media_id = strdup(media_id);
		_item->function = MEDIA_TAG_ADD;

		if(_item->media_id == NULL)
		{
			SAFE_FREE(_item);
			media_content_error("OUT_OF_MEMORY(0x%08x)", MEDIA_CONTENT_ERROR_OUT_OF_MEMORY);
			return MEDIA_CONTENT_ERROR_OUT_OF_MEMORY;
		}

		__media_tag_item_add(_tag, _item);
	}
	else
	{
		media_content_error("INVALID_PARAMETER(0x%08x)", MEDIA_CONTENT_ERROR_INVALID_PARAMETER);
		ret = MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
	}

	return ret;
}

int media_tag_remove_media(media_tag_h tag, const char *media_id)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	media_tag_s *_tag = (media_tag_s*)tag;

	if(_tag != NULL && STRING_VALID(media_id))
	{
		media_tag_item_s *_item = (media_tag_item_s*)calloc(1, sizeof(media_tag_item_s));
		media_content_retvm_if(_item == NULL, MEDIA_CONTENT_ERROR_OUT_OF_MEMORY, "OUT_OF_MEMORY");

		_item->media_id = strdup(media_id);
		_item->function = MEDIA_TAG_REMOVE;

		if(_item->media_id == NULL)
		{
			SAFE_FREE(_item);
			media_content_error("OUT_OF_MEMORY(0x%08x)", MEDIA_CONTENT_ERROR_OUT_OF_MEMORY);
			return MEDIA_CONTENT_ERROR_OUT_OF_MEMORY;
		}

		__media_tag_item_add(_tag, _item);
	}
	else
	{
		media_content_error("INVALID_PARAMETER(0x%08x)", MEDIA_CONTENT_ERROR_INVALID_PARAMETER);
		ret = MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
	}

	return ret;
}

int media_tag_set_name(media_tag_h tag, char *tag_name)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	media_tag_s *_tag = (media_tag_s*)tag;

	if(_tag != NULL && STRING_VALID(tag_name))
	{
		SAFE_FREE(_tag->name);

		media_tag_item_s *_item = (media_tag_item_s*)calloc(1, sizeof(media_tag_item_s));
		media_content_retvm_if(_item == NULL, MEDIA_CONTENT_ERROR_OUT_OF_MEMORY, "OUT_OF_MEMORY");

		_item->tag_name = strdup(tag_name);
		_item->function = MEDIA_TAG_UPDATE_TAG_NAME;

		if(_item->tag_name == NULL)
		{
			SAFE_FREE(_item);
			media_content_error("OUT_OF_MEMORY(0x%08x)", MEDIA_CONTENT_ERROR_OUT_OF_MEMORY);
			return MEDIA_CONTENT_ERROR_OUT_OF_MEMORY;
		}

		_tag->name = strdup(tag_name);
		if(_tag->name == NULL)
		{
			SAFE_FREE(_item->tag_name);
			SAFE_FREE(_item);
			media_content_error("OUT_OF_MEMORY(0x%08x)", MEDIA_CONTENT_ERROR_OUT_OF_MEMORY);
			return MEDIA_CONTENT_ERROR_OUT_OF_MEMORY;
		}

		__media_tag_item_add(_tag, _item);
	}
	else
	{
		media_content_error("INVALID_PARAMETER(0x%08x)", MEDIA_CONTENT_ERROR_INVALID_PARAMETER);
		ret = MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
	}

	return ret;
}

int media_tag_update_to_db(media_tag_h tag)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	media_tag_s *_tag = (media_tag_s*)tag;
	int idx = 0;
	int length = 0;
	media_tag_item_s *_tag_item = NULL;

	if(_tag == NULL)
	{
		media_content_error("INVALID_PARAMETER(0x%08x)", MEDIA_CONTENT_ERROR_INVALID_PARAMETER);
		return MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
	}

	if(_tag->item_list != NULL) {
		length = g_list_length(_tag->item_list);
	} else {
		media_content_debug("operation list length is 0");
		return MEDIA_CONTENT_ERROR_NONE;
	}

	for (idx = 0; idx < length; idx++) {
		_tag_item = (media_tag_item_s*)g_list_nth_data(_tag->item_list, idx);
		if(_tag_item != NULL) {
			switch(_tag_item->function) {
				case MEDIA_TAG_ADD:
				{
					ret = __media_tag_insert_item_to_tag(_tag->tag_id, _tag_item->media_id);
				}
				break;

				case MEDIA_TAG_REMOVE:
				{
					ret = __media_tag_remove_item_from_tag(_tag->tag_id, _tag_item->media_id);
				}
				break;

				case MEDIA_TAG_UPDATE_TAG_NAME:
				{
					ret = __media_tag_update_tag_name(_tag->tag_id, _tag_item->tag_name);
				}
				break;

				default:
				break;
			}
		}
	}

	__media_tag_item_release(_tag);

	return ret;
}
