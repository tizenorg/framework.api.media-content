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
 * @brief This file contains the image metadata API and related functions to proceed with them.
 *        Functions include cloning and destroying the image metadata, getting image metadata such as width, height, \n
 *        orientation, date taken, title, burst shot id and updating image to DB.
 */

/**
 * @addtogroup CAPI_CONTENT_MEDIA_IMAGE_MODULE
 * @{
 */


/**
 * @brief Clones the image metadata.
 * @details The function copies the image metadata handle from a source to destination.
 *
 * @since_tizen 2.3
 *
 * @remarks The destination handle must be released with image_meta_destroy().
 *
 * @param[out] dst  The destination handle to the image metadata
 * @param[in]  src  The source handle to the image metadata
 *
 * @return @c 0 on success, 
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_OUT_OF_MEMORY     Out of memory
 * @retval #MEDIA_CONTENT_ERROR_PERMISSION_DENIED Permission denied
 *
 * @see image_meta_destroy()
 */
int image_meta_clone(image_meta_h *dst, image_meta_h src);

/**
 * @brief Destroys the image metadata.
 * @details The function frees all resources related to the image metadata handle. This handle
 *          no longer can be used to perform any operation. A new handle has to
 *          be created before next usage.
 *
 * @since_tizen 2.3
 *
 * @param[in] image The image metadata handle
 *
 * @return @c 0 on success, 
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE Successful
 * @retval #MEDIA_CONTENT_ERROR_PERMISSION_DENIED Permission denied
 *
 * @pre Get a copy of image_meta handle by calling image_meta_clone().
 *
 * @see image_meta_clone()
 */
int image_meta_destroy(image_meta_h image);

/**
 * @brief Gets the ID of an image.
 * @since_tizen 2.3
 *
 * @param[in]  image    The image metadata handle
 * @param[out] media_id The ID of an image
 *
 * @return @c 0 on success, 
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_PERMISSION_DENIED Permission denied
 */
int image_meta_get_media_id(image_meta_h image, char **media_id);

/**
 * @brief Gets the image width in pixels.
 * @since_tizen 2.3
 *
 * @param[in]  image The image metadata handle
 * @param[out] width The image width in pixels
 *
 * @return @c 0 on success, 
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_PERMISSION_DENIED Permission denied
 */
int image_meta_get_width(image_meta_h image, int *width);

/**
 * @brief Gets the image height in pixels.
 * @since_tizen 2.3
 *
 * @param[in]  image  The image metadata handle
 * @param[out] height The image height in pixels
 *
 * @return @c 0 on success, 
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_PERMISSION_DENIED Permission denied
 */
int image_meta_get_height(image_meta_h image, int *height);

/**
 * @brief Gets the image orientation.
 * @since_tizen 2.3
 *
 * @param[in]  image       The image metadata handle
 * @param[out] orientation The image orientation
 *
 * @return @c 0 on success, 
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_PERMISSION_DENIED Permission denied
 */
int image_meta_get_orientation(image_meta_h image, media_content_orientation_e *orientation);

/**
 * @brief Gets the image creation time as time_t structure.
 * @since_tizen 2.3
 *
 * @param[in]  image      The image metadata handle
 * @param[out] date_taken The time, when image was taken (in seconds, since the Epoch)
 *
 * @return @c 0 on success, 
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_PERMISSION_DENIED Permission denied
 */
int image_meta_get_date_taken(image_meta_h image, char **date_taken);

/**
 * @brief Gets the burst shot ID.
 * @since_tizen 2.3
 *
 * @remarks You must release @a burst_id using free().
 *
 * @param[in]  image    The image metadata handle
 * @param[out] burst_id The ID of burst shot\ n
 *                      If @a burst_id is @c NULL, this is not burst shot.
 *
 * @return @c 0 on success, 
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_OUT_OF_MEMORY     Out of memory
 * @retval #MEDIA_CONTENT_ERROR_PERMISSION_DENIED Permission denied
 */
int image_meta_get_burst_id(image_meta_h image, char **burst_id);

/**
 * @brief Checks whether the media is a burst shot image.
 * @since_tizen 2.3
 *
 * @param[in]  image         The image metadata handle
 * @param[out] is_burst_shot @c true if the media is a burst shot image,
 *                           otherwise @c false if the media is not a burst shot image
 *
 * @return @c 0 on success, 
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_PERMISSION_DENIED Permission denied
 */
int image_meta_is_burst_shot(image_meta_h image, bool *is_burst_shot);

/**
 * @brief Sets an orientation of the image.
 * @since_tizen 2.3
 *
 * @param[in] image       The image metadata handle
 * @param[in] orientation The image orientation
 *
 * @return @c 0 on success, 
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE              Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_PERMISSION_DENIED Permission denied
 *
 * @post image_meta_update_to_db()
 */
int image_meta_set_orientation(image_meta_h image, media_content_orientation_e orientation);

/**
 * @brief Updates the image to the media database.
 *
 * @details The function updates the given image meta in the media database. The function should be called after any change in image attributes, to be updated to the media
 *          database. For example, after using image_meta_set_orientation() for setting the orientation of the image, the image_meta_update_to_db() function should be called so as to update
 *          the given image attributes in the media database.
 *
 * @since_tizen 2.3
 *
 * @privlevel public
 * @privilege %http://tizen.org/privilege/content.write
 *
 * @remarks Do not call this function in callback function of foreach function like media_info_foreach_media_from_db().
 *
 * @param[in] image The handle to the image
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
 * @see image_meta_set_orientation()
 */
int image_meta_update_to_db(image_meta_h image);

/**
 * @}
 */


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /*__TIZEN_IMAGE_META_H__*/
