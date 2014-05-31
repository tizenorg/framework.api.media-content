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


#ifndef __TIZEN_MEDIA_CONTENT_TYPE_H__
#define __TIZEN_MEDIA_CONTENT_TYPE_H__

#include <time.h>
#include <tizen.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


#ifndef DEPRECATED_API
#define DEPRECATED_API __attribute__ ((deprecated))
#endif

/**
* @addtogroup CAPI_MEDIA_CONTENT_MODULE
 * @{
 * @file media-content.h
 * @brief This file contains API related to media-content enumerations for media data types, groups, orientations,  \n
 *           classes of errors and definitions of media-data. \n
 *           Listed APIs are called when iterating over lists of album, group, bookmark and other media,  \n
 *           when media items and burst shot are inserted completely and when notification of media db change is subscribed.
 */

/**
 * @ingroup CAPI_MEDIA_CONTENT_MODULE
 * @brief The enumerations of the media file format.
 */
typedef enum
{
	MEDIA_CONTENT_TYPE_IMAGE		= 0,		/**<The type of image */
	MEDIA_CONTENT_TYPE_VIDEO		= 1,		/**<The type of video */
	MEDIA_CONTENT_TYPE_SOUND	= 2,		/**<The type of sound */
	MEDIA_CONTENT_TYPE_MUSIC		= 3,		/**<The type of music */
	MEDIA_CONTENT_TYPE_OTHERS	= 4,		/**<The type of other */
} media_content_type_e;

/**
 * @ingroup CAPI_CONTENT_MEDIA_FOLDER_MODULE
 * @brief The enumerations of the storage type.
 * @detail This information is used to establish where the folder is.
 */
typedef enum
{
	MEDIA_CONTENT_STORAGE_INTERNAL	= 0,		/**< The device's internal storage */
	MEDIA_CONTENT_STORAGE_EXTERNAL	= 1,		/**< The device's external storage */
} media_content_storage_e;

/**
 * @ingroup CAPI_MEDIA_CONTENT_MODULE
 * @brief The enumerations of media content db update item
 */
typedef enum {
	MEDIA_ITEM_FILE			= 0,				/**< File type ,an item updated to db */
	MEDIA_ITEM_DIRECTORY 	= 1,				/**< Directory type, an item updated to db */
} media_content_db_update_item_type_e;

/**
 * @ingroup CAPI_MEDIA_CONTENT_MODULE
 * @brief The enumerations of media content db update type
 */
typedef enum {
	MEDIA_CONTENT_INSERT	= 0,				/**< Insert, the type of DB update */
	MEDIA_CONTENT_DELETE 	= 1,				/**< Delete, The type of DB update */
	MEDIA_CONTENT_UPDATE	= 2,				/**< Update, The type of DB update */
} media_content_db_update_type_e;

/**
 * @ingroup CAPI_CONTENT_MEDIA_INFO_MODULE
 * @brief The type of orientation.
 */
typedef enum {
	MEDIA_CONTENT_ORIENTATION_NOT_AVAILABLE		= 0,		/**< Not available*/
	MEDIA_CONTENT_ORIENTATION_NORMAL			= 1,		/**< Normal*/
	MEDIA_CONTENT_ORIENTATION_HFLIP				= 2,		/**< Flip horizontal*/
	MEDIA_CONTENT_ORIENTATION_ROT_180			= 3,		/**< Rotate 180 degrees*/
	MEDIA_CONTENT_ORIENTATION_VFLIP				= 4,		/**< Flip vertical*/
	MEDIA_CONTENT_ORIENTATION_TRANSPOSE			= 5,		/**< Transpose*/
	MEDIA_CONTENT_ORIENTATION_ROT_90			= 6,		/**< Rotate 90 degrees*/
	MEDIA_CONTENT_ORIENTATION_TRANSVERSE		= 7,		/**< Transverse*/
	MEDIA_CONTENT_ORIENTATION_ROT_270			= 8,		/**< Rotate 270 degrees*/
} media_content_orientation_e;

/**
 * @ingroup CAPI_MEDIA_CONTENT_MODULE
 * @brief The enumerations of ordering.
 */
typedef enum
{
	MEDIA_CONTENT_ORDER_ASC		= 0,		/**< ascending order*/
	MEDIA_CONTENT_ORDER_DESC	= 1,		/**< descending order*/
} media_content_order_e;

/**
 * @ingroup CAPI_MEDIA_CONTENT_MODULE
 * @brief The enumerations of collations.
 */
typedef enum
{
	MEDIA_CONTENT_COLLATE_DEFAULT	= 0,		/**< default collation BINARY */
	MEDIA_CONTENT_COLLATE_NOCASE	= 1,		/**< collation NOCASE, not case sensitive */
	MEDIA_CONTENT_COLLATE_RTRIM		= 2,		/**< collation RTRIM, trailing space characters are ignored */
	MEDIA_CONTENT_COLLATE_LOCALIZED	= 3,		/**< collation LOCALIZATION, NOCASE also applied */
} media_content_collation_e;

/**
 * @ingroup CAPI_MEDIA_CONTENT_MODULE
 * @brief The enumerations of media content error
 */
typedef enum
{
	MEDIA_CONTENT_ERROR_NONE					= TIZEN_ERROR_NONE,					/**< Successful */
	MEDIA_CONTENT_ERROR_INVALID_PARAMETER		= TIZEN_ERROR_INVALID_PARAMETER,	/**< Invalid parameter */
	MEDIA_CONTENT_ERROR_OUT_OF_MEMORY 			= TIZEN_ERROR_OUT_OF_MEMORY,		/**< Out of memory */
	MEDIA_CONTENT_ERROR_INVALID_OPERATION		= TIZEN_ERROR_INVALID_OPERATION,	/**< Invalid Operation */
	MEDIA_CONTENT_FILE_NO_SPACE_ON_DEVICE		= TIZEN_ERROR_FILE_NO_SPACE_ON_DEVICE, /**< No space left on device */
	MEDIA_CONTENT_ERROR_DB_FAILED 				= TIZEN_ERROR_CONTENT_CLASS | 0x01,	/**< DB operation failed */
	MEDIA_CONTENT_ERROR_DB_BUSY 				= TIZEN_ERROR_CONTENT_CLASS | 0x02,	/**< DB operation BUSY */
	MEDIA_CONTENT_ERROR_NETWORK					= TIZEN_ERROR_CONTENT_CLASS | 0x03,	/**< Network Fail */
	MEDIA_CONTENT_ERROR_UNSUPPORTED_CONTENT		= TIZEN_ERROR_CONTENT_CLASS | 0x04,	/**< Unsupported Content */
} media_content_error_e;

/**
 * @ingroup CAPI_MEDIA_CONTENT_MODULE
 * @brief The enumerations of media group
 */
typedef enum {
	MEDIA_CONTENT_GROUP_DISPLAY_NAME = 0,
	MEDIA_CONTENT_GROUP_TYPE,
	MEDIA_CONTENT_GROUP_MIME_TYPE,
	MEDIA_CONTENT_GROUP_SIZE,
	MEDIA_CONTENT_GROUP_ADDED_TIME,
	MEDIA_CONTENT_GROUP_MODIFIED_TIME,
	MEDIA_CONTENT_GROUP_TITLE,
	MEDIA_CONTENT_GROUP_ARTIST,
	MEDIA_CONTENT_GROUP_ALBUM_ARTIST,
	MEDIA_CONTENT_GROUP_GENRE,
	MEDIA_CONTENT_GROUP_COMPOSER,
	MEDIA_CONTENT_GROUP_YEAR,
	MEDIA_CONTENT_GROUP_RECORDED_DATE,
	MEDIA_CONTENT_GROUP_COPYRIGHT,
	MEDIA_CONTENT_GROUP_TRACK_NUM,
	MEDIA_CONTENT_GROUP_DESCRIPTION,
	MEDIA_CONTENT_GROUP_LONGITUDE,
	MEDIA_CONTENT_GROUP_LATITUDE,
	MEDIA_CONTENT_GROUP_ALTITUDE,
	MEDIA_CONTENT_GROUP_BURST_IMAGE,
	MEDIA_CONTENT_GROUP_RATING,
	MEDIA_CONTENT_GROUP_AUTHOR,
	MEDIA_CONTENT_GROUP_PROVIDER,
	MEDIA_CONTENT_GROUP_CONTENT_NAME,
	MEDIA_CONTENT_GROUP_CATEGORY,
	MEDIA_CONTENT_GROUP_LOCATION_TAG,
	MEDIA_CONTENT_GROUP_AGE_RATING,
	MEDIA_CONTENT_GROUP_KEYWORD,
	MEDIA_CONTENT_GROUP_WEATHER,
	MEDIA_CONTENT_GROUP_MAX
} media_group_e;

/**
 * @ingroup CAPI_CONTENT_MEDIA_INFO_MODULE
 * @brief The handle to media info.
 */
typedef struct media_info_s *media_info_h;

/**
 * @ingroup CAPI_CONTENT_MEDIA_FOLDER_MODULE
 * @brief The handle to media folder.
 */
typedef struct media_folder_s *media_folder_h;

/**
 * @ingroup CAPI_CONTENT_MEDIA_PLAYLIST_MODULE
 * @brief The handle to media playlist.
 */
typedef struct media_playlist_s *media_playlist_h;

/**
 * @ingroup CAPI_CONTENT_MEDIA_TAG_MODULE
 * @brief The handle to media tag.
 */
typedef struct media_tag_s *media_tag_h;

/**
 * @ingroup CAPI_CONTENT_MEDIA_BOOKMARK_MODULE
 * @brief The handle to media bookmark.
 */
typedef struct media_bookmark_s *media_bookmark_h;

/**
 * @ingroup CAPI_CONTENT_MEDIA_ALBUM_MODULE
 * @brief The handle to media album.
 */
typedef struct media_album_s *media_album_h;

/**
 * @ingroup CAPI_CONTENT_MEDIA_IMAGE_MODULE
 * @brief The handle to image metadata.
 */
typedef struct image_meta_s *image_meta_h;

/**
 * @ingroup CAPI_CONTENT_MEDIA_VIDEO_META_MODULE
 * @brief The handle to video metadata.
 */
typedef struct video_meta_s *video_meta_h;

/**
 * @ingroup CAPI_CONTENT_MEDIA_AUDIO_META_MODULE
 * @brief The handle to audio metadata.
 */
typedef struct audio_meta_s *audio_meta_h;

/**
 * @ingroup CAPI_CONTENT_MEDIA_FILTER_MODULE
 * @brief The handle to media filter.
 */
typedef struct filter_s *filter_h;

/**
 * @ingroup CAPI_MEDIA_CONTENT_MODULE
 * @brief Called when the media scanning is finished.
 *
 * @param[in] error The error code
 * @param[in] user_data The user data passed from the foreach function
 * @pre media_content_scan()
 * @see media_content_scan()
 *
 */
typedef void (*media_scan_completed_cb)(media_content_error_e error, void * user_data);

/**
 * @ingroup CAPI_MEDIA_CONTENT_MODULE
 * @brief Called when notification of media db change is subscribed.
 *
 * @param[in] error The error code
 * @param[in] pid pid which publish notification
 * @param[in] update_item Update item of notification
 * @param[in] update_type Update type of notification
 * @param[in] media_type The type of media content(#media_content_type_e)
 * @param[in] uuid UUID of media or directory, which is updated
 * @param[in] path The path of media or directory
 * @param[in] mime_type The mime type of media info
 * @param[in] user_data The user data passed from the foreach function
 * @pre media_content_db_update_subscribe()
 * @see media_content_db_update_subscribe()
 *
 */
typedef void (*media_content_db_update_cb)(
									media_content_error_e error,
									int pid,
									media_content_db_update_item_type_e update_item,
									media_content_db_update_type_e update_type,
									media_content_type_e media_type,
									char *uuid,
									char *path,
									char *mime_type,
									void *user_data);


/**
 * @ingroup CAPI_CONTENT_MEDIA_INFO_MODULE
 * @brief Iterates over a list of media info.
 *
 * @details This callback is called for every available media info.\n
 *
 * @remarks To use the @a media outside this function, copy the handle with #media_info_clone() function.
 *
 * @param[in] media The handle to media info
 * @param[in] user_data The user data passed from the foreach function
 * @return true to continue with the next iteration of the loop,
 * @return false to break out of the loop.
 * @pre media_tag_foreach_media_from_db(), media_playlist_foreach_media_from_db(), media_genre_foreach_media_from_db(),
 * 	media_info_foreach_media_from_db(), media_folder_foreach_media_from_db() will invoke this function.
 * @see media_info_clone()
 * @see media_album_foreach_media_from_db()
 * @see media_playlist_foreach_media_from_db()
 * @see media_tag_foreach_media_from_db()
 * @see media_info_foreach_media_from_db()
 * @see media_folder_foreach_media_from_db()
 */
typedef bool (*media_info_cb)(media_info_h media, void *user_data);


/**
 * @ingroup CAPI_CONTENT_MEDIA_INFO_MODULE
 * @brief Called when media items is inserted completely.
 *
 *
 * @param[in] media The handle to media info
 * @param[in] user_data The user data passed from the foreach function
 * @pre media_info_insert_batch_to_db()
 * @see media_info_insert_batch_to_db()
 *
 */
typedef void (*media_insert_completed_cb)(media_content_error_e error, void * user_data);


/**
 * @ingroup CAPI_CONTENT_MEDIA_INFO_MODULE
 * @brief Called when burst shot is inserted completely.
 *
 * @param[in] media The handle to media info
 * @param[in] user_data The user data passed from the foreach function
 * @pre media_info_insert_burst_shot_to_db()
 * @see media_info_insert_burst_shot_to_db()
 *
 */
typedef void (*media_insert_burst_shot_completed_cb)(media_content_error_e error, void * user_data);


/**
 * @ingroup CAPI_CONTENT_MEDIA_INFO_MODULE
 * @brief Creates a thumbnail image.
 *
 * @details This callback is called for completion of generating the thumbnail image.\n
 *
 * @param[in] error The Error code
 * @param[in] path The Path of thumbnail which is generated
 * @param[in] user_data The user data passed from the foreach function
 * @pre media_info_create_thumbnail()
 * @see media_info_create_thumbnail()
 */
typedef void (*media_thumbnail_completed_cb)(media_content_error_e error, const char *path, void *user_data);


/**
 * @ingroup CAPI_CONTENT_MEDIA_FOLDER_MODULE
 * @brief Iterates over a list of folders.
 *
 * @details This callback is called for every available media folder.\n
 *
 * @remarks To use the @a folder outside this function, copy the handle with #media_folder_clone() function.
 *
 * @param[in] folder The handle to media folder
 * @param[in] user_data The user data passed from the foreach function
 * @return true to continue with the next iteration of the loop,
 * @return false to break out of the loop.
 * @pre media_folder_foreach_folder_from_db() will invoke this function.
 * @see media_folder_clone()
 * @see media_folder_foreach_folder_from_db()
 */
typedef bool (*media_folder_cb)(media_folder_h folder, void *user_data);

/**
 * @ingroup CAPI_CONTENT_MEDIA_PLAYLIST_MODULE
 * @brief Iterates over playlist list.
 *
 * @details This callback is called for every playlist in obtained list of playlists.\n
 *
 * @remarks To use the @a playlist outside this function, copy the handle with #media_playlist_clone() function.
 *
 * @param[in] playlist The handle to media playlist
 * @param[in] user_data The user data passed from the foreach function
 * @return true to continue with the next iteration of the loop,
 * @return false to break out of the loop.
 * @pre media_playlist_foreach_playlist_from_db() will invoke this function.
 * @see media_playlist_clone()
 * @see media_playlist_foreach_playlist_from_db()
 */
typedef bool (*media_playlist_cb)(media_playlist_h playlist, void *user_data);

/**
 * @ingroup CAPI_CONTENT_MEDIA_PLAYLIST_MODULE
 * @brief Iterates over playlist member.
 *
 * @details This callback is called for every media info with playlist member ID in obtained list of media info.\n
 *
 * @remarks To use the @a media outside this function, copy the handle with #media_info_clone() function.
 *
 * @param[in] playlist_member_id The ID to member of playlist
 * @param[in] media The handle to media info
 * @param[in] user_data The user data passed from the foreach function
 * @return true to continue with the next iteration of the loop,
 * @return false to break out of the loop.
 * @pre media_playlist_foreach_media_from_db() will invoke this function.
 * @see media_info_clone()
 * @see media_playlist_foreach_media_from_db()
 */
typedef bool(* playlist_member_cb)(int playlist_member_id, media_info_h media, void *user_data);

/**
 * @ingroup CAPI_CONTENT_MEDIA_TAG_MODULE
 * @brief Iterates over a list of tags
 *
 * @details This callback is called for every tag in the obtained list of tags.\n
 *
 * @remarks To use the @a tag outside this function, copy the handle with #media_tag_clone() function.
 *
 * @param[in] tag The handle to media tag
 * @param[in] user_data The user data passed from the foreach function
 * @return true to continue with the next iteration of the loop,
 * @return false to break out of the loop.
 * @pre media_tag_foreach_tag_from_db(), media_info_foreach_tag_from_db() will invoke this function.
 * @see media_tag_clone()
 * @see media_tag_foreach_tag_from_db()
 * @see media_info_foreach_tag_from_db()
 */
typedef bool (*media_tag_cb)(media_tag_h tag, void *user_data);

/**
 * @ingroup CAPI_CONTENT_MEDIA_BOOKMARK_MODULE
 * @brief Iterates over bookmark list
 *
 * @details This callback is called for every bookmark in obtained list of bookmarks.\n
 *
 * @remarks To use the @a bookmark outside this function, copy the handle with #media_bookmark_clone() function.
 *
 * @param[in] bookmark The handle to video bookmark
 * @param[in] user_data The user data passed from the foreach function
 * @return true to continue with the next iteration of the loop,
 * @return false to break out of the loop.
 * @pre media_info_foreach_bookmark_from_db() will invoke this function.
 * @see media_info_foreach_bookmark_from_db()
 */
typedef bool (*media_bookmark_cb)(media_bookmark_h bookmark, void *user_data);

/**
 * @ingroup CAPI_CONTENT_MEDIA_ALBUM_MODULE
 * @brief Iterates over album list
 *
 * @details This callback is called for every album in obtained list of groups.\n
 *
 * @remarks To use the @a album outside this function, copy the handle with #media_album_clone() function.
 *
 * @param[in] album The handle to media album
 * @param[in] user_data The user data passed from the foreach function
 * @return true to continue with the next iteration of the loop,
 * @return false to break out of the loop.
 * @pre media_album_foreach_album_from_db() will invoke this function.
 * @see media_album_clone()
 * @see media_album_foreach_album_from_db()
 */
typedef bool (*media_album_cb)(media_album_h album, void *user_data);

/**
 * @ingroup CAPI_CONTENT_MEDIA_GROUP_MODULE
 * @brief Iterates over media group list
 *
 * @details This callback is called for every group in obtained list of groups.\n
 *
 * @remarks You should not free group_name returned by this function.
 *
 * @param[in] group_name The name of media group
 * @param[in] user_data The user data passed from the foreach function
 * @return true to continue with the next iteration of the loop,
 * @return false to break out of the loop.
 * @pre media_group_foreach_group_from_db() will invoke this function.
 * @see media_group_foreach_group_from_db()
 */
typedef bool (*media_group_cb)(const char *group_name, void *user_data);

/**
 * @}
 */

/**
 * @addtogroup CAPI_CONTENT_MEDIA_INFO_MODULE
 * @{
 *
 */
#define MEDIA_ID "MEDIA_ID"	/**< media id */
#define MEDIA_PATH "MEDIA_PATH"	/**< media full path */
#define MEDIA_DISPLAY_NAME "MEDIA_DISPLAY_NAME"	/**< media base name */
#define MEDIA_TYPE "MEDIA_TYPE"		/**< media type. 0-image, 1-video, 2-sound, 3-music, 4-other*/
#define MEDIA_MIME_TYPE "MEDIA_MIME_TYPE"	/**< media mime type */
#define MEDIA_SIZE "MEDIA_SIZE"				/**< media mime size */
#define MEDIA_ADDED_TIME "MEDIA_ADDED_TIME"	/**< media added time */
#define MEDIA_MODIFIED_TIME "MEDIA_MODIFIED_TIME"		/**< media modified time */
#define MEDIA_TIMELINE "MEDIA_TIMELINE"		/**< media modified time */
#define MEDIA_THUMBNAIL_PATH "MEDIA_THUMBNAIL_PATH"	/**< media thumbnail path */
#define MEDIA_TITLE "MEDIA_TITLE"		/**< media title get from tag or file name */
#define MEDIA_ALBUM "MEDIA_ALBUM"	/**< media album name*/
#define MEDIA_ARTIST "MEDIA_ARTIST"		/**< media artist*/
#define MEDIA_ALBUM_ARTIST "MEDIA_ALBUM_ARTIST"		/**< media album_artist*/
#define MEDIA_GENRE "MEDIA_GENRE"		/**< media genre*/
#define MEDIA_COMPOSER "MEDIA_COMPOSER"	/**< media composer*/
#define MEDIA_YEAR "MEDIA_YEAR"		/**< media year*/
#define MEDIA_RECORDED_DATE "MEDIA_RECORDED_DATE"	/**< media recorded date*/
#define MEDIA_COPYRIGHT "MEDIA_COPYRIGHT"		/**< media copyright*/
#define MEDIA_TRACK_NUM "MEDIA_TRACK_NUM"	/**< media track number*/
#define MEDIA_DESCRIPTION "MEDIA_DESCRIPTION"	/**< media description*/
#define MEDIA_BITRATE "MEDIA_BITRATE"		/**< media bitrate*/
#define MEDIA_SAMPLERATE "MEDIA_SAMPLERATE"	/**< media sample rate*/
#define MEDIA_CHANNEL "MEDIA_CHANNEL"	/**< media channel*/
#define MEDIA_DURATION "MEDIA_DURATION"	/**< media duration */
#define MEDIA_LONGITUDE "MEDIA_LONGITUDE"		/**< media longitude */
#define MEDIA_LATITUDE "MEDIA_LATITUDE"	/**< media latitude */
#define MEDIA_ALTITUDE "MEDIA_ALTITUDE"	/**< media altitude */
#define MEDIA_WIDTH "MEDIA_WIDTH"	/**< media width*/
#define MEDIA_HEIGHT "MEDIA_HEIGHT"	/**< media height*/
#define MEDIA_DATETAKEN "MEDIA_DATETAKEN"	/**< media datetaken*/
#define MEDIA_ORIENTATION "MEDIA_ORIENTATION"	/**< media orientation*/
#define MEDIA_BURST_ID "BURST_ID" /**< media burst id*/
#define MEDIA_PLAYED_COUNT "MEDIA_PLAYED_COUNT"	/**< media playedcount*/
#define MEDIA_LAST_PLAYED_TIME "MEDIA_LAST_PLAYED_TIME"	/**< media last played time*/
#define MEDIA_LAST_PLAYED_POSITION "MEDIA_LAST_PLAYED_POSITION"	/**< media last played position of file*/
#define MEDIA_RATING "MEDIA_RATING"	/**< media rating*/
#define MEDIA_FAVOURITE "MEDIA_FAVORITE"	/**< 0-not favourite, 1-favourite*/
#define MEDIA_AUTHOR "MEDIA_AUTHOR"	/**< media authore*/
#define MEDIA_PROVIDER "MEDIA_PROVIDER"	/**< media provider*/
#define MEDIA_CONTENT_NAME "MEDIA_CONTENT_NAME"	/**< media content name*/
#define MEDIA_CATEGORY "MEDIA_CATEGORY"	/**< media category*/
#define MEDIA_LOCATION_TAG "MEDIA_LOCATION_TAG"	/**< media location tag*/
#define MEDIA_AGE_RATING "MEDIA_AGE_RATING"	/**< media age rating*/
#define MEDIA_KEYWORD "MEDIA_KEYWORD"	/**< media keyword*/
#define MEDIA_WEATHER "MEDIA_WEATHER"	/**< media weather*/
#define MEDIA_IS_DRM "MEDIA_IS_DRM"	/**< is drm. 0-not drm, 1-drm*/
#define MEDIA_STORAGE_TYPE "MEDIA_STORAGE_TYPE"	/**< media storage. 0-internal storage, 1-external storage*/

#define MEDIA_FILE_NAME_PINYIN "MEDIA_FILE_NAME_PINYIN"		/**< media file name pinyin */
#define MEDIA_TITLE_PINYIN "MEDIA_TITLE_PINYIN"		/**< media title pinyin */
#define MEDIA_ALBUM_PINYIN "MEDIA_ALBUM_PINYIN"	/**< media album pinyin*/
#define MEDIA_ARTIST_PINYIN "MEDIA_ARTIST_PINYIN"		/**< media artist pinyin*/
#define MEDIA_ALBUM_ARTIST_PINYIN "MEDIA_ALBUM_ARTIST_PINYIN"		/**< media album_artist pinyin*/
#define MEDIA_GENRE_PINYIN "MEDIA_GENRE_PINYIN"		/**< media genre pinyin*/
#define MEDIA_COMPOSER_PINYIN "MEDIA_COMPOSER_PINYIN"	/**< media composer pinyin*/
#define MEDIA_COPYRIGHT_PINYIN "MEDIA_COPYRIGHT_PINYIN"		/**< media copyright pinyin*/
#define MEDIA_DESCRIPTION_PINYIN "MEDIA_DESCRIPTION_PINYIN"	/**< media description pinyin*/
#define MEDIA_AUTHOR_PINYIN "MEDIA_AUTHOR_PINYIN"	/**< media authore pinyin*/
#define MEDIA_PROVIDER_PINYIN "MEDIA_PROVIDER_PINYIN"	/**< media provider pinyin*/
#define MEDIA_CONTENT_NAME_PINYIN "MEDIA_CONTENT_NAME_PINYIN"	/**< media content name pinyin*/
#define MEDIA_CATEGORY_PINYIN "MEDIA_CATEGORY_PINYIN"	/**< media category pinyin*/
#define MEDIA_LOCATION_TAG_PINYIN "MEDIA_LOCATION_TAG_PINYIN"	/**< media location tag pinyin*/
#define MEDIA_AGE_RATING_PINYIN "MEDIA_AGE_RATING_PINYIN"	/**< media age rating pinyin*/
#define MEDIA_KEYWORD_PINYIN "MEDIA_KEYWORD_PINYIN"	/**< media keyword pinyin*/
/**
 * @}
 */


/**

 * @addtogroup CAPI_CONTENT_MEDIA_FOLDER_MODULE
 * @{
 */
#define FOLDER_ID "FOLDER_ID"	/**< folder id */
#define FOLDER_PATH "FOLDER_PATH"	/**< folder full path */
#define FOLDER_NAME "FOLDER_NAME"		/**< folder base name */
#define FOLDER_MODIFIED_TIME "FOLDER_MODIFIED_TIME"	/**< folder modified time */
#define FOLDER_STORAGE_TYPE "FOLDER_STORAGE_TYPE"	/**< folder storage. 0-internal storage, 1-external storage*/
#define FOLDER_NAME_PINYIN "FOLDER_NAME_PINYIN"		/**< folder base name pinyin*/

/**
 * @}
 */

/**
 * @addtogroup CAPI_CONTENT_MEDIA_PLAYLIST_MODULE
 * @{
 */
#define PLAYLIST_NAME "PLAYLIST_NAME"	/**< playlist name */
#define PLAYLIST_MEMBER_ORDER "PLAYLIST_MEMBER_ORDER"	/**< playlist name */
#define PLAYLIST_MEDIA_COUNT "PLAYLIST_MEDIA_COUNT" /**< media count in playlist view */

/**
 * @}
 */

/**
 * @addtogroup CAPI_CONTENT_MEDIA_TAG_MODULE
 * @{
 */
#define TAG_NAME "TAG_NAME"	/**< tag name */
#define TAG_MEDIA_COUNT "TAG_MEDIA_COUNT"	/**< media count in tag view */

/**
 * @}
 */

/**
 * @addtogroup CAPI_CONTENT_MEDIA_BOOKMARK_MODULE
 * @{
 */
#define BOOKMARK_MARKED_TIME "BOOKMARK_MARKED_TIME"	/**< bookmark marked time */

/**
 * @}
 */

#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif /*__TIZEN_MEDIA_CONTENT_TYPE_H__*/
