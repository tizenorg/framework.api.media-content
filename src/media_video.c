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


int video_meta_destroy(video_meta_h video)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	video_meta_s *_video = (video_meta_s*)video;

	if(_video)
	{
		SAFE_FREE(_video->media_id);
		SAFE_FREE(_video->title);
		SAFE_FREE(_video->album);
		SAFE_FREE(_video->artist);
		SAFE_FREE(_video->album_artist);
		SAFE_FREE(_video->genre);
		SAFE_FREE(_video->composer);
		SAFE_FREE(_video->year);
		SAFE_FREE(_video->recorded_date);
		SAFE_FREE(_video->copyright);
		SAFE_FREE(_video->track_num);
		SAFE_FREE(_video);

		SAFE_FREE(_video);
		ret = MEDIA_CONTENT_ERROR_NONE;
	}
	else
	{
		media_content_error("INVALID_PARAMETER(0x%08x)", MEDIA_CONTENT_ERROR_INVALID_PARAMETER);
		ret = MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
	}

	return ret;
}

int video_meta_clone(video_meta_h *dst, video_meta_h src)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	video_meta_s *_src = (video_meta_s*)src;

	if(_src != NULL)
	{
		video_meta_s *_dst = (video_meta_s*)calloc(1, sizeof(video_meta_s));
		media_content_retvm_if(_dst == NULL, MEDIA_CONTENT_ERROR_OUT_OF_MEMORY, "OUT_OF_MEMORY");

		if(STRING_VALID(_src->media_id))
		{
			_dst->media_id = strdup(_src->media_id);
			if(_dst->media_id == NULL)
			{
				video_meta_destroy((video_meta_h)_dst);
				media_content_error("OUT_OF_MEMORY(0x%08x)", MEDIA_CONTENT_ERROR_OUT_OF_MEMORY);
				return MEDIA_CONTENT_ERROR_OUT_OF_MEMORY;
			}
		}

		if(STRING_VALID(_src->title))
		{
			_dst->title = strdup(_src->title);
			if(_dst->title == NULL)
			{
				video_meta_destroy((video_meta_h)_dst);
				media_content_error("OUT_OF_MEMORY(0x%08x)", MEDIA_CONTENT_ERROR_OUT_OF_MEMORY);
				return MEDIA_CONTENT_ERROR_OUT_OF_MEMORY;
			}
		}

		if(STRING_VALID(_src->album))
		{
			_dst->album = strdup(_src->album);
			if(_dst->album == NULL)
			{
				video_meta_destroy((video_meta_h)_dst);
				media_content_error("OUT_OF_MEMORY(0x%08x)", MEDIA_CONTENT_ERROR_OUT_OF_MEMORY);
				return MEDIA_CONTENT_ERROR_OUT_OF_MEMORY;
			}
		}

		if(STRING_VALID(_src->artist))
		{
			_dst->artist = strdup(_src->artist);
			if(_dst->artist == NULL)
			{
				video_meta_destroy((video_meta_h)_dst);
				media_content_error("OUT_OF_MEMORY(0x%08x)", MEDIA_CONTENT_ERROR_OUT_OF_MEMORY);
				return MEDIA_CONTENT_ERROR_OUT_OF_MEMORY;
			}
		}

		if(STRING_VALID(_src->album_artist))
		{
			_dst->album_artist = strdup(_src->album_artist);
			if(_dst->album_artist == NULL)
			{
				video_meta_destroy((video_meta_h)_dst);
				media_content_error("OUT_OF_MEMORY(0x%08x)", MEDIA_CONTENT_ERROR_OUT_OF_MEMORY);
				return MEDIA_CONTENT_ERROR_OUT_OF_MEMORY;
			}
		}

		if(STRING_VALID(_src->genre))
		{
			_dst->genre = strdup(_src->genre);
			if(_dst->genre == NULL)
			{
				video_meta_destroy((video_meta_h)_dst);
				media_content_error("OUT_OF_MEMORY(0x%08x)", MEDIA_CONTENT_ERROR_OUT_OF_MEMORY);
				return MEDIA_CONTENT_ERROR_OUT_OF_MEMORY;
			}
		}

		if(STRING_VALID(_src->composer))
		{
			_dst->composer = strdup(_src->composer);
			if(_dst->composer == NULL)
			{
				video_meta_destroy((video_meta_h)_dst);
				media_content_error("OUT_OF_MEMORY(0x%08x)", MEDIA_CONTENT_ERROR_OUT_OF_MEMORY);
				return MEDIA_CONTENT_ERROR_OUT_OF_MEMORY;
			}
		}

		if(STRING_VALID(_src->year))
		{
			_dst->year = strdup(_src->year);
			if(_dst->year == NULL)
			{
				video_meta_destroy((video_meta_h)_dst);
				media_content_error("OUT_OF_MEMORY(0x%08x)", MEDIA_CONTENT_ERROR_OUT_OF_MEMORY);
				return MEDIA_CONTENT_ERROR_OUT_OF_MEMORY;
			}
		}

		if(STRING_VALID(_src->recorded_date))
		{
			_dst->recorded_date = strdup(_src->recorded_date);
			if(_dst->recorded_date == NULL)
			{
				video_meta_destroy((video_meta_h)_dst);
				media_content_error("OUT_OF_MEMORY(0x%08x)", MEDIA_CONTENT_ERROR_OUT_OF_MEMORY);
				return MEDIA_CONTENT_ERROR_OUT_OF_MEMORY;
			}
		}

		if(STRING_VALID(_src->copyright))
		{
			_dst->copyright = strdup(_src->copyright);
			if(_dst->copyright == NULL)
			{
				video_meta_destroy((video_meta_h)_dst);
				media_content_error("OUT_OF_MEMORY(0x%08x)", MEDIA_CONTENT_ERROR_OUT_OF_MEMORY);
				return MEDIA_CONTENT_ERROR_OUT_OF_MEMORY;
			}
		}

		if(STRING_VALID(_src->track_num))
		{
			_dst->track_num = strdup(_src->track_num);
			if(_dst->track_num == NULL)
			{
				video_meta_destroy((video_meta_h)_dst);
				media_content_error("OUT_OF_MEMORY(0x%08x)", MEDIA_CONTENT_ERROR_OUT_OF_MEMORY);
				return MEDIA_CONTENT_ERROR_OUT_OF_MEMORY;
			}
		}

		_dst->bitrate = _src->bitrate;
		_dst->duration = _src->duration;
		_dst->width = _src->width;
		_dst->height = _src->height;
		_dst->played_count = _src->played_count;
		_dst->played_time = _src->played_time;
		_dst->played_position = _src->played_position;

		*dst = (video_meta_h)_dst;

		ret = MEDIA_CONTENT_ERROR_NONE;
	}
	else
	{
		media_content_error("INVALID_PARAMETER(0x%08x)", MEDIA_CONTENT_ERROR_INVALID_PARAMETER);
		ret = MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
	}

	return ret;
}

int video_meta_get_media_id(video_meta_h video, char **media_id)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	video_meta_s *_video = (video_meta_s*)video;
	if(_video)
	{
		if(STRING_VALID(_video->media_id))
		{
			char *new_string = strdup(_video->media_id);
			media_content_retvm_if(new_string == NULL, MEDIA_CONTENT_ERROR_OUT_OF_MEMORY, "OUT_OF_MEMORY");

			*media_id = new_string;
		}
		else
		{
			*media_id = NULL;
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

int video_meta_get_album(video_meta_h video, char **album)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	video_meta_s *_video = (video_meta_s*)video;
	if(_video)
	{
		if(STRING_VALID(_video->album))
		{
			char *new_string = strdup(_video->album);
			media_content_retvm_if(new_string == NULL, MEDIA_CONTENT_ERROR_OUT_OF_MEMORY, "OUT_OF_MEMORY");

			*album = new_string;
		}
		else
		{
			*album = NULL;
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
int video_meta_get_artist(video_meta_h video, char **artist)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	video_meta_s *_video = (video_meta_s*)video;
	if(_video)
	{
		if(STRING_VALID(_video->artist))
		{
			char *new_string = strdup(_video->artist);
			media_content_retvm_if(new_string == NULL, MEDIA_CONTENT_ERROR_OUT_OF_MEMORY, "OUT_OF_MEMORY");

			*artist = new_string;
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

int video_meta_get_album_artist(video_meta_h video, char **album_artist)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	video_meta_s *_video = (video_meta_s*)video;
	if(_video)
	{
		if(STRING_VALID(_video->album_artist))
		{
			char *new_string = strdup(_video->album_artist);
			media_content_retvm_if(new_string == NULL, MEDIA_CONTENT_ERROR_OUT_OF_MEMORY, "OUT_OF_MEMORY");

			*album_artist = new_string;
		}
		else
		{
			*album_artist = NULL;
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

int video_meta_get_genre(video_meta_h video, char **genre)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	video_meta_s *_video = (video_meta_s*)video;
	if(_video)
	{
		if(STRING_VALID(_video->genre))
		{
			char *new_string = strdup(_video->genre);
			media_content_retvm_if(new_string == NULL, MEDIA_CONTENT_ERROR_OUT_OF_MEMORY, "OUT_OF_MEMORY");

			*genre = new_string;
		}
		else
		{
			*genre = NULL;
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

int video_meta_get_composer(video_meta_h video, char **composer)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	video_meta_s *_video = (video_meta_s*)video;
	if(_video)
	{
		if(STRING_VALID(_video->composer))
		{
			char *new_string = strdup(_video->composer);
			media_content_retvm_if(new_string == NULL, MEDIA_CONTENT_ERROR_OUT_OF_MEMORY, "OUT_OF_MEMORY");

			*composer = new_string;
		}
		else
		{
			*composer = NULL;
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

int video_meta_get_year(video_meta_h video, char **year)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	video_meta_s *_video = (video_meta_s*)video;
	if(_video)
	{
		if(STRING_VALID(_video->year))
		{
			char *new_string = strdup(_video->year);
			media_content_retvm_if(new_string == NULL, MEDIA_CONTENT_ERROR_OUT_OF_MEMORY, "OUT_OF_MEMORY");

			*year = new_string;
		}
		else
		{
			*year = NULL;
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

int video_meta_get_recorded_date(video_meta_h video, char **recorded_date)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	video_meta_s *_video = (video_meta_s*)video;
	if(_video)
	{
		if(STRING_VALID(_video->recorded_date))
		{
			char *new_string = strdup(_video->recorded_date);
			media_content_retvm_if(new_string == NULL, MEDIA_CONTENT_ERROR_OUT_OF_MEMORY, "OUT_OF_MEMORY");

			*recorded_date = new_string;
		}
		else
		{
			*recorded_date = NULL;
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

int video_meta_get_copyright(video_meta_h video, char **copyright)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	video_meta_s *_video = (video_meta_s*)video;
	if(_video)
	{
		if(STRING_VALID(_video->copyright))
		{
			char *new_string = strdup(_video->copyright);
			media_content_retvm_if(new_string == NULL, MEDIA_CONTENT_ERROR_OUT_OF_MEMORY, "OUT_OF_MEMORY");

			*copyright = new_string;
		}
		else
		{
			*copyright = NULL;
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

int video_meta_get_track_num(video_meta_h video, char **track_num)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	video_meta_s *_video = (video_meta_s*)video;
	if(_video)
	{
		if(STRING_VALID(_video->track_num))
		{
			char *new_string = strdup(_video->track_num);
			media_content_retvm_if(new_string == NULL, MEDIA_CONTENT_ERROR_OUT_OF_MEMORY, "OUT_OF_MEMORY");

			*track_num = new_string;
		}
		else
		{
			*track_num = NULL;
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

int video_meta_get_bit_rate(video_meta_h video, int *bit_rate)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	video_meta_s *_video = (video_meta_s*)video;

	if(_video && bit_rate)
	{
		*bit_rate = _video->bitrate;
		ret = MEDIA_CONTENT_ERROR_NONE;
	}
	else
	{
		media_content_error("INVALID_PARAMETER(0x%08x)", MEDIA_CONTENT_ERROR_INVALID_PARAMETER);
		ret = MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
	}

	return ret;
}

int video_meta_get_duration(video_meta_h video, int *duration)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	video_meta_s *_video = (video_meta_s*)video;

	if(_video && duration)
	{
		*duration = _video->duration;
		ret = MEDIA_CONTENT_ERROR_NONE;
	}
	else
	{
		media_content_error("INVALID_PARAMETER(0x%08x)", MEDIA_CONTENT_ERROR_INVALID_PARAMETER);
		ret = MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
	}

	return ret;
}

int video_meta_get_width(video_meta_h video, int *width)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	video_meta_s *_video = (video_meta_s*)video;

	if(_video && width)
	{
		*width = _video->width;
		ret = MEDIA_CONTENT_ERROR_NONE;
	}
	else
	{
		media_content_error("INVALID_PARAMETER(0x%08x)", MEDIA_CONTENT_ERROR_INVALID_PARAMETER);
		ret = MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
	}

	return ret;
}

int video_meta_get_height(video_meta_h video, int *height)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	video_meta_s *_video = (video_meta_s*)video;
	if(_video && height)
	{
		*height = _video->height;
		ret = MEDIA_CONTENT_ERROR_NONE;
	}
	else
	{
		media_content_error("INVALID_PARAMETER(0x%08x)", MEDIA_CONTENT_ERROR_INVALID_PARAMETER);
		ret = MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
	}

	return ret;
}

int video_meta_get_played_count(video_meta_h video, int *played_count)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	video_meta_s *_video = (video_meta_s*)video;

	if(_video && played_count)
	{
		*played_count = _video->played_count;
		ret = MEDIA_CONTENT_ERROR_NONE;
	}
	else
	{
		media_content_error("INVALID_PARAMETER(0x%08x)", MEDIA_CONTENT_ERROR_INVALID_PARAMETER);
		ret = MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
	}

	return ret;
}

int video_meta_get_played_time(video_meta_h video, time_t* played_time)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	video_meta_s *_video = (video_meta_s*)video;

	if(_video)
	{
		*played_time = _video->played_time;
		ret = MEDIA_CONTENT_ERROR_NONE;
	}
	else
	{
		media_content_error("INVALID_PARAMETER(0x%08x)", MEDIA_CONTENT_ERROR_INVALID_PARAMETER);
		ret = MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
	}

	return ret;
}

int video_meta_get_played_position(video_meta_h video, int *played_position)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	video_meta_s *_video = (video_meta_s*)video;

	if(_video)
	{
		*played_position = _video->played_position;
		ret = MEDIA_CONTENT_ERROR_NONE;
	}
	else
	{
		media_content_error("INVALID_PARAMETER(0x%08x)", MEDIA_CONTENT_ERROR_INVALID_PARAMETER);
		ret = MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
	}

	return ret;
}

int video_meta_set_played_count(video_meta_h video, int played_count)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	video_meta_s *_video = (video_meta_s*)video;

	if(_video != NULL)
	{
		_video->played_count = played_count;
	}
	else
	{
		media_content_error("INVALID_PARAMETER(0x%08x)", MEDIA_CONTENT_ERROR_INVALID_PARAMETER);
		ret = MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
	}

	return ret;
}

int video_meta_set_played_time(video_meta_h video, time_t played_time)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	video_meta_s *_video = (video_meta_s*)video;

	if(_video != NULL)
	{
		_video->played_time = played_time;
	}
	else
	{
		media_content_error("INVALID_PARAMETER(0x%08x)", MEDIA_CONTENT_ERROR_INVALID_PARAMETER);
		ret = MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
	}

	return ret;
}

int video_meta_set_played_position(video_meta_h video, int played_position)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	video_meta_s *_video = (video_meta_s*)video;

	if(_video != NULL)
	{
		_video->played_position = played_position;
	}
	else
	{
		media_content_error("INVALID_PARAMETER(0x%08x)", MEDIA_CONTENT_ERROR_INVALID_PARAMETER);
		ret = MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
	}

	return ret;
}

int video_meta_update_to_db(video_meta_h video)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	video_meta_s *_video = (video_meta_s*)video;
	char *sql = NULL;

	if(_video != NULL && STRING_VALID(_video->media_id))
	{
		char storage_id[MEDIA_CONTENT_UUID_SIZE+1] = {0,};
		memset(storage_id, 0x00, sizeof(storage_id));

		ret = _media_db_get_storage_id_by_media_id(_video->media_id, storage_id);
		media_content_retv_if(ret != MEDIA_CONTENT_ERROR_NONE, ret);

		sql = sqlite3_mprintf(UPDATE_AV_META_FROM_MEDIA, storage_id, _video->played_count, _video->played_time, _video->played_position, _video->media_id);
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
