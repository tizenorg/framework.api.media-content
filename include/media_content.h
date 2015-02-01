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

#ifndef __TIZEN_MEDIA_CONTENT_H__
#define __TIZEN_MEDIA_CONTENT_H__

#include <media_audio.h>
#include <media_content_type.h>
#include <media_filter.h>
#include <media_folder.h>
#include <media_image.h>
#include <media_info.h>
#include <media_tag.h>
#include <media_video.h>
#include <media_group.h>
#include <media_playlist.h>
#include <media_bookmark.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 * @file media_content.h
 * @brief This file contains API providing functions for media content in DB. \n
 *        Operations include connect and disconnect the media content service, scanning media file and folder with subfolders, \n
 *        subscribing and unsubscribing notifications of media DB change.
 */

/**
 * @addtogroup CAPI_MEDIA_CONTENT_MODULE
 * @{
 */

/**
 * @brief Connects to the media content service.
 * @details Any media content related function call should be invoked after this function call.
 *
 * @since_tizen 2.3
 *
 * @return @c 0 on success, 
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE      Successful
 * @retval #MEDIA_CONTENT_ERROR_DB_FAILED DB operation failed
 * @retval #MEDIA_CONTENT_ERROR_PERMISSION_DENIED Permission denied
 *
 * @post media_content_disconnect()
 *
 * @see media_content_disconnect()
 */
int media_content_connect(void);

/**
 * @brief Disconnects from the media content service.
 * @details This function closes connection to the media content service. Any further media content related operation
 *          cannot be performed after this function is called.
 *
 * @since_tizen 2.3
 *
 * @return @c 0 on success, 
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE      Successful
 * @retval #MEDIA_CONTENT_ERROR_DB_FAILED DB operation failed
 * @retval #MEDIA_CONTENT_ERROR_PERMISSION_DENIED Permission denied
 *
 * @pre media_content_connect()
 *
 * @see media_content_connect()
 *
 */
int media_content_disconnect(void);

/**
 * @brief Requests to scan a media file.
 * @details This function requests to scan a media file to the media server.
 *          If media file is not registered to DB yet, that media file information will be added to the media DB. If it is already registered to the DB, then this tries to refresh information.
 *          If requested file does not exist on file system, information of the media file will be removed from the media DB.
 *
 * @since_tizen 2.3
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
 * @param[in] path The file path
 *
 * @return @c 0 on success, 
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE Successful
 * @retval #MEDIA_CONTENT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #MEDIA_CONTENT_ERROR_PERMISSION_DENIED Permission denied
 *
 * @pre This function requires opened connection to content service by media_content_connect().
 */
int media_content_scan_file(const char *path);

/**
 * @brief Requests to scan a media folder, asynchronously.
 * @details This function requests to scan a media folder to the media server with given completed callback function.
 *          media_scan_completed_cb() function will be called when the scanning is finished.
 *          The sub folders are also scanned, if there are sub folders in that folder. \n
 *          If any folder must not be scanned, a blank file ".scan_ignore" has to be created in that folder.
 *
 * @since_tizen 2.3
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
 * @param[in] path         The folder path
 * @param[in] is_recursive Set @c true to scan recursively subdirectories,
 *                         otherwise @c false to scan only the current directory
 * @param[in] callback     The callback to be invoked when the scanning is finished
 * @param[in] user_data    The user data to be passed to the callback function
 *
 * @return @c 0 on success, 
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE Successful
 * @retval #MEDIA_CONTENT_ERROR_PERMISSION_DENIED Permission denied
 *
 * @see media_scan_completed_cb()
 */
int media_content_scan_folder(const char *path, bool is_recursive, media_scan_completed_cb callback, void *user_data);

/**
 * @brief Subscribes notifications of the media DB change.
 * @details This function subscribes notifications of the media DB change which are published by the media server or other apps.
 *          media_content_db_update_cb() function will be called when notification of the media DB change is subscribed.
 *
 * @since_tizen 2.3
 *
 * @param[in] callback  The callback to be invoked when the scanning is finished
 * @param[in] user_data The user data to be passed to the callback function
 *
 * @return @c 0 on success, 
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE Successful
 * @retval #MEDIA_CONTENT_ERROR_PERMISSION_DENIED Permission denied
 *
 * @see media_content_db_update_cb()
 * @see media_content_unset_db_updated_cb()
 */
int media_content_set_db_updated_cb(media_content_db_update_cb callback, void *user_data);

/**
 * @brief Unsubscribes notifications of the media DB change.
 * @details This function unsubscribes notifications of the media DB change which are published by the media server or other apps.
 *
 * @since_tizen 2.3
 *
 * @return @c 0 on success, 
 *         otherwise a negative error value
 *
 * @retval #MEDIA_CONTENT_ERROR_NONE Successful
 * @retval #MEDIA_CONTENT_ERROR_PERMISSION_DENIED Permission denied
 *
 * @pre media_content_set_db_updated_cb()
 *
 * @see media_content_set_db_updated_cb()
 */
int media_content_unset_db_updated_cb(void);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __TIZEN_MEDIA_CONTENT_H__ */
