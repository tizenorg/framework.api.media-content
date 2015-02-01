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

#ifndef __TIZEN_MEDIA_PLAYLIST_H__
#define __TIZEN_MEDIA_PLAYLIST_H__

#include <media_content_type.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 * @file
 * @brief This file contains the playlist API and functions related with handling playlists. \n
 *        Functions include operations to get the number of playlists, the number of media-info for the playlist  \n
 *        and all media files in the playlist, to clone, destroy, insert and delete playlist from DB,  \n
 *        to handle with name, ID, thumbnail, played order and media info of the playlist.
 */

/**
 * @addtogroup CAPI_CONTENT_MEDIA_PLAYLIST_MODULE
 * @{
 */


/**
 * @brief Gets the number of playlists for the passed @a filter from the media database.
 * @since_tizen 2.3
 *
 * @param[in]  filter         The handle to the filter
 * @param[out] playlist_count The count of the media playlist
 *
 * @return @c 0 on success, 
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_DB_FAILED         DB operation failed
 * @retval #MEDIA_CONTENT_ERROR_PERMISSION_DENIED Permission denied
 *
 * @pre This function requires opened connection to content service by media_content_connect().
 *
 * @see media_content_connect()
 */
int media_playlist_get_playlist_count_from_db(filter_h filter, int *playlist_count);

/**
 * @brief Iterates through the media playlists with an optional @a filter from the media database.
 * @details This function gets all media playlists meeting the given filter.
 *          The callback function will be invoked for every retrieved media playlist.
 *          If @c NULL is passed to the filter, no filtering is applied.
 *
 * @since_tizen 2.3
 *
 * @param[in] filter    The handle to the audio filter
 * @param[in] callback  The callback function to be invoked
 * @param[in] user_data The user data to be passed to the callback function
 *
 * @return @c 0 on success, 
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_OUT_OF_MEMORY     Out of memory
 * @retval #MEDIA_CONTENT_ERROR_PERMISSION_DENIED Permission denied
 *
 * @pre  This function requires opened connection to content service by media_content_connect().
 * @post This function invokes media_playlist_cb().
 *
 * @see media_playlist_cb()
 * @see media_content_connect()
 * @see media_filter_create()
 */
int media_playlist_foreach_playlist_from_db(filter_h filter, media_playlist_cb callback, void *user_data);

/**
 * @brief Gets the number of the media info for the given playlist present in the media database.
 * @since_tizen 2.3
 *
 * @param[in]  playlist_id  The ID of the media playlist
 * @param[in]  filter       The media filter handle
 * @param[out] media_count  The number of media items
 *
 * @return @c 0 on success, 
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_PERMISSION_DENIED Permission denied
 *
 * @pre This function requires opened connection to content service by media_content_connect().
 *
 * @see media_content_connect()
 */
int media_playlist_get_media_count_from_db(int playlist_id, filter_h filter, int *media_count);

/**
 * @brief Iterates through the media files with an optional @a filter in the given audio playlist from the media database.
 * @details This function gets all media files associated with the given media playlist and
 *          meeting desired filter option and calls registered callback function for
 *          every retrieved media info. If @c NULL is passed to the @a filter, no filtering is applied.
 *
 * @since_tizen 2.3
 *
 * @param[in] playlist_id The ID of the media playlist
 * @param[in] filter      The audio filter handle
 * @param[in] callback    The callback function to be invoked
 * @param[in] user_data   The user data to be passed to the callback function
 *
 * @return @c 0 on success, 
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_OUT_OF_MEMORY     Out of memory
 * @retval #MEDIA_CONTENT_ERROR_PERMISSION_DENIED Permission denied
 *
 * @pre  This function requires opened connection to content service by media_content_connect().
 * @post This function invokes media_info_cb().
 *
 * @see media_info_cb()
 * @see media_content_connect()
 * @see media_filter_create()
 */
int media_playlist_foreach_media_from_db(int playlist_id, filter_h filter, playlist_member_cb callback, void *user_data);

/**
 * @brief Inserts a new playlist with the given name into the media database.
 * @since_tizen 2.3
 *
 * @privlevel public
 * @privilege %http://tizen.org/privilege/content.write
 *
 * @remarks You must release the created handle using media_playlist_destroy().
 *
 * @param[in]  name     The name of the inserted playlist
 * @param[out] playlist A created handle to media playlist
 *
 * @return @c 0 on success, 
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_OUT_OF_MEMORY     Out of memory
 * @retval #MEDIA_CONTENT_ERROR_INVALID_OPERATION Invalid operation
 * @retval #MEDIA_CONTENT_ERROR_DB_FAILED         DB Operation failed
 * @retval #MEDIA_CONTENT_ERROR_DB_BUSY           DB Operation busy
 * @retval #MEDIA_CONTENT_ERROR_NETWORK           Network fail
 * @retval #MEDIA_CONTENT_ERROR_PERMISSION_DENIED Permission denied
 *
 * @pre This function requires opened connection to content service by media_content_connect().
 *
 * @see media_content_connect()
 * @see media_playlist_delete_from_db()
 */
int media_playlist_insert_to_db(const char *name, media_playlist_h *playlist);

/**
 * @brief Deletes the given playlist from the media database.
 * @since_tizen 2.3
 *
 * @privlevel public
 * @privilege %http://tizen.org/privilege/content.write
 *
 * @param[in] playlist_id The ID of media playlist
 *
 * @return @c 0 on success, 
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_PERMISSION_DENIED Permission denied
 *
 * @pre This function requires opened connection to content service by media_content_connect().
 *
 * @see media_content_connect()
 * @see media_playlist_insert_to_db()
 */
int media_playlist_delete_from_db(int playlist_id);

/**
 * @brief Gets the media playlist from the media database.
 *
 * @details This function creates a new media playlist handle from the media database by the given @a playlist_id.
 *          The media playlist will be created and will be filled with the playlist information.
 *
 * @since_tizen 2.3
 *
 * @remarks You must release @a playlist using media_playlist_destroy().
 *
 * @param[in]  playlist_id The ID of the media playlist
 * @param[out] playlist    The media playlist handle associated with the playlist ID
 *
 * @return @c 0 on success, 
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_PERMISSION_DENIED Permission denied
 *
 * @pre This function requires opened connection to content service by media_content_connect().
 *
 * @see media_content_connect()
 * @see media_playlist_destroy()
 */
int media_playlist_get_playlist_from_db(int playlist_id, media_playlist_h *playlist);

/**
 * @brief Destroys a playlist handle.
 * @details This function frees all resources related to the playlist handle. This
 *          handle no longer can be used to perform any operation. A new handle has to
 *          be created before next usage.
 *
 * @since_tizen 2.3
 *
 * @param[in] playlist The media playlist handle
 *
 * @return @c 0 on success, 
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_PERMISSION_DENIED Permission denied
 *
 * @pre Get a copy of playlist handle by calling media_playlist_clone() or media_playlist_insert_to_db().
 *
 * @see media_playlist_clone()
 */
int media_playlist_destroy(media_playlist_h playlist);

/**
 * @brief Clones a playlist handle.
 * @details This function copies the media playlist handle from a source to
 *          destination. There is no media_playlist_create() function. The media_playlist_h is created internally and available through
 *          media playlist foreach function such as media_playlist_foreach_playlist_from_db().
 *          To use this handle outside of these foreach functions, use this function.
 *
 * @since_tizen 2.3
 *
 * @remarks The destination handle must be released using media_playlist_destroy().
 *
 * @param[in]  src  The source handle of a media playlist
 * @param[out] dst  The destination handle to a media playlist
 *
 * @return @c 0 on success, 
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_OUT_OF_MEMORY     Out of memory
 * @retval #MEDIA_CONTENT_ERROR_PERMISSION_DENIED Permission denied
 *
 * @see media_playlist_destroy()
 * @see media_playlist_foreach_playlist_from_db()
 */
int media_playlist_clone(media_playlist_h *dst, media_playlist_h src);

/**
 * @brief Gets the media playlist ID.
 * @since_tizen 2.3
 *
 * @param[in]  playlist    The media playlist handle
 * @param[out] playlist_id The ID of the media playlist
 *
 * @return @c 0 on success, 
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_PERMISSION_DENIED Permission denied
 */
int media_playlist_get_playlist_id(media_playlist_h playlist, int *playlist_id);

/**
 * @brief Gets a name of the playlist.
 * @since_tizen 2.3
 *
 * @remarks You must release @a playlist_name using free().
 *
 * @param[in]  playlist      The media playlist handle
 * @param[out] playlist_name The playlist name
 *
 * @return @c 0 on success, 
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_OUT_OF_MEMORY     Out of memory
 * @retval #MEDIA_CONTENT_ERROR_PERMISSION_DENIED Permission denied
 */
int media_playlist_get_name(media_playlist_h playlist, char **playlist_name);

/**
 * @brief Sets the name of the playlist.
 * @since_tizen 2.3
 *
 * @param[in] playlist      The media playlist handle
 * @param[in] playlist_name The name of the media playlist
 *
 * @return @c 0 on success, 
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_OUT_OF_MEMORY     Out of memory
 * @retval #MEDIA_CONTENT_ERROR_PERMISSION_DENIED Permission denied
 *
 * @post media_playlist_update_to_db()
 *
 */
int media_playlist_set_name(media_playlist_h playlist, const char *playlist_name);

/**
 * @brief Gets a thumbnail path of the playlist.
 * @since_tizen 2.3
 *
 * @remarks You must release @a path using free().
 *
 * @param[in]  playlist The media playlist handle
 * @param[out] path     The path of the thumbnail
 *
 * @return @c 0 on success, 
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_OUT_OF_MEMORY     Out of memory
 * @retval #MEDIA_CONTENT_ERROR_PERMISSION_DENIED Permission denied
 */
int media_playlist_get_thumbnail_path(media_playlist_h playlist, char **path);

/**
 * @brief Sets the thumbnail path of the playlist.
 * @since_tizen 2.3
 *
 * @param[in] playlist The media playlist handle
 * @param[in] path     The path of the thumbnail
 *
 * @return @c 0 on success, 
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_OUT_OF_MEMORY     Out of memory
 * @retval #MEDIA_CONTENT_ERROR_PERMISSION_DENIED Permission denied
 *
 * @post media_playlist_update_to_db()
 */
int media_playlist_set_thumbnail_path(media_playlist_h playlist, const char *path);


/**
 * @brief Sets the playing order in the playlist.
 * @since_tizen 2.3
 *
 * @param[in] playlist           The media playlist handle
 * @param[in] playlist_member_id The playlist member ID
 * @param[in] play_order         The playing order
 *
 * @return @c 0 on success, 
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_OUT_OF_MEMORY     Out of memory
 * @retval #MEDIA_CONTENT_ERROR_PERMISSION_DENIED Permission denied
 *
 * @post media_playlist_update_to_db()
 */
int media_playlist_set_play_order(media_playlist_h playlist, int playlist_member_id, int play_order);

/**
 * @brief Adds a new media info to the playlist.
 * @since_tizen 2.3
 *
 * @param[in] playlist The media playlist handle
 * @param[in] media_id The ID to the media info which is added
 *
 * @return @c 0 on success, 
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_OUT_OF_MEMORY     Out of memory
 * @retval #MEDIA_CONTENT_ERROR_PERMISSION_DENIED Permission denied
 *
 * @pre  This function requires opened connection to content service by media_content_connect().
 * @post media_playlist_update_to_db()
 *
 * @see media_content_connect()
 * @see media_playlist_remove_media()
 */
int media_playlist_add_media(media_playlist_h playlist, const char *media_id);

/**
 * @brief Removes the playlist members related with the media from the given playlist.
 * @since_tizen 2.3
 *
 * @param[in] playlist           The media playlist handle
 * @param[in] playlist_member_id The playlist member ID
 *
 * @return @c 0 on success, 
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_OUT_OF_MEMORY     Out of memory
 * @retval #MEDIA_CONTENT_ERROR_PERMISSION_DENIED Permission denied
 *
 * @pre  This function requires opened connection to content service by media_content_connect().
 * @post media_playlist_update_to_db()
 *
 * @see media_content_connect()
 * @see media_playlist_add_media()
 */
int media_playlist_remove_media(media_playlist_h playlist, int playlist_member_id);

/**
 * @brief Gets the played order of the playlist.
 * @since_tizen 2.3
 *
 * @param[in] playlist           The media playlist handle
 * @param[in] playlist_member_id The playlist member ID
 * @param[out] play_order        The played order
 *
 * @return @c 0 on success, 
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_PERMISSION_DENIED Permission denied
 */
int media_playlist_get_play_order(media_playlist_h playlist, int playlist_member_id, int *play_order);

/**
 * @brief Updates the media playlist to the media database.
 *
 * @details The function updates the given media playlist in the media database.
 *          The function should be called after any change in the playlist, to be updated to the media database.
 *          For example, after using media_playlist_set_name() for setting the name of the playlist, the
 *          media_playlist_update_to_db() function should be called so as to update
 *          the given playlist attributes in the media database.
 *
 * @since_tizen 2.3
 *
 * @privlevel public
 * @privilege %http://tizen.org/privilege/content.write
 *
 * @param[in] playlist The media playlist handle
 *
 * @return @c 0 on success, 
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_PERMISSION_DENIED Permission denied
 *
 * @pre This function requires opened connection to content service by media_content_connect().
 *
 * @see media_content_connect()
 * @see media_playlist_destroy()
 * @see media_playlist_add_media()
 * @see media_playlist_remove_media()
 * @see media_playlist_set_name()
 * @see media_playlist_set_play_order()
 */
int media_playlist_update_to_db(media_playlist_h playlist);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /*__TIZEN_MEDIA_PLAYLIST_H__*/
