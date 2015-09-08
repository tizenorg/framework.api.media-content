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


#ifndef __TIZEN_MEDIA_INFORMATION_H__
#define __TIZEN_MEDIA_INFORMATION_H__

#include <media_content_type.h>



#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 * @file media_info.h
 * @brief This file contains the media info API and related functions to proceed with it. \n
 *        You can use the functions to insert, clone, delete, get the number and content of files from DB. \n
 *        You can get and set properties and parameters such as storage type, provider, and category of media info, \n
 *        handling with thumbnail and updating media info to DB.
 */


/**
 * @addtogroup CAPI_CONTENT_MEDIA_INFO_MODULE
 * @{
 */

/**
 * @brief  Inserts a media file into the media database.
 * @details This function inserts a media item into the content storage.
 *          Normally, inserting a media file in database is done automatically by the media server, without calling this function.
 *          This function is only called when the media server is busy and the user needs to get quick result of inserting
 *          e.g. Taking a photo while media server is busy and the user wants to see the quick snapshot of the photo taken.
 *
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 * @privlevel public
 * @privilege %http://tizen.org/privilege/content.write \n
 *                   %http://tizen.org/privilege/mediastorage \n
 *                   %http://tizen.org/privilege/externalstorage
 *
 * @remarks You must release the handle using media_info_destroy(). \n
 *                   You must add privilege http://tizen.org/privilege/content.write. And You add more privilege depending on your choice of contents path. \n
 *                   If you want to access only internal storage by using  this API, you should add privilege http://tizen.org/privilege/mediastorage. \n
 *                   Or if you want to access only external storage by using  this API, you shold add privilege http://tizen.org/privilege/externalstorage. \n
 *                   If you can access both storage, you must add all privilege.
 *
 * @param[in]  path The path to the media file
 * @param[out] info The handle to the media info
 *
 * @return @c 0 on success, 
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_PERMISSION_DENIED Permission denied
 *
 * @pre This function requires opened connection to content service by media_content_connect().
 *
 * @see media_content_connect()
 */
int media_info_insert_to_db (const char *path, media_info_h *info);

/**
 * @brief Inserts media files into the media database, asynchronously.
 * @details This function inserts media items into the content storage.
 *          Normally, inserting a media file in database is done automatically by the media server, without calling this function.
 *          This function invokes media_insert_completed_cb() callback function when insertion to the media database is finished.
 *
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 * @privlevel public
 * @privilege %http://tizen.org/privilege/content.write \n
 *                   %http://tizen.org/privilege/mediastorage \n
 *                   %http://tizen.org/privilege/externalstorage
 *
 * @remarks You must add privilege http://tizen.org/privilege/content.write. And You add more privilege depending on your choice of contents path. \n
 *                   If you want to access only internal storage by using  this API, you should add privilege http://tizen.org/privilege/mediastorage. \n
 *                   Or if you want to access only external storage by using  this API, you shold add privilege http://tizen.org/privilege/externalstorage. \n
 *                   If you can access both storage, you must add all privilege.
 *
 * @param[in] path_array   The path array to the media files
 * @param[in] array_length The length of the array
 * @param[in] callback     The callback to be invoked when media items inserted completely
 * @param[in] user_data    The user data to be passed to the callback function
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
 * @see media_insert_completed_cb()
 */
int media_info_insert_batch_to_db(const char **path_array,unsigned int array_length, media_insert_completed_cb callback, void *user_data);

/**
 * @brief Inserts the burst shot images into the media database, asynchronously.
 * @details This function inserts burst images into the content storage.
 *          media_insert_burst_shot_completed_cb() will be called when insertion to media database is finished.
 *
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 * @privlevel public
 * @privilege %http://tizen.org/privilege/content.write \n
 *                   %http://tizen.org/privilege/mediastorage \n
 *                   %http://tizen.org/privilege/externalstorage
 *
 * @remarks You must add privilege http://tizen.org/privilege/content.write. And You add more privilege depending on your choice of contents path. \n
 *                   If you want to access only internal storage with this API, you should add privilege http://tizen.org/privilege/mediastorage. \n
 *                   Or if you want to access only external storage with this API, you shold add privilege http://tizen.org/privilege/externalstorage. \n
 *                   If you can access both storage, you must add all privilege.
 *
 * @param[in] path_array   The path array to the burst shot images
 * @param[in] array_length The length of the array
 * @param[in] callback     The callback to be invoked when the images are inserted completely
 * @param[in] user_data    The user data to be passed to the callback function
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
 * @see media_insert_burst_shot_completed_cb()
 */
int media_info_insert_burst_shot_to_db(const char **path_array,unsigned int array_length, media_insert_burst_shot_completed_cb callback, void *user_data);

/**
 * @brief Deletes a media file from the media database.
 * @details This function deletes a media item from the content storage.
 *          Normally, deleting a media file in the database is done automatically by the media server, without calling this function.
 *          This function is only called when the media server is busy and user needs to get quick result of deleting.
 *
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 * @privlevel public
 * @privilege %http://tizen.org/privilege/content.write
 *
 * @param[in] media_id  The ID to the media file
 *
 * @return @c 0 on success, 
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_PERMISSION_DENIED Permission denied
 *
 * @pre This function requires opened connection to content service by media_content_connect().
 *
 * @see media_content_connect()
 */
int media_info_delete_from_db(const char *media_id);

/**
 * @brief Deletes media files from the media database. 
 *        The media files for deletion can be specified as a condition in a filter.
 * @details This function deletes the media items from the content storage.
 *          Normally, deleting  media files in the database are done automatically by the media server, without calling this function.
 *          This function is only called when the media server is busy and user needs to get quick result of deleting.
 *
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 * @privlevel public
 * @privilege %http://tizen.org/privilege/content.write
 *
 * @param[in] filter  The handle to filter
 *
 * @return @c 0 on success, 
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_PERMISSION_DENIED Permission denied
 *
 * @pre This function requires opened connection to content service by media_content_connect().
 *
 * @see media_content_connect()
 */
int media_info_delete_batch_from_db(filter_h filter);


/**
 * @brief Destroys media info.
 * @details The function frees all resources related to the media info handle. This handle
 *          can no longer be used to perform any operation. New media info handle has to
 *          be created before the next usage.
 *
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 * @param[in]  media  The media info handle
 *
 * @return @c 0 on success, 
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_PERMISSION_DENIED Permission denied
 *
 * @pre Get copy of media_info handle by calling media_info_clone().
 *
 * @see media_info_clone()
 */
int media_info_destroy(media_info_h media);

/**
 * @brief Clones the media info handle.
 *
 * @details This function copies the media info handle from a source to the destination.
 *          There is no media_info_create() function. The media_info_h is created internally and
 *          available through media info foreach function such as media_info_foreach_media_from_db().
 *          To use this handle outside of these foreach functions, use this function.
 *
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 * @remarks You must release the destination handle using media_info_destroy().
 *
 * @param[out] dst  The destination handle to the media info
 * @param[in]  src  The source handle to media info
 *
 * @return @c 0 on success, 
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_OUT_OF_MEMORY     Out of memory
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_PERMISSION_DENIED Permission denied
 *
 * @see media_info_destroy()
 * @see media_album_foreach_media_from_db()
 * @see media_playlist_foreach_media_from_db()
 * @see media_group_foreach_media_from_db
 * @see media_tag_foreach_media_from_db()
 * @see media_info_foreach_media_from_db()
 * @see media_folder_foreach_media_from_db()
 */
int media_info_clone(media_info_h *dst, media_info_h src);

/**
 * @brief Gets the count of media info for the passed @a filter from the media database.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 * @param[in]  filter      The handle to filter
 * @param[out] media_count The count of media
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
int media_info_get_media_count_from_db(filter_h filter, int *media_count);

/**
 * @brief Iterates through media info from the media database.
 * @details This function gets all media info handles meeting the given @a filter.
 *          The @a callback function will be invoked for every retrieved media info.
 *          If @c NULL is passed to the @a filter, then no filtering is applied.
 *
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 * @remarks Do not call updating DB function like media_info_update_to_db(), media_info_refresh_metadata_to_db(), audio_meta_update_to_db(), image_meta_update_to_db() and video_meta_update_to_db()  in your callback function,
 *                   your callback function is invoked as inline function.
 *                   So, your callback function is in read state in SQLite. When you are in read state, sometimes you do not update DB.
 *                   We do not recommend you call updating DB function in callback of foreach function.
 *
 * @param[in] filter    The media info handle filter
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
 * @post This function invokes media_info_cb().
 *
 * @see media_content_connect()
 * @see #media_info_cb
 * @see media_info_filter_create()
 */
int media_info_foreach_media_from_db(filter_h filter, media_info_cb callback, void *user_data);

/**
 * @brief Gets the count of media tags for the passed @a filter in the given @a media_id from the media database.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 * @param[in]  media_id  The ID of the media info
 * @param[in]  filter    The handle to the media filter
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
int media_info_get_tag_count_from_db(const char *media_id, filter_h filter, int *tag_count);

/**
 * @brief Iterates through the media tag in the given media info from the media database.
 * @details This function gets all the media tags associated with the given @a media_id and calls registered callback function for every retrieved media tag.
 *
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 * @param[in] media_id  The ID of the media info
 * @param[in] filter    The handle to the media filter
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
 */
int media_info_foreach_tag_from_db(const char *media_id, filter_h filter, media_tag_cb callback, void *user_data);

/**
 * @brief Gets the number of bookmarks for the passed @a filter in the given media ID from the media database.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 * @param[in]  media_id        The ID of the media info
 * @param[in]  filter          The handle to the media filter
 * @param[out] bookmark_count  The count of the media tag
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
int media_info_get_bookmark_count_from_db(const char *media_id, filter_h filter, int *bookmark_count);

/**
 * @brief Iterates through the media bookmark in the given media info from the media database.
 * @details This function gets all media bookmarks associated with the given media and calls registered callback function for every retrieved media bookmark.
 *
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 * @param[in] media_id  The ID of the media info
 * @param[in] filter    The handle to the media filter
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
 * @post This function invokes media_bookmark_cb().
 *
 * @see media_content_connect()
 * @see media_bookmark_cb()
 */
int media_info_foreach_bookmark_from_db (const char *media_id, filter_h filter, media_bookmark_cb callback, void *user_data);

/**
 * @brief Gets the image metadata for a given media info.
 * @details This function returns an image metadata handle retrieved from the media info.
 *
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 * @remarks You must release the @a image handle using image_meta_destroy().
 *
 * @param[in]  media  The media info handle
 * @param[out] image  A handle to image metadata
 *
 * @return @c 0 on success, 
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_PERMISSION_DENIED Permission denied
 *
 * @see image_meta_destroy()
 */
int media_info_get_image(media_info_h media, image_meta_h *image);

/**
 * @brief Gets a video metadata for a given media info.
 * @details This function returns a video metadata handle retrieved from the media info handle.
 *
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 * @remarks You must release the @a video handle using video_meta_destroy().
 *
 * @param[in]  media The media info handle
 * @param[out] video A handle to the video meta
 *
 * @return @c 0 on success, 
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_OUT_OF_MEMORY     Out of memory
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_PERMISSION_DENIED Permission denied
 *
 * @see video_meta_destroy()
 */
int media_info_get_video(media_info_h media, video_meta_h *video);

/**
 * @brief Gets an audio metadata for a given media info.
 * @details This function returns an audio metadata handle retrieved from the media info handle.
 *
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 * @remarks You must release the @a audio handle using audio_meta_destroy().
 *
 * @param[in]  media The media info handle
 * @param[out] audio A handle to the audio meta
 *
 * @return @c 0 on success, 
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_OUT_OF_MEMORY     Out of memory
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_PERMISSION_DENIED Permission denied
 *
 * @see audio_meta_destroy()
 */
int media_info_get_audio(media_info_h media, audio_meta_h *audio);

/**
 * @brief Gets the tag ID for the media info.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 * @param[in]  media The media info handle
 * @param[out] media_id     The ID of the media tag
 *
 * @return @c 0 on success, 
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_PERMISSION_DENIED Permission denied
 */
int media_info_get_media_id(media_info_h media, char **media_id);

/**
 * @brief Gets the path to the media info.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 * @remarks You must release @a path using free().
 *
 * @param[in]  media The media info handle
 * @param[out] path  The path of the media info
 *
 * @return @c 0 on success, 
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_OUT_OF_MEMORY     Out of memory
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_PERMISSION_DENIED Permission denied
 *
 */
int media_info_get_file_path(media_info_h media, char **path);

/**
 * @brief Gets the name of the media info.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 * @remarks You must release @a name using free().
 *
 * @param[in]  media The media info handle
 * @param[out] name  The name of media info
 *
 * @return @c 0 on success, 
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_OUT_OF_MEMORY     Out of memory
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_PERMISSION_DENIED Permission denied
 */
int media_info_get_display_name(media_info_h media, char **name);

/**
 * @brief Gets the content type of the media info.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 * @param[in]  media The media info handle
 * @param[out] type  The type of the media content (#media_content_type_e)
 *
 * @return @c 0 on success, 
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_PERMISSION_DENIED Permission denied
 */
int media_info_get_media_type(media_info_h media, media_content_type_e *type);

/**
 * @brief Gets the MIME type from the media info.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 * @remarks You must release @a mime_type using free().
 *
 * @param[in]  media     The media info handle
 * @param[out] mime_type The MIME type of the media info
 *
 * @return @c 0 on success, 
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_OUT_OF_MEMORY     Out of memory
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_PERMISSION_DENIED Permission denied
 */
int media_info_get_mime_type(media_info_h media, char **mime_type);

/**
 * @brief Gets the media file size.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 * @param[in]  media The media info handle
 * @param[out] size  The type of the media content
 *
 * @return @c 0 on success, 
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_PERMISSION_DENIED Permission denied
 */
int media_info_get_size(media_info_h media, unsigned long long *size);

/**
 * @brief Gets the addition time of the media.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 * @param[in]  media      The media info handle
 * @param[out] added_time The added time to the DB
 *
 * @return @c 0 on success, 
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_PERMISSION_DENIED Permission denied
 */
int media_info_get_added_time(media_info_h media, time_t *added_time);

/**
 * @brief Gets the date of modification of media info.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 * @param[in]  media The media info handle
 * @param[out] time  The date of modification of the file \n
 *                   Get from the file.
 *
 * @return @c 0 on success, 
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_PERMISSION_DENIED Permission denied
 */
int media_info_get_modified_time(media_info_h media, time_t *time);

/**
 * @brief Gets the timeline of media info.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 * @param[in]  media The media info handle
 * @param[out] time  The date of the timeline
 *
 * @return @c 0 on success, 
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_PERMISSION_DENIED Permission denied
 */
int media_info_get_timeline(media_info_h media, time_t* time);

/**
 * @brief Gets the thumbnail of media info.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 * @remarks You must release @a path using free().
 *
 * @param[in]  media The media info handle
 * @param[out] path  The path to the thumbnail of the media info
 *
 * @return @c 0 on success, 
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_OUT_OF_MEMORY     Out of memory
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_PERMISSION_DENIED Permission denied
 */
int media_info_get_thumbnail_path(media_info_h media, char **path);

/**
 * @brief Gets the description of media info.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 * @remarks You must release @a description using free().
 *
 * @param[in]  media       The media info handle
 * @param[out] description The description of the media info
 *
 * @return @c 0 on success, 
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_OUT_OF_MEMORY     Out of memory
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_PERMISSION_DENIED Permission denied
 */
int media_info_get_description(media_info_h media, char **description);

/**
 * @brief Gets the longitude of media info.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 * @param[in]  media     The media info handle
 * @param[out] longitude The longitude of the media info
 *
 * @return @c 0 on success, 
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_PERMISSION_DENIED Permission denied
 */
int media_info_get_longitude(media_info_h media, double* longitude);

/**
 * @brief Gets the latitude of media info.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 * @param[in]  media    The media info handle
 * @param[out] latitude The latitude of the media info
 *
 * @return @c 0 on success, 
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_PERMISSION_DENIED Permission denied
 *
 */
int media_info_get_latitude(media_info_h media, double* latitude);

/**
 * @brief Gets the altitude of media info.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 * @param[in]  media    The media info handle
 * @param[out] altitude The altitude of the media info
 *
 * @return @c 0 on success, 
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_PERMISSION_DENIED Permission denied
 */
int media_info_get_altitude(media_info_h media, double* altitude);

/**
 * @brief Gets the weather of media info.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 * @param[in]  media   The media info handle
 * @param[out] weather The weather of the media info
 *
 * @return @c 0 on success, 
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_PERMISSION_DENIED Permission denied
 */
int media_info_get_weather(media_info_h media, char **weather);

/**
 * @brief Gets the rating of media info.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 * @param[in]  media  The media info handle
 * @param[out] rating The rating of the media info
 *
 * @return @c 0 on success, 
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_PERMISSION_DENIED Permission denied
 */
int media_info_get_rating(media_info_h media, int *rating);

/**
 * @brief Gets the favorite status of media info.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 * @param[in]  media     The media info handle
 * @param[out] favorite  @c true if media info is set as favorite, 
 *                       otherwise @c false if media info is not set as favorite
 *
 * @return @c 0 on success, 
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_PERMISSION_DENIED Permission denied
 */
int media_info_get_favorite(media_info_h media, bool* favorite);

/**
 * @brief Gets the author of media info.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 * @remarks You must release @a author using free().
 *
 * @param[in]  media  The media info handle
 * @param[out] author The author of the media info
 *
 * @return @c 0 on success, 
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_OUT_OF_MEMORY     Out of memory
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_PERMISSION_DENIED Permission denied
 */
int media_info_get_author(media_info_h media, char **author);

/**
 * @brief Gets the provider of media info.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 * @remarks You must release @a provider using free().
 *
 * @param[in]  media    The media info handle
 * @param[out] provider The provider of the media info
 *
 * @return @c 0 on success, 
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_OUT_OF_MEMORY     Out of memory
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_PERMISSION_DENIED Permission denied
 */
int media_info_get_provider(media_info_h media, char **provider);

/**
 * @brief Gets the content name of media info.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 * @remarks You must release @a content_name using free().
 *
 * @param[in]  media        The media info handle
 * @param[out] content_name The content name of the media info
 *
 * @return @c 0 on success, 
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_OUT_OF_MEMORY     Out of memory
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_PERMISSION_DENIED Permission denied
 */
int media_info_get_content_name(media_info_h media, char **content_name);

/**
 * @brief Gets the title of media info.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 * @remarks You must release @a title using free().
 *
 * @param[in]  media  The media info handle
 * @param[out] title  The title of the media info
 *
 * @return @c 0 on success, 
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_OUT_OF_MEMORY     Out of memory
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_PERMISSION_DENIED Permission denied
 */
int media_info_get_title(media_info_h media, char **title);

/**
 * @brief Gets the category of media info.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 * @remarks You must release @a category using free().
 *
 * @param[in]  media    The media info handle
 * @param[out] category The category of the media info
 *
 * @return @c 0 on success, 
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_OUT_OF_MEMORY     Out of memory
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_PERMISSION_DENIED Permission denied
 */
int media_info_get_category(media_info_h media, char **category);

/**
 * @brief Gets the location tag of media info.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 * @remarks You must release @a location_tag using free().
 *
 * @param[in]  media        The media info handle
 * @param[out] location_tag The location of the media info
 *
 * @return @c 0 on success, 
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_OUT_OF_MEMORY     Out of memory
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_PERMISSION_DENIED Permission denied
 */
int media_info_get_location_tag(media_info_h media, char **location_tag);

/**
 * @brief Gets the age_rating of media info.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 * @remarks You must release @a age_rating using free().
 *
 * @param[in]  media      The media info handle
 * @param[out] age_rating The age rating of the media info
 *
 * @return @c 0 on success, 
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_OUT_OF_MEMORY     Out of memory
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_PERMISSION_DENIED Permission denied
 */
int media_info_get_age_rating(media_info_h media, char **age_rating);

/**
 * @brief Gets the keyword of media info.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 * @remarks You must release @a keyword using free().
 *
 * @param[in]  media   The media info handle
 * @param[out] keyword The keyword of the media info
 *
 * @return @c 0 on success, 
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_OUT_OF_MEMORY     Out of memory
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_PERMISSION_DENIED Permission denied
 */
int media_info_get_keyword(media_info_h media, char **keyword);

/**
 * @brief Checks whether the media is protected via DRM.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 * @param[in]  media  The media info handle
 * @param[out] is_drm @c true if media is DRM media,
 *                    otherwise @c false if media is not DRM media
 *
 * @return @c 0 on success, 
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_PERMISSION_DENIED Permission denied
 */
int media_info_is_drm(media_info_h media, bool *is_drm);

/**
 * @brief Gets the storage type of media info.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 * @param[in]  media        The media info handle
 * @param[out] storage_type The storage type of the media info
 *
 * @return @c 0 on success, 
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_PERMISSION_DENIED Permission denied
 */
int media_info_get_storage_type(media_info_h media, media_content_storage_e *storage_type);

/**
 * @brief Gets the media info from the media database.
 *
 * @details This function creates a new media handle from the media database by the given @a media_id.
 *          Media info will be created and filled with information.
 *
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 * @remarks You must release @a media using media_tag_destroy().
 *
 * @param[in]  media_id The ID of media info
 * @param[out] media    The media handle associated with the media ID
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
 * @see media_info_destroy()
 */
int media_info_get_media_from_db(const char *media_id, media_info_h *media);

/**
 * @brief Sets the display name of media info.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 * @param[in] media        The media info handle
 * @param[in] display_name The display name of the media info
 *
 * @return @c 0 on success, 
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_OUT_OF_MEMORY     Out of memory
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_PERMISSION_DENIED Permission denied
 *
 * @post media_info_update_to_db().
 *
 */
int media_info_set_display_name(media_info_h media, const char *display_name);

/**
 * @brief Sets the description of media info.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 * @param[in] media       The media info handle
 * @param[in] description The description of the media info
 *
 * @return @c 0 on success, 
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_OUT_OF_MEMORY     Out of memory
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_PERMISSION_DENIED Permission denied
 *
 * @post media_info_update_to_db().
 *
 */
int media_info_set_description(media_info_h media, const char *description);

/**
 * @brief Sets the longitude of media info.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 * @param[in] media     The media info handle
 * @param[in] longitude The longitude of the media info
 *
 * @return @c 0 on success, 
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_OUT_OF_MEMORY     Out of memory
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_PERMISSION_DENIED Permission denied
 *
 * @post media_info_update_to_db().
 */
int media_info_set_longitude(media_info_h media, double longitude);

/**
 * @brief Sets the latitude of media info.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 * @param[in] media    The media info handle
 * @param[in] latitude The latitude of the media info
 *
 * @return @c 0 on success, 
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_OUT_OF_MEMORY     Out of memory
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_PERMISSION_DENIED Permission denied
 *
 * @post media_info_update_to_db().
 */
int media_info_set_latitude(media_info_h media, double latitude);

/**
 * @brief Sets the altitude of media info.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 * @param[in] media    The media info handle
 * @param[in] altitude The altitude of the media info
 *
 * @return @c 0 on success, 
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_OUT_OF_MEMORY     Out of memory
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_PERMISSION_DENIED Permission denied
 *
 * @post media_info_update_to_db().
 */
int media_info_set_altitude(media_info_h media, double altitude);

/**
 * @brief Sets the weather of media info.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 * @param[in] media   The media info handle
 * @param[in] weather The weather of the media info
 *
 * @return @c 0 on success, 
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_OUT_OF_MEMORY     Out of memory
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_PERMISSION_DENIED Permission denied
 *
 * @post media_info_update_to_db().
 *
 */
int media_info_set_weather(media_info_h media, const char *weather);

/**
 * @brief Sets the rating of media info.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 * @param[in] media  The media info handle
 * @param[in] rating The rating of the media info
 *
 * @return @c 0 on success, 
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_OUT_OF_MEMORY     Out of memory
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_PERMISSION_DENIED Permission denied
 *
 * @post media_info_update_to_db().
 */
int media_info_set_rating(media_info_h media, int rating);

/**
 * @brief Sets the favorite of media info.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 * @param[in] media    The media info handle
 * @param[in] favorite Set @c true to set the media info as favorite,
 *                     otherwise set @c false to not set the media info as favorite
 *
 * @return @c 0 on success, 
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_OUT_OF_MEMORY     Out of memory
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_PERMISSION_DENIED Permission denied
 */
int media_info_set_favorite(media_info_h media, bool favorite);

/**
 * @brief Sets the author of media info.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 * @param[in] media  The media info handle
 * @param[in] author The author of the media info
 *
 * @return @c 0 on success, 
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_OUT_OF_MEMORY     Out of memory
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_PERMISSION_DENIED Permission denied
 */
int media_info_set_author(media_info_h media, const char *author);

/**
 * @brief Sets the provider of media info.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 * @param[in] media    The media info handle
 * @param[in] provider The provider of the media info
 *
 * @return @c 0 on success, 
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_OUT_OF_MEMORY     Out of memory
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_PERMISSION_DENIED Permission denied
 */
int media_info_set_provider(media_info_h media, const char *provider);

/**
 * @brief Sets the content name of media info.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 * @param[in] media        The media info handle
 * @param[in] content_name The content name of the media info
 *
 * @return @c 0 on success, 
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_OUT_OF_MEMORY     Out of memory
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_PERMISSION_DENIED Permission denied
 *
 * @post media_info_update_to_db()
 */
int media_info_set_content_name(media_info_h media, const char *content_name);

/**
 * @brief Sets the category of media info.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 * @param[in] media    The media info handle
 * @param[in] category The category of the media info
 *
 * @return @c 0 on success, 
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_OUT_OF_MEMORY     Out of memory
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_PERMISSION_DENIED Permission denied
 *
 * @post media_info_update_to_db()
 */
int media_info_set_category(media_info_h media, const char *category);

/**
 * @brief Sets the location tag of media info.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 * @param[in] media        The media info handle
 * @param[in] location_tag The location of the media info
 *
 * @return @c 0 on success, 
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_OUT_OF_MEMORY     Out of memory
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_PERMISSION_DENIED Permission denied
 *
 * @post media_info_update_to_db()
 */
int media_info_set_location_tag(media_info_h media, const char *location_tag);

/**
 * @brief Sets the age rating of media info.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 * @param[in] media      The media info handle
 * @param[in] age_rating The age rating of the media info
 *
 * @return @c 0 on success, 
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_OUT_OF_MEMORY     Out of memory
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_PERMISSION_DENIED Permission denied
 *
 * @post media_info_update_to_db()
 */
int media_info_set_age_rating(media_info_h media, const char *age_rating);

/**
 * @brief Sets the keyword of media info.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 * @param[in] media   The media info handle
 * @param[in] keyword The keyword of the media info
 *
 * @return @c 0 on success, 
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_OUT_OF_MEMORY     Out of memory
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_PERMISSION_DENIED Permission denied
 *
 * @post media_info_update_to_db()
 */
int media_info_set_keyword(media_info_h media, const char *keyword);

/**
 * @brief Updates the media info to the media database.
 * 
 * @details The function updates the given media info in the media database.
 *
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 * @privlevel public
 * @privilege %http://tizen.org/privilege/content.write
 *
 * @remarks The function should be called after any change in media, to be updated to the media
 *          database. For example, after using media_info_set_display_name()
 *          for setting the name of the media, the media_info_update_to_db() function should be called so as to update
 *          the given media info attributes in the media database.
 *
 * @param[in] media The media info handle
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
 * @see media_info_set_display_name()
 * @see media_info_set_description()
 * @see media_info_set_longitude()
 * @see media_info_set_latitude()
 * @see media_info_set_altitude()
 * @see media_info_set_rating()
 * @see media_info_set_favorite()
 * @see media_info_set_author()
 * @see media_info_set_provider()
 * @see media_info_set_content_name()
 * @see media_info_set_category()
 * @see media_info_set_location_tag()
 * @see media_info_set_age_rating()
 */
int media_info_update_to_db(media_info_h media);

/**
 * @brief Refreshes the media metadata to the media database.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 * @privlevel public
 * @privilege %http://tizen.org/privilege/content.write \n
 *                   %http://tizen.org/privilege/mediastorage \n
 *                   %http://tizen.org/privilege/externalstorage
 *
 * @remarks You must add privilege http://tizen.org/privilege/content.write. And You add more privilege depending on your choice of contents path. \n
 *                   If you want to access only internal storage by using  this API, you should add privilege http://tizen.org/privilege/mediastorage. \n
 *                   Or if you want to access only external storage by using  this API, you shold add privilege http://tizen.org/privilege/externalstorage. \n
 *                   If you can access both storage, you should add all privilege.
 *
 * @param[in] media_id The ID of the media info
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
int media_info_refresh_metadata_to_db(const char *media_id);

/**
 * @brief Sets the added time of media info.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 * @param[in] media      The media info handle
 * @param[in] added_time The added time of the media info
 *
 * @return @c 0 on success, 
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_OUT_OF_MEMORY     Out of memory
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_PERMISSION_DENIED Permission denied
 *
 * @post media_info_update_to_db()
 */
int media_info_set_added_time(media_info_h media, time_t added_time);

/**
 * @brief Moves the media info to the given destination path in the media database.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 * @privlevel public
 * @privilege %http://tizen.org/privilege/content.write \n
 *                   %http://tizen.org/privilege/mediastorage \n
 *                   %http://tizen.org/privilege/externalstorage
 *
 * @remarks You must add privilege http://tizen.org/privilege/content.write. And You add more privilege depending on your choice of contents path. \n
 *                   If you want to access only internal storage by using  this API, you should add privilege http://tizen.org/privilege/mediastorage. \n
 *                   Or if you want to access only external storage by using this API, you shold add privilege http://tizen.org/privilege/externalstorage. \n
 *                   If you can access both storage, you should add all privilege.
 *
 * @param[in] media    The media info handle
 * @param[in] dst_path The path of destination
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
int media_info_move_to_db(media_info_h media, const char* dst_path);

/**
 * @brief Creates a thumbnail image for the given media, asynchronously.
 * @details This function creates an thumbnail image for given media item and calls registered callback function for completion of creating the thumbnail.
 *          If a thumbnail already exists for the given media, then the path of thumbnail will be returned in callback function.
 *
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 * @privlevel public
 * @privilege %http://tizen.org/privilege/content.write
 *
 * @param[in] media     The media info handle
 * @param[in] callback  The callback function to be invoked
 * @param[in] user_data The user data to be passed to the callback function
 *
 * @return @c 0 on success, 
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter (Especially, if the request is duplicated, this error returns.)
 * @retval #MEDIA_CONTENT_ERROR_DB_FAILED         DB operation failed
 * @retval #MEDIA_CONTENT_ERROR_PERMISSION_DENIED Permission denied
 *
 * @pre This function requires opened connection to content service by media_content_connect().
 *
 * @see media_content_connect()
 */
int media_info_create_thumbnail(media_info_h media, media_thumbnail_completed_cb callback, void *user_data);

/**
 * @brief Cancels the creation of image's thumbnail for the given media.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 * @privlevel public
 * @privilege %http://tizen.org/privilege/content.write
 *
 * @param[in] media The media info handle
 *
 * @return @c 0 on success, 
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_PERMISSION_DENIED Permission denied
 *
 * @pre This function requires opened connection to content service by media_content_connect().
 * @see media_content_connect()
 */
int media_info_cancel_thumbnail(media_info_h media);

/**
 * @}
 */


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __TIZEN_MEDIA_INFORMATION_H__ */
