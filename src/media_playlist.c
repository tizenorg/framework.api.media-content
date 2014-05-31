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


#include <media_filter.h>
#include <media_info.h>
#include <media_info_private.h>
#include <media_playlist.h>


static __thread GList *g_playlist_item_list = NULL;

static void __media_playlist_item_add(media_playlist_item_s *item_s);
static void __media_playlist_item_release(void);
static int __media_playlist_insert_playlist_record(const char *playlist_name, int *playlist_id);
static int __media_playlist_insert_item_to_playlist(int playlist_id, const char *media_id);
static int __media_playlist_remove_item_from_playlist(int playlist_id, int playlist_member_id);
static int __media_playlist_update_playlist_name(int playlist_id, const char *playlist_name);
static int __media_playlist_update_thumbnail_path(int playlist_id, const char *path);
static int __media_playlist_update_play_order(int playlist_id, int playlist_member_id, int play_order);

static void __media_playlist_item_add(media_playlist_item_s *item_s)
{
	g_playlist_item_list = g_list_append(g_playlist_item_list, item_s);
}

static void __media_playlist_item_release(void)
{
	int idx = 0;
	int list_cnt = 0;
	media_playlist_item_s *item = NULL;

	list_cnt = g_list_length(g_playlist_item_list);

	media_content_debug("list_cnt : [%d]", list_cnt);

	for(idx = 0; idx < list_cnt; idx++)
	{
		item = (media_playlist_item_s*)g_list_nth_data(g_playlist_item_list, idx);
		if(item != NULL)
		{
			SAFE_FREE(item->media_id);
			SAFE_FREE(item->playlist_name);
			SAFE_FREE(item->thumbnail_path);
			SAFE_FREE(item);
		}
	}

	g_list_free(g_playlist_item_list);
	g_playlist_item_list = NULL;

}

static int __media_playlist_insert_playlist_record(const char *playlist_name, int *playlist_id)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	char *query_str = NULL;
	char *select_query = NULL;
	sqlite3_stmt *stmt = NULL;

	query_str = sqlite3_mprintf(INSERT_PLAYLIST_TO_PLAYLIST, playlist_name);

	ret = _content_query_sql(query_str);
	sqlite3_free(query_str);
	media_content_retv_if(ret != MEDIA_CONTENT_ERROR_NONE, ret);

	select_query = sqlite3_mprintf(SELECT_PLAYLIST_ID_FROM_PLAYLIST, playlist_name);

	ret = _content_query_prepare(&stmt, select_query, NULL, NULL);
	sqlite3_free(select_query);
	media_content_retv_if(ret != MEDIA_CONTENT_ERROR_NONE, ret);

	while(sqlite3_step(stmt) == SQLITE_ROW)
	{
		*playlist_id = (int)sqlite3_column_int(stmt,0);
	}

	SQLITE3_FINALIZE(stmt);

	return MEDIA_CONTENT_ERROR_NONE;
}

static int __media_playlist_insert_item_to_playlist(int playlist_id, const char *media_id)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	sqlite3_stmt *stmt = NULL;
	char select_query[DEFAULT_QUERY_SIZE];
	char *query_str = NULL;
	int play_order = 0;

	memset(select_query, 0x00, sizeof(select_query));

	snprintf(select_query, sizeof(select_query), SELECT_MAX_PLAY_ORDER_FROM_PLAYLIST_VIEW, playlist_id);

	/* get the max play_order */
	ret = _content_query_prepare(&stmt, select_query, NULL, NULL);
	media_content_retv_if(ret != MEDIA_CONTENT_ERROR_NONE, ret);

	while(sqlite3_step(stmt) == SQLITE_ROW)
	{
		play_order = (int)sqlite3_column_int(stmt, 0);
	}

	SQLITE3_FINALIZE(stmt);

	++play_order;

	query_str = sqlite3_mprintf("INSERT INTO %q (playlist_id, media_uuid, play_order) values (%d, '%q', %d)",
			DB_TABLE_PLAYLIST_MAP, playlist_id, media_id, play_order);
	ret = _content_query_sql(query_str);
	sqlite3_free(query_str);

	return ret;
}

static int __media_playlist_remove_item_from_playlist(int playlist_id, int playlist_member_id)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	char *query_str = NULL;

	query_str = sqlite3_mprintf(REMOVE_PLAYLIST_ITEM_FROM_PLAYLIST_MAP, playlist_id, playlist_member_id);

	ret = _content_query_sql(query_str);
	sqlite3_free(query_str);

	return ret;
}

static int __media_playlist_update_playlist_name(int playlist_id, const char *playlist_name)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	char *query_str = NULL;

	query_str = sqlite3_mprintf(UPDATE_PLAYLIST_NAME_FROM_PLAYLIST, playlist_name, playlist_id);

	ret = _content_query_sql(query_str);
	sqlite3_free(query_str);

	return ret;
}

static int __media_playlist_update_thumbnail_path(int playlist_id, const char *path)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	char *query_str = NULL;

	query_str = sqlite3_mprintf(UPDATE_PLAYLIST_THUMBNAIL_FROM_PLAYLIST, path, playlist_id);

	ret = _content_query_sql(query_str);
	sqlite3_free(query_str);

	return ret;
}

static int __media_playlist_update_play_order(int playlist_id, int playlist_member_id, int play_order)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	char *query_str = NULL;

	query_str = sqlite3_mprintf(UPDATE_PLAYLIST_ORDER_FROM_PLAYLIST_MAP, play_order, playlist_id, playlist_member_id);

	ret = _content_query_sql(query_str);
	sqlite3_free(query_str);

	return ret;
}

int media_playlist_insert_to_db(const char *name, media_playlist_h *playlist)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	int playlist_id = 0;

	if(!STRING_VALID(name))
	{
		media_content_error("INVALID_PARAMETER(0x%08x)", MEDIA_CONTENT_ERROR_INVALID_PARAMETER);
		return MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
	}

	media_playlist_s *_playlist = (media_playlist_s*)calloc(1, sizeof(media_playlist_s));

	if(_playlist == NULL)
	{
		media_content_error("OUT_OF_MEMORY(0x%08x)", MEDIA_CONTENT_ERROR_OUT_OF_MEMORY);
		return MEDIA_CONTENT_ERROR_OUT_OF_MEMORY;
	}

	ret = __media_playlist_insert_playlist_record(name, &playlist_id);

	if(ret != MEDIA_CONTENT_ERROR_NONE)
	{
		SAFE_FREE(_playlist);
		return ret;
	}

	_playlist->playlist_id = playlist_id;
	_playlist->name = strdup(name);

	if(_playlist->name == NULL)
	{
		SAFE_FREE(_playlist);
		media_content_error("OUT_OF_MEMORY(0x%08x)", MEDIA_CONTENT_ERROR_OUT_OF_MEMORY);
		return MEDIA_CONTENT_ERROR_OUT_OF_MEMORY;
	}

	*playlist = (media_playlist_h)_playlist;

	return ret;
}

int media_playlist_delete_from_db(int playlist_id)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	char *query_str = NULL;

	if(playlist_id < 0)
	{
		media_content_error("INVALID_PARAMETER(0x%08x)", MEDIA_CONTENT_ERROR_INVALID_PARAMETER);
		return MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
	}

	query_str = sqlite3_mprintf(DELETE_PLAYLIST_FROM_PLAYLIST, playlist_id);

	ret = _content_query_sql(query_str);

	sqlite3_free(query_str);

	return ret;
}

int media_playlist_get_playlist_count_from_db(filter_h filter, int *playlist_count)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;

	if(playlist_count != NULL)
	{
		ret = _media_db_get_group_count(filter, MEDIA_GROUP_PLAYLIST, playlist_count);
	}
	else
	{
		media_content_error("INVALID_PARAMETER(0x%08x)", MEDIA_CONTENT_ERROR_INVALID_PARAMETER);
		ret = MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
	}

	return ret;
}

int media_playlist_foreach_playlist_from_db(filter_h filter, media_playlist_cb callback, void *user_data)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;

	if(callback == NULL)
	{
		media_content_error("INVALID_PARAMETER(0x%08x)", MEDIA_CONTENT_ERROR_INVALID_PARAMETER);
		return MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
	}

	ret = _media_db_get_playlist(filter, callback, user_data);

	return ret;
}

int media_playlist_get_media_count_from_db(int playlist_id, filter_h filter, int *media_count)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;

	if((playlist_id > 0) && (media_count != NULL))
	{
		ret = _media_db_get_group_item_count_by_id(playlist_id, filter, MEDIA_GROUP_PLAYLIST, media_count);
	}
	else
	{
		media_content_error("INVALID_PARAMETER(0x%08x)", MEDIA_CONTENT_ERROR_INVALID_PARAMETER);
		ret = MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
	}

	return ret;
}

int media_playlist_foreach_media_from_db(int playlist_id, filter_h filter, playlist_member_cb callback, void *user_data)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;

	if((playlist_id > 0) && (callback != NULL))
	{
		ret = _media_db_get_playlist_item(playlist_id, filter, callback, user_data);
	}
	else
	{
		media_content_error("INVALID_PARAMETER(0x%08x)", MEDIA_CONTENT_ERROR_INVALID_PARAMETER);
		ret = MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
	}

	return ret;
}

int media_playlist_destroy(media_playlist_h playlist)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	media_playlist_s *_playlist = (media_playlist_s*)playlist;

	if(_playlist)
	{
		SAFE_FREE(_playlist->name);
		SAFE_FREE(_playlist->thumbnail_path);
		SAFE_FREE(_playlist);

		ret = MEDIA_CONTENT_ERROR_NONE;
	}
	else
	{
		media_content_error("INVALID_PARAMETER(0x%08x)", MEDIA_CONTENT_ERROR_INVALID_PARAMETER);
		ret = MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
	}

	return ret;
}

int media_playlist_clone(media_playlist_h *dst, media_playlist_h src)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	media_playlist_s *_src = (media_playlist_s*)src;

	if(_src != NULL)
	{
		media_playlist_s *_dst = (media_playlist_s*)calloc(1, sizeof(media_playlist_s));
		if(_dst == NULL)
		{
			media_content_error("OUT_OF_MEMORY(0x%08x)", MEDIA_CONTENT_ERROR_OUT_OF_MEMORY);
			return MEDIA_CONTENT_ERROR_OUT_OF_MEMORY;
		}

		_dst->playlist_id = _src->playlist_id;

		if(STRING_VALID(_src->name))
		{
			_dst->name = strdup(_src->name);
			if(_dst->name == NULL)
			{
				media_playlist_destroy((media_playlist_h)_dst);
				media_content_error("OUT_OF_MEMORY(0x%08x)", MEDIA_CONTENT_ERROR_OUT_OF_MEMORY);
				return MEDIA_CONTENT_ERROR_OUT_OF_MEMORY;
			}
		}

		*dst = (media_playlist_h)_dst;

		ret = MEDIA_CONTENT_ERROR_NONE;
	}
	else
	{
		media_content_error("INVALID_PARAMETER(0x%08x)", MEDIA_CONTENT_ERROR_INVALID_PARAMETER);
		ret = MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
	}

	return ret;
}

int media_playlist_get_playlist_from_db(int playlist_id, media_playlist_h *playlist)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	sqlite3_stmt *stmt = NULL;
	char select_query[DEFAULT_QUERY_SIZE];

	if(playlist_id > 0)
	{
		memset(select_query, 0x00, sizeof(select_query));

		snprintf(select_query, sizeof(select_query), SELECT_PLAYLIST_FROM_PLAYLIST, playlist_id);

		ret = _content_query_prepare(&stmt, select_query, NULL, NULL);
		media_content_retv_if(ret != MEDIA_CONTENT_ERROR_NONE, ret);

		while(sqlite3_step(stmt) == SQLITE_ROW)
		{
			media_playlist_s *_playlist = (media_playlist_s*)calloc(1, sizeof(media_playlist_s));

			_playlist->playlist_id = (int)sqlite3_column_int(stmt, 0);
			if(STRING_VALID((const char *)sqlite3_column_text(stmt, 1)))
				_playlist->name = strdup((const char *)sqlite3_column_text(stmt, 1));
			if(STRING_VALID((const char *)sqlite3_column_text(stmt, 2)))
				_playlist->thumbnail_path = strdup((const char *)sqlite3_column_text(stmt, 2));

			*playlist = (media_playlist_h)_playlist;
		}

		SQLITE3_FINALIZE(stmt);
	}
	else
	{
		media_content_error("INVALID_PARAMETER(0x%08x)", MEDIA_CONTENT_ERROR_INVALID_PARAMETER);
		ret = MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
	}

	return ret;
}

int media_playlist_get_playlist_id(media_playlist_h playlist, int *playlist_id)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;

	media_playlist_s *_playlist = (media_playlist_s*)playlist;

	if((_playlist != NULL) && (playlist_id != NULL))
	{
		*playlist_id = _playlist->playlist_id;
	}
	else
	{
		media_content_error("INVALID_PARAMETER(0x%08x)", MEDIA_CONTENT_ERROR_INVALID_PARAMETER);
		ret = MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
	}

	return ret;
}

int media_playlist_get_name(media_playlist_h playlist, char **name)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	media_playlist_s *_playlist = (media_playlist_s*)playlist;
	if(_playlist)
	{
		if(STRING_VALID(_playlist->name))
		{
			*name = strdup(_playlist->name);
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

int media_playlist_get_thumbnail_path(media_playlist_h playlist, char **path)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	media_playlist_s *_playlist = (media_playlist_s*)playlist;
	if(_playlist)
	{
		if(STRING_VALID(_playlist->thumbnail_path))
		{
			*path = strdup(_playlist->thumbnail_path);
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

int media_playlist_get_play_order(media_playlist_h playlist, int playlist_member_id, int *play_order)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	media_playlist_s *_playlist = (media_playlist_s*)playlist;
	int playlist_id = 0;
	sqlite3_stmt *stmt = NULL;
	char select_query[DEFAULT_QUERY_SIZE];

	if((_playlist == NULL) || (playlist_member_id < 0) || (play_order == NULL))
	{
		media_content_error("INVALID_PARAMETER(0x%08x)", MEDIA_CONTENT_ERROR_INVALID_PARAMETER);
		return MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
	}

	playlist_id = _playlist->playlist_id;

	memset(select_query, 0x00, sizeof(select_query));

	snprintf(select_query, sizeof(select_query), SELECT_PLAY_ORDER_FROM_PLAYLIST_VIEW, playlist_id, playlist_member_id);

	ret = _content_query_prepare(&stmt, select_query, NULL, NULL);
	media_content_retv_if(ret != MEDIA_CONTENT_ERROR_NONE, ret);

	while(sqlite3_step(stmt) == SQLITE_ROW)
	{
		*play_order = (int)sqlite3_column_int(stmt, 0);
	}

	SQLITE3_FINALIZE(stmt);

	return ret;
}

int media_playlist_set_name(media_playlist_h playlist, const char *playlist_name)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	media_playlist_s *_playlist = (media_playlist_s*)playlist;

	if(_playlist != NULL && STRING_VALID(playlist_name))
	{
		SAFE_FREE(_playlist->name);

		media_playlist_item_s *item = (media_playlist_item_s*)calloc(1, sizeof(media_playlist_item_s));

		item->playlist_name = strdup(playlist_name);
		item->function = MEDIA_PLAYLIST_UPDATE_PLAYLIST_NAME;
		if(item->playlist_name == NULL)
		{
			SAFE_FREE(item);
			media_content_error("OUT_OF_MEMORY(0x%08x)", MEDIA_CONTENT_ERROR_OUT_OF_MEMORY);
			return MEDIA_CONTENT_ERROR_OUT_OF_MEMORY;
		}

		_playlist->name = strdup(playlist_name);
		if(_playlist->name == NULL)
		{
			SAFE_FREE(item->playlist_name);
			SAFE_FREE(item);
			media_content_error("OUT_OF_MEMORY(0x%08x)", MEDIA_CONTENT_ERROR_OUT_OF_MEMORY);
			return MEDIA_CONTENT_ERROR_OUT_OF_MEMORY;
		}

		__media_playlist_item_add(item);
	}
	else
	{
		media_content_error("INVALID_PARAMETER(0x%08x)", MEDIA_CONTENT_ERROR_INVALID_PARAMETER);
		ret = MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
	}

	return ret;
}

int media_playlist_set_thumbnail_path(media_playlist_h playlist, const char *path)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	media_playlist_s *_playlist = (media_playlist_s*)playlist;

	if(_playlist != NULL && STRING_VALID(path))
	{
		SAFE_FREE(_playlist->thumbnail_path);

		media_playlist_item_s *item = (media_playlist_item_s*)calloc(1, sizeof(media_playlist_item_s));

		item->thumbnail_path = strdup(path);
		item->function = MEDIA_PLAYLIST_UPDATE_THUMBNAIL_PATH;
		if(item->thumbnail_path == NULL)
		{
			SAFE_FREE(item);
			media_content_error("OUT_OF_MEMORY(0x%08x)", MEDIA_CONTENT_ERROR_OUT_OF_MEMORY);
			return MEDIA_CONTENT_ERROR_OUT_OF_MEMORY;
		}

		_playlist->thumbnail_path = strdup(path);
		if(_playlist->thumbnail_path == NULL)
		{
			SAFE_FREE(item->thumbnail_path);
			SAFE_FREE(item);
			media_content_error("OUT_OF_MEMORY(0x%08x)", MEDIA_CONTENT_ERROR_OUT_OF_MEMORY);
			return MEDIA_CONTENT_ERROR_OUT_OF_MEMORY;
		}

		__media_playlist_item_add(item);
	}
	else
	{
		media_content_error("INVALID_PARAMETER(0x%08x)", MEDIA_CONTENT_ERROR_INVALID_PARAMETER);
		ret = MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
	}

	return ret;
}

int media_playlist_set_play_order(media_playlist_h playlist, int playlist_member_id, int play_order)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	media_playlist_s *_playlist = (media_playlist_s*)playlist;

	if((_playlist != NULL) && (playlist_member_id > 0) && (play_order >= 0))
	{
		media_playlist_item_s *item = (media_playlist_item_s*)calloc(1, sizeof(media_playlist_item_s));

		item->playlist_member_id = playlist_member_id;
		item->function = MEDIA_PLAYLIST_UPDATE_PLAY_ORDER;
		item->play_order = play_order;

		__media_playlist_item_add(item);
	}
	else
	{
		media_content_error("INVALID_PARAMETER(0x%08x)", MEDIA_CONTENT_ERROR_INVALID_PARAMETER);
		ret = MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
	}

	return ret;
}

int media_playlist_add_media(media_playlist_h playlist, const char *media_id)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	media_playlist_s *_playlist = (media_playlist_s*)playlist;

	if(_playlist != NULL && STRING_VALID(media_id))
	{
		media_playlist_item_s *item = (media_playlist_item_s*)calloc(1, sizeof(media_playlist_item_s));

		item->media_id = strdup(media_id);
		item->function = MEDIA_PLAYLIST_ADD;

		if(item->media_id == NULL)
		{
			SAFE_FREE(item);
			media_content_error("OUT_OF_MEMORY(0x%08x)", MEDIA_CONTENT_ERROR_OUT_OF_MEMORY);
			return MEDIA_CONTENT_ERROR_OUT_OF_MEMORY;
		}

		__media_playlist_item_add(item);
	}
	else
	{
		media_content_error("INVALID_PARAMETER(0x%08x)", MEDIA_CONTENT_ERROR_INVALID_PARAMETER);
		ret = MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
	}

	return ret;
}


int media_playlist_remove_media(media_playlist_h playlist, int playlist_member_id)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	media_playlist_s *_playlist = (media_playlist_s*)playlist;

	if((_playlist != NULL) && (playlist_member_id > 0))
	{
		media_playlist_item_s *item = (media_playlist_item_s*)calloc(1, sizeof(media_playlist_item_s));

		item->playlist_member_id = playlist_member_id;
		item->function = MEDIA_PLAYLIST_REMOVE;

		__media_playlist_item_add(item);
	}
	else
	{
		media_content_error("INVALID_PARAMETER(0x%08x)", MEDIA_CONTENT_ERROR_INVALID_PARAMETER);
		ret = MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
	}

	return ret;
}

int media_playlist_update_to_db(media_playlist_h playlist)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	media_playlist_s *_playlist = (media_playlist_s*)playlist;
	int idx = 0;
	int length = 0;
	media_playlist_item_s *_playlist_item = NULL;

	if(_playlist == NULL)
	{
		media_content_error("INVALID_PARAMETER(0x%08x)", MEDIA_CONTENT_ERROR_INVALID_PARAMETER);
		return MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
	}

	length = g_list_length(g_playlist_item_list);

	for (idx = 0; idx < length; idx++) {
		_playlist_item = (media_playlist_item_s*)g_list_nth_data(g_playlist_item_list, idx);
		if(_playlist_item != NULL) {
			switch(_playlist_item->function) {
				case MEDIA_PLAYLIST_ADD:
				{
					ret = __media_playlist_insert_item_to_playlist(_playlist->playlist_id, _playlist_item->media_id);
				}
				break;

				case MEDIA_PLAYLIST_REMOVE:
				{
					ret = __media_playlist_remove_item_from_playlist(_playlist->playlist_id, _playlist_item->playlist_member_id);
				}
				break;

				case MEDIA_PLAYLIST_UPDATE_PLAYLIST_NAME:
				{
					ret = __media_playlist_update_playlist_name(_playlist->playlist_id, _playlist_item->playlist_name);
				}
				break;

				case MEDIA_PLAYLIST_UPDATE_THUMBNAIL_PATH:
				{
					ret = __media_playlist_update_thumbnail_path(_playlist->playlist_id, _playlist_item->thumbnail_path);
				}
				break;

				case MEDIA_PLAYLIST_UPDATE_PLAY_ORDER:
				{
					ret = __media_playlist_update_play_order(_playlist->playlist_id, _playlist_item->playlist_member_id, _playlist_item->play_order);
				}
				break;
			}
		}
	}

	__media_playlist_item_release();

	return ret;
}
