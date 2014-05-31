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



#ifndef __TIZEN_IMAGE_META_H__
#define __TIZEN_IMAGE_META_H__

#include <media_content_type.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/**
 * @addtogroup CAPI_CONTENT_MEDIA_IMAGE_MODULE
 * @{
 *
 * @file media_image.h
 * @brief This file contains the image metadata API and related functions to proceed with them. \n
 *        Functions include clonning and destroying image metadata, getting image metadata as width, height, \n
 *        orientation, date taken, title, weather, burst shot id and updating image to db.
 */

/**
 * @brief Clones image metadata.
 * @details Function copies the image metadata handle from source to destination.
 *
 * @remark The destination handle must be released with image_meta_destroy() by you.
 *
 * @param [out] dst A destination handle to image metadata
 * @param [in] src The source handle to image metadata
 * @return 0 on success, otherwise a negative error value.
 * @retval #MEDIA_CONTENT_ERROR_NONE Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_OUT_OF_MEMORY Out of memory
 * @see image_meta_destroy()
 */
int image_meta_clone(image_meta_h *dst, image_meta_h src);

/**
 * @brief Destroys image metadata.
 * @details The function frees all resources related to the image metadata handle. This handle
 * no longer can be used to perform any operation. A new handle has to
 * be created before next usage.
 *
 * @param [in] image The handle to image metadata
 * @return 0 on success, otherwise a negative error value.
 * @retval #MEDIA_CONTENT_ERROR_NONE Successful
 * @pre Get copy of image_meta handle by calling image_meta_clone()
 * @see image_meta_clone()
 */
int image_meta_destroy(image_meta_h image);

/**
 * @brief Gets the ID of image.
 *
 * @param [in] image The handle to image metadata
 * @param [out] media_id The ID of image
 * @return 0 on success, otherwise a negative error value.
 * @retval #MEDIA_CONTENT_ERROR_NONE Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 */
int image_meta_get_media_id(image_meta_h image, char **media_id);

/**
 * @brief Gets image's width in pixels.
 *
 * @param [in] image The handle to image metadata
 * @param [out] width The image width in pixels
 * @return 0 on success, otherwise a negative error value.
 * @retval #MEDIA_CONTENT_ERROR_NONE Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 */
int image_meta_get_width(image_meta_h image, int *width);

/**
 * @brief Gets image's height in pixels.
 *
 * @param [in] image The handle to image metadata
 * @param [out] height The image height in pixels
 * @return 0 on success, otherwise a negative error value.
 * @retval #MEDIA_CONTENT_ERROR_NONE Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 */
int image_meta_get_height(image_meta_h image, int *height);

/**
 * @brief Gets the image orientation.
 *
 * @param [in] image The handle to image metadata
 * @param [out] orientation The image orientation
 * @return 0 on success, otherwise a negative error value.
 * @retval #MEDIA_CONTENT_ERROR_NONE Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 */
int image_meta_get_orientation(image_meta_h image, media_content_orientation_e *orientation);

/**
 * @brief Gets the date, when image was created as time_t structure.
 *
 * @param [in] image The handle to image metadata
 * @param [out] date_taken The time, when image was taken (in seconds, since the Epoch)
 * @return 0 on success, otherwise a negative error value.
 * @retval #MEDIA_CONTENT_ERROR_NONE Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 */
int image_meta_get_date_taken(image_meta_h image, char **date_taken);

/**
 * @brief Gets the title.
 *
 * @remarks @a title must be released with free() by you.
 *
 * @param[in] media The handle to image metadata
 * @param[out] title title of image
 * @return 0 on success, otherwise a negative error value.
 * @retval #MEDIA_CONTENT_ERROR_NONE Successful
 * @retval #MEDIA_CONTENT_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 *
 */
int image_meta_get_title(image_meta_h image, char **title);

/**
 * @brief Gets the weather in maker note in exif.
 *        Example) WeatherInfo: Weather Condition=Sunny, Low Temp=22, High Temp=31
 *
 * @remarks @a weather must be released with free() by you.
 *
 * @param[in] image The handle to image metadata
 * @param[out] weather weather of image
 * @return 0 on success, otherwise a negative error value.
 * @retval #MEDIA_CONTENT_ERROR_NONE Successful
 * @retval #MEDIA_CONTENT_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 *
 */
int image_meta_get_weather(image_meta_h image, char **weather);

/**
 * @brief Gets the burst shot id.
 *
 * @remarks @a burst id must be released with free() by you.
 *
 * @param[in] media The handle toimage metadata
 * @param[out] burst_id The id of burst shot. if burst_id is NULL, this is not burst shot.
 * @return 0 on success, otherwise a negative error value.
 * @retval #MEDIA_CONTENT_ERROR_NONE Successful
 * @retval #MEDIA_CONTENT_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 *
 */
int image_meta_get_burst_id(image_meta_h image, char **burst_id);

/**
 * @brief Checks whether the media is burst shot image.
 *
 * @param[in] media The handle to image metadata
 * @param[out] is_burst_shot /@a true if the burst shot image,
 *                    /@a false if not burst shot image.
 * @return 0 on success, otherwise a negative error value.
 * @retval #MEDIA_CONTENT_ERROR_NONE Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 *
 */
int image_meta_is_burst_shot(image_meta_h image, bool *is_burst_shot);

/**
 * @brief Sets the weather information.
 *
 * @param [in] image The handle to image metadata
 * @param [in] weather The image weather information
 * @return 0 on success, otherwise a negative error value.
 * @retval #MEDIA_CONTENT_ERROR_NONE Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @post image_meta_update_to_db()
 */
int image_meta_set_weather(image_meta_h image, const char *weather);

/**
 * @brief Sets the image orientation.
 *
 * @param [in] image The handle to image metadata
 * @param [in] orientation The image orientation
 * @return 0 on success, otherwise a negative error value.
 * @retval #MEDIA_CONTENT_ERROR_NONE Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @post image_meta_update_to_db()
 */
int image_meta_set_orientation(image_meta_h image, media_content_orientation_e orientation);

/**
 * @brief Updates the image to the media database.
 *
 * @details The function updates the given image meta in the media database. The function should be called after any change in image attributes, to be updated to the media 
 * database. For example, after using image_meta_set_orientation() for setting the orientation of the image, image_meta_update_to_db() function should be called so as to update 
 * the given image attibutes in the media database.
 *
 * @param[in] image The handle to image
 * @return 0 on success, otherwise a negative error value.
 * @retval #MEDIA_CONTENT_ERROR_NONE Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @pre This function requires opened connection to content service by media_content_connect().
 * @see media_content_connect()
 * @see image_meta_set_orientation()
 *
 */
int image_meta_update_to_db(image_meta_h image);

/**
 * @}
 */


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /*__TIZEN_IMAGE_META_H__*/
