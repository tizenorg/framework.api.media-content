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
#include <media_group.h>


int media_album_get_album_count_from_db(filter_h filter, int *album_count)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;

	if(album_count != NULL)
	{
		ret = _media_db_get_group_count(filter, MEDIA_GROUP_ALBUM, album_count);
	}
	else
	{
		media_content_error("INVALID_PARAMETER(0x%08x)", MEDIA_CONTENT_ERROR_INVALID_PARAMETER);
		ret = MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
	}

	return ret;
}

int media_album_foreach_album_from_db(filter_h filter, media_album_cb callback, void *user_data)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;

	if(callback == NULL)
	{
		media_content_error("INVALID_PARAMETER(0x%08x)", MEDIA_CONTENT_ERROR_INVALID_PARAMETER);
		return MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
	}

	ret = _media_db_get_album(filter, callback, user_data);

	return ret;
}

int media_album_get_media_count_from_db(int album_id, filter_h filter, int *media_count)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;

	if((album_id > 0) && (media_count != NULL))
	{
		ret = _media_db_get_group_item_count_by_id(album_id, filter, MEDIA_GROUP_ALBUM, media_count);
	}
	else
	{
		media_content_error("INVALID_PARAMETER(0x%08x)", MEDIA_CONTENT_ERROR_INVALID_PARAMETER);
		ret = MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
	}

	return ret;
}

int media_album_foreach_media_from_db(int album_id, filter_h filter, media_info_cb callback, void *user_data)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;

	if((album_id > 0) && (callback != NULL))
	{
		ret = _media_db_get_group_item_by_id(album_id, filter, callback, user_data, MEDIA_GROUP_ALBUM);
	}
	else
	{
		media_content_error("INVALID_PARAMETER(0x%08x)", MEDIA_CONTENT_ERROR_INVALID_PARAMETER);
		return MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
	}

	return ret;
}

int media_album_get_album_from_db(int album_id, media_album_h *album)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	sqlite3_stmt *stmt = NULL;
	char select_query[DEFAULT_QUERY_SIZE];

	if(album_id < 0)
	{
		media_content_error("INVALID_PARAMETER(0x%08x)", MEDIA_CONTENT_ERROR_INVALID_PARAMETER);
		return MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
	}

	memset(select_query, 0x00, sizeof(select_query));

	snprintf(select_query, sizeof(select_query), SELECT_ALBUM_FROM_ALBUM, album_id);

	ret = _content_query_prepare(&stmt, select_query, NULL, NULL);
	media_content_retv_if(ret != MEDIA_CONTENT_ERROR_NONE, ret);

	while(sqlite3_step(stmt) == SQLITE_ROW)
	{
		media_album_s *_album = (media_album_s*)calloc(1, sizeof(media_album_s));
		if(_album == NULL)
		{
			media_content_error("OUT_OF_MEMORY(0x%08x)", MEDIA_CONTENT_ERROR_OUT_OF_MEMORY);
			SQLITE3_FINALIZE(stmt);
			return MEDIA_CONTENT_ERROR_OUT_OF_MEMORY;
		}

		_album->album_id = (int)sqlite3_column_int(stmt, 0);

		if(STRING_VALID((const char *)sqlite3_column_text(stmt, 1)))
			_album->name = strdup((const char *)sqlite3_column_text(stmt, 1));

		if(STRING_VALID((const char *)sqlite3_column_text(stmt, 2)))
			_album->artist = strdup((const char *)sqlite3_column_text(stmt, 2));

		*album = (media_album_h)_album;
	}

	SQLITE3_FINALIZE(stmt);

	return ret;
}

int media_album_destroy(media_album_h album)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	media_album_s *_album = (media_album_s*)album;

	if(_album)
	{
		SAFE_FREE(_album->name);
		SAFE_FREE(_album->artist);
		SAFE_FREE(_album->album_art_path);
		SAFE_FREE(_album);

		ret = MEDIA_CONTENT_ERROR_NONE;
	}
	else
	{
		media_content_error("INVALID_PARAMETER(0x%08x)", MEDIA_CONTENT_ERROR_INVALID_PARAMETER);
		ret = MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
	}

	return ret;
}

int media_album_clone(media_album_h *dst, media_album_h src)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	media_album_s *_src = (media_album_s*)src;

	if(_src != NULL)
	{
		media_album_s *_dst = (media_album_s*)calloc(1, sizeof(media_album_s));

		if(_dst == NULL)
		{
			media_content_error("OUT_OF_MEMORY(0x%08x)", MEDIA_CONTENT_ERROR_OUT_OF_MEMORY);
			return MEDIA_CONTENT_ERROR_OUT_OF_MEMORY;
		}

		_dst->album_id = _src->album_id;

		if(STRING_VALID(_src->name))
		{
			_dst->name = strdup(_src->name);
			if(_dst->name == NULL)
			{
				media_content_error("OUT_OF_MEMORY(0x%08x)", MEDIA_CONTENT_ERROR_OUT_OF_MEMORY);
				media_album_destroy((media_album_h)_dst);
				return MEDIA_CONTENT_ERROR_OUT_OF_MEMORY;
			}
		}

		if(STRING_VALID(_src->artist))
		{
			_dst->artist = strdup(_src->artist);
			if(_dst->artist == NULL)
			{
				media_content_error("OUT_OF_MEMORY(0x%08x)", MEDIA_CONTENT_ERROR_OUT_OF_MEMORY);
				media_album_destroy((media_album_h)_dst);
				return MEDIA_CONTENT_ERROR_OUT_OF_MEMORY;
			}
		}

		if(STRING_VALID(_src->album_art_path))
		{
			_dst->album_art_path = strdup(_src->album_art_path);
			if(_dst->album_art_path == NULL)
			{
				media_content_error("OUT_OF_MEMORY(0x%08x)", MEDIA_CONTENT_ERROR_OUT_OF_MEMORY);
				media_album_destroy((media_album_h)_dst);
				return MEDIA_CONTENT_ERROR_OUT_OF_MEMORY;
			}
		}

		*dst = (media_album_h)_dst;

		ret = MEDIA_CONTENT_ERROR_NONE;
	}
	else
	{
		media_content_error("INVALID_PARAMETER(0x%08x)", MEDIA_CONTENT_ERROR_INVALID_PARAMETER);
		ret = MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
	}

	return ret;
}

int media_album_get_album_id(media_album_h album, int *album_id)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	media_album_s *_album = (media_album_s*)album;

	if(_album && album_id)
	{
		*album_id = _album->album_id;
		ret = MEDIA_CONTENT_ERROR_NONE;
	}
	else
	{
		media_content_error("INVALID_PARAMETER(0x%08x)", MEDIA_CONTENT_ERROR_INVALID_PARAMETER);
		ret = MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
	}

	return ret;
}

int media_album_get_name(media_album_h album, char **name)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	media_album_s *_album = (media_album_s*)album;

	if(_album)
	{
		if(STRING_VALID(_album->name))
		{
			*name = strdup(_album->name);
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

int media_album_get_artist(media_album_h album, char **artist)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	media_album_s *_album = (media_album_s*)album;

	if(_album)
	{
		if(STRING_VALID(_album->artist))
		{
			*artist = strdup(_album->artist);
			if(*artist == NULL)
			{
				media_content_error("OUT_OF_MEMORY(0x%08x)", MEDIA_CONTENT_ERROR_OUT_OF_MEMORY);
				return MEDIA_CONTENT_ERROR_OUT_OF_MEMORY;
			}
		}
		else
		{
			*artist = NULL;
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

int media_album_get_album_art(media_album_h album, char **album_art)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	media_album_s *_album = (media_album_s*)album;

	if(_album)
	{
		if(STRING_VALID(_album->album_art_path))
		{
			*album_art = strdup(_album->album_art_path);
			if(*album_art == NULL)
			{
				media_content_error("OUT_OF_MEMORY(0x%08x)", MEDIA_CONTENT_ERROR_OUT_OF_MEMORY);
				return MEDIA_CONTENT_ERROR_OUT_OF_MEMORY;
			}
		}
		else
		{
			*album_art = NULL;
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

int media_group_get_group_count_from_db(filter_h filter, media_group_e group, int *group_count)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;

	if((group < MEDIA_CONTENT_GROUP_DISPLAY_NAME) || (group >= MEDIA_CONTENT_GROUP_MAX) || (group_count == NULL))
	{
		media_content_error("INVALID_PARAMETER(0x%08x)", MEDIA_CONTENT_ERROR_INVALID_PARAMETER);
		ret = MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
	}
	else
	{
		ret = _media_db_get_media_group_count(group, filter, group_count);
	}

	return ret;
}

int media_group_foreach_group_from_db(filter_h filter, media_group_e group, media_group_cb callback, void *user_data)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;

	if((callback == NULL) || (group < MEDIA_CONTENT_GROUP_DISPLAY_NAME) || (group >= MEDIA_CONTENT_GROUP_MAX))
	{
		media_content_error("INVALID_PARAMETER(0x%08x)", MEDIA_CONTENT_ERROR_INVALID_PARAMETER);
		ret = MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
	}
	else
	{
		ret = _media_db_get_media_group(group, filter, callback, user_data);
	}

	return ret;
}

int media_group_get_media_count_from_db(const char *group_name, media_group_e group, filter_h filter, int *media_count)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;

	if((media_count == NULL) || (group < MEDIA_CONTENT_GROUP_DISPLAY_NAME) || (group >= MEDIA_CONTENT_GROUP_MAX))
	{
		media_content_error("INVALID_PARAMETER(0x%08x)", MEDIA_CONTENT_ERROR_INVALID_PARAMETER);
		ret = MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
	}
	else
	{
		ret = _media_db_get_media_group_item_count(group_name, filter, group, media_count);
	}

	return ret;
}

int media_group_foreach_media_from_db(const char *group_name, media_group_e group, filter_h filter, media_info_cb callback, void *user_data)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;

	if((callback == NULL) || (group < MEDIA_CONTENT_GROUP_DISPLAY_NAME) || (group >= MEDIA_CONTENT_GROUP_MAX))
	{
		media_content_error("INVALID_PARAMETER(0x%08x)", MEDIA_CONTENT_ERROR_INVALID_PARAMETER);
		ret = MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
	}
	else
	{
		ret = _media_db_get_media_group_item(group_name, filter, group, callback, user_data);
	}

	return ret;
}
