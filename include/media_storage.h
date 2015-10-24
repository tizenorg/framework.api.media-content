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


#ifndef __TIZEN_MEDIA_STORAGE_H__
#define __TIZEN_MEDIA_STORAGE_H__


#include <media_content_type.h>


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 * @file media_storage.h
 * @brief This file contains API on main functional operations with storage that are related to media resources in the media database. \n
 *        Operations include: getting number of storages, cloning and destroying storage, getting storage`s ID, name, path and type.
 */

/**
 * @addtogroup CAPI_CONTENT_MEDIA_STORAGE_MODULE
 * @{
 */

/**
* @brief Gets media storage from database.
* @since_tizen 2.4
*
* @param[in] storage_id The storage id to get media storage info
* @param[out] storage The media storage handle
*
* @return @c 0 on success,
*           otherwise a negative error value
*
* @retval #MEDIA_CONTENT_ERROR_NONE 			 Successful
* @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
* @retval #MEDIA_CONTENT_ERROR_OUT_OF_MEMORY	 Out of memory
* @retval #MEDIA_CONTENT_ERROR_DB_FAILED		 DB Operation failed
* @retval #MEDIA_CONTENT_ERROR_DB_BUSY			 DB Operation busy
* @retval #MEDIA_CONTENT_ERROR_PERMISSION_DENIED Permission denied

*
* @pre This function requires opened connection to content service by media_content_connect().
*
* @see media_content_connect()
*/
int media_storage_get_storage_info_from_db(const char *storage_id, media_storage_h *storage);

/**
* @brief Gets the count of media storage for the passed @a filter from the media database.
* @since_tizen 2.4
*
* @param[in] filter The handle to filter
* @param[out] storage_count The count of storage
*
* @return @c 0 on success,
*           otherwise a negative error value
*
* @retval #MEDIA_CONTENT_ERROR_NONE 			 Successful
* @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
* @retval #MEDIA_CONTENT_ERROR_INVALID_OPERATION Invalid operation
* @retval #MEDIA_CONTENT_ERROR_DB_FAILED		 DB Operation failed
* @retval #MEDIA_CONTENT_ERROR_DB_BUSY			 DB Operation busy
* @retval #MEDIA_CONTENT_ERROR_PERMISSION_DENIED Permission denied
*
* @pre This function requires opened connection to content service by media_content_connect().
*
* @see media_content_connect()
*/
int media_storage_get_storage_count_from_db(filter_h filter, int *storage_count);

/**
 * @brief Iterates through media storage from the media database.
 * @details This function gets all media storage handles meeting the given @a filter.
 *          The @a callback function will be invoked for every retrieved media storage.
 *          If @c NULL is passed to the @a filter, then no filtering is applied.
 *
 * @since_tizen 2.4
 *
 * @param[in] filter    The media storage handle filter
 * @param[in] callback  The callback function to be invoked
 * @param[in] user_data The user data to be passed to the callback function
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_OUT_OF_MEMORY     Out of memory
 * @retval #MEDIA_CONTENT_ERROR_DB_FAILED         DB Operation failed
 * @retval #MEDIA_CONTENT_ERROR_DB_BUSY           DB Operation busy
 * @retval #MEDIA_CONTENT_ERROR_PERMISSION_DENIED Permission denied
 *
 * @pre  This function requires opened connection to content service by media_content_connect().
 * @post This function invokes media_storage_destroy().
 *
 * @see media_content_connect()
 * @see media_storage_destroy()
 */
int media_storage_foreach_storage_from_db(filter_h filter, media_storage_cb callback, void *user_data);

/**
 * @brief Gets the count of media files for the passed @a filter in the given @a storage from the media database.
 * @since_tizen 2.4
 *
 * @param[in] storage_id    The ID of the media storage
 * @param[in] filter       The filter of the media content
 * @param[out] media_count The count of media storage items
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_DB_FAILED         DB Operation failed
 * @retval #MEDIA_CONTENT_ERROR_DB_BUSY           DB Operation busy
 * @retval #MEDIA_CONTENT_ERROR_PERMISSION_DENIED Permission denied
 *
 * @pre This function requires opened connection to content service by media_content_connect().
 *
 * @see media_content_connect()
 */
int media_storage_get_media_count_from_db(const char *storage_id, filter_h filter, int *media_count);

/**
 * @brief Iterates through the media files with an optional @a filter in the given @a storage from the media database.
 * @details This function gets all media files associated with the given storage and
 *          meeting desired filter option and calls registered callback function for
 *          every retrieved media item. If @c NULL is passed to the @a filter, no filtering is applied.
 *
 * @since_tizen 2.4
 *
 * @remarks   Do not call updating DB function like media_info_update_to_db(), media_info_refresh_metadata_to_db(), audio_meta_update_to_db(), image_meta_update_to_db() and video_meta_update_to_db()  in your callback function,
 *                    your callback function is invoked as inline function. \n
 *                    So, your callback function is in read state in SQLite. When you are in read state, sometimes you do not update DB. \n
 *                    We do not recommend you call updating DB function in callback of foreach function.
 *
 * @param[in] storage_id The ID of the media storage
 * @param[in] filter    The handle to the media info filter
 * @param[in] callback  The callback function to be invoked
 * @param[in] user_data The user data to be passed to the callback function
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_OUT_OF_MEMORY     Out of memory
 * @retval #MEDIA_CONTENT_ERROR_DB_FAILED         DB Operation failed
 * @retval #MEDIA_CONTENT_ERROR_DB_BUSY           DB Operation busy
 * @retval #MEDIA_CONTENT_ERROR_PERMISSION_DENIED Permission denied
 *
 * @pre  This function requires opened connection to content service by media_content_connect().
 * @post This function invokes media_info_cb().
 *
 * @see #media_info_cb
 * @see media_content_connect()
 * @see media_filter_create()
 */
int media_storage_foreach_media_from_db(const char *storage_id, filter_h filter, media_info_cb callback, void *user_data);

/**
 * @brief Destroys media storage handle.
 * @details The function frees all resources related to the media storage handle. This handle
 *          can no longer be used to perform any operation. New media storage handle has to
 *          be created before the next usage.
 *
 * @since_tizen 2.4
 *
 * @param[in]  storage  The media storage handle
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre Get copy of media_storage_h handle by calling media_storage_clone().
 *
 * @see media_storage_clone()
 */
int media_storage_destroy(media_storage_h storage);

/**
 * @brief Clones the media storage handle.
 *
 * @details This function copies the media storage handle from a source to the destination.
 *          There is no media_storage_create() function. The media_storage_h is created internally and
 *          available through media storage foreach function such as media_storage_foreach_storage_from_db().
 *          To use this handle outside of these foreach functions, use this function.
 *
 * @since_tizen 2.4
 *
 * @remarks You must release the destination handle using media_storage_destroy().
 *
 * @param[out] dst  The destination handle to the media storage
 * @param[in]  src  The source handle to media storage
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_OUT_OF_MEMORY     Out of memory
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @see media_storage_destroy()
 * @see media_storage_foreach_storage_from_db()
 * @see media_storage_get_storage_info_from_db()
 */
int media_storage_clone(media_storage_h *dst, media_storage_h src);

/**
 * @brief Gets the storage id of media storage.
 * @since_tizen 2.4
 *
 * @remarks You must release @a storage_id using free().
 *
 * @param[in]  storage The media storage handle
 * @param[out] storage_id  The storage id of the media storage
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_OUT_OF_MEMORY     Out of memory
 */
int media_storage_get_id(media_storage_h storage, char **storage_id);

/**
 * @brief Gets the storage name of media storage.
 * @since_tizen 2.4
 *
 * @remarks You must release @a storage_name using free().
 *
 * @param[in]  storage The media storage handle
 * @param[out] storage_name  The storage name of the media storage
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_OUT_OF_MEMORY     Out of memory
 */
int media_storage_get_name(media_storage_h storage, char **storage_name);

/**
 * @brief Gets the storage path of media storage.
 * @since_tizen 2.4
 *
 * @remarks You must release @a storage_path using free().
 *
 * @param[in]  storage The media storage handle
 * @param[out] storage_path  The storage path of the media storage
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_OUT_OF_MEMORY     Out of memory
 */
int media_storage_get_path(media_storage_h storage, char **storage_path);

/**
 * @brief Gets the storage type of media storage.
 * @since_tizen 2.4
 *
 * @param[in]  storage The media storage handle
 * @param[out] storage_type  The storage type of the media storage
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 */
int media_storage_get_type(media_storage_h storage, media_content_storage_e *storage_type);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __TIZEN_MEDIA_STORAGE_H__ */
