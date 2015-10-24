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
 * @file media_video.h
 * @brief This file contains the video metadata API and related functions to proceed with video metadata. \n
 *        Functions include cloning and destroying video metadata, getting video metadata such as width, height, \n
 *        album, genre, played parameters etc. and updating video to DB.
 */

/**
 * @addtogroup CAPI_CONTENT_MEDIA_VIDEO_META_MODULE
 * @{
 */

/**
 * @brief Clones the video metadata.
 * @details This function copies the video metadata handle from a source to 
 *          destination.
 *
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 * @remarks You must release the destination handle using video_meta_destroy().
 *
 * @param[out] dst The destination handle to the video metadata
 * @param[in]  src The source handle to the video metadata
 *
 * @return @c 0 on success, 
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #MEDIA_CONTENT_ERROR_PERMISSION_DENIED Permission denied
 *
 * @see video_meta_destroy()
 */
int video_meta_clone(video_meta_h *dst, video_meta_h src);

/**
 * @brief Destroys the video metadata.
 * @details This function frees all resources related to the video metadata handle. This handle
 *          no longer can be used to perform any operation. A new handle has to
 *          be created before the next use.
 *
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 * @param[in] video The video metadata handle
 *
 * @return @c 0 on success, 
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_PERMISSION_DENIED Permission denied
 *
 * @pre Get copy of video metadata handle by calling video_meta_clone().
 *
 * @see video_meta_clone()
 */
int video_meta_destroy(video_meta_h video);

/**
 * @brief Gets the ID of the media of the given video metadata.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 * @remarks You must release @a media_id using free().
 *
 * @param[in]  video    The video metadata handle
 * @param[out] media_id The ID of the video
 *
 * @return @c 0 on success, 
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_OUT_OF_MEMORY     Out of memory
 * @retval #MEDIA_CONTENT_ERROR_PERMISSION_DENIED Permission denied
 */
int video_meta_get_media_id(video_meta_h video, char **media_id);

/**
 * @brief Gets the video's album.
 * @details If the value is an empty string, the method returns "Unknown".
 *
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 * @remarks You must release @a album using free().
 *
 * @param[in]  video The video metadata handle
 * @param[out] album The video album or @c NULL
 *
 * @return @c 0 on success, 
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_OUT_OF_MEMORY     Out of memory
 * @retval #MEDIA_CONTENT_ERROR_PERMISSION_DENIED Permission denied
 */
int video_meta_get_album(video_meta_h video, char **album);

/**
 * @brief Gets the video artist.
 * @details If the value is an empty string, the method returns "Unknown".
 *
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 * @remarks You must release @a artist using free().
 *
 * @param[in]  video  The video metadata handle
 * @param[out] artist The artist of the video metadata
 *
 * @return @c 0 on success, 
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_OUT_OF_MEMORY     Out of memory
 * @retval #MEDIA_CONTENT_ERROR_PERMISSION_DENIED Permission denied
 */
int video_meta_get_artist(video_meta_h video, char **artist);

/**
 * @brief Gets the video album artist.
 * @details If the value is an empty string, the method returns "Unknown".
 *
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 * @remarks You must release @a album_artist using free().
 *
 * @param[in]  video        The video metadata handle
 * @param[out] album_artist The album artist of the video metadata
 *
 * @return @c 0 on success, 
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_OUT_OF_MEMORY     Out of memory
 * @retval #MEDIA_CONTENT_ERROR_PERMISSION_DENIED Permission denied
 */
int video_meta_get_album_artist(video_meta_h video, char **album_artist);

/**
 * @brief Gets the video genre.
 * @details If the value is an empty string, the method returns "Unknown".
 *
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 * @remarks You must release @a genre using free().
 *
 * @param[in]  video The video metadata handle
 * @param[out] genre The genre of the video metadata
 *
 * @return @c 0 on success, 
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_OUT_OF_MEMORY     Out of memory
 * @retval #MEDIA_CONTENT_ERROR_PERMISSION_DENIED Permission denied
 */
int video_meta_get_genre(video_meta_h video, char **genre);

/**
 * @brief Gets the video composer.
 * @details If the value is an empty string, the method returns "Unknown".
 *
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 * @remarks You must release @a composer using free().
 *
 * @param[in]  video    The video metadata handle
 * @param[out] composer The composer of the video metadata
 *
 * @return @c 0 on success, 
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_OUT_OF_MEMORY     Out of memory
 * @retval #MEDIA_CONTENT_ERROR_PERMISSION_DENIED Permission denied
 */
int video_meta_get_composer(video_meta_h video, char **composer);

/**
 * @brief Gets the year of the video.
 * @details If the value is an empty string, the method returns "Unknown".
 *
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 * @remarks You must release @a year using free().
 *
 * @param[in]  video The video metadata handle
 * @param[out] year  The year of the video metadata
 *
 * @return @c 0 on success, 
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_OUT_OF_MEMORY     Out of memory
 * @retval #MEDIA_CONTENT_ERROR_PERMISSION_DENIED Permission denied
 */
int video_meta_get_year(video_meta_h video, char **year);

/**
 * @brief Gets the recorded date of the video.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 * @remarks You must release @a recorded_date using free().
 *
 * @param[in]  video         The video metadata handle
 * @param[out] recorded_date The recorded date of the video metadata
 *
 * @return @c 0 on success, 
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_OUT_OF_MEMORY     Out of memory
 * @retval #MEDIA_CONTENT_ERROR_PERMISSION_DENIED Permission denied
 */
int video_meta_get_recorded_date(video_meta_h video, char **recorded_date);

/**
 * @brief Gets the video copyright.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 * @remarks You must release @a copyright using free().
 *
 * @param[in]  video     The video metadata handle
 * @param[out] copyright The copyright of the video metadata
 *
 * @return @c 0 on success, 
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_OUT_OF_MEMORY     Out of memory
 * @retval #MEDIA_CONTENT_ERROR_PERMISSION_DENIED Permission denied
 */
int video_meta_get_copyright(video_meta_h video, char **copyright);

/**
 * @brief Gets the track number of the video.
 * @details If the value is an empty string, the method returns "Unknown".
 *
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 * @remarks You must release @a track_num using free().
 *
 * @param[in]  video     The video metadata handle
 * @param[out] track_num The track number of the video metadata
 *
 * @return @c 0 on success, 
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_OUT_OF_MEMORY     Out of memory
 * @retval #MEDIA_CONTENT_ERROR_PERMISSION_DENIED Permission denied
 */
int video_meta_get_track_num(video_meta_h video, char **track_num);

/**
 * @brief Gets the video bit rate.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 * @remarks You must release @a bit_rate using free().
 *
 * @param[in]  video     The video metadata handle
 * @param[out] bit_rate  The bit rate of the video metadata
 *
 * @return @c 0 on success, 
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_OUT_OF_MEMORY     Out of memory
 * @retval #MEDIA_CONTENT_ERROR_PERMISSION_DENIED Permission denied
 */
int video_meta_get_bit_rate(video_meta_h video, int *bit_rate);

/**
 * @brief Gets the duration of video metadata.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 * @param[in]  video    The video metadata handle
 * @param[out] duration The video duration in milliseconds
 *
 * @return @c 0 on success, 
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_PERMISSION_DENIED Permission denied
 */
int video_meta_get_duration(video_meta_h video, int *duration);

/**
 * @brief Gets the video width in pixels.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 * @param[in]  video The video metadata handle
 * @param[out] width The video width in pixels
 *
 * @return @c 0 on success, 
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_PERMISSION_DENIED Permission denied
 */
int video_meta_get_width(video_meta_h video, int *width);

/**
 * @brief Gets the video height in pixels.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 * @param[in]  video  The video metadata handle
 * @param[out] height The video height in pixels
 *
 * @return @c 0 on success, 
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_PERMISSION_DENIED Permission denied
 */
int video_meta_get_height(video_meta_h video, int *height);

/**
 * @deprecated Deprecated since 2.4. [Use media_info_get_played_count() instead]
 * @brief Gets the played count of the video.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 * @param[in]  video        The video metadata handle
 * @param[out] played_count The number of played
 *
 * @return @c 0 on success, 
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_PERMISSION_DENIED Permission denied
 */
int video_meta_get_played_count(video_meta_h video, int *played_count);

/**
 * @deprecated Deprecated since 2.4. [Use media_info_get_played_time() instead]
 * @brief Gets the last played time parameter of the video.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 * @param[in]  video       The video metadata handle
 * @param[out] played_time The time last played in the video
 *
 * @return @c 0 on success, 
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_PERMISSION_DENIED Permission denied
 */
int video_meta_get_played_time(video_meta_h video, time_t *played_time);

/**
 * @deprecated Deprecated since 2.4.
 * @brief Gets the position played parameter of the video.
 * @details This function returns the elapsed playback time parameter of the video as period
 *          starting from the beginning of the movie.
 *
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 * @param[in]  video           The video metadata handle
 * @param[out] played_position The position from the beginning of the video (in milliseconds)
 *
 * @return @c 0 on success, 
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_PERMISSION_DENIED Permission denied
 */
int video_meta_get_played_position(video_meta_h video, int *played_position);

/**
 * @deprecated Deprecated since 2.4. [Use media_info_increase_played_count() instead]
 * @brief Sets the played count of the video.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 * @param[in] video        The video metadata handle
 * @param[in] played_count The number of played
 *
 * @return @c 0 on success, 
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_PERMISSION_DENIED Permission denied
 *
 * @post video_meta_update_to_db().
 */
int video_meta_set_played_count(video_meta_h video, int played_count);

/**
 * @deprecated Deprecated since 2.4. [Use media_info_set_played_time() instead]
 * @brief Sets the time last played parameter of the video.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 * @param[in] video       The video metadata handle
 * @param[in] played_time The time last played in the video
 *
 * @return @c 0 on success, 
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_PERMISSION_DENIED Permission denied
 *
 * @post video_meta_update_to_db().
 */
int video_meta_set_played_time(video_meta_h video, time_t played_time);

/**
 * @deprecated Deprecated since 2.4.
 * @brief Sets the position played parameter of the video.
 * @details This function returns video's elapsed playback time parameter as period
 *          starting from the beginning of the movie.
 *
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 * @remarks  It is NOT recommended to use this attribute for your application-specific purpose because this attribute can be overwritten by other applications (even 0).
 *
 * @param[in] video           The video metadata handle
 * @param[in] played_position The position from the beginning of the video (in milliseconds)
 *
 * @return @c 0 on success, 
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_PERMISSION_DENIED Permission denied
 *
 * @post video_meta_update_to_db().
 */
int video_meta_set_played_position(video_meta_h video, int played_position);

/**
 * @brief Updates the video to the media database.
 *
 * @details The function updates the given video meta in the media database. The function should be called after any change in video attributes, to be updated to the media
 *          database. For example, after using video_meta_get_played_time() for setting the played time of the video, the video_meta_update_to_db() function should be called so as to update
 *          the given video attributes in the media database.
 *
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 * @privlevel public
 * @privilege %http://tizen.org/privilege/content.write
 *
 * @remarks Do not call this function in callback function of foreach function like media_info_foreach_media_from_db().
 *
 * @param[in] video The video metadata handle
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
 * @see video_meta_set_played_time()
 * @see video_meta_set_played_count()s
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
