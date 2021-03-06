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


#include <media-thumb-error.h>
#include <media_content.h>
#include <media_info_private.h>
#include <media-util-err.h>


static attribute_h g_attr_handle = NULL;
static attribute_h g_alias_attr_handle = NULL;
static MediaSvcHandle *db_handle = NULL;
static int ref_count = 0;


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

	ret = _media_filter_attribute_add(g_attr_handle, MEDIA_THUMBNAIL_PATH, DB_FIELD_MEDIA_THUMBNAIL_PATH);
	media_content_retv_if(ret != MEDIA_CONTENT_ERROR_NONE, ret);

	ret = _media_filter_attribute_add(g_attr_handle, MEDIA_TITLE, DB_FIELD_MEDIA_TITLE);
	media_content_retv_if(ret != MEDIA_CONTENT_ERROR_NONE, ret);

	ret = _media_filter_attribute_add(g_attr_handle, MEDIA_ALBUM, DB_FIELD_MEDIA_ALBUM);
	media_content_retv_if(ret != MEDIA_CONTENT_ERROR_NONE, ret);

	ret = _media_filter_attribute_add(g_attr_handle, MEDIA_ARTIST, DB_FIELD_MEDIA_ARTIST);
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

	ret = _media_filter_attribute_add(g_attr_handle, MEDIA_IS_DRM, DB_FIELD_MEDIA_IS_DRM);
	media_content_retv_if(ret != MEDIA_CONTENT_ERROR_NONE, ret);

	ret = _media_filter_attribute_add(g_attr_handle, MEDIA_STORAGE_TYPE, DB_FIELD_MEDIA_STORAGE_TYPE);
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

	ret = _media_filter_attribute_add(g_alias_attr_handle, MEDIA_THUMBNAIL_PATH, DB_TABLE_ALIAS_MEDIA"."DB_FIELD_MEDIA_THUMBNAIL_PATH);
	media_content_retv_if(ret != MEDIA_CONTENT_ERROR_NONE, ret);

	ret = _media_filter_attribute_add(g_alias_attr_handle, MEDIA_TITLE, DB_TABLE_ALIAS_MEDIA"."DB_FIELD_MEDIA_TITLE);
	media_content_retv_if(ret != MEDIA_CONTENT_ERROR_NONE, ret);

	ret = _media_filter_attribute_add(g_alias_attr_handle, MEDIA_ALBUM, DB_TABLE_ALIAS_MEDIA"."DB_FIELD_MEDIA_ALBUM);
	media_content_retv_if(ret != MEDIA_CONTENT_ERROR_NONE, ret);

	ret = _media_filter_attribute_add(g_alias_attr_handle, MEDIA_ARTIST, DB_TABLE_ALIAS_MEDIA"."DB_FIELD_MEDIA_ARTIST);
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

	ret = _media_filter_attribute_add(g_alias_attr_handle, MEDIA_KEYWORD, DB_TABLE_ALIAS_MEDIA"."MEDIA_KEYWORD);
	media_content_retv_if(ret != MEDIA_CONTENT_ERROR_NONE, ret);

	ret = _media_filter_attribute_add(g_alias_attr_handle, MEDIA_IS_DRM, DB_TABLE_ALIAS_MEDIA"."DB_FIELD_MEDIA_IS_DRM);
	media_content_retv_if(ret != MEDIA_CONTENT_ERROR_NONE, ret);

	ret = _media_filter_attribute_add(g_alias_attr_handle, MEDIA_STORAGE_TYPE, DB_TABLE_ALIAS_MEDIA"."DB_FIELD_MEDIA_STORAGE_TYPE);
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

	/* Album */

	return ret;
}

static int __media_content_create_attribute_handle(void)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;

	ret = __media_content_create_attr_handle();
	media_content_retv_if(ret != MEDIA_CONTENT_ERROR_NONE, ret);

	ret = __media_content_create_alias_attr_handle();

	return ret;
}

static int __media_content_destroy_attribute_handle(void)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;

	ret = _media_filter_attribute_destory(g_attr_handle);
	media_content_retv_if(ret != MEDIA_CONTENT_ERROR_NONE, ret);

	ret = _media_filter_attribute_destory(g_alias_attr_handle);

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
	char query[MAX_QUERY_SIZE];
	memset(query, '\0', sizeof(query));

	if(db_handle == NULL)
	{
		media_content_error("DB_FAILED(0x%08x) database is not connected", MEDIA_CONTENT_ERROR_DB_FAILED);
		return MEDIA_CONTENT_ERROR_DB_FAILED;
	}

	if(STRING_VALID(select_query)) {
		if(!STRING_VALID(condition_query)) {
			condition_query = " ";
		}

		if(!STRING_VALID(option_query)) {
			option_query = " ";

		}

		//query = sqlite3_mprintf("%s %s %s", select_query, condition_query, option_query);
		len = snprintf(query, sizeof(query), "%s %s %s", select_query, condition_query, option_query);
		if (len > 0) {
			query[len] = '\0';
		} else {
			media_content_error("snprintf failed");
			return MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
		}

		media_content_debug("Query : [%s]", query);

		err = sqlite3_prepare_v2((sqlite3*)db_handle, query, strlen(query), stmt, NULL);
		if(err != SQLITE_OK)
		{
			media_content_error("DB_FAILED(0x%08x) fail to sqlite3_prepare(), %s", MEDIA_CONTENT_ERROR_DB_FAILED, sqlite3_errmsg((sqlite3*)db_handle));

			if (err == SQLITE_BUSY)
				return MEDIA_CONTENT_ERROR_DB_BUSY;
			else
				return MEDIA_CONTENT_ERROR_DB_FAILED;
		}
	}
	else
	{
		media_content_error("INVALID_PARAMETER(0x%08x)", MEDIA_CONTENT_ERROR_INVALID_PARAMETER);
		return MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
	}

	return MEDIA_CONTENT_ERROR_NONE;
}

int _content_error_capi(int type, int content_error)
{
	media_content_debug("[type : %d] content_error : %d ", type, content_error);

	if(type == MEDIA_CONTENT_TYPE)
	{
		if(content_error == MEDIA_INFO_ERROR_NONE)
			return MEDIA_CONTENT_ERROR_NONE;
		else if(content_error == MEDIA_INFO_ERROR_INVALID_PARAMETER)
			return MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
		else if(content_error == MEDIA_INFO_ERROR_DATABASE_INTERNAL)
			return MEDIA_CONTENT_ERROR_DB_FAILED;
		else if(content_error == MEDIA_INFO_ERROR_DATABASE_CONNECT)
			return MEDIA_CONTENT_ERROR_DB_FAILED;
		else if(content_error == MEDIA_INFO_ERROR_DATABASE_DISCONNECT)
			return MEDIA_CONTENT_ERROR_DB_FAILED;
		else if(content_error == MEDIA_INFO_ERROR_DATABASE_NO_RECORD)
			return MEDIA_CONTENT_ERROR_DB_FAILED;

	} else if(type == MEDIA_THUMBNAIL_TYPE) {
		if(content_error == MEDIA_THUMB_ERROR_NONE)
			return MEDIA_CONTENT_ERROR_NONE;
		else if(content_error == MEDIA_THUMB_ERROR_INVALID_PARAMETER)
			return MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
		else if(content_error == MEDIA_THUMB_ERROR_DB)
			return MEDIA_CONTENT_ERROR_DB_FAILED;
		else if(content_error == MEDIA_THUMB_ERROR_DB)
			return MEDIA_CONTENT_ERROR_DB_FAILED;
		else if(content_error == MEDIA_THUMB_ERROR_NETWORK)
			return MEDIA_CONTENT_ERROR_NETWORK;
		else if(content_error == MEDIA_THUMB_ERROR_TIMEOUT)
			return MEDIA_CONTENT_ERROR_NETWORK;
		else if(content_error == MEDIA_THUMB_ERROR_MM_UTIL)			/* Error in mm-util lib */
			return MEDIA_CONTENT_ERROR_INVALID_OPERATION;
		else if(content_error == MEDIA_THUMB_ERROR_HASHCODE)		/* Failed to generate hash code */
			return MEDIA_CONTENT_ERROR_INVALID_OPERATION;
		else if(content_error == MEDIA_THUMB_ERROR_TOO_BIG)			/* Original is too big to make thumb */
			return MEDIA_CONTENT_ERROR_UNSUPPORTED_CONTENT;
		else if(content_error == MEDIA_THUMB_ERROR_UNSUPPORTED)	/* Unsupported type */
			return MEDIA_CONTENT_ERROR_UNSUPPORTED_CONTENT;

	} else if(type == MEDIA_REGISTER_TYPE) {
		if(content_error == MS_MEDIA_ERR_NONE)
			return MEDIA_CONTENT_ERROR_NONE;
		else if(content_error == MS_MEDIA_ERR_INVALID_PARAMETER || content_error == MS_MEDIA_ERR_INVALID_PATH)
			return MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
		else if(content_error == MS_MEDIA_ERR_INSERT_FAIL || content_error == MS_MEDIA_ERR_DRM_INSERT_FAIL)
			return MEDIA_CONTENT_ERROR_DB_FAILED;
	}

	return MEDIA_CONTENT_ERROR_INVALID_OPERATION;
}

int _content_query_sql(char *query_str)
{
	int err = MEDIA_CONTENT_ERROR_NONE;
	char *err_msg;

	if(db_handle == NULL)
	{
		media_content_error("DB_FAILED(0x%08x) database is not connected", MEDIA_CONTENT_ERROR_DB_FAILED);
		return MEDIA_CONTENT_ERROR_DB_FAILED;
	}

	err = sqlite3_exec(db_handle, query_str, NULL, NULL, &err_msg);
	if(SQLITE_OK != err)
	{
		if(err_msg)
		{
			media_content_error("DB_FAILED : %s", err_msg);
			media_content_error("DB_FAILED SQL: %s", query_str);
			sqlite3_free(err_msg);
		}

		media_content_error("DB_FAILED(0x%08x) database operation is failed", MEDIA_CONTENT_ERROR_DB_FAILED);

		if (err == SQLITE_BUSY)
			return MEDIA_CONTENT_ERROR_DB_BUSY;
		else
			return MEDIA_CONTENT_ERROR_DB_FAILED;
	}
	else
	{
		media_content_debug("DB_SUCCESS: %s", query_str);
	}

	if(err_msg)
		sqlite3_free(err_msg);

	return MEDIA_CONTENT_ERROR_NONE;
}

int media_content_connect(void)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;

	if(ref_count == 0)
	{
		if(db_handle == NULL)
		{
			ret = media_svc_connect(&db_handle);
		}

		ret = _content_error_capi(MEDIA_CONTENT_TYPE, ret);

		if(ret == MEDIA_CONTENT_ERROR_NONE) {
			ret = __media_content_create_attribute_handle();
		}
	}

	ref_count++;

	return ret;
}

int media_content_disconnect(void)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;

	if(ref_count > 0)
	{
		ref_count--;
	}
	else
	{
		media_content_error("DB_FAILED(0x%08x) database is not connected", MEDIA_CONTENT_ERROR_DB_FAILED);
		return MEDIA_CONTENT_ERROR_DB_FAILED;
	}

	if(ref_count == 0)
	{
		if(db_handle != NULL)
		{
			ret = media_svc_disconnect(db_handle);
			ret = _content_error_capi(MEDIA_CONTENT_TYPE, ret);
			if(ret == MEDIA_CONTENT_ERROR_NONE)
			{
				ret = __media_content_destroy_attribute_handle();
				db_handle = NULL;
			}
		}
	}

	return ret;
}
