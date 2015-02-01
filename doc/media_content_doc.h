/*
 * Copyright (c) 2011 Samsung Electronics Co., Ltd All Rights Reserved
 *
 * Licensed under the Apache License, Version 2.0 (the License);
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. 
 */


#ifndef __TIZEN_MEDIA_CONTENT_DOC_H__
#define __TIZEN_MEDIA_CONTENT_DOC_H__

/**
 * @defgroup CAPI_MEDIA_CONTENT_MODULE Media Content
 * @brief The Media Content API provides functions, enumerations used in the entire Content Service.
 *
 * @ingroup CAPI_CONTENT_FRAMEWORK
 *
 * @section CAPI_MEDIA_CONTENT_MODULE_HEADER Required Header
 *   \#include <media_content.h>
 *
 * @section CAPI_MEDIA_CONTENT_MODULE_OVERVIEW Overview
 * The Media Content API provides functions and enumerations used in the entire Content Service.\n
 * The information about media items i.e. image, audio and video, are managed in the content database and
 * operations that involve database requires an active connection with the media content service.\n
 * The API provides functions for connecting (#media_content_connect()) and disconnecting (#media_content_disconnect()) from the media content service.
 *
 * The API consists of @ref CAPI_CONTENT_MEDIA_FOLDER_MODULE,@ref CAPI_CONTENT_MEDIA_TAG_MODULE,@ref CAPI_CONTENT_MEDIA_FILTER_MODULE, @ref CAPI_CONTENT_MEDIA_INFO_MODULE API and others.
 * 
 * <table>
 * <tr>
 *    <th>API</th>
 *    <th>Description</th>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_CONTENT_MEDIA_FOLDER_MODULE </td>
 *    <td> Provide information about folders (e.g. path, name, modification date) stored on the device.\n
 * Provide information about the media items present in the folders.</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_CONTENT_MEDIA_TAG_MODULE </td>
 *    <td> Provide information about media tags.\n
 * Provide functions to insert or delete tag from database.\n
 * Provide functions to add and remove media item from tags in the database. </td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_CONTENT_MEDIA_FILTER_MODULE </td>
 *    <td> Provide functions for creating and destroying media filters.\n
 * Provide functions to get filter properties</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_CONTENT_MEDIA_INFO_MODULE </td>
 *    <td> Provide generic information about media content items (i.e. image, audio, video and others).\n
 *  Provide details about audio files (e.g. name, author, genre etc) present in the device.\n
 *  Provide details about image files (e.g. width, height, orientation etc) present in the device.\n
 *  Provide details about video files (e.g. width, height, duration etc) present in the device .</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_CONTENT_MEDIA_PLAYLIST_MODULE </td>
 *    <td> Provide information about the media playlist. </td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_CONTENT_MEDIA_ALBUM_MODULE </td>
 *    <td> Provide information about the media album. </td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_CONTENT_MEDIA_GROUP_MODULE </td>
 *    <td> Provide information about the media group(e.g. media artist, composer, genre, year). </td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_CONTENT_MEDIA_BOOKMARK_MODULE </td>
 *    <td> Provide information about the media bookmark. </td>
 * </tr> 
 * 
 * </table>
 *
 */

/**
 * @defgroup CAPI_CONTENT_MEDIA_FOLDER_MODULE Media Folder
 * @brief The Media folder API provides functions to get information about folders.
 *
 * @ingroup CAPI_MEDIA_CONTENT_MODULE
 *
 * @section CAPI_CONTENT_MEDIA_FOLDER_MODULE_HEADER Required Header
 *   \#include <media_content.h>
 *
 * @section CAPI_CONTENT_MEDIA_FOLDER_MODULE_OVERVIEW Overview
 * A Folder is used to organize media content files i.e. image, audio, video files, in the physical storage of the device.  
 * The Media Folder API provides functions to get basic information about existing folders e.g. folder name, path and storage type.
 * It also provides functions to get information related to media items present in the folder.
 *
 * For getting information about folder call the respective get functions e.g. to get path and name of a given folder call #media_folder_get_path() and #media_folder_get_name() function respectively and so on.\n
 * Similarly call media_folder_get_media_count_from_db() to get count of media items present in a given folder.
 * @subsection CAPI_CONTENT_MEDIA_FOLDER_FOREACH_OPERATIONS Foreach Operations
 * <div><table class="doxtable">
 *     <tr>
 *        <th><b>FOREACH</b></th>
 *        <th><b>CALLBACK</b></th>
 *        <th><b>DESCRIPTION</b></th>
 *     </tr>
 *     <tr>
 *        <td>media_folder_foreach_folder_from_db()</td>
 *        <td>media_folder_cb()</td>
 *        <td>Iterates over a folder information list</td>
 *     </tr>
 *     <tr>
 *        <td>media_folder_foreach_media_from_db()</td>
 *        <td>media_info_cb()</td>
 *        <td>Iterates media information trough the folder</td>
 *     </tr>
 * </table></div>
 *
 *
 */


/**
 * @defgroup CAPI_CONTENT_MEDIA_TAG_MODULE Media Tag
 * @brief  The Media Tag API provides functions to manage media content tags.
 *
 * @ingroup CAPI_MEDIA_CONTENT_MODULE
 *
 * @section CAPI_CONTENT_MEDIA_TAG_MODULE_HEADER Required Header
 *   \#include <media_content.h>
 *
 * @section CAPI_CONTENT_MEDIA_TAG_MODULE_OVERVIEW Overview
 * A Tag is a special piece of information that may be associated with media content items.
 * Tagging allows a user to organize large number of items into logical groups providing a simplified 
 * and faster way of accessing media content items.\n\n
 * Media Tag API provides functions to get basic information about existing tags and manage tags 
 * associated with Media Information (#media_info_h). 
 * For inserting a new tag to the database call #media_tag_insert_to_db() function and for deleting existing tag from database
 * call media_tag_delete_from_db() function.\n
 * A Media item can be associated and dissociated from tags by calling #media_tag_add_media() and #media_tag_remove_media() function respectively.
 * Finally, #media_tag_update_to_db function should be called so as to update the given item in the media database.
 *
 * @subsection CAPI_CONTENT_MEDIA_TAG_FOREACH_OPERATIONS Foreach Operations
 * <div><table class="doxtable">
 *     <tr>
 *        <th><b>FOREACH</b></th>
 *        <th><b>CALLBACK</b></th>
 *        <th><b>DESCRIPTION</b></th>
 *     </tr>
 *     <tr>
 *        <td>media_tag_foreach_tag_from_db()</td>
 *        <td> media_tag_cb()</td>
 *        <td>Iterates through tags</td>
 *     </tr>
 *     <tr>
 *        <td>media_tag_foreach_media_from_db()</td>
 *        <td>media_info_cb()</td>
 *        <td> Iterates through the media items for a given tag
 </td>
 *     </tr>
 * </table></div>
 *
 *
 *
 */


/**
 * @defgroup CAPI_CONTENT_MEDIA_FILTER_MODULE Media Filter
 * @brief The Media Filter API provides functions to manage media filters.
 *
 * @ingroup CAPI_MEDIA_CONTENT_MODULE
 *
 * @section CAPI_CONTENT_MEDIA_FILTER_MODULE_HEADER Required Header
 *   \#include <media_content.h>
 *
 * @section CAPI_CONTENT_MEDIA_FILTER_MODULE_OVERVIEW Overview
 * A Media filter is required for filtering information associated with media Folder, Tag, Audio, Bookmark and Media Information on basis of details like offset, count, order and condition for searching.\n
 * @ref CAPI_CONTENT_MEDIA_FILTER_MODULE API provides functions for creating and destroying media filters. \n
 * It provide functions to set properties and also provide functions for getting filter properties associated with a given media filter.
 *
 *\n
 * Setting media filter properties helps to limit the number of filtered items as following:
 * - Offset - Used to set starting position of the filter's search
 * - Count - Used to set number of items to be searched from offset
 * - Condition - Used to set keyword which user want to search
 * - Order - Used to set type of media to be ordered by the filter 
 *
 *\n
 * The Media Filter API provides functions for creating and destroying media filters.\n
 * It provide functions to set and get properties of the filter associated with a given media filter. \n
 * For creating a media filter (@ref filter_h), call #media_filter_create() function and call #media_filter_destroy() function for destroying an existing filter. \n
 * For setting filter properties call the respective set functions e.g. to set offset position, call #media_filter_set_offset() function and 
 * call #media_filter_set_condition() function to set the condition like an sql "where" clause. \n
 * Searchable expression can use one of the following forms:
 *
 *
 * - column = value
 * - column > value
 * - column >= value
 * - column < value
 * - column <= value
 * - value = column
 * -  * - value > column
 * - value >= column
 * - value < column
 * - value <= column
 * - column IN (value)
 * - column IN (value-list)
 * - column NOT IN (value)
 * - column NOT IN (value-list)
 * - column LIKE value
 * - expression1 AND expression2 OR expression3 
 * 
 *\n
 * 
 * Note that if you want to set qoutation(" ' " or " " ") as value of LIKE operator, you should use two times.(" '' " or " "" ") \n
 * And the optional ESCAPE clause is supported. Both percent symbol("%") and underscore symbol("_") are used in the LIKE pattern.\n
 * If these characters are used as value of LIKE operation, then the expression following the ESCAPE caluse of sqlite. \n
 *
 * For example, 
 * - column LIKE ('#%') ESCAPE('#') - "#" is escape character, it will be ignored. 
 *\n
 *
 *  
 * Similarly, call respective get function to get filter properties e.g. call #media_filter_get_condition() function 
 * to get condition of the media filter and call #media_filter_get_order() function to get order (#media_content_order_e) of the filtered items and so on.
 *
**/


/**
 * @defgroup CAPI_CONTENT_MEDIA_AUDIO_META_MODULE Audio Metadata
 * @brief The Audio Metadata API provides functions to get information about audio items.
 *
 * @ingroup CAPI_CONTENT_MEDIA_INFO_MODULE
 *
 * @section CAPI_CONTENT_MEDIA_AUDIO_META_MODULE_HEADER Required Header
 *   \#include <media_content.h>
 *
 * @section CAPI_CONTENT_MEDIA_AUDIO_META_MODULE_OVERVIEW Overview
 * The Audio Metadata API provides functions to get information about stored audio files.
 * Its purpose is threefold:
 * - to provide information about audio content
 * - to organize audio content logically (grouping)
 *
 * API provides functions that allow to check attributes of audio files. Following information about audio content is provided:
 * - album
 * - artist
 * - genre
 * - composer
 * - year 
 * - recorded_date
 * - copyright
 * - track number
 * - sample rate
 * - played count
 * - played time
 * - played position 
 * - bitrate
 *
 * <p>
 * For getting the audio handle (#audio_meta_h) from the media information (#media_info_h), call #media_info_get_audio() function.\n
 * For getting the information related to audio files stored in the device, call the respective get functions e.g. to get the artist of a audio, call #audio_meta_get_artist() function and 
 * to get bitrate of a audio, call #audio_meta_get_bit_rate() function and so on.\n
 * When the audio handle is no longer needed, it should be destroyed by calling #audio_meta_destroy() function.\n 
 *
 *
 *
 */


/**
 * @defgroup CAPI_CONTENT_MEDIA_PLAYLIST_MODULE Media Playlist
 * @brief The Media Playlist API provides functions to manage media playlists.
 *
 * @ingroup CAPI_MEDIA_CONTENT_MODULE
 *
 * @section CAPI_CONTENT_MEDIA_PLAYLIST_MODULE_HEADER Required Header
 *   \#include <media_content.h>
 *
 * @section CAPI_CONTENT_MEDIA_PLAYLIST_MODULE_OVERVIEW Overview
 * A Playlist is a list of songs which can be played in some sequence i.e. sequential or shuffled order.
 * The Media Playlist API provides functions to insert, delete or updates a media playlist in the database.
 * 
 * For inserting new playlist (#media_playlist_h) in the database, call  #media_playlist_insert_to_db() function and call #media_playlist_delete_from_db() function
 * to delete a playlist from the database.\n
 * For adding a media item to the playlist, call #media_playlist_add_media() function, for removing a media item from the playlist, call 
 * #media_playlist_remove_media() function.\n
 * Finally, #media_playlist_update_to_db() function should be called so as to update the given item in the media database.
 *
 * @subsection CAPI_CONTENT_MEDIA_PLAYLIST_FOREACH_OPERATIONS Foreach Operations
 * <div><table class="doxtable">
 *     <tr>
 *        <th><b>FOREACH</b></th>
 *        <th><b>CALLBACK</b></th>
 *        <th><b>DESCRIPTION</b></th>
 *     </tr>
 *     <tr>
 *        <td>media_playlist_foreach_playlist_from_db()</td>
 *        <td>media_playlist_cb()</td>
 *        <td>Iterates through playlist</td>
 *     </tr>
 *     <tr>
 *        <td>media_playlist_foreach_media_from_db()</td>
 *        <td>media_info_cb()</td>
 *        <td>Iterates through playlist's items</td>
 *     </tr>
 * </table></div>
 *
 *
 */


/**
 * @defgroup CAPI_CONTENT_MEDIA_ALBUM_MODULE Media Album
 * @brief The Media Album API provides information related to album of media items.
 *
 * @ingroup CAPI_MEDIA_CONTENT_MODULE
 *
 * @section CAPI_CONTENT_MEDIA_ALBUM_MODULE_HEADER Required Header
 *   \#include <media_content.h>
 *
 * @section CAPI_CONTENT_MEDIA_ALBUM_MODULE_OVERVIEW Overview
 * An album is a logical collection or grouping of related audio files. It is also used for filtering media items.\n
 * The Media Album API allows to manage media albums which contains all video and audio items from
 * the same album.
 *
 * The API provides functions to get and search media items in album group.
 * For getting the count of media items associated with a given album, call #media_album_get_media_count_from_db() function.
 *
 * @subsection CAPI_CONTENT_MEDIA_ALBUM_FOREACH_OPERATIONS Foreach Operations
 * <div><table class="doxtable">
 *     <tr>
 *        <th><b>FOREACH</b></th>
 *        <th><b>CALLBACK</b></th>
 *        <th><b>DESCRIPTION</b></th>
 *     </tr>
 *     <tr>
 *        <td>media_album_foreach_album_from_db()</td>
 *        <td>media_album_cb()</td>
 *        <td>Iterates through albums</td>
 *     </tr>
 *     <tr>
 *        <td>media_album_foreach_media_from_db()</td>
 *        <td>media_info_cb()</td>
 *        <td>Iterates through album's items</td>
 *     </tr>
 * </table></div>
 *
 *
 */


/**
 * @defgroup CAPI_CONTENT_MEDIA_GROUP_MODULE Media Group
 * @brief The Media Group API provides information related to artist of media group.
 *
 * @ingroup CAPI_MEDIA_CONTENT_MODULE
 *
 * @section CAPI_CONTENT_MEDIA_GROUP_MODULE_HEADER Required Header
 *   \#include <media_content.h>
 *
 * @section CAPI_CONTENT_MEDIA_GROUP_MODULE_OVERVIEW Overview
 * A Media Group represents logical grouping of media files with respect to their group name. It is also used for filtering media items.\n
 *
 * The API provides functions to get and search image, video and audio items in media group. 
 * For getting the count of media items associated with a given group, call #media_group_get_media_count_from_db() function.
 *
 *
 * @subsection CAPI_CONTENT_MEDIA_GROUP_FOREACH_OPERATIONS Foreach Operations
 * <div><table class="doxtable">
 *	<tr>
 *	   <th><b>FOREACH</b></th>
 *	   <th><b>CALLBACK</b></th>
 *	   <th><b>DESCRIPTION</b></th>
 *	</tr>
 *	<tr>
 *	   <td>media_group_foreach_group_from_db()</td>
 *	   <td> media_group_cb()</td>
 *	   <td>Iterates through group</td>
 *	</tr>
 *	<tr>
 *	   <td>media_group_foreach_media_from_db()</td>
 *	   <td>media_info_cb()</td>
 *	   <td>Iterates through group's items</td>
 *	</tr>
 * </table></div>
 *
 *
 */


/**
 * @defgroup CAPI_CONTENT_MEDIA_IMAGE_MODULE Image Metadata
 * @brief The Image Metadata API provides functions that allow to get information about
 * stored image files.
 *
 * @ingroup CAPI_CONTENT_MEDIA_INFO_MODULE
 *
 * @section CAPI_CONTENT_MEDIA_IMAGE_MODULE_HEADER Required Header
 *   \#include <media_content.h>
 *
 * @section CAPI_CONTENT_MEDIA_IMAGE_MODULE_OVERVIEW Overview
 * The Image Metadata API provides functions to get basic information associated with image files:
 * - width
 * - height
 * - date taken (when image was created)
 * - image orientation\n
 *
 *<p>
 * For getting the image handle (#image_meta_h) from the media information (#media_info_h), call #media_info_get_image() function.\n
 * For getting the information related to image files stored in the device call the respective get functions e.g. to get the width of a image, call #image_meta_get_width() function and 
 * to get orientation (#media_content_orientation_e) of a image, call #image_meta_get_orientation() function and so on.\n
 * When the image handle is no longer needed, it should be destroyed by calling #image_meta_destroy() function.\n 
 *
 */




/**
 * @defgroup CAPI_CONTENT_MEDIA_VIDEO_META_MODULE Video Metadata
 * @brief The Video Metadata API provides functions to get information about video files present in the device.
 *
 * @ingroup CAPI_CONTENT_MEDIA_INFO_MODULE
 *
 * @section CAPI_CONTENT_MEDIA_VIDEO_META_MODULE_HEADER Required Header
 *   \#include <media_content.h>
 *
 * @section CAPI_CONTENT_MEDIA_VIDEO_META_MODULE_OVERVIEW Overview
 * The Video Metadata API provides functions to get information about video files present in the device.
 * Following information about video content (#video_meta_h )is provided:
 * - artist
 * - album
 * - genre
 * - composer
 * - year
 * - recorded date
 * - copyright
 * - track number
 * - bit rate
 * - width
 * - height 
 * - played count
 * - played time
 * - played position
 * \n
 * and others.\n
 *<p>
 * For getting the video handle (#video_meta_h) from the media information (#media_info_h), call the media_info_get_video() function.\n
 * For getting the information related to video files stored in the device call the respective get functions e.g. to get duration of the video file
 * call #video_meta_get_duration() function  and so on.\n
 * When the video handle is no longer needed, it should be destroyed by calling #video_meta_destroy() function. 
 *
 *
 *
 */


/**
 * @defgroup CAPI_CONTENT_MEDIA_BOOKMARK_MODULE Media Bookmark
 * @brief The Media Bookmark Information API provides functions to manage bookmark information on the media items.
 *
 * @ingroup CAPI_MEDIA_CONTENT_MODULE
 *
 * @section CAPI_CONTENT_MEDIA_BOOKMARKINFO_MODULE_HEADER Required Header
 *   \#include <media_content.h>
 *
 * @section CAPI_CONTENT_MEDIA_BOOKMARKINFO_MODULE_OVERVIEW Overview
 * A Bookmark allows you to mark interesting moment in a media(video and audio) to enable fast searching.
 * The Bookmark Information API  provides functions to get information about bookmarks associated with video and audio items.
 *
 * API allows to:
 * - get and filter existing bookmarks
 * - insert new bookmarks
 * - delete bookmarks
 *
 * For inserting a bookmark to media item, call media_bookmark_insert_to_db() function and for deleting already set bookmark from a media, call 
 * #media_bookmark_delete_from_db() function.
 * For retrieving time where the bookmark is placed on the media, call #media_bookmark_get_marked_time() function.
 *
 * @subsection CAPI_CONTENT_MEDIA_BOOKMARKINFO_FOREACH_OPERATIONS Foreach Operations
 * <div><table class="doxtable" >
 *     <tr>
 *        <th><b>FOREACH</b></th>
 *        <th><b>CALLBACK</b></th>
 *        <th><b>DESCRIPTION</b></th>
 *     </tr>
 *     <tr>
 *        <td>media_info_foreach_bookmark_from_db()</td>
 *        <td>media_bookmark_cb()</td>
 *        <td>Iterates through bookmarks</td>
 *     </tr>
 *</table></div>
 *
 *
 */

/**
 * @defgroup CAPI_CONTENT_MEDIA_INFO_MODULE Media Information
 * @brief The Media Information API provides functions to get information about media items
 * stored on an internal and external storage.
 *
 * @ingroup CAPI_MEDIA_CONTENT_MODULE
 *
 * @section CAPI_CONTENT_MEDIA_INFO_MODULE_HEADER Required Header
 *   \#include <media_content.h>
 *
 * @section CAPI_CONTENT_MEDIA_INFO_MODULE_OVERVIEW Overview
 *
 * The Media Information API provides functions to get basic information e.g. path, date, type etc about media items (#media_info_h) present in the device.
 * Media Information (#media_info_h) is a generalization of media content of any type (audio, image, video and others).
 *
 * Received information about media items can be the processed using dedicated APIs:
 * <table>
 * <tr>
 *    <th>API</th>
 *    <th>Description</th>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_CONTENT_MEDIA_INFO_MODULE </td>
 *    <td> Provides details about all items present in the device.\n
 * Provide functions to get information (e.g. title, size, mime type etc) about the files.</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_CONTENT_MEDIA_AUDIO_META_MODULE </td>
 *    <td> Provides details about audio items present in the device.\n
 * Provide functions to get information (e.g. genre, album, year, bitrate etc) about the audio files.</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_CONTENT_MEDIA_IMAGE_MODULE </td>
 *    <td> Provides details about image items present in the device.\n
 * Provide functions to get information (e.g. longitude, description, date etc) about the image files.</td>
 * </tr>
 * <tr>
 *    <td>@ref CAPI_CONTENT_MEDIA_VIDEO_META_MODULE </td>
  *    <td> Provides details about video items present in the device.\n
 * Provide functions to get information (e.g. title, duration, date etc) about the video files.</td>
 * </tr>
 * </table>
 *
 * Video and Audio information can be further processed with respect to its properties i.e. playlist, album, using their respective APIs.
 * - @ref CAPI_CONTENT_MEDIA_PLAYLIST_MODULE 
 * - @ref CAPI_CONTENT_MEDIA_ALBUM_MODULE 
 * - @ref CAPI_CONTENT_MEDIA_BOOKMARK_MODULE  
 *
 *
 * @subsection CAPI_CONTENT_MEDIA_INFO_FOREACH_OPERATIONS Foreach Operations
 * <div><table class="doxtable">
 *     <tr>
 *        <th><b>FOREACH</b></th>
 *        <th><b>CALLBACK</b></th>
 *        <th><b>DESCRIPTION</b></th>
 *     </tr>
 *     <tr>
 *        <td>media_info_foreach_media_from_db()</td>
 *        <td>media_info_cb()</td>
 *        <td>Iterates through items</td>
 *     </tr>
 *     <tr>
 *        <td>media_info_foreach_tag_from_db()</td>
 *        <td>media_tag_cb()</td>
 *        <td>Iterate through tags</td>
 *     </tr>
 *     <tr>
 *        <td>media_info_foreach_bookmark_from_db()</td>
 *        <td>media_bookmark_cb()</td>
 *        <td>Iterate through bookmark</td>
 *     </tr> 
 * </table></div>
 *
 *
 *
 */

#endif /* __TIZEN_MEDIA_CONTENT_DOC_H__ */
