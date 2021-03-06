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
#include <media-thumbnail.h>
#include <media_info_private.h>
#include <media_util_private.h>
#include <media-svc.h>
#include <media-util-register.h>
#include <media-util-err.h>


static int __media_info_get_media_info_from_db(char *path, media_info_h media);
static void __media_info_thumbnail_completed_cb(int error, const char *path, void *user_data);

static int __media_info_get_media_info_from_db(char *path, media_info_h media)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	sqlite3_stmt *stmt = NULL;
	char *select_query = NULL;
	media_info_s *_media = (media_info_s*)media;

	if(_media == NULL)
	{
		media_content_error("INVALID_PARAMETER(0x%08x)", MEDIA_CONTENT_ERROR_INVALID_PARAMETER);
		return MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
	}

	select_query = sqlite3_mprintf(SELECT_MEDIA_BY_PATH, path);

	ret = _content_query_prepare(&stmt, select_query, NULL, NULL);
	sqlite3_free(select_query);
	media_content_retv_if(ret != MEDIA_CONTENT_ERROR_NONE, ret);

	if(sqlite3_step(stmt) == SQLITE_ROW)
	{
		_media_info_item_get_detail(stmt, (media_info_h)_media);
		media_content_debug("New Media ID: %s", _media->media_id);
	} else {
		media_content_debug("There's no media!!");
	}
/*
	while(sqlite3_step(stmt) == SQLITE_ROW)
	{
		_media_info_item_get_detail(stmt, (media_info_h)_media);
		media_content_debug("New Media ID: %s", _media->media_id);
	}
*/
	SQLITE3_FINALIZE(stmt);

	return ret;
}

static void __media_info_thumbnail_completed_cb(int error, const char *path, void *user_data)
{
	int error_value = MEDIA_CONTENT_ERROR_NONE;

	media_thumbnail_cb_s *_thumb_cb = (media_thumbnail_cb_s *)user_data;
	media_info_s *media = NULL;

	if(_thumb_cb != NULL)
	{
		media = _thumb_cb->handle;
		if((media != NULL) && STRING_VALID(path))
		{
			SAFE_FREE(media->thumbnail_path);
			media->thumbnail_path = strdup(path);
		}

		media_content_debug("error [%d], thumbnail_path [%s]", error, path);
		error_value = _content_error_capi(MEDIA_THUMBNAIL_TYPE, error);
		_thumb_cb->thumbnail_completed_cb(error_value, path, _thumb_cb->user_data);
	}

	SAFE_FREE(_thumb_cb);

	return;
}

void _media_info_item_get_detail(sqlite3_stmt* stmt, media_info_h media)
{
//#define MEDIA_INFO_ITEM "media_uuid, path, file_name, media_type, mime_type, size, added_time, modified_time, thumbnail_path, description,
//							rating, favourite, author, provider, content_name, category, location_tag, age_rating, keyword, is_drm, storage_type, longitude, latitude, altitude, width, height, datetaken, orientation, title, album, artist, genre, composer, year, recorded_date, copyright, track_num, bitrate, duration, played_count, last_played_time, last_played_position, samplerate, channel"

	media_info_s *_media = (media_info_s*)media;

	if(STRING_VALID((const char *)sqlite3_column_text(stmt, 0)))
		_media->media_id = strdup((const char *)sqlite3_column_text(stmt, 0));

	if(STRING_VALID((const char *)sqlite3_column_text(stmt, 1)))
		_media->file_path = strdup((const char *)sqlite3_column_text(stmt, 1));

	if(STRING_VALID((const char *)sqlite3_column_text(stmt, 2)))
		_media->display_name = strdup((const char *)sqlite3_column_text(stmt, 2));

	_media->media_type = (int)sqlite3_column_int(stmt, 3);

	if(STRING_VALID((const char *)sqlite3_column_text(stmt, 4)))
		_media->mime_type = strdup((const char *)sqlite3_column_text(stmt, 4));

	_media->size = (unsigned long long)sqlite3_column_int64(stmt, 5);

	_media->added_time = (int)sqlite3_column_int(stmt, 6);

	_media->modified_time = (int)sqlite3_column_int(stmt, 7);

	if(STRING_VALID((const char *)sqlite3_column_text(stmt, 8)))
		_media->thumbnail_path= strdup((const char *)sqlite3_column_text(stmt, 8));

	if(STRING_VALID((const char *)sqlite3_column_text(stmt, 9)))
		_media->description = strdup((const char *)sqlite3_column_text(stmt, 9));

	_media->rating = (int)sqlite3_column_int(stmt,10);

	_media->favourite = (int)sqlite3_column_int(stmt, 11);

	if(STRING_VALID((const char *)sqlite3_column_text(stmt, 12)))
		_media->author = strdup((const char *)sqlite3_column_text(stmt, 12));

	if(STRING_VALID((const char *)sqlite3_column_text(stmt, 13)))
		_media->provider = strdup((const char *)sqlite3_column_text(stmt, 13));

	if(STRING_VALID((const char *)sqlite3_column_text(stmt, 14)))
		_media->content_name = strdup((const char *)sqlite3_column_text(stmt, 14));

	if(STRING_VALID((const char *)sqlite3_column_text(stmt, 15)))
		_media->category = strdup((const char *)sqlite3_column_text(stmt, 15));

	if(STRING_VALID((const char *)sqlite3_column_text(stmt, 16)))
		_media->location_tag = strdup((const char *)sqlite3_column_text(stmt, 16));

	if(STRING_VALID((const char *)sqlite3_column_text(stmt, 17)))
		_media->age_rating = strdup((const char *)sqlite3_column_text(stmt, 17));

	if(STRING_VALID((const char *)sqlite3_column_text(stmt, 18)))
		_media->keyword = strdup((const char *)sqlite3_column_text(stmt, 18));

	_media->is_drm = (int)sqlite3_column_int(stmt, 19);

	_media->storage_type = (int)sqlite3_column_int(stmt, 20);

	_media->longitude = (double)sqlite3_column_double(stmt, 21);
	_media->latitude = (double)sqlite3_column_double(stmt, 22);
	_media->altitude = (double)sqlite3_column_double(stmt, 23);

	if(_media->media_type == MEDIA_CONTENT_TYPE_IMAGE) {
		_media->image_meta = (image_meta_s *)calloc(1, sizeof(image_meta_s));
		if(_media->image_meta) {
			if(STRING_VALID(_media->media_id))
				_media->image_meta->media_id = strdup(_media->media_id);

			_media->image_meta->width = sqlite3_column_int(stmt, 24);
			_media->image_meta->height = sqlite3_column_int(stmt, 25);

			if(STRING_VALID((const char *)sqlite3_column_text(stmt, 26)))
				_media->image_meta->date_taken = strdup((const char *)sqlite3_column_text(stmt, 26));
		
			_media->image_meta->orientation = sqlite3_column_int(stmt, 27);
		}

	} else if(_media->media_type == MEDIA_CONTENT_TYPE_VIDEO) {
		_media->video_meta = (video_meta_s *)calloc(1, sizeof(video_meta_s));
		if(_media->video_meta) {
			if(STRING_VALID(_media->media_id))
				_media->video_meta->media_id = strdup(_media->media_id);

			if(STRING_VALID((const char *)sqlite3_column_text(stmt, 28)))
				_media->video_meta->title = strdup((const char *)sqlite3_column_text(stmt, 28));

			if(STRING_VALID((const char *)sqlite3_column_text(stmt, 29)))
				_media->video_meta->album = strdup((const char *)sqlite3_column_text(stmt, 29));

			if(STRING_VALID((const char *)sqlite3_column_text(stmt, 30)))
				_media->video_meta->artist = strdup((const char *)sqlite3_column_text(stmt, 30));

			if(STRING_VALID((const char *)sqlite3_column_text(stmt, 31)))
				_media->video_meta->genre = strdup((const char *)sqlite3_column_text(stmt, 31));

			if(STRING_VALID((const char *)sqlite3_column_text(stmt, 32)))
				_media->video_meta->composer = strdup((const char *)sqlite3_column_text(stmt, 32));

			if(STRING_VALID((const char *)sqlite3_column_text(stmt, 33)))
				_media->video_meta->year = strdup((const char *)sqlite3_column_text(stmt, 33));

			if(STRING_VALID((const char *)sqlite3_column_text(stmt, 34)))
				_media->video_meta->recorded_date = strdup((const char *)sqlite3_column_text(stmt, 34));

			if(STRING_VALID((const char *)sqlite3_column_text(stmt, 35)))
				_media->video_meta->copyright = strdup((const char *)sqlite3_column_text(stmt, 35));

			if(STRING_VALID((const char *)sqlite3_column_text(stmt, 36)))
				_media->video_meta->track_num = strdup((const char *)sqlite3_column_text(stmt, 36));

			_media->video_meta->bitrate = sqlite3_column_int(stmt, 37);
			_media->video_meta->duration = sqlite3_column_int(stmt, 38);
			_media->video_meta->played_count = sqlite3_column_int(stmt, 39);
			_media->video_meta->played_time = sqlite3_column_int(stmt, 40);
			_media->video_meta->played_position = sqlite3_column_int(stmt, 41);
			_media->video_meta->width = sqlite3_column_int(stmt, 24);
			_media->video_meta->height = sqlite3_column_int(stmt, 25);
		}

	}
	else if((_media->media_type == MEDIA_CONTENT_TYPE_MUSIC) || (_media->media_type == MEDIA_CONTENT_TYPE_SOUND)) {
		_media->audio_meta = (audio_meta_s *)calloc(1, sizeof(audio_meta_s));
		if(_media->audio_meta) {
			if(STRING_VALID(_media->media_id))
				_media->audio_meta->media_id = strdup(_media->media_id);

			if(STRING_VALID((const char *)sqlite3_column_text(stmt, 28)))
				_media->audio_meta->title = strdup((const char *)sqlite3_column_text(stmt, 28));

			if(STRING_VALID((const char *)sqlite3_column_text(stmt, 29)))
				_media->audio_meta->album = strdup((const char *)sqlite3_column_text(stmt, 29));

			if(STRING_VALID((const char *)sqlite3_column_text(stmt, 30)))
				_media->audio_meta->artist = strdup((const char *)sqlite3_column_text(stmt, 30));

			if(STRING_VALID((const char *)sqlite3_column_text(stmt, 31)))
				_media->audio_meta->genre = strdup((const char *)sqlite3_column_text(stmt, 31));

			if(STRING_VALID((const char *)sqlite3_column_text(stmt, 32)))
				_media->audio_meta->composer = strdup((const char *)sqlite3_column_text(stmt, 32));

			if(STRING_VALID((const char *)sqlite3_column_text(stmt, 33)))
				_media->audio_meta->year = strdup((const char *)sqlite3_column_text(stmt, 33));

			if(STRING_VALID((const char *)sqlite3_column_text(stmt, 34)))
				_media->audio_meta->recorded_date = strdup((const char *)sqlite3_column_text(stmt, 34));

			if(STRING_VALID((const char *)sqlite3_column_text(stmt, 35)))
				_media->audio_meta->copyright = strdup((const char *)sqlite3_column_text(stmt, 35));

			if(STRING_VALID((const char *)sqlite3_column_text(stmt, 36)))
				_media->audio_meta->track_num = strdup((const char *)sqlite3_column_text(stmt, 36));

			_media->audio_meta->bitrate = sqlite3_column_int(stmt, 37);
			_media->audio_meta->duration = sqlite3_column_int(stmt, 38);
			_media->audio_meta->played_count = sqlite3_column_int(stmt, 39);
			_media->audio_meta->played_time = sqlite3_column_int(stmt, 40);
			_media->audio_meta->played_position = sqlite3_column_int(stmt, 41);
			_media->audio_meta->samplerate = sqlite3_column_int(stmt, 42);
			_media->audio_meta->channel = sqlite3_column_int(stmt, 43);
		}

	}
}

int media_info_insert_to_db (const char *path, media_info_h *info)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;

	if(!STRING_VALID(path))
	{
		media_content_error("INVALID_PARAMETER(0x%08x)", MEDIA_CONTENT_ERROR_INVALID_PARAMETER);
		return MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
	}

	if(info == NULL)
	{
		media_content_error("INVALID_PARAMETER(0x%08x)", MEDIA_CONTENT_ERROR_INVALID_PARAMETER);
		return MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
	}

	media_content_debug("Register: %s", path);
	ret = media_file_register(path);
	if(ret != MS_MEDIA_ERR_NONE)
	{
		media_content_error("media_file_register failed");
		return _content_error_capi(MEDIA_REGISTER_TYPE, ret);
	}

	media_info_s *_media = (media_info_s*)calloc(1, sizeof(media_info_s));
	if(_media == NULL)
	{
		media_content_error("OUT_OF_MEMORY(0x%08x)", MEDIA_CONTENT_ERROR_OUT_OF_MEMORY);
		return MEDIA_CONTENT_ERROR_OUT_OF_MEMORY;
	}

	_media->file_path = strdup(path);
	if(_media->file_path == NULL)
	{
		media_content_error("OUT_OF_MEMORY(0x%08x)", MEDIA_CONTENT_ERROR_OUT_OF_MEMORY);
		SAFE_FREE(_media);
		return MEDIA_CONTENT_ERROR_OUT_OF_MEMORY;
	}

	ret = __media_info_get_media_info_from_db(_media->file_path, (media_info_h)_media);

	*info = (media_info_h)_media;

	return ret;
}

int media_info_delete_from_db(const char *media_id)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;

	char *query_string = NULL;

	if(!STRING_VALID(media_id))
	{
		media_content_error("INVALID_PARAMETER(0x%08x)", MEDIA_CONTENT_ERROR_INVALID_PARAMETER);
		return MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
	}

	query_string = sqlite3_mprintf(DELETE_MEDIA_FROM_MEDIA, media_id);

	ret = _content_query_sql(query_string);

	sqlite3_free(query_string);

	return ret;
}

int media_info_destroy(media_info_h media)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	media_info_s *_media = (media_info_s*)media;

	if(_media)
	{
		SAFE_FREE(_media->media_id);
		SAFE_FREE(_media->file_path);
		SAFE_FREE(_media->display_name);
		SAFE_FREE(_media->mime_type);
		SAFE_FREE(_media->thumbnail_path);
		SAFE_FREE(_media->description);
		SAFE_FREE(_media->author);
		SAFE_FREE(_media->provider);
		SAFE_FREE(_media->content_name);
		SAFE_FREE(_media->category);
		SAFE_FREE(_media->location_tag);
		SAFE_FREE(_media->age_rating);
		SAFE_FREE(_media->keyword);

		if(_media->image_meta) {
			SAFE_FREE(_media->image_meta->media_id);
			SAFE_FREE(_media->image_meta->date_taken);

			SAFE_FREE(_media->image_meta);
		} else if(_media->video_meta) {
			SAFE_FREE(_media->video_meta->media_id);
			SAFE_FREE(_media->video_meta->title);
			SAFE_FREE(_media->video_meta->album);
			SAFE_FREE(_media->video_meta->artist);
			SAFE_FREE(_media->video_meta->genre);
			SAFE_FREE(_media->video_meta->composer);
			SAFE_FREE(_media->video_meta->year);
			SAFE_FREE(_media->video_meta->recorded_date);
			SAFE_FREE(_media->video_meta->copyright);
			SAFE_FREE(_media->video_meta->track_num);

			SAFE_FREE(_media->video_meta);
		} else if(_media->audio_meta) {
			SAFE_FREE(_media->audio_meta->media_id);
			SAFE_FREE(_media->audio_meta->title);
			SAFE_FREE(_media->audio_meta->album);
			SAFE_FREE(_media->audio_meta->artist);
			SAFE_FREE(_media->audio_meta->genre);
			SAFE_FREE(_media->audio_meta->composer);
			SAFE_FREE(_media->audio_meta->year);
			SAFE_FREE(_media->audio_meta->recorded_date);
			SAFE_FREE(_media->audio_meta->copyright);
			SAFE_FREE(_media->audio_meta->track_num);

			SAFE_FREE(_media->audio_meta);
		}

		SAFE_FREE(_media);

		ret = MEDIA_CONTENT_ERROR_NONE;
	}
	else
	{
		media_content_error("INVALID_PARAMETER(0x%08x)", MEDIA_CONTENT_ERROR_INVALID_PARAMETER);
		ret = MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
	}

	return ret;
}

int media_info_clone(media_info_h *dst, media_info_h src)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	media_info_s *_src = (media_info_s*)src;


	if(_src != NULL)
	{
		media_info_s *_dst = (media_info_s*)calloc(1, sizeof(media_info_s));

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
				media_content_error("OUT_OF_MEMORY(0x%08x)", MEDIA_CONTENT_ERROR_OUT_OF_MEMORY);
				media_info_destroy((media_info_h)_dst);
				return MEDIA_CONTENT_ERROR_OUT_OF_MEMORY;
			}
		}
		if(STRING_VALID(_src->file_path))
		{
			_dst->file_path = strdup(_src->file_path);
			if(_dst->file_path == NULL)
			{
				media_content_error("OUT_OF_MEMORY(0x%08x)", MEDIA_CONTENT_ERROR_OUT_OF_MEMORY);
				media_info_destroy((media_info_h)_dst);
				return MEDIA_CONTENT_ERROR_OUT_OF_MEMORY;
			}
		}
		if(STRING_VALID(_src->display_name))
		{
			_dst->display_name = strdup(_src->display_name);
			if(_dst->display_name == NULL)
			{
				media_content_error("OUT_OF_MEMORY(0x%08x)", MEDIA_CONTENT_ERROR_OUT_OF_MEMORY);
				media_info_destroy((media_info_h)_dst);
				return MEDIA_CONTENT_ERROR_OUT_OF_MEMORY;
			}
		}
		if(STRING_VALID(_src->mime_type))
		{
			_dst->mime_type = strdup(_src->mime_type);
			if(_dst->mime_type == NULL)
			{
				media_content_error("OUT_OF_MEMORY(0x%08x)", MEDIA_CONTENT_ERROR_OUT_OF_MEMORY);
				media_info_destroy((media_info_h)_dst);
				return MEDIA_CONTENT_ERROR_OUT_OF_MEMORY;
			}
		}
		if(STRING_VALID(_src->thumbnail_path))
		{
			_dst->thumbnail_path = strdup(_src->thumbnail_path);
			if(_dst->thumbnail_path == NULL)
			{
				media_content_error("OUT_OF_MEMORY(0x%08x)", MEDIA_CONTENT_ERROR_OUT_OF_MEMORY);
				media_info_destroy((media_info_h)_dst);
				return MEDIA_CONTENT_ERROR_OUT_OF_MEMORY;
			}
		}
		if(STRING_VALID(_src->description))
		{
			_dst->description = strdup(_src->description);
			if(_dst->description == NULL)
			{
				media_content_error("OUT_OF_MEMORY(0x%08x)", MEDIA_CONTENT_ERROR_OUT_OF_MEMORY);
				media_info_destroy((media_info_h)_dst);
				return MEDIA_CONTENT_ERROR_OUT_OF_MEMORY;
			}
		}
		if(STRING_VALID(_src->author))
		{
			_dst->author = strdup(_src->author);
			if(_dst->author == NULL)
			{
				media_content_error("OUT_OF_MEMORY(0x%08x)", MEDIA_CONTENT_ERROR_OUT_OF_MEMORY);
				media_info_destroy((media_info_h)_dst);
				return MEDIA_CONTENT_ERROR_OUT_OF_MEMORY;
			}
		}
		if(STRING_VALID(_src->provider))
		{
			_dst->provider = strdup(_src->provider);
			if(_dst->provider == NULL)
			{
				media_content_error("OUT_OF_MEMORY(0x%08x)", MEDIA_CONTENT_ERROR_OUT_OF_MEMORY);
				media_info_destroy((media_info_h)_dst);
				return MEDIA_CONTENT_ERROR_OUT_OF_MEMORY;
			}
		}
		if(STRING_VALID(_src->content_name))
		{
			_dst->content_name = strdup(_src->content_name);
			if(_dst->content_name == NULL)
			{
				media_content_error("OUT_OF_MEMORY(0x%08x)", MEDIA_CONTENT_ERROR_OUT_OF_MEMORY);
				media_info_destroy((media_info_h)_dst);
				return MEDIA_CONTENT_ERROR_OUT_OF_MEMORY;
			}
		}
		if(STRING_VALID(_src->category))
		{
			_dst->category = strdup(_src->category);
			if(_dst->category == NULL)
			{
				media_content_error("OUT_OF_MEMORY(0x%08x)", MEDIA_CONTENT_ERROR_OUT_OF_MEMORY);
				media_info_destroy((media_info_h)_dst);
				return MEDIA_CONTENT_ERROR_OUT_OF_MEMORY;
			}
		}
		if(STRING_VALID(_src->location_tag))
		{
			_dst->location_tag = strdup(_src->location_tag);
			if(_dst->location_tag == NULL)
			{
				media_content_error("OUT_OF_MEMORY(0x%08x)", MEDIA_CONTENT_ERROR_OUT_OF_MEMORY);
				media_info_destroy((media_info_h)_dst);
				return MEDIA_CONTENT_ERROR_OUT_OF_MEMORY;
			}
		}
		if(STRING_VALID(_src->age_rating))
		{
			_dst->age_rating = strdup(_src->age_rating);
			if(_dst->age_rating == NULL)
			{
				media_content_error("OUT_OF_MEMORY(0x%08x)", MEDIA_CONTENT_ERROR_OUT_OF_MEMORY);
				media_info_destroy((media_info_h)_dst);
				return MEDIA_CONTENT_ERROR_OUT_OF_MEMORY;
			}
		}
		if(STRING_VALID(_src->keyword))
		{
			_dst->keyword = strdup(_src->keyword);
			if(_dst->keyword == NULL)
			{
				media_content_error("OUT_OF_MEMORY(0x%08x)", MEDIA_CONTENT_ERROR_OUT_OF_MEMORY);
				media_info_destroy((media_info_h)_dst);
				return MEDIA_CONTENT_ERROR_OUT_OF_MEMORY;
			}
		}

		_dst->media_type = _src->media_type;
		_dst->size = _src->size;
		_dst->added_time = _src->added_time;
		_dst->modified_time = _src->modified_time;
		_dst->longitude = _src->longitude;
		_dst->latitude = _src->latitude;
		_dst->altitude = _src->altitude;
		_dst->rating = _src->rating;
		_dst->favourite = _src->favourite;
		_dst->is_drm = _src->is_drm;

		if(_src->media_type == MEDIA_CONTENT_TYPE_IMAGE && _src->image_meta) {
			_dst->image_meta = (image_meta_s *)calloc(1, sizeof(image_meta_s));
			if(_dst->image_meta == NULL)
			{
				media_content_error("OUT_OF_MEMORY(0x%08x)", MEDIA_CONTENT_ERROR_OUT_OF_MEMORY);
				media_info_destroy((media_info_h)_dst);
				return MEDIA_CONTENT_ERROR_OUT_OF_MEMORY;
			}

			if(STRING_VALID(_src->image_meta->media_id))
			{
				_dst->image_meta->media_id = strdup(_src->image_meta->media_id);
				if(_dst->image_meta->media_id == NULL)
				{
					media_content_error("OUT_OF_MEMORY(0x%08x)", MEDIA_CONTENT_ERROR_OUT_OF_MEMORY);
					media_info_destroy((media_info_h)_dst);
					return MEDIA_CONTENT_ERROR_OUT_OF_MEMORY;
				}
			}
			if(STRING_VALID(_src->image_meta->date_taken))
			{
				_dst->image_meta->date_taken = strdup(_src->image_meta->date_taken);
				if(_dst->image_meta->date_taken == NULL)
				{
					media_content_error("OUT_OF_MEMORY(0x%08x)", MEDIA_CONTENT_ERROR_OUT_OF_MEMORY);
					media_info_destroy((media_info_h)_dst);
					return MEDIA_CONTENT_ERROR_OUT_OF_MEMORY;
				}
			}

			_dst->image_meta->width = _src->image_meta->width;
			_dst->image_meta->height = _src->image_meta->height;
			_dst->image_meta->orientation = _src->image_meta->orientation;

		} else if(_src->media_type == MEDIA_CONTENT_TYPE_VIDEO && _src->video_meta) {
			_dst->video_meta = (video_meta_s *)calloc(1, sizeof(video_meta_s));
			if(_dst->video_meta == NULL)
			{
				media_content_error("OUT_OF_MEMORY(0x%08x)", MEDIA_CONTENT_ERROR_OUT_OF_MEMORY);
				media_info_destroy((media_info_h)_dst);
				return MEDIA_CONTENT_ERROR_OUT_OF_MEMORY;
			}

			if(STRING_VALID(_src->video_meta->media_id))
			{
				_dst->video_meta->media_id = strdup(_src->video_meta->media_id);
				if(_dst->video_meta->media_id == NULL)
				{
					media_content_error("OUT_OF_MEMORY(0x%08x)", MEDIA_CONTENT_ERROR_OUT_OF_MEMORY);
					media_info_destroy((media_info_h)_dst);
					return MEDIA_CONTENT_ERROR_OUT_OF_MEMORY;
				}
			}
			if(STRING_VALID(_src->video_meta->title))
			{
				_dst->video_meta->title = strdup(_src->video_meta->title);
				if(_dst->video_meta->title == NULL)
				{
					media_content_error("OUT_OF_MEMORY(0x%08x)", MEDIA_CONTENT_ERROR_OUT_OF_MEMORY);
					media_info_destroy((media_info_h)_dst);
					return MEDIA_CONTENT_ERROR_OUT_OF_MEMORY;
				}
			}
			if(STRING_VALID(_src->video_meta->album))
			{
				_dst->video_meta->album = strdup(_src->video_meta->album);
				if(_dst->video_meta->album == NULL)
				{
					media_content_error("OUT_OF_MEMORY(0x%08x)", MEDIA_CONTENT_ERROR_OUT_OF_MEMORY);
					media_info_destroy((media_info_h)_dst);
					return MEDIA_CONTENT_ERROR_OUT_OF_MEMORY;
				}
			}
			if(STRING_VALID(_src->video_meta->artist))
			{
				_dst->video_meta->artist = strdup(_src->video_meta->artist);
				if(_dst->video_meta->artist == NULL)
				{
					media_content_error("OUT_OF_MEMORY(0x%08x)", MEDIA_CONTENT_ERROR_OUT_OF_MEMORY);
					media_info_destroy((media_info_h)_dst);
					return MEDIA_CONTENT_ERROR_OUT_OF_MEMORY;
				}
			}
			if(STRING_VALID(_src->video_meta->genre))
			{
				_dst->video_meta->genre = strdup(_src->video_meta->genre);
				if(_dst->video_meta->genre == NULL)
				{
					media_content_error("OUT_OF_MEMORY(0x%08x)", MEDIA_CONTENT_ERROR_OUT_OF_MEMORY);
					media_info_destroy((media_info_h)_dst);
					return MEDIA_CONTENT_ERROR_OUT_OF_MEMORY;
				}
			}
			if(STRING_VALID(_src->video_meta->composer))
			{
				_dst->video_meta->composer = strdup(_src->video_meta->composer);
				if(_dst->video_meta->composer == NULL)
				{
					media_content_error("OUT_OF_MEMORY(0x%08x)", MEDIA_CONTENT_ERROR_OUT_OF_MEMORY);
					media_info_destroy((media_info_h)_dst);
					return MEDIA_CONTENT_ERROR_OUT_OF_MEMORY;
				}
			}
			if(STRING_VALID(_src->video_meta->year))
			{
				_dst->video_meta->year = strdup(_src->video_meta->year);
				if(_dst->video_meta->year == NULL)
				{
					media_content_error("OUT_OF_MEMORY(0x%08x)", MEDIA_CONTENT_ERROR_OUT_OF_MEMORY);
					media_info_destroy((media_info_h)_dst);
					return MEDIA_CONTENT_ERROR_OUT_OF_MEMORY;
				}
			}
			if(STRING_VALID(_src->video_meta->recorded_date))
			{
				_dst->video_meta->recorded_date = strdup(_src->video_meta->recorded_date);
				if(_dst->video_meta->recorded_date == NULL)
				{
					media_content_error("OUT_OF_MEMORY(0x%08x)", MEDIA_CONTENT_ERROR_OUT_OF_MEMORY);
					media_info_destroy((media_info_h)_dst);
					return MEDIA_CONTENT_ERROR_OUT_OF_MEMORY;
				}
			}
			if(STRING_VALID(_src->video_meta->copyright))
			{
				_dst->video_meta->copyright = strdup(_src->video_meta->copyright);
				if(_dst->video_meta->copyright == NULL)
				{
					media_content_error("OUT_OF_MEMORY(0x%08x)", MEDIA_CONTENT_ERROR_OUT_OF_MEMORY);
					media_info_destroy((media_info_h)_dst);
					return MEDIA_CONTENT_ERROR_OUT_OF_MEMORY;
				}
			}
			if(STRING_VALID(_src->video_meta->track_num))
			{
				_dst->video_meta->track_num = strdup(_src->video_meta->track_num);
				if(_dst->video_meta->year == NULL)
				{
					media_content_error("OUT_OF_MEMORY(0x%08x)", MEDIA_CONTENT_ERROR_OUT_OF_MEMORY);
					media_info_destroy((media_info_h)_dst);
					return MEDIA_CONTENT_ERROR_OUT_OF_MEMORY;
				}
			}

			_dst->video_meta->width = _src->video_meta->width;
			_dst->video_meta->height = _src->video_meta->height;
			_dst->video_meta->duration = _src->video_meta->duration;
			_dst->video_meta->bitrate = _src->video_meta->bitrate;
			_dst->video_meta->played_count = _src->video_meta->played_count;
			_dst->video_meta->played_time = _src->video_meta->played_time;
			_dst->video_meta->played_position = _src->video_meta->played_position;

		} else if((_src->media_type == MEDIA_CONTENT_TYPE_MUSIC || _src->media_type == MEDIA_CONTENT_TYPE_SOUND) && _src->audio_meta) {
			_dst->audio_meta = (audio_meta_s *)calloc(1, sizeof(audio_meta_s));
			if(_dst->audio_meta == NULL)
			{
				media_content_error("OUT_OF_MEMORY(0x%08x)", MEDIA_CONTENT_ERROR_OUT_OF_MEMORY);
				media_info_destroy((media_info_h)_dst);
				return MEDIA_CONTENT_ERROR_OUT_OF_MEMORY;
			}

			if(STRING_VALID(_src->audio_meta->media_id))
			{
				_dst->audio_meta->media_id = strdup(_src->audio_meta->media_id);
				if(_dst->audio_meta->media_id == NULL)
				{
					media_content_error("OUT_OF_MEMORY(0x%08x)", MEDIA_CONTENT_ERROR_OUT_OF_MEMORY);
					media_info_destroy((media_info_h)_dst);
					return MEDIA_CONTENT_ERROR_OUT_OF_MEMORY;
				}
			}
			if(STRING_VALID(_src->audio_meta->title))
			{
				_dst->audio_meta->title = strdup(_src->audio_meta->title);
				if(_dst->audio_meta->title == NULL)
				{
					media_content_error("OUT_OF_MEMORY(0x%08x)", MEDIA_CONTENT_ERROR_OUT_OF_MEMORY);
					media_info_destroy((media_info_h)_dst);
					return MEDIA_CONTENT_ERROR_OUT_OF_MEMORY;
				}
			}
			if(STRING_VALID(_src->audio_meta->album))
			{
				_dst->audio_meta->album = strdup(_src->audio_meta->album);
				if(_dst->audio_meta->album == NULL)
				{
					media_content_error("OUT_OF_MEMORY(0x%08x)", MEDIA_CONTENT_ERROR_OUT_OF_MEMORY);
					media_info_destroy((media_info_h)_dst);
					return MEDIA_CONTENT_ERROR_OUT_OF_MEMORY;
				}
			}
			if(STRING_VALID(_src->audio_meta->artist))
			{
				_dst->audio_meta->artist = strdup(_src->audio_meta->artist);
				if(_dst->audio_meta->artist == NULL)
				{
					media_content_error("OUT_OF_MEMORY(0x%08x)", MEDIA_CONTENT_ERROR_OUT_OF_MEMORY);
					media_info_destroy((media_info_h)_dst);
					return MEDIA_CONTENT_ERROR_OUT_OF_MEMORY;
				}
			}
			if(STRING_VALID(_src->audio_meta->genre))
			{
				_dst->audio_meta->genre = strdup(_src->audio_meta->genre);
				if(_dst->audio_meta->genre == NULL)
				{
					media_content_error("OUT_OF_MEMORY(0x%08x)", MEDIA_CONTENT_ERROR_OUT_OF_MEMORY);
					media_info_destroy((media_info_h)_dst);
					return MEDIA_CONTENT_ERROR_OUT_OF_MEMORY;
				}
			}
			if(STRING_VALID(_src->audio_meta->composer))
			{
				_dst->audio_meta->composer = strdup(_src->audio_meta->composer);
				if(_dst->audio_meta->composer == NULL)
				{
					media_content_error("OUT_OF_MEMORY(0x%08x)", MEDIA_CONTENT_ERROR_OUT_OF_MEMORY);
					media_info_destroy((media_info_h)_dst);
					return MEDIA_CONTENT_ERROR_OUT_OF_MEMORY;
				}
			}
			if(STRING_VALID(_src->audio_meta->year))
			{
				_dst->audio_meta->year = strdup(_src->audio_meta->year);
				if(_dst->audio_meta->year == NULL)
				{
					media_content_error("OUT_OF_MEMORY(0x%08x)", MEDIA_CONTENT_ERROR_OUT_OF_MEMORY);
					media_info_destroy((media_info_h)_dst);
					return MEDIA_CONTENT_ERROR_OUT_OF_MEMORY;
				}
			}
			if(STRING_VALID(_src->audio_meta->recorded_date))
			{
				_dst->audio_meta->recorded_date = strdup(_src->audio_meta->recorded_date);
				if(_dst->audio_meta->recorded_date == NULL)
				{
					media_content_error("OUT_OF_MEMORY(0x%08x)", MEDIA_CONTENT_ERROR_OUT_OF_MEMORY);
					media_info_destroy((media_info_h)_dst);
					return MEDIA_CONTENT_ERROR_OUT_OF_MEMORY;
				}
			}
			if(STRING_VALID(_src->audio_meta->copyright))
			{
				_dst->audio_meta->copyright = strdup(_src->audio_meta->copyright);
				if(_dst->audio_meta->copyright == NULL)
				{
					media_content_error("OUT_OF_MEMORY(0x%08x)", MEDIA_CONTENT_ERROR_OUT_OF_MEMORY);
					media_info_destroy((media_info_h)_dst);
					return MEDIA_CONTENT_ERROR_OUT_OF_MEMORY;
				}
			}
			if(STRING_VALID(_src->audio_meta->track_num))
			{
				_dst->audio_meta->track_num = strdup(_src->audio_meta->track_num);
				if(_dst->audio_meta->year == NULL)
				{
					media_content_error("OUT_OF_MEMORY(0x%08x)", MEDIA_CONTENT_ERROR_OUT_OF_MEMORY);
					media_info_destroy((media_info_h)_dst);
					return MEDIA_CONTENT_ERROR_OUT_OF_MEMORY;
				}
			}

			_dst->audio_meta->channel = _src->audio_meta->channel;
			_dst->audio_meta->samplerate = _src->audio_meta->samplerate;
			_dst->audio_meta->duration = _src->audio_meta->duration;
			_dst->audio_meta->bitrate = _src->audio_meta->bitrate;
			_dst->audio_meta->played_count = _src->audio_meta->played_count;
			_dst->audio_meta->played_time = _src->audio_meta->played_time;
			_dst->audio_meta->played_position = _src->audio_meta->played_position;

		}
		*dst = (media_info_h)_dst;

		ret = MEDIA_CONTENT_ERROR_NONE;
	}
	else
	{
		media_content_error("INVALID_PARAMETER(0x%08x)", MEDIA_CONTENT_ERROR_INVALID_PARAMETER);
		ret = MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
	}

	return ret;
}

int media_info_get_media_count_from_db(filter_h filter, int *media_count)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;

	media_content_debug_func();

	if(media_count == NULL)
	{
		media_content_error("INVALID_PARAMETER(0x%08x)", MEDIA_CONTENT_ERROR_INVALID_PARAMETER);
		ret = MEDIA_CONTENT_ERROR_INVALID_PARAMETER;

		return ret;
	}

	ret = _media_db_get_group_item_count(NULL, filter, MEDIA_GROUP_NONE, media_count);

	return ret;
}

int media_info_foreach_media_from_db(filter_h filter, media_info_cb callback, void *user_data)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;

	media_content_debug_func();

	if(callback == NULL)
	{
		media_content_error("INVALID_PARAMETER(0x%08x)", MEDIA_CONTENT_ERROR_INVALID_PARAMETER);
		return MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
	}

	ret = _media_db_get_group_item(NULL, filter, callback, user_data, MEDIA_GROUP_NONE);

	return ret;
}

int media_info_get_tag_count_from_db(const char *media_id, filter_h filter, int *tag_count)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;

	media_content_debug_func();

	if(STRING_VALID(media_id) && tag_count)
	{
		ret = _media_db_get_group_item_count(media_id, filter, MEDIA_GROUP_TAG_BY_MEDIA_ID, tag_count);
	}
	else
	{
		media_content_error("INVALID_PARAMETER(0x%08x)", MEDIA_CONTENT_ERROR_INVALID_PARAMETER);
		return MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
	}

	return ret;
}

int media_info_foreach_tag_from_db(const char *media_id, filter_h filter, media_tag_cb callback, void *user_data)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;

	media_content_debug_func();

	if((callback != NULL) && STRING_VALID(media_id))
	{
		ret = _media_db_get_tag(media_id, filter, callback, user_data);
	}
	else
	{
		media_content_error("INVALID_PARAMETER(0x%08x)", MEDIA_CONTENT_ERROR_INVALID_PARAMETER);
		ret = MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
	}

	return ret;
}

int media_info_get_bookmark_count_from_db(const char *media_id, filter_h filter, int *bookmark_count)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;

	media_content_debug_func();

	if(STRING_VALID(media_id) && bookmark_count)
	{
		ret = _media_db_get_group_item_count(media_id, filter, MEDIA_GROUP_BOOKMARK_BY_MEDIA_ID, bookmark_count);
	}
	else
	{
		media_content_error("INVALID_PARAMETER(0x%08x)", MEDIA_CONTENT_ERROR_INVALID_PARAMETER);
		ret = MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
	}

	return ret;
}

int media_info_foreach_bookmark_from_db (const char *media_id, filter_h filter, media_bookmark_cb callback, void *user_data)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;

	media_content_debug_func();

	if((callback != NULL) && STRING_VALID(media_id))
	{
		ret = _media_db_get_bookmark(media_id, filter, callback, user_data);
	}
	else
	{
		media_content_error("INVALID_PARAMETER(0x%08x)", MEDIA_CONTENT_ERROR_INVALID_PARAMETER);
		ret = MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
	}

	return ret;
}

int media_info_get_image(media_info_h media, image_meta_h *image)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;

	media_info_s *_media = (media_info_s*)media;
	media_content_debug_func();

	if(_media == NULL)
	{
		media_content_error("INVALID_PARAMETER(0x%08x)", MEDIA_CONTENT_ERROR_INVALID_PARAMETER);
		return MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
	}

	if(_media->media_type != MEDIA_CONTENT_TYPE_IMAGE)
	{
		media_content_error("INVALID_PARAMETER(0x%08x)", MEDIA_CONTENT_ERROR_INVALID_PARAMETER);
		return MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
	}

	if(_media->image_meta == NULL)
	{
		media_content_error("INVALID_PARAMETER(0x%08x)", MEDIA_CONTENT_ERROR_INVALID_PARAMETER);
		return MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
	}

	image_meta_s *_image = (image_meta_s*)calloc(1, sizeof(image_meta_s));

	if(_image == NULL)
	{
		media_content_error("OUT_OF_MEMORY(0x%08x)", MEDIA_CONTENT_ERROR_OUT_OF_MEMORY);
		return MEDIA_CONTENT_ERROR_OUT_OF_MEMORY;
	}

	if(STRING_VALID(_media->media_id)) {
		_image->media_id = strdup(_media->media_id);
	}

	_image->width = _media->image_meta->width;
	_image->height = _media->image_meta->height;
	_image->orientation = _media->image_meta->orientation;

	if(STRING_VALID(_media->image_meta->date_taken)) {
		_image->date_taken = strdup(_media->image_meta->date_taken);
	}

	*image = (image_meta_h)_image;

	return ret;
}

int media_info_get_video(media_info_h media, video_meta_h *video)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;

	media_info_s *_media = (media_info_s*)media;
	media_content_debug_func();

	if(_media == NULL)
	{
		media_content_error("INVALID_PARAMETER(0x%08x)", MEDIA_CONTENT_ERROR_INVALID_PARAMETER);
		return MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
	}

	if(_media->media_type != MEDIA_CONTENT_TYPE_VIDEO)
	{
		media_content_error("INVALID_PARAMETER(0x%08x)", MEDIA_CONTENT_ERROR_INVALID_PARAMETER);
		return MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
	}

	if(_media->video_meta == NULL)
	{
		media_content_error("INVALID_PARAMETER(0x%08x)", MEDIA_CONTENT_ERROR_INVALID_PARAMETER);
		return MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
	}

	video_meta_s *_video = (video_meta_s*)calloc(1, sizeof(video_meta_s));

	if(_video == NULL)
	{
		media_content_error("OUT_OF_MEMORY(0x%08x)", MEDIA_CONTENT_ERROR_OUT_OF_MEMORY);
		return MEDIA_CONTENT_ERROR_OUT_OF_MEMORY;
	}

	if(STRING_VALID(_media->media_id)) {
		_video->media_id = strdup(_media->media_id);
	}

	if(STRING_VALID(_media->video_meta->title)) {
		_video->title = strdup(_media->video_meta->title);
	}
	if(STRING_VALID(_media->video_meta->album)) {
		_video->album = strdup(_media->video_meta->album);
	}
	if(STRING_VALID(_media->video_meta->artist)) {
		_video->artist = strdup(_media->video_meta->artist);
	}
	if(STRING_VALID(_media->video_meta->genre)) {
		_video->genre = strdup(_media->video_meta->genre);
	}
	if(STRING_VALID(_media->video_meta->composer)) {
		_video->composer = strdup(_media->video_meta->composer);
	}
	if(STRING_VALID(_media->video_meta->year)) {
		_video->year = strdup(_media->video_meta->year);
	}
	if(STRING_VALID(_media->video_meta->recorded_date)) {
		_video->recorded_date = strdup(_media->video_meta->recorded_date);
	}
	if(STRING_VALID(_media->video_meta->copyright)) {
		_video->copyright = strdup(_media->video_meta->copyright);
	}
	if(STRING_VALID(_media->video_meta->track_num)) {
		_video->track_num = strdup(_media->video_meta->track_num);
	}

	_video->width = _media->video_meta->width;
	_video->height = _media->video_meta->height;
	_video->duration = _media->video_meta->duration;
	_video->bitrate = _media->video_meta->bitrate;
	_video->played_count = _media->video_meta->played_count;
	_video->played_time = _media->video_meta->played_time;
	_video->played_position = _media->video_meta->played_position;

	*video = (video_meta_h)_video;

	return ret;
}

int media_info_get_audio(media_info_h media, audio_meta_h *audio)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;

	media_info_s *_media = (media_info_s*)media;
	media_content_debug_func();

	if(_media == NULL)
	{
		media_content_error("INVALID_PARAMETER(0x%08x)", MEDIA_CONTENT_ERROR_INVALID_PARAMETER);
		return MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
	}

	if(_media->media_type != MEDIA_CONTENT_TYPE_MUSIC && _media->media_type != MEDIA_CONTENT_TYPE_SOUND)
	{
		media_content_error("INVALID_PARAMETER(0x%08x)", MEDIA_CONTENT_ERROR_INVALID_PARAMETER);
		return MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
	}

	if(_media->audio_meta == NULL)
	{
		media_content_error("INVALID_PARAMETER(0x%08x)", MEDIA_CONTENT_ERROR_INVALID_PARAMETER);
		return MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
	}

	audio_meta_s *_audio = (audio_meta_s*)calloc(1, sizeof(audio_meta_s));

	if(_audio == NULL)
	{
		media_content_error("OUT_OF_MEMORY(0x%08x)", MEDIA_CONTENT_ERROR_OUT_OF_MEMORY);
		return MEDIA_CONTENT_ERROR_OUT_OF_MEMORY;
	}

	if(STRING_VALID(_media->media_id)) {
		_audio->media_id = strdup(_media->media_id);
	}

	if(STRING_VALID(_media->audio_meta->title)) {
		_audio->title = strdup(_media->audio_meta->title);
	}
	if(STRING_VALID(_media->audio_meta->album)) {
		_audio->album = strdup(_media->audio_meta->album);
	}
	if(STRING_VALID(_media->audio_meta->artist)) {
		_audio->artist = strdup(_media->audio_meta->artist);
	}
	if(STRING_VALID(_media->audio_meta->genre)) {
		_audio->genre = strdup(_media->audio_meta->genre);
	}
	if(STRING_VALID(_media->audio_meta->composer)) {
		_audio->composer = strdup(_media->audio_meta->composer);
	}
	if(STRING_VALID(_media->audio_meta->year)) {
		_audio->year = strdup(_media->audio_meta->year);
	}
	if(STRING_VALID(_media->audio_meta->recorded_date)) {
		_audio->recorded_date = strdup(_media->audio_meta->recorded_date);
	}
	if(STRING_VALID(_media->audio_meta->copyright)) {
		_audio->copyright = strdup(_media->audio_meta->copyright);
	}
	if(STRING_VALID(_media->audio_meta->track_num)) {
		_audio->track_num = strdup(_media->audio_meta->track_num);
	}

	_audio->duration = _media->audio_meta->duration;
	_audio->bitrate = _media->audio_meta->bitrate;
	_audio->samplerate = _media->audio_meta->samplerate;
	_audio->channel = _media->audio_meta->channel;
	_audio->played_time = _media->audio_meta->played_time;
	_audio->played_count = _media->audio_meta->played_count;
	_audio->played_position = _media->audio_meta->played_position;

	*audio = (audio_meta_h)_audio;

	return ret;
}

int media_info_get_media_id(media_info_h media, char **media_id)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	media_info_s *_media = (media_info_s*)media;

	if(_media && media_id)
	{
		if(STRING_VALID(_media->media_id))
		{
			*media_id = strdup(_media->media_id);
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

int media_info_get_file_path(media_info_h media, char **path)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	media_info_s *_media = (media_info_s*)media;

	if(_media && path)
	{
		if(STRING_VALID(_media->file_path))
		{
			*path = strdup(_media->file_path);
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

int media_info_get_display_name(media_info_h media, char **name)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	media_info_s *_media = (media_info_s*)media;
	if(_media && name)
	{
		if(STRING_VALID(_media->display_name))
		{
			*name = strdup(_media->display_name);
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

int media_info_get_media_type(media_info_h media, media_content_type_e *type)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	media_info_s *_media = (media_info_s*)media;
	if(_media && type)
	{
		*type = _media->media_type;
		ret = MEDIA_CONTENT_ERROR_NONE;
	}
	else
	{
		media_content_error("INVALID_PARAMETER(0x%08x)", MEDIA_CONTENT_ERROR_INVALID_PARAMETER);
		ret = MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
	}

	return ret;
}

int media_info_get_mime_type(media_info_h media, char **mime_type)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	media_info_s *_media = (media_info_s*)media;

	if(_media && mime_type)
	{
		if(STRING_VALID(_media->mime_type))
		{
			*mime_type = strdup(_media->mime_type);
			if(*mime_type == NULL)
			{
				media_content_error("OUT_OF_MEMORY(0x%08x)", MEDIA_CONTENT_ERROR_OUT_OF_MEMORY);
				return MEDIA_CONTENT_ERROR_OUT_OF_MEMORY;
			}
		}
		else
		{
			*mime_type = NULL;
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

int media_info_get_size(media_info_h media, unsigned long long *size)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	media_info_s *_media = (media_info_s*)media;

	if(_media && size)
	{
		*size = _media->size;
		ret = MEDIA_CONTENT_ERROR_NONE;
	}
	else
	{
		media_content_error("INVALID_PARAMETER(0x%08x)", MEDIA_CONTENT_ERROR_INVALID_PARAMETER);
		ret = MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
	}

	return ret;
}

int media_info_get_added_time(media_info_h media, time_t *added_time)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	media_info_s *_media = (media_info_s*)media;
	if(_media && added_time)
	{
		*added_time = _media->added_time;
		ret = MEDIA_CONTENT_ERROR_NONE;
	}
	else
	{
		media_content_error("INVALID_PARAMETER(0x%08x)", MEDIA_CONTENT_ERROR_INVALID_PARAMETER);
		ret = MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
	}

	return ret;
}

int media_info_get_modified_time(media_info_h media, time_t* time)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	media_info_s *_media = (media_info_s*)media;
	if(_media && time)
	{
		*time = _media->modified_time;
		ret = MEDIA_CONTENT_ERROR_NONE;
	}
	else
	{
		media_content_error("INVALID_PARAMETER(0x%08x)", MEDIA_CONTENT_ERROR_INVALID_PARAMETER);
		ret = MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
	}

	return ret;
}

int media_info_get_thumbnail_path(media_info_h media, char **path)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	media_info_s *_media = (media_info_s*)media;
	if(_media && path)
	{
		if(STRING_VALID(_media->thumbnail_path))
		{
			*path = strdup(_media->thumbnail_path);
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

int media_info_get_description(media_info_h media, char **description)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	media_info_s *_media = (media_info_s*)media;

	if(_media && description)
	{
		if(STRING_VALID(_media->description))
		{
			*description = strdup(_media->description);
			if(*description == NULL)
			{
				media_content_error("OUT_OF_MEMORY(0x%08x)", MEDIA_CONTENT_ERROR_OUT_OF_MEMORY);
				return MEDIA_CONTENT_ERROR_OUT_OF_MEMORY;
			}
		}
		else
		{
			*description = NULL;
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

int media_info_get_longitude(media_info_h media, double* longitude)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	media_info_s *_media = (media_info_s*)media;

	if(_media && longitude)
	{
		*longitude = _media->longitude;
		ret = MEDIA_CONTENT_ERROR_NONE;
	}
	else
	{
		media_content_error("INVALID_PARAMETER(0x%08x)", MEDIA_CONTENT_ERROR_INVALID_PARAMETER);
		ret = MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
	}

	return ret;
}
int media_info_get_latitude(media_info_h media, double* latitude)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	media_info_s *_media = (media_info_s*)media;

	if(_media && latitude)
	{
		*latitude = _media->latitude;
		ret = MEDIA_CONTENT_ERROR_NONE;
	}
	else
	{
		media_content_error("INVALID_PARAMETER(0x%08x)", MEDIA_CONTENT_ERROR_INVALID_PARAMETER);
		ret = MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
	}

	return ret;
}

int media_info_get_altitude(media_info_h media, double *altitude)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	media_info_s *_media = (media_info_s*)media;

	if(_media && altitude)
	{
		*altitude = _media->altitude;
		ret = MEDIA_CONTENT_ERROR_NONE;
	}
	else
	{
		media_content_error("INVALID_PARAMETER(0x%08x)", MEDIA_CONTENT_ERROR_INVALID_PARAMETER);
		ret = MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
	}

	return ret;
}

int media_info_get_rating(media_info_h media, int *rating)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	media_info_s *_media = (media_info_s*)media;

	if(_media && rating)
	{
		*rating = _media->rating;
		ret = MEDIA_CONTENT_ERROR_NONE;
	}
	else
	{
		media_content_error("INVALID_PARAMETER(0x%08x)", MEDIA_CONTENT_ERROR_INVALID_PARAMETER);
		ret = MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
	}

	return ret;
}

int media_info_get_favorite(media_info_h media, bool* favorite)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	media_info_s *_media = (media_info_s*)media;

	if(_media && favorite)
	{

		*favorite = _media->favourite;
		ret = MEDIA_CONTENT_ERROR_NONE;
	}
	else
	{
		media_content_error("INVALID_PARAMETER(0x%08x)", MEDIA_CONTENT_ERROR_INVALID_PARAMETER);
		ret = MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
	}

	return ret;
}

int media_info_get_author(media_info_h media, char **author)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	media_info_s *_media = (media_info_s*)media;

	if(_media && author)
	{
		if(STRING_VALID(_media->author))
		{
			*author = strdup(_media->author);
			if(*author == NULL)
			{
				media_content_error("OUT_OF_MEMORY(0x%08x)", MEDIA_CONTENT_ERROR_OUT_OF_MEMORY);
				return MEDIA_CONTENT_ERROR_OUT_OF_MEMORY;
			}
		}
		else
		{
			*author = NULL;
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

int media_info_get_provider(media_info_h media, char **provider)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	media_info_s *_media = (media_info_s*)media;

	if(_media && provider)
	{
		if(STRING_VALID(_media->provider))
		{
			*provider = strdup(_media->provider);
			if(*provider == NULL)
			{
				media_content_error("OUT_OF_MEMORY(0x%08x)", MEDIA_CONTENT_ERROR_OUT_OF_MEMORY);
				return MEDIA_CONTENT_ERROR_OUT_OF_MEMORY;
			}
		}
		else
		{
			*provider = NULL;
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

int media_info_get_content_name(media_info_h media, char **content_name)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	media_info_s *_media = (media_info_s*)media;

	if(_media && content_name)
	{
		if(STRING_VALID(_media->content_name))
		{
			*content_name = strdup(_media->content_name);
			if(*content_name == NULL)
			{
				media_content_error("OUT_OF_MEMORY(0x%08x)", MEDIA_CONTENT_ERROR_OUT_OF_MEMORY);
				return MEDIA_CONTENT_ERROR_OUT_OF_MEMORY;
			}
		}
		else
		{
			*content_name = NULL;
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

int media_info_get_category(media_info_h media, char **category)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	media_info_s *_media = (media_info_s*)media;

	if(_media && category)
	{
		if(STRING_VALID(_media->category))
		{
			*category = strdup(_media->category);
			if(*category == NULL)
			{
				media_content_error("OUT_OF_MEMORY(0x%08x)", MEDIA_CONTENT_ERROR_OUT_OF_MEMORY);
				return MEDIA_CONTENT_ERROR_OUT_OF_MEMORY;
			}
		}
		else
		{
			*category = NULL;
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

int media_info_get_location_tag(media_info_h media, char **location_tag)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	media_info_s *_media = (media_info_s*)media;

	if(_media && location_tag)
	{
		if(STRING_VALID(_media->location_tag))
		{
			*location_tag = strdup(_media->location_tag);
			if(*location_tag == NULL)
			{
				media_content_error("OUT_OF_MEMORY(0x%08x)", MEDIA_CONTENT_ERROR_OUT_OF_MEMORY);
				return MEDIA_CONTENT_ERROR_OUT_OF_MEMORY;
			}
		}
		else
		{
			*location_tag = NULL;
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

int media_info_get_age_rating(media_info_h media, char **age_rating)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	media_info_s *_media = (media_info_s*)media;

	if(_media && age_rating)
	{
		if(STRING_VALID(_media->age_rating))
		{
			*age_rating = strdup(_media->age_rating);
			if(*age_rating == NULL)
			{
				media_content_error("OUT_OF_MEMORY(0x%08x)", MEDIA_CONTENT_ERROR_OUT_OF_MEMORY);
				return MEDIA_CONTENT_ERROR_OUT_OF_MEMORY;
			}
		}
		else
		{
			*age_rating = NULL;
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

int media_info_get_keyword(media_info_h media, char **keyword)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	media_info_s *_media = (media_info_s*)media;

	if(_media && keyword)
	{
		if(STRING_VALID(_media->keyword))
		{
			*keyword = strdup(_media->keyword);
			if(*keyword == NULL)
			{
				media_content_error("OUT_OF_MEMORY(0x%08x)", MEDIA_CONTENT_ERROR_OUT_OF_MEMORY);
				return MEDIA_CONTENT_ERROR_OUT_OF_MEMORY;
			}
		}
		else
		{
			*keyword = NULL;
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

int media_info_is_drm(media_info_h media, bool *is_drm)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	media_info_s *_media = (media_info_s*)media;

	if(_media)
	{
		*is_drm = _media->is_drm;
		ret = MEDIA_CONTENT_ERROR_NONE;
	}
	else
	{
		media_content_error("INVALID_PARAMETER(0x%08x)", MEDIA_CONTENT_ERROR_INVALID_PARAMETER);
		ret = MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
	}

	return ret;
}

int media_info_get_storage_type(media_info_h media, media_content_storage_e *storage_type)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	media_info_s *_media = (media_info_s*)media;

	if(_media && storage_type)
	{
		*storage_type = _media->storage_type;
		ret = MEDIA_CONTENT_ERROR_NONE;
	}
	else
	{
		media_content_error("INVALID_PARAMETER(0x%08x)", MEDIA_CONTENT_ERROR_INVALID_PARAMETER);
		ret = MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
	}

	return ret;
}

int media_info_get_media_from_db(const char *media_id, media_info_h *media)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	char select_query[DEFAULT_QUERY_SIZE];
	sqlite3_stmt *stmt = NULL;

	if(!STRING_VALID(media_id) || (media == NULL))
	{
		media_content_error("INVALID_PARAMETER(0x%08x)", MEDIA_CONTENT_ERROR_INVALID_PARAMETER);
		return MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
	}

	memset(select_query, 0x00, sizeof(select_query));

	snprintf(select_query, sizeof(select_query), SELECT_MEDIA_FROM_MEDIA, media_id);
	ret = _content_query_prepare(&stmt, select_query, NULL, NULL);
	media_content_retv_if(ret != MEDIA_CONTENT_ERROR_NONE, ret);

	while(sqlite3_step(stmt) == SQLITE_ROW)
	{
		media_info_s *_media = (media_info_s*)calloc(1, sizeof(media_info_s));

		if(_media == NULL)
		{
			media_content_error("OUT_OF_MEMORY(0x%08x)", MEDIA_CONTENT_ERROR_OUT_OF_MEMORY);
			SQLITE3_FINALIZE(stmt);
			return MEDIA_CONTENT_ERROR_OUT_OF_MEMORY;
		}

		_media_info_item_get_detail(stmt, (media_info_h)_media);

		*media = (media_info_h)_media;
	}

	SQLITE3_FINALIZE(stmt);

	return ret;
}

int media_info_set_display_name(media_info_h media, const char *display_name)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	media_info_s *_media = (media_info_s*)media;

	if(_media != NULL && STRING_VALID(display_name))
	{
		SAFE_FREE(_media->display_name);

		_media->display_name = strdup(display_name);
		if(_media->display_name == NULL)
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

int media_info_set_description(media_info_h media, const char *description)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	media_info_s *_media = (media_info_s*)media;

	if(_media != NULL)
	{
		SAFE_FREE(_media->description);

		if(STRING_VALID(description))
		{
			_media->description = strdup(description);

			if(_media->description == NULL)
			{
				media_content_error("OUT_OF_MEMORY(0x%08x)", MEDIA_CONTENT_ERROR_OUT_OF_MEMORY);
				return MEDIA_CONTENT_ERROR_OUT_OF_MEMORY;
			}
		}
		else
		{
			_media->description = NULL;
		}
	}
	else
	{
		media_content_error("INVALID_PARAMETER(0x%08x)", MEDIA_CONTENT_ERROR_INVALID_PARAMETER);
		ret = MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
	}

	return ret;
}

int media_info_set_longitude(media_info_h media, double longitude)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	media_info_s *_media = (media_info_s*)media;

	if(_media != NULL)
	{
		_media->longitude = longitude;
	}
	else
	{
		media_content_error("INVALID_PARAMETER(0x%08x)", MEDIA_CONTENT_ERROR_INVALID_PARAMETER);
		ret = MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
	}

	return ret;
}

int media_info_set_latitude(media_info_h media, double latitude)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	media_info_s *_media = (media_info_s*)media;

	if(_media != NULL)
	{
		_media->latitude = latitude;
	}
	else
	{
		media_content_error("INVALID_PARAMETER(0x%08x)", MEDIA_CONTENT_ERROR_INVALID_PARAMETER);
		ret = MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
	}

	return ret;
}

int media_info_set_altitude(media_info_h media, double altitude)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	media_info_s *_media = (media_info_s*)media;

	if(_media != NULL)
	{
		_media->altitude = altitude;
	}
	else
	{
		media_content_error("INVALID_PARAMETER(0x%08x)", MEDIA_CONTENT_ERROR_INVALID_PARAMETER);
		ret = MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
	}

	return ret;
}

int media_info_set_rating(media_info_h media, int rating)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	media_info_s *_media = (media_info_s*)media;

	if(_media != NULL)
	{
		_media->rating = rating;
	}
	else
	{
		media_content_error("INVALID_PARAMETER(0x%08x)", MEDIA_CONTENT_ERROR_INVALID_PARAMETER);
		ret = MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
	}

	return ret;
}

int media_info_set_added_time(media_info_h media, time_t added_time)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;

	media_info_s *_media = (media_info_s*)media;

	if((_media != NULL) && (added_time >= 0))
	{
		_media->added_time = added_time;
	}
	else
	{
		media_content_error("INVALID_PARAMETER(0x%08x)", MEDIA_CONTENT_ERROR_INVALID_PARAMETER);
		ret = MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
	}

	return ret;
}

int media_info_set_favorite(media_info_h media, bool favorite)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	media_info_s *_media = (media_info_s*)media;

	if(_media != NULL)
	{
		_media->favourite = favorite;
	}
	else
	{
		media_content_error("INVALID_PARAMETER(0x%08x)", MEDIA_CONTENT_ERROR_INVALID_PARAMETER);
		ret = MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
	}

	return ret;
}

int media_info_set_author(media_info_h media, const char *author)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	media_info_s *_media = (media_info_s*)media;

	if(_media != NULL)
	{
		if(STRING_VALID(author))
		{
			_media->author = strdup(author);
			if(_media->author == NULL)
			{
				media_content_error("OUT_OF_MEMORY(0x%08x)", MEDIA_CONTENT_ERROR_OUT_OF_MEMORY);
				return MEDIA_CONTENT_ERROR_OUT_OF_MEMORY;
			}
		}
		else
		{
			_media->author = NULL;
		}
	}
	else
	{
		media_content_error("INVALID_PARAMETER(0x%08x)", MEDIA_CONTENT_ERROR_INVALID_PARAMETER);
		ret = MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
	}

	return ret;
}

int media_info_set_provider(media_info_h media, const char *provider)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	media_info_s *_media = (media_info_s*)media;

	if(_media != NULL)
	{
		SAFE_FREE(_media->provider);

		if(STRING_VALID(provider))
		{
			_media->provider = strdup(provider);
			if(_media->provider == NULL)
			{
				media_content_error("OUT_OF_MEMORY(0x%08x)", MEDIA_CONTENT_ERROR_OUT_OF_MEMORY);
				return MEDIA_CONTENT_ERROR_OUT_OF_MEMORY;
			}
		}
		else
		{
			_media->provider = NULL;
		}
	}
	else
	{
		media_content_error("INVALID_PARAMETER(0x%08x)", MEDIA_CONTENT_ERROR_INVALID_PARAMETER);
		ret = MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
	}

	return ret;
}

int media_info_set_content_name(media_info_h media, const char *content_name)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	media_info_s *_media = (media_info_s*)media;

	if(_media != NULL)
	{
		SAFE_FREE(_media->content_name);

		if(STRING_VALID(content_name))
		{
			_media->content_name = strdup(content_name);
			if(_media->content_name == NULL)
			{
				media_content_error("OUT_OF_MEMORY(0x%08x)", MEDIA_CONTENT_ERROR_OUT_OF_MEMORY);
				return MEDIA_CONTENT_ERROR_OUT_OF_MEMORY;
			}
		}
		else
		{
			_media->content_name = NULL;
		}
	}
	else
	{
		media_content_error("INVALID_PARAMETER(0x%08x)", MEDIA_CONTENT_ERROR_INVALID_PARAMETER);
		ret = MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
	}

	return ret;
}

int media_info_set_category(media_info_h media, const char *category)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	media_info_s *_media = (media_info_s*)media;

	if(_media != NULL)
	{
		SAFE_FREE(_media->category);

		if(STRING_VALID(category))
		{
			_media->category = strdup(category);
			if(_media->category == NULL)
			{
				media_content_error("OUT_OF_MEMORY(0x%08x)", MEDIA_CONTENT_ERROR_OUT_OF_MEMORY);
				return MEDIA_CONTENT_ERROR_OUT_OF_MEMORY;
			}
		}
		else
		{
			_media->category = NULL;
		}
	}
	else
	{
		media_content_error("INVALID_PARAMETER(0x%08x)", MEDIA_CONTENT_ERROR_INVALID_PARAMETER);
		ret = MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
	}

	return ret;
}

int media_info_set_location_tag(media_info_h media, const char *location_tag)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	media_info_s *_media = (media_info_s*)media;

	if(_media != NULL)
	{
		SAFE_FREE(_media->location_tag);

		if(STRING_VALID(location_tag))
		{
			_media->location_tag = strdup(location_tag);
			if(_media->location_tag == NULL)
			{
				media_content_error("OUT_OF_MEMORY(0x%08x)", MEDIA_CONTENT_ERROR_OUT_OF_MEMORY);
				return MEDIA_CONTENT_ERROR_OUT_OF_MEMORY;
			}
		}
		else
		{
			_media->location_tag = NULL;
		}
	}
	else
	{
		media_content_error("INVALID_PARAMETER(0x%08x)", MEDIA_CONTENT_ERROR_INVALID_PARAMETER);
		ret = MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
	}

	return ret;
}

int media_info_set_age_rating(media_info_h media, const char *age_rating)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	media_info_s *_media = (media_info_s*)media;

	if(_media != NULL)
	{
		SAFE_FREE(_media->age_rating);

		if(STRING_VALID(age_rating))
		{
			_media->age_rating = strdup(age_rating);
			if(_media->age_rating == NULL)
			{
				media_content_error("OUT_OF_MEMORY(0x%08x)", MEDIA_CONTENT_ERROR_OUT_OF_MEMORY);
				return MEDIA_CONTENT_ERROR_OUT_OF_MEMORY;
			}
		}
		else
		{
			_media->age_rating = NULL;
		}
	}
	else
	{
		media_content_error("INVALID_PARAMETER(0x%08x)", MEDIA_CONTENT_ERROR_INVALID_PARAMETER);
		ret = MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
	}

	return ret;
}

int media_info_set_keyword(media_info_h media, const char *keyword)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	media_info_s *_media = (media_info_s*)media;

	if(_media != NULL)
	{
		SAFE_FREE(_media->keyword);

		if(STRING_VALID(keyword))
		{
			_media->keyword = strdup(keyword);
			if(_media->keyword == NULL)
			{
				media_content_error("OUT_OF_MEMORY(0x%08x)", MEDIA_CONTENT_ERROR_OUT_OF_MEMORY);
				return MEDIA_CONTENT_ERROR_OUT_OF_MEMORY;
			}
		}
		else
		{
			_media->keyword = NULL;
		}
	}
	else
	{
		media_content_error("INVALID_PARAMETER(0x%08x)", MEDIA_CONTENT_ERROR_INVALID_PARAMETER);
		ret = MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
	}

	return ret;
}

int media_info_update_to_db(media_info_h media)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	int len = 0;
	media_info_s *_media = (media_info_s*)media;
	char *set_sql = NULL;
	char sql[MAX_QUERY_SIZE];
	memset(sql, '\0', sizeof(sql));

	if(_media != NULL && STRING_VALID(_media->media_id))
	{
		set_sql = sqlite3_mprintf("file_name=%Q, added_time=%d, description=%Q, longitude=%f, latitude=%f, altitude=%f, \
			rating=%d, favourite=%d, author=%Q, provider=%Q, content_name=%Q, category=%Q, location_tag=%Q, age_rating=%Q, keyword=%Q",
			_media->display_name, _media->added_time, _media->description, _media->longitude, _media->latitude, _media->altitude, _media->rating, _media->favourite,
			_media->author, _media->provider, _media->content_name, _media->category, _media->location_tag, _media->age_rating, _media->keyword);

		len = snprintf(sql, sizeof(sql), "UPDATE %s SET %s WHERE media_uuid='%s'", DB_TABLE_MEDIA, set_sql, _media->media_id);
		sqlite3_free(set_sql);
		if (len > 0) {
			sql[len] = '\0';
		} else {
			media_content_error("snprintf failed");
			return MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
		}

		ret = _content_query_sql(sql);
	}
	else
	{
		media_content_error("INVALID_PARAMETER(0x%08x)", MEDIA_CONTENT_ERROR_INVALID_PARAMETER);
		ret = MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
	}

	return ret;
}

int media_info_refresh_metadata_to_db(const char *media_id)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	media_info_h media;
	char *file_path = NULL;
	media_content_type_e media_type = 0;
	media_content_storage_e storage_type = 0;

	if(!STRING_VALID(media_id))
	{
		media_content_error("INVALID_PARAMETER(0x%08x)", MEDIA_CONTENT_ERROR_INVALID_PARAMETER);
		return MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
	}

	ret = media_info_get_media_from_db(media_id, &media);
	media_content_retv_if(ret != MEDIA_CONTENT_ERROR_NONE, ret);

	ret = media_info_get_media_type(media, &media_type);
	if(ret != MEDIA_CONTENT_ERROR_NONE)
	{
		media_info_destroy(media);
		return ret;
	}

	ret = media_info_get_storage_type(media, &storage_type);
	if(ret != MEDIA_CONTENT_ERROR_NONE)
	{
		media_info_destroy(media);
		return ret;
	}

	ret = media_info_get_file_path(media, &file_path);
	if(ret != MEDIA_CONTENT_ERROR_NONE)
	{
		media_info_destroy(media);
		return ret;
	}

	ret = media_svc_refresh_item(_content_get_db_handle(), storage_type, file_path, media_type);

	SAFE_FREE(file_path);
	media_info_destroy(media);

	return ret;
}

int media_info_move_to_db(media_info_h media, const char* dst_path)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;

	int src_storage_type = 0;
	int dst_storage_type = 0;

	if(media == NULL || !STRING_VALID(dst_path))
	{
		media_content_error("INVALID_PARAMETER(0x%08x)", MEDIA_CONTENT_ERROR_INVALID_PARAMETER);
		return MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
	}

	media_info_s *_media = (media_info_s*)media;

	ret = _media_util_get_store_type_by_path(_media->file_path, &src_storage_type);
	media_content_retv_if(ret != MEDIA_CONTENT_ERROR_NONE, ret);
	ret = _media_util_get_store_type_by_path(dst_path, &dst_storage_type);
	media_content_retv_if(ret != MEDIA_CONTENT_ERROR_NONE, ret);

	ret = media_svc_move_item(_content_get_db_handle(), src_storage_type, _media->file_path, dst_storage_type, dst_path);
	return _content_error_capi(MEDIA_CONTENT_TYPE, ret);
}

int media_info_move_media_to_db(media_info_h media, const char* dst_path)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;

	int src_storage_type = 0;
	int dst_storage_type = 0;

	if(media == NULL || !STRING_VALID(dst_path))
	{
		media_content_error("INVALID_PARAMETER(0x%08x)", MEDIA_CONTENT_ERROR_INVALID_PARAMETER);
		return MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
	}

	media_info_s *_media = (media_info_s*)media;

	ret = _media_util_get_store_type_by_path(_media->file_path, &src_storage_type);
	media_content_retv_if(ret != MEDIA_CONTENT_ERROR_NONE, ret);
	ret = _media_util_get_store_type_by_path(dst_path, &dst_storage_type);
	media_content_retv_if(ret != MEDIA_CONTENT_ERROR_NONE, ret);

	ret = media_svc_move_item(_content_get_db_handle(), src_storage_type, _media->file_path, dst_storage_type, dst_path);
	return _content_error_capi(MEDIA_CONTENT_TYPE, ret);
}

int media_info_create_thumbnail(media_info_h media, media_thumbnail_completed_cb callback, void *user_data)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	media_info_s *_media = (media_info_s*)media;

	if(_media != NULL && STRING_VALID(_media->media_id) && STRING_VALID(_media->file_path))
	{
		media_thumbnail_cb_s *_thumb_cb = (media_thumbnail_cb_s*)calloc(1, sizeof(media_thumbnail_cb_s));
		_thumb_cb->handle = _media;
		_thumb_cb->user_data = user_data;
		_thumb_cb->thumbnail_completed_cb = callback;

		ret = thumbnail_request_from_db_async(_media->file_path, (ThumbFunc)__media_info_thumbnail_completed_cb, (void *)_thumb_cb);
		_content_error_capi(MEDIA_THUMBNAIL_TYPE, ret);
	}
	else
	{
		media_content_error("INVALID_PARAMETER(0x%08x)", MEDIA_CONTENT_ERROR_INVALID_PARAMETER);
		ret = MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
	}

	return ret;
}
