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



#ifndef __TIZEN_MEDIA_FILTER_H__
#define __TIZEN_MEDIA_FILTER_H__

#include <media_content_type.h>


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 * @addtogroup CAPI_CONTENT_MEDIA_FILTER_MODULE
 * @{
 *
 * @file media-filter.h
 * @brief This file contains the media filter API and related operatioins with filters \n
 *        The following functions include: creating and destroying media filter handles that are used to get filtered information,  \n
 *        making free all resources related to filter handle, limiting number of items returned, setting conditions for filter,  \n
 *        setting and getting media filter's content order and ordering keyword either descending or ascending.
 */

/**
 * @brief Creates a media filter handle.
 * @details This function creates a media filter handle. The handle can be
 * used to get filtered information based on filter properties i.e. offset, count, condition for searching and order.
 * @remarks The @a filter handle must be released with media_info_filter_destroy() by you.
 * @param[out] filter A handle to media filter
 * @return 0 on success, otherwise a negative error value.
 * @retval #MEDIA_CONTENT_ERROR_NONE Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_OUT_OF_MEMORY Out of memory
 * @see media_filter_destroy()
 *
 */
int media_filter_create(filter_h *filter);

/**
 * @brief Destroys a media filter handle.
 * @details The function frees all resources related to the media filter handle. The filter
 * handle no longer can be used to perform any operation. A new filter handle
 * has to be created before the next usage.
 *
 * @param[in] filter The handle to media filter
 * @return 0 on success, otherwise a negative error value.
 * @retval #MEDIA_CONTENT_ERROR_NONE Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @see media_filter_create()
 *
 */
int media_filter_destroy(filter_h filter);

/**
 * @brief Set the media filter's offset and count.
 * @details This function set the @a offset and @a count for the given filter used to limit number of items returned.
 * For example, if you set the @a offset as 10 and @a count as 5, then only searched data from 10 to 14 will be returned when the filter is used with foreach functions.
 *
 * @param[in] filter The handle to media filter
 * @param[in] offset The start position of the given filter(Starting from zero).
 * @param[in] count The number of items to be searched with respect to offset
 * @return return 0 on success, otherwise a negative error value.
 * @retval #MEDIA_CONTENT_ERROR_NONE Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @see media_filter_create()
 * @see media_filter_destroy()
 */
int media_filter_set_offset(filter_h filter, int offset, int count);

/**
 * @brief Set the @a condition for given @a filter.
 *
 * @param[in] filter The handle to media filter
 * @param[in] condition The condition which is used WHERE clause on a query
 * @param[in] collate_type The collate type for comparing two strings
 * @return return 0 on success, otherwise a negative error value.
 * @retval #MEDIA_CONTENT_ERROR_NONE Successful
 * @retval #MEDIA_CONTENT_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @see media_filter_create()
 * @see media_filter_destroy()
 */
int media_filter_set_condition(filter_h filter, const char *condition, media_content_collation_e collate_type);

/**
 * @brief Set the media filter's content @a order and @a order @a keyword either descending or ascending.
 *
 * @param[in] filter The handle to media filter
 * @param[in] order_type The search order type
 * @param[in] order_keyword The search order keyword
 * @param[in] collate_type The collate type for comparing two strings
 * @return return 0 on success, otherwise a negative error value.
 * @retval #MEDIA_CONTENT_ERROR_NONE Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @see media_filter_create()
 * @see media_filter_destroy()
 */
int media_filter_set_order(filter_h filter, media_content_order_e order_type, const char *order_keyword, media_content_collation_e collate_type);

/**
 * @brief Gets the @a offset and @a count for the given @a filter used to limit number of items returned.
 *
 * @param[in] filter The handle to Media filter
 * @param[out] offset The start position of the given filter(Starting from zero)
 * @param[out] count The number of items to be searched with respect to offset
 * @return return 0 on success, otherwise a negative error value.
 * @retval #MEDIA_CONTENT_ERROR_NONE Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @see media_filter_create()
 * @see media_filter_destroy()
 */
int media_filter_get_offset(filter_h filter, int *offset, int *count);

/**
 * @brief Get the @a condition for given @a filter.
 *
 * @remarks @a condition must be released with free() by you.
 * @param[in] filter The handle to media info filter
 * @param[out] condition The condition which is used WHERE clause on a query
 * @param[out] collate_type The collate type for comparing two strings
 * @return return 0 on success, otherwise a negative error value.
 * @retval #MEDIA_CONTENT_ERROR_NONE Successful
 * @retval #MEDIA_CONTENT_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @see media_filter_create()
 * @see media_filter_destroy()
 */
int media_filter_get_condition(filter_h filter, char **condition, media_content_collation_e *collate_type);

/**
 * @brief Get the media filter's content @a order and @a order @a keyword either descending or ascending.
 *
 * @remarks @a order_keyword must be released with free() by you.
 * @param[in] filter The handle to media filter
 * @param[out] order_type The search order type
 * @param[out] order_keyword The search order keyword
 * @param[out] collate_type The collate type for comparing two strings
 * @return return 0 on success, otherwise a negative error value.
 * @retval #MEDIA_CONTENT_ERROR_NONE Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @see media_filter_create()
 * @see media_filter_destroy()
 */
int media_filter_get_order(filter_h filter, media_content_order_e* order_type, char **order_keyword, media_content_collation_e *collate_type);


/**
 * @}
 */


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __TIZEN_MEDIA_FILTER_H__ */
