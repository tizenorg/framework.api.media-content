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

#define PLAYLIST_ARRAY_SIZE				20
#define PLAYLIST_ARRAY_EXPAND			10
#define MAX_TMP_STR						2048

static void __media_playlist_item_add(media_playlist_s *playlist, media_playlist_item_s *item_s);
static void __media_playlist_item_release(media_playlist_s *playlist);
static int __media_playlist_insert_playlist_record(const char *playlist_name, int *playlist_id);
static int __media_playlist_insert_item_to_playlist(int playlist_id, const char *media_id);
static int __media_playlist_remove_item_from_playlist(int playlist_id, int playlist_member_id);
static int __media_playlist_update_playlist_name(int playlist_id, const char *playlist_name);
static int __media_playlist_update_thumbnail_path(int playlist_id, const char *path);
static int __media_playlist_update_play_order(int playlist_id, int playlist_member_id, int play_order);
static int __media_playlist_reset_file(const char* playlist_path);
static int __media_playlist_append_to_file(const char* playlist_path, const char* path);
static int __media_playlist_import_item_from_file(const char* playlist_path, char*** const item_list, int* item_count);
static int __media_playlist_destroy_import_item(char** item_list, int item_count);
static void __media_playlist_destroy_export_item(gpointer data);

static void __media_playlist_item_add(media_playlist_s *playlist, media_playlist_item_s *item_s)
{
	playlist->item_list = g_list_append(playlist->item_list, item_s);
}

static void __media_playlist_item_release(media_playlist_s *playlist)
{
	int idx = 0;
	int list_cnt = 0;
	media_playlist_item_s *item = NULL;

	list_cnt = g_list_length(playlist->item_list);

	media_content_debug("list_cnt : [%d]", list_cnt);

	for(idx = 0; idx < list_cnt; idx++)
	{
		item = (media_playlist_item_s*)g_list_nth_data(playlist->item_list, idx);
		if(item != NULL)
		{
			SAFE_FREE(item->media_id);
			SAFE_FREE(item->playlist_name);
			SAFE_FREE(item->thumbnail_path);
			SAFE_FREE(item);
		}
	}

	g_list_free(playlist->item_list);
	playlist->item_list = NULL;

}

static int __media_playlist_insert_playlist_record(const char *playlist_name, int *playlist_id)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	char *query_str = NULL;
	char *select_query = NULL;
	sqlite3_stmt *stmt = NULL;

	query_str = sqlite3_mprintf(INSERT_PLAYLIST_TO_PLAYLIST, playlist_name);

	ret = _content_query_sql(query_str);
	SQLITE3_SAFE_FREE(query_str);
	media_content_retv_if(ret != MEDIA_CONTENT_ERROR_NONE, ret);

	select_query = sqlite3_mprintf(SELECT_PLAYLIST_ID_FROM_PLAYLIST, playlist_name);

	ret = _content_query_prepare(&stmt, select_query, NULL, NULL);
	SQLITE3_SAFE_FREE(select_query);
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
	SQLITE3_SAFE_FREE(query_str);

	return ret;
}

static int __media_playlist_remove_item_from_playlist(int playlist_id, int playlist_member_id)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	char *query_str = NULL;

	query_str = sqlite3_mprintf(REMOVE_PLAYLIST_ITEM_FROM_PLAYLIST_MAP, playlist_id, playlist_member_id);

	ret = _content_query_sql(query_str);
	SQLITE3_SAFE_FREE(query_str);

	return ret;
}

static int __media_playlist_update_playlist_name(int playlist_id, const char *playlist_name)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	char *query_str = NULL;

	query_str = sqlite3_mprintf(UPDATE_PLAYLIST_NAME_FROM_PLAYLIST, playlist_name, playlist_id);

	ret = _content_query_sql(query_str);
	SQLITE3_SAFE_FREE(query_str);

	return ret;
}

static int __media_playlist_update_thumbnail_path(int playlist_id, const char *path)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	char *query_str = NULL;

	query_str = sqlite3_mprintf(UPDATE_PLAYLIST_THUMBNAIL_FROM_PLAYLIST, path, playlist_id);

	ret = _content_query_sql(query_str);
	SQLITE3_SAFE_FREE(query_str);

	return ret;
}

static int __media_playlist_update_play_order(int playlist_id, int playlist_member_id, int play_order)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	char *query_str = NULL;

	query_str = sqlite3_mprintf(UPDATE_PLAYLIST_ORDER_FROM_PLAYLIST_MAP, play_order, playlist_id, playlist_member_id);

	ret = _content_query_sql(query_str);
	SQLITE3_SAFE_FREE(query_str);

	return ret;
}

static bool __media_playlist_media_info_cb(media_info_h media, void *user_data)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	char **media_id = (char**)user_data;

	ret = media_info_get_media_id(media, media_id);
	media_content_retvm_if(ret != MEDIA_CONTENT_ERROR_NONE, FALSE, "media_info_get_media_id fail");

	return TRUE;
}

static bool __media_playlist_member_cb(int playlist_member_id, media_info_h media, void *user_data)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	GList **list = (GList**)user_data;
	char *path = NULL;

	ret = media_info_get_file_path(media, &path);
	media_content_retvm_if(ret != MEDIA_CONTENT_ERROR_NONE, FALSE, "media_info_get_file_path fail");

	*list = g_list_append(*list, path);

	return TRUE;
}

static int __media_playlist_reset_file(const char* playlist_path)
{
	FILE *fp = NULL;

	fp = fopen(playlist_path, "wb");
	media_content_retvm_if(fp == NULL, MEDIA_CONTENT_ERROR_INVALID_OPERATION, "fopen fail");

	fputs("", fp);	// remove previous playlist

	fclose(fp);

	return MEDIA_CONTENT_ERROR_NONE;
}

static int __media_playlist_append_to_file(const char* playlist_path, const char* path)
{
	FILE *fp = NULL;

	fp = fopen(playlist_path, "a");	// append only
	media_content_retvm_if(fp == NULL, MEDIA_CONTENT_ERROR_INVALID_OPERATION, "fopen fail");

	fputs(path, fp);

	fputs("\n", fp);

	fclose(fp);

	return MEDIA_CONTENT_ERROR_NONE;
}

static int __media_playlist_import_item_from_file(const char* playlist_path, char*** const item_list, int* item_count)
{
	int current_index = 0;						// Current record number
	int current_max_size = PLAYLIST_ARRAY_SIZE;	// Current max number of records in array
	int tmp_str_len = 0;						// Length of the string
	char *buf = NULL;
	char *tmp_buf = NULL;
	char *tmp_str = NULL;						// Next line from buffer, this string is used for parsing

	FILE *fp = NULL;
	long int file_size = 0;

	*item_list = NULL; *item_count = 0;

	fp = fopen(playlist_path, "rb");		// Open as binary for precise estimation of file length
	media_content_retvm_if(fp == NULL, MEDIA_CONTENT_ERROR_INVALID_OPERATION, "fopen fail");

	fseek(fp, 0, SEEK_END);					// Move to the end of file
	file_size = ftell(fp);				// Here we can find the size of file
	fseek(fp, 0 , SEEK_SET);					// Return to the beginning of file

	if(file_size == 0) {
		media_content_debug("file is empty.");
		fclose(fp);
		return MEDIA_CONTENT_ERROR_NONE;
	}
	// Allocate the memory and copy file content there
	if(file_size > 0)
		buf = malloc(file_size + 1);

	if(buf == NULL)
	{
		media_content_error("Out of Memory");
		fclose(fp);
		return MEDIA_CONTENT_ERROR_OUT_OF_MEMORY;
	}

	tmp_buf = buf;

	if(fread(buf, file_size, 1, fp) != 1) {
		fclose(fp);
		SAFE_FREE(buf);
		media_content_stderror("fread fail");
		return MEDIA_CONTENT_ERROR_INVALID_OPERATION;
	}
	buf[file_size] = 0;
	fclose(fp);

	// Preliminary memory allocation
	*item_list = calloc(current_max_size, sizeof(char*));
	tmp_str = malloc(MAX_TMP_STR);
	if (tmp_str == NULL || *item_list == NULL) {
		SAFE_FREE(*item_list);
		SAFE_FREE(buf);
		SAFE_FREE(tmp_str);
		media_content_error("Out of Memory");
		return MEDIA_CONTENT_ERROR_OUT_OF_MEMORY;
	}
	// Here we create format string for sscanf(...) that allows to get a line from buffer
	char format[25];
	snprintf(format, 25, "%%%d[^\n]", MAX_TMP_STR);

	// This cycle gets lines one by one from buffer till the end of buffer. Empty line ("\n") must be treated specifically
	while((sscanf(tmp_buf, format, tmp_str) == 1) || (*tmp_buf == '\n')) {
		if(*tmp_buf == '\n') {// Check if there is an empty line, skip '\n' symbol
			tmp_buf += 1;

			if(tmp_buf < (buf + file_size))
				continue;			// We are still in buffer
			else
				break;				// Empty line was in the end of buffer
		}

		tmp_str_len = strlen(tmp_str);		// Save the length of line

		if(tmp_str[0] != '#') {			// Check that the line is not a comment
			if(!(current_index < (current_max_size - 1))) {				// Check if we have completely filled record array
				// Expand array size and relocate the array (records will be copied into new one)
				current_max_size += PLAYLIST_ARRAY_EXPAND;
				char **tmp_ptr = calloc(current_max_size, sizeof(char*));
				if (tmp_ptr == NULL) {
					__media_playlist_destroy_import_item(*item_list, current_index);
					SAFE_FREE(buf);
					SAFE_FREE(tmp_str);
					media_content_error("Out of Memory");
					return MEDIA_CONTENT_ERROR_OUT_OF_MEMORY;
				}
				memmove(tmp_ptr, *item_list, sizeof(char*) * current_index);
				SAFE_FREE(*item_list);
				*item_list = tmp_ptr;
			}

			// Save new file path (current string in tmp_str)
			(*item_list)[current_index] = malloc(tmp_str_len + 1);
			if ((*item_list)[current_index] == NULL) {
				__media_playlist_destroy_import_item(*item_list, current_index);
				SAFE_FREE(buf);
				SAFE_FREE(tmp_str);
				media_content_error("Out of Memory");
				return MEDIA_CONTENT_ERROR_OUT_OF_MEMORY;
			}
			memmove((*item_list)[current_index], tmp_str, tmp_str_len + 1);

			// Increase the index of buffer
			current_index += 1;
		}

		tmp_buf += (tmp_str_len + 1);				// Move position in buffer after the string that was parsed
	}

	*item_count = current_index;						// Now we need to save the number of records in array

	SAFE_FREE(buf);
	SAFE_FREE(tmp_str);						// Free temporary variables

	return MEDIA_CONTENT_ERROR_NONE;
}

static int __media_playlist_destroy_import_item(char** item_list, int item_count)
{
	int i;

	for(i = 0; i < item_count; ++i) {
		SAFE_FREE(item_list[i]);
		item_list[i] = NULL;
	}

	if (item_list != NULL) {
		SAFE_FREE(item_list);
		item_list = NULL;
	}

	return MEDIA_CONTENT_ERROR_NONE;
}

static void __media_playlist_destroy_export_item(gpointer data)
{
	SAFE_FREE(data);
	data = NULL;
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
	media_content_retvm_if(_playlist == NULL, MEDIA_CONTENT_ERROR_OUT_OF_MEMORY, "OUT_OF_MEMORY");

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

	SQLITE3_SAFE_FREE(query_str);

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
		media_content_retvm_if(_dst == NULL, MEDIA_CONTENT_ERROR_OUT_OF_MEMORY, "OUT_OF_MEMORY");

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

		media_playlist_s *_playlist = NULL;

		while(sqlite3_step(stmt) == SQLITE_ROW)
		{
			if(_playlist)
				media_playlist_destroy((media_playlist_h)_playlist);

			_playlist = (media_playlist_s*)calloc(1, sizeof(media_playlist_s));
			if(_playlist == NULL)
			{
				media_content_error("OUT_OF_MEMORY(0x%08x)", MEDIA_CONTENT_ERROR_OUT_OF_MEMORY);
				SQLITE3_FINALIZE(stmt);
				return MEDIA_CONTENT_ERROR_OUT_OF_MEMORY;
			}

			_playlist->playlist_id = (int)sqlite3_column_int(stmt, 0);
			if(STRING_VALID((const char *)sqlite3_column_text(stmt, 1)))
				_playlist->name = strdup((const char *)sqlite3_column_text(stmt, 1));
			if(STRING_VALID((const char *)sqlite3_column_text(stmt, 3)))
				_playlist->thumbnail_path = strdup((const char *)sqlite3_column_text(stmt, 3));

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

int media_playlist_get_thumbnail_path(media_playlist_h playlist, char **path)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	media_playlist_s *_playlist = (media_playlist_s*)playlist;
	if(_playlist)
	{
		if(STRING_VALID(_playlist->thumbnail_path))
		{
			*path = strdup(_playlist->thumbnail_path);
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
		media_content_retvm_if(item == NULL, MEDIA_CONTENT_ERROR_OUT_OF_MEMORY, "OUT_OF_MEMORY");

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

		__media_playlist_item_add(_playlist, item);
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
		media_content_retvm_if(item == NULL, MEDIA_CONTENT_ERROR_OUT_OF_MEMORY, "OUT_OF_MEMORY");

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

		__media_playlist_item_add(_playlist, item);
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
		media_content_retvm_if(item == NULL, MEDIA_CONTENT_ERROR_OUT_OF_MEMORY, "OUT_OF_MEMORY");

		item->playlist_member_id = playlist_member_id;
		item->function = MEDIA_PLAYLIST_UPDATE_PLAY_ORDER;
		item->play_order = play_order;

		__media_playlist_item_add(_playlist, item);
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
		media_content_retvm_if(item == NULL, MEDIA_CONTENT_ERROR_OUT_OF_MEMORY, "OUT_OF_MEMORY");

		item->media_id = strdup(media_id);
		item->function = MEDIA_PLAYLIST_ADD;

		if(item->media_id == NULL)
		{
			SAFE_FREE(item);
			media_content_error("OUT_OF_MEMORY(0x%08x)", MEDIA_CONTENT_ERROR_OUT_OF_MEMORY);
			return MEDIA_CONTENT_ERROR_OUT_OF_MEMORY;
		}

		__media_playlist_item_add(_playlist, item);
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
		media_content_retvm_if(item == NULL, MEDIA_CONTENT_ERROR_OUT_OF_MEMORY, "OUT_OF_MEMORY");

		item->playlist_member_id = playlist_member_id;
		item->function = MEDIA_PLAYLIST_REMOVE;

		__media_playlist_item_add(_playlist, item);
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

	if(_playlist->item_list != NULL) {
		length = g_list_length(_playlist->item_list);
	} else {
		media_content_debug("operation list length is 0");
		return MEDIA_CONTENT_ERROR_NONE;
	}

	for (idx = 0; idx < length; idx++) {
		_playlist_item = (media_playlist_item_s*)g_list_nth_data(_playlist->item_list, idx);
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

				default :
				break;
			}
		}
	}

	__media_playlist_item_release(_playlist);

	return ret;
}

int media_playlist_import_from_file(const char *path, const char *playlist_name, media_playlist_h *playlist)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	char** import_item_list = NULL;
	int import_item_count = 0;
	int idx;

	media_content_retvm_if(!STRING_VALID(path), MEDIA_CONTENT_ERROR_INVALID_PARAMETER, "Invalid path");
	media_content_retvm_if(!STRING_VALID(playlist_name), MEDIA_CONTENT_ERROR_INVALID_PARAMETER, "Invalid playlist_name");

	ret = media_playlist_insert_to_db(playlist_name, playlist);
	media_content_retvm_if(ret != MEDIA_CONTENT_ERROR_NONE, ret, "media_playlist_insert_to_db fail");

	ret = __media_playlist_import_item_from_file(path, &import_item_list, &import_item_count);
	if(ret != MEDIA_CONTENT_ERROR_NONE)
	{
		__media_playlist_destroy_import_item(import_item_list, import_item_count);
		media_content_error("Fail to get playlist from file");
		return ret;
	}

	if (import_item_count == 0)
	{
		media_content_debug("The playlist from file is empty");
	}

	for (idx=0; idx < import_item_count; idx++)
	{
		filter_h filter = NULL;
		char *media_id = NULL;
		char *condition = NULL;

		ret = media_filter_create(&filter);
		if(ret != MEDIA_CONTENT_ERROR_NONE)
		{
			__media_playlist_destroy_import_item(import_item_list, import_item_count);
			media_filter_destroy(filter);
			media_content_error("error media_filter_create");
			return ret;
		}
		condition = sqlite3_mprintf("path = '%q'", import_item_list[idx]);
		ret = media_filter_set_condition(filter, condition, MEDIA_CONTENT_COLLATE_DEFAULT);
		if(ret != MEDIA_CONTENT_ERROR_NONE)
		{
			__media_playlist_destroy_import_item(import_item_list, import_item_count);
			media_filter_destroy(filter);
			SQLITE3_SAFE_FREE(condition);
			media_content_error("error media_filter_set_condition");
			return ret;
		}
		ret = _media_db_get_group_item(NULL, filter, __media_playlist_media_info_cb, &media_id, MEDIA_GROUP_NONE);
		if(ret != MEDIA_CONTENT_ERROR_NONE)
		{
			__media_playlist_destroy_import_item(import_item_list, import_item_count);
			media_filter_destroy(filter);
			SAFE_FREE(media_id);
			SQLITE3_SAFE_FREE(condition);
			media_content_error("error media_info_foreach_media_from_db");
			return ret;
		}
		ret = media_playlist_add_media((media_playlist_h)*playlist, media_id);
		if(ret != MEDIA_CONTENT_ERROR_NONE)
		{
			__media_playlist_destroy_import_item(import_item_list, import_item_count);
			media_filter_destroy(filter);
			SAFE_FREE(media_id);
			SQLITE3_SAFE_FREE(condition);
			media_content_error("error media_playlist_add_media");
			return ret;
		}
		media_filter_destroy(filter);
		SAFE_FREE(media_id);
		SQLITE3_SAFE_FREE(condition);
	}

	ret = media_playlist_update_to_db(*playlist);
	if(ret != MEDIA_CONTENT_ERROR_NONE)
	{
		__media_playlist_destroy_import_item(import_item_list, import_item_count);
		media_content_error("Fail to update playlist to db");
		return ret;
	}
	__media_playlist_destroy_import_item(import_item_list, import_item_count);

	return ret;
}

int media_playlist_export_to_file(media_playlist_h playlist, const char* path)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	media_playlist_s *_playlist = (media_playlist_s*)playlist;
	GList *item_list = NULL;
	unsigned int idx = 0;

	media_content_retvm_if(!STRING_VALID(path), MEDIA_CONTENT_ERROR_INVALID_PARAMETER, "Invalid path");
	media_content_retvm_if(_playlist == NULL, MEDIA_CONTENT_ERROR_INVALID_PARAMETER, "Invalid playlist");
	media_content_retvm_if(_playlist->playlist_id <= 0, MEDIA_CONTENT_ERROR_INVALID_PARAMETER, "Invalid playlist_id");

	ret = _media_db_get_playlist_item(_playlist->playlist_id, NULL, __media_playlist_member_cb, &item_list);
	media_content_retvm_if(ret != MEDIA_CONTENT_ERROR_NONE, ret, "_media_db_get_playlist_item fail");

	ret = __media_playlist_reset_file(path);
	if(ret != MEDIA_CONTENT_ERROR_NONE)
	{
		g_list_free_full(item_list, __media_playlist_destroy_export_item);
		media_content_error("Fail to init playlist file");
		return ret;
	}

	for (idx=0; idx < g_list_length(item_list); idx++)
	{
		char *item = g_list_nth_data(item_list, idx);
		ret = __media_playlist_append_to_file(path, item);
		if(ret != MEDIA_CONTENT_ERROR_NONE)
		{
			g_list_free_full(item_list, __media_playlist_destroy_export_item);
			media_content_error("Fail to export paths into file");
			return ret;
		}
	}

	g_list_free_full(item_list, __media_playlist_destroy_export_item);

	return ret;
}
