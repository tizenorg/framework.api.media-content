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
#include <media_util_private.h>
#include <media_content_internal.h>

#include <unicode/uscript.h>
#include <unicode/uloc.h>
#include <unicode/ucol.h>
#include <vconf.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <fcntl.h>


static attribute_h g_attr_handle = NULL;
static attribute_h g_alias_attr_handle = NULL;
static MediaSvcHandle *db_handle = NULL;
static int ref_count = 0;
static GMutex db_mutex;

static media_noti_cb_s *g_noti_info = NULL;

static int __media_content_create_attr_handle(void);
static int __media_content_create_alias_attr_handle(void);
static int __media_content_create_attribute_handle(void);
static int __media_content_destroy_attribute_handle(void);

static int __media_content_create_attr_handle(void)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;

	ret = _media_filter_attribute_create(&g_attr_handle);
	media_content_retv_if(ret != MEDIA_CONTENT_ERROR_NONE, ret);

	/* Media Info */
	ret = _media_filter_attribute_add(g_attr_handle, MEDIA_ID, DB_FIELD_MEDIA_ID);
	media_content_retv_if(ret != MEDIA_CONTENT_ERROR_NONE, ret);

	ret = _media_filter_attribute_add(g_attr_handle, MEDIA_PATH, DB_FIELD_MEDIA_PATH);
	media_content_retv_if(ret != MEDIA_CONTENT_ERROR_NONE, ret);

	ret = _media_filter_attribute_add(g_attr_handle, MEDIA_DISPLAY_NAME, DB_FIELD_MEDIA_DISPLAY_NAME);
	media_content_retv_if(ret != MEDIA_CONTENT_ERROR_NONE, ret);

	ret = _media_filter_attribute_add(g_attr_handle, MEDIA_TYPE, DB_FIELD_MEDIA_TYPE);
	media_content_retv_if(ret != MEDIA_CONTENT_ERROR_NONE, ret);

	ret = _media_filter_attribute_add(g_attr_handle, MEDIA_MIME_TYPE, DB_FIELD_MEDIA_MIME_TYPE);
	media_content_retv_if(ret != MEDIA_CONTENT_ERROR_NONE, ret);

	ret = _media_filter_attribute_add(g_attr_handle, MEDIA_SIZE, DB_FIELD_MEDIA_SIZE);
	media_content_retv_if(ret != MEDIA_CONTENT_ERROR_NONE, ret);

	ret = _media_filter_attribute_add(g_attr_handle, MEDIA_ADDED_TIME, DB_FIELD_MEDIA_ADDED_TIME);
	media_content_retv_if(ret != MEDIA_CONTENT_ERROR_NONE, ret);

	ret = _media_filter_attribute_add(g_attr_handle, MEDIA_MODIFIED_TIME, DB_FIELD_MEDIA_MODIFIED_TIME);
	media_content_retv_if(ret != MEDIA_CONTENT_ERROR_NONE, ret);

	ret = _media_filter_attribute_add(g_attr_handle, MEDIA_TIMELINE, DB_FIELD_MEDIA_TIMELINE);
	media_content_retv_if(ret != MEDIA_CONTENT_ERROR_NONE, ret);

	ret = _media_filter_attribute_add(g_attr_handle, MEDIA_THUMBNAIL_PATH, DB_FIELD_MEDIA_THUMBNAIL_PATH);
	media_content_retv_if(ret != MEDIA_CONTENT_ERROR_NONE, ret);

	ret = _media_filter_attribute_add(g_attr_handle, MEDIA_TITLE, DB_FIELD_MEDIA_TITLE);
	media_content_retv_if(ret != MEDIA_CONTENT_ERROR_NONE, ret);

	ret = _media_filter_attribute_add(g_attr_handle, MEDIA_ALBUM, DB_FIELD_MEDIA_ALBUM);
	media_content_retv_if(ret != MEDIA_CONTENT_ERROR_NONE, ret);

	ret = _media_filter_attribute_add(g_attr_handle, MEDIA_ARTIST, DB_FIELD_MEDIA_ARTIST);
	media_content_retv_if(ret != MEDIA_CONTENT_ERROR_NONE, ret);

	ret = _media_filter_attribute_add(g_attr_handle, MEDIA_ALBUM_ARTIST, DB_FIELD_MEDIA_ALBUM_ARTIST);
	media_content_retv_if(ret != MEDIA_CONTENT_ERROR_NONE, ret);

	ret = _media_filter_attribute_add(g_attr_handle, MEDIA_GENRE, DB_FIELD_MEDIA_GENRE);
	media_content_retv_if(ret != MEDIA_CONTENT_ERROR_NONE, ret);

	ret = _media_filter_attribute_add(g_attr_handle, MEDIA_COMPOSER, DB_FIELD_MEDIA_COMPOSER);
	media_content_retv_if(ret != MEDIA_CONTENT_ERROR_NONE, ret);

	ret = _media_filter_attribute_add(g_attr_handle, MEDIA_YEAR, DB_FIELD_MEDIA_YEAR);
	media_content_retv_if(ret != MEDIA_CONTENT_ERROR_NONE, ret);

	ret = _media_filter_attribute_add(g_attr_handle, MEDIA_RECORDED_DATE, DB_FIELD_MEDIA_RECORDED_DATE);
	media_content_retv_if(ret != MEDIA_CONTENT_ERROR_NONE, ret);

	ret = _media_filter_attribute_add(g_attr_handle, MEDIA_COPYRIGHT, DB_FIELD_MEDIA_COPYRIGHT);
	media_content_retv_if(ret != MEDIA_CONTENT_ERROR_NONE, ret);

	ret = _media_filter_attribute_add(g_attr_handle, MEDIA_TRACK_NUM, DB_FIELD_MEDIA_TRACK_NUM);
	media_content_retv_if(ret != MEDIA_CONTENT_ERROR_NONE, ret);

	ret = _media_filter_attribute_add(g_attr_handle, MEDIA_DESCRIPTION, DB_FIELD_MEDIA_DESCRIPTION);
	media_content_retv_if(ret != MEDIA_CONTENT_ERROR_NONE, ret);

	ret = _media_filter_attribute_add(g_attr_handle, MEDIA_BITRATE, DB_FIELD_MEDIA_BITRATE);
	media_content_retv_if(ret != MEDIA_CONTENT_ERROR_NONE, ret);

	ret = _media_filter_attribute_add(g_attr_handle, MEDIA_BITPERSAMPLE, DB_FIELD_MEDIA_BITPERSAMPLE);
	media_content_retv_if(ret != MEDIA_CONTENT_ERROR_NONE, ret);

	ret = _media_filter_attribute_add(g_attr_handle, MEDIA_SAMPLERATE, DB_FIELD_MEDIA_SAMPLERATE);
	media_content_retv_if(ret != MEDIA_CONTENT_ERROR_NONE, ret);

	ret = _media_filter_attribute_add(g_attr_handle, MEDIA_CHANNEL, DB_FIELD_MEDIA_CHANNEL);
	media_content_retv_if(ret != MEDIA_CONTENT_ERROR_NONE, ret);

	ret = _media_filter_attribute_add(g_attr_handle, MEDIA_DURATION, DB_FIELD_MEDIA_DURATION);
	media_content_retv_if(ret != MEDIA_CONTENT_ERROR_NONE, ret);

	ret = _media_filter_attribute_add(g_attr_handle, MEDIA_LONGITUDE, DB_FIELD_MEDIA_LONGITUDE);
	media_content_retv_if(ret != MEDIA_CONTENT_ERROR_NONE, ret);

	ret = _media_filter_attribute_add(g_attr_handle, MEDIA_LATITUDE, DB_FIELD_MEDIA_LATITUDE);
	media_content_retv_if(ret != MEDIA_CONTENT_ERROR_NONE, ret);

	ret = _media_filter_attribute_add(g_attr_handle, MEDIA_ALTITUDE, DB_FIELD_MEDIA_ALTITUDE);
	media_content_retv_if(ret != MEDIA_CONTENT_ERROR_NONE, ret);

	ret = _media_filter_attribute_add(g_attr_handle, MEDIA_WIDTH, DB_FIELD_MEDIA_WIDTH);
	media_content_retv_if(ret != MEDIA_CONTENT_ERROR_NONE, ret);

	ret = _media_filter_attribute_add(g_attr_handle, MEDIA_HEIGHT, DB_FIELD_MEDIA_HEIGHT);
	media_content_retv_if(ret != MEDIA_CONTENT_ERROR_NONE, ret);

	ret = _media_filter_attribute_add(g_attr_handle, MEDIA_DATETAKEN, DB_FIELD_MEDIA_DATETAKEN);
	media_content_retv_if(ret != MEDIA_CONTENT_ERROR_NONE, ret);

	ret = _media_filter_attribute_add(g_attr_handle, MEDIA_ORIENTATION, DB_FIELD_MEDIA_ORIENTATION);
	media_content_retv_if(ret != MEDIA_CONTENT_ERROR_NONE, ret);

	ret = _media_filter_attribute_add(g_attr_handle, MEDIA_BURST_ID, DB_FIELD_MEDIA_BURST_ID);
	media_content_retv_if(ret != MEDIA_CONTENT_ERROR_NONE, ret);

	ret = _media_filter_attribute_add(g_attr_handle, MEDIA_PLAYED_COUNT, DB_FIELD_MEDIA_PLAYED_COUNT);
	media_content_retv_if(ret != MEDIA_CONTENT_ERROR_NONE, ret);

	ret = _media_filter_attribute_add(g_attr_handle, MEDIA_LAST_PLAYED_TIME, DB_FIELD_MEDIA_LAST_PLAYED_TIME);
	media_content_retv_if(ret != MEDIA_CONTENT_ERROR_NONE, ret);

	ret = _media_filter_attribute_add(g_attr_handle, MEDIA_LAST_PLAYED_POSITION, DB_FIELD_MEDIA_LAST_PLAYED_POSITION);
	media_content_retv_if(ret != MEDIA_CONTENT_ERROR_NONE, ret);

	ret = _media_filter_attribute_add(g_attr_handle, MEDIA_RATING, DB_FIELD_MEDIA_RATING);
	media_content_retv_if(ret != MEDIA_CONTENT_ERROR_NONE, ret);

	ret = _media_filter_attribute_add(g_attr_handle, MEDIA_FAVOURITE, DB_FIELD_MEDIA_FAVOURITE);
	media_content_retv_if(ret != MEDIA_CONTENT_ERROR_NONE, ret);

	ret = _media_filter_attribute_add(g_attr_handle, MEDIA_AUTHOR, DB_FIELD_MEDIA_AUTHOR);
	media_content_retv_if(ret != MEDIA_CONTENT_ERROR_NONE, ret);

	ret = _media_filter_attribute_add(g_attr_handle, MEDIA_PROVIDER, DB_FIELD_MEDIA_PROVIDER);
	media_content_retv_if(ret != MEDIA_CONTENT_ERROR_NONE, ret);

	ret = _media_filter_attribute_add(g_attr_handle, MEDIA_CONTENT_NAME, DB_FIELD_MEDIA_CONTENT_NAME);
	media_content_retv_if(ret != MEDIA_CONTENT_ERROR_NONE, ret);

	ret = _media_filter_attribute_add(g_attr_handle, MEDIA_CATEGORY, DB_FIELD_MEDIA_CATEGORY);
	media_content_retv_if(ret != MEDIA_CONTENT_ERROR_NONE, ret);

	ret = _media_filter_attribute_add(g_attr_handle, MEDIA_LOCATION_TAG, DB_FIELD_MEDIA_LOCATION_TAG);
	media_content_retv_if(ret != MEDIA_CONTENT_ERROR_NONE, ret);

	ret = _media_filter_attribute_add(g_attr_handle, MEDIA_AGE_RATING, DB_FIELD_MEDIA_AGE_RATING);
	media_content_retv_if(ret != MEDIA_CONTENT_ERROR_NONE, ret);

	ret = _media_filter_attribute_add(g_attr_handle, MEDIA_KEYWORD, DB_FIELD_MEDIA_KEYWORD);
	media_content_retv_if(ret != MEDIA_CONTENT_ERROR_NONE, ret);

	ret = _media_filter_attribute_add(g_attr_handle, MEDIA_WEATHER, DB_FIELD_MEDIA_WEATHER);
	media_content_retv_if(ret != MEDIA_CONTENT_ERROR_NONE, ret);

	ret = _media_filter_attribute_add(g_attr_handle, MEDIA_IS_DRM, DB_FIELD_MEDIA_IS_DRM);
	media_content_retv_if(ret != MEDIA_CONTENT_ERROR_NONE, ret);

	ret = _media_filter_attribute_add(g_attr_handle, MEDIA_STORAGE_TYPE, DB_FIELD_MEDIA_STORAGE_TYPE);
	media_content_retv_if(ret != MEDIA_CONTENT_ERROR_NONE, ret);

	/* Pinyin*/
	ret = _media_filter_attribute_add(g_attr_handle, MEDIA_FILE_NAME_PINYIN, DB_FIELD_MEDIA_FILE_NAME_PINYIN);
	media_content_retv_if(ret != MEDIA_CONTENT_ERROR_NONE, ret);

	ret = _media_filter_attribute_add(g_attr_handle, MEDIA_TITLE_PINYIN, DB_FIELD_MEDIA_TITLE_PINYIN);
	media_content_retv_if(ret != MEDIA_CONTENT_ERROR_NONE, ret);

	ret = _media_filter_attribute_add(g_attr_handle, MEDIA_ALBUM_PINYIN, DB_FIELD_MEDIA_ALBUM_PINYIN);
	media_content_retv_if(ret != MEDIA_CONTENT_ERROR_NONE, ret);

	ret = _media_filter_attribute_add(g_attr_handle, MEDIA_ARTIST_PINYIN, DB_FIELD_MEDIA_ARTIST_PINYIN);
	media_content_retv_if(ret != MEDIA_CONTENT_ERROR_NONE, ret);

	ret = _media_filter_attribute_add(g_attr_handle, MEDIA_ALBUM_ARTIST_PINYIN, DB_FIELD_MEDIA_ALBUM_ARTIST_PINYIN);
	media_content_retv_if(ret != MEDIA_CONTENT_ERROR_NONE, ret);

	ret = _media_filter_attribute_add(g_attr_handle, MEDIA_GENRE_PINYIN, DB_FIELD_MEDIA_GENRE_PINYIN);
	media_content_retv_if(ret != MEDIA_CONTENT_ERROR_NONE, ret);

	ret = _media_filter_attribute_add(g_attr_handle, MEDIA_COMPOSER_PINYIN, DB_FIELD_MEDIA_COMPOSER_PINYIN);
	media_content_retv_if(ret != MEDIA_CONTENT_ERROR_NONE, ret);

	ret = _media_filter_attribute_add(g_attr_handle, MEDIA_COPYRIGHT_PINYIN, DB_FIELD_MEDIA_COPYRIGHT_PINYIN);
	media_content_retv_if(ret != MEDIA_CONTENT_ERROR_NONE, ret);

	ret = _media_filter_attribute_add(g_attr_handle, MEDIA_DESCRIPTION_PINYIN, DB_FIELD_MEDIA_DESCRIPTION_PINYIN);
	media_content_retv_if(ret != MEDIA_CONTENT_ERROR_NONE, ret);

	ret = _media_filter_attribute_add(g_attr_handle, MEDIA_AUTHOR_PINYIN, DB_FIELD_MEDIA_AUTHOR_PINYIN);
	media_content_retv_if(ret != MEDIA_CONTENT_ERROR_NONE, ret);

	ret = _media_filter_attribute_add(g_attr_handle, MEDIA_PROVIDER_PINYIN, DB_FIELD_MEDIA_PROVIDER_PINYIN);
	media_content_retv_if(ret != MEDIA_CONTENT_ERROR_NONE, ret);

	ret = _media_filter_attribute_add(g_attr_handle, MEDIA_CONTENT_NAME_PINYIN, DB_FIELD_MEDIA_CONTENT_NAME_PINYIN);
	media_content_retv_if(ret != MEDIA_CONTENT_ERROR_NONE, ret);

	ret = _media_filter_attribute_add(g_attr_handle, MEDIA_CATEGORY_PINYIN, DB_FIELD_MEDIA_CATEGORY_PINYIN);
	media_content_retv_if(ret != MEDIA_CONTENT_ERROR_NONE, ret);

	ret = _media_filter_attribute_add(g_attr_handle, MEDIA_LOCATION_TAG_PINYIN, DB_FIELD_MEDIA_LOCATION_TAG_PINYIN);
	media_content_retv_if(ret != MEDIA_CONTENT_ERROR_NONE, ret);

	ret = _media_filter_attribute_add(g_attr_handle, MEDIA_AGE_RATING_PINYIN, DB_FIELD_MEDIA_AGE_RATING_PINYIN);
	media_content_retv_if(ret != MEDIA_CONTENT_ERROR_NONE, ret);

	ret = _media_filter_attribute_add(g_attr_handle, MEDIA_KEYWORD_PINYIN, DB_FIELD_MEDIA_KEYWORD_PINYIN);
	media_content_retv_if(ret != MEDIA_CONTENT_ERROR_NONE, ret);

	/* Playlist*/
	ret = _media_filter_attribute_add(g_attr_handle, PLAYLIST_NAME, DB_FIELD_PLAYLIST_NAME);
	media_content_retv_if(ret != MEDIA_CONTENT_ERROR_NONE, ret);

	ret = _media_filter_attribute_add(g_attr_handle, PLAYLIST_MEMBER_ORDER, DB_FIELD_PLAYLIST_MEMBER_ORDER);
	media_content_retv_if(ret != MEDIA_CONTENT_ERROR_NONE, ret);

	/* Playlist View */
	ret = _media_filter_attribute_add(g_attr_handle, PLAYLIST_MEDIA_COUNT, DB_FIELD_PLAYLIST_MEDIA_COUNT);
	media_content_retv_if(ret != MEDIA_CONTENT_ERROR_NONE, ret);

	/* Tag*/
	ret = _media_filter_attribute_add(g_attr_handle, TAG_NAME, DB_FIELD_TAG_NAME);
	media_content_retv_if(ret != MEDIA_CONTENT_ERROR_NONE, ret);

	/* Tag View */
	ret = _media_filter_attribute_add(g_attr_handle, TAG_MEDIA_COUNT, DB_FIELD_TAG_MEDIA_COUNT);
	media_content_retv_if(ret != MEDIA_CONTENT_ERROR_NONE, ret);

	/* Bookmark*/
	ret = _media_filter_attribute_add(g_attr_handle, BOOKMARK_MARKED_TIME, DB_FIELD_BOOKMARK_MARKED_TIME);
	media_content_retv_if(ret != MEDIA_CONTENT_ERROR_NONE, ret);

	/* Storage*/
	ret = _media_filter_attribute_add(g_attr_handle, MEDIA_STORAGE_ID, DB_FIELD_STORAGE_ID);
	media_content_retv_if(ret != MEDIA_CONTENT_ERROR_NONE, ret);

	ret = _media_filter_attribute_add(g_attr_handle, MEDIA_STORAGE_PATH, DB_FIELD_STORAGE_PATH);
	media_content_retv_if(ret != MEDIA_CONTENT_ERROR_NONE, ret);

	return ret;
}

static int __media_content_create_alias_attr_handle(void)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;

	ret = _media_filter_attribute_create(&g_alias_attr_handle);
	media_content_retv_if(ret != MEDIA_CONTENT_ERROR_NONE, ret);

	/* Media Info */
	ret = _media_filter_attribute_add(g_alias_attr_handle, MEDIA_ID, DB_TABLE_ALIAS_MEDIA"."DB_FIELD_MEDIA_ID);
	media_content_retv_if(ret != MEDIA_CONTENT_ERROR_NONE, ret);

	ret = _media_filter_attribute_add(g_alias_attr_handle, MEDIA_PATH, DB_TABLE_ALIAS_MEDIA"."DB_FIELD_MEDIA_PATH);
	media_content_retv_if(ret != MEDIA_CONTENT_ERROR_NONE, ret);

	ret = _media_filter_attribute_add(g_alias_attr_handle, MEDIA_DISPLAY_NAME, DB_TABLE_ALIAS_MEDIA"."DB_FIELD_MEDIA_DISPLAY_NAME);
	media_content_retv_if(ret != MEDIA_CONTENT_ERROR_NONE, ret);

	ret = _media_filter_attribute_add(g_alias_attr_handle, MEDIA_TYPE, DB_TABLE_ALIAS_MEDIA"."DB_FIELD_MEDIA_TYPE);
	media_content_retv_if(ret != MEDIA_CONTENT_ERROR_NONE, ret);

	ret = _media_filter_attribute_add(g_alias_attr_handle, MEDIA_MIME_TYPE, DB_TABLE_ALIAS_MEDIA"."DB_FIELD_MEDIA_MIME_TYPE);
	media_content_retv_if(ret != MEDIA_CONTENT_ERROR_NONE, ret);

	ret = _media_filter_attribute_add(g_alias_attr_handle, MEDIA_SIZE, DB_TABLE_ALIAS_MEDIA"."DB_FIELD_MEDIA_SIZE);
	media_content_retv_if(ret != MEDIA_CONTENT_ERROR_NONE, ret);

	ret = _media_filter_attribute_add(g_alias_attr_handle, MEDIA_ADDED_TIME, DB_TABLE_ALIAS_MEDIA"."DB_FIELD_MEDIA_ADDED_TIME);
	media_content_retv_if(ret != MEDIA_CONTENT_ERROR_NONE, ret);

	ret = _media_filter_attribute_add(g_alias_attr_handle, MEDIA_MODIFIED_TIME, DB_TABLE_ALIAS_MEDIA"."DB_FIELD_MEDIA_MODIFIED_TIME);
	media_content_retv_if(ret != MEDIA_CONTENT_ERROR_NONE, ret);

	ret = _media_filter_attribute_add(g_alias_attr_handle, MEDIA_TIMELINE, DB_TABLE_ALIAS_MEDIA"."DB_FIELD_MEDIA_TIMELINE);
	media_content_retv_if(ret != MEDIA_CONTENT_ERROR_NONE, ret);

	ret = _media_filter_attribute_add(g_alias_attr_handle, MEDIA_THUMBNAIL_PATH, DB_TABLE_ALIAS_MEDIA"."DB_FIELD_MEDIA_THUMBNAIL_PATH);
	media_content_retv_if(ret != MEDIA_CONTENT_ERROR_NONE, ret);

	ret = _media_filter_attribute_add(g_alias_attr_handle, MEDIA_TITLE, DB_TABLE_ALIAS_MEDIA"."DB_FIELD_MEDIA_TITLE);
	media_content_retv_if(ret != MEDIA_CONTENT_ERROR_NONE, ret);

	ret = _media_filter_attribute_add(g_alias_attr_handle, MEDIA_ALBUM, DB_TABLE_ALIAS_MEDIA"."DB_FIELD_MEDIA_ALBUM);
	media_content_retv_if(ret != MEDIA_CONTENT_ERROR_NONE, ret);

	ret = _media_filter_attribute_add(g_alias_attr_handle, MEDIA_ARTIST, DB_TABLE_ALIAS_MEDIA"."DB_FIELD_MEDIA_ARTIST);
	media_content_retv_if(ret != MEDIA_CONTENT_ERROR_NONE, ret);

	ret = _media_filter_attribute_add(g_alias_attr_handle, MEDIA_ALBUM_ARTIST, DB_TABLE_ALIAS_MEDIA"."DB_FIELD_MEDIA_ALBUM_ARTIST);
	media_content_retv_if(ret != MEDIA_CONTENT_ERROR_NONE, ret);

	ret = _media_filter_attribute_add(g_alias_attr_handle, MEDIA_GENRE, DB_TABLE_ALIAS_MEDIA"."DB_FIELD_MEDIA_GENRE);
	media_content_retv_if(ret != MEDIA_CONTENT_ERROR_NONE, ret);

	ret = _media_filter_attribute_add(g_alias_attr_handle, MEDIA_COMPOSER, DB_TABLE_ALIAS_MEDIA"."DB_FIELD_MEDIA_COMPOSER);
	media_content_retv_if(ret != MEDIA_CONTENT_ERROR_NONE, ret);

	ret = _media_filter_attribute_add(g_alias_attr_handle, MEDIA_YEAR, DB_TABLE_ALIAS_MEDIA"."DB_FIELD_MEDIA_YEAR);
	media_content_retv_if(ret != MEDIA_CONTENT_ERROR_NONE, ret);

	ret = _media_filter_attribute_add(g_alias_attr_handle, MEDIA_RECORDED_DATE, DB_TABLE_ALIAS_MEDIA"."DB_FIELD_MEDIA_RECORDED_DATE);
	media_content_retv_if(ret != MEDIA_CONTENT_ERROR_NONE, ret);

	ret = _media_filter_attribute_add(g_alias_attr_handle, MEDIA_COPYRIGHT, DB_TABLE_ALIAS_MEDIA"."DB_FIELD_MEDIA_COPYRIGHT);
	media_content_retv_if(ret != MEDIA_CONTENT_ERROR_NONE, ret);

	ret = _media_filter_attribute_add(g_alias_attr_handle, MEDIA_TRACK_NUM, DB_TABLE_ALIAS_MEDIA"."DB_FIELD_MEDIA_TRACK_NUM);
	media_content_retv_if(ret != MEDIA_CONTENT_ERROR_NONE, ret);

	ret = _media_filter_attribute_add(g_alias_attr_handle, MEDIA_DESCRIPTION, DB_TABLE_ALIAS_MEDIA"."DB_FIELD_MEDIA_DESCRIPTION);
	media_content_retv_if(ret != MEDIA_CONTENT_ERROR_NONE, ret);

	ret = _media_filter_attribute_add(g_alias_attr_handle, MEDIA_BITRATE, DB_TABLE_ALIAS_MEDIA"."DB_FIELD_MEDIA_BITRATE);
	media_content_retv_if(ret != MEDIA_CONTENT_ERROR_NONE, ret);

	ret = _media_filter_attribute_add(g_alias_attr_handle, MEDIA_BITPERSAMPLE, DB_TABLE_ALIAS_MEDIA"."DB_FIELD_MEDIA_BITPERSAMPLE);
	media_content_retv_if(ret != MEDIA_CONTENT_ERROR_NONE, ret);

	ret = _media_filter_attribute_add(g_alias_attr_handle, MEDIA_SAMPLERATE, DB_TABLE_ALIAS_MEDIA"."DB_FIELD_MEDIA_SAMPLERATE);
	media_content_retv_if(ret != MEDIA_CONTENT_ERROR_NONE, ret);

	ret = _media_filter_attribute_add(g_alias_attr_handle, MEDIA_CHANNEL, DB_TABLE_ALIAS_MEDIA"."DB_FIELD_MEDIA_CHANNEL);
	media_content_retv_if(ret != MEDIA_CONTENT_ERROR_NONE, ret);

	ret = _media_filter_attribute_add(g_alias_attr_handle, MEDIA_DURATION, DB_TABLE_ALIAS_MEDIA"."DB_FIELD_MEDIA_DURATION);
	media_content_retv_if(ret != MEDIA_CONTENT_ERROR_NONE, ret);

	ret = _media_filter_attribute_add(g_alias_attr_handle, MEDIA_LONGITUDE, DB_TABLE_ALIAS_MEDIA"."DB_FIELD_MEDIA_LONGITUDE);
	media_content_retv_if(ret != MEDIA_CONTENT_ERROR_NONE, ret);

	ret = _media_filter_attribute_add(g_alias_attr_handle, MEDIA_LATITUDE, DB_TABLE_ALIAS_MEDIA"."DB_FIELD_MEDIA_LATITUDE);
	media_content_retv_if(ret != MEDIA_CONTENT_ERROR_NONE, ret);

	ret = _media_filter_attribute_add(g_alias_attr_handle, MEDIA_ALTITUDE, DB_TABLE_ALIAS_MEDIA"."DB_FIELD_MEDIA_ALTITUDE);
	media_content_retv_if(ret != MEDIA_CONTENT_ERROR_NONE, ret);

	ret = _media_filter_attribute_add(g_alias_attr_handle, MEDIA_WIDTH, DB_TABLE_ALIAS_MEDIA"."DB_FIELD_MEDIA_WIDTH);
	media_content_retv_if(ret != MEDIA_CONTENT_ERROR_NONE, ret);

	ret = _media_filter_attribute_add(g_alias_attr_handle, MEDIA_HEIGHT, DB_TABLE_ALIAS_MEDIA"."DB_FIELD_MEDIA_HEIGHT);
	media_content_retv_if(ret != MEDIA_CONTENT_ERROR_NONE, ret);

	ret = _media_filter_attribute_add(g_alias_attr_handle, MEDIA_DATETAKEN, DB_TABLE_ALIAS_MEDIA"."DB_FIELD_MEDIA_DATETAKEN);
	media_content_retv_if(ret != MEDIA_CONTENT_ERROR_NONE, ret);

	ret = _media_filter_attribute_add(g_alias_attr_handle, MEDIA_ORIENTATION, DB_TABLE_ALIAS_MEDIA"."DB_FIELD_MEDIA_ORIENTATION);
	media_content_retv_if(ret != MEDIA_CONTENT_ERROR_NONE, ret);

	ret = _media_filter_attribute_add(g_alias_attr_handle, MEDIA_BURST_ID, DB_TABLE_ALIAS_MEDIA"."DB_FIELD_MEDIA_BURST_ID);
	media_content_retv_if(ret != MEDIA_CONTENT_ERROR_NONE, ret);

	ret = _media_filter_attribute_add(g_alias_attr_handle, MEDIA_PLAYED_COUNT, DB_TABLE_ALIAS_MEDIA"."DB_FIELD_MEDIA_PLAYED_COUNT);
	media_content_retv_if(ret != MEDIA_CONTENT_ERROR_NONE, ret);

	ret = _media_filter_attribute_add(g_alias_attr_handle, MEDIA_LAST_PLAYED_TIME, DB_TABLE_ALIAS_MEDIA"."DB_FIELD_MEDIA_LAST_PLAYED_TIME);
	media_content_retv_if(ret != MEDIA_CONTENT_ERROR_NONE, ret);

	ret = _media_filter_attribute_add(g_alias_attr_handle, MEDIA_LAST_PLAYED_POSITION, DB_TABLE_ALIAS_MEDIA"."DB_FIELD_MEDIA_LAST_PLAYED_POSITION);
	media_content_retv_if(ret != MEDIA_CONTENT_ERROR_NONE, ret);

	ret = _media_filter_attribute_add(g_alias_attr_handle, MEDIA_RATING, DB_TABLE_ALIAS_MEDIA"."DB_FIELD_MEDIA_RATING);
	media_content_retv_if(ret != MEDIA_CONTENT_ERROR_NONE, ret);

	ret = _media_filter_attribute_add(g_alias_attr_handle, MEDIA_FAVOURITE, DB_TABLE_ALIAS_MEDIA"."DB_FIELD_MEDIA_FAVOURITE);
	media_content_retv_if(ret != MEDIA_CONTENT_ERROR_NONE, ret);

	ret = _media_filter_attribute_add(g_alias_attr_handle, MEDIA_AUTHOR, DB_TABLE_ALIAS_MEDIA"."DB_FIELD_MEDIA_AUTHOR);
	media_content_retv_if(ret != MEDIA_CONTENT_ERROR_NONE, ret);

	ret = _media_filter_attribute_add(g_alias_attr_handle, MEDIA_PROVIDER, DB_TABLE_ALIAS_MEDIA"."DB_FIELD_MEDIA_PROVIDER);
	media_content_retv_if(ret != MEDIA_CONTENT_ERROR_NONE, ret);

	ret = _media_filter_attribute_add(g_alias_attr_handle, MEDIA_CONTENT_NAME, DB_TABLE_ALIAS_MEDIA"."DB_FIELD_MEDIA_CONTENT_NAME);
	media_content_retv_if(ret != MEDIA_CONTENT_ERROR_NONE, ret);

	ret = _media_filter_attribute_add(g_alias_attr_handle, MEDIA_CATEGORY, DB_TABLE_ALIAS_MEDIA"."DB_FIELD_MEDIA_CATEGORY);
	media_content_retv_if(ret != MEDIA_CONTENT_ERROR_NONE, ret);

	ret = _media_filter_attribute_add(g_alias_attr_handle, MEDIA_LOCATION_TAG, DB_TABLE_ALIAS_MEDIA"."DB_FIELD_MEDIA_LOCATION_TAG);
	media_content_retv_if(ret != MEDIA_CONTENT_ERROR_NONE, ret);

	ret = _media_filter_attribute_add(g_alias_attr_handle, MEDIA_AGE_RATING, DB_TABLE_ALIAS_MEDIA"."DB_FIELD_MEDIA_AGE_RATING);
	media_content_retv_if(ret != MEDIA_CONTENT_ERROR_NONE, ret);

	ret = _media_filter_attribute_add(g_alias_attr_handle, MEDIA_KEYWORD, DB_TABLE_ALIAS_MEDIA"."DB_FIELD_MEDIA_KEYWORD);
	media_content_retv_if(ret != MEDIA_CONTENT_ERROR_NONE, ret);

	ret = _media_filter_attribute_add(g_alias_attr_handle, MEDIA_WEATHER, DB_TABLE_ALIAS_MEDIA"."DB_FIELD_MEDIA_WEATHER);
	media_content_retv_if(ret != MEDIA_CONTENT_ERROR_NONE, ret);

	ret = _media_filter_attribute_add(g_alias_attr_handle, MEDIA_IS_DRM, DB_TABLE_ALIAS_MEDIA"."DB_FIELD_MEDIA_IS_DRM);
	media_content_retv_if(ret != MEDIA_CONTENT_ERROR_NONE, ret);

	ret = _media_filter_attribute_add(g_alias_attr_handle, MEDIA_STORAGE_TYPE, DB_TABLE_ALIAS_MEDIA"."DB_FIELD_MEDIA_STORAGE_TYPE);
	media_content_retv_if(ret != MEDIA_CONTENT_ERROR_NONE, ret);

	/* Pinyin*/
	ret = _media_filter_attribute_add(g_alias_attr_handle, MEDIA_FILE_NAME_PINYIN, DB_TABLE_ALIAS_MEDIA"."DB_FIELD_MEDIA_FILE_NAME_PINYIN);
	media_content_retv_if(ret != MEDIA_CONTENT_ERROR_NONE, ret);

	ret = _media_filter_attribute_add(g_alias_attr_handle, MEDIA_TITLE_PINYIN, DB_TABLE_ALIAS_MEDIA"."DB_FIELD_MEDIA_TITLE_PINYIN);
	media_content_retv_if(ret != MEDIA_CONTENT_ERROR_NONE, ret);

	ret = _media_filter_attribute_add(g_alias_attr_handle, MEDIA_ALBUM_PINYIN, DB_TABLE_ALIAS_MEDIA"."DB_FIELD_MEDIA_ALBUM_PINYIN);
	media_content_retv_if(ret != MEDIA_CONTENT_ERROR_NONE, ret);

	ret = _media_filter_attribute_add(g_alias_attr_handle, MEDIA_ARTIST_PINYIN, DB_TABLE_ALIAS_MEDIA"."DB_FIELD_MEDIA_ARTIST_PINYIN);
	media_content_retv_if(ret != MEDIA_CONTENT_ERROR_NONE, ret);

	ret = _media_filter_attribute_add(g_alias_attr_handle, MEDIA_ALBUM_ARTIST_PINYIN, DB_TABLE_ALIAS_MEDIA"."DB_FIELD_MEDIA_ALBUM_ARTIST_PINYIN);
	media_content_retv_if(ret != MEDIA_CONTENT_ERROR_NONE, ret);

	ret = _media_filter_attribute_add(g_alias_attr_handle, MEDIA_GENRE_PINYIN, DB_TABLE_ALIAS_MEDIA"."DB_FIELD_MEDIA_GENRE_PINYIN);
	media_content_retv_if(ret != MEDIA_CONTENT_ERROR_NONE, ret);

	ret = _media_filter_attribute_add(g_alias_attr_handle, MEDIA_COMPOSER_PINYIN, DB_TABLE_ALIAS_MEDIA"."DB_FIELD_MEDIA_COMPOSER_PINYIN);
	media_content_retv_if(ret != MEDIA_CONTENT_ERROR_NONE, ret);

	ret = _media_filter_attribute_add(g_alias_attr_handle, MEDIA_COPYRIGHT_PINYIN, DB_TABLE_ALIAS_MEDIA"."DB_FIELD_MEDIA_COPYRIGHT_PINYIN);
	media_content_retv_if(ret != MEDIA_CONTENT_ERROR_NONE, ret);

	ret = _media_filter_attribute_add(g_alias_attr_handle, MEDIA_DESCRIPTION_PINYIN, DB_TABLE_ALIAS_MEDIA"."DB_FIELD_MEDIA_DESCRIPTION_PINYIN);
	media_content_retv_if(ret != MEDIA_CONTENT_ERROR_NONE, ret);

	ret = _media_filter_attribute_add(g_alias_attr_handle, MEDIA_AUTHOR_PINYIN, DB_TABLE_ALIAS_MEDIA"."DB_FIELD_MEDIA_AUTHOR_PINYIN);
	media_content_retv_if(ret != MEDIA_CONTENT_ERROR_NONE, ret);

	ret = _media_filter_attribute_add(g_alias_attr_handle, MEDIA_PROVIDER_PINYIN, DB_TABLE_ALIAS_MEDIA"."DB_FIELD_MEDIA_PROVIDER_PINYIN);
	media_content_retv_if(ret != MEDIA_CONTENT_ERROR_NONE, ret);

	ret = _media_filter_attribute_add(g_alias_attr_handle, MEDIA_CONTENT_NAME_PINYIN, DB_TABLE_ALIAS_MEDIA"."DB_FIELD_MEDIA_CONTENT_NAME_PINYIN);
	media_content_retv_if(ret != MEDIA_CONTENT_ERROR_NONE, ret);

	ret = _media_filter_attribute_add(g_alias_attr_handle, MEDIA_CATEGORY_PINYIN, DB_TABLE_ALIAS_MEDIA"."DB_FIELD_MEDIA_CATEGORY_PINYIN);
	media_content_retv_if(ret != MEDIA_CONTENT_ERROR_NONE, ret);

	ret = _media_filter_attribute_add(g_alias_attr_handle, MEDIA_LOCATION_TAG_PINYIN, DB_TABLE_ALIAS_MEDIA"."DB_FIELD_MEDIA_LOCATION_TAG_PINYIN);
	media_content_retv_if(ret != MEDIA_CONTENT_ERROR_NONE, ret);

	ret = _media_filter_attribute_add(g_alias_attr_handle, MEDIA_AGE_RATING_PINYIN, DB_TABLE_ALIAS_MEDIA"."DB_FIELD_MEDIA_AGE_RATING_PINYIN);
	media_content_retv_if(ret != MEDIA_CONTENT_ERROR_NONE, ret);

	ret = _media_filter_attribute_add(g_alias_attr_handle, MEDIA_KEYWORD_PINYIN, DB_TABLE_ALIAS_MEDIA"."DB_FIELD_MEDIA_KEYWORD_PINYIN);
	media_content_retv_if(ret != MEDIA_CONTENT_ERROR_NONE, ret);

	/* Folder */
	ret = _media_filter_attribute_add(g_alias_attr_handle, FOLDER_ID, DB_TABLE_ALIAS_FOLDER"."DB_FIELD_FOLDER_ID);
	media_content_retv_if(ret != MEDIA_CONTENT_ERROR_NONE, ret);

	ret = _media_filter_attribute_add(g_alias_attr_handle, FOLDER_PATH, DB_TABLE_ALIAS_FOLDER"."DB_FIELD_FOLDER_PATH);
	media_content_retv_if(ret != MEDIA_CONTENT_ERROR_NONE, ret);

	ret = _media_filter_attribute_add(g_alias_attr_handle, FOLDER_NAME, DB_TABLE_ALIAS_FOLDER"."DB_FIELD_FOLDER_NAME);
	media_content_retv_if(ret != MEDIA_CONTENT_ERROR_NONE, ret);

	ret = _media_filter_attribute_add(g_alias_attr_handle, FOLDER_MODIFIED_TIME, DB_TABLE_ALIAS_FOLDER"."DB_FIELD_FOLDER_MODIFIED_TIME);
	media_content_retv_if(ret != MEDIA_CONTENT_ERROR_NONE, ret);

	ret = _media_filter_attribute_add(g_alias_attr_handle, FOLDER_STORAGE_TYPE, DB_TABLE_ALIAS_FOLDER"."DB_FIELD_FOLDER_STORAGE_TYPE);
	media_content_retv_if(ret != MEDIA_CONTENT_ERROR_NONE, ret);

	ret = _media_filter_attribute_add(g_alias_attr_handle, FOLDER_NAME_PINYIN, DB_TABLE_ALIAS_FOLDER"."DB_FIELD_FOLDER_NAME_PINYIN);
	media_content_retv_if(ret != MEDIA_CONTENT_ERROR_NONE, ret);

	ret = _media_filter_attribute_add(g_alias_attr_handle, FOLDER_ORDER, DB_TABLE_ALIAS_FOLDER"."DB_FIELD_FOLDER_ORDER);
	media_content_retv_if(ret != MEDIA_CONTENT_ERROR_NONE, ret);

	ret = _media_filter_attribute_add(g_alias_attr_handle, FOLDER_PARENT_FOLDER_ID, DB_TABLE_ALIAS_FOLDER"."DB_FIELD_FOLDER_PARENT_FOLDER_ID);
	media_content_retv_if(ret != MEDIA_CONTENT_ERROR_NONE, ret);
	/* Playlist*/
	ret = _media_filter_attribute_add(g_alias_attr_handle, PLAYLIST_NAME, DB_TABLE_ALIAS_PLAYLIST"."DB_FIELD_PLAYLIST_NAME);
	media_content_retv_if(ret != MEDIA_CONTENT_ERROR_NONE, ret);

	ret = _media_filter_attribute_add(g_alias_attr_handle, PLAYLIST_MEMBER_ORDER, DB_TABLE_ALIAS_PLAYLIST_MAP"."DB_FIELD_PLAYLIST_MEMBER_ORDER);
	media_content_retv_if(ret != MEDIA_CONTENT_ERROR_NONE, ret);

	/* Tag*/
	ret = _media_filter_attribute_add(g_alias_attr_handle, TAG_NAME, DB_TABLE_ALIAS_TAG"."DB_FIELD_TAG_NAME);
	media_content_retv_if(ret != MEDIA_CONTENT_ERROR_NONE, ret);

	/* Bookmark*/
	ret = _media_filter_attribute_add(g_alias_attr_handle, BOOKMARK_MARKED_TIME, DB_TABLE_ALIAS_BOOKMARK"."DB_FIELD_BOOKMARK_MARKED_TIME);
	media_content_retv_if(ret != MEDIA_CONTENT_ERROR_NONE, ret);

	return ret;
}

static int __media_content_create_attribute_handle(void)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;

	ret = __media_content_create_attr_handle();
	media_content_retv_if(ret != MEDIA_CONTENT_ERROR_NONE, ret);

	ret = __media_content_create_alias_attr_handle();
	if(ret != MEDIA_CONTENT_ERROR_NONE)
		_media_filter_attribute_destory(g_attr_handle);

	return ret;
}

static int __media_content_destroy_attribute_handle(void)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;

	ret = _media_filter_attribute_destory(g_attr_handle);
	ret = _media_filter_attribute_destory(g_alias_attr_handle);

	g_attr_handle = NULL;
	g_alias_attr_handle = NULL;

	return ret;
}

attribute_h _content_get_attirbute_handle(void)
{
	return g_attr_handle;
}

attribute_h _content_get_alias_attirbute_handle(void)
{
	return g_alias_attr_handle;
}

MediaSvcHandle* _content_get_db_handle(void)
{
	return db_handle;
}

int _content_query_prepare(sqlite3_stmt **stmt, char *select_query, char *condition_query, char *option_query)
{
	int len = 0;
	int err = MEDIA_CONTENT_ERROR_NONE;
	char query[MAX_QUERY_SIZE] = {0, };
	memset(query, '\0', sizeof(query));

	media_content_retvm_if(db_handle == NULL, MEDIA_CONTENT_ERROR_DB_FAILED, "database is not connected");
	media_content_retvm_if(!STRING_VALID(select_query), MEDIA_CONTENT_ERROR_INVALID_PARAMETER, "invalid select_query");

	if(!STRING_VALID(condition_query)) {
		condition_query = (char *)" ";
	}

	if(!STRING_VALID(option_query)) {
		option_query = (char *)" ";

	}

	//query = sqlite3_mprintf("%s %s %s", select_query, condition_query, option_query);
	len = snprintf(query, sizeof(query), "%s %s %s", select_query, condition_query, option_query);
	if (len > 0 && len < MAX_QUERY_SIZE) {
		query[len] = '\0';
	} else if (len >= MAX_QUERY_SIZE) {
		query[MAX_QUERY_SIZE -1] = '\0';
	} else {
		media_content_error("snprintf failed");
		return MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
	}

	media_content_sec_debug("Query : [%s]", query);

	err = sqlite3_prepare_v2((sqlite3*)db_handle, query, strlen(query), stmt, NULL);
	if(err != SQLITE_OK)
	{
		media_content_error("DB_FAILED(0x%08x) fail to sqlite3_prepare(), %s", MEDIA_CONTENT_ERROR_DB_FAILED, sqlite3_errmsg((sqlite3*)db_handle));

		if (err == SQLITE_BUSY) {
			media_content_error(" BUSY ERROR");
			return MEDIA_CONTENT_ERROR_DB_BUSY;
		} else if (err == SQLITE_PERM) {
			media_content_error("PERMISSION EROR");
			return MEDIA_CONTENT_ERROR_PERMISSION_DENIED;
		} else {
			media_content_error("OTHER ERROR");
			return MEDIA_CONTENT_ERROR_DB_FAILED;
		}
	}

	return MEDIA_CONTENT_ERROR_NONE;
}

int _content_error_capi(int type, int content_error)
{
	if(content_error != MEDIA_CONTENT_ERROR_NONE)
	{
		media_content_error("[type : %d] content_error : %d ", type, content_error);
	}

	/*Error None*/
	if(content_error == MS_MEDIA_ERR_NONE)
		return MEDIA_CONTENT_ERROR_NONE;

	/* Internal operation error*/
	else if((content_error == MS_MEDIA_ERR_INVALID_PARAMETER) ||
		(content_error == MS_MEDIA_ERR_INVALID_PATH) ||
		(content_error == MS_MEDIA_ERR_THUMB_DUPLICATED_REQUEST))
		return MEDIA_CONTENT_ERROR_INVALID_PARAMETER;

	else if(content_error == MS_MEDIA_ERR_OUT_OF_MEMORY)
		return MEDIA_CONTENT_ERROR_OUT_OF_MEMORY;

	/* DB operation error*/
	else if(content_error == MS_MEDIA_ERR_DB_BUSY_FAIL)
		return MEDIA_CONTENT_ERROR_DB_BUSY;

	else if((content_error <= MS_MEDIA_ERR_DB_CONNECT_FAIL) && (content_error >= MS_MEDIA_ERR_DB_INTERNAL))
		return MEDIA_CONTENT_ERROR_DB_FAILED;

	/* IPC operation error*/
	else if((content_error <= MS_MEDIA_ERR_SOCKET_CONN) && (content_error >= MS_MEDIA_ERR_SOCKET_INTERNAL))
		return MEDIA_CONTENT_ERROR_NETWORK;

	/* MEDIA SERVER error*/
	else if(content_error == MS_MEDIA_ERR_PERMISSION_DENIED)
		return MEDIA_CONTENT_ERROR_PERMISSION_DENIED;

	/* Thumbnail error*/
	else if(content_error == MS_MEDIA_ERR_THUMB_TOO_BIG)
			return MEDIA_CONTENT_ERROR_UNSUPPORTED_CONTENT;

	/*ETC*/
	return MEDIA_CONTENT_ERROR_INVALID_OPERATION;
}

int _content_query_sql(char *query_str)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;

	//DB will be updated by Media Server.
	ret = media_svc_request_update_db(query_str);

	return _content_error_capi(MEDIA_CONTENT_TYPE, ret);
}

int media_content_connect(void)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;

	if (g_mutex_trylock(&db_mutex)) {
		media_content_info("ref count : %d", ref_count);

		if(ref_count == 0)
		{
			if(db_handle == NULL)
			{
				ret = __media_content_create_attribute_handle();
				if(ret == MEDIA_CONTENT_ERROR_NONE) {
					ret = media_svc_connect(&db_handle);
					ret = _content_error_capi(MEDIA_CONTENT_TYPE, ret);
					if(ret == MEDIA_CONTENT_ERROR_NONE) {
						ref_count++;
					} else {
						__media_content_destroy_attribute_handle();
					}
				} else {
					media_content_error("Internal DB Connection Error");
				}
			} else {
				media_content_error("Wrong DB Connection status");
				ret = MEDIA_CONTENT_ERROR_DB_FAILED;
			}
		} else {
			if(db_handle != NULL) {
				ref_count++;
			} else {
				media_content_error("Wrong DB Handle status");
				ret = MEDIA_CONTENT_ERROR_DB_FAILED;
			}
		}

		media_content_info("ref count changed to: %d", ref_count);
		g_mutex_unlock(&db_mutex);
	} else {
		media_content_error("mutex is NULL");
		ret = MEDIA_CONTENT_ERROR_DB_FAILED;
	}

	return ret;
}

int media_content_disconnect(void)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;

	if (g_mutex_trylock(&db_mutex)) {
		media_content_debug("ref count : %d", ref_count);
		if(ref_count > 0)
		{
			if(db_handle != NULL) {
				ref_count--;
			} else {
				media_content_error("Wrong DB Handle status");
				ret = MEDIA_CONTENT_ERROR_DB_FAILED;
			}
		}
		else
		{
			media_content_error("DB_FAILED(0x%08x) database is not connected", MEDIA_CONTENT_ERROR_DB_FAILED);
			g_mutex_unlock(&db_mutex);
			return MEDIA_CONTENT_ERROR_DB_FAILED;
		}

		if(ref_count == 0)
		{
			if(db_handle != NULL)
			{
				ret = media_svc_disconnect(db_handle);
				ret = _content_error_capi(MEDIA_CONTENT_TYPE, ret);
				if(ret == MEDIA_CONTENT_ERROR_NONE) {
					ret = __media_content_destroy_attribute_handle();
					db_handle = NULL;
				} else {
					media_content_error("database disconnect fail");
					ref_count++;
				}
			} else {
				media_content_error("Wrong DB Handle status");
				ret = MEDIA_CONTENT_ERROR_DB_FAILED;
			}

			g_mutex_unlock(&db_mutex);

			media_content_info("ref count changed to: %d", ref_count);

			return ret;
		}

		g_mutex_unlock(&db_mutex);
	} else {
		media_content_error("mutex is NULL");
		ret = MEDIA_CONTENT_ERROR_INVALID_OPERATION;
	}

	media_content_info("ref count changed to: %d", ref_count);

	return ret;
}

int media_content_scan_file(const char *path)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	bool ignore_file = FALSE;
	bool ignore_dir = FALSE;
	char *folder_path = NULL;
	int check_file = MEDIA_CONTENT_ERROR_NONE;
	char storage_id[MEDIA_CONTENT_UUID_SIZE+1] = {0, };

	media_content_retvm_if(!STRING_VALID(path), MEDIA_CONTENT_ERROR_INVALID_PARAMETER, "invalid path");

	media_content_sec_debug("Path : %s", path);

	ret = _media_util_check_ignore_file(path, &ignore_file);
	media_content_retvm_if(ignore_file == TRUE, MEDIA_CONTENT_ERROR_INVALID_PARAMETER, "Invalid path");

	memset(storage_id, 0x00, sizeof(storage_id));
	ret = media_svc_get_storage_id(_content_get_db_handle(), path, storage_id);
	if(ret != MS_MEDIA_ERR_NONE)
	{
		media_content_error("media_svc_get_storage_id failed : %d", ret);
		return _content_error_capi(MEDIA_CONTENT_TYPE, ret);
	}

	check_file = _media_util_check_file_exist(path);
	if (check_file == MEDIA_CONTENT_ERROR_NONE) {
		/* This means this path has to be inserted or refreshed */
		folder_path = g_path_get_dirname(path);
		ret = _media_util_check_ignore_dir(folder_path, &ignore_dir);
		SAFE_FREE(folder_path);

		media_content_retvm_if(ignore_dir == TRUE, MEDIA_CONTENT_ERROR_INVALID_PARAMETER, "Invalid folder path");

		media_svc_storage_type_e storage_type;

		ret = media_svc_get_storage_type(path, &storage_type);
		if(ret != MS_MEDIA_ERR_NONE) {
			media_content_sec_error("media_svc_get_storage_type failed : %d (%s)", ret, path);
			return _content_error_capi(MEDIA_CONTENT_TYPE, ret);
		}
		ret = media_svc_check_item_exist_by_path(_content_get_db_handle(), storage_id, path);
		if (ret == MS_MEDIA_ERR_NONE) {
			/* Refresh */
			ret = media_svc_refresh_item(_content_get_db_handle(), storage_id, storage_type, path);
			if(ret != MS_MEDIA_ERR_NONE) {
				media_content_error("media_svc_refresh_item failed : %d", ret);
				return _content_error_capi(MEDIA_CONTENT_TYPE, ret);
			}

		} else if (ret == MS_MEDIA_ERR_DB_NO_RECORD) {
			/* Insert */
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
		} else {
			media_content_error("media_svc_check_item_exist_by_path failed : %d", ret);
			return _content_error_capi(MEDIA_CONTENT_TYPE, ret);
		}
	} else if (check_file == MEDIA_CONTENT_ERROR_PERMISSION_DENIED) {
		media_content_error("You have no permission for this file %d", ret);
		return MEDIA_CONTENT_ERROR_PERMISSION_DENIED;
	} else {
		/* This means this path has to be deleted */
		media_content_debug("This path doesn't exists in file system... So now start to delete it from DB");
		ret = media_svc_delete_item_by_path(_content_get_db_handle(), storage_id, path);
		if(ret != MS_MEDIA_ERR_NONE) {
			media_content_error("media_svc_delete_item_by_path failed : %d", ret);
			return _content_error_capi(MEDIA_CONTENT_TYPE, ret);
		}
	}

	return _content_error_capi(MEDIA_CONTENT_TYPE, ret);
}

void _media_content_scan_cb(media_request_result_s* result, void *user_data)
{
	int err = -1;
	media_content_scan_cb_data *cb_data = user_data;

	err = result->result;

	if (cb_data && cb_data->callback) {
		media_content_debug("User callback is being called now");
		cb_data->callback(err, cb_data->user_data);
	}

	SAFE_FREE(cb_data);

	return;
}

static int __media_content_check_dir(const char *path)
{
	DIR *dp = NULL;

	dp = opendir(path);
	if (dp == NULL) {
		media_content_sec_error("path [%s]", path);
		media_content_stderror("open dir fail");

		if (errno == EACCES || errno == EPERM) {
			return MEDIA_CONTENT_ERROR_PERMISSION_DENIED;
		} else {
			return MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
		}
	}

	closedir(dp);

	return MEDIA_CONTENT_ERROR_NONE;
}

int media_content_scan_folder(const char *path, bool is_recursive, media_scan_completed_cb callback, void *user_data)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	bool ignore_dir = FALSE;
	char storage_id[MEDIA_CONTENT_UUID_SIZE+1] = {0, };

	media_content_retvm_if(!STRING_VALID(path), MEDIA_CONTENT_ERROR_INVALID_PARAMETER, "Invalid path");

	memset(storage_id, 0x00, sizeof(storage_id));

	ret = __media_content_check_dir(path);
	media_content_retvm_if(ret == MEDIA_CONTENT_ERROR_PERMISSION_DENIED, ret, "Permission Denied");

	if (ret == MEDIA_CONTENT_ERROR_NONE) {
		/* If directory exist check that's ignore directory or not*/
		ret = _media_util_check_ignore_dir(path, &ignore_dir);
		media_content_retvm_if(ignore_dir == TRUE, MEDIA_CONTENT_ERROR_INVALID_PARAMETER, "Invalid folder path");
	} else {
		/* This means this folder has to be deleted */
		media_content_debug("This path doesn't exists in file system... So will be deleted it from DB");
	}

	media_content_scan_cb_data *cb_data = NULL;
	cb_data = (media_content_scan_cb_data *)malloc(sizeof(media_content_scan_cb_data));
	media_content_retvm_if(cb_data == NULL, MEDIA_CONTENT_ERROR_OUT_OF_MEMORY, "OUT_OF_MEMORY");

	cb_data->callback = callback;
	cb_data->user_data = user_data;

	ret = media_svc_get_storage_id(_content_get_db_handle(), path, storage_id);
	/*FIX ME. need to check ret value?*/

	ret = media_directory_scanning_async(path, storage_id, is_recursive, _media_content_scan_cb, cb_data);
	if(ret != MS_MEDIA_ERR_NONE) {
		media_content_error("media_directory_scanning_async failed : %d", ret);
	}

	return _content_error_capi(MEDIA_REGISTER_TYPE, ret);
}

int media_content_cancel_scan_folder(const char *path)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;

	ret = media_directory_scanning_cancel(path);
	if(ret != MS_MEDIA_ERR_NONE) {
		media_content_error("media_directory_scanning_async failed : %d", ret);
	}

	return _content_error_capi(MEDIA_REGISTER_TYPE, ret);
}

void _media_content_db_update_noti_cb(
							int pid,
							media_item_type_e item,
							media_item_update_type_e update_type,
							char* path,
							char* uuid,
							media_type_e content_type,
							char *mime_type,
							void *user_data)
{
	int error_value = MEDIA_CONTENT_ERROR_NONE;

	media_noti_cb_s *_noti_info = (media_noti_cb_s *)user_data;

	if(_noti_info != NULL)
	{
		if (_noti_info->update_noti_cb)
			_noti_info->update_noti_cb(error_value, pid, item, update_type, content_type, uuid, path, mime_type, _noti_info->user_data);
	}

	return;
}

int media_content_set_db_updated_cb(media_content_db_update_cb callback, void *user_data)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;

	media_content_retvm_if(callback == NULL, MEDIA_CONTENT_ERROR_INVALID_PARAMETER, "Invalid callback");
	media_content_retvm_if(g_noti_info != NULL, MEDIA_CONTENT_ERROR_INVALID_OPERATION, "Noti callback is already set");

	g_noti_info = (media_noti_cb_s*)calloc(1, sizeof(media_noti_cb_s));
	media_content_retvm_if(g_noti_info == NULL, MEDIA_CONTENT_ERROR_OUT_OF_MEMORY, "OUT_OF_MEMORY");

	g_noti_info->update_noti_cb = callback;
	g_noti_info->user_data = user_data;

	ret = media_db_update_subscribe(_media_content_db_update_noti_cb, (void *)g_noti_info);

	return _content_error_capi(MEDIA_REGISTER_TYPE, ret);
}

int media_content_unset_db_updated_cb(void)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;

	SAFE_FREE(g_noti_info);
	ret = media_db_update_unsubscribe();

	return _content_error_capi(MEDIA_REGISTER_TYPE, ret);
}

int media_content_set_db_updated_cb_v2(media_content_noti_h *noti_handle, media_content_db_update_cb callback, void *user_data)
{
        int ret = MEDIA_CONTENT_ERROR_NONE;
        media_noti_cb_s *noti_info = NULL;

        if (noti_handle == NULL) {
                media_content_error("INVALID_PARAMETER(0x%08x)", MEDIA_CONTENT_ERROR_INVALID_PARAMETER);
                return MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
        }

        if (callback == NULL) {
                media_content_error("INVALID_PARAMETER(0x%08x)", MEDIA_CONTENT_ERROR_INVALID_PARAMETER);
                return MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
        }

        noti_info = (media_noti_cb_s*)calloc(1, sizeof(media_noti_cb_s));
        if (noti_info == NULL) {
                media_content_error("Failed to create noti info");
                return MEDIA_CONTENT_ERROR_OUT_OF_MEMORY;
        }

        noti_info->update_noti_cb = callback;
        noti_info->user_data = user_data;

        ret = media_db_update_subscribe_internal((MediaNotiHandle*)noti_handle, _media_content_db_update_noti_cb, (void *)noti_info);

        return _content_error_capi(MEDIA_REGISTER_TYPE, ret);
}

void __media_content_clear_user_data(void *user_data)
{
        media_noti_cb_s *noti_info = user_data;

        SAFE_FREE(noti_info);

        return;
}

int media_content_unset_db_updated_cb_v2(media_content_noti_h noti_handle)
{
        int ret = MEDIA_CONTENT_ERROR_NONE;

        ret = media_db_update_unsubscribe_internal((MediaNotiHandle)noti_handle, __media_content_clear_user_data);

        return _content_error_capi(MEDIA_REGISTER_TYPE, ret);
}

