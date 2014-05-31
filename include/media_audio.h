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
 * @addtogroup CAPI_CONTENT_MEDIA_AUDIO_META_MODULE
 * @{
 *
 * @file audio_meta.h
 * @brief This file contains the audio metadata API and related structure and enumeration. \n
 *        Description of the audio content involves: album, artist, album_artist, author, genre and description tags. \n
 *        Parameters of the recording are also supported, as: format, bitrate, duration, size etc.
 */


/**
 * @brief Destroys audio metadata.
 *
 * @param [in] audio The handle to audio metadata.
 * @return 0 on success, otherwise a negative error value.
 * @retval #MEDIA_CONTENT_ERROR_NONE Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @pre Get copy of audio metadata handle handle by calling audio_meta_clone()
 * @see audio_meta_clone()
 */
int audio_meta_destroy(audio_meta_h audio);

/**
 * @brief Clones audio metadata.
 * @details Function copies the audio metadata handle handle from source to destination.
 *
 * @remark The destination handle must be released with audio_meta_destroy() by you.
 *
 * @param [out] dst A destination handle to audio metadata
 * @param [in] src The source handle to audio metadata
 * @return 0 on success, otherwise a negative error value.
 * @retval #MEDIA_CONTENT_ERROR_NONE Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_OUT_OF_MEMORY Out of memory
 * @see audio_meta_destroy()
 */
int audio_meta_clone(audio_meta_h *dst, audio_meta_h src);

/**
 * @brief Gets id of audio of given audio metadata.
 *
 * @remarks @a audio id must be released with free() by you.
 *
 * @param [in] audio The handle to audio metadata
 * @param [out] media_id The id of the audio
 * @return 0 on success, otherwise a negative error value.
 * @retval #MEDIA_CONTENT_ERROR_NONE Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_OUT_OF_MEMORY Out of memory
 */
int audio_meta_get_media_id(audio_meta_h audio, char **media_id);

/**
 * @brief Gets title of audio of given audio metadata.
 *
 * @remarks @a audio title must be released with free() by you.
 *
 * @param [in] audio The handle to audio metadata
 * @param [out] title The title of the audio
 * @return 0 on success, otherwise a negative error value.
 * @retval #MEDIA_CONTENT_ERROR_NONE Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_OUT_OF_MEMORY Out of memory
 */
int audio_meta_get_title(audio_meta_h audio, char **title);

/**
 * @brief Gets name of album of given audio metadata.\n
 * If the value is an empty string, the method returns "Unknown".
 *
 * @remarks @a album_name must be released with free() by you.
 *
 * @param [in] audio The handle to audio metadata
 * @param [out] album_name The name of the album
 * @return 0 on success, otherwise a negative error value.
 * @retval #MEDIA_CONTENT_ERROR_NONE Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_OUT_OF_MEMORY Out of memory
 */
int audio_meta_get_album(audio_meta_h audio, char **album_name);

/**
 * @brief Gets name of artist of given audio metadata.\n
 * If the value is an empty string, the method returns "Unknown".
 *
 * @remarks @a artist_name must be released with free() by you.
 *
 * @param [in] audio The handle to audio metadata
 * @param [out] artist_name The name of the artist
 * @return 0 on success, otherwise a negative error value.
 * @retval #MEDIA_CONTENT_ERROR_NONE Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_OUT_OF_MEMORY Out of memory
 */
int audio_meta_get_artist(audio_meta_h audio, char **artist_name);

/**
 * @brief Gets name of album_artist of given audio metadata.\n
 * If the value is an empty string, the method returns "Unknown".
 *
 * @remarks @a album_artist_name must be released with free() by you.
 *
 * @param [in] audio The handle to audio metadata
 * @param [out] album_artist_name The name of the album_artist
 * @return 0 on success, otherwise a negative error value.
 * @retval #MEDIA_CONTENT_ERROR_NONE Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_OUT_OF_MEMORY Out of memory
 */
int audio_meta_get_album_artist(audio_meta_h audio, char **album_artist_name);

/**

 * @brief Gets name of genre of given audio metadata.\n
 * If the value is an empty string, the method returns "Unknown".
 *
 * @remarks @a genre_name must be released with free() by you.
 *
 * @param [in] audio The handle to audio metadata
 * @param [out] genre_name The name of the genre
 * @return 0 on success, otherwise a negative error value.
 * @retval #MEDIA_CONTENT_ERROR_NONE Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_OUT_OF_MEMORY Out of memory
 */
int audio_meta_get_genre(audio_meta_h audio, char **genre_name);

/**
 * @brief Gets name of composer of given audio metadata.\n
 * If the value is an empty string, the method returns "Unknown".
 *
 * @remarks @a author_name must be released with free() by you.
 *
 * @param [in] audio The handle to audio metadata
 * @param [out] author_name The name of the author of audio
 * @return 0 on success, otherwise a negative error value.
 * @retval #MEDIA_CONTENT_ERROR_NONE Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_OUT_OF_MEMORY Out of memory
 */
int audio_meta_get_composer(audio_meta_h audio, char **composer_name);

/**
 * @brief Gets year of given audio metadata.\n
 * If the value is an empty string, the method returns "Unknown".
 *
 * @remarks @a year must be released with free() by you.
 *
 * @param [in] audio The handle to audio metadata
 * @param [out] year The year of the audio file
 * @return 0 on success, otherwise a negative error value.
 * @retval #MEDIA_CONTENT_ERROR_NONE Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_OUT_OF_MEMORY Out of memory
 */
int audio_meta_get_year(audio_meta_h audio, char **year);

/**
 * @brief Gets recorded date of given audio metadata.
 *
 * @remarks @a recorded date must be released with free() by you.
 *
 * @param [in] audio The handle to audio metadata
 * @param [out] recorded_date The recorded date of the audio file
 * @return 0 on success, otherwise a negative error value.
 * @retval #MEDIA_CONTENT_ERROR_NONE Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_OUT_OF_MEMORY Out of memory
 */
int audio_meta_get_recorded_date(audio_meta_h audio, char **recorded_date);

/**
 * @brief Gets copyright notice of given audio metadata.
 *
 * @remarks @a copyright must be released with free() by you.
 *
 * @param [in] audio The handle to audio metadata
 * @param [out] copyright The audio copyright notice
 * @return 0 on success, otherwise a negative error value.
 * @retval #MEDIA_CONTENT_ERROR_NONE Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_OUT_OF_MEMORY Out of memory
 */
int audio_meta_get_copyright(audio_meta_h audio, char **copyright);

/**
 * @brief Gets track number of given audio metadata. \n
 * If the value is an empty string, the method returns "Unknown".
 *
 * @param [in] audio The handle to audio metadata
 * @param [out] track_num The audio track number
 * @return 0 on success, otherwise a negative error value.
 * @retval #MEDIA_CONTENT_ERROR_NONE Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 */
int audio_meta_get_track_num(audio_meta_h audio, char **track_num);

/**
 * @brief Gets bitrate of given audio metadata in bitrate per second.
 *
 * @param [in] audio The handle to audio metadata
 * @param [out] bit_rate The audio bitrate in bit per second [bps]
 * @return 0 on success, otherwise a negative error value.
 * @retval #MEDIA_CONTENT_ERROR_NONE Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 */
int audio_meta_get_bit_rate(audio_meta_h audio, int *bit_rate);

/**
 * @brief Gets sample rate of given audio metadata.
 *
 * @param [in] audio The handle to audio metadata
 * @param [out] sample_rate The audio sample rate[hz]
 * @return 0 on success, otherwise a negative error value.
 * @retval #MEDIA_CONTENT_ERROR_NONE Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 */
int audio_meta_get_sample_rate(audio_meta_h audio, int *sample_rate);

/**
 * @brief Gets channel of given audio metadata.
 *
 * @param [in] audio The handle to audio metadata
 * @param [out] channel The channel of audio
 * @return 0 on success, otherwise a negative error value.
 * @retval #MEDIA_CONTENT_ERROR_NONE Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 */
int audio_meta_get_channel(audio_meta_h audio, int *channel);

/**
 * @brief Gets track duration of given audio metadata.
 *
 * @param [in] audio The handle to audio metadata
 * @param [out] duration The audio file duration
 * @return 0 on success, otherwise a negative error value.
 * @retval #MEDIA_CONTENT_ERROR_NONE Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 */
int audio_meta_get_duration(audio_meta_h audio, int *duration);

/**
 * @brief Gets number which represents how many times given audio has been played.
 *
 * @param [in] audio The handle to audio metadata
 * @param [out] count_played The counter of audio played
 * @return 0 on success, otherwise a negative error value.
 * @retval #MEDIA_CONTENT_ERROR_NONE Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 */
int audio_meta_get_played_count(audio_meta_h audio, int *played_count);

/**
 * @brief Gets the audio's played time parameter.
 * @details Function returns audio's elapsed playback time parameter as period
 * starting from the beginning of the track.
 *
 * @param [in] audio The handle to audio metadata
 * @param [out] played_time The elapsed time of the audio
 * @return 0 on success, otherwise a negative error value.
 * @retval #MEDIA_CONTENT_ERROR_NONE Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 */
int audio_meta_get_played_time(audio_meta_h audio, time_t *played_time);

/**
 * @brief Gets the audio's played position parameter.
 * @details Function returns audio's elapsed playback position parameter as period
 * starting from the beginning of the track.
 *
 * @param [in] audio The handle to audio metadata
 * @param [out] played_position The elapsed time of the audio
 * @return 0 on success, otherwise a negative error value.
 * @retval #MEDIA_CONTENT_ERROR_NONE Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 */
int audio_meta_get_played_position(audio_meta_h audio, int *played_position);

/**
 * @brief Sets the played count to audio meta handle.
 *
 * @param [in] audio The handle to audio metadata
 * @param [in] played_count The played count of audio
 * @return 0 on success, otherwise a negative error value.
 * @retval #MEDIA_CONTENT_ERROR_NONE Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @post audio_meta_update_to_db()
 */
int audio_meta_set_played_count(audio_meta_h audio, int played_count);

/**
 * @brief Sets the played time to audio meta handle.
 *
 * @param [in] audio The handle to audio metadata
 * @param [in] played_time The played time of audio
 * @return 0 on success, otherwise a negative error value.
 * @retval #MEDIA_CONTENT_ERROR_NONE Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @post audio_meta_update_to_db() 
 */
int audio_meta_set_played_time(audio_meta_h audio, time_t played_time);

/**
 * @brief Sets the played position to audio meta handle.
 *
 * @param [in] audio The handle to audio metadata
 * @param [in] played_position The played position of audio
 * @return 0 on success, otherwise a negative error value.
 * @retval #MEDIA_CONTENT_ERROR_NONE Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @post audio_meta_update_to_db()
 */
int audio_meta_set_played_position(audio_meta_h audio, int played_position);

/**
 * @brief Updates audio metadata which is modified attributes to the media database.
 *
 * @details The function updates the given audio meta in the media database.\n
 * The function should be called after any change in the attributes, to be updated to the media database.\n
 * For example, after using audio_meta_set_played_count() for changing the count of the played,
 * audio_meta_update_to_db() function should be called so as to update the given the attibutes in the media database.
 *
 * @param [in] audio The handle to audio metadata
 * @return 0 on success, otherwise a negative error value.
 * @retval #MEDIA_CONTENT_ERROR_NONE Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @pre This function requires opened connection to content service by media_content_connect().
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
