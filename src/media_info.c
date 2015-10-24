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

#include <media_info.h>
#include <media-thumbnail.h>
#include <media_info_private.h>
#include <media_util_private.h>

static void __media_info_insert_completed_cb(media_request_result_s *result, void *user_data);
static void __media_info_thumbnail_completed_cb(int error, const char *path, void *user_data);
static bool __media_info_delete_batch_cb(media_info_h media, void *user_data);
static int __media_info_insert_batch(media_batch_insert_e insert_type, const char **path_array, unsigned int array_length, media_insert_completed_cb completed_cb, void *user_data);

static int __media_info_get_media_path_by_id_from_db(const char *media_id, char **path)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	sqlite3_stmt *stmt = NULL;
	char *select_query = NULL;

	media_content_retvm_if(!STRING_VALID(media_id), MEDIA_CONTENT_ERROR_INVALID_PARAMETER, "invalid media_id");

	select_query = sqlite3_mprintf(SELECT_MEDIA_PATH_BY_ID, media_id);

	ret = _content_query_prepare(&stmt, select_query, NULL, NULL);
	SQLITE3_SAFE_FREE(select_query);
	media_content_retv_if(ret != MEDIA_CONTENT_ERROR_NONE, ret);

	if(sqlite3_step(stmt) == SQLITE_ROW)
	{
		if(STRING_VALID((const char *)sqlite3_column_text(stmt, 0)))
			*path = strdup((const char *)sqlite3_column_text(stmt, 0));
	} else {
		media_content_error("There's no media with this ID : %s", media_id);
		*path = NULL;
		ret = MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
	}

	SQLITE3_FINALIZE(stmt);

	return ret;
}

static void __media_info_insert_completed_cb(media_request_result_s *result, void *user_data)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	media_insert_cb_s *_cb_data = (media_insert_cb_s *)user_data;

	if (_cb_data) {
		if (result) {
			ret = _content_error_capi(MEDIA_CONTENT_TYPE, result->result);
		}

		if (_cb_data->insert_completed_cb)
			_cb_data->insert_completed_cb(ret, _cb_data->user_data);

		if (STRING_VALID(_cb_data->insert_list_path)) {
			if (unlink(_cb_data->insert_list_path) < 0) {
				media_content_stderror("failed to delete");
			}
			SAFE_FREE(_cb_data->insert_list_path);
		}
	}

	SAFE_FREE(_cb_data);

	return;
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
		if (_thumb_cb->thumbnail_completed_cb)
			_thumb_cb->thumbnail_completed_cb(error_value, path, _thumb_cb->user_data);
	}

	SAFE_FREE(_thumb_cb);

	return;
}

static bool __media_info_delete_batch_cb(media_info_h media, void *user_data)
{
	int err = MEDIA_CONTENT_ERROR_NONE;
	char *thumb_path = NULL;
	media_content_type_e media_type = 0;
	GArray *thumb_list = (GArray *)user_data;

	if(media == NULL)
	{
		media_content_debug("NO Item");
		return true;
	}

	err = media_info_get_media_type(media, &media_type);
	if (err == MEDIA_CONTENT_ERROR_NONE) {
		media_content_debug("media_type : [%d]", media_type);

		media_info_get_thumbnail_path(media, &thumb_path);
		if (STRING_VALID(thumb_path)) {
			if (strncmp(MEDIA_CONTENT_THUMB_DEFAULT_PATH, thumb_path, strlen(MEDIA_CONTENT_THUMB_DEFAULT_PATH)) != 0) {
				g_array_append_val(thumb_list, thumb_path);
			}
		}
	} else {
		media_content_error("media_info_get_media_type failed");
	}

	return true;
}

static int __media_info_insert_batch(media_batch_insert_e insert_type, const char **path_array,
					unsigned int array_length,
					media_insert_completed_cb completed_cb,
					void *user_data)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	FILE *fp = NULL;
	char list_path[255] = {0,};
	unsigned int idx = 0;
	int nwrites = 0;

	for (idx = 0; idx < BATCH_REQUEST_MAX; idx++) {
		snprintf(list_path, sizeof(list_path), "%s/request-%ld-%d", MEDIA_CONTENT_INSERT_FILES_PATH, media_content_gettid(), idx);

		if (g_file_test(list_path, G_FILE_TEST_EXISTS | G_FILE_TEST_IS_REGULAR)) {
			memset(list_path, 0x00, sizeof(list_path));
			continue;
		} else {
			media_content_debug("The request file list path : %s", list_path);
			break;
		}
	}

	if (idx == BATCH_REQUEST_MAX) {
		media_content_error("Too many batch request for one thread");
		return MEDIA_CONTENT_ERROR_DB_BUSY;
	}

	fp = fopen(list_path, "w");
	if (fp == NULL) {
		media_content_error("failed to open file : [%s]", list_path);
		media_content_stderror("failed to open file");
		return MEDIA_CONTENT_ERROR_INVALID_OPERATION;
	}

	for (idx = 0; idx < array_length; idx++) {
		if (STRING_VALID(path_array[idx])) {
			int size = strlen(path_array[idx]);

			ret = _media_util_check_file_exist(path_array[idx]);
			if (ret != MEDIA_CONTENT_ERROR_NONE) {
				fclose(fp);
				if (unlink(list_path) < 0) {
					media_content_stderror("failed to delete");
				}
				return ret;
			}

			nwrites = fwrite(path_array[idx], 1, size, fp);
			if (nwrites != size) {
				media_content_stderror("failed to write");
				fclose(fp);
				if (unlink(list_path) < 0) {
					media_content_stderror("failed to delete");
				}
				return MEDIA_CONTENT_ERROR_INVALID_OPERATION;
			}

			nwrites = fwrite("\n", 1, 1, fp);
			if (nwrites != 1) {
				media_content_stderror("failed to write");
				fclose(fp);
				if (unlink(list_path) < 0) {
					media_content_stderror("failed to delete");
				}
				return MEDIA_CONTENT_ERROR_INVALID_OPERATION;
			}
		} else {
			media_content_error("path[%d] is invalid string", idx);
		}
	}

	fclose(fp);

	media_insert_cb_s *_cb_data = (media_insert_cb_s *)calloc(1, sizeof(media_insert_cb_s));
	media_content_retvm_if(_cb_data == NULL, MEDIA_CONTENT_ERROR_OUT_OF_MEMORY, "OUT_OF_MEMORY");

	_cb_data->insert_completed_cb = completed_cb;
	_cb_data->user_data = user_data;
	_cb_data->insert_list_path = strdup(list_path);

	if(insert_type == MEDIA_BATCH_INSERT_NORMAL)
		ret = media_files_register(list_path, __media_info_insert_completed_cb, _cb_data);
	else if(insert_type == MEDIA_BATCH_INSERT_BURSTSHOT)
		ret = media_burstshot_register(list_path, __media_info_insert_completed_cb, _cb_data);
	else
		ret = MEDIA_CONTENT_ERROR_INVALID_PARAMETER;

	if (ret != MEDIA_CONTENT_ERROR_NONE) {
		media_content_error("media_files_register failed : %d", ret);
		if (unlink(list_path) < 0) {
			media_content_stderror("failed to delete");
		}
		return _content_error_capi(MEDIA_CONTENT_TYPE, ret);
	}

	return ret;
}

void _media_info_item_get_detail(sqlite3_stmt* stmt, media_info_h media)
{
	media_info_s *_media = (media_info_s*)media;

	if(STRING_VALID((const char *)sqlite3_column_text(stmt, MEDIA_INFO_UUID)))
		_media->media_id = strdup((const char *)sqlite3_column_text(stmt, MEDIA_INFO_UUID));

	if(STRING_VALID((const char *)sqlite3_column_text(stmt, MEDIA_INFO_PATH)))
		_media->file_path = strdup((const char *)sqlite3_column_text(stmt, MEDIA_INFO_PATH));

	if(STRING_VALID((const char *)sqlite3_column_text(stmt, MEDIA_INFO_FILE_NAME)))
		_media->display_name = strdup((const char *)sqlite3_column_text(stmt, MEDIA_INFO_FILE_NAME));

	_media->media_type = (int)sqlite3_column_int(stmt, MEDIA_INFO_TYPE);

	if(STRING_VALID((const char *)sqlite3_column_text(stmt, MEDIA_INFO_MIME_TYPE)))
		_media->mime_type = strdup((const char *)sqlite3_column_text(stmt, MEDIA_INFO_MIME_TYPE));

	_media->size = (unsigned long long)sqlite3_column_int64(stmt, MEDIA_INFO_SIZE);

	_media->added_time = (int)sqlite3_column_int(stmt, MEDIA_INFO_ADDED_TIME);

	_media->modified_time = (int)sqlite3_column_int(stmt, MEDIA_INFO_MODIFIED_TIME);

	if(STRING_VALID((const char *)sqlite3_column_text(stmt, MEDIA_INFO_THUMBNAIL_PATH)))
		_media->thumbnail_path= strdup((const char *)sqlite3_column_text(stmt, MEDIA_INFO_THUMBNAIL_PATH));

	if(STRING_VALID((const char *)sqlite3_column_text(stmt, MEDIA_INFO_DESCRIPTION)))
		_media->description = strdup((const char *)sqlite3_column_text(stmt, MEDIA_INFO_DESCRIPTION));

	_media->rating = (int)sqlite3_column_int(stmt, MEDIA_INFO_RATING);

	_media->favourite = (int)sqlite3_column_int(stmt, MEDIA_INFO_FAVOURITE);

	if(STRING_VALID((const char *)sqlite3_column_text(stmt, MEDIA_INFO_AUTHOR)))
		_media->author = strdup((const char *)sqlite3_column_text(stmt, MEDIA_INFO_AUTHOR));

	if(STRING_VALID((const char *)sqlite3_column_text(stmt, MEDIA_INFO_PROVIDER)))
		_media->provider = strdup((const char *)sqlite3_column_text(stmt, MEDIA_INFO_PROVIDER));

	if(STRING_VALID((const char *)sqlite3_column_text(stmt, MEDIA_INFO_CONTENT_NAME)))
		_media->content_name = strdup((const char *)sqlite3_column_text(stmt, MEDIA_INFO_CONTENT_NAME));

	if(STRING_VALID((const char *)sqlite3_column_text(stmt, MEDIA_INFO_CATEGORY)))
		_media->category = strdup((const char *)sqlite3_column_text(stmt, MEDIA_INFO_CATEGORY));

	if(STRING_VALID((const char *)sqlite3_column_text(stmt, MEDIA_INFO_LOCATION_TAG)))
		_media->location_tag = strdup((const char *)sqlite3_column_text(stmt, MEDIA_INFO_LOCATION_TAG));

	if(STRING_VALID((const char *)sqlite3_column_text(stmt, MEDIA_INFO_AGE_RATING)))
		_media->age_rating = strdup((const char *)sqlite3_column_text(stmt, MEDIA_INFO_AGE_RATING));

	if(STRING_VALID((const char *)sqlite3_column_text(stmt, MEDIA_INFO_KEYWORD)))
		_media->keyword = strdup((const char *)sqlite3_column_text(stmt, MEDIA_INFO_KEYWORD));

	_media->is_drm = (int)sqlite3_column_int(stmt, MEDIA_INFO_IS_DRM);

	_media->storage_type = (int)sqlite3_column_int(stmt, MEDIA_INFO_STORAGE_TYPE);

	_media->longitude = (double)sqlite3_column_double(stmt, MEDIA_INFO_LONGITUDE);
	_media->latitude = (double)sqlite3_column_double(stmt, MEDIA_INFO_LATITUDE);
	_media->altitude = (double)sqlite3_column_double(stmt, MEDIA_INFO_ALTITUDE);

	_media->played_count = sqlite3_column_int(stmt, MEDIA_INFO_PLAYED_COUNT);
	_media->played_time = sqlite3_column_int(stmt, MEDIA_INFO_LAST_PLAYED_TIME);
	_media->played_position = sqlite3_column_int(stmt, MEDIA_INFO_LAST_PLAYED_POSITION);

	if(STRING_VALID((const char *)sqlite3_column_text(stmt, MEDIA_INFO_TITLE)))
		_media->title = strdup((const char *)sqlite3_column_text(stmt, MEDIA_INFO_TITLE));

	_media->timeline = (double)sqlite3_column_double(stmt, MEDIA_INFO_TIMELINE);

	if(STRING_VALID((const char *)sqlite3_column_text(stmt, MEDIA_INFO_WEATHER)))
		_media->weather = strdup((const char *)sqlite3_column_text(stmt, MEDIA_INFO_WEATHER));

	_media->sync_status = (int)sqlite3_column_int(stmt, MEDIA_INFO_SYNC_STATUS);

	if(STRING_VALID((const char *)sqlite3_column_text(stmt, MEDIA_INFO_STORAGE_UUID)))
		_media->storage_uuid = strdup((const char *)sqlite3_column_text(stmt, MEDIA_INFO_STORAGE_UUID));

	if(_media->media_type == MEDIA_CONTENT_TYPE_IMAGE) {
		_media->image_meta = (image_meta_s *)calloc(1, sizeof(image_meta_s));
		if(_media->image_meta) {
			if(STRING_VALID(_media->media_id))
				_media->image_meta->media_id = strdup(_media->media_id);

			_media->image_meta->width = sqlite3_column_int(stmt, MEDIA_INFO_WIDTH);
			_media->image_meta->height = sqlite3_column_int(stmt, MEDIA_INFO_HEIGHT);
			_media->image_meta->fnumber = (double)sqlite3_column_double(stmt, MEDIA_INFO_FNUMBER);
			_media->image_meta->iso = sqlite3_column_int(stmt, MEDIA_INFO_ISO);

			if(STRING_VALID((const char *)sqlite3_column_text(stmt, MEDIA_INFO_DATETAKEN)))
				_media->image_meta->date_taken = strdup((const char *)sqlite3_column_text(stmt, MEDIA_INFO_DATETAKEN));

			_media->image_meta->orientation = sqlite3_column_int(stmt, MEDIA_INFO_ORIENTATION);

			if(STRING_VALID((const char *)sqlite3_column_text(stmt, MEDIA_INFO_TITLE)))
				_media->image_meta->title = strdup((const char *)sqlite3_column_text(stmt, MEDIA_INFO_TITLE));

			if(STRING_VALID((const char *)sqlite3_column_text(stmt, MEDIA_INFO_BURST_ID)))
				_media->image_meta->burst_id = strdup((const char *)sqlite3_column_text(stmt, MEDIA_INFO_BURST_ID));

			if(STRING_VALID((const char *)sqlite3_column_text(stmt, MEDIA_INFO_WEATHER)))
				_media->image_meta->weather = strdup((const char *)sqlite3_column_text(stmt, MEDIA_INFO_WEATHER));

			if(STRING_VALID((const char *)sqlite3_column_text(stmt, MEDIA_INFO_EXPOSURE_TIME)))
				_media->image_meta->exposure_time = strdup((const char *)sqlite3_column_text(stmt, MEDIA_INFO_EXPOSURE_TIME));

			if(STRING_VALID((const char *)sqlite3_column_text(stmt, MEDIA_INFO_MODEL)))
				_media->image_meta->model = strdup((const char *)sqlite3_column_text(stmt, MEDIA_INFO_MODEL));
		}

	} else if(_media->media_type == MEDIA_CONTENT_TYPE_VIDEO) {
		_media->video_meta = (video_meta_s *)calloc(1, sizeof(video_meta_s));
		if(_media->video_meta) {
			if(STRING_VALID(_media->media_id))
				_media->video_meta->media_id = strdup(_media->media_id);

			_media->video_meta->width = sqlite3_column_int(stmt, MEDIA_INFO_WIDTH);
			_media->video_meta->height = sqlite3_column_int(stmt, MEDIA_INFO_HEIGHT);

			if(STRING_VALID((const char *)sqlite3_column_text(stmt, MEDIA_INFO_TITLE)))
				_media->video_meta->title = strdup((const char *)sqlite3_column_text(stmt, MEDIA_INFO_TITLE));

			if(STRING_VALID((const char *)sqlite3_column_text(stmt, MEDIA_INFO_ALBUM)))
				_media->video_meta->album = strdup((const char *)sqlite3_column_text(stmt, MEDIA_INFO_ALBUM));

			if(STRING_VALID((const char *)sqlite3_column_text(stmt, MEDIA_INFO_ARTIST)))
				_media->video_meta->artist = strdup((const char *)sqlite3_column_text(stmt, MEDIA_INFO_ARTIST));

			if(STRING_VALID((const char *)sqlite3_column_text(stmt, MEDIA_INFO_ALBUM_ARTIST)))
				_media->video_meta->album_artist = strdup((const char *)sqlite3_column_text(stmt, MEDIA_INFO_ALBUM_ARTIST));

			if(STRING_VALID((const char *)sqlite3_column_text(stmt, MEDIA_INFO_GENRE)))
				_media->video_meta->genre = strdup((const char *)sqlite3_column_text(stmt, MEDIA_INFO_GENRE));

			if(STRING_VALID((const char *)sqlite3_column_text(stmt, MEDIA_INFO_COMPOSER)))
				_media->video_meta->composer = strdup((const char *)sqlite3_column_text(stmt, MEDIA_INFO_COMPOSER));

			if(STRING_VALID((const char *)sqlite3_column_text(stmt, MEDIA_INFO_YEAR)))
				_media->video_meta->year = strdup((const char *)sqlite3_column_text(stmt, MEDIA_INFO_YEAR));

			if(STRING_VALID((const char *)sqlite3_column_text(stmt, MEDIA_INFO_RECORDED_DATE)))
				_media->video_meta->recorded_date = strdup((const char *)sqlite3_column_text(stmt, MEDIA_INFO_RECORDED_DATE));

			if(STRING_VALID((const char *)sqlite3_column_text(stmt, MEDIA_INFO_COPYRIGHT)))
				_media->video_meta->copyright = strdup((const char *)sqlite3_column_text(stmt, MEDIA_INFO_COPYRIGHT));

			if(STRING_VALID((const char *)sqlite3_column_text(stmt, MEDIA_INFO_TRACK_NUM)))
				_media->video_meta->track_num = strdup((const char *)sqlite3_column_text(stmt, MEDIA_INFO_TRACK_NUM));

			_media->video_meta->bitrate = sqlite3_column_int(stmt, MEDIA_INFO_BITRATE);
			_media->video_meta->duration = sqlite3_column_int(stmt, MEDIA_INFO_DURATION);
			_media->video_meta->played_count = sqlite3_column_int(stmt, MEDIA_INFO_PLAYED_COUNT);
			_media->video_meta->played_time = sqlite3_column_int(stmt, MEDIA_INFO_LAST_PLAYED_TIME);
			_media->video_meta->played_position = sqlite3_column_int(stmt, MEDIA_INFO_LAST_PLAYED_POSITION);
		}

	}
	else if((_media->media_type == MEDIA_CONTENT_TYPE_MUSIC) || (_media->media_type == MEDIA_CONTENT_TYPE_SOUND)) {
		_media->audio_meta = (audio_meta_s *)calloc(1, sizeof(audio_meta_s));
		if(_media->audio_meta) {
			if(STRING_VALID(_media->media_id))
				_media->audio_meta->media_id = strdup(_media->media_id);

			if(STRING_VALID((const char *)sqlite3_column_text(stmt, MEDIA_INFO_TITLE)))
				_media->audio_meta->title = strdup((const char *)sqlite3_column_text(stmt, MEDIA_INFO_TITLE));

			if(STRING_VALID((const char *)sqlite3_column_text(stmt, MEDIA_INFO_ALBUM)))
				_media->audio_meta->album = strdup((const char *)sqlite3_column_text(stmt, MEDIA_INFO_ALBUM));

			if(STRING_VALID((const char *)sqlite3_column_text(stmt, MEDIA_INFO_ARTIST)))
				_media->audio_meta->artist = strdup((const char *)sqlite3_column_text(stmt, MEDIA_INFO_ARTIST));

			if(STRING_VALID((const char *)sqlite3_column_text(stmt, MEDIA_INFO_ALBUM_ARTIST)))
				_media->audio_meta->album_artist = strdup((const char *)sqlite3_column_text(stmt, MEDIA_INFO_ALBUM_ARTIST));

			if(STRING_VALID((const char *)sqlite3_column_text(stmt, MEDIA_INFO_GENRE)))
				_media->audio_meta->genre = strdup((const char *)sqlite3_column_text(stmt, MEDIA_INFO_GENRE));

			if(STRING_VALID((const char *)sqlite3_column_text(stmt, MEDIA_INFO_COMPOSER)))
				_media->audio_meta->composer = strdup((const char *)sqlite3_column_text(stmt, MEDIA_INFO_COMPOSER));

			if(STRING_VALID((const char *)sqlite3_column_text(stmt, MEDIA_INFO_YEAR)))
				_media->audio_meta->year = strdup((const char *)sqlite3_column_text(stmt, MEDIA_INFO_YEAR));

			if(STRING_VALID((const char *)sqlite3_column_text(stmt, MEDIA_INFO_RECORDED_DATE)))
				_media->audio_meta->recorded_date = strdup((const char *)sqlite3_column_text(stmt, MEDIA_INFO_RECORDED_DATE));

			if(STRING_VALID((const char *)sqlite3_column_text(stmt, MEDIA_INFO_COPYRIGHT)))
				_media->audio_meta->copyright = strdup((const char *)sqlite3_column_text(stmt, MEDIA_INFO_COPYRIGHT));

			if(STRING_VALID((const char *)sqlite3_column_text(stmt, MEDIA_INFO_TRACK_NUM)))
				_media->audio_meta->track_num = strdup((const char *)sqlite3_column_text(stmt, MEDIA_INFO_TRACK_NUM));

			_media->audio_meta->bitrate = sqlite3_column_int(stmt, MEDIA_INFO_BITRATE);
			_media->audio_meta->bitpersample = sqlite3_column_int(stmt, MEDIA_INFO_BITPERSAMPLE);
			_media->audio_meta->duration = sqlite3_column_int(stmt, MEDIA_INFO_DURATION);
			_media->audio_meta->played_count = sqlite3_column_int(stmt, MEDIA_INFO_PLAYED_COUNT);
			_media->audio_meta->played_time = sqlite3_column_int(stmt, MEDIA_INFO_LAST_PLAYED_TIME);
			_media->audio_meta->played_position = sqlite3_column_int(stmt, MEDIA_INFO_LAST_PLAYED_POSITION);
			_media->audio_meta->samplerate = sqlite3_column_int(stmt, MEDIA_INFO_SAMPLERATE);
			_media->audio_meta->channel = sqlite3_column_int(stmt, MEDIA_INFO_CHANNEL);
		}
	}
}

int _media_info_get_media_info_from_db(const char *path, const char *storage_id, media_info_h media)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	sqlite3_stmt *stmt = NULL;
	char *select_query = NULL;
	media_info_s *_media = (media_info_s*)media;

	media_content_retvm_if(_media == NULL, MEDIA_CONTENT_ERROR_INVALID_PARAMETER, "invalid media");

	select_query = sqlite3_mprintf(SELECT_MEDIA_BY_PATH, storage_id, path);

	ret = _content_query_prepare(&stmt, select_query, NULL, NULL);
	SQLITE3_SAFE_FREE(select_query);
	media_content_retv_if(ret != MEDIA_CONTENT_ERROR_NONE, ret);

	if(sqlite3_step(stmt) == SQLITE_ROW)
	{
		_media_info_item_get_detail(stmt, (media_info_h)_media);
	} else {
		media_content_error("There's no media!!");
		ret = MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
	}

	SQLITE3_FINALIZE(stmt);

	return ret;
}

int media_info_insert_to_db(const char *path, media_info_h *info)
{
	bool ignore_file = FALSE;
	bool ignore_dir = FALSE;
	char *folder_path = NULL;
	char storage_id[MEDIA_CONTENT_UUID_SIZE+1] = {0,};
	int ret = MEDIA_CONTENT_ERROR_NONE;

	media_content_retvm_if(!STRING_VALID(path), MEDIA_CONTENT_ERROR_INVALID_PARAMETER, "invalid path");
	media_content_retvm_if(info == NULL, MEDIA_CONTENT_ERROR_INVALID_PARAMETER, "invalid info");

	ret = _media_util_check_ignore_file(path, &ignore_file);
	media_content_retvm_if(ignore_file == TRUE, MEDIA_CONTENT_ERROR_INVALID_PARAMETER, "Invalid path");

	ret = _media_util_check_file_exist(path);
	media_content_retv_if(ret != MEDIA_CONTENT_ERROR_NONE, ret);

	folder_path = g_path_get_dirname(path);
	ret = _media_util_check_ignore_dir(folder_path, &ignore_dir);
	SAFE_FREE(folder_path);

	media_content_retvm_if(ignore_dir == TRUE, MEDIA_CONTENT_ERROR_INVALID_PARAMETER, "Invalid folder path");

	memset(storage_id, 0x00, sizeof(storage_id));
	ret = media_svc_get_storage_id(_content_get_db_handle(), path, storage_id);
	if(ret != MS_MEDIA_ERR_NONE)
	{
		media_content_error("media_svc_get_storage_id failed : %d", ret);
		return _content_error_capi(MEDIA_CONTENT_TYPE, ret);
	}

	ret = media_svc_check_item_exist_by_path(_content_get_db_handle(), storage_id, path);
	if (ret == MS_MEDIA_ERR_DB_NO_RECORD) {
		media_content_sec_debug("media_svc_check_item_exist_by_path : no record : %s", path);

		media_svc_storage_type_e storage_type = 0;

		ret = media_svc_get_storage_type(path, &storage_type);
		if(ret != MS_MEDIA_ERR_NONE) {
			media_content_sec_error("media_svc_get_storage_type failed : %d", ret);
			return _content_error_capi(MEDIA_CONTENT_TYPE, ret);
		}

		ret = media_svc_insert_item_immediately(_content_get_db_handle(), storage_id, storage_type, path);

		if(ret != MS_MEDIA_ERR_NONE) {
			if (ret == MS_MEDIA_ERR_DB_CONSTRAINT_FAIL) {
				media_content_sec_error("This item is already inserted. This may be normal operation because other process already did this (%s)", path);
				ret = MEDIA_CONTENT_ERROR_NONE;
			} else {
				media_content_sec_error("media_svc_insert_item_immediately failed : %d (%s)", ret, path);
			}

			return _content_error_capi(MEDIA_CONTENT_TYPE, ret);
		}
	} else if (ret != MS_MEDIA_ERR_NONE) {
		media_content_sec_error("media_svc_check_item_exist_by_path failed : %d (%s)", ret, path);
		return _content_error_capi(MEDIA_CONTENT_TYPE, ret);
	}

	media_info_s *_media = (media_info_s*)calloc(1, sizeof(media_info_s));
	media_content_retvm_if(_media == NULL, MEDIA_CONTENT_ERROR_OUT_OF_MEMORY, "OUT_OF_MEMORY");

	ret = _media_info_get_media_info_from_db(path, storage_id, (media_info_h)_media);

	*info = (media_info_h)_media;

	return ret;
}

int media_info_insert_batch_to_db(
					const char **path_array,
					unsigned int array_length,
					media_insert_completed_cb completed_cb,
					void *user_data)
{
	media_content_retvm_if(path_array == NULL, MEDIA_CONTENT_ERROR_INVALID_PARAMETER, "Invalid path_array");
	media_content_retvm_if(array_length <= 0, MEDIA_CONTENT_ERROR_INVALID_PARAMETER, "Invalid array_length");

	return __media_info_insert_batch(MEDIA_BATCH_INSERT_NORMAL, path_array, array_length, completed_cb, user_data);
}

int media_info_insert_burst_shot_to_db(const char **path_array, unsigned int array_length, media_insert_burst_shot_completed_cb callback, void *user_data)
{
	media_content_retvm_if(path_array == NULL, MEDIA_CONTENT_ERROR_INVALID_PARAMETER, "Invalid path_array");
	media_content_retvm_if(array_length <= 0, MEDIA_CONTENT_ERROR_INVALID_PARAMETER, "Invalid array_length");

	return __media_info_insert_batch(MEDIA_BATCH_INSERT_BURSTSHOT, path_array, array_length, callback, user_data);
}

int media_info_delete_from_db(const char *media_id)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	char *path = NULL;
	char storage_id[MEDIA_CONTENT_UUID_SIZE+1] = {0,};

	if(!STRING_VALID(media_id))
	{
		media_content_error("INVALID_PARAMETER(0x%08x)", MEDIA_CONTENT_ERROR_INVALID_PARAMETER);
		return MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
	}

	memset(storage_id, 0x00, sizeof(storage_id));

	ret = __media_info_get_media_path_by_id_from_db(media_id, &path);
	if(ret != MEDIA_CONTENT_ERROR_NONE) {
		media_content_error("__media_info_get_media_path_by_id_from_db failed : %d", ret);
		SAFE_FREE(path);
		return ret;
	}

	ret = _media_db_get_storage_id_by_media_id(media_id, storage_id);
	if(ret != MEDIA_CONTENT_ERROR_NONE) {
		media_content_error("_media_db_get_storage_id_by_media_id failed : %d", ret);
		SAFE_FREE(path);
		return ret;
	}

	ret = media_svc_delete_item_by_path(_content_get_db_handle(), storage_id, path);
	SAFE_FREE(path);

	return _content_error_capi(MEDIA_CONTENT_TYPE, ret);
}

static int __media_info_delete_thumb_from_list(GArray *thumb_list)
{
	int i = 0;
	int list_len = 0;
	char *thumb_path = NULL;

	if (thumb_list != NULL) {

		list_len = thumb_list->len;

		for (i = 0; i < list_len; i ++) {
			thumb_path = g_array_index(thumb_list, char*, i);
			media_content_debug("thumb path [%s]", thumb_path);
			if (unlink(thumb_path) < 0) {
				media_content_stderror("failed to delete");
			}
		}
	}
	return MEDIA_CONTENT_ERROR_NONE;
}

static int __media_info_release_thumb_list(GArray *thumb_list)
{
	int i = 0;
	int list_len = 0;
	char *thumb_path = NULL;

	if (thumb_list != NULL) {

		list_len = thumb_list->len;

		for (i = 0; i < list_len; i ++) {
			thumb_path = g_array_index(thumb_list, char*, 0);
			g_array_remove_index(thumb_list,0);
			SAFE_FREE(thumb_path);
		}

		g_array_free(thumb_list, FALSE);
	}

	return MEDIA_CONTENT_ERROR_NONE;
}

int media_info_delete_batch_from_db(filter_h filter)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;

	char *query_string = NULL;
	filter_s *_filter = NULL;
	attribute_h attr;
	char *condition_query = NULL;
	GArray *thumb_list = NULL;

	media_content_retvm_if(filter == NULL, MEDIA_CONTENT_ERROR_INVALID_PARAMETER, "Invalid filter");

	thumb_list = g_array_new(FALSE, FALSE, sizeof(char*));

	/* Delete thumbnail of each item */
	ret = _media_db_get_group_item(NULL, filter, __media_info_delete_batch_cb, thumb_list, MEDIA_GROUP_NONE);

	_filter = (filter_s*)filter;
	attr = _content_get_attirbute_handle();

	if(_filter->condition)
	{
		ret = _media_filter_attribute_generate(attr, _filter->condition, _filter->condition_collate_type, &condition_query);
		if (ret != MEDIA_CONTENT_ERROR_NONE) {
			__media_info_release_thumb_list(thumb_list);
			return ret;
		}
	}

	if(STRING_VALID(condition_query))
	{
		if (_filter->storage_id == NULL) {
			/*FIX ME*/
			query_string = sqlite3_mprintf(DELETE_MEDIA_FROM_MEDIA_BATCH, "media", condition_query);
		} else {
			query_string = sqlite3_mprintf(DELETE_MEDIA_FROM_MEDIA_BATCH, _filter->storage_id, condition_query);
		}
	}
	else
	{
		SAFE_FREE(condition_query);
		__media_info_release_thumb_list(thumb_list);

		return MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
	}

	ret = _content_query_sql(query_string);
	if (ret == MEDIA_CONTENT_ERROR_NONE) {
		/* Send notification for this In this case, send noti for internal storage and external storage */
		media_content_debug("Batch deletion is successfull. Send notification for this");
		if (_filter->storage_id != NULL) {
			char *storage_path = NULL;
			media_svc_get_storage_path(_content_get_db_handle(), _filter->storage_id, &storage_path);
			if (STRING_VALID(storage_path))
				media_svc_publish_noti(_content_get_db_handle(), MS_MEDIA_ITEM_DIRECTORY, MS_MEDIA_ITEM_UPDATE, storage_path, -1, NULL, NULL);

			SAFE_FREE(storage_path);
		} else {
			/*FIX ME*/
			media_svc_publish_noti(_content_get_db_handle(), MS_MEDIA_ITEM_DIRECTORY, MS_MEDIA_ITEM_UPDATE, MEDIA_ROOT_PATH_INTERNAL, -1, NULL, NULL);
			media_svc_publish_noti(_content_get_db_handle(), MS_MEDIA_ITEM_DIRECTORY, MS_MEDIA_ITEM_UPDATE, MEDIA_ROOT_PATH_SDCARD, -1, NULL, NULL);
		}

		__media_info_delete_thumb_from_list(thumb_list);
		__media_info_release_thumb_list(thumb_list);
	}

	SAFE_FREE(condition_query);
	SQLITE3_SAFE_FREE(query_string);

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
		SAFE_FREE(_media->title);
		SAFE_FREE(_media->weather);
		SAFE_FREE(_media->storage_uuid);

		if(_media->image_meta) {
			SAFE_FREE(_media->image_meta->media_id);
			SAFE_FREE(_media->image_meta->date_taken);
			SAFE_FREE(_media->image_meta->burst_id);
			SAFE_FREE(_media->image_meta->exposure_time);
			SAFE_FREE(_media->image_meta->model);
			SAFE_FREE(_media->image_meta->title);
			SAFE_FREE(_media->image_meta->weather);

			SAFE_FREE(_media->image_meta);
		}

		if(_media->video_meta) {
			SAFE_FREE(_media->video_meta->media_id);
			SAFE_FREE(_media->video_meta->title);
			SAFE_FREE(_media->video_meta->album);
			SAFE_FREE(_media->video_meta->artist);
			SAFE_FREE(_media->video_meta->album_artist);
			SAFE_FREE(_media->video_meta->genre);
			SAFE_FREE(_media->video_meta->composer);
			SAFE_FREE(_media->video_meta->year);
			SAFE_FREE(_media->video_meta->recorded_date);
			SAFE_FREE(_media->video_meta->copyright);
			SAFE_FREE(_media->video_meta->track_num);

			SAFE_FREE(_media->video_meta);
		}

		if(_media->audio_meta) {
			SAFE_FREE(_media->audio_meta->media_id);
			SAFE_FREE(_media->audio_meta->title);
			SAFE_FREE(_media->audio_meta->album);
			SAFE_FREE(_media->audio_meta->artist);
			SAFE_FREE(_media->audio_meta->album_artist);
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
		media_content_retvm_if(_dst == NULL, MEDIA_CONTENT_ERROR_OUT_OF_MEMORY, "OUT_OF_MEMORY");

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

		if(STRING_VALID(_src->weather))
		{
			_dst->weather = strdup(_src->weather);
			if(_dst->weather == NULL)
			{
				media_content_error("OUT_OF_MEMORY(0x%08x)", MEDIA_CONTENT_ERROR_OUT_OF_MEMORY);
				media_info_destroy((media_info_h)_dst);
				return MEDIA_CONTENT_ERROR_OUT_OF_MEMORY;
			}
		}

		if(STRING_VALID(_src->title))
		{
			_dst->title = strdup(_src->title);
			if(_dst->title == NULL)
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

		if(STRING_VALID(_src->storage_uuid))
		{
			_dst->storage_uuid = strdup(_src->storage_uuid);
			if(_dst->storage_uuid == NULL)
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
		_dst->timeline = _src->timeline;
		_dst->longitude = _src->longitude;
		_dst->latitude = _src->latitude;
		_dst->altitude = _src->altitude;
		_dst->rating = _src->rating;
		_dst->favourite = _src->favourite;
		_dst->is_drm = _src->is_drm;
		_dst->storage_type = _src->storage_type;
		_dst->played_count = _src->played_count;
		_dst->played_time = _src->played_time;
		_dst->played_position = _src->played_position;
		_dst->sync_status = _src->sync_status;

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

			if(STRING_VALID(_src->image_meta->burst_id))
			{
				_dst->image_meta->burst_id = strdup(_src->image_meta->burst_id);
				if(_dst->image_meta->burst_id == NULL)
				{
					media_content_error("OUT_OF_MEMORY(0x%08x)", MEDIA_CONTENT_ERROR_OUT_OF_MEMORY);
					media_info_destroy((media_info_h)_dst);
					return MEDIA_CONTENT_ERROR_OUT_OF_MEMORY;
				}
			}

			if(STRING_VALID(_src->image_meta->weather))
			{
				_dst->image_meta->weather = strdup(_src->image_meta->weather);
				if(_dst->image_meta->weather == NULL)
				{
					media_content_error("OUT_OF_MEMORY(0x%08x)", MEDIA_CONTENT_ERROR_OUT_OF_MEMORY);
					media_info_destroy((media_info_h)_dst);
					return MEDIA_CONTENT_ERROR_OUT_OF_MEMORY;
				}
			}

			if(STRING_VALID(_src->image_meta->exposure_time))
			{
				_dst->image_meta->exposure_time = strdup(_src->image_meta->exposure_time);
				if(_dst->image_meta->exposure_time == NULL)
				{
					media_content_error("OUT_OF_MEMORY(0x%08x)", MEDIA_CONTENT_ERROR_OUT_OF_MEMORY);
					media_info_destroy((media_info_h)_dst);
					return MEDIA_CONTENT_ERROR_OUT_OF_MEMORY;
				}
			}

			if(STRING_VALID(_src->image_meta->model))
			{
				_dst->image_meta->model = strdup(_src->image_meta->model);
				if(_dst->image_meta->model == NULL)
				{
					media_content_error("OUT_OF_MEMORY(0x%08x)", MEDIA_CONTENT_ERROR_OUT_OF_MEMORY);
					media_info_destroy((media_info_h)_dst);
					return MEDIA_CONTENT_ERROR_OUT_OF_MEMORY;
				}
			}

			_dst->image_meta->fnumber = _src->image_meta->fnumber;
			_dst->image_meta->iso = _src->image_meta->iso;
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
			if(STRING_VALID(_src->video_meta->album_artist))
			{
				_dst->video_meta->album_artist = strdup(_src->video_meta->album_artist);
				if(_dst->video_meta->album_artist == NULL)
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
				if(_dst->video_meta->track_num == NULL)
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
			if(STRING_VALID(_src->audio_meta->album_artist))
			{
				_dst->audio_meta->album_artist = strdup(_src->audio_meta->album_artist);
				if(_dst->audio_meta->album_artist == NULL)
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
				if(_dst->audio_meta->track_num == NULL)
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
			_dst->audio_meta->bitpersample = _src->audio_meta->bitpersample;
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

	media_content_retvm_if(callback == NULL, MEDIA_CONTENT_ERROR_INVALID_PARAMETER, "Invalid callback");

	ret = _media_db_get_group_item(NULL, filter, callback, user_data, MEDIA_GROUP_NONE);

	return ret;
}

int media_info_get_tag_count_from_db(const char *media_id, filter_h filter, int *tag_count)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;

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

	media_content_retvm_if(_media == NULL, MEDIA_CONTENT_ERROR_INVALID_PARAMETER, "Invalid media");
	media_content_retvm_if(_media->media_type != MEDIA_CONTENT_TYPE_IMAGE, MEDIA_CONTENT_ERROR_INVALID_PARAMETER, "Invalid media_type");
	media_content_retvm_if(_media->image_meta == NULL, MEDIA_CONTENT_ERROR_INVALID_PARAMETER, "Invalid image_meta");

	image_meta_s *_image = (image_meta_s*)calloc(1, sizeof(image_meta_s));
	media_content_retvm_if(_image == NULL, MEDIA_CONTENT_ERROR_OUT_OF_MEMORY, "OUT_OF_MEMORY");

	if(STRING_VALID(_media->media_id)) {
		_image->media_id = strdup(_media->media_id);
	}

	_image->width = _media->image_meta->width;
	_image->height = _media->image_meta->height;
	_image->orientation = _media->image_meta->orientation;
	_image->fnumber = _media->image_meta->fnumber;
	_image->iso = _media->image_meta->iso;

	if(STRING_VALID(_media->image_meta->date_taken)) {
		_image->date_taken = strdup(_media->image_meta->date_taken);
	}

	if(STRING_VALID(_media->image_meta->title)) {
		_image->title = strdup(_media->image_meta->title);
	}

	if(STRING_VALID(_media->image_meta->burst_id)) {
		_image->burst_id = strdup(_media->image_meta->burst_id);
	}

	if(STRING_VALID(_media->image_meta->weather)) {
		_image->weather = strdup(_media->image_meta->weather);
	}

	if(STRING_VALID(_media->image_meta->exposure_time)) {
		_image->exposure_time = strdup(_media->image_meta->exposure_time);
	}

	if(STRING_VALID(_media->image_meta->model)) {
		_image->model = strdup(_media->image_meta->model);
	}

	*image = (image_meta_h)_image;

	return ret;
}

int media_info_get_video(media_info_h media, video_meta_h *video)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;

	media_info_s *_media = (media_info_s*)media;

	media_content_retvm_if(_media == NULL, MEDIA_CONTENT_ERROR_INVALID_PARAMETER, "Invalid media");
	media_content_retvm_if(_media->media_type != MEDIA_CONTENT_TYPE_VIDEO, MEDIA_CONTENT_ERROR_INVALID_PARAMETER, "Invalid media_type");
	media_content_retvm_if(_media->video_meta == NULL, MEDIA_CONTENT_ERROR_INVALID_PARAMETER, "Invalid video_meta");

	video_meta_s *_video = (video_meta_s*)calloc(1, sizeof(video_meta_s));
	media_content_retvm_if(_video == NULL, MEDIA_CONTENT_ERROR_OUT_OF_MEMORY, "OUT_OF_MEMORY");

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
	if(STRING_VALID(_media->video_meta->album_artist)) {
		_video->album_artist = strdup(_media->video_meta->album_artist);
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


	media_content_retvm_if(_media == NULL, MEDIA_CONTENT_ERROR_INVALID_PARAMETER, "Invalid media");
	media_content_retvm_if(_media->media_type != MEDIA_CONTENT_TYPE_MUSIC && _media->media_type != MEDIA_CONTENT_TYPE_SOUND, MEDIA_CONTENT_ERROR_INVALID_PARAMETER, "Invalid media_type");
	media_content_retvm_if(_media->audio_meta == NULL, MEDIA_CONTENT_ERROR_INVALID_PARAMETER, "Invalid audio_meta");

	audio_meta_s *_audio = (audio_meta_s*)calloc(1, sizeof(audio_meta_s));
	media_content_retvm_if(_audio == NULL, MEDIA_CONTENT_ERROR_OUT_OF_MEMORY, "OUT_OF_MEMORY");

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
	if(STRING_VALID(_media->audio_meta->album_artist)) {
		_audio->album_artist = strdup(_media->audio_meta->album_artist);
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
	_audio->bitpersample = _media->audio_meta->bitpersample;
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
			media_content_retvm_if(*media_id == NULL, MEDIA_CONTENT_ERROR_OUT_OF_MEMORY, "OUT_OF_MEMORY");
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

int media_info_get_display_name(media_info_h media, char **name)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	media_info_s *_media = (media_info_s*)media;
	if(_media && name)
	{
		if(STRING_VALID(_media->display_name))
		{
			*name = strdup(_media->display_name);
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
			media_content_retvm_if(*mime_type == NULL, MEDIA_CONTENT_ERROR_OUT_OF_MEMORY, "OUT_OF_MEMORY");
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

int media_info_get_timeline(media_info_h media, time_t* time)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	media_info_s *_media = (media_info_s*)media;
	if(_media && time)
	{
		*time = _media->timeline;
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

int media_info_get_title(media_info_h media, char **title)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	media_info_s *_media = (media_info_s*)media;

	if(_media && title)
	{
		if(STRING_VALID(_media->title))
		{
			*title = strdup(_media->title);
			media_content_retvm_if(*title == NULL, MEDIA_CONTENT_ERROR_OUT_OF_MEMORY, "OUT_OF_MEMORY");
		}
		else
		{
			*title = NULL;
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
			media_content_retvm_if(*description == NULL, MEDIA_CONTENT_ERROR_OUT_OF_MEMORY, "OUT_OF_MEMORY");
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

int media_info_get_weather(media_info_h media, char **weather)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	media_info_s *_media = (media_info_s*)media;

	if(_media && weather)
	{
		if(STRING_VALID(_media->weather))
		{
			*weather = strdup(_media->weather);
			media_content_retvm_if(*weather == NULL, MEDIA_CONTENT_ERROR_OUT_OF_MEMORY, "OUT_OF_MEMORY");
		}
		else
		{
			*weather = NULL;
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
			media_content_retvm_if(*author == NULL, MEDIA_CONTENT_ERROR_OUT_OF_MEMORY, "OUT_OF_MEMORY");
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
			media_content_retvm_if(*provider == NULL, MEDIA_CONTENT_ERROR_OUT_OF_MEMORY, "OUT_OF_MEMORY");
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
			media_content_retvm_if(*content_name == NULL, MEDIA_CONTENT_ERROR_OUT_OF_MEMORY, "OUT_OF_MEMORY");
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
			media_content_retvm_if(*category == NULL, MEDIA_CONTENT_ERROR_OUT_OF_MEMORY, "OUT_OF_MEMORY");
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
			media_content_retvm_if(*location_tag == NULL, MEDIA_CONTENT_ERROR_OUT_OF_MEMORY, "OUT_OF_MEMORY");
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
			media_content_retvm_if(*age_rating == NULL, MEDIA_CONTENT_ERROR_OUT_OF_MEMORY, "OUT_OF_MEMORY");
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
			media_content_retvm_if(*keyword == NULL, MEDIA_CONTENT_ERROR_OUT_OF_MEMORY, "OUT_OF_MEMORY");
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

int media_info_get_storage_id(media_info_h media, char **storage_id)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	media_info_s *_media = (media_info_s*)media;

	if(_media && storage_id)
	{
		if(STRING_VALID(_media->storage_uuid))
		{
			*storage_id = strdup(_media->storage_uuid);
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

int media_info_get_played_count(media_info_h media, int *played_count)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	media_info_s *_media = (media_info_s*)media;

	if(_media)
	{
		*played_count = _media->played_count;
		ret = MEDIA_CONTENT_ERROR_NONE;
	}
	else
	{
		media_content_error("INVALID_PARAMETER(0x%08x)", MEDIA_CONTENT_ERROR_INVALID_PARAMETER);
		ret = MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
	}

	return ret;
}

int media_info_get_played_time(media_info_h media, time_t* played_time)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	media_info_s *_media = (media_info_s*)media;

	if(_media)
	{
		*played_time = _media->played_time;
		ret = MEDIA_CONTENT_ERROR_NONE;
	}
	else
	{
		media_content_error("INVALID_PARAMETER(0x%08x)", MEDIA_CONTENT_ERROR_INVALID_PARAMETER);
		ret = MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
	}

	return ret;
}

int media_info_increase_played_count(media_info_h media)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;

	media_info_s *_media = (media_info_s*)media;

	if(_media)
	{
		_media->played_count += 1;
	}
	else
	{
		media_content_error("INVALID_PARAMETER(0x%08x)", MEDIA_CONTENT_ERROR_INVALID_PARAMETER);
		ret = MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
	}

	return ret;
}

int media_info_set_played_time(media_info_h media)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	time_t current_time;

	media_info_s *_media = (media_info_s*)media;

	if(_media != NULL)
	{
		time(&current_time);
		_media->played_time = current_time;
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
	char storage_id[MEDIA_CONTENT_UUID_SIZE+1] = {0,};
	sqlite3_stmt *stmt = NULL;

	if(!STRING_VALID(media_id) || (media == NULL))
	{
		media_content_error("INVALID_PARAMETER(0x%08x)", MEDIA_CONTENT_ERROR_INVALID_PARAMETER);
		return MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
	}

	memset(select_query, 0x00, sizeof(select_query));
	memset(storage_id, 0x00, sizeof(storage_id));

	ret = _media_db_get_storage_id_by_media_id(media_id, storage_id);
	media_content_retv_if(ret != MEDIA_CONTENT_ERROR_NONE, ret);

	snprintf(select_query, sizeof(select_query), SELECT_MEDIA_FROM_MEDIA, storage_id, media_id);

	ret = _content_query_prepare(&stmt, select_query, NULL, NULL);
	media_content_retv_if(ret != MEDIA_CONTENT_ERROR_NONE, ret);

	media_info_s *_media = NULL;

	while(sqlite3_step(stmt) == SQLITE_ROW)
	{
		if(_media)
			media_info_destroy((media_info_h)_media);

		_media = (media_info_s*)calloc(1, sizeof(media_info_s));

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
		media_content_retvm_if(_media->display_name == NULL, MEDIA_CONTENT_ERROR_OUT_OF_MEMORY, "OUT_OF_MEMORY");
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
			media_content_retvm_if(_media->description == NULL, MEDIA_CONTENT_ERROR_OUT_OF_MEMORY, "OUT_OF_MEMORY");
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

int media_info_set_weather(media_info_h media, const char *weather)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	media_info_s *_media = (media_info_s*)media;

	if(_media != NULL)
	{
		SAFE_FREE(_media->weather);

		if(STRING_VALID(weather))
		{
			_media->weather = strdup(weather);
			media_content_retvm_if(_media->weather == NULL, MEDIA_CONTENT_ERROR_OUT_OF_MEMORY, "OUT_OF_MEMORY");
		}
		else
		{
			_media->weather = NULL;
		}
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
		if(favorite == TRUE)
		{
			time(&_media->favourite);
		}
		else
		{
			_media->favourite = 0;
		}
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
			media_content_retvm_if(_media->author == NULL, MEDIA_CONTENT_ERROR_OUT_OF_MEMORY, "OUT_OF_MEMORY");
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
			media_content_retvm_if(_media->provider == NULL, MEDIA_CONTENT_ERROR_OUT_OF_MEMORY, "OUT_OF_MEMORY");
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
			media_content_retvm_if(_media->content_name == NULL, MEDIA_CONTENT_ERROR_OUT_OF_MEMORY, "OUT_OF_MEMORY");
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
			media_content_retvm_if(_media->category == NULL, MEDIA_CONTENT_ERROR_OUT_OF_MEMORY, "OUT_OF_MEMORY");
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
			media_content_retvm_if(_media->location_tag == NULL, MEDIA_CONTENT_ERROR_OUT_OF_MEMORY, "OUT_OF_MEMORY");
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
			media_content_retvm_if(_media->age_rating == NULL, MEDIA_CONTENT_ERROR_OUT_OF_MEMORY, "OUT_OF_MEMORY");
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
			media_content_retvm_if(_media->keyword == NULL, MEDIA_CONTENT_ERROR_OUT_OF_MEMORY, "OUT_OF_MEMORY");
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
	media_info_s *_media = (media_info_s*)media;
	char *set_sql = NULL;
	char *sql = NULL;

	if(_media != NULL && STRING_VALID(_media->media_id))
	{
		/* This sql is due to sqlite3_mprintf's wrong operation when using floating point in the text format */
		/* This code will be removed when sqlite3_mprintf works clearly */
		char *test_sql = sqlite3_mprintf("%f, %f, %f", _media->longitude, _media->latitude, _media->altitude);
		SQLITE3_SAFE_FREE(test_sql);

		/*Update Pinyin If Support Pinyin*/
		char *file_name_pinyin = NULL;
		char *description_pinyin = NULL;
		char *author_pinyin = NULL;
		char *provider_pinyin = NULL;
		char *content_name_pinyin = NULL;
		char *category_pinyin = NULL;
		char *location_tag_pinyin = NULL;
		char *age_rating_pinyin = NULL;
		char *keyword_pinyin = NULL;
		bool pinyin_support = FALSE;

		/*Update Pinyin If Support Pinyin*/
		media_svc_check_pinyin_support(&pinyin_support);
		if(pinyin_support)
		{
			if(STRING_VALID(_media->display_name))
				media_svc_get_pinyin(_content_get_db_handle(), _media->display_name, &file_name_pinyin);
			if(STRING_VALID(_media->description))
				media_svc_get_pinyin(_content_get_db_handle(), _media->description, &description_pinyin);
			if(STRING_VALID(_media->author))
				media_svc_get_pinyin(_content_get_db_handle(), _media->author, &author_pinyin);
			if(STRING_VALID(_media->provider))
				media_svc_get_pinyin(_content_get_db_handle(), _media->provider, &provider_pinyin);
			if(STRING_VALID(_media->content_name))
				media_svc_get_pinyin(_content_get_db_handle(), _media->content_name, &content_name_pinyin);
			if(STRING_VALID(_media->category))
				media_svc_get_pinyin(_content_get_db_handle(), _media->category, &category_pinyin);
			if(STRING_VALID(_media->location_tag))
				media_svc_get_pinyin(_content_get_db_handle(), _media->location_tag, &location_tag_pinyin);
			if(STRING_VALID(_media->age_rating))
				media_svc_get_pinyin(_content_get_db_handle(), _media->age_rating, &age_rating_pinyin);
			if(STRING_VALID(_media->keyword))
				media_svc_get_pinyin(_content_get_db_handle(), _media->keyword, &keyword_pinyin);
		}

		set_sql = sqlite3_mprintf("file_name=%Q, added_time=%d, description=%Q, longitude=%f, latitude=%f, altitude=%f, \
			played_count=%d, last_played_time=%d, last_played_position=%d, \
			rating=%d, favourite=%d, author=%Q, provider=%Q, content_name=%Q, category=%Q, location_tag=%Q, age_rating=%Q, keyword=%Q, weather=%Q, sync_status=%d, \
			file_name_pinyin=%Q, description_pinyin=%Q, author_pinyin=%Q, provider_pinyin=%Q, content_name_pinyin=%Q, category_pinyin=%Q, location_tag_pinyin=%Q, age_rating_pinyin=%Q, keyword_pinyin=%Q, title=%Q",
			_media->display_name, _media->added_time, _media->description, _media->longitude, _media->latitude, _media->altitude, _media->played_count, _media->played_time, _media->played_position, _media->rating, _media->favourite,
			_media->author, _media->provider, _media->content_name, _media->category, _media->location_tag, _media->age_rating, _media->keyword, _media->weather, _media->sync_status,
			file_name_pinyin, description_pinyin, author_pinyin, provider_pinyin, content_name_pinyin, category_pinyin, location_tag_pinyin, age_rating_pinyin, keyword_pinyin, _media->title);

		sql = sqlite3_mprintf("UPDATE %Q SET %s WHERE media_uuid=%Q", _media->storage_uuid, set_sql, _media->media_id);

		ret = _content_query_sql(sql);
		SQLITE3_SAFE_FREE(set_sql);
		SQLITE3_SAFE_FREE(sql);

		SAFE_FREE(description_pinyin);
		SAFE_FREE(author_pinyin);
		SAFE_FREE(provider_pinyin);
		SAFE_FREE(content_name_pinyin);
		SAFE_FREE(category_pinyin);
		SAFE_FREE(location_tag_pinyin);
		SAFE_FREE(age_rating_pinyin);
		SAFE_FREE(keyword_pinyin);

		if(_media->storage_type == MEDIA_CONTENT_STORAGE_CLOUD)
		{
			set_sql = NULL;
			sql = NULL;

			if(_media->media_type == MEDIA_CONTENT_TYPE_VIDEO)
			{
				set_sql = sqlite3_mprintf("title=%Q, album=%Q, artist=%Q, genre=%Q, duration=%d, width=%d, height=%d", \
				_media->title, _media->video_meta->album, _media->video_meta->artist, _media->video_meta->genre, _media->video_meta->duration, _media->video_meta->width, _media->video_meta->height);
			}
			else if((_media->media_type == MEDIA_CONTENT_TYPE_MUSIC) || (_media->media_type == MEDIA_CONTENT_TYPE_SOUND))
			{
				set_sql = sqlite3_mprintf("title=%Q, album=%Q, artist=%Q, genre=%Q, duration=%d", \
				_media->title, _media->audio_meta->album, _media->audio_meta->artist, _media->audio_meta->genre, _media->audio_meta->duration);
			}
			else if(_media->media_type == MEDIA_CONTENT_TYPE_IMAGE)
			{
				set_sql = sqlite3_mprintf("title=%Q, width=%d, height=%d", _media->title, _media->image_meta->width, _media->image_meta->height);
			}
			else
			{
				set_sql = sqlite3_mprintf("title=%Q", _media->title);
			}

			sql = sqlite3_mprintf("UPDATE %Q SET %s WHERE media_uuid=%Q", _media->storage_uuid, set_sql, _media->media_id);

			ret = _content_query_sql(sql);

			SQLITE3_SAFE_FREE(set_sql);
			SQLITE3_SAFE_FREE(sql);
		}

		if (ret == MEDIA_CONTENT_ERROR_NONE) {
			/* Send notification for this update */
			media_content_debug("Update is successfull. Send notification for this");
			if (_media->file_path && _media->mime_type) {
				media_svc_publish_noti(_content_get_db_handle(), MS_MEDIA_ITEM_FILE, MS_MEDIA_ITEM_UPDATE, _media->file_path, _media->media_type, _media->media_id, _media->mime_type);
			} else {
				media_content_error("Can't Send Noti : path or mime type is NULL");
			}
		}
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
	media_info_h media = NULL;
	char *file_path = NULL;
	media_content_storage_e storage_type = 0;
	char storage_id[MEDIA_CONTENT_UUID_SIZE+1] = {0,};

	if(!STRING_VALID(media_id))
	{
		media_content_error("INVALID_PARAMETER(0x%08x)", MEDIA_CONTENT_ERROR_INVALID_PARAMETER);
		return MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
	}

	ret = media_info_get_media_from_db(media_id, &media);
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

	if(storage_type == MEDIA_CONTENT_STORAGE_CLOUD)
	{
		media_info_destroy(media);
		media_content_error("Can't refresh cloud content!!");
		return MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
	}

	ret = media_info_get_file_path(media, &file_path);
	if(ret != MEDIA_CONTENT_ERROR_NONE)
	{
		media_info_destroy(media);
		return ret;
	}

	ret = _media_util_check_file_exist(file_path);
	if (ret != MEDIA_CONTENT_ERROR_NONE) {
		SAFE_FREE(file_path);
		media_info_destroy(media);
		return ret;
	}

	memset(storage_id, 0x00, sizeof(storage_id));
	ret = media_svc_get_storage_id(_content_get_db_handle(), file_path, storage_id);
	if(ret != MS_MEDIA_ERR_NONE)
	{
		media_content_error("media_svc_get_storage_id failed : %d", ret);
		SAFE_FREE(file_path);
		media_info_destroy(media);
		return _content_error_capi(MEDIA_CONTENT_TYPE, ret);
	}

	ret = media_svc_refresh_item(_content_get_db_handle(), storage_id, storage_type, file_path);
	if (ret != MS_MEDIA_ERR_NONE)
	{
		ret = _content_error_capi(MEDIA_CONTENT_TYPE, ret);
	}

	SAFE_FREE(file_path);
	media_info_destroy(media);

	return ret;
}

int media_info_move_to_db(media_info_h media, const char* dst_path)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	bool ignore_file = FALSE;
	bool ignore_dir = FALSE;
	char *folder_path = NULL;
	media_svc_storage_type_e src_storage_type = 0;
	media_svc_storage_type_e dst_storage_type = 0;
	char storage_id[MEDIA_CONTENT_UUID_SIZE+1] = {0,};

	media_content_retvm_if(media == NULL, MEDIA_CONTENT_ERROR_INVALID_PARAMETER, "invalid media");
	media_content_retvm_if(!STRING_VALID(dst_path), MEDIA_CONTENT_ERROR_INVALID_PARAMETER, "invalid dst_path");

	media_info_s *_media = (media_info_s*)media;

	ret = _media_util_check_ignore_file(dst_path, &ignore_file);
	media_content_retvm_if(ignore_file == TRUE, MEDIA_CONTENT_ERROR_INVALID_PARAMETER, "Invalid path");

	ret = _media_util_check_file_exist(dst_path);
	media_content_retv_if(ret != MEDIA_CONTENT_ERROR_NONE, ret);

	folder_path = g_path_get_dirname(dst_path);
	ret = _media_util_check_ignore_dir(folder_path, &ignore_dir);
	SAFE_FREE(folder_path);

	media_content_retvm_if(ignore_dir == TRUE, MEDIA_CONTENT_ERROR_INVALID_PARAMETER, "Invalid folder path");

	ret = media_svc_get_storage_type(_media->file_path, &src_storage_type);
	if(ret != MS_MEDIA_ERR_NONE)
	{
		media_content_sec_error("media_svc_get_storage_type failed : %d", ret);
		return _content_error_capi(MEDIA_CONTENT_TYPE, ret);
	}

	ret = media_svc_get_storage_type(dst_path, &dst_storage_type);
	if(ret != MS_MEDIA_ERR_NONE)
	{
		media_content_sec_error("media_svc_get_storage_type failed : %d", ret);
		return _content_error_capi(MEDIA_CONTENT_TYPE, ret);
	}

	memset(storage_id, 0x00, sizeof(storage_id));
	ret = media_svc_get_storage_id(_content_get_db_handle(), _media->file_path, storage_id);
	if(ret != MS_MEDIA_ERR_NONE)
	{
		media_content_error("media_svc_get_storage_id failed : %d", ret);
		return _content_error_capi(MEDIA_CONTENT_TYPE, ret);
	}

	ret = media_svc_move_item(_content_get_db_handle(), storage_id, src_storage_type, _media->file_path, dst_storage_type, dst_path);
	return _content_error_capi(MEDIA_CONTENT_TYPE, ret);
}

int media_info_create_thumbnail(media_info_h media, media_thumbnail_completed_cb callback, void *user_data)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	media_info_s *_media = (media_info_s*)media;

	if(_media != NULL && STRING_VALID(_media->media_id) && STRING_VALID(_media->file_path))
	{
		media_thumbnail_cb_s *_thumb_cb = (media_thumbnail_cb_s*)calloc(1, sizeof(media_thumbnail_cb_s));
		media_content_retvm_if(_thumb_cb == NULL, MEDIA_CONTENT_ERROR_OUT_OF_MEMORY, "OUT_OF_MEMORY");

		_thumb_cb->handle = _media;
		_thumb_cb->user_data = user_data;
		_thumb_cb->thumbnail_completed_cb = callback;

		ret = thumbnail_request_from_db_async(_media->file_path, (ThumbFunc)__media_info_thumbnail_completed_cb, (void *)_thumb_cb);
		ret = _content_error_capi(MEDIA_THUMBNAIL_TYPE, ret);
	}
	else
	{
		media_content_error("INVALID_PARAMETER(0x%08x)", MEDIA_CONTENT_ERROR_INVALID_PARAMETER);
		ret = MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
	}

	return ret;
}

int media_info_cancel_thumbnail(media_info_h media)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	media_info_s *_media = (media_info_s*)media;

	if(_media != NULL && STRING_VALID(_media->media_id) && STRING_VALID(_media->file_path))
	{
		ret = thumbnail_request_cancel_media(_media->file_path);
		ret = _content_error_capi(MEDIA_THUMBNAIL_TYPE, ret);
	}
	else
	{
		media_content_error("INVALID_PARAMETER(0x%08x)", MEDIA_CONTENT_ERROR_INVALID_PARAMETER);
		ret = MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
	}

	return ret;
}

static int __media_info_map_data_usr_to_svc(media_info_s *media, media_svc_content_info_s **service_content, media_content_storage_e storage_type)
{
	if(media == NULL)
	{
		media_content_error("OUT_OF_MEMORY(0x%08x)", MEDIA_CONTENT_ERROR_INVALID_PARAMETER);
		return MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
	}

	media_svc_content_info_s *svc_content_info = calloc(1, sizeof(media_svc_content_info_s));
	media_content_retvm_if(svc_content_info == NULL, MEDIA_CONTENT_ERROR_OUT_OF_MEMORY, "OUT_OF_MEMORY");

	svc_content_info->path = g_strdup(media->file_path);
	svc_content_info->media_type = media->media_type;
	svc_content_info->mime_type = g_strdup(media->mime_type);
	svc_content_info->size = media->size;
	svc_content_info->storage_type = storage_type;
	svc_content_info->storage_uuid = g_strdup(media->storage_uuid);

	svc_content_info->added_time = media->added_time;
	svc_content_info->modified_time = media->modified_time;
	svc_content_info->thumbnail_path = g_strdup(media->thumbnail_path);
	svc_content_info->is_drm = media->is_drm;

	svc_content_info->media_meta.title = g_strdup(media->title);
	svc_content_info->media_meta.rating = media->rating;
	svc_content_info->media_meta.description = g_strdup(media->description);
	svc_content_info->media_meta.longitude = media->longitude;
	svc_content_info->media_meta.latitude = media->latitude;
	svc_content_info->media_meta.altitude = media->altitude;
	svc_content_info->media_meta.weather = g_strdup(media->weather);

	switch(media->media_type)
	{
		case MEDIA_CONTENT_TYPE_IMAGE:
			svc_content_info->media_meta.width = media->image_meta->width;
			svc_content_info->media_meta.height = media->image_meta->height;
			svc_content_info->media_meta.datetaken = g_strdup(media->image_meta->date_taken);
			svc_content_info->media_meta.orientation = media->image_meta->orientation;
			break;
		case MEDIA_CONTENT_TYPE_VIDEO:
			svc_content_info->media_meta.album = g_strdup(media->audio_meta->album);
			svc_content_info->media_meta.artist = g_strdup(media->audio_meta->artist);
			svc_content_info->media_meta.album_artist = g_strdup(media->audio_meta->album_artist);
			svc_content_info->media_meta.genre = g_strdup(media->audio_meta->genre);
			svc_content_info->media_meta.composer = g_strdup(media->audio_meta->composer);
			svc_content_info->media_meta.year = g_strdup(media->audio_meta->year);
			svc_content_info->media_meta.recorded_date = g_strdup(media->audio_meta->recorded_date);
			svc_content_info->media_meta.copyright = g_strdup(media->audio_meta->copyright);
			svc_content_info->media_meta.track_num = g_strdup(media->audio_meta->track_num);
			svc_content_info->media_meta.bitrate = media->audio_meta->bitrate;
			svc_content_info->media_meta.duration = media->audio_meta->duration;
			svc_content_info->media_meta.width = media->image_meta->width;
			svc_content_info->media_meta.height = media->image_meta->height;
			break;
		case MEDIA_CONTENT_TYPE_SOUND:
		case MEDIA_CONTENT_TYPE_MUSIC:
			svc_content_info->media_meta.album = g_strdup(media->audio_meta->album);
			svc_content_info->media_meta.artist = g_strdup(media->audio_meta->artist);
			svc_content_info->media_meta.album_artist = g_strdup(media->audio_meta->album_artist);
			svc_content_info->media_meta.genre = g_strdup(media->audio_meta->genre);
			svc_content_info->media_meta.composer = g_strdup(media->audio_meta->composer);
			svc_content_info->media_meta.year = g_strdup(media->audio_meta->year);
			svc_content_info->media_meta.recorded_date = g_strdup(media->audio_meta->recorded_date);
			svc_content_info->media_meta.copyright = g_strdup(media->audio_meta->copyright);
			svc_content_info->media_meta.track_num = g_strdup(media->audio_meta->track_num);
			svc_content_info->media_meta.bitrate = media->audio_meta->bitrate;
			svc_content_info->media_meta.duration = media->audio_meta->duration;
			svc_content_info->media_meta.channel = media->audio_meta->channel;
			svc_content_info->media_meta.samplerate = media->audio_meta->samplerate;
			break;
		case MEDIA_CONTENT_TYPE_OTHERS:
		default:
			break;
	}

	*service_content = svc_content_info;
	return MEDIA_CONTENT_ERROR_NONE;
}

static int __media_info_set_str_data(media_info_h media, media_info_item_e data_type, const char *str_data)
{
	media_info_s *_media = (media_info_s*)media;

	if(_media == NULL)
	{
		media_content_error("INVALID_PARAMETER(0x%08x)", MEDIA_CONTENT_ERROR_INVALID_PARAMETER);
		return MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
	}

	switch(data_type)
	{
		case MEDIA_INFO_PATH:
			SAFE_FREE(_media->file_path);
			if(STRING_VALID(str_data))
			{
				_media->file_path = strdup(str_data);
				media_content_retvm_if(_media->file_path == NULL, MEDIA_CONTENT_ERROR_OUT_OF_MEMORY, "OUT_OF_MEMORY");
			}
			else
			{
				_media->file_path = NULL;
			}
			break;
		case MEDIA_INFO_MIME_TYPE:
			SAFE_FREE(_media->mime_type);
			if(STRING_VALID(str_data))
			{
				_media->mime_type = strdup(str_data);
				media_content_retvm_if(_media->mime_type == NULL, MEDIA_CONTENT_ERROR_OUT_OF_MEMORY, "OUT_OF_MEMORY");
			}
			else
			{
				_media->mime_type = NULL;
			}
			break;
		case MEDIA_INFO_THUMBNAIL_PATH:
			SAFE_FREE(_media->thumbnail_path);
			if(STRING_VALID(str_data))
			{
				_media->thumbnail_path = strdup(str_data);
				media_content_retvm_if(_media->thumbnail_path == NULL, MEDIA_CONTENT_ERROR_OUT_OF_MEMORY, "OUT_OF_MEMORY");
			}
			else
			{
				_media->thumbnail_path = NULL;
			}
			break;
		case MEDIA_INFO_TITLE:
			SAFE_FREE(_media->title);
			if(STRING_VALID(str_data))
			{
				_media->title = strdup(str_data);
				media_content_retvm_if(_media->title == NULL, MEDIA_CONTENT_ERROR_OUT_OF_MEMORY, "OUT_OF_MEMORY");
			}
			else
			{
				_media->title = NULL;
			}
			break;
		case MEDIA_INFO_STORAGE_UUID:
			SAFE_FREE(_media->storage_uuid);
			if(STRING_VALID(str_data))
			{
				_media->storage_uuid = strdup(str_data);
				media_content_retvm_if(_media->storage_uuid == NULL, MEDIA_CONTENT_ERROR_OUT_OF_MEMORY, "OUT_OF_MEMORY");
			}
			else
			{
				_media->storage_uuid = NULL;
			}
			break;
		case MEDIA_INFO_ALBUM:
			if(_media->media_type == MEDIA_CONTENT_TYPE_VIDEO)
			{
				SAFE_FREE(_media->video_meta->album);
				if(STRING_VALID(str_data))
				{
					_media->video_meta->album = strdup(str_data);
					media_content_retvm_if(_media->video_meta->album == NULL, MEDIA_CONTENT_ERROR_OUT_OF_MEMORY, "OUT_OF_MEMORY");
				}
				else
				{
					_media->video_meta->album = NULL;
				}
			}
			else if((_media->media_type == MEDIA_CONTENT_TYPE_MUSIC) || (_media->media_type == MEDIA_CONTENT_TYPE_SOUND))
			{
				SAFE_FREE(_media->audio_meta->album);
				if(STRING_VALID(str_data))
				{
					_media->audio_meta->album = strdup(str_data);
					media_content_retvm_if(_media->audio_meta->album == NULL, MEDIA_CONTENT_ERROR_OUT_OF_MEMORY, "OUT_OF_MEMORY");
				}
				else
				{
					_media->audio_meta->album = NULL;
				}
			}
			else
			{
				media_content_error("Invalid media type");
				return MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
			}
			break;
		case MEDIA_INFO_ARTIST:
			if(_media->media_type == MEDIA_CONTENT_TYPE_VIDEO)
			{
				SAFE_FREE(_media->video_meta->artist);
				if(STRING_VALID(str_data))
				{
					_media->video_meta->artist = strdup(str_data);
					media_content_retvm_if(_media->video_meta->artist == NULL, MEDIA_CONTENT_ERROR_OUT_OF_MEMORY, "OUT_OF_MEMORY");
				}
				else
				{
					_media->video_meta->artist = NULL;
				}
			}
			else if((_media->media_type == MEDIA_CONTENT_TYPE_MUSIC) || (_media->media_type == MEDIA_CONTENT_TYPE_SOUND))
			{
				SAFE_FREE(_media->audio_meta->artist);
				if(STRING_VALID(str_data))
				{
					_media->audio_meta->artist = strdup(str_data);
					media_content_retvm_if(_media->audio_meta->artist == NULL, MEDIA_CONTENT_ERROR_OUT_OF_MEMORY, "OUT_OF_MEMORY");
				}
				else
				{
					_media->audio_meta->artist = NULL;
				}
			}
			else
			{
				media_content_error("Invalid media type");
				return MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
			}
			break;
		case MEDIA_INFO_GENRE:
			if(_media->media_type == MEDIA_CONTENT_TYPE_VIDEO)
			{
				SAFE_FREE(_media->video_meta->genre);
				if(STRING_VALID(str_data))
				{
					_media->video_meta->genre = strdup(str_data);
					media_content_retvm_if(_media->video_meta->genre == NULL, MEDIA_CONTENT_ERROR_OUT_OF_MEMORY, "OUT_OF_MEMORY");
				}
				else
				{
					_media->video_meta->genre = NULL;
				}
			}
			else if((_media->media_type == MEDIA_CONTENT_TYPE_MUSIC) || (_media->media_type == MEDIA_CONTENT_TYPE_SOUND))
			{
				SAFE_FREE(_media->audio_meta->genre);
				if(STRING_VALID(str_data))
				{
					_media->audio_meta->genre = strdup(str_data);
					media_content_retvm_if(_media->audio_meta->genre == NULL, MEDIA_CONTENT_ERROR_OUT_OF_MEMORY, "OUT_OF_MEMORY");
				}
				else
				{
					_media->audio_meta->genre = NULL;
				}
			}
			else
			{
				media_content_error("Invalid media type");
				return MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
			}
			break;
		case MEDIA_INFO_RECORDED_DATE:
			if(_media->media_type == MEDIA_CONTENT_TYPE_VIDEO)
			{
				SAFE_FREE(_media->video_meta->recorded_date);
				if(STRING_VALID(str_data))
				{
					_media->video_meta->recorded_date = strdup(str_data);
					media_content_retvm_if(_media->video_meta->recorded_date == NULL, MEDIA_CONTENT_ERROR_OUT_OF_MEMORY, "OUT_OF_MEMORY");
				}
				else
				{
					_media->video_meta->recorded_date = NULL;
				}
			}
			else if((_media->media_type == MEDIA_CONTENT_TYPE_MUSIC) || (_media->media_type == MEDIA_CONTENT_TYPE_SOUND))
			{
				SAFE_FREE(_media->audio_meta->recorded_date);
				if(STRING_VALID(str_data))
				{
					_media->audio_meta->recorded_date = strdup(str_data);
					media_content_retvm_if(_media->audio_meta->recorded_date == NULL, MEDIA_CONTENT_ERROR_OUT_OF_MEMORY, "OUT_OF_MEMORY");
				}
				else
				{
					_media->audio_meta->recorded_date = NULL;
				}
			}
			else
			{
				media_content_error("Invalid media type");
				return MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
			}
			break;
		default:
			media_content_error("INVALID_PARAMETER(0x%08x)", MEDIA_CONTENT_ERROR_INVALID_PARAMETER);
			return MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
	}

	return MEDIA_CONTENT_ERROR_NONE;
}

int media_info_insert_to_db_with_data(media_info_h media, media_info_h *info)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;

	media_info_s *_media = (media_info_s*)media;

	media_content_retvm_if(_media == NULL, MEDIA_CONTENT_ERROR_INVALID_PARAMETER, "Invalid media");
	media_content_retvm_if(!STRING_VALID(_media->file_path), MEDIA_CONTENT_ERROR_INVALID_PARAMETER, "Invalid file_path");
	media_content_retvm_if(!STRING_VALID(_media->mime_type), MEDIA_CONTENT_ERROR_INVALID_PARAMETER, "Invalid mime_type");
	media_content_retvm_if(!STRING_VALID(_media->storage_uuid), MEDIA_CONTENT_ERROR_INVALID_PARAMETER, "Invalid storage_uuid");
	media_content_retvm_if(!STRING_VALID(_media->title), MEDIA_CONTENT_ERROR_INVALID_PARAMETER, "Invalid title");
	media_content_retvm_if(_media->storage_type < 0, MEDIA_CONTENT_ERROR_INVALID_PARAMETER, "Invalid storage_type [%d]", _media->storage_type);
	media_content_retvm_if(_media->size <= 0, MEDIA_CONTENT_ERROR_INVALID_PARAMETER, "Invalid size [%d]", _media->size);
	media_content_retvm_if(_media->modified_time <= 0, MEDIA_CONTENT_ERROR_INVALID_PARAMETER, "Invalid modified_time [%d]", _media->modified_time);

	if ((_media->media_type < MEDIA_CONTENT_TYPE_IMAGE) || (_media->media_type > MEDIA_CONTENT_TYPE_OTHERS))
	{
		media_content_error("invalid media type [%d]", _media->media_type);
		return MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
	}

	media_content_sec_debug("storage[%d], path[%s], media_type[%d]", _media->storage_type, _media->file_path, _media->media_type);

	media_svc_content_info_s *svc_content_info = NULL;

	ret = __media_info_map_data_usr_to_svc(_media, &svc_content_info, _media->storage_type);
	media_content_retvm_if(ret != MEDIA_CONTENT_ERROR_NONE, ret, "__media_info_map_data_usr_to_svc fail");

	if(svc_content_info == NULL)
	{
		media_content_error("INVALID_PARAMETER(0x%08x)", MEDIA_CONTENT_ERROR_INVALID_PARAMETER);
		return MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
	}

	ret = media_svc_insert_item_immediately_with_data(_content_get_db_handle(), svc_content_info);
	if(ret != MS_MEDIA_ERR_NONE) {
		media_content_sec_error("media_svc_insert_item_immediately_with_data failed : %d (%s)", ret, _media->file_path);
		media_svc_destroy_content_info(svc_content_info);
		SAFE_FREE(svc_content_info);
		return _content_error_capi(MEDIA_CONTENT_TYPE, ret);
	}
	/*free the svc_content_info*/
	media_svc_destroy_content_info(svc_content_info);
	SAFE_FREE(svc_content_info);

	if(info != NULL)
	{
		media_info_s *_get_media = (media_info_s*)calloc(1, sizeof(media_info_s));
		media_content_retvm_if(_get_media == NULL, MEDIA_CONTENT_ERROR_OUT_OF_MEMORY, "OUT_OF_MEMORY");

		ret = _media_info_get_media_info_from_db(_media->file_path, _media->storage_uuid, (media_info_h)_get_media);

		*info = (media_info_h)_get_media;
	}

	return ret;
}

int media_info_create_handle(media_info_h *media)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;

	if(media == NULL)
	{
		media_content_error("INVALID_PARAMETER(0x%08x)", MEDIA_CONTENT_ERROR_INVALID_PARAMETER);
		return MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
	}

	media_info_s *_media = (media_info_s*)calloc(1,sizeof(media_info_s));
	media_content_retvm_if(_media == NULL, MEDIA_CONTENT_ERROR_OUT_OF_MEMORY, "OUT_OF_MEMORY");

	_media->audio_meta = (audio_meta_s *)calloc(1, sizeof(audio_meta_s));
	if(_media->audio_meta == NULL)
	{
		SAFE_FREE(_media);
		media_content_error("OUT_OF_MEMORY(0x%08x)", MEDIA_CONTENT_ERROR_OUT_OF_MEMORY);
		return MEDIA_CONTENT_ERROR_OUT_OF_MEMORY;
	}

	_media->video_meta = (video_meta_s *)calloc(1, sizeof(video_meta_s));
	if(_media->video_meta == NULL)
	{
		SAFE_FREE(_media->audio_meta);
		SAFE_FREE(_media);
		media_content_error("OUT_OF_MEMORY(0x%08x)", MEDIA_CONTENT_ERROR_OUT_OF_MEMORY);
		return MEDIA_CONTENT_ERROR_OUT_OF_MEMORY;
	}

	_media->image_meta = (image_meta_s *)calloc(1, sizeof(image_meta_s));
	if(_media->image_meta == NULL)
	{
		SAFE_FREE(_media->audio_meta);
		SAFE_FREE(_media->video_meta);
		SAFE_FREE(_media);
		media_content_error("OUT_OF_MEMORY(0x%08x)", MEDIA_CONTENT_ERROR_OUT_OF_MEMORY);
		return MEDIA_CONTENT_ERROR_OUT_OF_MEMORY;
	}

	_media->storage_type = -1;
	_media->media_type = -1;
	_media->modified_time = -1;
	_media->size = -1;

	*media = (media_info_h)_media;

	return ret;
}

int media_info_set_path(media_info_h media, const char *path)
{
	return __media_info_set_str_data(media, MEDIA_INFO_PATH, path);
}

int media_info_set_mime_type(media_info_h media, const char *mime_type)
{
	return __media_info_set_str_data(media, MEDIA_INFO_MIME_TYPE, mime_type);
}

int media_info_set_title(media_info_h media, const char *title)
{
	return __media_info_set_str_data(media, MEDIA_INFO_TITLE, title);
}

int media_info_set_album(media_info_h media, const char *album)
{
	return __media_info_set_str_data(media, MEDIA_INFO_ALBUM, album);
}

int media_info_set_artist(media_info_h media, const char *artist)
{
	return __media_info_set_str_data(media, MEDIA_INFO_ARTIST, artist);
}

int media_info_set_genre(media_info_h media, const char *genre)
{
	return __media_info_set_str_data(media, MEDIA_INFO_GENRE, genre);
}

int media_info_set_recorded_date(media_info_h media, const char *recorded_date)
{
	return __media_info_set_str_data(media, MEDIA_INFO_RECORDED_DATE, recorded_date);
}

int media_info_set_thumbnail_path(media_info_h media, const char *thumbnail_path)
{
	return __media_info_set_str_data(media, MEDIA_INFO_THUMBNAIL_PATH, thumbnail_path);
}

int media_info_set_storage_id(media_info_h media, const char *storage_id)
{
	return __media_info_set_str_data(media, MEDIA_INFO_STORAGE_UUID, storage_id);
}

int media_info_set_size(media_info_h media, unsigned long long size)
{
	media_info_s *_media = (media_info_s*)media;

	if(_media == NULL)
	{
		media_content_error("INVALID_PARAMETER(0x%08x)", MEDIA_CONTENT_ERROR_INVALID_PARAMETER);
		return MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
	}

	_media->size = size;

	return MEDIA_CONTENT_ERROR_NONE;
}

int media_info_set_modified_time(media_info_h media, time_t modified_time)
{
	media_info_s *_media = (media_info_s*)media;

	if(_media == NULL)
	{
		media_content_error("INVALID_PARAMETER(0x%08x)", MEDIA_CONTENT_ERROR_INVALID_PARAMETER);
		return MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
	}

	_media->modified_time = modified_time;

	return MEDIA_CONTENT_ERROR_NONE;
}

int media_info_set_media_type(media_info_h media, media_content_type_e type)
{
	media_info_s *_media = (media_info_s*)media;

	if(_media == NULL)
	{
		media_content_error("INVALID_PARAMETER(0x%08x)", MEDIA_CONTENT_ERROR_INVALID_PARAMETER);
		return MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
	}

	_media->media_type = type;

	return MEDIA_CONTENT_ERROR_NONE;
}

int media_info_set_duration(media_info_h media, int duration)
{
	media_info_s *_media = (media_info_s*)media;

	if(_media == NULL)
	{
		media_content_error("INVALID_PARAMETER(0x%08x)", MEDIA_CONTENT_ERROR_INVALID_PARAMETER);
		return MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
	}

	if(_media->media_type == MEDIA_CONTENT_TYPE_VIDEO)
		_media->video_meta->duration = duration;
	else if((_media->media_type == MEDIA_CONTENT_TYPE_MUSIC) || (_media->media_type == MEDIA_CONTENT_TYPE_SOUND))
		_media->audio_meta->duration = duration;
	else
	{
		media_content_error("Invalid media type");
		return MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
	}

	return MEDIA_CONTENT_ERROR_NONE;
}

int media_info_set_width(media_info_h media, int width)
{
	media_info_s *_media = (media_info_s*)media;

	if(_media == NULL)
	{
		media_content_error("INVALID_PARAMETER(0x%08x)", MEDIA_CONTENT_ERROR_INVALID_PARAMETER);
		return MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
	}

	if(_media->media_type == MEDIA_CONTENT_TYPE_VIDEO)
		_media->video_meta->width = width;
	else if(_media->media_type == MEDIA_CONTENT_TYPE_IMAGE)
		_media->image_meta->width = width;
	else
	{
		media_content_error("Invalid media type");
		return MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
	}

	return MEDIA_CONTENT_ERROR_NONE;
}

int media_info_set_height(media_info_h media, int height)
{
	media_info_s *_media = (media_info_s*)media;

	if(_media == NULL)
	{
		media_content_error("INVALID_PARAMETER(0x%08x)", MEDIA_CONTENT_ERROR_INVALID_PARAMETER);
		return MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
	}

	if(_media->media_type == MEDIA_CONTENT_TYPE_VIDEO)
		_media->video_meta->height = height;
	else if(_media->media_type == MEDIA_CONTENT_TYPE_IMAGE)
		_media->image_meta->height = height;
	else
	{
		media_content_error("Invalid media type");
		return MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
	}

	return MEDIA_CONTENT_ERROR_NONE;
}

int media_info_set_storage_type(media_info_h media, media_content_storage_e storage_type)
{
	media_info_s *_media = (media_info_s*)media;

	if(_media == NULL)
	{
		media_content_error("INVALID_PARAMETER(0x%08x)", MEDIA_CONTENT_ERROR_INVALID_PARAMETER);
		return MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
	}

	_media->storage_type = storage_type;

	return MEDIA_CONTENT_ERROR_NONE;
}
