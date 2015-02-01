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


int audio_meta_destroy(audio_meta_h audio)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	audio_meta_s *_audio = (audio_meta_s*)audio;

	if(_audio)
	{
		SAFE_FREE(_audio->media_id);
		SAFE_FREE(_audio->title);
		SAFE_FREE(_audio->album);
		SAFE_FREE(_audio->artist);
		SAFE_FREE(_audio->album_artist);
		SAFE_FREE(_audio->genre);
		SAFE_FREE(_audio->composer);
		SAFE_FREE(_audio->year);
		SAFE_FREE(_audio->recorded_date);
		SAFE_FREE(_audio->copyright);
		SAFE_FREE(_audio->track_num);
		SAFE_FREE(_audio);

		ret = MEDIA_CONTENT_ERROR_NONE;
	}
	else
	{
		media_content_error("INVALID_PARAMETER(0x%08x)", MEDIA_CONTENT_ERROR_INVALID_PARAMETER);
		ret = MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
	}

	return ret;
}

int audio_meta_clone(audio_meta_h *dst, audio_meta_h src)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	audio_meta_s *_src = (audio_meta_s*)src;

	if(_src != NULL)
	{
		audio_meta_s *_dst = (audio_meta_s*)calloc(1, sizeof(audio_meta_s));

		if(_dst == NULL)
		{

			media_content_error("OUT_OF_MEMORY(0x%08x)", MEDIA_CONTENT_ERROR_OUT_OF_MEMORY);
			return MEDIA_CONTENT_ERROR_OUT_OF_MEMORY;
		}

		if(STRING_VALID(_src->media_id))
		{
			_dst->media_id = strdup(_src->media_id);
			if(_dst->media_id == NULL)
			{
				audio_meta_destroy((audio_meta_h)_dst);
				media_content_error("OUT_OF_MEMORY(0x%08x)", MEDIA_CONTENT_ERROR_OUT_OF_MEMORY);
				return MEDIA_CONTENT_ERROR_OUT_OF_MEMORY;
			}
		}

		if(STRING_VALID(_src->title))
		{
			_dst->title = strdup(_src->title);
			if(_dst->title == NULL)
			{
				audio_meta_destroy((audio_meta_h)_dst);
				media_content_error("OUT_OF_MEMORY(0x%08x)", MEDIA_CONTENT_ERROR_OUT_OF_MEMORY);
				return MEDIA_CONTENT_ERROR_OUT_OF_MEMORY;
			}
		}

		if(STRING_VALID(_src->album))
		{
			_dst->album = strdup(_src->album);
			if(_dst->album == NULL)
			{
				audio_meta_destroy((audio_meta_h)_dst);
				media_content_error("OUT_OF_MEMORY(0x%08x)", MEDIA_CONTENT_ERROR_OUT_OF_MEMORY);
				return MEDIA_CONTENT_ERROR_OUT_OF_MEMORY;
			}
		}

		if(STRING_VALID(_src->artist))
		{
			_dst->artist = strdup(_src->artist);
			if(_dst->artist == NULL)
			{
				audio_meta_destroy((audio_meta_h)_dst);
				media_content_error("OUT_OF_MEMORY(0x%08x)", MEDIA_CONTENT_ERROR_OUT_OF_MEMORY);
				return MEDIA_CONTENT_ERROR_OUT_OF_MEMORY;
			}
		}

		if(STRING_VALID(_src->album_artist))
		{
			_dst->album_artist = strdup(_src->album_artist);
			if(_dst->album_artist == NULL)
			{
				audio_meta_destroy((audio_meta_h)_dst);
				media_content_error("OUT_OF_MEMORY(0x%08x)", MEDIA_CONTENT_ERROR_OUT_OF_MEMORY);
				return MEDIA_CONTENT_ERROR_OUT_OF_MEMORY;
			}
		}

		if(STRING_VALID(_src->genre))
		{
			_dst->genre = strdup(_src->genre);
			if(_dst->genre == NULL)
			{
				audio_meta_destroy((audio_meta_h)_dst);
				media_content_error("OUT_OF_MEMORY(0x%08x)", MEDIA_CONTENT_ERROR_OUT_OF_MEMORY);
				return MEDIA_CONTENT_ERROR_OUT_OF_MEMORY;
			}
		}

		if(STRING_VALID(_src->composer))
		{
			_dst->composer = strdup(_src->composer);
			if(_dst->composer == NULL)
			{
				audio_meta_destroy((audio_meta_h)_dst);
				media_content_error("OUT_OF_MEMORY(0x%08x)", MEDIA_CONTENT_ERROR_OUT_OF_MEMORY);
				return MEDIA_CONTENT_ERROR_OUT_OF_MEMORY;
			}
		}

		if(STRING_VALID(_src->year))
		{
			_dst->year = strdup(_src->year);
			if(_dst->year == NULL)
			{
				audio_meta_destroy((audio_meta_h)_dst);
				media_content_error("OUT_OF_MEMORY(0x%08x)", MEDIA_CONTENT_ERROR_OUT_OF_MEMORY);
				return MEDIA_CONTENT_ERROR_OUT_OF_MEMORY;
			}
		}

		if(STRING_VALID(_src->recorded_date))
		{
			_dst->recorded_date = strdup(_src->recorded_date);
			if(_dst->recorded_date == NULL)
			{
				audio_meta_destroy((audio_meta_h)_dst);
				media_content_error("OUT_OF_MEMORY(0x%08x)", MEDIA_CONTENT_ERROR_OUT_OF_MEMORY);
				return MEDIA_CONTENT_ERROR_OUT_OF_MEMORY;
			}
		}

		if(STRING_VALID(_src->copyright))
		{
			_dst->copyright = strdup(_src->copyright);
			if(_dst->copyright == NULL)
			{
				audio_meta_destroy((audio_meta_h)_dst);
				media_content_error("OUT_OF_MEMORY(0x%08x)", MEDIA_CONTENT_ERROR_OUT_OF_MEMORY);
				return MEDIA_CONTENT_ERROR_OUT_OF_MEMORY;
			}
		}

		if(STRING_VALID(_src->track_num))
		{
			_dst->track_num = strdup(_src->track_num);
			if(_dst->track_num == NULL)
			{
				audio_meta_destroy((audio_meta_h)_dst);
				media_content_error("OUT_OF_MEMORY(0x%08x)", MEDIA_CONTENT_ERROR_OUT_OF_MEMORY);
				return MEDIA_CONTENT_ERROR_OUT_OF_MEMORY;
			}
		}

		_dst->bitrate = _src->bitrate;
		_dst->bitpersample = _src->bitpersample;
		_dst->samplerate = _src->samplerate;
		_dst->channel = _src->channel;
		_dst->duration = _src->duration;
		_dst->played_count = _src->played_count;
		_dst->played_time = _src->played_time;
		_dst->played_position = _src->played_position;

		*dst = (audio_meta_h)_dst;

		ret = MEDIA_CONTENT_ERROR_NONE;
	}
	else
	{
		media_content_error("INVALID_PARAMETER(0x%08x)", MEDIA_CONTENT_ERROR_INVALID_PARAMETER);
		ret = MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
	}

	return ret;
}

int audio_meta_get_media_id(audio_meta_h audio, char **media_id)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	audio_meta_s *_audio = (audio_meta_s*)audio;
	if(_audio)
	{
		if(STRING_VALID(_audio->media_id))
		{
			*media_id = strdup(_audio->media_id);
			if(*media_id == NULL)
			{
				media_content_error("OUT_OF_MEMORY(0x%08x)", MEDIA_CONTENT_ERROR_OUT_OF_MEMORY);
				return MEDIA_CONTENT_ERROR_OUT_OF_MEMORY;
			}
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

int audio_meta_get_album(audio_meta_h audio, char **album_name)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	audio_meta_s *_audio = (audio_meta_s*)audio;
	if(_audio)
	{
		if(STRING_VALID(_audio->album))
		{
			*album_name = strdup(_audio->album);
			if(*album_name == NULL)
			{
				media_content_error("OUT_OF_MEMORY(0x%08x)", MEDIA_CONTENT_ERROR_OUT_OF_MEMORY);
				return MEDIA_CONTENT_ERROR_OUT_OF_MEMORY;
			}
		}
		else
		{
			*album_name = NULL;
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

int audio_meta_get_artist(audio_meta_h audio, char **artist_name)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	audio_meta_s *_audio = (audio_meta_s*)audio;

	if(_audio)
	{
		if(STRING_VALID(_audio->artist))
		{
			*artist_name = strdup(_audio->artist);
			if(*artist_name == NULL)
			{
				media_content_error("OUT_OF_MEMORY(0x%08x)", MEDIA_CONTENT_ERROR_OUT_OF_MEMORY);
				return MEDIA_CONTENT_ERROR_OUT_OF_MEMORY;
			}
		}
		else
		{
			*artist_name = NULL;
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

int audio_meta_get_album_artist(audio_meta_h audio, char **album_artist_name)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	audio_meta_s *_audio = (audio_meta_s*)audio;

	if(_audio)
	{
		if(STRING_VALID(_audio->album_artist))
		{
			*album_artist_name = strdup(_audio->album_artist);
			if(*album_artist_name == NULL)
			{
				media_content_error("OUT_OF_MEMORY(0x%08x)", MEDIA_CONTENT_ERROR_OUT_OF_MEMORY);
				return MEDIA_CONTENT_ERROR_OUT_OF_MEMORY;
			}
		}
		else
		{
			*album_artist_name = NULL;
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

int audio_meta_get_genre(audio_meta_h audio, char **genre_name)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	audio_meta_s *_audio = (audio_meta_s*)audio;
	if(_audio)
	{
		if(STRING_VALID(_audio->genre))
		{
			*genre_name = strdup(_audio->genre);
			if(*genre_name == NULL)
			{
				media_content_error("OUT_OF_MEMORY(0x%08x)", MEDIA_CONTENT_ERROR_OUT_OF_MEMORY);
				return MEDIA_CONTENT_ERROR_OUT_OF_MEMORY;
			}
		}
		else
		{
			*genre_name = NULL;
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

int audio_meta_get_composer(audio_meta_h audio, char **composer_name)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	audio_meta_s *_audio = (audio_meta_s*)audio;
	if(_audio)
	{
		if(STRING_VALID(_audio->composer))
		{
			*composer_name = strdup(_audio->composer);
			if(*composer_name == NULL)
			{
				media_content_error("OUT_OF_MEMORY(0x%08x)", MEDIA_CONTENT_ERROR_OUT_OF_MEMORY);
				return MEDIA_CONTENT_ERROR_OUT_OF_MEMORY;
			}
		}
		else
		{
			*composer_name = NULL;
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

int audio_meta_get_year(audio_meta_h audio, char **year)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	audio_meta_s *_audio = (audio_meta_s*)audio;
	if(_audio)
	{
		if(STRING_VALID(_audio->year))
		{
			*year = strdup(_audio->year);
			if(*year == NULL)
			{
				media_content_error("OUT_OF_MEMORY(0x%08x)", MEDIA_CONTENT_ERROR_OUT_OF_MEMORY);
				return MEDIA_CONTENT_ERROR_OUT_OF_MEMORY;
			}
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

int audio_meta_get_recorded_date(audio_meta_h audio, char **recorded_date)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	audio_meta_s *_audio = (audio_meta_s*)audio;
	if(_audio)
	{
		if(STRING_VALID(_audio->recorded_date))
		{
			*recorded_date = strdup(_audio->recorded_date);
			if(*recorded_date == NULL)
			{
				media_content_error("OUT_OF_MEMORY(0x%08x)", MEDIA_CONTENT_ERROR_OUT_OF_MEMORY);
				return MEDIA_CONTENT_ERROR_OUT_OF_MEMORY;
			}
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

int audio_meta_get_copyright(audio_meta_h audio, char **copyright)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	audio_meta_s *_audio = (audio_meta_s*)audio;
	if(_audio)
	{
		if(STRING_VALID(_audio->copyright))
		{
			*copyright = strdup(_audio->copyright);
			if(*copyright == NULL)
			{
				media_content_error("OUT_OF_MEMORY(0x%08x)", MEDIA_CONTENT_ERROR_OUT_OF_MEMORY);
				return MEDIA_CONTENT_ERROR_OUT_OF_MEMORY;
			}
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

int audio_meta_get_track_num(audio_meta_h audio, char **track_num)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	audio_meta_s *_audio = (audio_meta_s*)audio;
	if(_audio)
	{
		if(STRING_VALID(_audio->track_num))
		{
			*track_num = strdup(_audio->track_num);
			if(*track_num == NULL)
			{
				media_content_error("OUT_OF_MEMORY(0x%08x)", MEDIA_CONTENT_ERROR_OUT_OF_MEMORY);
				return MEDIA_CONTENT_ERROR_OUT_OF_MEMORY;
			}
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

int audio_meta_get_bit_rate(audio_meta_h audio, int *bit_rate)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	audio_meta_s *_audio = (audio_meta_s*)audio;

	if(_audio && bit_rate)
	{
		*bit_rate = _audio->bitrate;
		ret = MEDIA_CONTENT_ERROR_NONE;
	}
	else
	{
		media_content_error("INVALID_PARAMETER(0x%08x)", MEDIA_CONTENT_ERROR_INVALID_PARAMETER);
		ret = MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
	}

	return ret;
}

int audio_meta_get_bitpersample(audio_meta_h audio, int *bitpersample)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	audio_meta_s *_audio = (audio_meta_s*)audio;

	if(_audio && bitpersample)
	{
		*bitpersample = _audio->bitpersample;
		ret = MEDIA_CONTENT_ERROR_NONE;
	}
	else
	{
		media_content_error("INVALID_PARAMETER(0x%08x)", MEDIA_CONTENT_ERROR_INVALID_PARAMETER);
		ret = MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
	}

	return ret;
}

int audio_meta_get_sample_rate(audio_meta_h audio, int *sample_rate)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	audio_meta_s *_audio = (audio_meta_s*)audio;

	if(_audio && sample_rate)
	{
		*sample_rate = _audio->samplerate;
		ret = MEDIA_CONTENT_ERROR_NONE;
	}
	else
	{
		media_content_error("INVALID_PARAMETER(0x%08x)", MEDIA_CONTENT_ERROR_INVALID_PARAMETER);
		ret = MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
	}

	return ret;
}

int audio_meta_get_channel(audio_meta_h audio, int *channel)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	audio_meta_s *_audio = (audio_meta_s*)audio;

	if(_audio && channel)
	{
		*channel = _audio->channel;
		ret = MEDIA_CONTENT_ERROR_NONE;
	}
	else
	{
		media_content_error("INVALID_PARAMETER(0x%08x)", MEDIA_CONTENT_ERROR_INVALID_PARAMETER);
		ret = MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
	}

	return ret;
}

int audio_meta_get_duration(audio_meta_h audio, int *duration)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	audio_meta_s *_audio = (audio_meta_s*)audio;

	if(_audio)
	{
		*duration = _audio->duration;
		ret = MEDIA_CONTENT_ERROR_NONE;
	}
	else
	{
		media_content_error("INVALID_PARAMETER(0x%08x)", MEDIA_CONTENT_ERROR_INVALID_PARAMETER);
		ret = MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
	}

	return ret;
}

int audio_meta_get_played_count(audio_meta_h audio, int *played_count)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	audio_meta_s *_audio = (audio_meta_s*)audio;

	if(_audio)
	{
		*played_count = _audio->played_count;
		ret = MEDIA_CONTENT_ERROR_NONE;
	}
	else
	{
		media_content_error("INVALID_PARAMETER(0x%08x)", MEDIA_CONTENT_ERROR_INVALID_PARAMETER);
		ret = MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
	}

	return ret;
}

int audio_meta_get_played_time(audio_meta_h audio, time_t* played_time)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	audio_meta_s *_audio = (audio_meta_s*)audio;

	if(_audio)
	{
		*played_time = _audio->played_time;
		ret = MEDIA_CONTENT_ERROR_NONE;
	}
	else
	{
		media_content_error("INVALID_PARAMETER(0x%08x)", MEDIA_CONTENT_ERROR_INVALID_PARAMETER);
		ret = MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
	}

	return ret;
}

int audio_meta_get_played_position(audio_meta_h audio, int *played_position)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	audio_meta_s *_audio = (audio_meta_s*)audio;

	if(_audio)
	{
		*played_position = _audio->played_position;
		ret = MEDIA_CONTENT_ERROR_NONE;
	}
	else
	{
		media_content_error("INVALID_PARAMETER(0x%08x)", MEDIA_CONTENT_ERROR_INVALID_PARAMETER);
		ret = MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
	}

	return ret;
}

int audio_meta_set_played_count(audio_meta_h audio, int played_count)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;

	audio_meta_s *_audio = (audio_meta_s*)audio;

	if((_audio != NULL) && (played_count >= 0))
	{
		_audio->played_count = played_count;
	}
	else
	{
		media_content_error("INVALID_PARAMETER(0x%08x)", MEDIA_CONTENT_ERROR_INVALID_PARAMETER);
		ret = MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
	}

	return ret;
}

int audio_meta_set_played_time(audio_meta_h audio, time_t played_time)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;

	audio_meta_s *_audio = (audio_meta_s*)audio;

	if((_audio != NULL) && (played_time >= 0))
	{
		_audio->played_time = played_time;
	}
	else
	{
		media_content_error("INVALID_PARAMETER(0x%08x)", MEDIA_CONTENT_ERROR_INVALID_PARAMETER);
		ret = MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
	}

	return ret;
}

int audio_meta_set_played_position(audio_meta_h audio, int played_position)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	audio_meta_s *_audio = (audio_meta_s*)audio;

	if((_audio != NULL) && (played_position >= 0))
	{
		_audio->played_position = played_position;
	}
	else
	{
		media_content_error("INVALID_PARAMETER(0x%08x)", MEDIA_CONTENT_ERROR_INVALID_PARAMETER);
		ret = MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
	}

	return ret;
}

int audio_meta_update_to_db(audio_meta_h audio)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	audio_meta_s *_audio = (audio_meta_s*)audio;
	char *sql = NULL;

	if(_audio != NULL && STRING_VALID(_audio->media_id))
	{
		sql = sqlite3_mprintf(UPDATE_AV_META_FROM_MEDIA, _audio->played_count, _audio->played_time, _audio->played_position, _audio->media_id);
		ret = _content_query_sql(sql);
		sqlite3_free(sql);
	}
	else
	{
		media_content_error("INVALID_PARAMETER(0x%08x)", MEDIA_CONTENT_ERROR_INVALID_PARAMETER);
		ret = MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
	}

	return ret;
}
