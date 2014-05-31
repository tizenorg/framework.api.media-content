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



#ifndef __TIZEN_MEDIA_BOOKMARK_H__
#define __TIZEN_MEDIA_BOOKMARK_H__

#include <media_content_type.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/**
 * @addtogroup CAPI_CONTENT_MEDIA_BOOKMARK_MODULE
 * @{
 *
 * @file media_bookmark.h
 * @brief This file contains API on main functional operations with bookmarks that are related to media resources in the media database. \n
 *         Operations include: inserting a new bookmark in media to the media database, removing bookmark from database, \n
 *         getting number of bookmarks, cloning and destroying bookmark, getting bookmark`s id, time marked parameter and thumbnail.
 */

/**
 * @brief Inserts a new bookmark in media on specified time offset to the media database.
 *
 * @param [in] media_id The id of media
 * @param [in] time The bookmark time offset(in seconds)
 * @param [in] thumbnail_path The thumbnail path of video bookmark. If the media type is audio, then thumbnail is null.
 * @return 0 on success, otherwise a negative error value.
 * @retval #MEDIA_CONTENT_ERROR_NONE Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @pre This function requires opened connection to content service by media_content_connect().
 * @see media_content_connect()
 * @see media_bookmark_delete_from_db()
 *
 */
int media_bookmark_insert_to_db(const char *media_id, time_t time, const char *thumbnail_path);

/**
 * @brief Removes media bookmark from the media database.
 *
 * @param [in] bookmark The handle to media bookmark
 * @return 0 on success, otherwise a negative error value.
 * @retval #MEDIA_CONTENT_ERROR_NONE Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @pre This function requires opened connection to content service by media_content_connect().
 * @see media_content_connect()
 * @see media_bookmark_insert_to_db()
 *
 */
int media_bookmark_delete_from_db(int bookmark_id);

/**
 * @brief Gets number of bookmark with optional filter from media database.
 *
 * @param [in]	filter	The handle to media filter
 * @param [out] bookmark_count The count of media bookmark
 * @return 0 on success, otherwise a negative error value.
 * @retval #MEDIA_CONTENT_ERROR_NONE Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @pre This function requires opened connection to content service by media_content_connect().
 * @see media_content_connect()
 */
int media_bookmark_get_bookmark_count_from_db(filter_h filter, int *bookmark_count);

/**
 * @brief Clones media bookmark.
 * @details This function copies the media bookmark handle from a source to destination. There is no media_bookmark_create() function.
 * The media_bookmark_h is created internally and available through media bookmark foreach function such as media_info_foreach_bookmark_from_db().
 * To use this handle outside of these foreach functions, use this function.
 *
 * @remark The destination handle must be released with media_bookmark_destroy() by you.
 *
 * @param [out] dst A destination handle to media bookmark
 * @param [in] src The source handle to media bookmark
 * @return 0 on success, otherwise a negative error value.
 * @retval #MEDIA_CONTENT_ERROR_NONE Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_OUT_OF_MEMORY Out of memory
 * @see media_bookmark_destroy()
 * @see media_info_foreach_bookmark_from_db()
 *
 */
int media_bookmark_clone(media_bookmark_h *dst, media_bookmark_h src);

/**
 * @brief Destroys media bookmark.
 * @details Function frees all resources related to bookmark handle. This handle
 * no longer can be used to perform any operation. A new handle has to
 * be created before the next use.
 *
 * @param [in] bookmark The handle to media bookmark
 * @return 0 on success, otherwise a negative error value.
 * @retval #MEDIA_CONTENT_ERROR_NONE Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @pre Get copy of bookmark handle by calling media_bookmark_clone()
 * @see media_bookmark_clone()
 */
int media_bookmark_destroy(media_bookmark_h bookmark);

/**
 * @brief Gets bookmark's id.
 *
 * @param [in] bookmark The handle to media bookmark
 * @param [out] bookmark_id The id of media bookmark
 * @return 0 on success, otherwise a negative error value.
 * @retval #MEDIA_CONTENT_ERROR_NONE Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 */
int media_bookmark_get_bookmark_id(media_bookmark_h bookmark, int *bookmark_id);

/**
 * @brief Gets bookmark's time marked parameter.
 * @details Function returns time offset in milliseconds from beginning of the movie on which bookmark
 * was placed.
 *
 * @param [in] bookmark The handle to media bookmark
 * @param [out] marked_time The bookmark time offset(in milliseconds)
 * @return 0 on success, otherwise a negative error value.
 * @retval #MEDIA_CONTENT_ERROR_NONE Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 */
int media_bookmark_get_marked_time(media_bookmark_h bookmark, time_t *marked_time);

/**
 * @brief Gets the media bookmark's thumbnail.
 *
 * @remarks @a path must be released with free() by you.
 *
 * @param [in] bookmark The handle to media bookmark
 * @param [out] path The thumbnail path of media bookmark
 * @return 0 on success, otherwise a negative error value.
 * @retval #MEDIA_CONTENT_ERROR_NONE Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_OUT_OF_MEMORY Out of memory
 */
int media_bookmark_get_thumbnail_path(media_bookmark_h bookmark, char **path);


/**
 * @}
 */

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /*__TIZEN_MEDIA_BOOKMARK_H__*/
