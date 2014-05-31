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
 * @addtogroup CAPI_CONTENT_MEDIA_INFO_MODULE
 * @{
 * @file media_info.h
 * @brief This file contains the media info API and related functions to proceed with it. \n
 *        Functions are capabale to insert, clone, delete, get number, content of files from db. \n
 *        Get and set properties and parameters such as storage type, provider, category etc. of media info, \n
 *        handling with thumbnail and updaing media info to db.
 */

/**
 * @brief Inserts media file into the media database.
 * @details This function inserts an media item into the content storage.
 * Normally, inserting a media file in database is done automatically by media server, without calling this function.
 * This function is only called when media server is busy and user needs to get quick result of inserting
 * e.g. Taking a photo while media server is busy and user want to see the quick snapshot of the photo taken.
 * @remark The handle must be released with media_info_destroy() by you.
 *
 * @param[in] path The path to the media file
 * @param[out] info The handle to the media info
 * @return 0 on success, otherwise a negative error value.
 * @retval #MEDIA_CONTENT_ERROR_NONE Successful
 * @pre This function requires opened connection to content service by media_content_connect().
 * @see media_content_connect()
 */
int media_info_insert_to_db (const char *path, media_info_h *info);

/**
 * @brief Insert media files into the media database, asynchronously.
 * @details This function insert an media items into the content storage.
 * Normally, inserting a media file in database is done automatically by media server, without calling this function.
 * This function invokes media_insert_completed_cb callback function.
 * #media_insert_completed_cb will be called when insertion to media database is finished.
 *
 * @param[in] path_array The path array to the media files
 * @param[in] array_length The length of array
 * @param[in] callback The callback to invoke when media items inserted completely
 * @param[in] user_data The user data to be passed to the callback function
 * @return 0 on success, otherwise a negative error value.
 * @retval #MEDIA_CONTENT_ERROR_NONE Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_DB_FAILED DB operation failed
 * @pre This function requires opened connection to content service by media_content_connect().
 * @see media_content_connect()
 * @see media_insert_completed_cb()
 */
int media_info_insert_batch_to_db(const char **path_array,unsigned int array_length, media_insert_completed_cb callback, void *user_data);

/**
 * @brief Insert the burst shot images into the media database, asynchronously.
 * @details This function insert the busrt images into the content storage.
 * #media_insert_burst_shot_completed_cb will be called when insertion to media database is finished.
 *
 * @param[in] path_array The path array to the burst shot images
 * @param[in] array_length The length of array
 * @param[in] callback The callback to invoke when the images inserted completely
 * @param[in] user_data The user data to be passed to the callback function
 * @return 0 on success, otherwise a negative error value.
 * @retval #MEDIA_CONTENT_ERROR_NONE Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_DB_FAILED DB operation failed
 * @pre This function requires opened connection to content service by media_content_connect().
 * @see media_content_connect()
 * @see media_insert_burst_shot_completed_cb()
 */
int media_info_insert_burst_shot_to_db(const char **path_array,unsigned int array_length, media_insert_burst_shot_completed_cb callback, void *user_data);

/**
 * @brief Deletes media file from the media database.
 * @details This function deletes an media item from the content storage.
 * Normally, deleting a media file in database is done automatically by media server, without calling this function.
 * This function is only called when media server is busy and user needs to get quick result of deleting
 *
 * @param[in] media_id The ID to the media file
 * @return 0 on success, otherwise a negative error value.
 * @retval #MEDIA_CONTENT_ERROR_NONE Successful
 * @pre This function requires opened connection to content service by media_content_connect().
 * @see media_content_connect()
 */
int media_info_delete_from_db(const char *media_id);

/**
 * @brief Deletes media files from the media database. The media files to delete can be specified as a condition in a filter.
 * @details This function deletes the media items from the content storage.
 * Normally, deleting  media files in database are done automatically by media server, without calling this function.
 * This function is only called when media server is busy and user needs to get quick result of deleting
 *
 * @param[in] filter The handle to filter
 * @return 0 on success, otherwise a negative error value.
 * @retval #MEDIA_CONTENT_ERROR_NONE Successful
 * @pre This function requires opened connection to content service by media_content_connect().
 * @see media_content_connect()
 */
int media_info_delete_batch_from_db(filter_h filter);


/**
 * @brief Destroys the media info.
 * @details The function frees all resources related to the media info handle. This handle
 * no longer can be used to perform any operation. New media info handle has to
 * be created before next usage.
 *
 * @param[in] media The handle to media info
 * @return 0 on success, otherwise a negative error value.
 * @retval #MEDIA_CONTENT_ERROR_NONE Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @pre Get copy of media_info handle by calling media_info_clone()
 * @see media_info_clone()
 */
int media_info_destroy(media_info_h media);

/**
 * @brief Clones the media info handle.
 *
 * @details This function copies the media info handle from a source to destination.
 * There is no media_info_create() function. The media_info_h is created internally and
 * available through media info foreach function such as media_info_foreach_media_from_db().
 * To use this handle outside of these foreach functions, use this function.
 * @remark The destination handle must be released with media_info_destroy() by you.
 *
 * @param[out] dst A destination handle to media info
 * @param[in] src The source handle to media info
 * @return 0 on success, otherwise a negative error value.
 * @retval #MEDIA_CONTENT_ERROR_NONE Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_OUT_OF_MEMORY Out of memory
 * @see media_info_destroy()
 * @see media_album_foreach_media_from_db()
 * @see media_playlist_foreach_media_from_db()
 * @see media_group_foreach_media_from_db
 * @see media_tag_foreach_media_from_db()
 * @see media_info_foreach_media_from_db()
 * @see media_folder_foreach_media_from_db()
 *
 */
int media_info_clone(media_info_h *dst, media_info_h src);

/**
 * @brief Gets the number of media info for the passed @a filter from the media database.
 *
 * @param[in] filter The handle to filter.
 * @param[out] media_count The count of media
 * @return 0 on success, otherwise a negative error value.
 * @retval #MEDIA_CONTENT_ERROR_NONE Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_DB_FAILED DB operation failed
 * @pre This function requires opened connection to content service by media_content_connect().
 * @see media_content_connect()
 *
 */
int media_info_get_media_count_from_db(filter_h filter, int *media_count);

/**
 * @brief Iterates through media info from the media database.
 * @details This function gets all media info handles meeting the given @a filter. The @a callback function will be invoked for every retrieved media info.
 * If NULL is passed to the @a filter, no filtering is applied.
 * @param[in] filter The handle to media info filter
 * @param[in] callback The callback function to invoke
 * @param[in] user_data The user data to be passed to the callback function
 * @return 0 on success, otherwise a negative error value.
 * @retval #MEDIA_CONTENT_ERROR_NONE Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_OUT_OF_MEMORY Out of memory
 * @pre This function requires opened connection to content service by media_content_connect().
 * @post This function invokes media_info_cb().
 * @see media_content_connect()
 * @see #media_info_cb
 * @see media_info_filter_create()
 *
 */
int media_info_foreach_media_from_db(filter_h filter, media_info_cb callback, void *user_data);

/**
 * @brief Gets the number of media tag for the passed @a filter in the given @a media ID from the media database.
 *
 * @param[in] media_id The ID of media info
 * @param[in] filter The handle to media filter
 * @param[out] tag_count The count of media tag
 * @return 0 on success, otherwise a negative error value.
 * @retval #MEDIA_CONTENT_ERROR_NONE Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_DB_FAILED DB operation failed
 * @pre This function requires opened connection to content service by media_content_connect().
 * @see media_content_connect()
 *
 */
int media_info_get_tag_count_from_db(const char *media_id, filter_h filter, int *tag_count);

/**
 * @brief Iterates through the media tag in the given @a media @a info from the media database.
 * @details This function gets all media tag associated with the given @a media and calls registered callback function for every retrieved media tag.
 * @param[in] media_id The ID of media info
 * @param[in] filter The handle to media filter
 * @param[in] callback The callback function to invoke
 * @param[in] user_data The user data to be passed to the callback function
 * @return 0 on success, otherwise a negative error value.
 * @retval #MEDIA_CONTENT_ERROR_NONE Successful
 * @retval #MEDIA_CONTENT_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @pre This function requires opened connection to content service by media_content_connect().
 * @post This function invokes media_tag_cb().
 * @see media_content_connect()
 * @see #media_tag_cb
 */
int media_info_foreach_tag_from_db(const char *media_id, filter_h filter, media_tag_cb callback, void *user_data);

/**
 * @brief Gets the number of bookmark for the passed @a filter in the given @a media ID from the media database.
 *
 * @param[in] media_id The ID of media info
 * @param[in] filter The handle to media filter
 * @param[out] bookmark_count The count of media tag
 * @return 0 on success, otherwise a negative error value.
 * @retval #MEDIA_CONTENT_ERROR_NONE Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_DB_FAILED DB operation failed
 * @pre This function requires opened connection to content service by media_content_connect().
 * @see media_content_connect()
 *
 */
int media_info_get_bookmark_count_from_db(const char *media_id, filter_h filter, int *bookmark_count);

/**
 * @brief Iterates through the media bookmark in the given @a media @a info from the media database.
 * @details This function gets all media bookmark associated with the given @a media and calls registered callback function for every retrieved media bookmark.
 * @param[in] media_id The ID of media info
 * @param[in] filter The handle to media filter
 * @param[in] callback The callback function to invoke
 * @param[in] user_data The user data to be passed to the callback function
 * @return 0 on success, otherwise a negative error value.
 * @retval #MEDIA_CONTENT_ERROR_NONE Successful
 * @retval #MEDIA_CONTENT_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @pre This function requires opened connection to content service by media_content_connect().
 * @post This function invokes media_bookmark_cb().
 * @see media_content_connect()
 * @see #media_bookmark_cb
 */
int media_info_foreach_bookmark_from_db (const char *media_id, filter_h filter, media_bookmark_cb callback, void *user_data);

/**
 * @brief Gets image metadata for a given media info.
 * @details This function returns an image metadata handle retrieved from the media info.
 *
 * @remark The @a image handle must be released with image_meta_destroy() by you.
 *
 * @param [in] media The handle to media info
 * @param[out] image A handle to image meta
 * @return 0 on success, otherwise a negative error value.
 * @retval #MEDIA_CONTENT_ERROR_NONE Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @see image_meta_destroy()
 */
int media_info_get_image(media_info_h media, image_meta_h *image);

/**
 * @brief Gets video metadata for a given media info.
 * @details This function returns a video metadata handle retrieved from the media info handle.
 *
 * @remark The @a video handle must be released with video_meta_destroy() by you.
 *
 * @param [in] media The handle to media info
 * @param[out] video A handle to the video meta
 * @return 0 on success, otherwise a negative error value.
 * @retval #MEDIA_CONTENT_ERROR_NONE Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_OUT_OF_MEMORY Out of memory
 * @see video_meta_destroy()
 *
 */
int media_info_get_video(media_info_h media, video_meta_h *video);

/**
 * @brief Gets audio metadata for a given media info.
 * @details This function returns an audio metadata handle retrieved from the media info handle.
 *
 * @remark The @a audio handle must be released with audio_meta_destroy() by you.
 *
 * @param [in] media The handle to media info
 * @param[out] audio A handle to the audio meta
 * @return 0 on success, otherwise a negative error value.
 * @retval #MEDIA_CONTENT_ERROR_NONE Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_OUT_OF_MEMORY Out of memory
 * @see audio_meta_destroy()
 */
int media_info_get_audio(media_info_h media, audio_meta_h *audio);

/**
 * @brief Gets ID to media info.
 *
 * @param [in] media_id The ID if media info
 * @param [out] media_id The ID of media tag
 * @return 0 on success, otherwise a negative error value.
 * @retval #MEDIA_CONTENT_ERROR_NONE Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 */
int media_info_get_media_id(media_info_h media, char **media_id);

/**
 * @brief Gets path to media info.
 *
 * @remarks @a path must be released with free() by you.
 *
 * @param[in] media The handle to media info
 * @param[out] path The path of media info
 * @return 0 on success, otherwise a negative error value.
 * @retval #MEDIA_CONTENT_ERROR_NONE Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_OUT_OF_MEMORY Out of memory
 *
 */
int media_info_get_file_path(media_info_h media, char **path);

/**
 * @brief Gets name to media info.
 *
 * @remarks @a name must be released with free() by you.
 *
 * @param[in] media The handle to media info
 * @param[out] name The name of media info
 * @return 0 on success, otherwise a negative error value.
 * @retval #MEDIA_CONTENT_ERROR_NONE Successful
 * @retval #MEDIA_CONTENT_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 *
 */
int media_info_get_display_name(media_info_h media, char **name);

/**
 * @brief Gets media info's content type.
 *
 * @param[in] media The handle to media info
 * @param[out] type The type of media content(#media_content_type_e)
 * @return 0 on success, otherwise a negative error value.
 * @retval #MEDIA_CONTENT_ERROR_NONE Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 *
 */
int media_info_get_media_type(media_info_h media, media_content_type_e *type);

/**
 * @brief Gets name to media info.
 *
 * @remarks @a mime_type must be released with free() by you.
 *
 * @param[in] media The handle to media info
 * @param[out] mime_type The mime type of media info
 * @return 0 on success, otherwise a negative error value.
 * @retval #MEDIA_CONTENT_ERROR_NONE Successful
 * @retval #MEDIA_CONTENT_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 *
 */
int media_info_get_mime_type(media_info_h media, char **mime_type);

/**
 * @brief Gets media file's size.
 *
 * @param[in] media The handle to media info
 * @param[out] size The type of media content
 * @return 0 on success, otherwise a negative error value.
 * @retval #MEDIA_CONTENT_ERROR_NONE Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 *
 */
int media_info_get_size(media_info_h media, unsigned long long *size);

/**
 * @brief Gets added time.
 *
 * @param[in] media The handle to media info
 * @param[out] added_time The added time to DB
 * @return 0 on success, otherwise a negative error value.
 * @retval #MEDIA_CONTENT_ERROR_NONE Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 *
 */
int media_info_get_added_time(media_info_h media, time_t *added_time);

/**
 * @brief Gets media info's date of modification.
 *
 * @param[in] media The handle to media info
 * @param[out] time The date of modification of File. Get from File
 * @return 0 on success, otherwise a negative error value.
 * @retval #MEDIA_CONTENT_ERROR_NONE Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 *
 */
int media_info_get_modified_time(media_info_h media, time_t *time);

/**
 * @brief Gets media info's timeline.
 *
 * @param[in] media The handle to media info
 * @param[out] time The date of timeline.
 * @return 0 on success, otherwise a negative error value.
 * @retval #MEDIA_CONTENT_ERROR_NONE Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 *
 */
int media_info_get_timeline(media_info_h media, time_t* time);

/**
 * @brief Gets the thumbnail to media info.
 *
 * @remarks @a path must be released with free() by you.
 *
 * @param[in] media The handle to media info
 * @param[out] path The path to thumbnail of media info
 * @return 0 on success, otherwise a negative error value.
 * @retval #MEDIA_CONTENT_ERROR_NONE Successful
 * @retval #MEDIA_CONTENT_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 *
 */
int media_info_get_thumbnail_path(media_info_h media, char **path);

/**
 * @brief Gets the description to media info.
 *
 * @remarks @a description must be released with free() by you.
 *
 * @param[in] media The handle to media info
 * @param[out] description The description of media info
 * @return 0 on success, otherwise a negative error value.
 * @retval #MEDIA_CONTENT_ERROR_NONE Successful
 * @retval #MEDIA_CONTENT_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 *
 */
int media_info_get_description(media_info_h media, char **description);

/**
 * @brief Gets media info's longitude.
 *
 * @param[in] media The handle to media info
 * @param[out] longitude The longitude of media info
 * @return 0 on success, otherwise a negative error value.
 * @retval #MEDIA_CONTENT_ERROR_NONE Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 *
 */
int media_info_get_longitude(media_info_h media, double* longitude);

/**
 * @brief Gets media info's latitude.
 *
 * @param[in] media The handle to media info
 * @param[out] latitude The latitude of media info
 * @return 0 on success, otherwise a negative error value.
 * @retval #MEDIA_CONTENT_ERROR_NONE Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 *
 */
int media_info_get_latitude(media_info_h media, double* latitude);

/**
 * @brief Gets media info's altitude of modification.
 *
 * @param[in] media The handle to media info
 * @param[out] altitude The altitude of media info
 * @return 0 on success, otherwise a negative error value.
 * @retval #MEDIA_CONTENT_ERROR_NONE Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 *
 */
int media_info_get_altitude(media_info_h media, double* altitude);

/**
 * @brief Gets media info's weather.
 *
 * @param[in] media The handle to media info
 * @param[out] weater The weather of media info
 * @return 0 on success, otherwise a negative error value.
 * @retval #MEDIA_CONTENT_ERROR_NONE Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 *
 */
int media_info_get_weather(media_info_h media, char **weather);

/**
 * @brief Gets media info's rating.
 *
 * @param[in] media The handle to media info
 * @param[out] rating The rating of media info
 * @return 0 on success, otherwise a negative error value.
 * @retval #MEDIA_CONTENT_ERROR_NONE Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 *
 */
int media_info_get_rating(media_info_h media, int *rating);

/**
 * @brief Gets the given media info's favorite status.
 *
 * @param [in] media The handle to media info
 * @param [out] favorite The media favorite status(non zero if favorite, 0 if not favorite)
 * @return 0 on success, otherwise a negative error value.
 * @retval #MEDIA_CONTENT_ERROR_NONE Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 *
 */
int media_info_get_favorite(media_info_h media, bool* favorite);

/**
 * @brief Gets the author to media info.
 *
 * @remarks @a author must be released with free() by you.
 *
 * @param[in] media The handle to media info
 * @param[out] author The author of media info
 * @return 0 on success, otherwise a negative error value.
 * @retval #MEDIA_CONTENT_ERROR_NONE Successful
 * @retval #MEDIA_CONTENT_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 *
 */
int media_info_get_author(media_info_h media, char **author);

/**
 * @brief Gets the provider to media info.
 *
 * @remarks @a provider must be released with free() by you.
 *
 * @param[in] media The handle to media info
 * @param[out] provider The provider of media info
 * @return 0 on success, otherwise a negative error value.
 * @retval #MEDIA_CONTENT_ERROR_NONE Successful
 * @retval #MEDIA_CONTENT_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 *
 */
int media_info_get_provider(media_info_h media, char **provider);

/**
 * @brief Gets the content name to media info.
 *
 * @remarks @a content_name must be released with free() by you.
 *
 * @param[in] media The handle to media info
 * @param[out] content_name The content name of media info
 * @return 0 on success, otherwise a negative error value.
 * @retval #MEDIA_CONTENT_ERROR_NONE Successful
 * @retval #MEDIA_CONTENT_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 *
 */
int media_info_get_content_name(media_info_h media, char **content_name);

/**
 * @brief Gets the title to media info.
 *
 * @remarks @a title must be released with free() by you.
 *
 * @param[in] media The handle to media info
 * @param[out] title The title of media info
 * @return 0 on success, otherwise a negative error value.
 * @retval #MEDIA_CONTENT_ERROR_NONE Successful
 * @retval #MEDIA_CONTENT_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 *
 */
int media_info_get_title(media_info_h media, char **title);

/**
 * @brief Gets the provider to media info.
 *
 * @remarks @a category must be released with free() by you.
 *
 * @param[in] media The handle to media info
 * @param[out] category The category of media info
 * @return 0 on success, otherwise a negative error value.
 * @retval #MEDIA_CONTENT_ERROR_NONE Successful
 * @retval #MEDIA_CONTENT_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 *
 */
int media_info_get_category(media_info_h media, char **category);

/**
 * @brief Gets the location_tag to media info.
 *
 * @remarks @a location_tag must be released with free() by you.
 *
 * @param[in] media The handle to media info
 * @param[out] location_tag The location of media info
 * @return 0 on success, otherwise a negative error value.
 * @retval #MEDIA_CONTENT_ERROR_NONE Successful
 * @retval #MEDIA_CONTENT_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 *
 */
int media_info_get_location_tag(media_info_h media, char **location_tag);

/**
 * @brief Gets the age_rating to media info.
 *
 * @remarks @a age_rating must be released with free() by you.
 *
 * @param[in] media The handle to media info
 * @param[out] age_rating The age rating of media info
 * @return 0 on success, otherwise a negative error value.
 * @retval #MEDIA_CONTENT_ERROR_NONE Successful
 * @retval #MEDIA_CONTENT_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 *
 */
int media_info_get_age_rating(media_info_h media, char **age_rating);

/**
 * @brief Gets the keyword to media info.
 *
 * @remarks @a keyword must be released with free() by you.
 *
 * @param[in] media The handle to media info
 * @param[out] keyword The keyword of media info
 * @return 0 on success, otherwise a negative error value.
 * @retval #MEDIA_CONTENT_ERROR_NONE Successful
 * @retval #MEDIA_CONTENT_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 *
 */
int media_info_get_keyword(media_info_h media, char **keyword);

/**
 * @brief Checks whether the media is protected via drm.
 *
 * @param[in] media The handle to media info
 * @param[out] is_drm /@a true if the drm media,
 *                    /@a false if not drm.
 * @return 0 on success, otherwise a negative error value.
 * @retval #MEDIA_CONTENT_ERROR_NONE Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 *
 */
int media_info_is_drm(media_info_h media, bool *is_drm);

/**
 * @brief Gets media info's storage_type.
 *
 * @param[in] media The handle to media info
 * @param[out] storage_type The storage type of media info
 * @return 0 on success, otherwise a negative error value.
 * @retval #MEDIA_CONTENT_ERROR_NONE Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 *
 */
int media_info_get_storage_type(media_info_h media, media_content_storage_e *storage_type);

/**
 * @brief Gets the media info from the media database.
 *
 * @details This function creates a new media handle from the media database by the given media_id.
 * media info will be created, which is filled with info information.
 *
 * @remarks @a media must be released with media_tag_destroy() by you.
 *
 * @param[in] media_id The ID of media info
 * @param[out] media The media handle associated with the media ID
 * @return 0 on success, otherwise a negative error value.
 * @retval #MEDIA_CONTENT_ERROR_NONE Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @pre This function requires opened connection to content service by media_content_connect().
 * @see media_content_connect()
 * @see media_info_destroy()
 *
 */
int media_info_get_media_from_db(const char *media_id, media_info_h *media);

/**
 * @brief Sets display name to media info.
 *
 * @param[in] media The handle to media info
 * @param[in] display_name The display name of media info
 * @return 0 on success, otherwise a negative error value.
 * @retval #MEDIA_CONTENT_ERROR_NONE Successful
 * @retval #MEDIA_CONTENT_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @post media_info_update_to_db()
 *
 */
int media_info_set_display_name(media_info_h media, const char *display_name);

/**
 * @brief Sets description to media info.
 *
 * @param[in] media The handle to media info
 * @param[in] description The description of media info
 * @return 0 on success, otherwise a negative error value.
 * @retval #MEDIA_CONTENT_ERROR_NONE Successful
 * @retval #MEDIA_CONTENT_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @post media_info_update_to_db()
 *
 */
int media_info_set_description(media_info_h media, const char *description);

/**
 * @brief Sets longitude to media info.
 *
 * @param[in] media The handle to media info
 * @param[in] longitude The longitude of media info
 * @return 0 on success, otherwise a negative error value.
 * @retval #MEDIA_CONTENT_ERROR_NONE Successful
 * @retval #MEDIA_CONTENT_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @post media_info_update_to_db()
 *
 */
int media_info_set_longitude(media_info_h media, double longitude);

/**
 * @brief Sets latitude to media info.
 *
 * @param[in] media The handle to media info
 * @param[in] latitude The latitude of media info
 * @return 0 on success, otherwise a negative error value.
 * @retval #MEDIA_CONTENT_ERROR_NONE Successful
 * @retval #MEDIA_CONTENT_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @post media_info_update_to_db()
 *
 */
int media_info_set_latitude(media_info_h media, double latitude);

/**
 * @brief Sets altitude to media info.
 *
 * @param[in] media The handle to media info
 * @param[in] altitude The altitude of media info
 * @return 0 on success, otherwise a negative error value.
 * @retval #MEDIA_CONTENT_ERROR_NONE Successful
 * @retval #MEDIA_CONTENT_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @post media_info_update_to_db()
 *
 */
int media_info_set_altitude(media_info_h media, double altitude);

/**
 * @brief Sets weather to media info.
 *
 * @param[in] media The handle to media info
 * @param[in] weather The weather of media info
 * @return 0 on success, otherwise a negative error value.
 * @retval #MEDIA_CONTENT_ERROR_NONE Successful
 * @retval #MEDIA_CONTENT_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @post media_info_update_to_db()
 *
 */
int media_info_set_weather(media_info_h media, const char *weather);

/**
 * @brief Sets rating to media info.
 *
 * @param[in] media The handle to media info
 * @param[in] rating The rating of media info
 * @return 0 on success, otherwise a negative error value.
 * @retval #MEDIA_CONTENT_ERROR_NONE Successful
 * @retval #MEDIA_CONTENT_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @post media_info_update_to_db()
 *
 */
int media_info_set_rating(media_info_h media, int rating);

/**
 * @brief Sets favorite to media info.
 *
 * @param[in] media The handle to media info
 * @param[in] favorite The favorite of media info
 * @return 0 on success, otherwise a negative error value.
 * @retval #MEDIA_CONTENT_ERROR_NONE Successful
 * @retval #MEDIA_CONTENT_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 *
 */
int media_info_set_favorite(media_info_h media, bool favorite);

/**
 * @brief Sets author to media info.
 *
 * @param[in] media The handle to media info
 * @param[in] author The author of media info
 * @return 0 on success, otherwise a negative error value.
 * @retval #MEDIA_CONTENT_ERROR_NONE Successful
 * @retval #MEDIA_CONTENT_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 *
 */
int media_info_set_author(media_info_h media, const char *author);

/**
 * @brief Sets provider to media info.
 *
 * @param[in] media The handle to media info
 * @param[in] provider The provider of media info
 * @return 0 on success, otherwise a negative error value.
 * @retval #MEDIA_CONTENT_ERROR_NONE Successful
 * @retval #MEDIA_CONTENT_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 *
 */
int media_info_set_provider(media_info_h media, const char *provider);

/**
 * @brief Sets content name to media info.
 *
 * @param[in] media The handle to media info
 * @param[in] content_name The content name of media info
 * @return 0 on success, otherwise a negative error value.
 * @retval #MEDIA_CONTENT_ERROR_NONE Successful
 * @retval #MEDIA_CONTENT_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @post media_info_update_to_db()
 *
 */
int media_info_set_content_name(media_info_h media, const char *content_name);

/**
 * @brief Sets category to media info.
 *
 * @param[in] media The handle to media info
 * @param[in] category The category of media info
 * @return 0 on success, otherwise a negative error value.
 * @retval #MEDIA_CONTENT_ERROR_NONE Successful
 * @retval #MEDIA_CONTENT_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @post media_info_update_to_db()
 *
 */
int media_info_set_category(media_info_h media, const char *category);

/**
 * @brief Sets location tag to media info.
 *
 * @param[in] media The handle to media info
 * @param[in] location_tag The location of media info
 * @return 0 on success, otherwise a negative error value.
 * @retval #MEDIA_CONTENT_ERROR_NONE Successful
 * @retval #MEDIA_CONTENT_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @post media_info_update_to_db()
 *
 */
int media_info_set_location_tag(media_info_h media, const char *location_tag);

/**
 * @brief Sets age rating to media info.
 *
 * @param[in] media The handle to media info
 * @param[in] age_rating The age rating of media info
 * @return 0 on success, otherwise a negative error value.
 * @retval #MEDIA_CONTENT_ERROR_NONE Successful
 * @retval #MEDIA_CONTENT_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @post media_info_update_to_db()
 *
 */
int media_info_set_age_rating(media_info_h media, const char *age_rating);

/**
 * @brief Sets keyword to media info.
 *
 * @param[in] media The handle to media info
 * @param[in] keyword The keyword of media info
 * @return 0 on success, otherwise a negative error value.
 * @retval #MEDIA_CONTENT_ERROR_NONE Successful
 * @retval #MEDIA_CONTENT_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @post media_info_update_to_db()
 *
 */
int media_info_set_keyword(media_info_h media, const char *keyword);

/**
 * @brief Updates the media info to the media database.
 *
 * @details The function updates the given media info in the media database. The function should be called after any change in media, to be updated to the media
 * database. For example, after using media_info_set_display_name() for setting the name of the media, media_info_update_to_db() function should be called so as to update
 * the given media info attibutes in the media database.
 *
 * @param[in] media The handle to media info
 * @return 0 on success, otherwise a negative error value.
 * @retval #MEDIA_CONTENT_ERROR_NONE Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @pre This function requires opened connection to content service by media_content_connect().
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
 *
 */
int media_info_update_to_db(media_info_h media);

/**
 * @brief Refresh the metadata of media to media database.
 *
 * @param[in] media_id The ID of media info
 * @return 0 on success, otherwise a negative error value.
 * @retval #MEDIA_CONTENT_ERROR_NONE Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_DB_FAILED DB operation failed
 * @pre This function requires opened connection to content service by media_content_connect().
 * @see media_content_connect()
 *
 */
int media_info_refresh_metadata_to_db(const char *media_id);

/**
 * @brief Sets added_time to media info.
 *
 * @param[in] media The handle to media info
 * @param[in] added_time The added time of media info
 * @return 0 on success, otherwise a negative error value.
 * @retval #MEDIA_CONTENT_ERROR_NONE Successful
 * @retval #MEDIA_CONTENT_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @post media_info_update_to_db()
 *
 */
int media_info_set_added_time(media_info_h media, time_t added_time);

/**
 * @brief Moves the media info to the given destination path in the media database.
 *
 * @param[in] media The handle to media info
 * @param[in] dst_path The path of destination
 * @return 0 on success, otherwise a negative error value.
 * @retval #MEDIA_CONTENT_ERROR_NONE Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_DB_FAILED DB operation failed
 * @pre This function requires opened connection to content service by media_content_connect().
 * @see media_content_connect()
 *
 */
int media_info_move_to_db(media_info_h media, const char* dst_path);

/**
 * @brief Creates a thumbnail image for given the media, asynchronously.
 * @details This function creates an thumbnail image for given media item and and calls registered callback function for completion of creating the thumbnail.
 * If there already exist a thumbnail for given media, then the path of thumbnail will be return in callback function.
 *
 * @param[in] media The handle to media info
 * @param[in] callback The callback function to invoke
 * @param[in] user_data The user data to be passed to the callback function
 * @return 0 on success, otherwise a negative error value.
 * @retval #MEDIA_CONTENT_ERROR_NONE Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter ( Especially, if the request is duplicated, this error returns. )
 * @retval #MEDIA_CONTENT_ERROR_DB_FAILED DB operation failed
 * @pre This function requires opened connection to content service by media_content_connect().
 * @see media_content_connect()
 */
int media_info_create_thumbnail(media_info_h media, media_thumbnail_completed_cb callback, void *user_data);


/**
 * @brief Cancel to creates a thumbnail image for given the media.
 *
 * @param[in] media The handle to media info
 * @return 0 on success, otherwise a negative error value.
 * @retval #MEDIA_CONTENT_ERROR_NONE Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
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
