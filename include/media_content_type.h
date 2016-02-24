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
 * @file media_content_type.h
 * @brief This file contains API related to media-content enumerations for media data types, groups, orientations, \n
 *        classes of errors and definitions of media-data. \n
 *        Listed APIs are called when iterating over lists of album, group, bookmark and other media,  \n
 *        when media items and burst shot are inserted completely and when notification of media DB change is subscribed.
 */

/**
* @addtogroup CAPI_MEDIA_CONTENT_MODULE
 * @{
 */

/**
 * @ingroup CAPI_MEDIA_CONTENT_MODULE
 * @brief Enumeration for the media file format.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 */
typedef enum
{
    MEDIA_CONTENT_TYPE_IMAGE   = 0,  /**<The type of an image */
    MEDIA_CONTENT_TYPE_VIDEO   = 1,  /**<The type of a video */
    MEDIA_CONTENT_TYPE_SOUND   = 2,  /**<The type of sound */
    MEDIA_CONTENT_TYPE_MUSIC   = 3,  /**<The type of music */
    MEDIA_CONTENT_TYPE_OTHERS  = 4,  /**<The type of other */
} media_content_type_e;

/**
 * @ingroup CAPI_CONTENT_MEDIA_CONTENT_MODULE
 * @brief Enumeration for the storage type.
 * @details This information is used to establish where the folder is.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 */
typedef enum
{
	MEDIA_CONTENT_STORAGE_INTERNAL	= 0,  /**< The device's internal storage */
	MEDIA_CONTENT_STORAGE_EXTERNAL	= 1,  /**< The device's external storage like sd card*/
	MEDIA_CONTENT_STORAGE_EXTERNAL_USB = 2, /**< The external USB storage (Since 2.4) */
	MEDIA_CONTENT_STORAGE_CLOUD	= 100,  /**< The Cloud storage (Since 2.4) */
} media_content_storage_e;

/**
 * @ingroup CAPI_MEDIA_CONTENT_MODULE
 * @brief Enumeration for media content DB update items.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 */
typedef enum {
    MEDIA_ITEM_FILE       = 0,              /**< File type, an item updated to DB */
    MEDIA_ITEM_DIRECTORY  = 1,              /**< Directory type, an item updated to DB */
} media_content_db_update_item_type_e;

/**
 * @ingroup CAPI_MEDIA_CONTENT_MODULE
 * @brief Enumeration for media content DB update types.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 */
typedef enum {
    MEDIA_CONTENT_INSERT  = 0,              /**< Insert, the type of DB update */
    MEDIA_CONTENT_DELETE  = 1,              /**< Delete, The type of DB update */
    MEDIA_CONTENT_UPDATE  = 2,              /**< Update, The type of DB update */
} media_content_db_update_type_e;

/**
 * @ingroup CAPI_CONTENT_MEDIA_INFO_MODULE
 * @brief Enumeration for orientation types.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 */
typedef enum {
    MEDIA_CONTENT_ORIENTATION_NOT_AVAILABLE  = 0,       /**< Not available*/
    MEDIA_CONTENT_ORIENTATION_NORMAL         = 1,       /**< Normal*/
    MEDIA_CONTENT_ORIENTATION_HFLIP          = 2,       /**< Flip horizontal*/
    MEDIA_CONTENT_ORIENTATION_ROT_180        = 3,       /**< Rotate 180 degrees*/
    MEDIA_CONTENT_ORIENTATION_VFLIP          = 4,       /**< Flip vertical*/
    MEDIA_CONTENT_ORIENTATION_TRANSPOSE      = 5,       /**< Transpose*/
    MEDIA_CONTENT_ORIENTATION_ROT_90         = 6,       /**< Rotate 90 degrees*/
    MEDIA_CONTENT_ORIENTATION_TRANSVERSE     = 7,       /**< Transverse*/
    MEDIA_CONTENT_ORIENTATION_ROT_270        = 8,       /**< Rotate 270 degrees*/
} media_content_orientation_e;

/**
 * @ingroup CAPI_MEDIA_CONTENT_MODULE
 * @brief Enumeration for ordering.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 */
typedef enum
{
    MEDIA_CONTENT_ORDER_ASC   = 0,      /**< Ascending order*/
    MEDIA_CONTENT_ORDER_DESC  = 1,      /**< Descending order*/
} media_content_order_e;

/**
 * @ingroup CAPI_MEDIA_CONTENT_MODULE
 * @brief Enumeration for collations.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 */
typedef enum
{
    MEDIA_CONTENT_COLLATE_DEFAULT   = 0,        /**< Default collation BINARY */
    MEDIA_CONTENT_COLLATE_NOCASE    = 1,        /**< Collation NOCASE, not case sensitive */
    MEDIA_CONTENT_COLLATE_RTRIM     = 2,        /**< Collation RTRIM, trailing space characters are ignored */
    MEDIA_CONTENT_COLLATE_LOCALIZED = 3,        /**< Collation LOCALIZATION, NOCASE also applied */
} media_content_collation_e;

#define MEDIA_CONTENT_ERROR_CLASS				TIZEN_ERROR_MEDIA_CONTENT

/**
 * @ingroup CAPI_MEDIA_CONTENT_MODULE
 * @brief Enumeration for a media content error.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 */
 typedef enum
{
    MEDIA_CONTENT_ERROR_NONE                    = TIZEN_ERROR_NONE,                    /**< Successful */
    MEDIA_CONTENT_ERROR_INVALID_PARAMETER       = TIZEN_ERROR_INVALID_PARAMETER,       /**< Invalid parameter */
    MEDIA_CONTENT_ERROR_OUT_OF_MEMORY           = TIZEN_ERROR_OUT_OF_MEMORY,           /**< Out of memory */
    MEDIA_CONTENT_ERROR_INVALID_OPERATION       = TIZEN_ERROR_INVALID_OPERATION,       /**< Invalid Operation */
    MEDIA_CONTENT_FILE_NO_SPACE_ON_DEVICE       = TIZEN_ERROR_FILE_NO_SPACE_ON_DEVICE, /**< No space left on device */
    MEDIA_CONTENT_ERROR_PERMISSION_DENIED		  = TIZEN_ERROR_PERMISSION_DENIED,		  /**< Permission denied */
    MEDIA_CONTENT_ERROR_DB_FAILED               = MEDIA_CONTENT_ERROR_CLASS | 0x01,    /**< DB operation failed */
    MEDIA_CONTENT_ERROR_DB_BUSY                 = MEDIA_CONTENT_ERROR_CLASS | 0x02,    /**< DB operation BUSY */
    MEDIA_CONTENT_ERROR_NETWORK                 = MEDIA_CONTENT_ERROR_CLASS | 0x03,    /**< Network Fail */
    MEDIA_CONTENT_ERROR_UNSUPPORTED_CONTENT     = MEDIA_CONTENT_ERROR_CLASS | 0x04,    /**< Unsupported Content */
} media_content_error_e;

/**
 * @ingroup CAPI_MEDIA_CONTENT_MODULE
 * @brief Enumeration for a media group.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 */
typedef enum {
    MEDIA_CONTENT_GROUP_DISPLAY_NAME = 0,    /**< Media group ID for display name */
    MEDIA_CONTENT_GROUP_TYPE,                /**< Media group ID for a media type */
    MEDIA_CONTENT_GROUP_MIME_TYPE,           /**< Media group ID for a mime type */
    MEDIA_CONTENT_GROUP_SIZE,                /**< Media group ID for content size */
    MEDIA_CONTENT_GROUP_ADDED_TIME,          /**< Media group ID for the added time */
    MEDIA_CONTENT_GROUP_MODIFIED_TIME,       /**< Media group ID for the modified time */
    MEDIA_CONTENT_GROUP_TITLE,               /**< Media group ID for a content title */
    MEDIA_CONTENT_GROUP_ARTIST,              /**< Media group ID for an artist*/
    MEDIA_CONTENT_GROUP_ALBUM_ARTIST,        /**< Media group ID for an album artist */
    MEDIA_CONTENT_GROUP_GENRE,               /**< Media group ID for a genre*/
    MEDIA_CONTENT_GROUP_COMPOSER,            /**< Media group ID for a composer*/
    MEDIA_CONTENT_GROUP_YEAR,                /**< Media group ID for a year*/
    MEDIA_CONTENT_GROUP_RECORDED_DATE,       /**< Media group ID for the recorded date*/
    MEDIA_CONTENT_GROUP_COPYRIGHT,           /**< Media group ID for the copyright*/
    MEDIA_CONTENT_GROUP_TRACK_NUM,           /**< Media group ID for a track number*/
    MEDIA_CONTENT_GROUP_DESCRIPTION,         /**< Media group ID for a description */
    MEDIA_CONTENT_GROUP_LONGITUDE,           /**< Media group ID for the longitude*/
    MEDIA_CONTENT_GROUP_LATITUDE,            /**< Media group ID for the latitude*/
    MEDIA_CONTENT_GROUP_ALTITUDE,            /**< Media group ID for the altitude*/
    MEDIA_CONTENT_GROUP_BURST_IMAGE,         /**< Media group ID for the burst shot*/
    MEDIA_CONTENT_GROUP_RATING,              /**< Media group ID for a rating*/
    MEDIA_CONTENT_GROUP_AUTHOR,              /**< Media group ID for an author*/
    MEDIA_CONTENT_GROUP_PROVIDER,            /**< Media group ID for a provider*/
    MEDIA_CONTENT_GROUP_CONTENT_NAME,        /**< Media group ID for the content name*/
    MEDIA_CONTENT_GROUP_CATEGORY,            /**< Media group ID for a category*/
    MEDIA_CONTENT_GROUP_LOCATION_TAG,        /**< Media group ID for a location tag*/
    MEDIA_CONTENT_GROUP_AGE_RATING,          /**< Media group ID for an age rating*/
    MEDIA_CONTENT_GROUP_KEYWORD,             /**< Media group ID for a keyword*/
    MEDIA_CONTENT_GROUP_WEATHER,             /**< Media group ID for the weather*/
    MEDIA_CONTENT_GROUP_MAX                  /**< Invalid media group ID*/
} media_group_e;

/**
 * @ingroup CAPI_CONTENT_MEDIA_INFO_MODULE
 * @brief The structure type for the Media info handle.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 */
typedef struct media_info_s *media_info_h;

/**
 * @ingroup CAPI_CONTENT_MEDIA_FOLDER_MODULE
 * @brief The structure type for the Media folder handle.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 */
typedef struct media_folder_s *media_folder_h;

/**
 * @ingroup CAPI_CONTENT_MEDIA_PLAYLIST_MODULE
 * @brief The structure type for the Media playlist handle.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 */
typedef struct media_playlist_s *media_playlist_h;

/**
 * @ingroup CAPI_CONTENT_MEDIA_TAG_MODULE
 * @brief The structure type for the Media tag handle.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 */
typedef struct media_tag_s *media_tag_h;

/**
 * @ingroup CAPI_CONTENT_MEDIA_BOOKMARK_MODULE
 * @brief The structure type for the Media bookmark handle.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 */
typedef struct media_bookmark_s *media_bookmark_h;

/**
 * @ingroup CAPI_CONTENT_MEDIA_ALBUM_MODULE
 * @brief The structure type for the Media album handle.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 */
typedef struct media_album_s *media_album_h;

/**
 * @ingroup CAPI_CONTENT_MEDIA_IMAGE_MODULE
 * @brief The structure type for the Image metadata handle.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 */
typedef struct image_meta_s *image_meta_h;

/**
 * @ingroup CAPI_CONTENT_MEDIA_VIDEO_META_MODULE
 * @brief The structure type for the Video metadata handle.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 */
typedef struct video_meta_s *video_meta_h;

/**
 * @ingroup CAPI_CONTENT_MEDIA_AUDIO_META_MODULE
 * @brief The structure type for the Audio metadata handle.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 */
typedef struct audio_meta_s *audio_meta_h;

/**
 * @ingroup CAPI_CONTENT_MEDIA_FILTER_MODULE
 * @brief The structure type for the Media filter handle.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 */
typedef struct filter_s *filter_h;

/**
 * @ingroup CAPI_CONTENT_MEDIA_STORAGE_MODULE
 * @brief The structure type for the Media storage handle.
 * @since_tizen 2.4
 */
typedef void *media_storage_h;

/**
 * @ingroup CAPI_MEDIA_CONTENT_MODULE
 * @brief Called when the media scanning is finished.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 * @param[in] error     The error code
 * @param[in] user_data The user data passed from the foreach function
 *
 * @pre media_content_scan().
 *
 * @see media_content_scan()
 *
 */
typedef void (*media_scan_completed_cb)(media_content_error_e error, void * user_data);

/**
 * @ingroup CAPI_MEDIA_CONTENT_MODULE
 * @brief Called when the notification of the media DB change is subscribed.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 * @param[in] error       The error code
 * @param[in] pid         The PID which publishes notification
 * @param[in] update_item The update item of notification
 * @param[in] update_type The update type of notification
 * @param[in] media_type  The type of the media content (#media_content_type_e)
 * @param[in] uuid        The UUID of media or directory, which is updated
 * @param[in] path        The path of the media or directory
 * @param[in] mime_type   The mime type of the media info
 * @param[in] user_data   The user data passed from the foreach function
 *
 * @pre media_content_db_update_subscribe().
 * @see media_content_db_update_subscribe()
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
 * @brief Called for every available media info.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 * @details Iterates over a list of media info.
 *
 * @remarks To use the @a media outside this function, copy the handle with media_info_clone() function.
 *
 * @param[in] media     The handle to the media info
 * @param[in] user_data The user data passed from the foreach function
 *
 * @return @c true to continue with the next iteration of the loop,
 *         otherwise @c false to break out of the loop
 *
 * @pre media_tag_foreach_media_from_db(), media_playlist_foreach_media_from_db(), media_genre_foreach_media_from_db(),
 *      media_info_foreach_media_from_db(), media_folder_foreach_media_from_db() will invoke this function.
 *
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
 * @brief Called when media items are inserted completely.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 * @param[in] media     The handle to the media info
 * @param[in] user_data The user data passed from the foreach function
 *
 * @pre media_info_insert_batch_to_db()
 *
 * @see media_info_insert_batch_to_db()
 */
typedef void (*media_insert_completed_cb)(media_content_error_e error, void * user_data);


/**
 * @ingroup CAPI_CONTENT_MEDIA_INFO_MODULE
 * @brief Called when the burst shot is inserted completely.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 * @param[in] media     The handle to the media info
 * @param[in] user_data The user data passed from the foreach function
 *
 * @pre media_info_insert_burst_shot_to_db()
 *
 * @see media_info_insert_burst_shot_to_db()
 *
 */
typedef void (*media_insert_burst_shot_completed_cb)(media_content_error_e error, void * user_data);


/**
 * @ingroup CAPI_CONTENT_MEDIA_INFO_MODULE
 * @brief Called when creating a thumbnail image.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 * @details This callback is called for completion of generating the thumbnail image.
 *
 * @param[in] error     The error code
 * @param[in] path      The path of the thumbnail which is generated
 * @param[in] user_data The user data passed from the foreach function
 *
 * @pre media_info_create_thumbnail()
 *
 * @see media_info_create_thumbnail()
 */
typedef void (*media_thumbnail_completed_cb)(media_content_error_e error, const char *path, void *user_data);


/**
 * @ingroup CAPI_CONTENT_MEDIA_FOLDER_MODULE
 * @brief Called for every available media folder.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 * @details Iterates over a list of folders.
 *
 * @remarks To use the @a folder outside this function, copy the handle with the media_folder_clone() function.
 *
 * @param[in] folder    The handle to the media folder
 * @param[in] user_data The user data passed from the foreach function
 *
 * @return @c true to continue with the next iteration of the loop,
 *         otherwise @c false to break out of the loop
 *
 * @pre media_folder_foreach_folder_from_db() will invoke this function.
 * @see media_folder_clone()
 * @see media_folder_foreach_folder_from_db()
 */
typedef bool (*media_folder_cb)(media_folder_h folder, void *user_data);

/**
 * @ingroup CAPI_CONTENT_MEDIA_PLAYLIST_MODULE
 * @brief Called for every playlist in the obtained list of playlists.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 * @details Iterates over a playlist list.
 *
 * @remarks To use the @a playlist outside this function, copy the handle with the media_playlist_clone() function.
 *
 * @param[in] playlist  The handle to the media playlist
 * @param[in] user_data The user data passed from the foreach function
 *
 * @return @c true to continue with the next iteration of the loop,
 *         otherwise @c false to break out of the loop
 *
 * @pre media_playlist_foreach_playlist_from_db() will invoke this function.
 *
 * @see media_playlist_clone()
 * @see media_playlist_foreach_playlist_from_db()
 */
typedef bool (*media_playlist_cb)(media_playlist_h playlist, void *user_data);

/**
 * @ingroup CAPI_CONTENT_MEDIA_PLAYLIST_MODULE
 * @brief Called for every media info with playlist member ID in the obtained list of media info.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 * @details Iterates over playlist members.
 *
 * @remarks To use the @a media outside this function, copy the handle with the media_info_clone() function.
 *
 * @param[in] playlist_member_id The ID to member of the playlist
 * @param[in] media              The handle to the media info
 * @param[in] user_data          The user data passed from the foreach function
 *
 * @return @c true to continue with the next iteration of the loop,
 *         otherwise @c false to break out of the loop
 *
 * @pre media_playlist_foreach_media_from_db() will invoke this function.
 *
 * @see media_info_clone()
 * @see media_playlist_foreach_media_from_db()
 */
typedef bool(* playlist_member_cb)(int playlist_member_id, media_info_h media, void *user_data);

/**
 * @ingroup CAPI_CONTENT_MEDIA_TAG_MODULE
 * @brief Called for every tag in the obtained list of tags.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 * @details Iterates over a list of tags.
 *
 * @remarks To use the @a tag outside this function, copy the handle with the media_tag_clone() function.
 *
 * @param[in] tag       The handle to the media tag
 * @param[in] user_data The user data passed from the foreach function
 *
 * @return @c true to continue with the next iteration of the loop,
 *         otherwise @c false to break out of the loop
 *
 * @pre media_tag_foreach_tag_from_db(), media_info_foreach_tag_from_db() will invoke this function.
 *
 * @see media_tag_clone()
 * @see media_tag_foreach_tag_from_db()
 * @see media_info_foreach_tag_from_db()
 */
typedef bool (*media_tag_cb)(media_tag_h tag, void *user_data);

/**
 * @ingroup CAPI_CONTENT_MEDIA_BOOKMARK_MODULE
 * @brief Called for every bookmark in the obtained list of bookmarks.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 * @details Iterates over a bookmark list.
 *
 * @remarks To use the @a bookmark outside this function, copy the handle with the media_bookmark_clone() function.
 *
 * @param[in] bookmark  The handle to the video bookmark
 * @param[in] user_data The user data passed from the foreach function
 *
 * @return @c true to continue with the next iteration of the loop,
 *         otherwise @c false to break out of the loop
 *
 * @pre media_info_foreach_bookmark_from_db() will invoke this function.
 *
 * @see media_info_foreach_bookmark_from_db()
 */
typedef bool (*media_bookmark_cb)(media_bookmark_h bookmark, void *user_data);

/**
 * @ingroup CAPI_CONTENT_MEDIA_ALBUM_MODULE
 * @brief Called for every album in the obtained list of groups.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 * @details Iterates over an album list.
 *
 * @remarks To use the @a album outside this function, copy the handle with the media_album_clone() function.
 *
 * @param[in] album     The handle to the media album
 * @param[in] user_data The user data passed from the foreach function
 *
 * @return @c true to continue with the next iteration of the loop,
 *         otherwise @c false to break out of the loop
 *
 * @pre media_album_foreach_album_from_db() will invoke this function.
 *
 * @see media_album_clone()
 * @see media_album_foreach_album_from_db()
 */
typedef bool (*media_album_cb)(media_album_h album, void *user_data);

/**
 * @ingroup CAPI_CONTENT_MEDIA_GROUP_MODULE
 * @brief Called for every group in the obtained list of groups.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 * @details Iterates over a media group list.
 *
 * @remarks You should not free @a group_name returned by this function.
 *
 * @param[in] group_name The name of the media group
 * @param[in] user_data  The user data passed from the foreach function
 *
 * @return @c true to continue with the next iteration of the loop,
 *         otherwise @c false to break out of the loop
 *
 * @pre media_group_foreach_group_from_db() will invoke this function.
 *
 * @see media_group_foreach_group_from_db()
 */
typedef bool (*media_group_cb)(const char *group_name, void *user_data);

/**
 * @ingroup CAPI_CONTENT_MEDIA_STORAGE_MODULE
 * @brief Called for every storage in the obtained list of storages.
 * @since_tizen 2.4
 *
 * @details Iterates over a media storage list.
 *
 * @remarks You should not destroy @a storage returned by this function.
 *
 * @param[in] storage     The handle of the media storage
 * @param[in] user_data  The user data passed from the foreach function
 *
 * @return @c true to continue with the next iteration of the loop,
 *         otherwise @c false to break out of the loop
 *
 * @pre media_storage_foreach_storage_from_db() will invoke this function.
 *
 * @see media_storage_foreach_storage_from_db()
 */
typedef bool (*media_storage_cb)(media_storage_h storage, void *user_data);

/**
 * @}
 */

/**
 * @addtogroup CAPI_CONTENT_MEDIA_FILTER_MODULE
 * @{
 */

 /**
 * @brief You can use above define to set the condition of media filter and order keyword.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 */
#define MEDIA_ID "MEDIA_ID" /**< Media ID */

/**
 * @brief You can use above define to set the condition of media filter and order keyword.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 */
#define MEDIA_PATH "MEDIA_PATH"  /**< Media full path */

/**
 * @brief You can use above define to set the condition of media filter and order keyword.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 */
#define MEDIA_DISPLAY_NAME "MEDIA_DISPLAY_NAME"  /**< Media base name */

/**
 * @brief You can use above define to set the condition of media filter and order keyword.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 */
#define MEDIA_TYPE "MEDIA_TYPE"  /**< Media type: 0-image, 1-video, 2-sound, 3-music, 4-other*/

/**
 * @brief You can use above define to set the condition of media filter and order keyword.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 */
#define MEDIA_MIME_TYPE "MEDIA_MIME_TYPE"  /**< Media MIME type */


/**
 * @brief You can use above define to set the condition of media filter and order keyword.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 */
#define MEDIA_SIZE "MEDIA_SIZE"  /**< Media MIME size */

/**
 * @brief You can use above define to set the condition of media filter and order keyword.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 */
#define MEDIA_ADDED_TIME "MEDIA_ADDED_TIME"  /**< Media added time */

/**
 * @brief You can use above define to set the condition of media filter and order keyword.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 */
#define MEDIA_MODIFIED_TIME "MEDIA_MODIFIED_TIME"  /**< Media modified time */

/**
 * @brief You can use above define to set the condition of media filter and order keyword.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 */
#define MEDIA_TIMELINE "MEDIA_TIMELINE"  /**< Media modified time */

/**
 * @brief You can use above define to set the condition of media filter and order keyword.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 */
#define MEDIA_THUMBNAIL_PATH "MEDIA_THUMBNAIL_PATH"  /**< Media thumbnail path */

/**
 * @brief You can use above define to set the condition of media filter and order keyword.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 */
#define MEDIA_TITLE "MEDIA_TITLE"  /**< Media title get from tag or file name */

/**
 * @brief You can use above define to set the condition of media filter and order keyword.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 */
#define MEDIA_ALBUM "MEDIA_ALBUM"  /**< Media album name*/

/**
 * @brief You can use above define to set the condition of media filter and order keyword.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 */
#define MEDIA_ARTIST "MEDIA_ARTIST"  /**< Media artist*/

/**
 * @brief You can use above define to set the condition of media filter and order keyword.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 */
#define MEDIA_ALBUM_ARTIST "MEDIA_ALBUM_ARTIST"  /**< Media album_artist*/

/**
 * @brief You can use above define to set the condition of media filter and order keyword.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 */
#define MEDIA_GENRE "MEDIA_GENRE"  /**< Media genre*/

/**
 * @brief You can use above define to set the condition of media filter and order keyword.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 */
#define MEDIA_COMPOSER "MEDIA_COMPOSER"  /**< Media composer*/

/**
 * @brief You can use above define to set the condition of media filter and order keyword.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 */
#define MEDIA_YEAR "MEDIA_YEAR"  /**< Media year*/

/**
 * @brief You can use above define to set the condition of media filter and order keyword.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 */
#define MEDIA_RECORDED_DATE "MEDIA_RECORDED_DATE"  /**< Media recorded date*/

/**
 * @brief You can use above define to set the condition of media filter and order keyword.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 */
#define MEDIA_COPYRIGHT "MEDIA_COPYRIGHT"  /**< Media copyright*/

/**
 * @brief You can use above define to set the condition of media filter and order keyword.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 */
#define MEDIA_TRACK_NUM "MEDIA_TRACK_NUM"  /**< Media track number*/

/**
 * @brief You can use above define to set the condition of media filter and order keyword.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 */
#define MEDIA_DESCRIPTION "MEDIA_DESCRIPTION"  /**< Media description*/

/**
 * @brief You can use above define to set the condition of media filter and order keyword.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 */
#define MEDIA_BITRATE "MEDIA_BITRATE"  /**< Media bitrate*/

/**
 * @brief You can use above define to set the condition of media filter and order keyword.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 */
#define MEDIA_BITPERSAMPLE "MEDIA_BITPERSAMPLE"  /**< Media bit per sample*/

/**
 * @brief You can use above define to set the condition of media filter and order keyword.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 */
#define MEDIA_SAMPLERATE "MEDIA_SAMPLERATE"  /**< Media sample rate*/

/**
 * @brief You can use above define to set the condition of media filter and order keyword.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 */
#define MEDIA_CHANNEL "MEDIA_CHANNEL"  /**< Media channel*/

/**
 * @brief You can use above define to set the condition of media filter and order keyword.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 */
#define MEDIA_DURATION "MEDIA_DURATION"  /**< Media duration */

/**
 * @brief You can use above define to set the condition of media filter and order keyword.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 */
#define MEDIA_LONGITUDE "MEDIA_LONGITUDE"  /**< Media longitude */

/**
 * @brief You can use above define to set the condition of media filter and order keyword.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 */
#define MEDIA_LATITUDE "MEDIA_LATITUDE"  /**< Media latitude */

/**
 * @brief You can use above define to set the condition of media filter and order keyword.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 */
#define MEDIA_ALTITUDE "MEDIA_ALTITUDE"  /**< Media altitude */

/**
 * @brief You can use above define to set the condition of media filter and order keyword.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 */
#define MEDIA_WIDTH "MEDIA_WIDTH"  /**< Media width*/

/**
 * @brief You can use above define to set the condition of media filter and order keyword.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 */
#define MEDIA_HEIGHT "MEDIA_HEIGHT"  /**< Media height*/

/**
 * @brief You can use above define to set the condition of media filter and order keyword.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 */
#define MEDIA_DATETAKEN "MEDIA_DATETAKEN"  /**< Media datetaken*/

/**
 * @brief You can use above define to set the condition of media filter and order keyword.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 */
#define MEDIA_ORIENTATION "MEDIA_ORIENTATION"  /**< Media orientation*/

/**
 * @brief You can use above define to set the condition of media filter and order keyword.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 */
#define MEDIA_BURST_ID "BURST_ID"  /**< Media burst ID*/

/**
 * @brief You can use above define to set the condition of media filter and order keyword.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 */
#define MEDIA_PLAYED_COUNT "MEDIA_PLAYED_COUNT"  /**< Media playedcount*/

/**
 * @brief You can use above define to set the condition of media filter and order keyword.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 */
#define MEDIA_LAST_PLAYED_TIME "MEDIA_LAST_PLAYED_TIME"  /**< Media last played time*/

/**
 * @brief You can use above define to set the condition of media filter and order keyword.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 */
#define MEDIA_LAST_PLAYED_POSITION "MEDIA_LAST_PLAYED_POSITION"  /**< Media last played position of file*/

/**
 * @brief You can use above define to set the condition of media filter and order keyword.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 */
#define MEDIA_RATING "MEDIA_RATING"  /**< Media rating*/

/**
 * @brief You can use above define to set the condition of media filter and order keyword.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 */
#define MEDIA_FAVOURITE "MEDIA_FAVOURITE"  /**< 0-not favourite, 1-favourite*/

/**
 * @brief You can use above define to set the condition of media filter and order keyword.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 */
#define MEDIA_AUTHOR "MEDIA_AUTHOR"  /**< Media authore*/

/**
 * @brief You can use above define to set the condition of media filter and order keyword.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 */
#define MEDIA_PROVIDER "MEDIA_PROVIDER"  /**< Media provider*/

/**
 * @brief You can use above define to set the condition of media filter and order keyword.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 */
#define MEDIA_CONTENT_NAME "MEDIA_CONTENT_NAME"  /**< Media content name*/

/**
 * @brief You can use above define to set the condition of media filter and order keyword.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 */
#define MEDIA_CATEGORY "MEDIA_CATEGORY"  /**< Media category*/

/**
 * @brief You can use above define to set the condition of media filter and order keyword.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 */
#define MEDIA_LOCATION_TAG "MEDIA_LOCATION_TAG"  /**< Media location tag*/

/**
 * @brief You can use above define to set the condition of media filter and order keyword.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 */
#define MEDIA_AGE_RATING "MEDIA_AGE_RATING"  /**< Media age rating*/

/**
 * @brief You can use above define to set the condition of media filter and order keyword.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 */
#define MEDIA_KEYWORD "MEDIA_KEYWORD"  /**< Media keyword*/

/**
 * @brief You can use above define to set the condition of media filter and order keyword.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 */
#define MEDIA_WEATHER "MEDIA_WEATHER"  /**< Media weather*/

/**
 * @brief You can use above define to set the condition of media filter and order keyword.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 */
#define MEDIA_IS_DRM "MEDIA_IS_DRM"  /**< Is DRM. 0-not drm, 1-drm*/

/**
 * @brief You can use above define to set the condition of media filter and order keyword.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 */
#define MEDIA_STORAGE_TYPE "MEDIA_STORAGE_TYPE"  /**< Media storage. 0-internal storage, 1-external storage*/

/**
 * @brief You can use above define to set the condition of media filter and order keyword.
 * @since_tizen 2.4
 *
 */
#define MEDIA_EXPOSURE_TIME "MEDIA_EXPOSURE_TIME"	/**< media exposure_time*/

/**
 * @brief You can use above define to set the condition of media filter and order keyword.
 * @since_tizen 2.4
 *
 */
#define MEDIA_FNUMBER "MEDIA_FNUMBER"	/**< media fnumber*/

/**
 * @brief You can use above define to set the condition of media filter and order keyword.
 * @since_tizen 2.4
 *
 */
#define MEDIA_ISO "MEDIA_ISO"	/**< media iso*/

/**
 * @brief You can use above define to set the condition of media filter and order keyword.
 * @since_tizen 2.4
 *
 */
#define MEDIA_MODEL "MEDIA_MODEL"	/**< media model*/

/**
 * @brief You can use above define to set the condition of media filter and order keyword.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 */
#define MEDIA_FILE_NAME_PINYIN "MEDIA_FILE_NAME_PINYIN"  /**< Media file name pinyin */

/**
 * @brief You can use above define to set the condition of media filter and order keyword.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 */
#define MEDIA_TITLE_PINYIN "MEDIA_TITLE_PINYIN"  /**< Media title pinyin */

/**
 * @brief You can use above define to set the condition of media filter and order keyword.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 */
#define MEDIA_ALBUM_PINYIN "MEDIA_ALBUM_PINYIN"  /**< Media album pinyin*/

/**
 * @brief You can use above define to set the condition of media filter and order keyword.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 */
#define MEDIA_ARTIST_PINYIN "MEDIA_ARTIST_PINYIN"  /**< Media artist pinyin*/

/**
 * @brief You can use above define to set the condition of media filter and order keyword.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 */
#define MEDIA_ALBUM_ARTIST_PINYIN "MEDIA_ALBUM_ARTIST_PINYIN"  /**< Media album_artist pinyin*/

/**
 * @brief You can use above define to set the condition of media filter and order keyword.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 */
#define MEDIA_GENRE_PINYIN "MEDIA_GENRE_PINYIN"  /**< Media genre pinyin*/

/**
 * @brief You can use above define to set the condition of media filter and order keyword.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 */
#define MEDIA_COMPOSER_PINYIN "MEDIA_COMPOSER_PINYIN"  /**< Media composer pinyin*/

/**
 * @brief You can use above define to set the condition of media filter and order keyword.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 */
#define MEDIA_COPYRIGHT_PINYIN "MEDIA_COPYRIGHT_PINYIN"  /**< Media copyright pinyin*/

/**
 * @brief You can use above define to set the condition of media filter and order keyword.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 */
#define MEDIA_DESCRIPTION_PINYIN "MEDIA_DESCRIPTION_PINYIN"  /**< Media description pinyin*/

/**
 * @brief You can use above define to set the condition of media filter and order keyword.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 */
#define MEDIA_AUTHOR_PINYIN "MEDIA_AUTHOR_PINYIN"  /**< Media authore pinyin*/

/**
 * @brief You can use above define to set the condition of media filter and order keyword.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 */
#define MEDIA_PROVIDER_PINYIN "MEDIA_PROVIDER_PINYIN"  /**< Media provider pinyin*/

/**
 * @brief You can use above define to set the condition of media filter and order keyword.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 */
#define MEDIA_CONTENT_NAME_PINYIN "MEDIA_CONTENT_NAME_PINYIN"  /**< Media content name pinyin*/

/**
 * @brief You can use above define to set the condition of media filter and order keyword.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 */
#define MEDIA_CATEGORY_PINYIN "MEDIA_CATEGORY_PINYIN"  /**< Media category pinyin*/

/**
 * @brief You can use above define to set the condition of media filter and order keyword.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 */
#define MEDIA_LOCATION_TAG_PINYIN "MEDIA_LOCATION_TAG_PINYIN"  /**< Media location tag pinyin*/

/**
 * @brief You can use above define to set the condition of media filter and order keyword.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 */
#define MEDIA_AGE_RATING_PINYIN "MEDIA_AGE_RATING_PINYIN"  /**< Media age rating pinyin*/

/**
 * @brief You can use above define to set the condition of media filter and order keyword.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 */
#define MEDIA_KEYWORD_PINYIN "MEDIA_KEYWORD_PINYIN"  /**< Media keyword pinyin*/

/**
 * @}
 */


/**
 * @addtogroup CAPI_CONTENT_MEDIA_FOLDER_MODULE
 * @{
 */

 /**
 * @brief You can use above define to set the condition of folder filter and order keyword.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 */
#define FOLDER_ID "FOLDER_ID"  /**< Folder ID */

/**
* @brief You can use above define to set the condition of folder filter and order keyword.
* @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
*/
#define FOLDER_PATH "FOLDER_PATH"  /**< Folder full path */

/**
* @brief You can use above define to set the condition of folder filter and order keyword.
* @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
*/
#define FOLDER_NAME "FOLDER_NAME"  /**< Folder base name */

/**
* @brief You can use above define to set the condition of folder filter and order keyword.
* @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
*/
#define FOLDER_MODIFIED_TIME "FOLDER_MODIFIED_TIME"  /**< Folder modified time */

/**
* @brief You can use above define to set the condition of folder filter and order keyword.
* @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
*/
#define FOLDER_STORAGE_TYPE "FOLDER_STORAGE_TYPE"  /**< Folder storage. 0-internal storage, 1-external storage*/

/**
* @brief You can use above define to set the condition of folder filter and order keyword.
* @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
*/
#define FOLDER_NAME_PINYIN "FOLDER_NAME_PINYIN"  /**< Folder base name pinyin*/

/**
 * @brief You can use above define to set the condition of folder filter and order keyword.
 * @since_tizen 2.4
 */
#define FOLDER_ORDER "FOLDER_ORDER"  /**< Folder order info */

/**
 * @brief You can use above define to set the condition of folder filter and order keyword.
 * @since_tizen 2.4
 */
#define FOLDER_PARENT_FOLDER_ID "FOLDER_PARENT_FOLDER_ID"	/**< parent folder id */

/**
 * @}
 */

/**
 * @addtogroup CAPI_CONTENT_MEDIA_PLAYLIST_MODULE
 * @{
 * @brief You can use above define to set the condition of playlist filter and order keyword.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 */
#define PLAYLIST_NAME "PLAYLIST_NAME"  /**< Playlist name */

/**
 * @brief You can use above define to set the condition of playlist filter and order keyword.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 */
#define PLAYLIST_MEMBER_ORDER "PLAYLIST_MEMBER_ORDER"  /**< Playlist name */

/**
 * @brief You can use above define to set the condition of playlist filter and order keyword.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 */
#define PLAYLIST_MEDIA_COUNT "PLAYLIST_MEDIA_COUNT"  /**< Media count in playlist view */

/**
 * @}
 */

/**
 * @addtogroup CAPI_CONTENT_MEDIA_TAG_MODULE
 * @{
 * @brief You can use above define to set the condition of tag filter and order keyword.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 */
#define TAG_NAME "TAG_NAME"  /**< Tag name */

/**
 * @brief You can use above define to set the condition of tag filter and order keyword.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 */
#define TAG_MEDIA_COUNT "TAG_MEDIA_COUNT"  /**< Media count in tag view */

/**
 * @}
 */

/**
 * @addtogroup CAPI_CONTENT_MEDIA_BOOKMARK_MODULE
 * @{
 * @brief You can use above define to set the condition of bookmark filter and order keyword.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 */
#define BOOKMARK_MARKED_TIME "BOOKMARK_MARKED_TIME"  /**< Bookmark marked time */

/**
 * @}
 */

/**
 * @addtogroup CAPI_CONTENT_MEDIA_STORAGE_MODULE
 * @{
 * @brief You can use above define to set the condition of storage filter and order keyword.
 * @since_tizen 2.4
 */
#define MEDIA_STORAGE_ID					"STORAGE_ID"  /**< Storage ID */

/**
 * @brief You can use above define to set the condition of storage filter and order keyword.
 * @since_tizen 2.4
 */
#define MEDIA_STORAGE_PATH				"STORAGE_PATH"  /**< Storage path */

/**
 * @}
 */


#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif /*__TIZEN_MEDIA_CONTENT_TYPE_H__*/
