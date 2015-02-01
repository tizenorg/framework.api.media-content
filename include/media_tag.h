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



#ifndef __TIZEN_MEDIA_TAG_H__
#define __TIZEN_MEDIA_TAG_H__

#include <media_content_type.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 * @file media_tag.h
 * @brief This file contains the media tag API and functions related with handling tags. \n
 *        Functions include operations to get the number and content of the tag, the number of media files  \n
 *        and all media items in the tag, to clone, destroy, insert and delete tag from DB,  \n
 *        to handle with name, ID, and media info of the tag.
 */


/**
 * @addtogroup CAPI_CONTENT_MEDIA_TAG_MODULE
 * @{
 */

/**
 * @brief Inserts a new tag in the media database.
 * @since_tizen 2.3
 *
 * @privlevel public
 * @privilege %http://tizen.org/privilege/content.write
 *
 * @remarks The created tag handle must be released using media_tag_destroy().
 *
 * @param[in]  tag_name The tag name to be inserted
 * @param[out] tag      The created handle to the media tag
 *
 * @return @c 0 on success, 
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_OUT_OF_MEMORY     Out of memory
 * @retval #MEDIA_CONTENT_ERROR_PERMISSION_DENIED Permission denied
 *
 * @pre This function requires opened connection to content service by media_content_connect().
 *
 * @see media_content_connect()
 * @see media_tag_delete_from_db()
 * @see media_tag_destroy()
 */
int media_tag_insert_to_db(const char *tag_name, media_tag_h *tag);

/**
 * @brief Deletes a given tag from the media database.
 * @since_tizen 2.3
 *
 * @privlevel public
 * @privilege %http://tizen.org/privilege/content.write
 *
 * @param[in] tag_id The ID of media tag
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
 * @see media_tag_insert_to_db()
 */
int media_tag_delete_from_db(int tag_id);

/**
 * @brief Gets the count of the tag for the passed @a filter from the media database.
 * @since_tizen 2.3
 *
 * @param[in]  filter    The handle to the filter
 * @param[out] tag_count The count of the media tag
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
int media_tag_get_tag_count_from_db(filter_h filter, int *tag_count);

/**
 * @brief Iterates through tags from the media database.
 * @details This function gets all tags meeting a desired @a filter
 *          and calls a registered callback function for every retrieved tag.
 *          If @c NULL is passed to the @a filter, no filtering is applied.
 *
 * @since_tizen 2.3
 *
 * @param[in] filter    The tag filter handle
 * @param[in] callback  The callback function to be invoked
 * @param[in] user_data The user data to be passed to the callback function
 *
 * @return @c 0 on success, 
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_OUT_OF_MEMORY     Out of memory
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_PERMISSION_DENIED Permission denied
 *
 * @pre  This function requires opened connection to content service by media_content_connect().
 * @post This function invokes media_tag_cb().
 *
 * @see media_content_connect()
 * @see #media_tag_cb
 * @see media_filter_create()
 */
int media_tag_foreach_tag_from_db (filter_h filter, media_tag_cb callback, void *user_data);

/**
 * @brief Gets the number of media files for the passed @a filter in the given @a tag from the media database.
 * @since_tizen 2.3
 *
 * @param[in]  tag_id      The ID of the media tag
 * @param[in]  filter      The handle to the media filter
 * @param[out] media_count The count of media items
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
int media_tag_get_media_count_from_db (int tag_id, filter_h filter, int *media_count);

/**
 * @brief Iterates through media items for a given tag from the media database.
 * @details This function gets all media items associated with a given tag and
 *          meeting a desired @a filter and calls a registered callback function for
 *          every retrieved media item. If @c NULL is passed to the @a filter, no filtering is applied.
 *
 * @since_tizen 2.3
 *
 * @param[in] tag_id    The ID of the media tag
 * @param[in] filter    The handle to the media filter
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
 * @post This function invokes media_info_cb().
 *
 * @see media_content_connect()
 * @see media_info_cb()
 * @see media_filter_create()
 */
int media_tag_foreach_media_from_db(int tag_id, filter_h filter, media_info_cb callback, void *user_data);

/**
 * @brief Clones the media tag.
 * @details This function copies the media tag handle from a source to destination.
 *          There is no media_tag_create() function. The media_tag_h is created internally and available through media tag foreach function
 *          such as media_tag_foreach_tag_from_db().
 *          To use this handle outside of these foreach functions, use this function.
 *
 * @since_tizen 2.3
 *
 * @remarks You must release the destination handle using media_tag_destroy().
 *
 * @param[out] dst The destination handle to the media tag
 * @param[in]  src The source handle to the media tag
 *
 * @return @c 0 on success, 
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_OUT_OF_MEMORY     Out of memory
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_PERMISSION_DENIED Permission denied
 *
 * @see media_tag_destroy()
 */
int media_tag_clone(media_tag_h *dst, media_tag_h src);

/**
 * @brief Destroys the media tag.
 * @details This function frees all resources related to the tag handle. The tag handle can no longer
 *          be used for any operation. A new tag handle has to be created before next usage.
 *
 * @since_tizen 2.3
 *
 * @param[in] tag The media tag handle
 *
 * @return @c 0 on success, 
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_PERMISSION_DENIED Permission denied
 *
 * @pre A copy of the media tag handle created by calling media_tag_clone() or media_tag_insert_to_db().
 *
 * @see media_tag_clone()
 * @see media_tag_insert_to_db()
 */
int media_tag_destroy(media_tag_h tag);

/**
 * @brief Gets the media tag ID.
 * @since_tizen 2.3
 *
 * @param[in]  tag     The media tag handle
 * @param[out] tag_id  The ID of the media tag
 *
 * @return @c 0 on success, 
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_PERMISSION_DENIED Permission denied
 */
int media_tag_get_tag_id(media_tag_h tag, int *tag_id);

/**
 * @brief Gets the tag name.
 * @since_tizen 2.3
 *
 * @remarks You must release @a tag_name using free().
 *
 * @param[in]  tag      The media tag handle
 * @param[out] tag_name The name of the tag
 *
 * @return @c 0 on success, 
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_OUT_OF_MEMORY     Out of memory
 * @retval #MEDIA_CONTENT_ERROR_PERMISSION_DENIED Permission denied
 */
int media_tag_get_name(media_tag_h tag, char **tag_name);

/**
 * @brief Gets the media tag from the media database.
 *
 * @details This function creates a new media tag handle from the media database by the given @a tag_id.
 *          Media tag will be created and will be filled with tag information.
 *
 * @since_tizen 2.3
 *
 * @remarks You must release @a folder using media_tag_destroy().
 *
 * @param[in]  tag_id The ID of the media tag
 * @param[out] tag    The media tag handle associated with the tag ID
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
 * @see media_tag_destroy()
 */
int media_tag_get_tag_from_db(int tag_id, media_tag_h *tag);

/**
 * @brief Adds a new media info to the tag.
 * @since_tizen 2.3
 *
 * @param[in] tag      The media tag handle
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
 * @post media_tag_update_to_db()
 *
 * @see media_content_connect()
 * @see media_tag_remove_media()
 */
int media_tag_add_media(media_tag_h tag, const char *media_id);

/**
 * @brief Removes the media info from the given tag.
 * @since_tizen 2.3
 *
 * @param[in] tag      The media tag handle
 * @param[in] media_id The ID to the media info which is removed
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
 * @post media_tag_update_to_db()
 *
 * @see media_content_connect()
 * @see media_tag_add_media()
 */
int media_tag_remove_media(media_tag_h tag, const char *media_id);

/**
 * @brief Sets the name of the tag.
 * @since_tizen 2.3
 *
 * @param[in] tag      The media tag handle
 * @param[in] tag_name The name of the media tag
 *
 * @return @c 0 on success, 
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_OUT_OF_MEMORY     Out of memory
 * @retval #MEDIA_CONTENT_ERROR_PERMISSION_DENIED Permission denied
 *
 * @post media_tag_update_to_db()
 */
int media_tag_set_name(media_tag_h tag, char *tag_name);

/**
 * @brief Updates the media tag to the media database.
 *
 * @details The function updates the given media tag in the media database. The function should be called after any change in tag attributes, to be updated to the media
 *          database. For example, after using media_tag_set_name() for setting the name of the tag, the media_tag_update_to_db() function should be called so as to update
 *          the given tag attributes in the media database.
 *
 * @since_tizen 2.3
 *
 * @privlevel public
 * @privilege %http://tizen.org/privilege/content.write
 *
 * @param[in] tag The media tag handle
 *
 * @return @c 0 on success, 
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 *
 * @pre This function requires opened connection to content service by media_content_connect().
 *
 * @see media_content_connect()
 * @see media_tag_destroy()
 * @see media_tag_add_media()
 * @see media_tag_remove_media()
 * @see media_tag_set_name()
 */
int media_tag_update_to_db(media_tag_h tag);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __TIZEN_MEDIA_TAG_H__ */
