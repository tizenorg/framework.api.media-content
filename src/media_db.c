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

static char * __media_db_get_group_name(media_group_e group);
static int __media_db_make_query(filter_h filter, attribute_h attr, char *select_query, int select_query_size, char **condition_query, char **option_query);

static char * __media_db_get_group_name(media_group_e group)
{
	switch(group)
	{
		case MEDIA_CONTENT_GROUP_DISPLAY_NAME:
			return DB_FIELD_MEDIA_DISPLAY_NAME;
		case MEDIA_CONTENT_GROUP_TYPE:
			return DB_FIELD_MEDIA_TYPE;
		case MEDIA_CONTENT_GROUP_MIME_TYPE:
			return DB_FIELD_MEDIA_MIME_TYPE;
		case MEDIA_CONTENT_GROUP_SIZE:
			return DB_FIELD_MEDIA_SIZE;
		case MEDIA_CONTENT_GROUP_ADDED_TIME:
			return DB_FIELD_MEDIA_ADDED_TIME;
		case MEDIA_CONTENT_GROUP_MODIFIED_TIME:
			return DB_FIELD_MEDIA_MODIFIED_TIME;
		case MEDIA_CONTENT_GROUP_TITLE:
			return DB_FIELD_MEDIA_TITLE;
		case MEDIA_CONTENT_GROUP_ARTIST:
			return DB_FIELD_MEDIA_ARTIST;
		case MEDIA_CONTENT_GROUP_ALBUM_ARTIST:
			return DB_FIELD_MEDIA_ALBUM_ARTIST;
		case MEDIA_CONTENT_GROUP_GENRE:
			return DB_FIELD_MEDIA_GENRE;
		case MEDIA_CONTENT_GROUP_COMPOSER:
			return DB_FIELD_MEDIA_COMPOSER;
		case MEDIA_CONTENT_GROUP_YEAR:
			return DB_FIELD_MEDIA_YEAR;
		case MEDIA_CONTENT_GROUP_RECORDED_DATE:
			return DB_FIELD_MEDIA_RECORDED_DATE;
		case MEDIA_CONTENT_GROUP_COPYRIGHT:
			return DB_FIELD_MEDIA_COPYRIGHT;
		case MEDIA_CONTENT_GROUP_TRACK_NUM:
			return DB_FIELD_MEDIA_TRACK_NUM;
		case MEDIA_CONTENT_GROUP_DESCRIPTION:
			return DB_FIELD_MEDIA_DESCRIPTION;
		case MEDIA_CONTENT_GROUP_LONGITUDE:
			return DB_FIELD_MEDIA_LONGITUDE;
		case MEDIA_CONTENT_GROUP_LATITUDE:
			return DB_FIELD_MEDIA_LATITUDE;
		case MEDIA_CONTENT_GROUP_ALTITUDE:
			return DB_FIELD_MEDIA_ALTITUDE;
		case MEDIA_CONTENT_GROUP_BURST_IMAGE:
			return DB_FIELD_MEDIA_BURST_ID;
		case MEDIA_CONTENT_GROUP_RATING:
			return DB_FIELD_MEDIA_RATING;
		case MEDIA_CONTENT_GROUP_AUTHOR:
			return DB_FIELD_MEDIA_AUTHOR;
		case MEDIA_CONTENT_GROUP_PROVIDER:
			return DB_FIELD_MEDIA_PROVIDER;
		case MEDIA_CONTENT_GROUP_CONTENT_NAME:
			return DB_FIELD_MEDIA_CONTENT_NAME;
		case MEDIA_CONTENT_GROUP_CATEGORY:
			return DB_FIELD_MEDIA_CATEGORY;
		case MEDIA_CONTENT_GROUP_LOCATION_TAG:
			return DB_FIELD_MEDIA_LOCATION_TAG;
		case MEDIA_CONTENT_GROUP_AGE_RATING:
			return DB_FIELD_MEDIA_AGE_RATING;
		case MEDIA_CONTENT_GROUP_KEYWORD:
			return DB_FIELD_MEDIA_KEYWORD;
		case MEDIA_CONTENT_GROUP_WEATHER:
			return DB_FIELD_MEDIA_WEATHER;
		default:
			return NULL;
	}

	return NULL;
}

static int __media_db_make_query(filter_h filter, attribute_h attr, char *select_query, int select_query_size, char **condition_query, char **option_query)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	filter_s *_filter = NULL;

	if(filter != NULL)
	{
		_filter = (filter_s*)filter;

		if(STRING_VALID(_filter->condition))
		{
			/*bracket should be added to condition. If application use "OR" condition, F/W restriction condition like "validity=1" is disregared
			ex) select path from media where validity=1 and media_type=3 or media_type=1;*/
			char bracket_added_condition[MAX_QUERY_SIZE] = {0, };
			memset(bracket_added_condition, 0x00, sizeof(bracket_added_condition));

			SAFE_STRLCAT(bracket_added_condition, QUERY_KEYWORD_OPEN_BRACKET, MAX_QUERY_SIZE);
			SAFE_STRLCAT(bracket_added_condition, _filter->condition, MAX_QUERY_SIZE);
			SAFE_STRLCAT(bracket_added_condition, QUERY_KEYWORD_BRACKET, MAX_QUERY_SIZE);
			{
			ret = _media_filter_attribute_generate(attr, bracket_added_condition, _filter->condition_collate_type, condition_query);
			media_content_retv_if(ret != MEDIA_CONTENT_ERROR_NONE, ret);
			}
		}

		ret = _media_filter_attribute_option_generate(attr, filter, option_query);
		if(ret != MEDIA_CONTENT_ERROR_NONE)
		{
			SAFE_FREE(*condition_query);
			return ret;
		}

		if(STRING_VALID(*condition_query))
		{
			SAFE_STRLCAT(select_query, QUERY_KEYWORD_SPACE, select_query_size);
			SAFE_STRLCAT(select_query, QUERY_KEYWORD_AND, select_query_size);
		}
	}

	return ret;
}

int _media_db_get_group_count(filter_h filter, group_list_e group_type, int *group_count)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	sqlite3_stmt *stmt = NULL;
	char select_query[MAX_QUERY_SIZE] = {0, };
	char *condition_query = NULL;
	char *option_query = NULL;
	attribute_h attr = NULL;

	attr = _content_get_attirbute_handle();
	memset(select_query, 0x00, sizeof(select_query));

	switch(group_type) {
		case MEDIA_GROUP_NONE:
		case MEDIA_GROUP_TAG_BY_MEDIA_ID:
		case MEDIA_GROUP_BOOKMARK_BY_MEDIA_ID:
			{
				media_content_error("INVALID_PARAMETER(0x%08x)", MEDIA_CONTENT_ERROR_INVALID_PARAMETER);
				return MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
			}
			break;
		case MEDIA_GROUP_ALBUM:
			attr = _content_get_alias_attirbute_handle();
			if(!SAFE_STRLCPY(select_query, SELECT_ALBUM_COUNT, sizeof(select_query)))
			{
				media_content_error("INVALID_PARAMETER(0x%08x)", MEDIA_CONTENT_ERROR_INVALID_PARAMETER);
				return MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
			}

			break;
		case MEDIA_GROUP_FOLDER:
			attr = _content_get_alias_attirbute_handle();
			if(!SAFE_STRLCPY(select_query, SELECT_FOLDER_COUNT, sizeof(select_query)))
			{
				media_content_error("INVALID_PARAMETER(0x%08x)", MEDIA_CONTENT_ERROR_INVALID_PARAMETER);
				return MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
			}
			break;
		case MEDIA_GROUP_PLAYLIST:
			if(!SAFE_STRLCPY(select_query, SELECT_PLAYLIST_COUNT, sizeof(select_query)))
			{
				media_content_error("INVALID_PARAMETER(0x%08x)", MEDIA_CONTENT_ERROR_INVALID_PARAMETER);
				return MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
			}
			break;
		case MEDIA_GROUP_TAG:
			if(!SAFE_STRLCPY(select_query, SELECT_TAG_COUNT, sizeof(select_query)))
			{
				media_content_error("INVALID_PARAMETER(0x%08x)", MEDIA_CONTENT_ERROR_INVALID_PARAMETER);
				return MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
			}
			break;
		case MEDIA_GROUP_BOOKMARK:
			attr = _content_get_alias_attirbute_handle();
			if(!SAFE_STRLCPY(select_query, SELECT_BOOKMARK_COUNT, sizeof(select_query)))
			{
				media_content_error("INVALID_PARAMETER(0x%08x)", MEDIA_CONTENT_ERROR_INVALID_PARAMETER);
				return MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
			}
			break;
	}

	ret = __media_db_make_query(filter, attr, select_query, sizeof(select_query), &condition_query, &option_query);
	media_content_retv_if(ret != MEDIA_CONTENT_ERROR_NONE, ret);

	ret = _content_query_prepare(&stmt, select_query, condition_query, option_query);
	SAFE_FREE(condition_query);
	SAFE_FREE(option_query);
	media_content_retv_if(ret != MEDIA_CONTENT_ERROR_NONE, ret);

	while(sqlite3_step(stmt) == SQLITE_ROW)
	{
		*group_count = (int)sqlite3_column_int(stmt, 0);
	}

	SQLITE3_FINALIZE(stmt);

	return ret;
}

int _media_db_get_media_group_count(media_group_e group, filter_h filter, int *group_count)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	sqlite3_stmt *stmt = NULL;
	char select_query[MAX_QUERY_SIZE] = {0, };
	char *condition_query = NULL;
	char *option_query = NULL;
	attribute_h attr = NULL;

	attr = _content_get_attirbute_handle();
	memset(select_query, 0x00, sizeof(select_query));

	snprintf(select_query, sizeof(select_query), SELECT_MEDIA_GROUP_COUNT, __media_db_get_group_name(group));

	ret = __media_db_make_query(filter, attr, select_query, sizeof(select_query), &condition_query, &option_query);
	media_content_retv_if(ret != MEDIA_CONTENT_ERROR_NONE, ret);

	SAFE_STRLCAT(select_query, QUERY_KEYWORD_SPACE, sizeof(select_query));
	if(STRING_VALID(condition_query))
		SAFE_STRLCAT(select_query, condition_query, sizeof(select_query));
	if(STRING_VALID(option_query))
		SAFE_STRLCAT(select_query, option_query, sizeof(select_query));
	SAFE_STRLCAT(select_query, QUERY_KEYWORD_BRACKET, sizeof(select_query));

	SAFE_FREE(condition_query);
	SAFE_FREE(option_query);

	ret = _content_query_prepare(&stmt, select_query, condition_query, option_query);
	SAFE_FREE(condition_query);
	SAFE_FREE(option_query);
	media_content_retv_if(ret != MEDIA_CONTENT_ERROR_NONE, ret);

	while(sqlite3_step(stmt) == SQLITE_ROW)
	{
		*group_count = (int)sqlite3_column_int(stmt, 0);
	}

	SQLITE3_FINALIZE(stmt);

	return ret;
}

int _media_db_get_media_group(media_group_e group, filter_h filter, media_group_cb callback, void *user_data)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	char select_query[MAX_QUERY_SIZE] = {0, };
	char *condition_query = NULL;
	char *option_query = NULL;
	char *name = NULL;
	sqlite3_stmt *stmt = NULL;
	attribute_h attr = NULL;

	attr = _content_get_attirbute_handle();
	memset(select_query, 0x00, sizeof(select_query));

	snprintf(select_query, sizeof(select_query), SELECT_MEDIA_GROUP_LIST, __media_db_get_group_name(group));

	ret = __media_db_make_query(filter, attr, select_query, sizeof(select_query), &condition_query, &option_query);
	media_content_retv_if(ret != MEDIA_CONTENT_ERROR_NONE, ret);

	ret = _content_query_prepare(&stmt, select_query, condition_query, option_query);
	SAFE_FREE(condition_query);
	SAFE_FREE(option_query);
	media_content_retv_if(ret != MEDIA_CONTENT_ERROR_NONE, ret);

	while(sqlite3_step(stmt) == SQLITE_ROW)
	{
		if(STRING_VALID((const char *)sqlite3_column_text(stmt, 0)))
		{
			name = strdup((const char *)sqlite3_column_text(stmt, 0));
		}

		if(callback(name, user_data) == false)
		{
			SAFE_FREE(name);
			break;
		}

		SAFE_FREE(name);
	}

	SQLITE3_FINALIZE(stmt);

	return ret;
}

int _media_db_get_album(filter_h filter, media_album_cb callback, void *user_data)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	char select_query[MAX_QUERY_SIZE] = {0, };
	char *condition_query = NULL;
	char *option_query = NULL;
	sqlite3_stmt *stmt = NULL;
	attribute_h attr = NULL;

	attr = _content_get_alias_attirbute_handle();
	memset(select_query, 0x00, sizeof(select_query));

	if(!SAFE_STRLCPY(select_query, SELECT_ALBUM_LIST, sizeof(select_query)))
	{
		media_content_error("INVALID_PARAMETER(0x%08x)", MEDIA_CONTENT_ERROR_INVALID_PARAMETER);
		return MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
	}

	ret = __media_db_make_query(filter, attr, select_query, sizeof(select_query), &condition_query, &option_query);
	media_content_retv_if(ret != MEDIA_CONTENT_ERROR_NONE, ret);

	ret = _content_query_prepare(&stmt, select_query, condition_query, option_query);
	SAFE_FREE(condition_query);
	SAFE_FREE(option_query);
	media_content_retv_if(ret != MEDIA_CONTENT_ERROR_NONE, ret);

	while(sqlite3_step(stmt) == SQLITE_ROW)
	{
		media_album_s *album = (media_album_s*)calloc(1, sizeof(media_album_s));

		if(album == NULL)
		{
			media_content_error("OUT_OF_MEMORY(0x%08x)", MEDIA_CONTENT_ERROR_OUT_OF_MEMORY);
			SQLITE3_FINALIZE(stmt);
			return MEDIA_CONTENT_ERROR_OUT_OF_MEMORY;
		}

		album->album_id = (int)sqlite3_column_int(stmt, 0);

		if(STRING_VALID((const char *)sqlite3_column_text(stmt, 1)))
			album->name = strdup((const char *)sqlite3_column_text(stmt, 1));

		if(STRING_VALID((const char *)sqlite3_column_text(stmt, 2)))
			album->artist = strdup((const char *)sqlite3_column_text(stmt, 2));

		if(STRING_VALID((const char *)sqlite3_column_text(stmt, 3)))
			album->album_art_path = strdup((const char *)sqlite3_column_text(stmt, 3));

		if(callback((media_album_h)album, user_data) == false)
		{
			media_album_destroy((media_album_h)album);
			break;
		}

		media_album_destroy((media_album_h)album);
	}

	SQLITE3_FINALIZE(stmt);

	return ret;
}

int _media_db_get_folder(filter_h filter, media_folder_cb callback, void *user_data)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	char select_query[MAX_QUERY_SIZE] = {0, };
	char *condition_query = NULL;
	char *option_query = NULL;
	sqlite3_stmt *stmt = NULL;
	attribute_h attr = NULL;

	attr = _content_get_alias_attirbute_handle();
	memset(select_query, 0x00, sizeof(select_query));

	if(!SAFE_STRLCPY(select_query, SELECT_FOLDER_LIST, sizeof(select_query)))
	{
		media_content_error("INVALID_PARAMETER(0x%08x)", MEDIA_CONTENT_ERROR_INVALID_PARAMETER);
		return MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
	}

	ret = __media_db_make_query(filter, attr, select_query, sizeof(select_query), &condition_query, &option_query);
	media_content_retv_if(ret != MEDIA_CONTENT_ERROR_NONE, ret);

	ret = _content_query_prepare(&stmt, select_query, condition_query, option_query);

	/*this is temporary log to fix bug*/
	if (condition_query != NULL) media_content_error("condition_query %s", condition_query);
	if (option_query != NULL) media_content_error("condition_query %s", option_query);

	SAFE_FREE(condition_query);
	SAFE_FREE(option_query);
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

		if(STRING_VALID((const char *)sqlite3_column_text(stmt, 0)))
			_folder->folder_id = strdup((const char *)sqlite3_column_text(stmt, 0));

		if(STRING_VALID((const char *)sqlite3_column_text(stmt, 1)))
			_folder->path = strdup((const char *)sqlite3_column_text(stmt, 1));

		if(STRING_VALID((const char *)sqlite3_column_text(stmt, 2)))
			_folder->name = strdup((const char *)sqlite3_column_text(stmt, 2));

		_folder->storage_type = (int)sqlite3_column_int(stmt,3);

		_folder->modified_time = (int)sqlite3_column_int(stmt,4);

		if(callback((media_folder_h)_folder, user_data) == false)
		{
			media_folder_destroy((media_folder_h) _folder);
			break;
		}

		media_folder_destroy((media_folder_h) _folder);
	}

	SQLITE3_FINALIZE(stmt);

	return ret;
}

int _media_db_get_playlist(filter_h filter, media_playlist_cb callback, void *user_data)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	char select_query[MAX_QUERY_SIZE] = {0, };
	char *condition_query = NULL;
	char *option_query = NULL;
	sqlite3_stmt *stmt = NULL;
	attribute_h attr = NULL;

	attr = _content_get_attirbute_handle();
	memset(select_query, 0x00, sizeof(select_query));

	if(!SAFE_STRLCPY(select_query, SELECT_PLAYLIST_LIST, sizeof(select_query)))
	{
		media_content_error("INVALID_PARAMETER(0x%08x)", MEDIA_CONTENT_ERROR_INVALID_PARAMETER);
		return MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
	}

	ret = __media_db_make_query(filter, attr, select_query, sizeof(select_query), &condition_query, &option_query);
	media_content_retv_if(ret != MEDIA_CONTENT_ERROR_NONE, ret);

	ret = _content_query_prepare(&stmt, select_query, condition_query, option_query);
	SAFE_FREE(condition_query);
	SAFE_FREE(option_query);
	media_content_retv_if(ret != MEDIA_CONTENT_ERROR_NONE, ret);

	while(sqlite3_step(stmt) == SQLITE_ROW)
	{
		media_playlist_s *_playlist = (media_playlist_s*)calloc(1, sizeof(media_playlist_s));

		if(_playlist == NULL)
		{
			media_content_error("OUT_OF_MEMORY(0x%08x)", MEDIA_CONTENT_ERROR_OUT_OF_MEMORY);
			SQLITE3_FINALIZE(stmt);
			return MEDIA_CONTENT_ERROR_OUT_OF_MEMORY;
		}

		_playlist->playlist_id = (int)sqlite3_column_int(stmt, 0);

		if(STRING_VALID((const char *)sqlite3_column_text(stmt, 1)))
			_playlist->name = strdup((const char *)sqlite3_column_text(stmt, 1));

		if(STRING_VALID((const char *)sqlite3_column_text(stmt, 2)))
			_playlist->thumbnail_path = strdup((const char *)sqlite3_column_text(stmt, 2));

		if(callback((media_playlist_h)_playlist, user_data) == false)
		{
			media_playlist_destroy((media_playlist_h)_playlist);
			break;
		}
		media_playlist_destroy((media_playlist_h)_playlist);
	}

	SQLITE3_FINALIZE(stmt);

	return ret;
}

int _media_db_get_playlist_item(int playlist_id, filter_h filter, playlist_member_cb callback, void *user_data)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	char select_query[MAX_QUERY_SIZE] = {0, };
	char *condition_query = NULL;
	char *option_query = NULL;
	sqlite3_stmt *stmt = NULL;
	attribute_h attr = NULL;

	attr = _content_get_attirbute_handle();
	memset(select_query, 0x00, sizeof(select_query));

	//snprintf(select_query, sizeof(select_query), SELECT_PLAYLIST_ITEM_ID_FROM_PLAYLIST_VIEW, playlist_id);
	snprintf(select_query, sizeof(select_query), SELECT_PLAYLIST_ITEM_ALL_FROM_PLAYLIST_VIEW, playlist_id);

	ret = __media_db_make_query(filter, attr, select_query, sizeof(select_query), &condition_query, &option_query);
	media_content_retv_if(ret != MEDIA_CONTENT_ERROR_NONE, ret);

	ret = _content_query_prepare(&stmt, select_query, condition_query, option_query);
	SAFE_FREE(condition_query);
	SAFE_FREE(option_query);
	media_content_retv_if(ret != MEDIA_CONTENT_ERROR_NONE, ret);

	while(sqlite3_step(stmt) == SQLITE_ROW)
	{
		int playlist_member_id = 0;
		playlist_member_id = (int)sqlite3_column_int(stmt, 50);	//50 is Just for Commercial!

		media_info_s *_media = (media_info_s*)calloc(1, sizeof(media_info_s));

		if(_media == NULL)
		{
			media_content_error("OUT_OF_MEMORY(0x%08x)", MEDIA_CONTENT_ERROR_OUT_OF_MEMORY);
			SQLITE3_FINALIZE(stmt);
			return MEDIA_CONTENT_ERROR_OUT_OF_MEMORY;
		}

		_media_info_item_get_detail(stmt, (media_info_h)_media);

		if(callback(playlist_member_id, (media_info_h)_media, user_data) == false)
		{
			media_info_destroy((media_info_h)_media);
			break;
		}
		media_info_destroy((media_info_h)_media);

	}

	SQLITE3_FINALIZE(stmt);

	return ret;
}

int _media_db_get_tag(const char *media_id, filter_h filter, media_tag_cb callback, void *user_data)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	char select_query[MAX_QUERY_SIZE] = {0, };
	char *condition_query = NULL;
	char *option_query = NULL;
	sqlite3_stmt *stmt = NULL;
	attribute_h attr = NULL;

	attr = _content_get_attirbute_handle();
	memset(select_query, 0x00, sizeof(select_query));

	if(!STRING_VALID(media_id))
	{
		if(!SAFE_STRLCPY(select_query, SELECT_TAG_LIST, sizeof(select_query)))
		{
			media_content_error("INVALID_PARAMETER(0x%08x)", MEDIA_CONTENT_ERROR_INVALID_PARAMETER);
			return MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
		}
	}
	else
	{
		snprintf(select_query, sizeof(select_query), SELECT_TAG_LIST_BY_MEDIA_ID, media_id);
	}

	ret = __media_db_make_query(filter, attr, select_query, sizeof(select_query), &condition_query, &option_query);
	media_content_retv_if(ret != MEDIA_CONTENT_ERROR_NONE, ret);

	SAFE_STRLCAT(select_query, QUERY_KEYWORD_SPACE, sizeof(select_query));

	ret = _content_query_prepare(&stmt, select_query, condition_query, option_query);
	SAFE_FREE(condition_query);
	SAFE_FREE(option_query);
	media_content_retv_if(ret != MEDIA_CONTENT_ERROR_NONE, ret);

	while(sqlite3_step(stmt) == SQLITE_ROW)
	{
		media_tag_s *_tag = (media_tag_s*)calloc(1, sizeof(media_tag_s));

		if(_tag == NULL)
		{
			media_content_error("OUT_OF_MEMORY(0x%08x)", MEDIA_CONTENT_ERROR_OUT_OF_MEMORY);
			SQLITE3_FINALIZE(stmt);
			return MEDIA_CONTENT_ERROR_OUT_OF_MEMORY;
		}

		_tag->tag_id = (int)sqlite3_column_int(stmt, 0);

		if(STRING_VALID((const char *)sqlite3_column_text(stmt, 1)))
			_tag->name = strdup((const char *)sqlite3_column_text(stmt, 1));

		if(callback((media_tag_h)_tag, user_data) == false)
		{
			media_tag_destroy((media_tag_h)_tag);
			break;
		}
		media_tag_destroy((media_tag_h)_tag);
	}

	SQLITE3_FINALIZE(stmt);

	return ret;
}

int _media_db_get_bookmark(const char *media_id, filter_h filter, media_bookmark_cb callback, void *user_data)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	char select_query[MAX_QUERY_SIZE] = {0, };
	char *condition_query = NULL;
	char *option_query = NULL;
	sqlite3_stmt *stmt = NULL;
	attribute_h attr = NULL;

	attr = _content_get_attirbute_handle();

	memset(select_query, 0x00, sizeof(select_query));

	snprintf(select_query, sizeof(select_query), SELECT_BOOKMARK_LIST_BY_MEDIA_ID, media_id);

	ret = __media_db_make_query(filter, attr, select_query, sizeof(select_query), &condition_query, &option_query);
	media_content_retv_if(ret != MEDIA_CONTENT_ERROR_NONE, ret);

	ret = _content_query_prepare(&stmt, select_query, condition_query, option_query);
	SAFE_FREE(condition_query);
	SAFE_FREE(option_query);
	media_content_retv_if(ret != MEDIA_CONTENT_ERROR_NONE, ret);

	while(sqlite3_step(stmt) == SQLITE_ROW)
	{
		media_bookmark_s *bookmark = (media_bookmark_s*)calloc(1, sizeof(media_bookmark_s));

		if(bookmark == NULL)
		{
			media_content_error("OUT_OF_MEMORY(0x%08x)", MEDIA_CONTENT_ERROR_OUT_OF_MEMORY);
			SQLITE3_FINALIZE(stmt);
			return MEDIA_CONTENT_ERROR_OUT_OF_MEMORY;
		}

		bookmark->bookmark_id = (int)sqlite3_column_int(stmt, 0);

		if(STRING_VALID((const char *)sqlite3_column_text(stmt, 1)))
			bookmark->media_id = strdup((const char *)sqlite3_column_text(stmt, 1));

		bookmark->marked_time = (int)sqlite3_column_int(stmt, 2);

		if(STRING_VALID((const char *)sqlite3_column_text(stmt, 3)))
			bookmark->thumbnail_path = strdup((const char *)sqlite3_column_text(stmt, 3));

		if(callback((media_bookmark_h)bookmark, user_data) == false)
		{
			media_bookmark_destroy((media_bookmark_h)bookmark);
			break;
		}

		media_bookmark_destroy((media_bookmark_h)bookmark);
	}

	SQLITE3_FINALIZE(stmt);

	return ret;

}

int _media_db_get_group_item_count_by_id(int group_id, filter_h filter, group_list_e group_type, int *item_count)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	sqlite3_stmt *stmt = NULL;
	char select_query[MAX_QUERY_SIZE] = {0, };
	char *condition_query = NULL;
	char *option_query = NULL;
	attribute_h attr = NULL;

	attr = _content_get_attirbute_handle();
	memset(select_query, 0x00, sizeof(select_query));

	if(group_type == MEDIA_GROUP_ALBUM)
	{
		snprintf(select_query, sizeof(select_query), SELECT_MEDIA_COUNT_FROM_ALBUM, group_id);
	}
	else if(group_type == MEDIA_GROUP_PLAYLIST)
	{
		snprintf(select_query, sizeof(select_query), SELECT_MEDIA_COUNT_FROM_PLAYLIST, group_id);
	}
	else if(group_type == MEDIA_GROUP_TAG)
	{
		snprintf(select_query, sizeof(select_query), SELECT_MEDIA_COUNT_FROM_TAG, group_id);
	}
	else
	{
		media_content_error("INVALID_PARAMETER(0x%08x)", MEDIA_CONTENT_ERROR_INVALID_PARAMETER);
		return MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
	}

	ret = __media_db_make_query(filter, attr, select_query, sizeof(select_query), &condition_query, &option_query);
	media_content_retv_if(ret != MEDIA_CONTENT_ERROR_NONE, ret);

	ret = _content_query_prepare(&stmt, select_query, condition_query, option_query);
	SAFE_FREE(condition_query);
	SAFE_FREE(option_query);
	media_content_retv_if(ret != MEDIA_CONTENT_ERROR_NONE, ret);

	while(sqlite3_step(stmt) == SQLITE_ROW)
	{
		*item_count = (int)sqlite3_column_int(stmt, 0);
	}

	SQLITE3_FINALIZE(stmt);

	return ret;
}

int _media_db_get_group_item_count(const char *group_name, filter_h filter, group_list_e group_type, int *item_count)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	sqlite3_stmt *stmt = NULL;
	filter_s *_filter = NULL;
	char select_query[MAX_QUERY_SIZE] = {0, };
	char *tmp_query = NULL;
	char *condition_query = NULL;
	char *option_query = NULL;
	bool is_simple = FALSE;
	attribute_h attr = NULL;

	attr = _content_get_attirbute_handle();
	memset(select_query, 0x00, sizeof(select_query));

	if(group_type == MEDIA_GROUP_NONE)
	{
		/* There are 2 ways to get count for media table for performance
			If user wants to set offset and count, use SQL SELECT_MEDIA_COUNT_FROM_MEDIA.
			If user wants to get count without setting count, SELECT_MEDIA_COUNT_FROM_MEDIA_SIMPLE */
		_filter = (filter_s*)filter;
		if (_filter && ((_filter->offset < 0) && (_filter->count < 0))) {

				SAFE_STRLCAT(select_query, SELECT_MEDIA_COUNT_FROM_MEDIA_SIMPLE, sizeof(select_query));
				is_simple = TRUE;
		} else {
			SAFE_STRLCAT(select_query, SELECT_MEDIA_COUNT_FROM_MEDIA, sizeof(select_query));
		}
	}
	else if(group_type == MEDIA_GROUP_FOLDER)
	{
		tmp_query = sqlite3_mprintf(SELECT_MEDIA_COUNT_FROM_FOLDER, group_name);
		SAFE_STRLCAT(select_query, tmp_query, sizeof(select_query));
	}
	else if(group_type == MEDIA_GROUP_TAG_BY_MEDIA_ID)
	{
		snprintf(select_query, sizeof(select_query), SELECT_TAG_COUNT_BY_MEDIA_ID, group_name);
	}
	else if(group_type == MEDIA_GROUP_BOOKMARK_BY_MEDIA_ID)
	{
		snprintf(select_query, sizeof(select_query), SELECT_BOOKMARK_COUNT_BY_MEDIA_ID, group_name);
	}
	else
	{
		media_content_error("INVALID_PARAMETER(0x%08x)", MEDIA_CONTENT_ERROR_INVALID_PARAMETER);
		return MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
	}

	ret = __media_db_make_query(filter, attr, select_query, sizeof(select_query), &condition_query, &option_query);
	if(ret != MEDIA_CONTENT_ERROR_NONE)
	{
		if(tmp_query != NULL)
			sqlite3_free(tmp_query);
		return ret;
	}

	if(group_type == MEDIA_GROUP_NONE)
	{
		SAFE_STRLCAT(select_query, QUERY_KEYWORD_SPACE, sizeof(select_query));
		if(STRING_VALID(condition_query))
			SAFE_STRLCAT(select_query, condition_query, sizeof(select_query));
		if(STRING_VALID(option_query))
			SAFE_STRLCAT(select_query, option_query, sizeof(select_query));

		if (!is_simple)
			SAFE_STRLCAT(select_query, QUERY_KEYWORD_BRACKET, sizeof(select_query));

		SAFE_FREE(condition_query);
		SAFE_FREE(option_query);
	}

	ret = _content_query_prepare(&stmt, select_query, condition_query, option_query);
	if(tmp_query != NULL)
		sqlite3_free(tmp_query);
	SAFE_FREE(condition_query);
	SAFE_FREE(option_query);
	media_content_retv_if(ret != MEDIA_CONTENT_ERROR_NONE, ret);

	while(sqlite3_step(stmt) == SQLITE_ROW)
	{
		*item_count = (int)sqlite3_column_int(stmt, 0);
	}

	SQLITE3_FINALIZE(stmt);

	return ret;
}

int _media_db_get_group_item_by_id(int group_id, filter_h filter, media_info_cb callback, void *user_data, group_list_e group_type)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	char select_query[MAX_QUERY_SIZE] = {0, };
	char *condition_query = NULL;
	char *option_query = NULL;
	sqlite3_stmt *stmt = NULL;
	attribute_h attr = NULL;

	attr = _content_get_attirbute_handle();
	memset(select_query, 0x00, sizeof(select_query));

	if(group_type == MEDIA_GROUP_ALBUM)
	{
		snprintf(select_query, sizeof(select_query), SELECT_MEDIA_FROM_ALBUM, group_id);
	}
	else if(group_type == MEDIA_GROUP_PLAYLIST)
	{
		snprintf(select_query, sizeof(select_query), SELECT_MEDIA_FROM_PLAYLIST, group_id);
	}
	else if(group_type == MEDIA_GROUP_TAG)
	{
		snprintf(select_query, sizeof(select_query), SELECT_MEDIA_FROM_TAG, group_id);
	}
	else
	{
		media_content_error("INVALID_PARAMETER(0x%08x)", MEDIA_CONTENT_ERROR_INVALID_PARAMETER);
		return MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
	}

	ret = __media_db_make_query(filter, attr, select_query, sizeof(select_query), &condition_query, &option_query);
	media_content_retv_if(ret != MEDIA_CONTENT_ERROR_NONE, ret);

	ret = _content_query_prepare(&stmt, select_query, condition_query, option_query);
	SAFE_FREE(condition_query);
	SAFE_FREE(option_query);
	media_content_retv_if(ret != MEDIA_CONTENT_ERROR_NONE, ret);

	while(sqlite3_step(stmt) == SQLITE_ROW)
	{
		media_info_s *item = (media_info_s*)calloc(1, sizeof(media_info_s));
		if(item == NULL)
		{
			media_content_error("OUT_OF_MEMORY(0x%08x)", MEDIA_CONTENT_ERROR_OUT_OF_MEMORY);
			SQLITE3_FINALIZE(stmt);
			return MEDIA_CONTENT_ERROR_OUT_OF_MEMORY;
		}

		_media_info_item_get_detail(stmt, (media_info_h)item);

		if(callback((media_info_h)item, user_data) == false)
		{
			media_info_destroy((media_info_h)item);
			break;
		}

		media_info_destroy((media_info_h)item);
	}

	SQLITE3_FINALIZE(stmt);

	return ret;
}

int _media_db_get_group_item(const char *group_name, filter_h filter, media_info_cb callback, void *user_data, group_list_e group_type)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	char select_query[MAX_QUERY_SIZE] = {0, };
	char *condition_query = NULL;
	char *option_query = NULL;
	sqlite3_stmt *stmt = NULL;
	attribute_h attr = NULL;

	attr = _content_get_attirbute_handle();
	memset(select_query, 0x00, sizeof(select_query));

	if(group_type == MEDIA_GROUP_NONE)
	{
		SAFE_STRLCAT(select_query, SELECT_MEDIA_ITEM, sizeof(select_query));
	}
	else if(group_type == MEDIA_GROUP_FOLDER)
	{
		snprintf(select_query, sizeof(select_query), SELECT_MEDIA_FROM_FOLDER, group_name);
	}
	else
	{
		media_content_error("INVALID_PARAMETER(0x%08x)", MEDIA_CONTENT_ERROR_INVALID_PARAMETER);
		return MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
	}

	ret = __media_db_make_query(filter, attr, select_query, sizeof(select_query), &condition_query, &option_query);
	media_content_retv_if(ret != MEDIA_CONTENT_ERROR_NONE, ret);

	ret = _content_query_prepare(&stmt, select_query, condition_query, option_query);
	SAFE_FREE(condition_query);
	SAFE_FREE(option_query);
	media_content_retv_if(ret != MEDIA_CONTENT_ERROR_NONE, ret);

	while(sqlite3_step(stmt) == SQLITE_ROW)
	{
		media_info_s *item = (media_info_s*)calloc(1, sizeof(media_info_s));
		if(item == NULL)
		{
			media_content_error("OUT_OF_MEMORY(0x%08x)", MEDIA_CONTENT_ERROR_OUT_OF_MEMORY);
			SQLITE3_FINALIZE(stmt);
			return MEDIA_CONTENT_ERROR_OUT_OF_MEMORY;
		}

		_media_info_item_get_detail(stmt, (media_info_h)item);

		if(callback((media_info_h)item, user_data) == false)
		{
			media_info_destroy((media_info_h)item);
			break;
		}

		media_info_destroy((media_info_h)item);
	}

	SQLITE3_FINALIZE(stmt);

	return ret;
}

int _media_db_get_media_group_item_count(const char *group_name, filter_h filter, media_group_e group, int *item_count)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	sqlite3_stmt *stmt = NULL;
	char select_query[MAX_QUERY_SIZE] = {0, };
	char *tmp_query = NULL;
	char *condition_query = NULL;
	char *option_query = NULL;
	attribute_h attr = NULL;

	attr = _content_get_attirbute_handle();
	memset(select_query, 0x00, sizeof(select_query));

	if(group_name != NULL)
	{
		tmp_query = sqlite3_mprintf(SELECT_MEDIA_COUNT_FROM_GROUP, __media_db_get_group_name(group), group_name);
		SAFE_STRLCAT(select_query, tmp_query, sizeof(select_query));
	}
	else
	{
		snprintf(select_query, sizeof(select_query), SELECT_MEDIA_COUNT_FROM_GROUP_NULL, __media_db_get_group_name(group));
	}

	ret = __media_db_make_query(filter, attr, select_query, sizeof(select_query), &condition_query, &option_query);
	if(ret != MEDIA_CONTENT_ERROR_NONE)
	{
		if(tmp_query != NULL)
			sqlite3_free(tmp_query);
		return ret;
	}

	SAFE_STRLCAT(select_query, QUERY_KEYWORD_SPACE, sizeof(select_query));
	if(STRING_VALID(condition_query))
		SAFE_STRLCAT(select_query, condition_query, sizeof(select_query));
	if(STRING_VALID(option_query))
		SAFE_STRLCAT(select_query, option_query, sizeof(select_query));
	SAFE_STRLCAT(select_query, QUERY_KEYWORD_BRACKET, sizeof(select_query));

	SAFE_FREE(condition_query);
	SAFE_FREE(option_query);

	ret = _content_query_prepare(&stmt, select_query, condition_query, option_query);
	if(tmp_query != NULL)
		sqlite3_free(tmp_query);
	SAFE_FREE(condition_query);
	SAFE_FREE(option_query);
	media_content_retv_if(ret != MEDIA_CONTENT_ERROR_NONE, ret);

	while(sqlite3_step(stmt) == SQLITE_ROW)
	{
		*item_count = (int)sqlite3_column_int(stmt, 0);
	}

	SQLITE3_FINALIZE(stmt);

	return ret;
}

int _media_db_get_media_group_item(const char *group_name, filter_h filter, media_group_e group, media_info_cb callback, void *user_data)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	char select_query[MAX_QUERY_SIZE] = {0, };
	char *tmp_query = NULL;
	char *condition_query = NULL;
	char *option_query = NULL;
	sqlite3_stmt *stmt = NULL;
	attribute_h attr = NULL;

	attr = _content_get_attirbute_handle();
	memset(select_query, 0x00, sizeof(select_query));

	if(group_name != NULL)
	{
		tmp_query = sqlite3_mprintf(SELECT_MEDIA_FROM_GROUP, __media_db_get_group_name(group), group_name);
		SAFE_STRLCAT(select_query, tmp_query, sizeof(select_query));
	}
	else
	{
		snprintf(select_query, sizeof(select_query), SELECT_MEDIA_FROM_GROUP_NULL, __media_db_get_group_name(group));
	}

	ret = __media_db_make_query(filter, attr, select_query, sizeof(select_query), &condition_query, &option_query);
	if(ret != MEDIA_CONTENT_ERROR_NONE)
	{
		if(tmp_query != NULL)
			sqlite3_free(tmp_query);
		return ret;
	}

	ret = _content_query_prepare(&stmt, select_query, condition_query, option_query);
	if(tmp_query != NULL)
		sqlite3_free(tmp_query);
	SAFE_FREE(condition_query);
	SAFE_FREE(option_query);
	media_content_retv_if(ret != MEDIA_CONTENT_ERROR_NONE, ret);

	while(sqlite3_step(stmt) == SQLITE_ROW)
	{
		media_info_s *item = (media_info_s*)calloc(1, sizeof(media_info_s));
		if(item == NULL)
		{
			media_content_error("OUT_OF_MEMORY(0x%08x)", MEDIA_CONTENT_ERROR_OUT_OF_MEMORY);
			SQLITE3_FINALIZE(stmt);
			return MEDIA_CONTENT_ERROR_OUT_OF_MEMORY;
		}

		_media_info_item_get_detail(stmt, (media_info_h)item);

		if(callback((media_info_h)item, user_data) == false)
		{
			media_info_destroy((media_info_h)item);
			break;
		}

		media_info_destroy((media_info_h)item);
	}

	SQLITE3_FINALIZE(stmt);

	return ret;
}
