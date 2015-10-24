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


#ifndef __TIZEN_AUDIO_META_H__
#define __TIZEN_AUDIO_META_H__

#include <media_content_type.h>


#ifdef __cplusplus
extern "C" {
#endif

/**
 * @file media_audio.h
 * @brief This file contains the audio metadata API and related structure and enumeration. \n
 *        Description of the audio content involves: album, artist, album_artist, author, genre and description tags. \n
 *        Parameters of the recording are also supported such as format, bitrate, duration, size etc.
 */

/**
 * @addtogroup CAPI_CONTENT_MEDIA_AUDIO_META_MODULE
 * @{
 */


/**
 * @brief Destroys the audio metadata.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 * @param[in] audio The audio metadata handle
 *
 * @return @c 0 on success, 
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_PERMISSION_DENIED Permission denied
 *
 * @pre Get a copy of audio metadata handle handle by calling audio_meta_clone().
 *
 * @see audio_meta_clone()
 */
int audio_meta_destroy(audio_meta_h audio);

/**
 * @brief Clones the audio metadata.
 * @details This function copies the audio metadata handle from source to destination.
 *
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 * @remarks The destination handle must be released using audio_meta_destroy().
 *
 * @param[out] dst  The destination handle to audio metadata
 * @param[in]  src  The source handle to the audio metadata
 *
 * @return @c 0 on success, 
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_OUT_OF_MEMORY     Out of memory
 * @retval #MEDIA_CONTENT_ERROR_PERMISSION_DENIED Permission denied
 *
 * @see audio_meta_destroy()
 */
int audio_meta_clone(audio_meta_h *dst, audio_meta_h src);

/**
 * @brief Gets the audio ID of the given audio metadata.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 * @remarks You must release @a media_id using free().
 *
 * @param[in]  audio    The audio metadata handle
 * @param[out] media_id The ID of the audio
 *
 * @return @c 0 on success, 
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_OUT_OF_MEMORY     Out of memory
 * @retval #MEDIA_CONTENT_ERROR_PERMISSION_DENIED Permission denied
 */
int audio_meta_get_media_id(audio_meta_h audio, char **media_id);

/**
 * @brief Gets the album name of the given audio metadata.
 * @details If the value is an empty string, the method returns "Unknown".
 *
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 * @remarks You must release @a album_name using free().
 *
 * @param[in]  audio      The audio metadata handle
 * @param[out] album_name The name of the album
 *
 * @return @c 0 on success, 
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_OUT_OF_MEMORY     Out of memory
 * @retval #MEDIA_CONTENT_ERROR_PERMISSION_DENIED Permission denied
 */
int audio_meta_get_album(audio_meta_h audio, char **album_name);

/**
 * @brief Gets the artist name of the given audio metadata.
 * @details If the value is an empty string, the method returns "Unknown".
 *
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 * @remarks You must release @a artist_name using free().
 *
 * @param[in]  audio       The audio metadata handle
 * @param[out] artist_name The name of the artist
 *
 * @return @c 0 on success, 
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_OUT_OF_MEMORY     Out of memory
 * @retval #MEDIA_CONTENT_ERROR_PERMISSION_DENIED Permission denied
 */
int audio_meta_get_artist(audio_meta_h audio, char **artist_name);

/**
 * @brief Gets the album artist name of the given audio metadata.
 * @details If the value is an empty string, the method returns "Unknown".
 *
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 * @remarks You must release @a album_artist_name using free().
 *
 * @param[in]  audio             The audio metadata handle
 * @param[out] album_artist_name The name of the album artist
 *
 * @return @c 0 on success, 
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_OUT_OF_MEMORY     Out of memory
 * @retval #MEDIA_CONTENT_ERROR_PERMISSION_DENIED Permission denied
 */
int audio_meta_get_album_artist(audio_meta_h audio, char **album_artist_name);

/**
 * @brief Gets the genre name of the given audio metadata.
 * @details If the value is an empty string, the method returns "Unknown".
 *
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 * @remarks You must release @a genre_name using free().
 *
 * @param[in]  audio      The audio metadata handle
 * @param[out] genre_name The name of the genre
 *
 * @return @c 0 on success, 
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_OUT_OF_MEMORY     Out of memory
 * @retval #MEDIA_CONTENT_ERROR_PERMISSION_DENIED Permission denied
 */
int audio_meta_get_genre(audio_meta_h audio, char **genre_name);

/**
 * @brief Gets the composer name of the given audio metadata.
 * @details If the value is an empty string, the method returns "Unknown".
 *
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 * @remarks You must release @a author_name using free().
 *
 * @param[in]  audio       The audio metadata handle
 * @param[out] composer_name The name of the author of the audio
 *
 * @return @c 0 on success, 
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_OUT_OF_MEMORY     Out of memory
 * @retval #MEDIA_CONTENT_ERROR_PERMISSION_DENIED Permission denied
 */
int audio_meta_get_composer(audio_meta_h audio, char **composer_name);

/**
 * @brief Gets the year of the given audio metadata.
 * @details If the value is an empty string, the method returns "Unknown".
 *
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 * @remarks You must release @a year using free().
 *
 * @param[in]  audio The audio metadata handle
 * @param[out] year  The year of the audio file
 *
 * @return @c 0 on success, 
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_OUT_OF_MEMORY     Out of memory
 * @retval #MEDIA_CONTENT_ERROR_PERMISSION_DENIED Permission denied
 */
int audio_meta_get_year(audio_meta_h audio, char **year);

/**
 * @brief Gets the recorded date of the given audio metadata.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 * @remarks You must release @a recorded_date using free().
 *
 * @param[in]  audio         The audio metadata handle
 * @param[out] recorded_date The recorded date of the audio file
 *
 * @return @c 0 on success, 
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_OUT_OF_MEMORY     Out of memory
 * @retval #MEDIA_CONTENT_ERROR_PERMISSION_DENIED Permission denied
 */
int audio_meta_get_recorded_date(audio_meta_h audio, char **recorded_date);

/**
 * @brief Gets the copyright notice of the given audio metadata.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 * @remarks You must release @a copyright using free().
 *
 * @param[in]  audio     The audio metadata handle
 * @param[out] copyright The audio copyright notice
 *
 * @return @c 0 on success, 
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_OUT_OF_MEMORY     Out of memory
 * @retval #MEDIA_CONTENT_ERROR_PERMISSION_DENIED Permission denied
 */
int audio_meta_get_copyright(audio_meta_h audio, char **copyright);

/**
 * @brief Gets the track number of the given audio metadata.
 * @details If the value is an empty string, the method returns "Unknown".
 *
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 * @param[in]  audio     The audio metadata handle
 * @param[out] track_num The audio track number
 *
 * @return @c 0 on success, 
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_PERMISSION_DENIED Permission denied
 */
int audio_meta_get_track_num(audio_meta_h audio, char **track_num);

/**
 * @brief Gets the bitrate of the given audio metadata in bitrate per second.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 * @param[in]  audio    The audio metadata handle
 * @param[out] bit_rate The audio bitrate in bit per second [bps]
 *
 * @return @c 0 on success, 
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_PERMISSION_DENIED Permission denied
 */
int audio_meta_get_bit_rate(audio_meta_h audio, int *bit_rate);

/**
 * @brief Gets bit per sample of the given audio metadata.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 * @param [in]  audio        The handle to the audio metadata
 * @param [out] bitpersample The audio bit per sample
 *
 * @return @c 0 on success, 
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_PERMISSION_DENIED Permission denied
 */

int audio_meta_get_bitpersample(audio_meta_h audio, int *bitpersample);

/**
 * @brief Gets the sample rate of the given audio metadata.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 * @param[in]  audio       The audio metadata handle
 * @param[out] sample_rate The audio sample rate[hz]
 *
 * @return @c 0 on success, 
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_PERMISSION_DENIED Permission denied
 */
int audio_meta_get_sample_rate(audio_meta_h audio, int *sample_rate);

/**
 * @brief Gets the channel of the given audio metadata.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 * @param[in]  audio   The audio metadata handle
 * @param[out] channel The channel of the audio
 *
 * @return @c 0 on success, 
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_PERMISSION_DENIED Permission denied
 */
int audio_meta_get_channel(audio_meta_h audio, int *channel);

/**
 * @brief Gets the track duration of the given audio metadata.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 * @param[in]  audio    The audio metadata handle
 * @param[out] duration The audio file duration
 *
 * @return @c 0 on success, 
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_PERMISSION_DENIED Permission denied
 */
int audio_meta_get_duration(audio_meta_h audio, int *duration);

/**
 * @deprecated Deprecated since 2.4. [Use media_info_get_played_count() instead]
 * @brief Gets the number of times the given audio has been played.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 * @param[in]  audio        The audio metadata handle
 * @param[out] played_count The counter of the audio played
 *
 * @return @c 0 on success, 
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_PERMISSION_DENIED Permission denied
 */
int audio_meta_get_played_count(audio_meta_h audio, int *played_count);

/**
 * @deprecated Deprecated since 2.4. [Use media_info_get_played_time() instead]
 * @brief Gets the played time parameter of an audio.
 * @details This function returns audio's elapsed playback time parameter as a period
 *          starting from the beginning of the track.
 *
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 * @param[in]  audio       The audio metadata handle
 * @param[out] played_time The elapsed time of the audio
 *
 * @return @c 0 on success, 
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_PERMISSION_DENIED Permission denied
 */
int audio_meta_get_played_time(audio_meta_h audio, time_t *played_time);

/**
 * @deprecated Deprecated since 2.4.
 * @brief Gets the played position parameter of an audio.
 * @details This function returns audio's elapsed playback position parameter as a period
 *          starting from the beginning of the track.
 *
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 * @param[in]  audio           The audio metadata handle
 * @param[out] played_position The elapsed time of the audio
 *
 * @return @c 0 on success, 
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_PERMISSION_DENIED Permission denied
 */
int audio_meta_get_played_position(audio_meta_h audio, int *played_position);

/**
 * @deprecated Deprecated since 2.4. [Use media_info_increase_played_count() instead]
 * @brief Sets the played count to an audio meta handle.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 * @param[in] audio        The audio metadata handle
 * @param[in] played_count The played count of the audio
 *
 * @return @c 0 on success, 
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_PERMISSION_DENIED Permission denied
 *
 * @post audio_meta_update_to_db().
 */
int audio_meta_set_played_count(audio_meta_h audio, int played_count);

/**
 * @deprecated Deprecated since 2.4. [Use media_info_set_played_time() instead]
 * @brief Sets the played time to an audio meta handle.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 * @param[in] audio       The audio metadata handle
 * @param[in] played_time The played time of the audio
 *
 * @return @c 0 on success, 
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_PERMISSION_DENIED Permission denied
 *
 * @post audio_meta_update_to_db().
 */
int audio_meta_set_played_time(audio_meta_h audio, time_t played_time);

/**
 * @deprecated Deprecated since 2.4.
 * @brief Sets the played position to an audio meta handle.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 * @remarks  It is NOT recommended to use this attribute for your application-specific purpose because this attribute can be overwritten by other applications (even 0).
 *
 * @param[in] audio           The audio metadata handle
 * @param[in] played_position The played position of the audio
 *
 * @return @c 0 on success, 
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_PERMISSION_DENIED Permission denied
 *
 * @post audio_meta_update_to_db().
 */
int audio_meta_set_played_position(audio_meta_h audio, int played_position);

/**
 * @brief Updates an audio metadata with modified attributes in the media database.
 * @details The function updates the given audio meta in the media database.
 *
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 * @privlevel public
 * @privilege %http://tizen.org/privilege/content.write
 *
 * @remarks The function should be called after any change in the attributes, to update the media database.
 *          For example, after using audio_meta_set_played_count() for changing the count of the played, the
 *          audio_meta_update_to_db() function should be called to update the given attributes in the media database. \n
 *          Do not call this function in callback function of foreach function like media_info_foreach_media_from_db().
 *
 * @param[in] audio The audio metadata handle
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
 * @see audio_meta_set_played_count()
 * @see audio_meta_set_played_time()
 * @see audio_meta_set_played_position()
 */
int audio_meta_update_to_db(audio_meta_h audio);

/**
 * @}
 */


#ifdef __cplusplus
}
#endif

#endif /*__TIZEN_AUDIO_META_H__*/
