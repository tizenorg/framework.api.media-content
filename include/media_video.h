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



#ifndef __TIZEN_VIDEO_META_H__
#define __TIZEN_VIDEO_META_H__

#include <media_content_type.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/**
 * @addtogroup CAPI_CONTENT_MEDIA_VIDEO_META_MODULE
 * @{
 *
 * @file media_video.h
 * @brief This file contains the video metadata API and related functions to proceed with video metadata. \n
 *        Functions include clonning and destroying video metadata, getting video metadata as width, height, \n
 *        album, genre, played parameters etc. and updating video to db.
 */

/**
 * @brief Clones video metadata.
 * @details This function copies the video metadata handle from a source to
 * destination.

 * @remark The destination handle must be released with video_meta_destroy() by you.
 *
 * @param [out] dst A destination handle to video metadata
 * @param [in] src The source handle to video metadata
 * @return 0 on success, otherwise a negative error value.
 * @retval #MEDIA_CONTENT_ERROR_NONE Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_OUT_OF_MEMORY Out of memory
 * @see video_meta_destroy()
 */
int video_meta_clone(video_meta_h *dst, video_meta_h src);

/**
 * @brief Destroys video metadata.
 * @details Function frees all resources related to video metadata handle. This handle
 * no longer can be used to perform any operation. A new handle has to
 * be created before the next use.
 *
 * @param [in] video The handle to video metadata
 * @return 0 on success, otherwise a negative error value.
 * @retval #MEDIA_CONTENT_ERROR_NONE Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @pre Get copy of video metadata handle by calling video_meta_clone()
 * @see video_meta_clone()
 *
 */
int video_meta_destroy(video_meta_h video);

/**
 * @brief Gets id of media of given video metadata.
 *
 * @remarks @a media id must be released with free() by you.
 *
 * @param [in] video The handle to video metadata
 * @param [out] media_id The id of the video
 * @return 0 on success, otherwise a negative error value.
 * @retval #MEDIA_CONTENT_ERROR_NONE Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_OUT_OF_MEMORY Out of memory
 */
int video_meta_get_media_id(video_meta_h video, char **media_id);

/**
 * @brief Gets the video's title.
 *
 * @remarks @a title must be released with free() by you.
 *
 * @param [in] video The handle to video metadata
 * @param [out] title The title of video metadata
 * @return 0 on success, otherwise a negative error value.
 * @retval #MEDIA_CONTENT_ERROR_NONE Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_OUT_OF_MEMORY Out of memory
 */
int video_meta_get_title(video_meta_h video, char **title);

/**
 * @brief Gets the video's album.
 * If the value is an empty string, the method returns "Unknown".
 *
 * @remarks @a album must be released with free() by you.
 *
 * @param [in] video The handle to video metadata
 * @param [out] album The video album or NULL
 * @return 0 on success, otherwise a negative error value.
 * @retval #MEDIA_CONTENT_ERROR_NONE Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_OUT_OF_MEMORY Out of memory
 */
int video_meta_get_album(video_meta_h video, char **album);

/**
 * @brief Gets the video's artist.
 * If the value is an empty string, the method returns "Unknown".
 *
 * @remarks @a artist must be released with free() by you.
 *
 * @param [in] video The handle to video metadata
 * @param [out] artist The artist of video metadata
 * @return 0 on success, otherwise a negative error value.
 * @retval #MEDIA_CONTENT_ERROR_NONE Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_OUT_OF_MEMORY Out of memory
 */
int video_meta_get_artist(video_meta_h video, char **artist);

/**
 * @brief Gets the video's album_artist.
 * If the value is an empty string, the method returns "Unknown".
 *
 * @remarks @a album_artist must be released with free() by you.
 *
 * @param [in] video The handle to video metadata
 * @param [out] album_artist The album_artist of video metadata
 * @return 0 on success, otherwise a negative error value.
 * @retval #MEDIA_CONTENT_ERROR_NONE Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_OUT_OF_MEMORY Out of memory
 */
int video_meta_get_album_artist(video_meta_h video, char **album_artist);

/**
 * @brief Gets the video's genre.
 * If the value is an empty string, the method returns "Unknown".
 *
 * @remarks @a genre must be released with free() by you.
 *
 * @param [in] video The handle to video metadata
 * @param [out] genre The genre of video metadata
 * @return 0 on success, otherwise a negative error value.
 * @retval #MEDIA_CONTENT_ERROR_NONE Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_OUT_OF_MEMORY Out of memory
 */
int video_meta_get_genre(video_meta_h video, char **genre);

/**
 * @brief Gets the video's composer.
 * If the value is an empty string, the method returns "Unknown".
 *
 * @remarks @a composer must be released with free() by you.
 *
 * @param [in] video The handle to video metadata
 * @param [out] composer The composer of video metadata
 * @return 0 on success, otherwise a negative error value.
 * @retval #MEDIA_CONTENT_ERROR_NONE Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_OUT_OF_MEMORY Out of memory
 */
int video_meta_get_composer(video_meta_h video, char **composer);

/**
 * @brief Gets the video's year.
 * If the value is an empty string, the method returns "Unknown".
 *
 * @remarks @a year must be released with free() by you.
 *
 * @param [in] video The handle to video metadata
 * @param [out] year The year of video metadata
 * @return 0 on success, otherwise a negative error value.
 * @retval #MEDIA_CONTENT_ERROR_NONE Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_OUT_OF_MEMORY Out of memory
 */
int video_meta_get_year(video_meta_h video, char **year);

/**
 * @brief Gets the video's recorded_date.
 *
 * @remarks @a recorded_date must be released with free() by you.
 *
 * @param [in] video The handle to video metadata
 * @param [out] recorded_date The recorded_date of video metadata
 * @return 0 on success, otherwise a negative error value.
 * @retval #MEDIA_CONTENT_ERROR_NONE Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_OUT_OF_MEMORY Out of memory
 */
int video_meta_get_recorded_date(video_meta_h video, char **recorded_date);

/**
 * @brief Gets the video's copyright.
 *
 * @remarks @a copyright must be released with free() by you.
 *
 * @param [in] video The handle to video metadata
 * @param [out] copyright The copyright of video metadata
 * @return 0 on success, otherwise a negative error value.
 * @retval #MEDIA_CONTENT_ERROR_NONE Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_OUT_OF_MEMORY Out of memory
 */
int video_meta_get_copyright(video_meta_h video, char **copyright);

/**
 * @brief Gets the video's track number.
 * If the value is an empty string, the method returns "Unknown".
 *
 * @remarks @a track_num must be released with free() by you.
 *
 * @param [in] video The handle to video metadata
 * @param [out] track_num The track number of video metadata
 * @return 0 on success, otherwise a negative error value.
 * @retval #MEDIA_CONTENT_ERROR_NONE Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_OUT_OF_MEMORY Out of memory
 */
int video_meta_get_track_num(video_meta_h video, char **track_num);

/**
 * @brief Gets the video's bit rate.
 *
 * @remarks @a bit_rate must be released with free() by you.
 *
 * @param [in] video The handle to video metadata
 * @param [out] bit_rate The bit rate of video metadata
 * @return 0 on success, otherwise a negative error value.
 * @retval #MEDIA_CONTENT_ERROR_NONE Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_OUT_OF_MEMORY Out of memory
 */
int video_meta_get_bit_rate(video_meta_h video, int *bit_rate);

/**
 * @brief Gets duration of video metadata.
 *
 * @param [in] video The handle to video metadata
 * @param [out] duration The video duration in milliseconds
 * @return 0 on success, otherwise a negative error value.
 * @retval #MEDIA_CONTENT_ERROR_NONE Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 */
int video_meta_get_duration(video_meta_h video, int *duration);

/**
 * @brief Gets the video's width in pixels.
 *
 * @param [in] video The handle to video metadata
 * @param [out] width The video width in pixels
 * @return 0 on success, otherwise a negative error value.
 * @retval #MEDIA_CONTENT_ERROR_NONE Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 */
int video_meta_get_width(video_meta_h video, int *width);

/**
 * @brief Gets the video's height in pixels.
 *
 * @param [in] video The handle to video metadata
 * @param [out] height The video height in pixels
 * @return 0 on success, otherwise a negative error value.
 * @retval #MEDIA_CONTENT_ERROR_NONE Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 */
int video_meta_get_height(video_meta_h video, int *height);

/**
 * @brief Gets the video's played count.
 *
 * @param [in] video The handle to video metadata
 * @param [out] played_count The number of played
 * @return 0 on success, otherwise a negative error value.
 * @retval #MEDIA_CONTENT_ERROR_NONE Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 */
int video_meta_get_played_count(video_meta_h video, int *played_count);

/**
 * @brief Gets the video's time last played parameter.
 *
 * @param [in] video The handle to video metadata
 * @param [out] played_time The time last played in the video
 * @return 0 on success, otherwise a negative error value.
 * @retval #MEDIA_CONTENT_ERROR_NONE Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 */
int video_meta_get_played_time(video_meta_h video, time_t *played_time);

/**
 * @brief Gets the video's position played parameter.
 * @details Function returns video's elapsed playback time parameter as period
 * starting from the beginning of the movie.
 *
 * @param [in] video The handle to video metadata
 * @param [out] played_position The position from the beginning of the video (in milliseconds)
 * @return 0 on success, otherwise a negative error value.
 * @retval #MEDIA_CONTENT_ERROR_NONE Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 */
int video_meta_get_played_position(video_meta_h video, int *played_position);

/**
 * @brief Sets the video's played count.
 *
 * @param [in] video The handle to video metadata
 * @param [in] played_count The number of played
 * @return 0 on success, otherwise a negative error value.
 * @retval #MEDIA_CONTENT_ERROR_NONE Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @post video_meta_update_to_db()
 */
int video_meta_set_played_count(video_meta_h video, int played_count);

/**
 * @brief Sets the video's time last played parameter.
 *
 * @param [in] video The handle to video metadata
 * @param [in] played_time The time last played in the video
 * @return 0 on success, otherwise a negative error value.
 * @retval #MEDIA_CONTENT_ERROR_NONE Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @post video_meta_update_to_db()
 */
int video_meta_set_played_time(video_meta_h video, time_t played_time);

/**
 * @brief Sets the video's position played parameter.
 * @details Function returns video's elapsed playback time parameter as period
 * starting from the beginning of the movie.
 *
 * @param [in] video The handle to video metadata
 * @param [in] played_position The position from the beginning of the video (in milliseconds)
 * @return 0 on success, otherwise a negative error value.
 * @retval #MEDIA_CONTENT_ERROR_NONE Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @post video_meta_update_to_db()
 */
int video_meta_set_played_position(video_meta_h video, int played_position);

/**
 * @brief Updates the video to the media database.
 *
 * @details The function updates the given video meta in the media database. The function should be called after any change in video attributes, to be updated to the media
 * database. For example, after using video_meta_get_played_time() for setting the played time of the video, video_meta_update_to_db() function should be called so as to update
 * the given video attibutes in the media database.
 *
 * @param[in] image The handle to image
 * @return 0 on success, otherwise a negative error value.
 * @retval #MEDIA_CONTENT_ERROR_NONE Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @pre This function requires opened connection to content service by media_content_connect().
 * @see media_content_connect()
 * @see video_meta_set_played_time()
 * @see video_meta_set_played_count()
 * @see video_meta_set_played_position()
 */
int video_meta_update_to_db(video_meta_h video);

/**
 *@}
 */

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /*__TIZEN_VIDEO_META_H__*/
