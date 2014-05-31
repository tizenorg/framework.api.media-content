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
 * @addtogroup CAPI_MEDIA_CONTENT_MODULE
 * @{
 * @file media-content.h
 * @brief This file contains API on functions proceeding with media content in db. \n
 *          Operations include connect and disconnect the media content service,scanning media file and folder with subfolders, \n
 *          subscribing and unsubscribing notifications of media db change.
 */

/**
 * @brief Connects to the media content service.
 * @details Any media content related function call should be invoked after this function call.
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #MEDIA_CONTENT_ERROR_NONE Successful
 * @retval #MEDIA_CONTENT_ERROR_DB_FAILED DB operation failed
 * @post media_content_disconnect()
 * @see media_content_disconnect()
 *
 */
int media_content_connect(void);

/**
 * @brief Disconnects from the media content service.
 * @details This function closes connection to the media content service. Any further media content related operation
 * cannot be performed after this function is called.
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #MEDIA_CONTENT_ERROR_NONE Successful
 * @retval #MEDIA_CONTENT_ERROR_DB_FAILED DB operation failed
 * @pre media_content_connect()
 * @see media_content_connect()
 *
 */
int media_content_disconnect(void);

/**
 * @brief Requests to scan a media file.
 * @details This function requests to scan a media file to media server.
 * if media file was not registered to db yet, that media file information will be added to media db. And if already registered to db, then try to refresh information.
 * if requested file is not exist on file system, information of the media file will be removed from media db.
 * @param[in] path The file path
 * @return 0 on success, otherwise a negative error value.
 * @retval #MEDIA_CONTENT_ERROR_NONE Successful
 * @pre This function requires opened connection to content service by media_content_connect().
 *
 */
int media_content_scan_file(const char *path);


/**
 * @brief Requests to scan a media folder, asynchronously.
 * @details This function requests to scan a media folder to media server with given completed callback fucntion.
 * #media_scan_completed_cb() function will be called when the scanning is finished.
 * The sub folders are also scanned, if there are sub folder in that folder. \n
 * If you want that the any folders are not scanned, you have to create a blank file ".scan_ignore" in that folder.
 * @param[in] path The folder path
 * @param[in] is_recursive /@a true if scan recursively subdirectories,
 *                    /@a false if scan only current directory,
 * @param[in] callback The callback to invoke when the scanning is finished
 * @param[in] user_data The user data to be passed to the callback function
 * @return 0 on success, otherwise a negative error value.
 * @retval #MEDIA_CONTENT_ERROR_NONE Successful
 * @see media_scan_completed_cb()
 *
 */
int media_content_scan_folder(const char *path, bool is_recursive, media_scan_completed_cb callback, void *user_data);

/**
 * @brief Subscribe notifications of media db change.
 * @details This function subscribes notifications of media db change, which are published by media server or other apps.
 * #media_content_db_update_cb() function will be called when notification of media db change is subscribed.
 * @param[in] callback The callback to invoke when the scanning is finished
 * @param[in] user_data The user data to be passed to the callback function
 * @return 0 on success, otherwise a negative error value.
 * @retval #MEDIA_CONTENT_ERROR_NONE Successful
 * @see media_content_db_update_cb()
 * @see media_content_unset_db_updated_cb()
 *
 */
int media_content_set_db_updated_cb(media_content_db_update_cb callback, void *user_data);

/**

 * @brief Unsubscribe notifications of media db change.
 * @details This function unsubscribes notifications of media db change, which are published by media server or other apps.
 * @return 0 on success, otherwise a negative error value.
 * @retval #MEDIA_CONTENT_ERROR_NONE Successful
 * @pre media_content_set_db_updated_cb()
 * @see media_content_set_db_updated_cb()
 *
 */
int media_content_unset_db_updated_cb(void);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __TIZEN_MEDIA_CONTENT_H__ */
