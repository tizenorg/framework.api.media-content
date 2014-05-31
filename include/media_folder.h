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


#ifndef __TIZEN_MEDIA_FOLDER_H__
#define __TIZEN_MEDIA_FOLDER_H__


#include <media_content_type.h>


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 * @addtogroup CAPI_CONTENT_MEDIA_FOLDER_MODULE
 * @{
 *
 * @file media-folder.h
 * @brief This file contains API realted to all operations with media folder in db.  \n
 *        These functions include getting number of folder and media files filtered from db,  \n
 *        iterating through media files and folders filtered in the given folder from db;  \n
 *        clonning and destroying media folder, getting its name, ID, absolute path and date \n
 *        and updating the media folder to the media database.
 */

/**
 * @brief Gets the number of folder for the passed @a filter from the media database.
 *
 * @param[in] filter The handle to filter. \n To allow searching over different content types, you should use @a filter_h.
 * @param[out] folder_count The count of media folder
 * @return 0 on success, otherwise a negative error value.
 * @retval #MEDIA_CONTENT_ERROR_NONE Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_DB_FAILED DB operation failed
 * @pre This function requires opened connection to content service by media_content_connect().
 * @see media_content_connect()
 *
 */
int media_folder_get_folder_count_from_db(filter_h filter, int *folder_count);

/**
 * @brief Iterates through available media folders with optional @a filter from the media database.
 * @details This function gets media folder handles meeting the given 
 * @a filter. The @a callback function will be invoked for every retrieved
 * folder. If NULL is passed to the @a filter, no filtering is applied.
 *
 * @param[in] filter The handle to media folder filter
 * @param[in] callback The callback function to invoke
 * @param[in] user_data The user data to be passed to the callback function
 * @return return 0 on success, otherwise a negative error value.
 * @retval #MEDIA_CONTENT_ERROR_NONE Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_OUT_OF_MEMORY Out of memory
 * @pre This function requires opened connection to content service by media_content_connect().
 * @pre A filter handle has to be created by calling media_folder_filter_create()
 * @post This function invokes media_folder_cb()
 * @see media_content_connect()
 * @see #media_folder_cb
 * @see media_filter_create()
 */
int media_folder_foreach_folder_from_db(filter_h filter, media_folder_cb callback, void *user_data);

/**
 * @brief Gets the number of media files for the passed @a filter in the given @a folder from the media database.
 *
 * @param[in] folder_id The ID of media folder
 * @param[in] filter The filter of media content
 * @param[out] media_count The count of media folder items
 * @return 0 on success, otherwise a negative error value.
 * @retval #MEDIA_CONTENT_ERROR_NONE Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_DB_FAILED DB operation failed
 * @pre This function requires opened connection to content service by media_content_connect().
 * @see media_content_connect()
 *
 */
int media_folder_get_media_count_from_db(const char *folder_id, filter_h filter, int *media_count);

/**
 * @brief Iterates through the media files with optional @a filter in the given @a folder from the media database.
 * @details This function gets all media files associated with the given folder and
 * meeting desired filter option and calls registered callback function for
 * every retrieved media item. If NULL is passed to the @a filter, no filtering is applied.
 *
 * @param[in] folder_id The ID of media folder
 * @param[in] filter The handle to media info filter
 * @param[in] callback The callback function to invoke
 * @param[in] user_data The user data to be passed to the callback function
 * @return 0 on success, otherwise a negative error value.
 * @retval #MEDIA_CONTENT_ERROR_NONE Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_OUT_OF_MEMORY Out of memory
 * @pre This function requires opened connection to content service by media_content_connect().
 * @post This function invokes media_info_cb()
 * @see #media_info_cb
 * @see media_content_connect()
 * @see media_folder_filter_create()
 */
int media_folder_foreach_media_from_db(const char *folder_id, filter_h filter, media_info_cb callback, void *user_data);

/**
 * @brief Clones the media folder.
 * @details This function copies the media folder handle from a source to
 * destination. There is no media_folder_create() function. The media_folder_h is created internally and available through 
 * media folder foreach function such as media_folder_foreach_folder_from_db(). To use this handle outside of these foreach functions, 
 * use this function.
 * @remark The destination handle must be released with media_folder_destroy() by you.
 * @param[out] dst A destination handle to media folder
 * @param[in] src The source handle to media folder
 * @return 0 on success, otherwise a negative error value.
 * @retval #MEDIA_CONTENT_ERROR_NONE Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @see media_folder_destroy()
 * @see media_folder_foreach_folder_from_db()
 */
int media_folder_clone(media_folder_h *dst, media_folder_h src);

/**
 * @brief Destroys the media folder.
 * @details The function frees all resources related to the folder handle. This handle
 * no longer can be used to perform any operation. A new handle has to
 * be created before the next use.
 *
 * @param[in] folder The handle to media folder
 * @return 0 on success, otherwise a negative error value.
 * @retval #MEDIA_CONTENT_ERROR_NONE Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @pre A copy of the media folder handle created by calling media_folder_clone()
 * @see media_folder_clone()
 */
int media_folder_destroy(media_folder_h folder);

/**
 * @brief Gets media folder's ID.
 *
 * @remarks @a folder_id must be released with free() by you.
 *
 * @param [in] folder The handle to media folder
 * @param [out] folder_id The ID of media folder
 * @return 0 on success, otherwise a negative error value.
 * @retval #MEDIA_CONTENT_ERROR_NONE Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 */
int media_folder_get_folder_id(media_folder_h folder, char **folder_id);

/**
 * @brief Gets the absolute path to the folder.
 *
 * @remarks @a path must be released with free() by you.
 *
 * @param[in] folder The handle to media folder
 * @param[out] path The path of the media folder
 * @return 0 on success, otherwise a negative error value.
 * @retval #MEDIA_CONTENT_ERROR_NONE Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_OUT_OF_MEMORY Out of memory
 *
 */
int media_folder_get_path(media_folder_h folder, char **path);

/**
 * @brief Gets the folder name.
 *
 * @remarks @a folder_name must be released with free() by you.
 *
 * @param[in] folder The handle to media folder
 * @param[out] folder_name The name of the media folder
 * @return 0 on success, otherwise a negative error value.
 * @retval #MEDIA_CONTENT_ERROR_NONE Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_OUT_OF_MEMORY Out of memory
 *
 */
int media_folder_get_name(media_folder_h folder, char **folder_name);

/**
 * @brief Gets the modified date of the folder.
 *
 * @param[in] folder The handle to media folder
 * @param[out] date The modified date of folder
 * @return 0 on success, otherwise a negative error value.
 * @retval #MEDIA_CONTENT_ERROR_NONE Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 *
 */
int media_folder_get_modified_time(media_folder_h folder, time_t *date);

/**
 * @brief Gets the folder storage type.
 *
 * @param[in] folder The handle to media folder
 * @param[out] storage_type The storage type of the media folder
 * @return 0 on success, otherwise a negative error value.
 * @retval #MEDIA_CONTENT_ERROR_NONE Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 *
 */
int media_folder_get_storage_type(media_folder_h folder, media_content_storage_e *storage_type);

/**
 * @brief Gets the media folder from the media database.
 *
 * @details This function creates a new media folder handle from the media database by the given folder_id.
 * media folder will be created, which is filled with folder information.
 *
 * @remarks @a folder must be released with media_folder_destroy() by you.
 *
 * @param[in] folder_id The ID of media folder
 * @param[out] folder The media folder handle associated with the folder ID
 * @return 0 on success, otherwise a negative error value.
 * @retval #MEDIA_CONTENT_ERROR_NONE Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @pre This function requires opened connection to content service by media_content_connect().
 * @see media_content_connect()
 * @see media_folder_destroy()
 *
 */
int media_folder_get_folder_from_db(const char *folder_id, media_folder_h *folder);

/**
 * @brief Updates the media folder to the media database.
 *
 * @details The function updates the given media folder in the media database. The function should be called after any change in folder attributes, to be updated to the media 
 * database. For example, after using media_folder_set_name() for setting the name of the folder, media_folder_update_to_db() function should be called so as to update 
 * the given folder attibutes in the media database.
 *
 * @param[in] folder The handle to media folder
 * @return 0 on success, otherwise a negative error value.
 * @retval #MEDIA_CONTENT_ERROR_NONE Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @pre This function requires opened connection to content service by media_content_connect().
 * @see media_content_connect()
 * @see media_folder_destroy()
 * @see media_folder_set_name()
 *
 */
int media_folder_update_to_db(media_folder_h folder);

/**
 * @brief Sets the folder name.
 *
 * @param[in] folder The handle to media folder
 * @param[in] name The name of the media folder
 * @return 0 on success, otherwise a negative error value.
 * @retval #MEDIA_CONTENT_ERROR_NONE Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_OUT_OF_MEMORY Out of memory
 * @post media_folder_update_to_db()
 *
 */
int media_folder_set_name(media_folder_h folder, const char *name);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __TIZEN_MEDIA_FOLDER_H__ */
