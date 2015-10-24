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


int media_bookmark_insert_to_db(const char *media_id, time_t time, const char *thumbnail_path)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	char *sql = NULL;

	if(STRING_VALID(media_id))
	{
		sql = sqlite3_mprintf(INSERT_BOOKMARK_TO_BOOKMARK, media_id, time, thumbnail_path);
		ret = _content_query_sql(sql);
		SQLITE3_SAFE_FREE(sql);
	}
	else
	{
		media_content_error("INVALID_PARAMETER(0x%08x)", MEDIA_CONTENT_ERROR_INVALID_PARAMETER);
		ret = MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
	}

	return ret;
}

int media_bookmark_delete_from_db(int bookmark_id)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	char *query_str = NULL;

	if(bookmark_id < 0)
	{
		media_content_error("INVALID_PARAMETER(0x%08x)", MEDIA_CONTENT_ERROR_INVALID_PARAMETER);
		return MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
	}

	query_str = sqlite3_mprintf(DELETE_BOOKMARK_FROM_BOOKMARK, bookmark_id);

	ret = _content_query_sql(query_str);

	SQLITE3_SAFE_FREE(query_str);

	return ret;
}

int media_bookmark_get_bookmark_count_from_db(filter_h filter, int *bookmark_count)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;

	if (bookmark_count == NULL)
	{
		media_content_error("INVALID_PARAMETER(0x%08x)", MEDIA_CONTENT_ERROR_INVALID_PARAMETER);
		return MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
	}

	ret = _media_db_get_group_count(filter, MEDIA_GROUP_BOOKMARK, bookmark_count);

	return ret;
}

int media_bookmark_destroy(media_bookmark_h bookmark)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	media_bookmark_s *_bookmark = (media_bookmark_s*)bookmark;

	if(_bookmark)
	{
		SAFE_FREE(_bookmark->media_id);
		SAFE_FREE(_bookmark->thumbnail_path);
		free(_bookmark);
		ret = MEDIA_CONTENT_ERROR_NONE;
	}
	else
	{
		media_content_error("INVALID_PARAMETER(0x%08x)", MEDIA_CONTENT_ERROR_INVALID_PARAMETER);
		ret = MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
	}

	return ret;
}
int media_bookmark_clone(media_bookmark_h *dst, media_bookmark_h src)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;

	if((src != NULL))
	{
		media_bookmark_s *_src = (media_bookmark_s*)src;
		media_bookmark_s *_dst = (media_bookmark_s*)calloc(1, sizeof(media_bookmark_s));
		media_content_retvm_if(_dst == NULL, MEDIA_CONTENT_ERROR_OUT_OF_MEMORY, "OUT_OF_MEMORY");

		_dst->bookmark_id = _src->bookmark_id;

		if(STRING_VALID(_src->media_id))
		{
			_dst->media_id = (char*)strdup(_src->media_id);
			if(_dst->media_id == NULL)
			{
				media_content_error("OUT_OF_MEMORY(0x%08x)", MEDIA_CONTENT_ERROR_OUT_OF_MEMORY);
				media_bookmark_destroy((media_bookmark_h)_dst);
				return MEDIA_CONTENT_ERROR_OUT_OF_MEMORY;
			}
		}

		_dst->marked_time = _src->marked_time;

		if(STRING_VALID(_src->thumbnail_path))
		{
			_dst->thumbnail_path = (char*)strdup(_src->thumbnail_path);
			if(_dst->thumbnail_path == NULL)
			{
				media_content_error("OUT_OF_MEMORY(0x%08x)", MEDIA_CONTENT_ERROR_OUT_OF_MEMORY);
				media_bookmark_destroy((media_bookmark_h)_dst);
				return MEDIA_CONTENT_ERROR_OUT_OF_MEMORY;
			}
		}

		*dst = (media_bookmark_h)_dst;

		ret = MEDIA_CONTENT_ERROR_NONE;
	}
	else
	{
		media_content_error("INVALID_PARAMETER(0x%08x)", MEDIA_CONTENT_ERROR_INVALID_PARAMETER);
		ret = MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
	}

	return ret;
}

int media_bookmark_get_bookmark_id(media_bookmark_h bookmark, int *bookmark_id)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	media_bookmark_s *_bookmark = (media_bookmark_s*)bookmark;

	if(_bookmark)
	{
		*bookmark_id = _bookmark->bookmark_id;
		ret = MEDIA_CONTENT_ERROR_NONE;
	}
	else
	{
		media_content_error("INVALID_PARAMETER(0x%08x)", MEDIA_CONTENT_ERROR_INVALID_PARAMETER);
		ret = MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
	}

	return ret;
}

int media_bookmark_get_marked_time(media_bookmark_h bookmark, time_t* marked_time)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	media_bookmark_s *_bookmark = (media_bookmark_s*)bookmark;

	if(_bookmark)
	{
		*marked_time = _bookmark->marked_time;
		ret = MEDIA_CONTENT_ERROR_NONE;
	}
	else
	{
		media_content_error("INVALID_PARAMETER(0x%08x)", MEDIA_CONTENT_ERROR_INVALID_PARAMETER);
		ret = MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
	}

	return ret;
}

int media_bookmark_get_thumbnail_path(media_bookmark_h bookmark, char **path)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	media_bookmark_s *_bookmark = (media_bookmark_s*)bookmark;

	if(_bookmark)
	{
		if(STRING_VALID(_bookmark->thumbnail_path))
		{
			*path = strdup(_bookmark->thumbnail_path);
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
