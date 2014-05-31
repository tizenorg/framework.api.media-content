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

#ifndef __TIZEN_MEDIA_GROUP_H__
#define __TIZEN_MEDIA_GROUP_H__


#include <media_content_type.h>


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 * @addtogroup CAPI_CONTENT_MEDIA_ALBUM_MODULE
 * @{
 *
 * @file media_group.h
 * @brief This file contains API related to handling different operations with album and other media data groups in db.  \n
 *        The following APIs are capable to get number of albums, media info in the given album from db, \n
 *        to clone, destroy and get all albums and media files associated with the given media album from db,  \n
 *        to get name, ID, artist, album art path from album; to get number of groups and their names,   \n
 *        to get the number of media files and their content associated with the given group from db.
 */

/**
 * @brief Gets the number of album for the passed @a filter from the media database.
 *
 * @param[in] filter The handle to media filter.
 * @param[out] album_count The count of media album
 * @return 0 on success, otherwise a negative error value.
 * @retval #MEDIA_CONTENT_ERROR_NONE Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_DB_FAILED DB operation failed
 * @pre This function requires opened connection to content service by media_content_connect().
 * @see media_content_connect()
 *
 */
int media_album_get_album_count_from_db(filter_h filter, int *album_count);

/**
 * @brief Iterates through the media album with optional @a filter from the media database.
 * @details This function gets all media album handles meeting the given filter.
 * The callback function will be invoked for every retrieved media album.
 * If NULL is passed to the filter, no filtering is applied.
 *
 * @param [in] filter The handle to media filter
 * @param [in] callback The callback function to invoke
 * @param [in] user_data The user data to be passed to the callback function
 * @return 0 on success, otherwise a negative error value.
 * @retval #MEDIA_CONTENT_ERROR_NONE Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_OUT_OF_MEMORY Out of memory
 * @pre This function requires opened connection to content service by media_content_connect().
 * @post This function invokes media_album_cb().
 * @see #media_album_cb
 * @see media_content_connect()
 * @see media_filter_create()
 *
 */
int media_album_foreach_album_from_db(filter_h filter, media_album_cb callback, void *user_data);

/**
 * @brief Gets number of media info for the given album present in the media database.
 *
 * @param [in] album_id The ID of media album
 * @param [in] filter The handle to media filter
 * @param [out] media_count A count of album
 * @return 0 on success, otherwise a negative error value.
 * @retval #MEDIA_CONTENT_ERROR_NONE Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @pre This function requires opened connection to content service by media_content_connect().
 * @see media_content_connect()
 */
int media_album_get_media_count_from_db (int album_id, filter_h filter, int *media_count);

/**
 * @brief Iterates through the media files with optional @a filter in the given @a media @a album from the media database.
 * @details This function gets all media files associated with the given media album and
 * meeting desired filter option and calls registered callback function for
 * every retrieved media info. If NULL is passed to the @a filter, no filtering is applied.
 *
 * @param [in] album_id The ID of media album
 * @param [in] filter The handle to media filter
 * @param [in] callback The callback function to invoke
 * @param [in] user_data The user data to be passed to the callback function
 * @return 0 on success, otherwise a negative error value.
 * @retval #MEDIA_CONTENT_ERROR_NONE Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @pre This function requires opened connection to content service by media_content_connect().
 * @post This function invokes media_info_cb().
 * @see #media_info_cb
 * @see media_content_connect()
 * @see media_filter_create()
 */
int media_album_foreach_media_from_db(int album_id, filter_h filter, media_info_cb callback, void *user_data);

/**
 * @brief Destroys album handle.
 * @details Function frees all resources related to album handle. This handle
 * no longer can be used to perform any operation. A new handle has to
 * be created before the next use.
 *
 * @param [in] album The handle to media album
 * @return 0 on success, otherwise a negative error value.
 * @retval #MEDIA_CONTENT_ERROR_NONE Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @pre Get copy of album handle by calling media_album_clone()
 * @see media_album_clone()
 */
int media_album_destroy(media_album_h album);

/**
 * @brief Clones media album.
 * @details This function copies the media album handle from a source to
 * destination. There is no media_album_create() function. The media_album_h is created internally and available through 
 * media album foreach function such as media_album_foreach_album_from_db(). To use this handle outside of these foreach functions, 
 * use this function.
 *
 * @remark The destination handle must be released with media_album_destroy() by you.
 *
 * @param [in] src The source handle to media album
 * @param [out] dst A destination handle to media album
 * @return 0 on success, otherwise a negative error value.
 * @retval #MEDIA_CONTENT_ERROR_NONE Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_OUT_OF_MEMORY Out of memory
 * @see media_album_destroy()
 * @see media_album_foreach_album_from_db()
 */
int media_album_clone(media_album_h *dst, media_album_h src);

/**
 * @brief Gets a ID of the album.
 *
 * @param [in] album The handle to media album
 * @param [out] album_id A ID of media album
 * @return 0 on success, otherwise a negative error value.
 * @retval #MEDIA_CONTENT_ERROR_NONE Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_OUT_OF_MEMORY Out of memory
 * @see media_album_foreach_album_from_db()
 *
 */
int media_album_get_album_id(media_album_h album, int *album_id);

/**
 * @brief Gets a name of the album.
 *
 * @remarks @a album_name must be released with free() by you.
 *
 * @param [in] album The handle to media album
 * @param [out] album_name A name of media album handle
 * @return 0 on success, otherwise a negative error value.
 * @retval #MEDIA_CONTENT_ERROR_NONE Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_OUT_OF_MEMORY Out of memory
 *
 */
int media_album_get_name(media_album_h album, char **album_name);

/**
 * @brief Gets a name of the artist from album.
 *
 * @remarks @a artist must be released with free() by you.
 *
 * @param [in] album The handle to media album
 * @param [out] artist A name of media artist
 * @return 0 on success, otherwise a negative error value.
 * @retval #MEDIA_CONTENT_ERROR_NONE Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_OUT_OF_MEMORY Out of memory
 *
 */
int media_album_get_artist(media_album_h album, char **artist);

/**
 * @brief Gets a album art path from album.
 *
 * @remarks @a album_art must be released with free() by you.
 *
 * @param [in] album The handle to media album
 * @param [out] album_art A path of media album_art
 * @return 0 on success, otherwise a negative error value.
 * @retval #MEDIA_CONTENT_ERROR_NONE Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_OUT_OF_MEMORY Out of memory
 *
 */
int media_album_get_album_art(media_album_h album, char **album_art);

/**
 * @brief Gets the media album from the media database.
 *
 * @details This function creates a new media album handle from the media database by the given album_id.
 * media album will be created, which is filled with album information.
 *
 * @remarks @a folder must be released with media_album_destroy() by you.
 *
 * @param[in] album_id The ID of media album
 * @param[out] album The album handle associated with the album ID
 * @return 0 on success, otherwise a negative error value.
 * @retval #MEDIA_CONTENT_ERROR_NONE Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @pre This function requires opened connection to content service by media_content_connect().
 * @see media_content_connect()
 * @see media_album_destroy()
 *
 */
int media_album_get_album_from_db(int album_id, media_album_h *album);


/**
 * @}
 */


/**
* @addtogroup CAPI_CONTENT_MEDIA_GROUP_MODULE
* @{
*/

 /**
 * @brief Gets the number of group for the passed @a filter from the media database.
 *
 * @param[in] filter The handle to media filter
 * @param [in] group The type of media group
 * @param[out] group_count The count of media group
 * @return 0 on success, otherwise a negative error value.
 * @retval #MEDIA_CONTENT_ERROR_NONE Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_DB_FAILED DB operation failed
 * @pre This function requires opened connection to content service by media_content_connect().
 * @see media_content_connect()
 *
 */
int media_group_get_group_count_from_db(filter_h filter, media_group_e group, int *group_count);

 /**
 * @brief Iterates through the media group with optional @a filter from the media database.
 * @details This function gets the names of media group meeting the given filter.
 * The callback function will be invoked for every retrieved media group.
 * If NULL is passed to the filter, no filtering is applied.
 *
 * @param [in] filter The handle to media filter
 * @param [in] group The type of media group
 * @param [in] callback The callback function to invoke
 * @param [in] user_data The user data to be passed to the callback function
 * @return 0 on success, otherwise a negative error value.
 * @retval #MEDIA_CONTENT_ERROR_NONE Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @pre This function requires opened connection to content service by media_content_connect().
 * @post This function invokes media_group_cb().
 * @see #media_group_cb
 * @see media_content_connect()
 * @see media_filter_create()
 */
int media_group_foreach_group_from_db(filter_h filter, media_group_e group, media_group_cb callback, void *user_data);

 /**
 * @brief Gets number of media info for the given media group present in the media database.
 *
 * @param [in] group_name The name of media group
 * @param [in] group The type of media group
 * @param [in] filter The handle to media filter
 * @param [out] media_count The count of media
 * @return 0 on success, otherwise a negative error value.
 * @retval #MEDIA_CONTENT_ERROR_NONE Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @pre This function requires opened connection to content service by media_content_connect().
 * @see media_content_connect()
 */
int media_group_get_media_count_from_db(const char *group_name, media_group_e group, filter_h filter, int *media_count);

 /**
 * @brief Iterates through the media files with optional @a filter in the given @a group from the media database.
 * @details This function gets all media files associated with the given group and
 * meeting desired filter option and calls registered callback function for
 * every retrieved media info. If NULL is passed to the @a filter, no filtering is applied.
 *
 * @param [in] group_name The name of media group
 * @param [in] group The type of media group
 * @param [in] filter The handle to media filter
 * @param [in] callback The callback function to invoke
 * @param [in] user_data The user data to be passed to the callback function
 * @return 0 on success, otherwise a negative error value.
 * @retval #MEDIA_CONTENT_ERROR_NONE Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @pre This function requires opened connection to content service by media_content_connect().
 * @post This function invokes media_info_cb().
 * @see #media_info_cb
 * @see media_content_connect()
 * @see media_filter_create()
 */
int media_group_foreach_media_from_db(const char *group_name, media_group_e group, filter_h filter, media_info_cb callback, void *user_data);

 /**
  * @}
  */

#ifdef __cplusplus
 }
#endif /* __cplusplus */

#endif /* __TIZEN_MEDIA_GROUP_H__ */
