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


#ifndef __TIZEN_MEDIA_INFO_PRIVATE_H__
#define __TIZEN_MEDIA_INFO_PRIVATE_H__


#include <unistd.h>
#include <asm/unistd.h>
#include <string.h>
#include <sqlite3.h>
#include <stdlib.h>
#include <stdio.h>
#include <glib.h>
#include <media-svc.h>
#include <media_content_type.h>
#include <dlog.h>
#include <media-util.h>
#include <errno.h>


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#ifdef LOG_TAG
#undef LOG_TAG
#endif


/**
 * @addtogroup CAPI_CONTENT_MEDIA_INFO_MODULE
* @{
*
* @file media_info_private.h
* @brief This file contains the media info API and related structure and enumeration. \n
*        Description of the audio, video,image content involves: album, artist, album_artist, author, genre and description tags. \n
*        Parameters of the recording are also supported, as: format, bitrate, duration, size etc. \n
*        Defenitions of media DB fields and tables, operations with media data relating to DB and handling with media filter attributes.
*/


#define LOG_TAG "CAPI_CONTENT_MEDIA_CONTENT"

#define SAFE_STRLCPY(dst, src, n)	((g_strlcpy(dst, src, n) < n) ? TRUE : FALSE)
#define SAFE_STRLCAT(dst, src, n)	g_strlcat(dst, src, n);
#define SAFE_FREE(src)		{if(src) {free(src); src = NULL;}}
#define STRING_VALID(str)		((str != NULL && strlen(str) > 0) ? TRUE : FALSE)
#define SQLITE3_FINALIZE(x)	{if(x != NULL) {sqlite3_finalize(x);}}
#define SQLITE3_SAFE_FREE(x)	{if(x != NULL) {sqlite3_free(x);x = NULL;}}

#define MEDIA_CONTENT_THUMB_DEFAULT_PATH 	MEDIA_THUMB_ROOT_PATH"/.thumb/thumb_default.png"
#define MEDIA_CONTENT_INSERT_FILES_PATH		MEDIA_DATA_PATH"/"

#define MAX_QUERY_SIZE 4096
#define DEFAULT_QUERY_SIZE 1024
#define COLLATE_STR_SIZE 32
#define MEDIA_CONTENT_UUID_SIZE	36
#define BATCH_REQUEST_MAX 300

typedef enum {
	MEDIA_INFO_UUID = 0,
	MEDIA_INFO_PATH,
	MEDIA_INFO_FILE_NAME,
	MEDIA_INFO_TYPE,
	MEDIA_INFO_MIME_TYPE,
	MEDIA_INFO_SIZE,
	MEDIA_INFO_ADDED_TIME,
	MEDIA_INFO_MODIFIED_TIME,
	MEDIA_INFO_THUMBNAIL_PATH,
	MEDIA_INFO_DESCRIPTION,
	MEDIA_INFO_RATING,			//10
	MEDIA_INFO_FAVOURITE,
	MEDIA_INFO_AUTHOR,
	MEDIA_INFO_PROVIDER,
	MEDIA_INFO_CONTENT_NAME,
	MEDIA_INFO_CATEGORY,
	MEDIA_INFO_LOCATION_TAG,
	MEDIA_INFO_AGE_RATING,
	MEDIA_INFO_KEYWORD,
	MEDIA_INFO_IS_DRM,
	MEDIA_INFO_STORAGE_TYPE,	//20
	MEDIA_INFO_LONGITUDE,
	MEDIA_INFO_LATITUDE,
	MEDIA_INFO_ALTITUDE,
	MEDIA_INFO_EXPOSURE_TIME,
	MEDIA_INFO_FNUMBER,
	MEDIA_INFO_ISO,
	MEDIA_INFO_MODEL,
	MEDIA_INFO_WIDTH,
	MEDIA_INFO_HEIGHT,
	MEDIA_INFO_DATETAKEN,		//30
	MEDIA_INFO_ORIENTATION,
	MEDIA_INFO_TITLE,
	MEDIA_INFO_ALBUM,
	MEDIA_INFO_ARTIST,
	MEDIA_INFO_ALBUM_ARTIST,
	MEDIA_INFO_GENRE,
	MEDIA_INFO_COMPOSER,
	MEDIA_INFO_YEAR,
	MEDIA_INFO_RECORDED_DATE,
	MEDIA_INFO_COPYRIGHT,		//40
	MEDIA_INFO_TRACK_NUM,
	MEDIA_INFO_BITRATE,
	MEDIA_INFO_BITPERSAMPLE,
	MEDIA_INFO_DURATION,
	MEDIA_INFO_PLAYED_COUNT,
	MEDIA_INFO_LAST_PLAYED_TIME,
	MEDIA_INFO_LAST_PLAYED_POSITION,
	MEDIA_INFO_SAMPLERATE,
	MEDIA_INFO_CHANNEL,
	MEDIA_INFO_BURST_ID,		//50
	MEDIA_INFO_TIMELINE,
	MEDIA_INFO_WEATHER,
	MEDIA_INFO_SYNC_STATUS,
	MEDIA_INFO_STORAGE_UUID,
	MEDIA_INFO_ITEM_MAX,
} media_info_item_e;

typedef enum {
	MEDIA_CONTENT_TYPE = 0,
	MEDIA_THUMBNAIL_TYPE,
	MEDIA_REGISTER_TYPE
} media_info_error_type_e;

typedef enum {
	MEDIA_TAG_ADD,
	MEDIA_TAG_REMOVE,
	MEDIA_TAG_UPDATE_TAG_NAME,
} tag_function_e;

typedef enum {
	MEDIA_PLAYLIST_ADD,
	MEDIA_PLAYLIST_REMOVE,
	MEDIA_PLAYLIST_UPDATE_PLAYLIST_NAME,
	MEDIA_PLAYLIST_UPDATE_THUMBNAIL_PATH,
	MEDIA_PLAYLIST_UPDATE_PLAY_ORDER,
} playlist_function_e;

typedef enum {
	MEDIA_GROUP_NONE,
	MEDIA_GROUP_ALBUM,
	MEDIA_GROUP_FOLDER,
	MEDIA_GROUP_PLAYLIST,
	MEDIA_GROUP_TAG,
	MEDIA_GROUP_BOOKMARK,
	MEDIA_GROUP_TAG_BY_MEDIA_ID,
	MEDIA_GROUP_BOOKMARK_BY_MEDIA_ID,
	MEDIA_GROUP_STORAGE,
} group_list_e;

typedef enum {
	MEDIA_BATCH_INSERT_NORMAL,
	MEDIA_BATCH_INSERT_BURSTSHOT,
} media_batch_insert_e;

typedef struct _filter_s
{
	char *storage_id;
	char *condition;
	char *order_keyword;
	media_content_order_e order_type;
	media_content_collation_e condition_collate_type;
	media_content_collation_e order_collate_type;
	int offset;
	int count;
}filter_s;

typedef struct
{
	char *folder_id;				//image id, audio id, video id
	char *parent_folder_id;
	char *path;
	char *name;
	time_t modified_time;
	media_content_storage_e storage_type;
	char *storage_uuid;
	int folder_order;
}media_folder_s;

typedef struct
{
	int tag_id;
	char *name;
	GList *item_list;
}media_tag_s;

typedef struct
{
	int bookmark_id;
	char *media_id;
	time_t marked_time;
	char *thumbnail_path;
}media_bookmark_s;

typedef struct
{
	int album_id;
	char *name;
	char *artist;
	char *album_art_path;
}media_album_s;

typedef struct
{
	int playlist_id;	//playlist id
	char *name;		// playlist name
	char *thumbnail_path;		//playlist thumbnail path
	GList *item_list;
}media_playlist_s;

typedef struct
{
	char *media_id;
	int width;
	int height;
	char *date_taken;
	char *title;
	char *burst_id;
	char *weather;
	char *exposure_time;
	double fnumber;
	int iso;
	char *model;
	media_content_orientation_e orientation;
}image_meta_s;

typedef struct
{
	char *media_id;
	char *title;
	char *album;
	char *artist;
	char *album_artist;
	char *genre;
	char *composer;
	char *year;
	char *recorded_date;
	char *copyright;
	char *track_num;
	int bitrate;
	int duration;
	int width;
	int height;
	int played_count;
	time_t played_time;
	int played_position;
}video_meta_s;

typedef struct
{
	char *media_id;
	char *title;
	char *album;
	char *artist;
	char *album_artist;
	char *genre;
	char *composer;
	char *year;
	char *recorded_date;
	char *copyright;
	char *track_num;
	int bitrate;
	int bitpersample;
	int samplerate;
	int channel;
	int duration;
	int played_count;
	time_t played_time;
	int played_position;
}audio_meta_s;

typedef struct
{
	char *media_id;
	char *file_path;
	char *display_name;
	media_content_type_e media_type;
	char *mime_type;
	unsigned long long size;
	time_t added_time;
	time_t modified_time;
	time_t timeline;
	char *thumbnail_path;
	char *description;
	double longitude;
	double latitude;
	double altitude;
	char *weather;
	int rating;
	time_t favourite;
	char *title;
	char *author;
	char *provider;
	char *content_name;
	char *category;
	char *location_tag;
	char *age_rating;
	char *keyword;
	int is_drm;
	int storage_type;
	int played_count;
	time_t played_time;
	int played_position;
	char *storage_uuid;
	int sync_status;
	image_meta_s *image_meta;
	video_meta_s *video_meta;
	audio_meta_s *audio_meta;
}media_info_s;

typedef struct
{
	char *media_id;		// media_uuid
	int function;			// Add, remove, modify
	char *tag_name;		// tag_name
}media_tag_item_s;

typedef struct
{
	char *media_id;		// media_uuid
	int function;			// Add, remove, modify
	char *playlist_name;	// playlist_name
	char *thumbnail_path;		//playlist thumbnail path
	int playlist_member_id;	// playlist unique id of media. Same content which has same media_id can be added to Playlist
	int play_order;		//play_order
}media_playlist_item_s;

typedef struct
{
	char *storage_id;
	char *storage_name;
	char *storage_path;
	char *storage_account;
	int storage_type;
}media_storage_s;

typedef struct _attribute_map_s
{
	GHashTable *attr_map;
}attribute_s;

typedef struct
{
	media_info_s *handle;
	void *user_data;
	media_thumbnail_completed_cb thumbnail_completed_cb;
}media_thumbnail_cb_s;

typedef struct
{
	media_insert_completed_cb insert_completed_cb;
	char *insert_list_path;
	void *user_data;
} media_insert_cb_s;

typedef struct
{
	media_content_db_update_cb update_noti_cb;
	void *user_data;
} media_noti_cb_s;

typedef struct attribute_s *attribute_h;


typedef struct _media_content_cb_data {
	media_scan_completed_cb callback;
	void *user_data;
} media_content_scan_cb_data;


/* DB Table */
#define DB_TABLE_MEDIA			"media"
#define DB_TABLE_FOLDER		"folder"
#define DB_TABLE_ALBUM			"album"
#define DB_TABLE_TAG			"tag"
#define DB_TABLE_TAG_MAP		"tag_map"
#define DB_TABLE_PLAYLIST		"playlist"
#define DB_TABLE_PLAYLIST_MAP	"playlist_map"
#define DB_TABLE_BOOKMARK		"bookmark"
#define DB_TABLE_STORAGE		"storage"
#define DB_TABLE_MEDIA_VIEW	"media_view"

/* DB View */
#define DB_VIEW_PLAYLIST	"playlist_view"
#define DB_VIEW_TAG			"tag_view"

/* DB Table Alias */
#define DB_TABLE_ALIAS_MEDIA			"m"
#define DB_TABLE_ALIAS_FOLDER			"f"
#define DB_TABLE_ALIAS_PLAYLIST			"p"
#define DB_TABLE_ALIAS_PLAYLIST_MAP	"pm"
#define DB_TABLE_ALIAS_TAG				"t"
#define DB_TABLE_ALIAS_BOOKMARK		"b"
#define DB_TABLE_ALIAS_ALBUM			"a"

/* DB field for media */
#define DB_FIELD_MEDIA_ID					"media_uuid"
#define DB_FIELD_MEDIA_PATH				"path"
#define DB_FIELD_MEDIA_DISPLAY_NAME		"file_name"
#define DB_FIELD_MEDIA_TYPE					"media_type"
#define DB_FIELD_MEDIA_MIME_TYPE			"mime_type"
#define DB_FIELD_MEDIA_SIZE					"size"
#define DB_FIELD_MEDIA_ADDED_TIME			"added_time"
#define DB_FIELD_MEDIA_MODIFIED_TIME		"modified_time"
#define DB_FIELD_MEDIA_TIMELINE				"timeline"
#define DB_FIELD_MEDIA_THUMBNAIL_PATH	"thumbnail_path"
#define DB_FIELD_MEDIA_TITLE				"title"
#define DB_FIELD_MEDIA_ALBUM				"album"
#define DB_FIELD_MEDIA_ARTIST				"artist"
#define DB_FIELD_MEDIA_ALBUM_ARTIST			"album_artist"
#define DB_FIELD_MEDIA_GENRE				"genre"
#define DB_FIELD_MEDIA_COMPOSER			"composer"
#define DB_FIELD_MEDIA_YEAR				"year"
#define DB_FIELD_MEDIA_RECORDED_DATE		"recorded_date"
#define DB_FIELD_MEDIA_COPYRIGHT			"copyright"
#define DB_FIELD_MEDIA_TRACK_NUM			"track_num"
#define DB_FIELD_MEDIA_DESCRIPTION			"description"
#define DB_FIELD_MEDIA_BITRATE				"bitrate"
#define DB_FIELD_MEDIA_BITPERSAMPLE		"bitpersample"
#define DB_FIELD_MEDIA_SAMPLERATE			"samplerate"
#define DB_FIELD_MEDIA_CHANNEL			"channel"
#define DB_FIELD_MEDIA_DURATION			"duration"
#define DB_FIELD_MEDIA_LONGITUDE			"longitude"
#define DB_FIELD_MEDIA_LATITUDE			"latitude"
#define DB_FIELD_MEDIA_ALTITUDE			"altitude"
#define DB_FIELD_MEDIA_WIDTH				"width"
#define DB_FIELD_MEDIA_HEIGHT				"height"
#define DB_FIELD_MEDIA_DATETAKEN			"datetaken"
#define DB_FIELD_MEDIA_ORIENTATION		"orientation"
#define DB_FIELD_MEDIA_BURST_ID			"burst_id"
#define DB_FIELD_MEDIA_PLAYED_COUNT		"played_count"
#define DB_FIELD_MEDIA_LAST_PLAYED_TIME		"last_played_time"
#define DB_FIELD_MEDIA_LAST_PLAYED_POSITION	"last_played_position"
#define DB_FIELD_MEDIA_RATING				"rating"
#define DB_FIELD_MEDIA_FAVOURITE			"favourite"
#define DB_FIELD_MEDIA_AUTHOR				"author"
#define DB_FIELD_MEDIA_PROVIDER			"provider"
#define DB_FIELD_MEDIA_CONTENT_NAME		"content_name"
#define DB_FIELD_MEDIA_CATEGORY			"category"
#define DB_FIELD_MEDIA_LOCATION_TAG		"location_tag"
#define DB_FIELD_MEDIA_AGE_RATING			"age_rating"
#define DB_FIELD_MEDIA_KEYWORD			"keyword"
#define DB_FIELD_MEDIA_WEATHER			"weather"
#define DB_FIELD_MEDIA_IS_DRM				"is_drm"
#define DB_FIELD_MEDIA_STORAGE_TYPE		"storage_type"
#define DB_FIELD_MEDIA_EXPOSURE_TIME	"exposure_time"
#define DB_FIELD_MEDIA_FNUMBER			"fnumber"
#define DB_FIELD_MEDIA_ISO				"iso"
#define DB_FIELD_MEDIA_MODEL			"model"
#define DB_FIELD_MEDIA_FILE_NAME_PINYIN			"file_name_pinyin"
#define DB_FIELD_MEDIA_TITLE_PINYIN					"title_pinyin"
#define DB_FIELD_MEDIA_ALBUM_PINYIN				"album_pinyin"
#define DB_FIELD_MEDIA_ARTIST_PINYIN				"artist_pinyin"
#define DB_FIELD_MEDIA_ALBUM_ARTIST_PINYIN		"album_artist_pinyin"
#define DB_FIELD_MEDIA_GENRE_PINYIN				"genre_pinyin"
#define DB_FIELD_MEDIA_COMPOSER_PINYIN			"composer_pinyin"
#define DB_FIELD_MEDIA_COPYRIGHT_PINYIN			"copyright_pinyin"
#define DB_FIELD_MEDIA_DESCRIPTION_PINYIN			"description_pinyin"
#define DB_FIELD_MEDIA_AUTHOR_PINYIN				"author_pinyin"
#define DB_FIELD_MEDIA_PROVIDER_PINYIN			"provider_pinyin"
#define DB_FIELD_MEDIA_CONTENT_NAME_PINYIN		"content_name_pinyin"
#define DB_FIELD_MEDIA_CATEGORY_PINYIN			"category_pinyin"
#define DB_FIELD_MEDIA_LOCATION_TAG_PINYIN		"location_tag_pinyin"
#define DB_FIELD_MEDIA_AGE_RATING_PINYIN			"age_rating_pinyin"
#define DB_FIELD_MEDIA_KEYWORD_PINYIN				"keyword_pinyin"

/* DB field for folder */
#define DB_FIELD_FOLDER_ID				"folder_uuid"
#define DB_FIELD_FOLDER_PATH			"path"
#define DB_FIELD_FOLDER_NAME			"name"
#define DB_FIELD_FOLDER_MODIFIED_TIME	"modified_time"
#define DB_FIELD_FOLDER_STORAGE_TYPE	"storage_type"
#define DB_FIELD_FOLDER_NAME_PINYIN	"name_pinyin"
#define DB_FIELD_FOLDER_ORDER			"folder_order"
#define DB_FIELD_FOLDER_PARENT_FOLDER_ID	"parent_folder_uuid"

/* DB field for playlist */
#define DB_FIELD_PLAYLIST_ID					"playlist_id"
#define DB_FIELD_PLAYLIST_NAME				"name"
#define DB_FIELD_PLAYLIST_MEMBER_ORDER	"play_order"
#define DB_FIELD_PLAYLIST_MEDIA_COUNT			"media_count"

/* DB field for tag */
#define DB_FIELD_TAG_ID			"tag_id"
#define DB_FIELD_TAG_NAME		"name"
#define DB_FIELD_TAG_MEDIA_COUNT			"media_count"

/* DB field for bookmark */
#define DB_FIELD_BOOKMARK_ID	"bookmark_id"
#define DB_FIELD_BOOKMARK_MARKED_TIME	"marked_time"

/* DB field for album*/
#define DB_FIELD_ALBUM_ID			"album_id"
#define DB_FIELD_ALBUM_NAME		"name"
#define DB_FIELD_ALBUM_ARTIST		"artist"

/* DB field for Storage*/
#define DB_FIELD_STORAGE_ID			"storage_uuid"
#define DB_FIELD_STORAGE_PATH		"storage_path"

/* DB Query Keyword */
#define QUERY_KEYWORD_AND "AND"
#define QUERY_KEYWORD_OR "OR"
#define QUERY_KEYWORD_ORDER_BY "ORDER BY"
#define QUERY_KEYWORD_LIMIT "limit"
#define QUERY_KEYWORD_DESC "DESC"
#define QUERY_KEYWORD_SPACE " "
#define QUERY_KEYWORD_OPEN_BRACKET "("
#define QUERY_KEYWORD_BRACKET ")"

/* DB TABLE JOIN */
//#define FOLDER_MEDIA_JOIN					"("DB_TABLE_FOLDER" AS f LEFT OUTER JOIN '%s' AS m ON f.folder_uuid=m.folder_uuid AND m.validity=1) WHERE f.storage_uuid IN (SELECT storage_uuid FROM "DB_TABLE_STORAGE" WHERE validity=1) "
#define FOLDER_MEDIA_JOIN					"("DB_TABLE_FOLDER" AS f LEFT OUTER JOIN '%s' AS m ON f.folder_uuid=m.folder_uuid AND m.validity=1) WHERE f.validity=1 "
#define BOOKMARK_MEDIA_JOIN				"("DB_TABLE_BOOKMARK" AS b INNER JOIN '%s' AS m \
											ON (b.media_uuid = m.media_uuid)) WHERE m.validity=1"
#define ALBUM_MEDIA_JOIN					"("DB_TABLE_ALBUM" AS a INNER JOIN '%s' AS m \
											ON (a.album_id = m.album_id)) WHERE m.validity=1"

/* Get Group List */
#define SELECT_ALBUM_LIST			"SELECT DISTINCT a.album_id, a.name, a.artist, a.album_art FROM "ALBUM_MEDIA_JOIN
#define SELECT_MEDIA_GROUP_LIST	"SELECT DISTINCT %s FROM '%s' WHERE validity=1 "

#define SELECT_FOLDER_LIST 			"SELECT DISTINCT f.folder_uuid, f.path, f.name, f.storage_type, f.modified_time, f.storage_uuid, f.folder_order, f.parent_folder_uuid FROM "FOLDER_MEDIA_JOIN
#define SELECT_FOLDER_LIST_BY_STORAGE_ID	SELECT_FOLDER_LIST"AND f.storage_uuid='%s' "
#define SELECT_TAG_LIST				"SELECT DISTINCT tag_id, name FROM "DB_VIEW_TAG" WHERE 1 "
#define SELECT_PLAYLIST_LIST			"SELECT DISTINCT playlist_id, name, p_thumbnail_path FROM "DB_VIEW_PLAYLIST" WHERE 1 "

/* Get Group Count */
#define SELECT_ALBUM_COUNT		"SELECT COUNT(DISTINCT a.album_id) FROM "ALBUM_MEDIA_JOIN
#define SELECT_FOLDER_COUNT		"SELECT COUNT(DISTINCT f.folder_uuid) FROM "FOLDER_MEDIA_JOIN
#define SELECT_FOLDER_COUNT_BY_STORAGE_ID		SELECT_FOLDER_COUNT"AND f.storage_uuid='%s' "
#define SELECT_TAG_COUNT			"SELECT COUNT(DISTINCT tag_id) FROM "DB_VIEW_TAG" WHERE 1 "
#define SELECT_PLAYLIST_COUNT		"SELECT COUNT(DISTINCT playlist_id) FROM "DB_VIEW_PLAYLIST" WHERE 1 "
#define SELECT_BOOKMARK_COUNT	"SELECT COUNT(DISTINCT b.bookmark_id) FROM "BOOKMARK_MEDIA_JOIN
#define SELECT_MEDIA_GROUP_COUNT	"SELECT COUNT(*) FROM ("SELECT_MEDIA_GROUP_LIST
/*count(distinct x) count only non-null values, but select distinct X returns include null. so sync the result of count and list, don't use count(distinct x)*/

/* Get Media Count of Group */
#define SELECT_MEDIA_COUNT_FROM_MEDIA			"SELECT COUNT(*) FROM ("SELECT_MEDIA_ITEM		//to apply limit condition. "SELECT COUNT(*) FROM "DB_TABLE_MEDIA" WHERE validity=1"
#define SELECT_MEDIA_COUNT_FROM_MEDIA_SIMPLE	"SELECT COUNT(*) FROM '%s' WHERE validity=1 "
#define SELECT_MEDIA_COUNT_FROM_ALBUM			"SELECT COUNT(*) FROM '%s' WHERE validity=1 AND album_id='%d'"
#define SELECT_MEDIA_COUNT_FROM_GROUP			"SELECT COUNT(*) FROM ("SELECT_MEDIA_FROM_GROUP		//to apply limit condition.
#define SELECT_MEDIA_COUNT_FROM_GROUP_NULL		"SELECT COUNT(*) FROM ("SELECT_MEDIA_FROM_GROUP_NULL	//to apply limit condition.
#define SELECT_MEDIA_COUNT_FROM_FOLDER			"SELECT COUNT(*) FROM '%s' WHERE validity=1 AND folder_uuid='%q'"
#define SELECT_MEDIA_COUNT_FROM_STORAGE			"SELECT COUNT(*) FROM '%s' WHERE validity=1 AND storage_uuid='%q'"
#define SELECT_MEDIA_COUNT_FROM_TAG				"SELECT COUNT(*) FROM "DB_VIEW_TAG" WHERE (tag_id=%d AND media_count>0) "
#define SELECT_MEDIA_COUNT_FROM_PLAYLIST			"SELECT COUNT(*) FROM "DB_VIEW_PLAYLIST" WHERE (playlist_id=%d and media_count>0) "

/* Get Group Info by Group ID*/
#define SELECT_ALBUM_FROM_ALBUM		"SELECT * FROM "DB_TABLE_ALBUM" WHERE album_id=%d"
#define SELECT_FOLDER_FROM_FOLDER		"SELECT * FROM "DB_TABLE_FOLDER" WHERE folder_uuid='%s'"
#define SELECT_FOLDER_BY_PATH			"SELECT * FROM "DB_TABLE_FOLDER" WHERE path='%q'"
#define SELECT_PLAYLIST_FROM_PLAYLIST	"SELECT * FROM "DB_TABLE_PLAYLIST" WHERE playlist_id=%d"
#define SELECT_TAG_FROM_TAG			"SELECT * FROM "DB_TABLE_TAG" WHERE tag_id=%d"
#define SELECT_TAG_BY_NAME				"SELECT * FROM "DB_TABLE_TAG" WHERE name='%q'"

/* Tag info*/
#define INSERT_TAG_TO_TAG						"INSERT INTO "DB_TABLE_TAG" (name) VALUES (%Q)"
#define REMOVE_TAG_ITEM_FROM_TAG_MAP		"DELETE FROM "DB_TABLE_TAG_MAP" WHERE tag_id=%d AND media_uuid='%q'"
#define UPDATE_TAG_NAME_FROM_TAG			"UPDATE "DB_TABLE_TAG" SET name='%q' WHERE tag_id=%d"
#define SELECT_TAG_COUNT_BY_MEDIA_ID			"SELECT COUNT(*) FROM "DB_VIEW_TAG" WHERE media_uuid = '%s'"
#define SELECT_TAG_LIST_BY_MEDIA_ID				"SELECT tag_id, name FROM "DB_VIEW_TAG" WHERE media_uuid = '%s'"

/* Get Media list of Group */
#define MEDIA_INFO_ITEM "media_uuid, path, file_name, media_type, mime_type, size, added_time, modified_time, thumbnail_path, description, \
							rating, favourite, author, provider, content_name, category, location_tag, age_rating, keyword, is_drm, storage_type, longitude, latitude, altitude, exposure_time, fnumber, iso, model, width, height, datetaken, orientation, title, album, artist, album_artist, genre, composer, year, recorded_date, copyright, track_num, bitrate, bitpersample, duration, played_count, last_played_time, last_played_position, samplerate, channel, burst_id, timeline, weather, sync_status, storage_uuid"

/*Folder info*/
#define UPDATE_FOLDER_ORDER					"UPDATE "DB_TABLE_FOLDER" SET folder_order=%d WHERE folder_uuid=%Q"

/* Playlist Info */
#define INSERT_PLAYLIST_TO_PLAYLIST						"INSERT INTO "DB_TABLE_PLAYLIST" (name) VALUES (%Q)"
#define UPDATE_PLAYLIST_NAME_FROM_PLAYLIST			"UPDATE "DB_TABLE_PLAYLIST" SET name='%q' WHERE playlist_id=%d"
#define UPDATE_PLAYLIST_THUMBNAIL_FROM_PLAYLIST		"UPDATE "DB_TABLE_PLAYLIST" SET thumbnail_path='%q' WHERE playlist_id=%d"
#define SELECT_PLAYLIST_ID_FROM_PLAYLIST				"SELECT playlist_id FROM "DB_TABLE_PLAYLIST" WHERE name='%q'"
#define SELECT_PLAYLIST_ITEM_ID_FROM_PLAYLIST_VIEW	"SELECT pm_id, media_uuid FROM "DB_VIEW_PLAYLIST" WHERE (playlist_id=%d and media_count>0) "
#define SELECT_PLAYLIST_ITEM_ALL_FROM_PLAYLIST_VIEW	"SELECT "MEDIA_INFO_ITEM", pm_id FROM "DB_VIEW_PLAYLIST" WHERE (playlist_id=%d and media_count>0) "
#define SELECT_PLAY_ORDER_FROM_PLAYLIST_VIEW			"SELECT play_order FROM "DB_VIEW_PLAYLIST" WHERE playlist_id=%d and pm_id=%d"
#define SELECT_MAX_PLAY_ORDER_FROM_PLAYLIST_VIEW	"SELECT MAX(play_order) FROM "DB_VIEW_PLAYLIST" WHERE playlist_id=%d"
#define REMOVE_PLAYLIST_ITEM_FROM_PLAYLIST_MAP		"DELETE FROM "DB_TABLE_PLAYLIST_MAP" WHERE playlist_id=%d AND _id=%d"
#define UPDATE_PLAYLIST_ORDER_FROM_PLAYLIST_MAP		"UPDATE "DB_TABLE_PLAYLIST_MAP" SET play_order=%d WHERE playlist_id=%d AND _id=%d"

/* Bookmark */
#define INSERT_BOOKMARK_TO_BOOKMARK			"INSERT INTO "DB_TABLE_BOOKMARK" (media_uuid, marked_time, thumbnail_path) VALUES ('%q', '%d', %Q)"
#define SELECT_BOOKMARK_COUNT_BY_MEDIA_ID		"SELECT COUNT(*) FROM "BOOKMARK_MEDIA_JOIN" AND b.media_uuid='%s'"
#define SELECT_BOOKMARK_LIST_BY_MEDIA_ID			"SELECT b.bookmark_id, b.media_uuid, b.marked_time, b.thumbnail_path FROM "BOOKMARK_MEDIA_JOIN" AND b.media_uuid='%s'"

/* Update Meta*/
#define UPDATE_AV_META_FROM_MEDIA	"UPDATE '%s' SET played_count=%d, last_played_time=%d, last_played_position=%d WHERE media_uuid='%q'"
#define UPDATE_IMAGE_META_FROM_MEDIA	"UPDATE '%s' SET orientation=%d, weather=%Q WHERE media_uuid='%q'"

#define SELECT_MEDIA_ITEM 					"SELECT "MEDIA_INFO_ITEM" FROM '%s' WHERE validity=1"
#define SELECT_MEDIA_FROM_MEDIA			"SELECT "MEDIA_INFO_ITEM" FROM '%s' WHERE validity=1 AND media_uuid='%s'"
#define SELECT_MEDIA_BY_PATH				"SELECT "MEDIA_INFO_ITEM" FROM '%s' WHERE validity=1 AND path='%q'"
#define SELECT_MEDIA_FROM_ALBUM			"SELECT "MEDIA_INFO_ITEM" FROM '%s' WHERE validity=1 AND album_id=%d"
#define SELECT_MEDIA_FROM_GROUP			"SELECT "MEDIA_INFO_ITEM" FROM '%s' WHERE validity=1 AND %s='%q'"
#define SELECT_MEDIA_FROM_GROUP_NULL	"SELECT "MEDIA_INFO_ITEM" FROM '%s' WHERE validity=1 AND %s IS NULL"
#define SELECT_MEDIA_FROM_FOLDER			"SELECT "MEDIA_INFO_ITEM" FROM '%s' WHERE validity=1 AND folder_uuid='%s'"
#define SELECT_MEDIA_FROM_STORAGE		"SELECT "MEDIA_INFO_ITEM" FROM '%s' WHERE validity=1 AND storage_uuid='%s'"
#define SELECT_MEDIA_FROM_TAG				"SELECT "MEDIA_INFO_ITEM" FROM '%s' WHERE media_uuid IN (SELECT media_uuid FROM "DB_TABLE_TAG_MAP" WHERE tag_id=%d) AND validity=1"
#define SELECT_MEDIA_FROM_PLAYLIST		"SELECT "MEDIA_INFO_ITEM" FROM '%s' WHERE media_uuid IN (SELECT media_uuid FROM "DB_TABLE_PLAYLIST_MAP" WHERE playlist_id=%d) AND validity=1"
#define SELECT_MEDIA_PATH_BY_ID			"SELECT path FROM "DB_TABLE_MEDIA_VIEW" WHERE media_uuid='%q'"
#define SELECT_MEDIA_STORAGE_ID_BY_ID		"SELECT storage_uuid FROM "DB_TABLE_MEDIA_VIEW" WHERE media_uuid='%q'"

/* Delete */
#define DELETE_MEDIA_FROM_MEDIA_BATCH	"DELETE FROM  '%s' WHERE %s"
#define DELETE_PLAYLIST_FROM_PLAYLIST		"DELETE FROM "DB_TABLE_PLAYLIST" WHERE playlist_id=%d"
#define DELETE_TAG_FROM_TAG				"DELETE FROM "DB_TABLE_TAG" WHERE tag_id=%d"
#define DELETE_BOOKMARK_FROM_BOOKMARK	"DELETE FROM "DB_TABLE_BOOKMARK" WHERE bookmark_id=%d"

/* Storage*/
#define SELECT_STORAGE_COUNT 				"SELECT COUNT(*) FROM "DB_TABLE_STORAGE" WHERE validity=1"
#define SELECT_STORAGE_LIST 				"SELECT * FROM "DB_TABLE_STORAGE" WHERE validity=1"
#define SELECT_STORAGE_INFO_FROM_STORAGE	"SELECT * FROM "DB_TABLE_STORAGE" WHERE validity=1 AND storage_uuid='%s'"

int _content_query_prepare(sqlite3_stmt **stmt, char *select_query, char *condition_query, char *option_query);
int _content_error_capi(int type, int cotent_error);
int _content_query_sql(char *query_str);
MediaSvcHandle* _content_get_db_handle(void);
attribute_h _content_get_attirbute_handle(void);
attribute_h _content_get_alias_attirbute_handle(void);
int _media_info_get_media_info_from_db(const char *path, const char *storage_id, media_info_h media);
void _media_info_item_get_detail(sqlite3_stmt *stmt, media_info_h media);
int _media_db_get_group_count(filter_h filter, group_list_e group_type, int *group_count);
int _media_db_get_media_group_count(media_group_e group, filter_h filter, int *group_count);
int _media_db_get_media_group(media_group_e group, filter_h filter, media_group_cb callback, void *user_data);
int _media_db_get_album(filter_h filter, media_album_cb callback, void *user_data);
int _media_db_get_folder(filter_h filter, media_folder_cb callback, void *user_data);
int _media_db_get_playlist(filter_h filter, media_playlist_cb callback, void *user_data);
int _media_db_get_playlist_item(int playlist_id, filter_h filter, playlist_member_cb callback, void *user_data);
int _media_db_get_tag(const char *media_id, filter_h filter, media_tag_cb callback, void *user_data);
int _media_db_get_bookmark(const char *media_id, filter_h filter, media_bookmark_cb callback, void *user_data);
int _media_db_get_group_item_count_by_id(int group_id, filter_h filter, group_list_e group_type, int *item_count);
int _media_db_get_group_item_count(const char *group_name, filter_h filter, group_list_e group_type, int *item_count);
int _media_db_get_group_item_by_id(int group_id, filter_h filter, media_info_cb callback, void *user_data, group_list_e group_type);
int _media_db_get_group_item(const char *group_name, filter_h filter, media_info_cb callback, void *user_data, group_list_e group_type);
int _media_db_get_media_group_item_count(const char *group_name, filter_h filter, media_group_e group, int *item_count);
int _media_db_get_media_group_item(const char *group_name, filter_h filter, media_group_e group, media_info_cb callback, void *user_data);
int _media_db_get_storage(filter_h filter, media_storage_cb callback, void *user_data);
int _media_db_get_storage_id_by_media_id(const char *media_id, char *storage_id);

/**
 * @internal
 * @brief Creates a media filter attribute handle.
 * @details This function creates a media filter attribute handle. The handle can be
 * used to convert to attributes of database from attributes of user.
 * @remarks The @a handle must be released with media_filter_attribute_destory() by you.
 * @param[out] filter A handle to media filter attribute
 * @return 0 on success, otherwise a negative error value.
 * @retval #MEDIA_CONTENT_ERROR_NONE Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #MEDIA_CONTENT_ERROR_PERMISSION_DENIED Permission denied
 * @see media_filter_attribute_destory()
 *
 */
int _media_filter_attribute_create(attribute_h *attr);

/**
 * @internal
 * @brief Add the attributes to the handle.
 * @details This function add the attribute to handle.
 * @param[in] filter The handle to media filter attribute
 * @param[in] user_attr The user attribute
 * @param[in] platform_attr The platform attribute
 * @return 0 on success, otherwise a negative error value.
 * @retval #MEDIA_CONTENT_ERROR_NONE Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #MEDIA_CONTENT_ERROR_PERMISSION_DENIED Permission denied
 * @see media_filter_attribute_remove()
 *
 */
int _media_filter_attribute_add(attribute_h atrr, const char *user_attr, const char *platform_attr);

/**
 * @internal
 * @brief Destroys a media filter attribute handle.
 * @details The function frees all resources related to the media filter attribute handle. The filter attribute
 * handle no longer can be used to perform any operation. A new handle
 * has to be created before the next usage.
 *
 * @param[in] filter The handle to media filter attribute
 * @return 0 on success, otherwise a negative error value.
 * @retval #MEDIA_CONTENT_ERROR_NONE Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_PERMISSION_DENIED Permission denied
 * @see media_filter_create()
 *
 */
int _media_filter_attribute_destory(attribute_h attr);

/**
 * @internal
 * @brief Replace to platform attributes from user attributes.
 * @details This function replace to platform attributes from user attributes to generate the WHERE clause
 * @param[in] filter The handle to media filter attribute
 * @param[in] user_attr The user attribute
 * @param[in] platform_attr The platform attribute
 * @return 0 on success, otherwise a negative error value.
 * @retval #MEDIA_CONTENT_ERROR_NONE Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #MEDIA_CONTENT_ERROR_PERMISSION_DENIED Permission denied
 * @see media_filter_attribute_create()
 * @see media_filter_attribute_destory()
 *
 */
int _media_filter_attribute_generate(attribute_h attr, char *condition, media_content_collation_e collate_type, char **generated_condition);


/**
 * @internal
 * @brief Replace to platform attributes from user attributes.
 * @details This function replace to platform attributes from user attributes to generate the WHERE clause
 * @param[in] filter The handle to media filter attribute
 * @param[in] attr The attribute
 * @param[in] generated_option The handle to generated option
 * @return 0 on success, otherwise a negative error value.
 * @retval #MEDIA_CONTENT_ERROR_NONE Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_DB_FAILED Filed DB
 * @retval #MEDIA_CONTENT_ERROR_PERMISSION_DENIED Permission denied
 * @see media_filter_attribute_create()
 * @see media_filter_attribute_destory()
 *
 */

int _media_filter_attribute_option_generate(attribute_h attr, filter_h filter, char **generated_option);

#define FONT_COLOR_RESET    "\033[0m"
#define FONT_COLOR_RED      "\033[31m"
#define FONT_COLOR_GREEN    "\033[32m"
#define FONT_COLOR_YELLOW   "\033[33m"
#define FONT_COLOR_BLUE     "\033[34m"
#define FONT_COLOR_PURPLE   "\033[35m"
#define FONT_COLOR_CYAN     "\033[36m"
#define FONT_COLOR_GRAY     "\033[37m"

#define media_content_gettid() syscall(__NR_gettid)

#define media_content_retv_if(expr, val) do { \
			if(expr) { \
				LOGE(FONT_COLOR_RED"[%d]", media_content_gettid());    \
				return (val); \
			} \
		} while (0)

#define media_content_retvm_if(expr, val, fmt, arg...) do { \
			if(expr) { \
				LOGE(FONT_COLOR_RED"[%d]"fmt"",media_content_gettid(), ##arg);     \
				return (val); \
			} \
		} while (0)

#define media_content_debug(fmt, arg...) do { \
			LOGD(FONT_COLOR_RESET"[%d]"fmt"", media_content_gettid(), ##arg);     \
		} while (0)

#define media_content_info(fmt, arg...) do { \
			LOGI(FONT_COLOR_GREEN"[%d]"fmt"", media_content_gettid() ,##arg);     \
		} while (0)

#define media_content_error(fmt, arg...) do { \
			LOGE(FONT_COLOR_RED"[%d]"fmt"",media_content_gettid(), ##arg);     \
		} while (0)

#define media_content_debug_func() do { \
			LOGD(FONT_COLOR_RESET"[%d]", media_content_gettid());     \
		} while (0)

#define media_content_sec_debug(fmt, arg...) do { \
			SECURE_LOGD(FONT_COLOR_RESET"[%d]"fmt"", media_content_gettid(), ##arg);     \
		} while (0)

#define media_content_sec_warn(fmt, arg...) do { \
			SECURE_LOGW(FONT_COLOR_GREEN"[%d]"fmt"",media_content_gettid(), ##arg);     \
		} while (0)

#define media_content_sec_error(fmt, arg...) do { \
			SECURE_LOGE(FONT_COLOR_RED"[%d]"fmt"",media_content_gettid(), ##arg);     \
		} while (0)

#define ERR_BUF_LENGTH 256
#define media_content_stderror(fmt) do { \
			char media_content_stderror_buf[ERR_BUF_LENGTH] = {0, }; \
			strerror_r(errno, media_content_stderror_buf, ERR_BUF_LENGTH);	\
			LOGE(FONT_COLOR_RED fmt" : STANDARD ERROR [%s]", media_content_stderror_buf); \
		} while (0)

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /*__TIZEN_MEDIA_INFO_PRIVATE_H__*/
