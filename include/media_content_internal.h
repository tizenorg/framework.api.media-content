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



#ifndef __TIZEN_MEDIA_CONTENT_INTERNAL_H__
#define __TIZEN_MEDIA_CONTENT_INTERNAL_H__

#include <media_content_type.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 * @internal
 * @file media_content_internal.h
 * @brief This file contains API on main functional operations with storage that are related to media resources in the media database. \n
 *        Operations include: inserting a new storage in media to the media database, removing storage from database, \n
 */

/**
 * @ingroup CAPI_MEDIA_CONTENT_MODULE
 * @brief Enumeration for a media storage scan status.
 * @since_tizen 2.4
 */
typedef enum{
	MEDIA_STORAGE_SCAN_NONE	= 0,				/**< Media Scanner not detect storage yet*/
	MEDIA_STORAGE_SCAN_PREPARE,				/**< Media Scanner detect storage but not scanning yet*/
	MEDIA_STORAGE_SCAN_PROCESSING,			/**< Media Scanner Start Scanning storage*/
	MEDIA_STORAGE_SCAN_STOP,					/**< Media Scanner Stop Scanning storage. Not scanning items can be exist*/
	MEDIA_STORAGE_SCAN_DONE,					/**< Scanning Done but need to extract metadata*/
	MEDIA_STORAGE_SCAN_META_PROCESSING,		/**< Scanning Done and start to extract metadata*/
	MEDIA_STORAGE_SCAN_META_STOP,			/**< Stop extract metadata*/
	MEDIA_STORAGE_SCAN_COMPLETE	,			/**< Complete scanning*/
}media_storage_scan_status_e;

/**
* @internal
* @brief Insert media storage to database.
* @since_tizen 2.4
*
* @remarks You must release @a storage using media_storage_destroy().
*
* @privlevel platform
* @privilege %http://tizen.org/privilege/content.write
*
* @param[in] storage_name The storage name to insert to database
* @param[in] storage_path The storage path to insert to database
* @param[in] storage_account The storage account to insert to database
* @param[in] storage_type The storage type to insert to database
* @param[out] storage The media storage handle
*
* @return @c 0 on success,
*           otherwise a negative error value
*
* @retval #MEDIA_CONTENT_ERROR_NONE Successful
* @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
* @retval #MEDIA_CONTENT_ERROR_DB_FAILED		 DB operation failed
* @retval #MEDIA_CONTENT_ERROR_PERMISSION_DENIED Permission denied
*
* @see media_storage_delete_from_db()
*/
int media_storage_insert_to_db(const char *storage_name, const char *storage_path, const char *storage_account, media_content_storage_e storage_type, media_storage_h *storage);

/**
* @internal
* @brief Deletes the database for media storage.
* @since_tizen 2.4
*
* @privlevel platform
* @privilege %http://tizen.org/privilege/content.write
*
* @param[in] storage_id The storage ID to delete from database
*
* @return @c 0 on success,
*           otherwise a negative error value
*
* @retval #MEDIA_CONTENT_ERROR_NONE Successful
* @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
* @retval #MEDIA_CONTENT_ERROR_DB_FAILED		 DB operation failed
* @retval #MEDIA_CONTENT_ERROR_PERMISSION_DENIED Permission denied
*
* @see media_storage_insert_to_db()
*/
int media_storage_delete_from_db(const char *storage_id);

/**
 * @brief Gets the storage account of media storage.
 * @since_tizen 2.4
 *
 * @remarks You must release @a storage_account using free().
 *
 * @param[in]  storage The media storage handle
 * @param[out] storage_account  The storage account of the media storage
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_OUT_OF_MEMORY     Out of memory
 */
int media_storage_get_storage_account(media_storage_h storage, char **storage_account);

/**
* @internal
* @brief Creates the media info handle.
* @since_tizen 2.4
*
* @remarks You must release @a media using media_info_destroy().
*
* @param[out] media The media info handle
*
* @return @c 0 on success,
*           otherwise a negative error value
*
* @retval #MEDIA_CONTENT_ERROR_NONE Successful
* @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
* @retval #MEDIA_CONTENT_ERROR_DB_FAILED		 DB operation failed
* @retval #MEDIA_CONTENT_ERROR_PERMISSION_DENIED Permission denied
*
* @see media_info_destroy()
*/
int media_info_create_handle(media_info_h *media);

/**
* @internal
* @brief Inserts media info to database with media info data.
* @since_tizen 2.4
*
* @remarks You must release @a info using media_info_destroy().
*
* @privlevel platform
* @privilege %http://tizen.org/privilege/content.write
*
* @param[in] media The media info handle
* @param[out] info The media info handle to get data from database
*
* @return @c 0 on success,
*           otherwise a negative error value
*
* @retval #MEDIA_CONTENT_ERROR_NONE Successful
* @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
* @retval #MEDIA_CONTENT_ERROR_DB_FAILED		 DB operation failed
* @retval #MEDIA_CONTENT_ERROR_PERMISSION_DENIED Permission denied
*/
int media_info_insert_to_db_with_data(media_info_h media, media_info_h *info);

/**
* @internal
* @brief Sets the path of media info handle.
* @since_tizen 2.4
*
* @param[in] media The media info handle
* @param[in] path The path of the media info
*
* @return @c 0 on success,
*           otherwise a negative error value
*
* @retval #MEDIA_CONTENT_ERROR_NONE Successful
* @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
* @retval #MEDIA_CONTENT_ERROR_DB_FAILED		 DB operation failed
* @retval #MEDIA_CONTENT_ERROR_PERMISSION_DENIED Permission denied
*
* @post media_info_insert_to_db_with_data()
*/
int media_info_set_path(media_info_h media, const char *path);

/**
* @internal
* @brief Sets the MIME type of media info handle.
* @since_tizen 2.4
*
* @param[in] media The media info handle
* @param[in] mime_type The MIME type of the media info
*
* @return @c 0 on success,
*           otherwise a negative error value
*
* @retval #MEDIA_CONTENT_ERROR_NONE Successful
* @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
* @retval #MEDIA_CONTENT_ERROR_DB_FAILED		 DB operation failed
* @retval #MEDIA_CONTENT_ERROR_PERMISSION_DENIED Permission denied
*
* @post media_info_insert_to_db_with_data()
*/
int media_info_set_mime_type(media_info_h media, const char *mime_type);

/**
* @internal
* @brief Sets the title of media info handle.
* @since_tizen 2.4
*
* @param[in] media The media info handle
* @param[in] title The title of media info handle
*
* @return @c 0 on success,
*           otherwise a negative error value
*
* @retval #MEDIA_CONTENT_ERROR_NONE Successful
* @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
* @retval #MEDIA_CONTENT_ERROR_DB_FAILED		 DB operation failed
* @retval #MEDIA_CONTENT_ERROR_PERMISSION_DENIED Permission denied
*
* @post media_info_insert_to_db_with_data()
*/
int media_info_set_title(media_info_h media, const char *title);

/**
* @internal
* @brief Sets the album of media info handle.
* @since_tizen 2.4
*
* @param[in] media The media info handle
* @param[in] album The album of media info handle
*
* @return @c 0 on success,
*           otherwise a negative error value
*
* @retval #MEDIA_CONTENT_ERROR_NONE Successful
* @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
* @retval #MEDIA_CONTENT_ERROR_DB_FAILED		 DB operation failed
* @retval #MEDIA_CONTENT_ERROR_PERMISSION_DENIED Permission denied
*
* @post media_info_insert_to_db_with_data()
*/
int media_info_set_album(media_info_h media, const char *album);

/**
* @internal
* @brief Sets the artist of media info handle.
* @since_tizen 2.4
*
* @param[in] media The media info handle
* @param[in] artist The artist of media info handle
*
* @return @c 0 on success,
*           otherwise a negative error value
*
* @retval #MEDIA_CONTENT_ERROR_NONE Successful
* @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
* @retval #MEDIA_CONTENT_ERROR_DB_FAILED		 DB operation failed
* @retval #MEDIA_CONTENT_ERROR_PERMISSION_DENIED Permission denied
*
* @post media_info_insert_to_db_with_data()
*/
int media_info_set_artist(media_info_h media, const char *artist);

/**
* @internal
* @brief Sets the genre of media info handle.
* @since_tizen 2.4
*
* @param[in] media The media info handle
* @param[in] genre The genre of media info handle
*
* @return @c 0 on success,
*           otherwise a negative error value
*
* @retval #MEDIA_CONTENT_ERROR_NONE Successful
* @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
* @retval #MEDIA_CONTENT_ERROR_DB_FAILED		 DB operation failed
* @retval #MEDIA_CONTENT_ERROR_PERMISSION_DENIED Permission denied
*
* @post media_info_insert_to_db_with_data()
*/
int media_info_set_genre(media_info_h media, const char *genre);

/**
* @internal
* @brief Sets the recorded date of media info handle.
* @since_tizen 2.4
*
* @param[in] media The media info handle
* @param[in] recorded_date The recorded date of media info handle
*
* @return @c 0 on success,
*           otherwise a negative error value
*
* @retval #MEDIA_CONTENT_ERROR_NONE Successful
* @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
* @retval #MEDIA_CONTENT_ERROR_DB_FAILED		 DB operation failed
* @retval #MEDIA_CONTENT_ERROR_PERMISSION_DENIED Permission denied
*
* @post media_info_insert_to_db_with_data()
*/
int media_info_set_recorded_date(media_info_h media, const char *recorded_date);

/**
* @internal
* @brief Sets the thumbnail path of media info handle.
* @since_tizen 2.4
*
* @param[in] media The media info handle
* @param[in] thumbnail_path The thumbnail path of media info handle
*
* @return @c 0 on success,
*           otherwise a negative error value
*
* @retval #MEDIA_CONTENT_ERROR_NONE Successful
* @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
* @retval #MEDIA_CONTENT_ERROR_DB_FAILED		 DB operation failed
* @retval #MEDIA_CONTENT_ERROR_PERMISSION_DENIED Permission denied
*
* @post media_info_insert_to_db_with_data()
*/
int media_info_set_thumbnail_path(media_info_h media, const char *thumbnail_path);

/**
* @internal
* @brief Sets the size of media info handle.
* @since_tizen 2.4
*
* @param[in] media The media info handle
* @param[in] size The size of media info handle
*
* @return @c 0 on success,
*           otherwise a negative error value
*
* @retval #MEDIA_CONTENT_ERROR_NONE Successful
* @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
* @retval #MEDIA_CONTENT_ERROR_DB_FAILED		 DB operation failed
* @retval #MEDIA_CONTENT_ERROR_PERMISSION_DENIED Permission denied
*
* @post media_info_insert_to_db_with_data()
*/
int media_info_set_size(media_info_h media, unsigned long long size);

/**
* @internal
* @brief Sets the modified time of media info handle.
* @since_tizen 2.4
*
* @param[in] media The media info handle
* @param[in] modified_time The modified time of media info handle
*
* @return @c 0 on success,
*           otherwise a negative error value
*
* @retval #MEDIA_CONTENT_ERROR_NONE Successful
* @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
* @retval #MEDIA_CONTENT_ERROR_DB_FAILED		 DB operation failed
* @retval #MEDIA_CONTENT_ERROR_PERMISSION_DENIED Permission denied
*
* @post media_info_insert_to_db_with_data()
*/
int media_info_set_modified_time(media_info_h media, time_t modified_time);

/**
* @internal
* @brief Sets the media type of media info handle.
* @since_tizen 2.4
*
* @param[in] media The media info handle
* @param[in] type The media type of media info handle
*
* @return @c 0 on success,
*           otherwise a negative error value
*
* @retval #MEDIA_CONTENT_ERROR_NONE Successful
* @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
* @retval #MEDIA_CONTENT_ERROR_DB_FAILED		 DB operation failed
* @retval #MEDIA_CONTENT_ERROR_PERMISSION_DENIED Permission denied
*
* @post media_info_insert_to_db_with_data()
*/
int media_info_set_media_type(media_info_h media, media_content_type_e type);

/**
* @internal
* @brief Sets the duration of media info handle.
* @since_tizen 2.4
*
* @param[in] media The media info handle
* @param[in] duration The duration of media info handle
*
* @return @c 0 on success,
*           otherwise a negative error value
*
* @retval #MEDIA_CONTENT_ERROR_NONE Successful
* @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
* @retval #MEDIA_CONTENT_ERROR_DB_FAILED		 DB operation failed
* @retval #MEDIA_CONTENT_ERROR_PERMISSION_DENIED Permission denied
*
* @post media_info_insert_to_db_with_data()
*/
int media_info_set_duration(media_info_h media, int duration);

/**
* @internal
* @brief Sets the width of media info handle.
* @since_tizen 2.4
*
* @param[in] media The media info handle
* @param[in] width The width of media info handle
*
* @return @c 0 on success,
*           otherwise a negative error value
*
* @retval #MEDIA_CONTENT_ERROR_NONE Successful
* @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
* @retval #MEDIA_CONTENT_ERROR_DB_FAILED		 DB operation failed
* @retval #MEDIA_CONTENT_ERROR_PERMISSION_DENIED Permission denied
*
* @post media_info_insert_to_db_with_data()
*/
int media_info_set_width(media_info_h media, int width);

/**
* @internal
* @brief Sets the height of media info handle.
* @since_tizen 2.4
*
* @param[in] media The media info handle
* @param[in] height The height of media info handle
*
* @return @c 0 on success,
*           otherwise a negative error value
*
* @retval #MEDIA_CONTENT_ERROR_NONE Successful
* @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
* @retval #MEDIA_CONTENT_ERROR_DB_FAILED		 DB operation failed
* @retval #MEDIA_CONTENT_ERROR_PERMISSION_DENIED Permission denied
*
* @post media_info_insert_to_db_with_data()
*/
int media_info_set_height(media_info_h media, int height);

/**
* @internal
* @brief Sets the storage type of media info handle.
* @since_tizen 2.4
*
* @param[in] media The media info handle
* @param[in] storage_type The storage type of media info handle
*
* @return @c 0 on success,
*           otherwise a negative error value
*
* @retval #MEDIA_CONTENT_ERROR_NONE Successful
* @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
* @retval #MEDIA_CONTENT_ERROR_DB_FAILED		 DB operation failed
* @retval #MEDIA_CONTENT_ERROR_PERMISSION_DENIED Permission denied
*
* @post media_info_insert_to_db_with_data()
*/
int media_info_set_storage_type(media_info_h media, media_content_storage_e storage_type);

/**
* @internal
* @brief Sets the storage type of media info handle.
* @since_tizen 2.4
*
* @param[in] media The media info handle
* @param[in] storage_id The storage id of media info handle
*
* @return @c 0 on success,
*           otherwise a negative error value
*
* @retval #MEDIA_CONTENT_ERROR_NONE Successful
* @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
* @retval #MEDIA_CONTENT_ERROR_DB_FAILED		 DB operation failed
* @retval #MEDIA_CONTENT_ERROR_PERMISSION_DENIED Permission denied
*
* @post media_info_insert_to_db_with_data()
*/
int media_info_set_storage_id(media_info_h media, const char *storage_id);

/**
 * @brief Gets the storage scan status of media storage.
 * @since_tizen 2.4
 *
 * @param[in]  storage The media storage handle
 * @param[out] scan_status  The storage type of the media storage
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 */
int media_storage_get_scan_status(const char *storage_uuid, media_storage_scan_status_e *scan_status);

/* Handle for dbus notification to use multiple callback */
typedef void *media_content_noti_h;

/**
 * @brief Subscribes notifications of the media DB change.
 * @details This function subscribes notifications of the media DB change which are published by the media server or other apps.
 *          media_content_db_update_cb() function will be called when notification of the media DB change is subscribed.
 *
 * @since_tizen 2.4
 *
 * @param[in] callback  The callback to be invoked when the scanning is finished
 * @param[in] user_data The user data to be passed to the callback function
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_INVALID_OPERATION Invalid operation
 * @retval #MEDIA_CONTENT_ERROR_OUT_OF_MEMORY     Out of memory
 * @retval #MEDIA_CONTENT_ERROR_PERMISSION_DENIED Permission denied
 *
 * @see media_content_db_update_cb()
 * @see media_content_unset_db_updated_cb_v2()
 */
int media_content_set_db_updated_cb_v2(media_content_noti_h *noti_handle, media_content_db_update_cb callback, void *user_data);


/**
 * @brief Unsubscribes notifications of the media DB change.
 * @details This function unsubscribes notifications of the media DB change which are published by the media server or other apps.
 *
 * @since_tizen 2.4
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE Successful
 * @retval #MEDIA_CONTENT_ERROR_PERMISSION_DENIED Permission denied
 *
 * @pre media_content_set_db_updated_cb_v2()
 *
 * @see media_content_set_db_updated_cb_v2()
 */
int media_content_unset_db_updated_cb_v2(media_content_noti_h noti_handle);


#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /*__TIZEN_MEDIA_CONTENT_INTERNAL_H__*/
