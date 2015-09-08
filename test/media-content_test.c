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

#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <media_content.h>
#include <media_info_private.h>
#include <dlog.h>
#include <pthread.h>
#include <glib.h>

filter_h g_filter = NULL;
filter_h g_filter_g = NULL;	//filter for group like folder, tag, playlist, album, year ...

GMainLoop *g_loop = NULL;
static int g_cnt = 0;
static int g_media_cnt = 0;

#define test_audio_id	"0f999626-6218-450c-a4ad-181a3bab6ebf"
#define test_video_id	"c1a92494-cc5b-4d74-aa7d-253199234548"
#define test_image_id "db1c184c-6f31-43b4-b924-8c00ac5b6197"
media_folder_h g_folder = NULL;

bool get_audio_meta(audio_meta_h audio)
{
	char *c_value = NULL;
	int i_value = 0;
	time_t t_value = 0;
	int ret = MEDIA_CONTENT_ERROR_NONE;

	media_content_debug("=== audio meta ===");

	ret = audio_meta_get_media_id(audio, &c_value);
	if(ret != MEDIA_CONTENT_ERROR_NONE)
		media_content_error("error when get meta : [%d]", ret);
	media_content_debug("audio_id : [%s]", c_value);
	SAFE_FREE(c_value);

	ret = audio_meta_get_album(audio, &c_value);
	if(ret != MEDIA_CONTENT_ERROR_NONE)
		media_content_error("error when get meta : [%d]", ret);
	media_content_debug("album : [%s]", c_value);
	SAFE_FREE(c_value);

	ret = audio_meta_get_artist(audio, &c_value);
	if(ret != MEDIA_CONTENT_ERROR_NONE)
		media_content_error("error when get meta : [%d]", ret);
	media_content_debug("artist : [%s]", c_value);
	SAFE_FREE(c_value);

	ret = audio_meta_get_album_artist(audio, &c_value);
	if(ret != MEDIA_CONTENT_ERROR_NONE)
		media_content_error("error when get meta : [%d]", ret);
	media_content_debug("album_artist : [%s]", c_value);
	SAFE_FREE(c_value);

	ret = audio_meta_get_genre(audio, &c_value);
	if(ret != MEDIA_CONTENT_ERROR_NONE)
		media_content_error("error when get meta : [%d]", ret);
	media_content_debug("genre : [%s]", c_value);
	SAFE_FREE(c_value);

	ret = audio_meta_get_composer(audio, &c_value);
	if(ret != MEDIA_CONTENT_ERROR_NONE)
		media_content_error("error when get meta : [%d]", ret);
	media_content_debug("composer : [%s]", c_value);
	SAFE_FREE(c_value);

	ret = audio_meta_get_year(audio, &c_value);
	if(ret != MEDIA_CONTENT_ERROR_NONE)
		media_content_error("error when get meta : [%d]", ret);
	media_content_debug("year : [%s]", c_value);
	SAFE_FREE(c_value);

	ret = audio_meta_get_recorded_date(audio, &c_value);
	if(ret != MEDIA_CONTENT_ERROR_NONE)
		media_content_error("error when get meta : [%d]", ret);
	media_content_debug("recorded_date : [%s]", c_value);
	SAFE_FREE(c_value);

	ret = audio_meta_get_copyright(audio, &c_value);
	if(ret != MEDIA_CONTENT_ERROR_NONE)
		media_content_error("error when get meta : [%d]", ret);
	media_content_debug("copyright : [%s]", c_value);
	SAFE_FREE(c_value);

	ret = audio_meta_get_track_num(audio, &c_value);
	if(ret != MEDIA_CONTENT_ERROR_NONE)
		media_content_error("error when get meta : [%d]", ret);
	media_content_debug("track_num : [%s]", c_value);
	SAFE_FREE(c_value);

	ret = audio_meta_get_bit_rate(audio, &i_value);
	if(ret != MEDIA_CONTENT_ERROR_NONE)
		media_content_error("error when get meta : [%d]", ret);
	media_content_debug("bitrate : [%d]", i_value);

	ret = audio_meta_get_sample_rate(audio, &i_value);
	if(ret != MEDIA_CONTENT_ERROR_NONE)
		media_content_error("error when get meta : [%d]", ret);
	media_content_debug("samplerate : [%d]", i_value);

	ret = audio_meta_get_channel(audio, &i_value);
	if(ret != MEDIA_CONTENT_ERROR_NONE)
		media_content_error("error when get meta : [%d]", ret);
	media_content_debug("channel : [%d]", i_value);

	ret = audio_meta_get_duration(audio, &i_value);
	if(ret != MEDIA_CONTENT_ERROR_NONE)
		media_content_error("error when get meta : [%d]", ret);
	media_content_debug("duration : [%d]", i_value);

	ret = audio_meta_get_played_count(audio, &i_value);
	if(ret != MEDIA_CONTENT_ERROR_NONE)
		media_content_error("error when get meta : [%d]", ret);
	media_content_debug("played_count : [%d]", i_value);

	ret = audio_meta_get_played_time(audio, &t_value);
	if(ret != MEDIA_CONTENT_ERROR_NONE)
		media_content_error("error when get meta : [%d]", ret);
	media_content_debug("played_time : [%d]", t_value);

	ret = audio_meta_get_played_position(audio, &i_value);
	if(ret != MEDIA_CONTENT_ERROR_NONE)
		media_content_error("error when get meta : [%d]", ret);
	media_content_debug("played_position : [%d]", i_value);

	return true;
}

bool get_video_meta(video_meta_h video)
{
	char *c_value = NULL;
	int i_value = 0;
	time_t t_value = 0;
	int ret = MEDIA_CONTENT_ERROR_NONE;

	media_content_debug("=== video meta ===");

	ret = video_meta_get_media_id(video, &c_value);
	if(ret != MEDIA_CONTENT_ERROR_NONE)
		media_content_error("error when get meta : [%d]", ret);
	media_content_debug("video_id : [%s]", c_value);
	SAFE_FREE(c_value);

	ret = video_meta_get_album(video, &c_value);
	if(ret != MEDIA_CONTENT_ERROR_NONE)
		media_content_error("error when get meta : [%d]", ret);
	media_content_debug("album : [%s]", c_value);
	SAFE_FREE(c_value);

	ret = video_meta_get_artist(video, &c_value);
	if(ret != MEDIA_CONTENT_ERROR_NONE)
		media_content_error("error when get meta : [%d]", ret);
	media_content_debug("artist : [%s]", c_value);
	SAFE_FREE(c_value);

	ret = video_meta_get_album_artist(video, &c_value);
	if(ret != MEDIA_CONTENT_ERROR_NONE)
		media_content_error("error when get meta : [%d]", ret);
	media_content_debug("album_artist : [%s]", c_value);
	SAFE_FREE(c_value);

	ret = video_meta_get_genre(video, &c_value);
	if(ret != MEDIA_CONTENT_ERROR_NONE)
		media_content_error("error when get meta : [%d]", ret);
	media_content_debug("genre : [%s]", c_value);
	SAFE_FREE(c_value);

	ret = video_meta_get_composer(video, &c_value);
	if(ret != MEDIA_CONTENT_ERROR_NONE)
		media_content_error("error when get meta : [%d]", ret);
	media_content_debug("omposer : [%s]", c_value);
	SAFE_FREE(c_value);

	ret = video_meta_get_year(video, &c_value);
	if(ret != MEDIA_CONTENT_ERROR_NONE)
		media_content_error("error when get meta : [%d]", ret);
	media_content_debug("year : [%s]", c_value);
	SAFE_FREE(c_value);

	ret = video_meta_get_recorded_date(video, &c_value);
	if(ret != MEDIA_CONTENT_ERROR_NONE)
		media_content_error("error when get meta : [%d]", ret);
	media_content_debug("recorded_date : [%s]", c_value);
	SAFE_FREE(c_value);

	ret = video_meta_get_copyright(video, &c_value);
	if(ret != MEDIA_CONTENT_ERROR_NONE)
		media_content_error("error when get meta : [%d]", ret);
	media_content_debug("copyright : [%s]", c_value);
	SAFE_FREE(c_value);

	ret = video_meta_get_track_num(video, &c_value);
	if(ret != MEDIA_CONTENT_ERROR_NONE)
		media_content_error("error when get meta : [%d]", ret);
	media_content_debug("track_num : [%s]", c_value);
	SAFE_FREE(c_value);

	ret = video_meta_get_bit_rate(video, &i_value);
	if(ret != MEDIA_CONTENT_ERROR_NONE)
		media_content_error("error when get meta : [%d]", ret);
	media_content_debug("bitrate : [%d]", i_value);

	ret = video_meta_get_duration(video, &i_value);
	if(ret != MEDIA_CONTENT_ERROR_NONE)
		media_content_error("error when get meta : [%d]", ret);
	media_content_debug("duration : [%d]", i_value);

	ret = video_meta_get_width(video, &i_value);
	if(ret != MEDIA_CONTENT_ERROR_NONE)
		media_content_error("error when get meta : [%d]", ret);
	media_content_debug("width : [%d]", i_value);

	ret = video_meta_get_height(video, &i_value);
	if(ret != MEDIA_CONTENT_ERROR_NONE)
		media_content_error("error when get meta : [%d]", ret);
	media_content_debug("height : [%d]", i_value);

	ret = video_meta_get_played_count(video, &i_value);
	if(ret != MEDIA_CONTENT_ERROR_NONE)
		media_content_error("error when get meta : [%d]", ret);
	media_content_debug("played_count : [%d]", i_value);

	ret = video_meta_get_played_time(video, &t_value);
	if(ret != MEDIA_CONTENT_ERROR_NONE)
		media_content_error("error when get meta : [%d]", ret);
	media_content_debug("played_time : [%d]", t_value);

	ret = video_meta_get_played_position(video, &i_value);
	if(ret != MEDIA_CONTENT_ERROR_NONE)
		media_content_error("error when get meta : [%d]", ret);
	media_content_debug("played_position : [%d]", i_value);

	return true;
}

bool gallery_folder_list_cb(media_folder_h folder, void *user_data)
{
	media_folder_h new_folder = NULL;
	media_folder_clone(&new_folder, folder);

	GList **list = (GList**)user_data;
	*list = g_list_append(*list, new_folder);

	return true;
}

bool gallery_media_item_cb(media_info_h media, void *user_data)
{
	media_info_h new_media = NULL;
	media_info_clone(&new_media, media);

	GList **list = (GList**)user_data;
	*list = g_list_append(*list, new_media);

	return true;
}

bool gallery_tag_item_cb(media_tag_h tag, void *user_data)
{
	media_tag_h new_tag = NULL;
	media_tag_clone(&new_tag, tag);

	GList **list = (GList**)user_data;
	*list = g_list_append(*list, new_tag);

	return true;
}

bool gallery_bookmarks_cb(media_bookmark_h bookmark, void *user_data)
{
	media_bookmark_h new_bm = NULL;
	int ret = MEDIA_CONTENT_ERROR_NONE;

	ret = media_bookmark_clone(&new_bm, bookmark);
	if(ret != MEDIA_CONTENT_ERROR_NONE)
		media_content_error("error media_bookmark_clone : [%d]", ret);

	GList **list = (GList**)user_data;
	*list = g_list_append(*list, new_bm);

	return true;
}

bool media_item_cb(media_info_h media, void *user_data)
{
	char *c_value = NULL;
	char *media_id = NULL;
	media_content_type_e media_type = 0;
	int ret = MEDIA_CONTENT_ERROR_NONE;

	if(media == NULL)
	{
		media_content_debug("NO Item");
		return true;
	}

	ret = media_info_get_media_type(media, &media_type);
	if(ret != MEDIA_CONTENT_ERROR_NONE)
		media_content_error("error when get info : [%d]", ret);
	media_content_debug("media_type : [%d]", media_type);

	ret = media_info_get_media_id(media, &media_id);
	if(ret != MEDIA_CONTENT_ERROR_NONE)
		media_content_error("error when get info : [%d]", ret);
	media_content_debug("media_id : [%s]", media_id);

	ret = media_info_get_file_path(media, &c_value);
	if(ret != MEDIA_CONTENT_ERROR_NONE)
		media_content_error("error when get info : [%d]", ret);
	media_content_debug("file_path : [%s]", c_value);
	SAFE_FREE(c_value);

#if 0
	int i_value = 0;
	time_t t_value = 0;
	bool b_value = false;
	unsigned long long size = 0;

	if(media_type == MEDIA_CONTENT_TYPE_MUSIC)
	{
		audio_meta_h audio;

		if(media_info_get_audio(media, &audio) == MEDIA_CONTENT_ERROR_NONE)
		{
			get_audio_meta(audio);
			ret = audio_meta_destroy(audio);
			if(ret != MEDIA_CONTENT_ERROR_NONE)
				media_content_error("error audio_meta_destroy : [%d]", ret);
		}
		else
			media_content_error("[audio_error]");

	}
	else if(media_type == MEDIA_CONTENT_TYPE_IMAGE)
	{
		image_meta_h image;
		media_content_orientation_e orientation = 0;
		bool is_burst_shot = false;
		char *burst_id = NULL;
		char *weather = NULL;

		if(media_info_get_image(media, &image) == MEDIA_CONTENT_ERROR_NONE)
		{
			ret = image_meta_get_orientation(image, &orientation);
			if(ret != MEDIA_CONTENT_ERROR_NONE)
				media_content_error("error image_meta_get_orientation : [%d]", ret);
			else
				media_content_debug("[image] orientation : %d", orientation);

			ret = image_meta_is_burst_shot(image, &is_burst_shot);
			if(ret != MEDIA_CONTENT_ERROR_NONE)
				media_content_error("error image_meta_is_burst_shot : [%d]", ret);
			if(is_burst_shot)
			{
				ret = image_meta_get_burst_id(image, &burst_id);
				if(ret != MEDIA_CONTENT_ERROR_NONE)
					media_content_error("error image_meta_get_burst_id : [%d]", ret);
				else
					media_content_debug("[image] burst_id : [%s]", burst_id);

				SAFE_FREE(burst_id);
			}

			ret = image_meta_destroy(image);
			if(ret != MEDIA_CONTENT_ERROR_NONE)
				media_content_error("error image_meta_destroy : [%d]", ret);
		}
		else
			media_content_error("[image_error]");

	}
	else if(media_type == MEDIA_CONTENT_TYPE_VIDEO)
	{
		video_meta_h video;

		if(media_info_get_video(media, &video) == MEDIA_CONTENT_ERROR_NONE)
		{
			get_video_meta(video);
			ret = video_meta_destroy(video);
			if(ret != MEDIA_CONTENT_ERROR_NONE)
				media_content_error("error video_meta_destroy : [%d]", ret);
		}
		else
			media_content_error("[video_error]");

	}
	else
	{
		media_content_debug("Other Content");
	}

	media_content_debug("=== media_info ===");
	ret = media_info_get_file_path(media, &c_value);
	if(ret != MEDIA_CONTENT_ERROR_NONE)
		media_content_error("error when get info : [%d]", ret);
	media_content_debug("file_path : [%s]", c_value);
	SAFE_FREE(c_value);

	ret = media_info_get_display_name(media, &c_value);
	if(ret != MEDIA_CONTENT_ERROR_NONE)
		media_content_error("error when get info : [%d]", ret);
	media_content_debug("display_name : [%s]", c_value);
	SAFE_FREE(c_value);

	ret = media_info_get_mime_type(media, &c_value);
	if(ret != MEDIA_CONTENT_ERROR_NONE)
		media_content_error("error when get info : [%d]", ret);
	media_content_debug("mime_type : [%s]", c_value);
	SAFE_FREE(c_value);

	ret = media_info_get_thumbnail_path(media, &c_value);
	if(ret != MEDIA_CONTENT_ERROR_NONE)
		media_content_error("error when get info : [%d]", ret);
	media_content_debug("thumbnail_path : [%s]", c_value);
	SAFE_FREE(c_value);

	ret = media_info_get_description(media, &c_value);
	if(ret != MEDIA_CONTENT_ERROR_NONE)
		media_content_error("error when get info : [%d]", ret);
	media_content_debug("description : [%s]", c_value);
	SAFE_FREE(c_value);

	ret = media_info_get_author(media, &c_value);
	if(ret != MEDIA_CONTENT_ERROR_NONE)
		media_content_error("error when get info : [%d]", ret);
	media_content_debug("author : [%s]", c_value);
	SAFE_FREE(c_value);

	ret = media_info_get_provider(media, &c_value);
	if(ret != MEDIA_CONTENT_ERROR_NONE)
		media_content_error("error when get info : [%d]", ret);
	media_content_debug("provider : [%s]", c_value);
	SAFE_FREE(c_value);

	ret = media_info_get_content_name(media, &c_value);
	if(ret != MEDIA_CONTENT_ERROR_NONE)
		media_content_error("error when get info : [%d]", ret);
	media_content_debug("content_name : [%s]", c_value);
	SAFE_FREE(c_value);

	ret = media_info_get_category(media, &c_value);
	if(ret != MEDIA_CONTENT_ERROR_NONE)
		media_content_error("error when get info : [%d]", ret);
	media_content_debug("category : [%s]", c_value);
	SAFE_FREE(c_value);

	ret = media_info_get_location_tag(media, &c_value);
	if(ret != MEDIA_CONTENT_ERROR_NONE)
		media_content_error("error when get info : [%d]", ret);
	media_content_debug("location_tag : [%s]", c_value);
	SAFE_FREE(c_value);

	ret = media_info_get_age_rating(media, &c_value);
	if(ret != MEDIA_CONTENT_ERROR_NONE)
		media_content_error("error when get info : [%d]", ret);
	media_content_debug("age_rating : [%s]", c_value);
	SAFE_FREE(c_value);

	ret = media_info_get_keyword(media, &c_value);
	if(ret != MEDIA_CONTENT_ERROR_NONE)
		media_content_error("error when get info : [%d]", ret);
	media_content_debug("keyword : [%s]", c_value);
	SAFE_FREE(c_value);

	ret = media_info_get_size(media, &size);
	if(ret != MEDIA_CONTENT_ERROR_NONE)
		media_content_error("error when get info : [%d]", ret);
	media_content_debug("size : [%lld]", size);

	ret = media_info_get_added_time(media, &t_value);
	if(ret != MEDIA_CONTENT_ERROR_NONE)
		media_content_error("error when get info : [%d]", ret);
	media_content_debug("added_time : [%d]", t_value);

	ret = media_info_get_modified_time(media, &t_value);
	if(ret != MEDIA_CONTENT_ERROR_NONE)
		media_content_error("error when get info : [%d]", ret);
	media_content_debug("modified_time : [%d]", t_value);

	ret = media_info_get_timeline(media, &t_value);
	if(ret != MEDIA_CONTENT_ERROR_NONE)
		media_content_error("error when get info : [%d]", ret);
	media_content_debug("timeline : [%d]", t_value);

	ret = media_info_get_rating(media, &i_value);
	if(ret != MEDIA_CONTENT_ERROR_NONE)
		media_content_error("error when get info : [%d]", ret);
	media_content_debug("rating : [%d]", i_value);

	ret = media_info_get_favorite(media, &b_value);
	if(ret != MEDIA_CONTENT_ERROR_NONE)
		media_content_error("error when get info : [%d]", ret);
	media_content_debug("favorite : [%d]", b_value);

	ret = media_info_is_drm(media, &b_value);
	if(ret != MEDIA_CONTENT_ERROR_NONE)
		media_content_error("error when get info : [%d]", ret);
	media_content_debug("is_drm : [%d]", b_value);

	ret = media_info_set_weather(media, "Sunny");
	if(ret != MEDIA_CONTENT_ERROR_NONE) {
		media_content_error("Fail to set weather");
		return ret;

	ret = media_info_get_weather(media, &c_value);
	if(ret != MEDIA_CONTENT_ERROR_NONE)
		media_content_error("error when get info : [%d]", ret);
	media_content_debug("weather : [%s]", c_value);
	SAFE_FREE(c_value);

	/* Media server can't update when another db handle holds DB connection by sqlite3_prepare */
	//ret = media_info_set_location_tag(media, "Test location tag");
	//media_info_update_to_db(media);
	SAFE_FREE(media_id);
#endif
	SAFE_FREE(media_id);
	return true;
}

bool folder_list_cb(media_folder_h folder, void *user_data)
{
	int item_count;
	char *folder_id = NULL;
	char *folder_path = NULL;
	char *folder_name = NULL;
	media_content_storage_e storage_type;
	bool ret;
	media_folder_h *_folder = (media_folder_h*)user_data;

	if(folder != NULL)
	{
		if(_folder != NULL)
			media_folder_clone(_folder, folder);

		if(media_folder_get_folder_id(folder, &folder_id) != MEDIA_CONTENT_ERROR_NONE)
		{
			media_content_error("[ERROR] media_folder_get_folder_id is failed");
			return false;
		}
		media_content_debug("folder_id = [%s]", folder_id);

		if(media_folder_get_path(folder, &folder_path) != MEDIA_CONTENT_ERROR_NONE)
		{
			SAFE_FREE(folder_id);
			media_content_error("[ERROR] media_folder_get_path is failed");
			return false;
		}
		media_content_debug("folder_path = [%s]", folder_path);
		SAFE_FREE(folder_path);

		if(media_folder_get_name(folder, &folder_name) != MEDIA_CONTENT_ERROR_NONE)
		{
			SAFE_FREE(folder_id);
			SAFE_FREE(folder_path);
			media_content_error("[ERROR] media_folder_get_name is failed");
			return false;
		}
		media_content_debug("folder_name = [%s]", folder_name);
		SAFE_FREE(folder_name);

		if(media_folder_get_storage_type(folder, &storage_type) != MEDIA_CONTENT_ERROR_NONE)
		{
			SAFE_FREE(folder_id);
			SAFE_FREE(folder_path);
			SAFE_FREE(folder_name);
			media_content_error("[ERROR] media_folder_get_storage_type is failed");
			return false;
		}
		media_content_debug("storage_type = [%d]", storage_type);

		if(media_folder_get_media_count_from_db(folder_id, g_filter, &item_count) != MEDIA_CONTENT_ERROR_NONE)
		{
			SAFE_FREE(folder_id);
			SAFE_FREE(folder_path);
			SAFE_FREE(folder_name);
			media_content_error("[ERROR] media_folder_get_media_count_from_db is failed");
			return false;
		}

		if(media_folder_foreach_media_from_db(folder_id, g_filter, media_item_cb, NULL) != MEDIA_CONTENT_ERROR_NONE)
		{
			SAFE_FREE(folder_id);
			SAFE_FREE(folder_path);
			SAFE_FREE(folder_name);
			media_content_error("[ERROR] media_folder_foreach_media_from_db is failed");
			return false;
		}

		SAFE_FREE(folder_id);
		SAFE_FREE(folder_path);
		SAFE_FREE(folder_name);

		ret = true;
	}
	else
	{
		ret = false;
	}

	return ret;
}

bool test_album_from_db(int album_id)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	media_album_h album_h;
	int test_album_id = 0;
	char *album_name = NULL;
	char *artist = NULL;

	ret = media_album_get_album_from_db(album_id, &album_h);
	if(ret != MEDIA_CONTENT_ERROR_NONE)
	{
		media_content_error("error when get album");
		return false;
	}

	if(media_album_get_album_id(album_h, &test_album_id) != MEDIA_CONTENT_ERROR_NONE)
	{
		media_album_destroy(album_h);
		return false;
	}

	media_content_debug("test_album_id : [%d]", test_album_id);

	if(media_album_get_name(album_h, &album_name) != MEDIA_CONTENT_ERROR_NONE)
	{
		media_album_destroy(album_h);
		return false;
	}

	media_content_debug("album_name : [%s]", album_name);

	if(media_album_get_artist(album_h, &artist) != MEDIA_CONTENT_ERROR_NONE)
	{
		media_album_destroy(album_h);
		/* fix prevent: Resource Leak */
		SAFE_FREE(album_name);
		return false;
	}

	media_content_debug("artist : [%s]", artist);

	SAFE_FREE(album_name);
	SAFE_FREE(artist);

	media_album_destroy(album_h);

	return true;
}

bool playlist_list_cb(media_playlist_h playlist, void *user_data)
{
	int playlist_id = 0;
	char *playlist_name = NULL;
	media_playlist_h playlist_h;
	char *playlist_thumbnail_path = NULL;

	media_content_debug("playlist_list_cb ======");

	GList **list = (GList**)user_data;

	if(playlist == NULL)
	{
		media_content_debug(" playlist handle is NULL");
		return false;
	}

	media_playlist_get_playlist_id(playlist, &playlist_id);
	media_content_debug("playlist_id : %d", playlist_id);

	if(user_data != NULL)
		*list = g_list_append(*list, (gpointer)playlist_id);

	media_playlist_get_name(playlist, &playlist_name);
	media_content_debug("playlist_name : %s", playlist_name);
	SAFE_FREE(playlist_name);

	media_playlist_get_thumbnail_path(playlist, &playlist_thumbnail_path);
	media_content_debug("playlist_thumbnail_path : %s", playlist_thumbnail_path);
	SAFE_FREE(playlist_thumbnail_path);

	media_playlist_get_playlist_from_db(playlist_id, &playlist_h);

	media_playlist_destroy(playlist_h);

	return true;
}

bool tag_list_cb(media_tag_h tag, void *user_data)
{
	int tag_id = 0;
	char *tag_name = NULL;
	int ret = MEDIA_CONTENT_ERROR_NONE;

	if(tag == NULL)
	{
		media_content_debug(" tag handle is NULL");
		return false;
	}

	ret = media_tag_get_tag_id(tag, &tag_id);
	if(ret != MEDIA_CONTENT_ERROR_NONE)
		media_content_error("error media_tag_get_tag_id : [%d]", ret);
	else
		media_content_debug("tag_id : %d", tag_id);

	ret = media_tag_get_name(tag, &tag_name);
	if(ret != MEDIA_CONTENT_ERROR_NONE)
		media_content_error("error media_tag_get_name : [%d]", ret);
	else
		media_content_debug("tag_name : %s", tag_name);
	SAFE_FREE(tag_name);

	return true;
}

bool bookmarks_cb(media_bookmark_h bookmark, void *user_data)
{
	media_bookmark_h *_bookmark = (media_bookmark_h*)bookmark;
	int ret = MEDIA_CONTENT_ERROR_NONE;

	if(_bookmark != NULL)
	{
		char *name = NULL;
		time_t time = 0;
		int bookmark_id = 0;

		ret = media_bookmark_get_bookmark_id(bookmark, &bookmark_id);
		if(ret != MEDIA_CONTENT_ERROR_NONE)
			media_content_error("error media_bookmark_get_bookmark_id : [%d]", ret);
		else
			media_content_debug("bookmark_id : %d", bookmark_id);

		ret = media_bookmark_get_thumbnail_path(bookmark, &name);
		if(ret != MEDIA_CONTENT_ERROR_NONE)
			media_content_error("error media_bookmark_get_thumbnail_path : [%d]", ret);
		else
			media_content_debug("bookmark thumbnail_path : %s", name);
		SAFE_FREE(name);

		ret = media_bookmark_get_marked_time(bookmark, &time);
		if(ret != MEDIA_CONTENT_ERROR_NONE)
			media_content_error("error media_bookmark_get_marked_time : [%d]", ret);
		else
			media_content_debug("bookmark marked_time : %d", time);
	}

	return true;
}

bool album_list_cb(media_album_h album, void *user_data)
{
	int album_id = 0;
	char *album_name = NULL;
	char *artist = NULL;
	char *album_art = NULL;
	int media_count = 0;
	int ret = MEDIA_CONTENT_ERROR_NONE;
	filter_h filter = NULL;

	/*Set Filter*/
	char *condition = "MEDIA_TYPE=3";	/*MEDIA_TYPE 0-image, 1-video, 2-sound, 3-music, 4-other*/

	ret = media_filter_create(&filter);
	if(ret != MEDIA_CONTENT_ERROR_NONE) {
		media_content_error("Fail to create filter");
		return ret;
	}
	ret = media_filter_set_condition(filter, condition, MEDIA_CONTENT_COLLATE_DEFAULT);
	if(ret != MEDIA_CONTENT_ERROR_NONE) {
		media_filter_destroy(filter);
		media_content_error("Fail to set condition");
		return ret;
	}
	ret = media_filter_set_order(filter, MEDIA_CONTENT_ORDER_ASC, MEDIA_TITLE, MEDIA_CONTENT_COLLATE_NOCASE);
	if(ret != MEDIA_CONTENT_ERROR_NONE) {
		media_filter_destroy(filter);
		media_content_error("Fail to set order");
		return ret;
	}

	if(album != NULL)
	{
		if(media_album_get_album_id(album, &album_id) != MEDIA_CONTENT_ERROR_NONE)
		{
			media_filter_destroy(filter);
			return false;
		}

		media_content_debug("album_id : [%d]", album_id);

		if(media_album_get_name(album, &album_name) != MEDIA_CONTENT_ERROR_NONE)
		{
			media_filter_destroy(filter);
			return false;
		}

		media_content_debug("album_name : [%s]", album_name);
		SAFE_FREE(album_name);

		if(media_album_get_artist(album, &artist) != MEDIA_CONTENT_ERROR_NONE)
		{
			media_filter_destroy(filter);
			return false;
		}

		media_content_debug("artist : [%s]", artist);
		SAFE_FREE(artist);

		if(media_album_get_album_art(album, &album_art) != MEDIA_CONTENT_ERROR_NONE)
		{
			media_filter_destroy(filter);
			return false;
		}

		media_content_debug("album_art : [%s]", album_art);
		SAFE_FREE(album_art);

		if(media_album_get_media_count_from_db(album_id, filter, &media_count) != MEDIA_CONTENT_ERROR_NONE)
		{
			media_filter_destroy(filter);
			return false;
		}

		media_content_debug("media_count : [%d]", media_count);

		if(media_album_foreach_media_from_db(album_id, filter, media_item_cb, NULL) != MEDIA_CONTENT_ERROR_NONE)
		{
			media_filter_destroy(filter);
			return false;
		}

		test_album_from_db(album_id);
	}
	else
	{
		media_content_error("album item not Found!!");
	}

	media_filter_destroy(filter);

	return true;
}

bool group_list_cb(const char *group_name, void *user_data)
{
	int media_count = 0;
	int *idx = user_data;

	media_content_debug("group item : [%s] [%d]", group_name, *idx);

	if(media_group_get_media_count_from_db(group_name, *idx, g_filter, &media_count) != MEDIA_CONTENT_ERROR_NONE)
		return false;

	media_content_debug("media_count : [%d]", media_count);

	if(media_group_foreach_media_from_db(group_name, *idx, g_filter, media_item_cb, NULL) != MEDIA_CONTENT_ERROR_NONE)
		return false;

	return true;
}

bool playlist_item_cb(int playlist_member_id, media_info_h media, void *user_data)
{
	media_content_debug("playlist_member_id : [%d]", playlist_member_id);

	GList **list = (GList**)user_data;
	*list = g_list_append(*list, (gpointer)playlist_member_id);

	//media_item_cb(media, user_data);

	return true;
}

int test_filter_create(void)
{
	media_content_debug("\n============Filter Create============\n\n");

	int ret = MEDIA_CONTENT_ERROR_NONE;

	/* Filter for media */
	char *condition = "MEDIA_TYPE=3";	/*MEDIA_TYPE 0-image, 1-video, 2-sound, 3-music, 4-other*/
	//char *condition = "MEDIA_TYPE IS NOT 0 AND MEDIA_DESCRIPTION IS NOT NULL";	/*MEDIA_TYPE 0-image, 1-video, 2-sound, 3-music, 4-other*/

	ret = media_filter_create(&g_filter);

	/* Set condition and collate
	 * Condition string : You can make where statement of sql.
	 * Colation : You can use collation when comparing.
	 * Ex) In case of FILE_NAME='Samsung' as condition string,
	 *     if you want to compare with NOCASE collation,
	 *     call media_filter_set_condition(g_filter, condition, MEDIA_CONTENT_COLLATE_NOCASE);
	 *     if you want to compare in case-sensitive,
	 *     call media_filter_set_condition(g_filter, condition, MEDIA_CONTENT_COLLATE_DEFAULT);
	 */
	ret = media_filter_set_condition(g_filter, condition, MEDIA_CONTENT_COLLATE_DEFAULT);

	/* Collation of ordering
	 * If you want to get list, which is sorted by NOCASE,
	 * call media_filter_set_order(g_filter, MEDIA_CONTENT_ORDER_ASC, MEDIA_ARTIST, MEDIA_CONTENT_COLLATE_NOCASE);
	 * Or,
	 * call media_filter_set_order(g_filter, MEDIA_CONTENT_ORDER_ASC, MEDIA_ARTIST, MEDIA_CONTENT_COLLATE_DEFAULT);
	 */
	/* Able to set multi column to set order */
	//ret = media_filter_set_order(g_filter, MEDIA_CONTENT_ORDER_ASC, "MEDIA_MODIFIED_TIME, MEDIA_DISPLAY_NAME", MEDIA_CONTENT_COLLATE_DEFAULT);
	ret = media_filter_set_order(g_filter, MEDIA_CONTENT_ORDER_ASC, MEDIA_ARTIST, MEDIA_CONTENT_COLLATE_DEFAULT);
	//ret = media_filter_set_order(g_filter, MEDIA_CONTENT_ORDER_ASC, MEDIA_TRACK_NUM_INT, MEDIA_CONTENT_COLLATE_DEFAULT);

	/* Filter for group */
	char *g_condition = "TAG_NAME like \"\%my\%\"";
	//char *g_condition = "BOOKMARK_MARKED_TIME > 300";

	ret = media_filter_create(&g_filter_g);

	ret = media_filter_set_condition(g_filter_g, g_condition, MEDIA_CONTENT_COLLATE_DEFAULT);
	ret = media_filter_set_order(g_filter_g, MEDIA_CONTENT_ORDER_DESC, TAG_NAME, MEDIA_CONTENT_COLLATE_DEFAULT);

	return ret;
}

int test_filter_destroy(void)
{
	media_content_debug("\n============Filter Create============\n\n");

	int ret = MEDIA_CONTENT_ERROR_NONE;

	ret = media_filter_destroy(g_filter);

	return ret;
}

int test_connect_database(void)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;

	media_content_debug("\n============DB Connection Test============\n\n");

	ret = media_content_connect();

	if(ret == MEDIA_CONTENT_ERROR_NONE)
	{
		media_content_debug("connection is success\n\n");
	}
	else
		media_content_error("connection is failed\n\n");

	return ret;
}

int test_gallery_scenario(void)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	int i;
	filter_h filter = NULL;

	int count;
	GList *folder_list = NULL;
	media_folder_h folder_handle = NULL;

	/* First, Get folder list */
	ret = media_folder_foreach_folder_from_db(filter, gallery_folder_list_cb, &folder_list);
	if(ret != MEDIA_CONTENT_ERROR_NONE) {
		media_content_error("media_folder_foreach_folder_from_db failed: %d", ret);
		return -1;
	} else {
		media_content_debug("media_folder_foreach_folder_from_db success!!");
		char *folder_id = NULL;
		char *folder_name = NULL;
		char *folder_path = NULL;

		for(i = 0; i < g_list_length(folder_list); i++) {
			folder_handle = (media_folder_h)g_list_nth_data(folder_list, i);

			ret = media_folder_get_folder_id(folder_handle, &folder_id);
			if(ret != MEDIA_CONTENT_ERROR_NONE)
				media_content_error("media_folder_get_folder_id failed: %d", ret);
			ret = media_folder_get_name(folder_handle, &folder_name);
			if(ret != MEDIA_CONTENT_ERROR_NONE)
				media_content_error("media_folder_get_name failed: %d", ret);
			ret = media_folder_get_path(folder_handle, &folder_path);
			if(ret != MEDIA_CONTENT_ERROR_NONE)
				media_content_error("media_folder_get_path failed: %d", ret);

			media_content_debug("folder_id [%d] : %s", i, folder_id);
			media_content_debug("folder_name [%d] : %s", i, folder_name);
			media_content_debug("folder_path [%d] : %s", i, folder_path);


			ret = media_folder_get_media_count_from_db(folder_id, filter, &count);
			/* User should free these string */
			SAFE_FREE(folder_id);
			SAFE_FREE(folder_name);
			SAFE_FREE(folder_path);
			if(ret != MEDIA_CONTENT_ERROR_NONE) {
				media_content_error("media_folder_get_media_count_from_db failed: %d", ret);
				return -1;
			} else {
				media_content_debug("media count [%d] : %d", i, count);
			}
		}
	}

	/* To check performance */
	struct timeval start, end;
	gettimeofday(&start, NULL);

	/* Second, Get all item list */
	media_info_h media_handle = NULL;
	GList *all_item_list = NULL;

	media_content_collation_e collate_type = MEDIA_CONTENT_COLLATE_NOCASE;
	media_content_order_e order_type = MEDIA_CONTENT_ORDER_DESC;
	ret = media_filter_create(&filter);
	if(ret != MEDIA_CONTENT_ERROR_NONE) {
		media_content_error("Fail to create filter");
		return ret;
	}
	ret = media_filter_set_condition(filter, "MEDIA_TYPE = 0", collate_type);
	if(ret != MEDIA_CONTENT_ERROR_NONE) {
		media_filter_destroy(filter);
		media_content_error("Fail to set condition");
		return ret;
	}
	ret = media_filter_set_order(filter, order_type, MEDIA_DISPLAY_NAME, collate_type);
	if(ret != MEDIA_CONTENT_ERROR_NONE) {
		media_filter_destroy(filter);
		media_content_error("Fail to set order");
		return ret;
	}

	ret = media_info_foreach_media_from_db(filter, gallery_media_item_cb, &all_item_list);
	if(ret != MEDIA_CONTENT_ERROR_NONE) {
		media_content_error("media_info_foreach_media_from_db failed: %d", ret);
		media_filter_destroy(filter);
		return -1;
	} else {
		media_content_debug("media_info_foreach_media_from_db success");
		media_content_type_e media_type;
		char *media_id = NULL;
		char *media_name = NULL;
		char *media_path = NULL;

		for(i = 0; i < g_list_length(all_item_list); i++) {
			media_handle = (media_info_h)g_list_nth_data(all_item_list, i);

			ret = media_info_get_media_id(media_handle, &media_id);
			if(ret != MEDIA_CONTENT_ERROR_NONE)
				media_content_error("media_info_get_media_id failed: %d", ret);
			ret = media_info_get_media_type(media_handle, &media_type);
			if(ret != MEDIA_CONTENT_ERROR_NONE)
				media_content_error("media_info_get_media_type failed: %d", ret);
			ret = media_info_get_display_name(media_handle, &media_name);
			if(ret != MEDIA_CONTENT_ERROR_NONE)
				media_content_error("media_info_get_display_name failed: %d", ret);
			ret = media_info_get_file_path(media_handle, &media_path);
			if(ret != MEDIA_CONTENT_ERROR_NONE)
				media_content_error("media_info_get_file_path failed: %d", ret);

			if(media_type == MEDIA_CONTENT_TYPE_IMAGE) {
				image_meta_h image_handle;
				int width = 0, height = 0;
				media_content_orientation_e orientation = 0;
				char *datetaken = NULL;
				char *burst_id = NULL;

				ret = media_info_get_image(media_handle, &image_handle);
				if(ret != MEDIA_CONTENT_ERROR_NONE) {
					media_content_error("media_info_get_image failed: %d", ret);
				} else {
					ret = image_meta_get_width(image_handle, &width);
					if(ret != MEDIA_CONTENT_ERROR_NONE)
						media_content_error("error image_meta_get_width : [%d]", ret);
					ret = image_meta_get_height(image_handle, &height);
					if(ret != MEDIA_CONTENT_ERROR_NONE)
						media_content_error("error image_meta_get_height : [%d]", ret);
					ret = image_meta_get_orientation(image_handle, &orientation);
					if(ret != MEDIA_CONTENT_ERROR_NONE)
						media_content_error("error image_meta_get_orientation : [%d]", ret);
					ret = image_meta_get_date_taken(image_handle, &datetaken);
					if(ret != MEDIA_CONTENT_ERROR_NONE)
						media_content_error("error image_meta_get_date_taken : [%d]", ret);
					ret = image_meta_get_burst_id(image_handle, &burst_id);
					if(ret != MEDIA_CONTENT_ERROR_NONE)
						media_content_error("error image_meta_get_burst_id : [%d]", ret);

					media_content_debug("This is Image");
					media_content_debug("Width : %d, Height : %d, Orientation : %d, Date taken : %s", width, height, orientation, datetaken);
				}

				SAFE_FREE(datetaken);
				SAFE_FREE(burst_id);
				ret = image_meta_destroy(image_handle);
				if(ret != MEDIA_CONTENT_ERROR_NONE)
					media_content_error("error image_meta_destroy : [%d]", ret);

			} else if(media_type == MEDIA_CONTENT_TYPE_VIDEO) {
				video_meta_h video_handle;
				char *title = NULL, *artist = NULL, *album = NULL, *album_artist = NULL;
				int duration = 0;
				time_t time_played = 0;

				ret = media_info_get_video(media_handle, &video_handle);
				if(ret != MEDIA_CONTENT_ERROR_NONE) {
					media_content_error("media_info_get_video failed: %d", ret);
				} else {
					ret = video_meta_get_artist(video_handle, &artist);
					if(ret != MEDIA_CONTENT_ERROR_NONE)
						media_content_error("error video_meta_get_artist : [%d]", ret);
					ret = video_meta_get_album(video_handle, &album);
					if(ret != MEDIA_CONTENT_ERROR_NONE)
						media_content_error("error video_meta_get_album : [%d]", ret);
					ret = video_meta_get_album_artist(video_handle, &album_artist);
					if(ret != MEDIA_CONTENT_ERROR_NONE)
						media_content_error("error video_meta_get_album_artist : [%d]", ret);
					ret = video_meta_get_duration(video_handle, &duration);
					if(ret != MEDIA_CONTENT_ERROR_NONE)
						media_content_error("error video_meta_get_duration : [%d]", ret);
					ret = video_meta_get_played_time(video_handle, &time_played);
					if(ret != MEDIA_CONTENT_ERROR_NONE)
						media_content_error("error video_meta_get_played_time : [%d]", ret);

					media_content_debug("This is Video");
					media_content_debug("Title: %s, Album: %s, Artist: %s, Album_artist: %s \n Duration: %d, Played time: %d", title, album, artist, album_artist, duration, time_played);
				}

				SAFE_FREE(title);
				SAFE_FREE(artist);
				SAFE_FREE(album);
				SAFE_FREE(album_artist);

				ret = video_meta_destroy(video_handle);
				if(ret != MEDIA_CONTENT_ERROR_NONE)
					media_content_error("error video_meta_destroy : [%d]", ret);
			}
			//media_content_debug("media_id [%d] : %s", i, media_id);
			//media_content_debug("media_name [%d] : %s", i, media_name);
			//media_content_debug("media_path [%d] : %s", i, media_path);

			SAFE_FREE(media_id);
			SAFE_FREE(media_name);
			SAFE_FREE(media_path);
		}
	}

	media_filter_destroy(filter);
	filter = NULL;

	/* To check performance */
	gettimeofday(&end, NULL);
	long time = (end.tv_sec * 1000000 + end.tv_usec) - (start.tv_sec * 1000000 + start.tv_usec);
	media_content_debug("Time : %ld\n", time);

	/* Third, Get item list of a folder */
	GList *item_list = NULL;

	for(i = 0; i < g_list_length(folder_list); i++) {
		int j = 0;
		char *folder_id = NULL;
		char *folder_name = NULL;
		folder_handle = (media_folder_h)g_list_nth_data(folder_list, i);

		media_folder_get_folder_id(folder_handle, &folder_id);
		media_folder_get_name(folder_handle, &folder_name);

		ret = media_folder_foreach_media_from_db(folder_id, filter, gallery_media_item_cb, &item_list);
		SAFE_FREE(folder_id);
		SAFE_FREE(folder_name);

		if(ret != MEDIA_CONTENT_ERROR_NONE) {
			media_content_error("media_folder_foreach_media_from_db failed: %d", ret);
			return -1;
		} else {
			media_content_error("media_folder_foreach_media_from_db success!", ret);
			char *media_id = NULL;
			char *media_name = NULL;
			char *media_path = NULL;
			media_content_type_e media_type = 0;

			for(j = 0; j < g_list_length(item_list); j++) {
				media_handle = (media_info_h)g_list_nth_data(item_list, j);

				ret = media_info_get_media_id(media_handle, &media_id);
				if(ret != MEDIA_CONTENT_ERROR_NONE)
					media_content_error("media_info_get_media_id failed: %d", ret);
				ret = media_info_get_display_name(media_handle, &media_name);
				if(ret != MEDIA_CONTENT_ERROR_NONE)
					media_content_error("media_info_get_display_name failed: %d", ret);
				ret = media_info_get_file_path(media_handle, &media_path);
				if(ret != MEDIA_CONTENT_ERROR_NONE)
					media_content_error("media_info_get_file_path failed: %d", ret);
				ret = media_info_get_media_type(media_handle, &media_type);
				if(ret != MEDIA_CONTENT_ERROR_NONE)
					media_content_error("media_info_get_media_type failed: %d", ret);

				media_content_debug("[%s] media_id [%d] : %s", folder_name, j, media_id);
				media_content_debug("[%s] media_type [%d] : %d", folder_name, j, media_type);
				media_content_debug("[%s] media_name [%d] : %s", folder_name, j, media_name);
				media_content_debug("[%s] media_path [%d] : %s", folder_name, j, media_path);

				if(media_type == MEDIA_CONTENT_TYPE_IMAGE) {
					image_meta_h image_handle;
					int width = 0, height = 0;
					media_content_orientation_e orientation = 0;
					char *datetaken = NULL;

					ret = media_info_get_image(media_handle, &image_handle);
					if(ret != MEDIA_CONTENT_ERROR_NONE) {
						media_content_error("media_info_get_image failed: %d", ret);
					} else {
						ret = image_meta_get_width(image_handle, &width);
						if(ret != MEDIA_CONTENT_ERROR_NONE)
							media_content_error("error image_meta_get_width : [%d]", ret);
						ret = image_meta_get_height(image_handle, &height);
						if(ret != MEDIA_CONTENT_ERROR_NONE)
							media_content_error("error image_meta_get_height : [%d]", ret);
						ret = image_meta_get_orientation(image_handle, &orientation);
						if(ret != MEDIA_CONTENT_ERROR_NONE)
							media_content_error("error image_meta_get_orientation : [%d]", ret);
						ret = image_meta_get_date_taken(image_handle, &datetaken);
						if(ret != MEDIA_CONTENT_ERROR_NONE)
							media_content_error("error image_meta_get_date_taken : [%d]", ret);

						media_content_debug("This is Image");
						media_content_debug("Width : %d, Height : %d, Orientation : %d, Date taken : %s", width, height, orientation, datetaken);
					}

					SAFE_FREE(datetaken);
					ret = image_meta_destroy(image_handle);
					if(ret != MEDIA_CONTENT_ERROR_NONE)
						media_content_error("error image_meta_destroy : [%d]", ret);

				} else if(media_type == MEDIA_CONTENT_TYPE_VIDEO) {
					video_meta_h video_handle;
					char *title = NULL, *artist = NULL, *album = NULL, *album_artist = NULL;;
					int duration = 0;
					time_t time_played;

					ret = media_info_get_video(media_handle, &video_handle);
					if(ret != MEDIA_CONTENT_ERROR_NONE) {
						media_content_error("media_info_get_video failed: %d", ret);
					} else {
						ret = video_meta_get_artist(video_handle, &artist);
						if(ret != MEDIA_CONTENT_ERROR_NONE)
							media_content_error("error video_meta_get_artist : [%d]", ret);
						ret = video_meta_get_album(video_handle, &album);
						if(ret != MEDIA_CONTENT_ERROR_NONE)
							media_content_error("error video_meta_get_album : [%d]", ret);
						ret = video_meta_get_album_artist(video_handle, &album_artist);
						if(ret != MEDIA_CONTENT_ERROR_NONE)
							media_content_error("error video_meta_get_album_artist : [%d]", ret);
						ret = video_meta_get_duration(video_handle, &duration);
						if(ret != MEDIA_CONTENT_ERROR_NONE)
							media_content_error("error video_meta_get_duration : [%d]", ret);
						ret = video_meta_get_played_time(video_handle, &time_played);
						if(ret != MEDIA_CONTENT_ERROR_NONE)
							media_content_error("error video_meta_get_played_time : [%d]", ret);

						media_content_debug("This is Video");
						media_content_debug("Title: %s, Album: %s, Artist: %s, Album_artist: %s \n Duration: %d, Played time: %d\n", title, album, artist, album_artist, duration, time_played);
					}

					SAFE_FREE(title);
					SAFE_FREE(artist);
					SAFE_FREE(album);
					SAFE_FREE(album_artist);

					ret = video_meta_destroy(video_handle);
					if(ret != MEDIA_CONTENT_ERROR_NONE)
						media_content_error("error video_meta_destroy : [%d]", ret);

					/* Get bookmar list in case of video */
					media_bookmark_h bm_handle;
					GList *bm_list = NULL;

					ret = media_info_foreach_bookmark_from_db(media_id, filter, gallery_bookmarks_cb, &bm_list);
					if(ret != MEDIA_CONTENT_ERROR_NONE) {
						media_content_error("media_info_foreach_bookmark_from_db failed: %d", ret);
					} else {
						media_content_debug("media_info_foreach_bookmark_from_db success");

						int k = 0;

						for(k = 0; k < g_list_length(bm_list); k++) {
							bm_handle = (media_bookmark_h)g_list_nth_data(bm_list, k);
							time_t marked_time;
							char *thumb_path = NULL;

							ret = media_bookmark_get_marked_time(bm_handle, &marked_time);
							if(ret != MEDIA_CONTENT_ERROR_NONE)
								media_content_error("error media_bookmark_get_marked_time : [%d]", ret);
							ret = media_bookmark_get_thumbnail_path(bm_handle, &thumb_path);
							if(ret != MEDIA_CONTENT_ERROR_NONE)
								media_content_error("error media_bookmark_get_thumbnail_path : [%d]", ret);

							media_content_debug("Bookmark time : %d\nBookmar thumb: %s", marked_time, thumb_path);
							SAFE_FREE(thumb_path);
						}

						/* Remove bookmark list */
						int l = 0;
						if(bm_list) {
							for(l = 0; l < g_list_length(bm_list); l++) {
								bm_handle = (media_bookmark_h)g_list_nth_data(bm_list, l);
								ret = media_bookmark_destroy(bm_handle);
								if(ret != MEDIA_CONTENT_ERROR_NONE)
									media_content_error("error media_bookmark_destroy : [%d]", ret);
							}

							g_list_free(bm_list);
						}

					}
				}

				SAFE_FREE(media_id);
				SAFE_FREE(media_name);
				SAFE_FREE(media_path);
			}
		}
	}

	/* Get tag list */
	media_tag_h tag_handle = NULL;
	GList *tag_list = NULL;
	GList *media_list_in_tag = NULL;

	ret = media_tag_foreach_tag_from_db (filter, gallery_tag_item_cb, &tag_list);

	if(ret != MEDIA_CONTENT_ERROR_NONE) {
		media_content_error("media_tag_foreach_tag_from_db failed: %d", ret);
		return -1;
	} else {
		media_content_error("media_tag_foreach_tag_from_db success");
		char *tag_name = NULL;
		int tag_id;

		for(i = 0; i < g_list_length(tag_list); i++) {
			tag_handle = (media_tag_h)g_list_nth_data(tag_list, i);
			media_tag_get_tag_id(tag_handle, &tag_id);
			media_tag_get_name(tag_handle, &tag_name);

			media_content_debug("[%d] %s", tag_id, tag_name);

			ret = media_tag_foreach_media_from_db(tag_id, filter, gallery_media_item_cb, &media_list_in_tag);
			if(ret != MEDIA_CONTENT_ERROR_NONE) {
				media_content_error("media_tag_foreach_media_from_db failed: %d", ret);
				return -1;
			} else {
				media_content_error("media_tag_foreach_media_from_db success");
				int j = 0;
				media_info_h tag_media_handle;
				char *media_id = NULL;
				char *media_name = NULL;
				char *media_path = NULL;
				media_content_type_e media_type = 0;

				for(j = 0; j < g_list_length(media_list_in_tag); j++) {
					tag_media_handle = (media_info_h)g_list_nth_data(media_list_in_tag, j);
					ret = media_info_get_media_id(tag_media_handle, &media_id);
					if(ret != MEDIA_CONTENT_ERROR_NONE)
						media_content_error("media_info_get_media_id failed: %d", ret);
					ret = media_info_get_display_name(tag_media_handle, &media_name);
					if(ret != MEDIA_CONTENT_ERROR_NONE)
						media_content_error("media_info_get_display_name failed: %d", ret);
					ret = media_info_get_file_path(tag_media_handle, &media_path);
					if(ret != MEDIA_CONTENT_ERROR_NONE)
						media_content_error("media_info_get_file_path failed: %d", ret);
					ret = media_info_get_media_type(tag_media_handle, &media_type);
					if(ret != MEDIA_CONTENT_ERROR_NONE)
						media_content_error("media_info_get_media_type failed: %d", ret);

					media_content_debug("[%s] media_id [%d] : %s", tag_name, j, media_id);
					media_content_debug("[%s] media_type [%d] : %d", tag_name, j, media_type);
					media_content_debug("[%s] media_name [%d] : %s", tag_name, j, media_name);
					media_content_debug("[%s] media_path [%d] : %s", tag_name, j, media_path);
				}
				SAFE_FREE(media_id);
				SAFE_FREE(media_name);
				SAFE_FREE(media_path);
			}
			SAFE_FREE(tag_name);
		}
	}

	/* Remove folder list */
	if(folder_list) {
		for(i = 0; i < g_list_length(folder_list); i++) {
			folder_handle = (media_folder_h)g_list_nth_data(folder_list, i);
			media_folder_destroy(folder_handle);
		}

		g_list_free(folder_list);
	}

	/* Remove all items list */
	if(all_item_list) {
		for(i = 0; i < g_list_length(all_item_list); i++) {
			media_handle = (media_info_h)g_list_nth_data(all_item_list, i);
			ret = media_info_destroy(media_handle);
			if(ret != MEDIA_CONTENT_ERROR_NONE)
				media_content_error("media_info_destroy failed: %d", ret);
		}

		g_list_free(all_item_list);
	}

	/* Remove items list */
	if(item_list) {
		for(i = 0; i < g_list_length(item_list); i++) {
			media_handle = (media_info_h)g_list_nth_data(item_list, i);
			ret = media_info_destroy(media_handle);
			if(ret != MEDIA_CONTENT_ERROR_NONE)
				media_content_error("media_info_destroy failed: %d", ret);
		}

		g_list_free(item_list);
	}

	/* Remove tag list */
	if(tag_list) {
		for(i = 0; i < g_list_length(tag_list); i++) {
			tag_handle = (media_tag_h)g_list_nth_data(tag_list, i);
			ret = media_tag_destroy(tag_handle);
			if(ret != MEDIA_CONTENT_ERROR_NONE)
				media_content_error("error media_tag_destroy : [%d]", ret);
		}

		g_list_free(tag_list);
	}

	/* Remove media list in a tag */
	if(media_list_in_tag) {
		for(i = 0; i < g_list_length(media_list_in_tag); i++) {
			media_handle = (media_info_h)g_list_nth_data(media_list_in_tag, i);
			ret = media_info_destroy(media_handle);
			if(ret != MEDIA_CONTENT_ERROR_NONE)
				media_content_error("media_info_destroy failed: %d", ret);
		}

		g_list_free(media_list_in_tag);
	}

	return MEDIA_CONTENT_ERROR_NONE;
}

/*Get All Music file. sort by Title and not case sensitive*/
int test_get_all_music_files(void)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	int media_count = 0;
	filter_h filter;

	/*Set Filter*/
	char *condition = "MEDIA_TYPE=3";	/*0-image, 1-video, 2-sound, 3-music, 4-other*/

	ret = media_filter_create(&filter);
	if(ret != MEDIA_CONTENT_ERROR_NONE) {
		media_content_error("Fail to create filter");
		return ret;
	}
	ret = media_filter_set_condition(filter, condition, MEDIA_CONTENT_COLLATE_LOCALIZED);
	if(ret != MEDIA_CONTENT_ERROR_NONE) {
		media_filter_destroy(filter);
		media_content_error("Fail to set condition");
		return ret;
	}
	ret = media_filter_set_order(filter, MEDIA_CONTENT_ORDER_ASC, MEDIA_TITLE, MEDIA_CONTENT_COLLATE_LOCALIZED);
	if(ret != MEDIA_CONTENT_ERROR_NONE) {
		media_filter_destroy(filter);
		media_content_error("Fail to set order");
		return ret;
	}

	/*Get Media Count*/
	ret = media_info_get_media_count_from_db(filter, &media_count);
	if(ret != MEDIA_CONTENT_ERROR_NONE) {
		media_filter_destroy(filter);
		media_content_error("Fail to get media count");
		return ret;
	}

	media_content_debug("media_count : [%d]", media_count);

	ret = media_info_foreach_media_from_db(filter, media_item_cb, NULL);
	if(ret != MEDIA_CONTENT_ERROR_NONE) {
		media_filter_destroy(filter);
		media_content_error("Fail to get media");
		return ret;
	}

	ret = media_filter_destroy(filter);

	return ret;
}

int test_media_info_operation(void)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	int media_count = 0;

	media_content_debug("\n============Media info Test============\n\n");

	test_filter_create();

	ret = media_info_get_media_count_from_db(g_filter, &media_count);
	if(ret != MEDIA_CONTENT_ERROR_NONE)
		media_content_error("media_info_get_media_count_from_db failed: %d", ret);
	else
		media_content_debug("media_count : [%d]", media_count);

	ret = media_info_foreach_media_from_db(g_filter, media_item_cb, NULL);
	if(ret == MEDIA_CONTENT_ERROR_NONE)
		media_content_debug("media_info_foreach_media_from_db is success");
	else
		media_content_error("media_info_foreach_media_from_db is failed");

	test_filter_destroy();

	return ret;
}

int test_media_info_operation_2(void)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	int bookmark_count = 0;

	media_content_debug("\n============Media info Test 2============\n\n");

	test_filter_create();

#if 0
	/* Bookmark */
	char *thumbnail_path1 = "/opt/media/Images and videos/My video clips/teat11.jpg";
	media_bookmark_insert_to_db(test_video_id, 100, thumbnail_path1);
	media_bookmark_insert_to_db(test_video_id, 200, thumbnail_path1);

	media_info_get_bookmark_count_from_db(test_video_id, g_filter_g, &bookmark_count);

	media_content_debug("bookmark_count : [%d]", bookmark_count);

	ret = media_info_foreach_bookmark_from_db(test_video_id, g_filter_g, bookmarks_cb, NULL);
#endif

	/* Tag */
	ret = media_info_get_tag_count_from_db(test_audio_id, g_filter_g, &bookmark_count);
	if(ret != MEDIA_CONTENT_ERROR_NONE)
		media_content_error("media_info_get_tag_count_from_db failed: %d", ret);
	else
		media_content_debug("tag_count : [%d]", bookmark_count);

	ret = media_info_foreach_tag_from_db(test_audio_id, g_filter_g, tag_list_cb, NULL);
	if(ret != MEDIA_CONTENT_ERROR_NONE)
		media_content_error("media_info_foreach_tag_from_db failed: %d", ret);

	test_filter_destroy();

	return ret;
}

int test_folder_operation(void)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	filter_h filter;
	media_folder_h folder = NULL;
	char *folder_id = NULL;
	int folder_count = 0;

	media_content_debug("\n============Folder Test============\n\n");

	test_filter_create();

	ret = media_filter_create(&filter);
	if(ret != MEDIA_CONTENT_ERROR_NONE)
	{
		media_content_error("[ERROR] media_folder_filter_create is failed");
		return ret;
	}

	media_filter_set_condition(filter, "(MEDIA_TYPE = 0 or MEDIA_TYPE = 1) and MEDIA_STORAGE_TYPE=0", MEDIA_CONTENT_COLLATE_DEFAULT);	/*MEDIA_TYPE 0-image, 1-video, 2-sound, 3-music, 4-other*/
	media_filter_set_offset(filter, 0,5);
	media_filter_set_order(filter,MEDIA_CONTENT_ORDER_DESC, MEDIA_PATH, MEDIA_CONTENT_COLLATE_NOCASE);

	ret = media_folder_get_folder_count_from_db(filter, &folder_count);
	media_content_debug("Folder count : %d", folder_count);

	ret = media_folder_foreach_folder_from_db(filter, folder_list_cb, &folder);

	//test.3 get the media list in first folder
	filter_h m_filter = NULL;
	ret = media_filter_create(&m_filter);
	if(ret != MEDIA_CONTENT_ERROR_NONE)
	{
		test_filter_destroy();
		media_filter_destroy(filter);
		media_content_error("[ERROR] media_info_filter_create is failed");
		return ret;
	}

	media_filter_set_condition(m_filter, "MEDIA_TYPE=1 and MEDIA_STORAGE_TYPE=0", MEDIA_CONTENT_COLLATE_DEFAULT);	/*MEDIA_TYPE 0-image, 1-video, 2-sound, 3-music, 4-other*/
	media_filter_set_offset(m_filter, 0,5);
	media_filter_set_order(m_filter,MEDIA_CONTENT_ORDER_DESC, MEDIA_PATH, MEDIA_CONTENT_COLLATE_NOCASE);

	media_folder_get_folder_id(folder, &folder_id);
	media_content_debug("folder_id : %s", folder_id);

	ret = media_folder_foreach_media_from_db(folder_id, m_filter, media_item_cb, NULL);

	if(ret != MEDIA_CONTENT_ERROR_NONE)
	{
		media_content_error("[ERROR] media_folder_foreach_media_from_db is failed, error code : %d", ret);
	}

	media_filter_destroy(filter);
	media_filter_destroy(m_filter);

	test_filter_destroy();

	/* fix prevent: Resource Leak */
	SAFE_FREE(folder_id);

	return ret;
}

bool folder_update_cb(media_folder_h folder, void *user_data)
{
	char *folder_id = NULL;
	char *folder_path = NULL;
	char *folder_name = NULL;
	bool ret = true;

	if(folder != NULL)
	{
		ret = media_folder_get_folder_id(folder, &folder_id);
		media_content_retv_if(ret != MEDIA_CONTENT_ERROR_NONE, ret);
		media_content_debug("folder_id = [%s]", folder_id);
		SAFE_FREE(folder_id);

		ret = media_folder_get_path(folder, &folder_path);
		media_content_retv_if(ret != MEDIA_CONTENT_ERROR_NONE, ret);
		media_content_debug("folder_path = [%s]", folder_path);
		SAFE_FREE(folder_path);

		ret = media_folder_get_name(folder, &folder_name);
		media_content_retv_if(ret != MEDIA_CONTENT_ERROR_NONE, ret);
		media_content_debug("folder_name = [%s]", folder_name);
		SAFE_FREE(folder_name);

		ret = media_folder_clone(&g_folder, folder);
		media_content_retv_if(ret != MEDIA_CONTENT_ERROR_NONE, ret);

		ret = true;
	}
	else
	{
		ret = false;
	}

	return ret;
}


int test_folder_update(void)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	filter_h filter;
	int folder_count = 0;

	media_content_debug("\n============Folder Update Test============\n\n");

	ret = media_filter_create(&filter);
	media_content_retv_if(ret != MEDIA_CONTENT_ERROR_NONE, ret);

	media_filter_set_condition(filter, "MEDIA_TYPE=0 and MEDIA_STORAGE_TYPE=0", MEDIA_CONTENT_COLLATE_DEFAULT);	/*MEDIA_TYPE 0-image, 1-video, 2-sound, 3-music, 4-other*/

	ret = media_folder_get_folder_count_from_db(filter, &folder_count);

	media_content_debug("Folder count : %d", folder_count);

	ret = media_folder_foreach_folder_from_db(filter, folder_update_cb, NULL);

	media_filter_destroy(filter);

	ret = media_folder_set_name(g_folder, "test_folder");

	ret = media_folder_update_to_db(g_folder);

	ret = media_folder_destroy(g_folder);

	return ret;
}

int test_playlist_operation(void)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	media_playlist_h playlist_1 = NULL;
	media_playlist_h playlist_2 = NULL;
	media_playlist_h playlist_3 = NULL;
	int playlist_id_1 = 0;
	int playlist_id_2 = 0;
	int playlist_id_3 = 0;
	char *playlist_name_1 = "myPlaylist_1";
	char *playlist_name_2 = "myPlaylist_2";
	char *playlist_name_3 = "myPlaylist_3";
	int playlist_count = 0;
	int media_count = 0;
	int order_1 = 0;
	int order_2 = 0;
	int order_3 = 0;
	int order_4 = 0;
	int order_5 = 0;
	filter_h filter = NULL;
	filter_h m_filter = NULL;

	media_content_debug("\n============Playlist Test============\n\n");

	/* Filter for playlist */

	char *condition = "(MEDIA_TYPE=1 or MEDIA_TYPE=3)";	/*0-image, 1-video, 2-sound, 3-music, 4-other*/

	ret = media_filter_create(&filter);
	ret = media_filter_set_condition(filter, condition, MEDIA_CONTENT_COLLATE_NOCASE);
	ret = media_filter_set_order(filter, MEDIA_CONTENT_ORDER_ASC, PLAYLIST_NAME, MEDIA_CONTENT_COLLATE_NOCASE);

	/* Create Playlist */
	media_playlist_insert_to_db(playlist_name_1, &playlist_1);
	media_playlist_insert_to_db(playlist_name_2, &playlist_2);
	media_playlist_insert_to_db(playlist_name_3, &playlist_3);

	if(playlist_1 != NULL)
	{
		/* Add media to Playlist */
		media_playlist_add_media(playlist_1, test_audio_id);
		media_playlist_add_media(playlist_1, test_audio_id);
		media_playlist_add_media(playlist_1, test_video_id);

		#if 0
		char *playlist_thumb_path = "/opt/usr/media/Images/Default.jpg";
		media_playlist_set_thumbnail_path(playlist_1, playlist_thumb_path);
		#endif

		media_playlist_update_to_db(playlist_1);
	}

	if(playlist_2 != NULL)
	{
		media_playlist_add_media(playlist_2, test_audio_id);
		media_playlist_add_media(playlist_2, test_audio_id);
		media_playlist_update_to_db(playlist_2);
	}

	/* Get Playlist Count*/
	ret = media_playlist_get_playlist_count_from_db(filter, &playlist_count);
	if (ret == 0)
		media_content_debug("playlist_count [%d]", playlist_count);

	/* Get Playlist*/
	GList *playlist_id_list = NULL;
	media_playlist_foreach_playlist_from_db(filter, playlist_list_cb, &playlist_id_list);

	/* Get Playlist id*/
	playlist_id_1 = (int)g_list_nth_data(playlist_id_list, 0);
	playlist_id_2 = (int)g_list_nth_data(playlist_id_list, 1);
	playlist_id_3 = (int)g_list_nth_data(playlist_id_list, 2);
	media_content_debug("playlist_id_1 [%d]", playlist_id_1);
	media_content_debug("playlist_id_2 [%d]", playlist_id_2);
	media_content_debug("playlist_id_3 [%d]", playlist_id_3);

	/* Filter for media*/
	ret = media_filter_create(&m_filter);

	ret = media_filter_set_condition(m_filter, condition, MEDIA_CONTENT_COLLATE_DEFAULT);

	ret = media_filter_set_order(m_filter, MEDIA_CONTENT_ORDER_ASC, PLAYLIST_MEMBER_ORDER, MEDIA_CONTENT_COLLATE_DEFAULT);

	/* Get media count */
	media_playlist_get_media_count_from_db(playlist_id_1, m_filter, &media_count);
	media_content_debug("playlist_1_media_count [%d]", media_count);

	media_playlist_get_media_count_from_db(playlist_id_2, m_filter, &media_count);
	media_content_debug("playlist_2_media_count [%d]", media_count);

	media_playlist_get_media_count_from_db(playlist_id_3, m_filter, &media_count);
	media_content_debug("playlist_3_media_count [%d]", media_count);

	/* Get media of playlist */
	GList *playlist_member_id_list = NULL;
	GList *playlist_member_id_list_1 = NULL;

	media_playlist_foreach_media_from_db(playlist_id_1, m_filter, playlist_item_cb, &playlist_member_id_list);
	media_playlist_foreach_media_from_db(playlist_id_2, m_filter, playlist_item_cb, &playlist_member_id_list_1);

	int playlist_member_id_1_1 =0;
	int playlist_member_id_1_2 =0;
	int playlist_member_id_1_3 =0;
	int playlist_member_id_2_1 =0;
	int playlist_member_id_2_2 =0;

	playlist_member_id_1_1 = (int)g_list_nth_data(playlist_member_id_list, 0);
	playlist_member_id_1_2 = (int)g_list_nth_data(playlist_member_id_list, 1);
	playlist_member_id_1_3 = (int)g_list_nth_data(playlist_member_id_list, 2);
	playlist_member_id_2_1 = (int)g_list_nth_data(playlist_member_id_list_1, 0);
	playlist_member_id_2_2 = (int)g_list_nth_data(playlist_member_id_list_1, 1);

	media_content_debug("playlist_member_id_1_1 [%d]", playlist_member_id_1_1);
	media_content_debug("playlist_member_id_1_2 [%d]", playlist_member_id_1_2);
	media_content_debug("playlist_member_id_1_3 [%d]", playlist_member_id_1_3);
	media_content_debug("playlist_member_id_2_1 [%d]", playlist_member_id_2_1);
	media_content_debug("playlist_member_id_2_2 [%d]", playlist_member_id_2_2);

	media_playlist_get_play_order(playlist_1, playlist_member_id_1_1, &order_1);
	media_playlist_get_play_order(playlist_1, playlist_member_id_1_2, &order_2);
	media_playlist_get_play_order(playlist_1, playlist_member_id_1_3, &order_3);
	media_playlist_get_play_order(playlist_2, playlist_member_id_2_1, &order_4);
	media_playlist_get_play_order(playlist_2, playlist_member_id_2_2, &order_5);
	media_content_debug("order_1 [%d] order_2 [%d] order_3 [%d] order_4 [%d] order_5 [%d]", order_1, order_2, order_3, order_4, order_5);

	/* Update Playlist */
	media_playlist_remove_media(playlist_2, playlist_member_id_2_1);
	media_playlist_add_media(playlist_2, test_video_id);
	media_playlist_set_name(playlist_2, "test_playlist");
	media_playlist_set_play_order(playlist_2, playlist_member_id_2_2, order_5+100);
	media_playlist_update_to_db(playlist_2);

	/* Get Updated Playlist*/
	media_playlist_foreach_playlist_from_db(filter, playlist_list_cb, NULL);

	/* deletes the playlist */
	//media_playlist_delete_from_db(playlist_id_1);
	//media_playlist_delete_from_db(playlist_id_2);

	if(playlist_1 != NULL)
		media_playlist_destroy(playlist_1);
	if(playlist_2 != NULL)
		media_playlist_destroy(playlist_2);
	if(playlist_3 != NULL)
		media_playlist_destroy(playlist_3);

	g_list_free(playlist_id_list);
	g_list_free(playlist_member_id_list);
	g_list_free(playlist_member_id_list_1);

	if(filter != NULL)
		ret = media_filter_destroy(filter);
	if(m_filter != NULL)
		ret = media_filter_destroy(m_filter);

	return ret;
}

int test_tag_operation(void)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	media_tag_h tag_1;
	media_tag_h tag_2;
	media_tag_h tag_3;
	int tag_id_1 = 0;
	int tag_id_2 = 0;
	int tag_id_3 = 0;
	char *tag_name_1 = "myTag_1";
	char *tag_name_2 = "myTag_2";
	char *tag_name_3 = "myTag_3";
	int tag_count = 0;
	int media_count = 0;
	filter_h filter;

	media_content_debug("\n============Tag Test============\n\n");

	char *g_condition = "TAG_NAME like \"%%my%%\"";

	ret = media_filter_create(&filter);

	ret = media_filter_set_condition(filter, g_condition, MEDIA_CONTENT_COLLATE_DEFAULT);

	ret = media_filter_set_order(filter, MEDIA_CONTENT_ORDER_DESC, TAG_NAME, MEDIA_CONTENT_COLLATE_DEFAULT);

	/* Create Tag */
	ret = media_tag_insert_to_db(tag_name_1, &tag_1);
	if(ret != MEDIA_CONTENT_ERROR_NONE)
		media_content_error("error media_tag_insert_to_db : [%d]", ret);
	ret = media_tag_insert_to_db(tag_name_2, &tag_2);
	if(ret != MEDIA_CONTENT_ERROR_NONE)
		media_content_error("error media_tag_insert_to_db : [%d]", ret);
	ret = media_tag_insert_to_db(tag_name_3, &tag_3);
	if(ret != MEDIA_CONTENT_ERROR_NONE)
		media_content_error("error media_tag_insert_to_db : [%d]", ret);

	/* Add media to Tag */
	ret = media_tag_add_media(tag_1, test_audio_id);
	if(ret != MEDIA_CONTENT_ERROR_NONE)
		media_content_error("error media_tag_add_media : [%d]", ret);
	ret = media_tag_add_media(tag_1, test_video_id);
	if(ret != MEDIA_CONTENT_ERROR_NONE)
		media_content_error("error media_tag_add_media : [%d]", ret);
	ret = media_tag_update_to_db(tag_1);
	if(ret != MEDIA_CONTENT_ERROR_NONE)
		media_content_error("error media_tag_update_to_db : [%d]", ret);

	ret = media_tag_add_media(tag_2, test_audio_id);
	if(ret != MEDIA_CONTENT_ERROR_NONE)
		media_content_error("error media_tag_add_media : [%d]", ret);
	ret = media_tag_update_to_db(tag_2);
	if(ret != MEDIA_CONTENT_ERROR_NONE)
		media_content_error("error media_tag_update_to_db : [%d]", ret);

	/* Get Tag Count*/
	ret = media_tag_get_tag_count_from_db(filter, &tag_count);
	if(ret != MEDIA_CONTENT_ERROR_NONE)
		media_content_error("error media_tag_get_tag_count_from_db : [%d]", ret);
	else
		media_content_debug("tag_count [%d]", tag_count);

	/* Get Tag*/
	ret = media_tag_foreach_tag_from_db(filter, tag_list_cb, NULL);
	if(ret != MEDIA_CONTENT_ERROR_NONE)
		media_content_error("error media_tag_foreach_tag_from_db : [%d]", ret);

	/* Get Tag id*/
	ret = media_tag_get_tag_id(tag_1, &tag_id_1);
	if(ret != MEDIA_CONTENT_ERROR_NONE)
		media_content_error("error media_tag_get_tag_id : [%d]", ret);
	else
		media_content_debug("tag_id_1 [%d]", tag_id_1);

	ret = media_tag_get_tag_id(tag_2, &tag_id_2);
	if(ret != MEDIA_CONTENT_ERROR_NONE)
		media_content_error("error media_tag_get_tag_id : [%d]", ret);
	else
		media_content_debug("tag_id_2 [%d]", tag_id_2);

	ret = media_tag_get_tag_id(tag_3, &tag_id_3);
	if(ret != MEDIA_CONTENT_ERROR_NONE)
		media_content_error("error media_tag_get_tag_id : [%d]", ret);
	else
		media_content_debug("tag_id_3 [%d]", tag_id_3);

	/* Get media count */
	ret = media_tag_get_media_count_from_db(tag_id_1, NULL, &media_count);
	if(ret != MEDIA_CONTENT_ERROR_NONE)
		media_content_error("error media_tag_get_media_count_from_db : [%d]", ret);
	else
		media_content_debug("tag_1_media_count [%d]", media_count);

	ret = media_tag_get_media_count_from_db(tag_id_2, NULL, &media_count);
	if(ret != MEDIA_CONTENT_ERROR_NONE)
		media_content_error("error media_tag_get_media_count_from_db : [%d]", ret);
	else
		media_content_debug("tag_2_media_count [%d]", media_count);

	/* Get media of Tag */
	ret = media_tag_foreach_media_from_db(tag_id_1, NULL, media_item_cb, NULL);
	if(ret != MEDIA_CONTENT_ERROR_NONE)
		media_content_error("error media_tag_foreach_media_from_db : [%d]", ret);

	ret = media_tag_foreach_media_from_db(tag_id_2, NULL, media_item_cb, NULL);
	if(ret != MEDIA_CONTENT_ERROR_NONE)
		media_content_error("error media_tag_foreach_media_from_db : [%d]", ret);

	/* Update Tag */
	ret = media_tag_add_media(tag_2, test_video_id);
	if(ret != MEDIA_CONTENT_ERROR_NONE)
		media_content_error("error media_tag_add_media : [%d]", ret);
	ret = media_tag_set_name(tag_2, "test_tag");
	if(ret != MEDIA_CONTENT_ERROR_NONE)
		media_content_error("error media_tag_set_name : [%d]", ret);
	ret = media_tag_update_to_db(tag_2);
	if(ret != MEDIA_CONTENT_ERROR_NONE)
		media_content_error("error media_tag_update_to_db : [%d]", ret);

	/* Get Updated Tag*/
	ret = media_tag_foreach_tag_from_db(filter, tag_list_cb, NULL);
	if(ret != MEDIA_CONTENT_ERROR_NONE)
		media_content_error("error media_tag_foreach_tag_from_db : [%d]", ret);

	/* deletes the tag */
	ret = media_tag_delete_from_db(tag_id_1);
	if(ret != MEDIA_CONTENT_ERROR_NONE)
		media_content_error("error media_tag_delete_from_db : [%d]", ret);
	ret = media_tag_delete_from_db(tag_id_2);
	if(ret != MEDIA_CONTENT_ERROR_NONE)
		media_content_error("error media_tag_delete_from_db : [%d]", ret);

	ret = media_tag_destroy(tag_1);
	if(ret != MEDIA_CONTENT_ERROR_NONE)
		media_content_error("error media_tag_destroy : [%d]", ret);
	ret = media_tag_destroy(tag_2);
	if(ret != MEDIA_CONTENT_ERROR_NONE)
		media_content_error("error media_tag_destroy : [%d]", ret);
	ret = media_filter_destroy(filter);
	if(ret != MEDIA_CONTENT_ERROR_NONE)
		media_content_error("error media_filter_destroy : [%d]", ret);

	return ret;
}


int test_bookmark_operation(void)
{
	//bookmark is only supported for video information.
	int ret = MEDIA_CONTENT_ERROR_NONE;
	int bookmark_count = 0;
	filter_h filter;

	media_content_debug("\n============Bookmark Test============\n\n");

	char *g_condition = "BOOKMARK_MARKED_TIME > 300";

	ret = media_filter_create(&filter);
	if(ret != MEDIA_CONTENT_ERROR_NONE)
		media_content_error("error media_filter_create : [%d]", ret);

	ret = media_filter_set_condition(filter, g_condition, MEDIA_CONTENT_COLLATE_DEFAULT);
	if(ret != MEDIA_CONTENT_ERROR_NONE)
		media_content_error("error media_filter_set_condition : [%d]", ret);

	ret = media_filter_set_order(filter, MEDIA_CONTENT_ORDER_DESC, BOOKMARK_MARKED_TIME, MEDIA_CONTENT_COLLATE_DEFAULT);
	if(ret != MEDIA_CONTENT_ERROR_NONE)
		media_content_error("error media_filter_set_order : [%d]", ret);

	//insert bookmark to video
	char *thumbnail_path1 = "/opt/usr/media/Images and videos/My video clips/teat11.jpg";
	ret = media_bookmark_insert_to_db(test_video_id, 400, thumbnail_path1);
	if(ret != MEDIA_CONTENT_ERROR_NONE)
		media_content_error("error media_bookmark_insert_to_db : [%d]", ret);

	ret = media_bookmark_insert_to_db(test_video_id, 600, thumbnail_path1);
	if(ret != MEDIA_CONTENT_ERROR_NONE)
		media_content_error("error media_bookmark_insert_to_db : [%d]", ret);

	ret = media_bookmark_get_bookmark_count_from_db(filter, &bookmark_count);
	if(ret != MEDIA_CONTENT_ERROR_NONE)
		media_content_error("error media_bookmark_get_bookmark_count_from_db : [%d]", ret);
	else
		media_content_debug("bookmark_count = [%d]", bookmark_count);

	ret = media_filter_destroy(filter);
	if(ret != MEDIA_CONTENT_ERROR_NONE)
		media_content_error("error media_filter_destroy : [%d]", ret);

	return ret;
}

int test_album_list(void)
{
	media_content_debug("\n============Album Test============\n\n");

	int ret = MEDIA_CONTENT_ERROR_NONE;
	int album_count = 0;
	filter_h filter;

	/*Set Filter*/
	char *condition = "MEDIA_TYPE=3";	/*0-image, 1-video, 2-sound, 3-music, 4-other*/

	ret = media_filter_create(&filter);
	if(ret != MEDIA_CONTENT_ERROR_NONE) {
		media_content_error("Fail to create filter");
		return ret;
	}
	ret = media_filter_set_condition(filter, condition, MEDIA_CONTENT_COLLATE_DEFAULT);
	if(ret != MEDIA_CONTENT_ERROR_NONE) {
		media_filter_destroy(filter);
		media_content_error("Fail to set condition");
		return ret;
	}
	ret = media_filter_set_order(filter, MEDIA_CONTENT_ORDER_ASC, MEDIA_ALBUM, MEDIA_CONTENT_COLLATE_NOCASE);
	if(ret != MEDIA_CONTENT_ERROR_NONE) {
		media_filter_destroy(filter);
		media_content_error("Fail to set order");
		return ret;
	}

	ret = media_album_get_album_count_from_db(filter, &album_count);
	if(ret != MEDIA_CONTENT_ERROR_NONE) {
		media_filter_destroy(filter);
		return ret;
	} else {
		media_content_debug("album_count [%d]", album_count);
	}

	ret = media_album_foreach_album_from_db(filter, album_list_cb, NULL);
	if(ret != MEDIA_CONTENT_ERROR_NONE)
		media_content_error("error media_album_foreach_album_from_db : [%d]", ret);

	ret = media_filter_destroy(filter);
	if(ret != MEDIA_CONTENT_ERROR_NONE)
		media_content_error("error media_filter_destroy : [%d]", ret);

	return ret;
}

int test_group_operation(void)
{
	media_content_debug("\n============Group Test============\n\n");

	int ret = MEDIA_CONTENT_ERROR_NONE;
	int group_count = 0;
	int idx = 0;

	ret = test_filter_create();
	if(ret != MEDIA_CONTENT_ERROR_NONE) {
		media_content_error("[error(0x%08x)", ret);
		return ret;
	}

	for(idx = 0; idx < MEDIA_CONTENT_GROUP_MAX; idx++)
	{
		ret = media_group_get_group_count_from_db(g_filter, idx, &group_count);
		if(ret != MEDIA_CONTENT_ERROR_NONE) {
			test_filter_destroy();
			media_content_error("media_group_get_group_count_from_db fail. idx=[%d]", ret, idx);
			return ret;
		} else {
			media_content_debug("[%2d]group_count [%d]", idx, group_count);
		}

		ret = media_group_foreach_group_from_db(g_filter, idx, group_list_cb, &idx);
	}
	ret = test_filter_destroy();

	return ret;
}

int test_update_operation()
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	int i;
	media_info_h media_handle = NULL;
	GList *all_item_list = NULL;

	/* Get all item list */
	ret = media_info_foreach_media_from_db(NULL, gallery_media_item_cb, &all_item_list);
	if(ret != MEDIA_CONTENT_ERROR_NONE) {
		media_content_error("media_info_foreach_media_from_db failed: %d", ret);
		return -1;
	} else {
		media_content_debug("media_info_foreach_media_from_db success");
		char *media_id = NULL;
		char *media_path = NULL;
		media_content_type_e media_type = 0;

		for(i = 0; i < g_list_length(all_item_list); i++) {
			media_handle = (media_info_h)g_list_nth_data(all_item_list, i);

			ret = media_info_get_media_id(media_handle, &media_id);
			if(ret != MEDIA_CONTENT_ERROR_NONE)
				media_content_error("media_info_get_media_id failed: %d", ret);
			ret = media_info_get_file_path(media_handle, &media_path);
			if(ret != MEDIA_CONTENT_ERROR_NONE)
				media_content_error("media_info_get_file_path failed: %d", ret);
			ret = media_info_get_media_type(media_handle, &media_type);
			if(ret != MEDIA_CONTENT_ERROR_NONE)
				media_content_error("media_info_get_media_type failed: %d", ret);

			media_content_debug("media_id [%d] : %s", i, media_id);
			media_content_debug("media_type [%d] : %d", i, media_type);
			media_content_debug("media_path [%d] : %s", i, media_path);
#if 0
			if(media_type == MEDIA_CONTENT_TYPE_IMAGE) {
				image_meta_h image_handle;
				media_content_orientation_e orientation;

				ret = media_info_get_image(media_handle, &image_handle);
				if(ret != MEDIA_CONTENT_ERROR_NONE) {
					media_content_error("media_info_get_image failed: %d", ret);
				} else {
					media_content_debug("media_info_get_image success");

					//update image meta
					orientation = MEDIA_CONTENT_ORIENTATION_ROT_180;
					image_meta_set_orientation(image_handle, orientation);

					ret = image_meta_update_to_db(image_handle);
					if(ret != MEDIA_CONTENT_ERROR_NONE) {
						media_content_error("image_meta_update_to_db failed: %d", ret);
					} else {
						media_content_debug("image_meta_update_to_db success");
					}
				}

			} else if(media_type == MEDIA_CONTENT_TYPE_VIDEO) {
				video_meta_h video_handle;

				ret = media_info_get_video(media_handle, &video_handle);
				if(ret != MEDIA_CONTENT_ERROR_NONE) {
					media_content_error("media_info_get_video failed: %d", ret);
				} else {
					media_content_debug("media_info_get_video success");

					//update video meta
					video_meta_set_played_count(video_handle,5);
					video_meta_set_played_time(video_handle,5);
					video_meta_set_played_position(video_handle,5);
					video_meta_update_to_db(video_handle);

					ret = video_meta_update_to_db(video_handle);
					if(ret != MEDIA_CONTENT_ERROR_NONE) {
						media_content_error("video_meta_update_to_db failed: %d", ret);
					} else {
						media_content_debug("video_meta_update_to_db success");
					}
				}
			} else if(media_type == MEDIA_CONTENT_TYPE_MUSIC) {//update audio meta
				audio_meta_h audio_handle = NULL;
				ret = media_info_get_audio(media_handle, &audio_handle);
				if(ret != MEDIA_CONTENT_ERROR_NONE) {
					media_content_error("media_info_get_audio failed: %d", ret);
				} else {
					media_content_debug("media_info_get_audio success");

					audio_meta_set_played_count(audio_handle,5);
					audio_meta_set_played_time(audio_handle,1000);
					audio_meta_set_played_position(audio_handle,180);

					ret = audio_meta_update_to_db(audio_handle);
					if(ret != MEDIA_CONTENT_ERROR_NONE) {
						media_content_error("audio_meta_update_to_db failed: %d", ret);
					} else {
						media_content_debug("audio_meta_update_to_db success");
					}
				}

			}
#endif
		}

		/* fix prevent: Resource Leak */
		SAFE_FREE(media_id);
		SAFE_FREE(media_path);
	}

	return MEDIA_CONTENT_ERROR_NONE;
}

int test_insert(void)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	char *path = "/opt/usr/media/Images/Default.jpg";
	//char *path = "/opt/usr/media/Others/other.txt";
	//char *path = NULL;
	media_info_h media_item = NULL;
	media_content_debug("\n============DB Insert Test============\n\n");

	ret = media_info_insert_to_db(path, &media_item);

	if((ret == MEDIA_CONTENT_ERROR_NONE) && (media_item != NULL))
	{
		media_content_debug("Insertion is success");
	}
	else
	{
		media_content_error("Insertion is failed");
		ret = media_info_destroy(media_item);
		return ret;
	}

	char *media_id = NULL;

	ret = media_info_get_media_id(media_item, &media_id);
	if(ret != MEDIA_CONTENT_ERROR_NONE)
		media_content_error("media_info_get_media_id failed: %d", ret);
	else
		media_content_debug("Media ID: %s", media_id);

	SAFE_FREE(media_id);

	ret = media_info_update_to_db(media_item);
	if(ret == MEDIA_CONTENT_ERROR_NONE)
		media_content_debug("media_info_update_to_db is success");
	else
		media_content_error("media_info_update_to_db is failed");

	ret = media_info_destroy(media_item);
	if(ret != MEDIA_CONTENT_ERROR_NONE)
		media_content_error("media_info_destroy failed: %d", ret);

	return ret;
}

int test_move(void)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	const char *move_media_id = "60aea677-4742-408e-b5f7-f2628062d06d";
	char *dst_path = "/opt/usr/media/Images/XX/Default1.jpg";
	media_info_h move_media = NULL;

	ret = media_info_get_media_from_db(move_media_id, &move_media);
	if(ret == MEDIA_CONTENT_ERROR_NONE)
		media_content_debug("media_info_get_media_from_db success");
	else
		media_content_error("media_info_get_media_from_db failed: %d", ret);

	media_content_debug("\n============DB Move Test============\n\n");

	if(move_media) {
		ret = media_info_move_to_db(move_media, dst_path);

		if(ret == MEDIA_CONTENT_ERROR_NONE)
			media_content_debug("Move is success");
		else
			media_content_error("Move is failed");

		ret = media_info_destroy(move_media);
	} else {
		media_content_debug("There is no item : %s", move_media_id);
	}

	return ret;
}

void thumbnail_completed_cb(media_content_error_e error, const char *path, void *user_data)
{
	char *thumbnail_path = NULL;
	int ret = MEDIA_CONTENT_ERROR_NONE;
	g_cnt++;

	media_content_debug("=================[%d][%d]", g_media_cnt, g_cnt);
	media_content_debug("error_code [%d]", error);
	media_content_debug("thumbnail_path [%s]", path);
	if(user_data != NULL)
	{
		media_info_h media = (media_info_h)user_data;
		ret = media_info_get_thumbnail_path(media, &thumbnail_path);
		if(ret != MEDIA_CONTENT_ERROR_NONE)
			media_content_error("media_info_get_thumbnail_path failed: %d", ret);
		else
			media_content_debug("thumbnail_path get from media[%s]", thumbnail_path);
		SAFE_FREE(thumbnail_path);
		ret = media_info_destroy(media);
		if(ret != MEDIA_CONTENT_ERROR_NONE)
			media_content_error("media_info_destroy failed: %d", ret);
	}

	if(g_cnt == g_media_cnt)
		g_main_loop_quit(g_loop);

	return;
}

bool thumbnail_create_cb(media_info_h media, void *user_data)
{
	char *media_id = NULL;
	media_info_h dst = NULL;
	int ret = MEDIA_CONTENT_ERROR_NONE;

	if(media == NULL)
	{
		media_content_debug("NO Item");
		return true;
	}

	ret = media_info_get_media_id(media, &media_id);
	if(ret != MEDIA_CONTENT_ERROR_NONE)
		media_content_error("media_info_get_media_id failed: %d", ret);
	else
		media_content_debug("media_id : [%s]", media_id);

	ret = media_info_clone(&dst, media);
	if(ret != MEDIA_CONTENT_ERROR_NONE)
		media_content_error("media_info_clone failed: %d", ret);
	else
	{
		ret = media_info_create_thumbnail(dst, thumbnail_completed_cb, dst);
		if(ret != MEDIA_CONTENT_ERROR_NONE)
			media_content_error("media_info_create_thumbnail failed: %d", ret);
	}

	/* fix prevent: Resource leak */
	SAFE_FREE(media_id);

	return true;
}

bool thumbnail_cancel_cb(media_info_h media, void *user_data)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;

	char *media_id = NULL;
	media_info_h dst = NULL;

	g_cnt++;

	if(media == NULL)
	{
		media_content_debug("NO Item");
		return true;
	}

	ret = media_info_get_media_id(media, &media_id);
	if(ret != MEDIA_CONTENT_ERROR_NONE)
		media_content_error("media_info_get_media_id failed: %d", ret);
	else
		media_content_debug("media_id : [%s]", media_id);

	ret = media_info_clone(&dst, media);
	if(ret != MEDIA_CONTENT_ERROR_NONE)
		media_content_error("media_info_clone failed: %d", ret);

	ret = media_info_cancel_thumbnail(dst);
	if(ret != MEDIA_CONTENT_ERROR_NONE)
		media_content_error("media_info_cancel_thumbnail failed: %d", ret);

	ret = media_info_destroy(dst);
	if(ret != MEDIA_CONTENT_ERROR_NONE)
			media_content_error("media_info_destroy failed: %d", ret);

	if(g_cnt == g_media_cnt)
		g_main_loop_quit(g_loop);

	/* fix prevent: Resource leak */
	SAFE_FREE(media_id);

	return true;
}

gboolean create_thumbnail_start(gpointer data)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;

	ret = media_info_foreach_media_from_db(g_filter, thumbnail_create_cb, NULL);

	if(ret == MEDIA_CONTENT_ERROR_NONE)
		media_content_debug("media_info_foreach_media_from_db is success");
	else
		media_content_error("media_info_foreach_media_from_db is failed");

	return false;
}

gboolean cancel_thumbnail_start(gpointer data)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;

	ret = media_info_foreach_media_from_db(g_filter, thumbnail_cancel_cb, NULL);

	if(ret == MEDIA_CONTENT_ERROR_NONE)
		media_content_debug("media_info_foreach_media_from_db is success");
	else
		media_content_error("media_info_foreach_media_from_db is failed");

	return false;
}

int test_create_thumbnail(int cancel)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	GSource *source = NULL;
	GMainContext *context = NULL;

	test_filter_create();

	ret = media_info_get_media_count_from_db(g_filter, &g_media_cnt);
	if(ret != MEDIA_CONTENT_ERROR_NONE)
		media_content_error("media_info_get_media_count_from_db failed: %d", ret);
	else
		media_content_debug("media_count : [%d]", g_media_cnt);

	g_loop = g_main_loop_new(NULL, FALSE);
	context = g_main_loop_get_context(g_loop);
	source = g_idle_source_new();
	g_source_set_callback (source, create_thumbnail_start, NULL, NULL);
	g_source_attach (source, context);

	/* Logic to cancel */
	if (cancel) {
		GSource *cancel_src = NULL;
		cancel_src = g_idle_source_new();
		g_source_set_callback (cancel_src, cancel_thumbnail_start, NULL, NULL);
		g_source_attach (cancel_src, context);
	}

	g_main_loop_run(g_loop);
	g_main_loop_unref(g_loop);

	test_filter_destroy();

	return ret;
}

int test_disconnect_database(void)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	media_content_debug("\n============DB Disconnection Test============\n\n");

	ret = media_content_disconnect();

	if(ret == MEDIA_CONTENT_ERROR_NONE)
		media_content_debug("disconnection is success");
	else
		media_content_error("disconnection is failed");

	return ret;
}

int test_request_update_db(void)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	media_info_h media = NULL;

	//get the content of Over the horizon
	ret = media_info_get_media_from_db("0f999626-6218-450c-a4ad-181a3bab6ebf", &media);
	if(ret != MEDIA_CONTENT_ERROR_NONE)
		media_content_error("media_info_get_media_from_db failed: %d", ret);

	test_disconnect_database();

	ret = media_info_set_content_name(media, "TEST_content_name");
	if(ret != MEDIA_CONTENT_ERROR_NONE)
		media_content_error("media_info_set_content_name failed: %d", ret);

	ret = media_info_update_to_db(media);
	if(ret == MEDIA_CONTENT_ERROR_NONE)
		media_content_debug("media_info_update_to_db is success");
	else
		media_content_error("media_info_update_to_db is failed");

	if(media != NULL)
		media_info_destroy(media);

	return ret;
}

int g_total_photo_size = 0;
int g_total_video_size = 0;
int g_total_mp3_size = 0;
int g_total_voice_memo_size = 0;

bool dft_cb(media_info_h media, void *user_data)
{
	unsigned long long file_size = 0;
	media_content_type_e media_type = -1;
	char *mime_type = NULL;
	int ret = MEDIA_CONTENT_ERROR_NONE;

	if(media == NULL)
	{
		return true;
	}

	ret = media_info_get_media_type(media, &media_type);
	if(ret != MEDIA_CONTENT_ERROR_NONE)
		media_content_error("media_info_get_media_type failed: %d", ret);
	ret = media_info_get_size(media, &file_size);
	if(ret != MEDIA_CONTENT_ERROR_NONE)
		media_content_error("media_info_get_size failed: %d", ret);
	ret = media_info_get_mime_type(media, &mime_type);
	if(ret != MEDIA_CONTENT_ERROR_NONE)
		media_content_error("media_info_get_mime_type failed: %d", ret);

	if(media_type == MEDIA_CONTENT_TYPE_IMAGE)
	{
		g_total_photo_size += file_size;
	}
	else if(media_type == MEDIA_CONTENT_TYPE_VIDEO)
	{
		g_total_video_size += file_size;
	}
	else if(media_type == MEDIA_CONTENT_TYPE_SOUND)
	{
		g_total_voice_memo_size += file_size;
	}
	else if(media_type == MEDIA_CONTENT_TYPE_MUSIC)
	{
		if((mime_type != NULL) && (!strcmp("audio/mpeg", mime_type)))
		{
			g_total_mp3_size += file_size;
		}
		else
		{
			g_total_voice_memo_size += file_size;
		}
	}
	else
	{
		media_content_debug("invalid media_type");
	}

	if(mime_type != NULL)
		free(mime_type);

	return true;

}

int DFT_test(void)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	media_content_debug("\n============DFT_test============\n\n");

	filter_h filter = NULL;
	int media_cnt = 0;

	/*MEDIA_TYPE 0-image, 1-video, 2-sound, 3-music, 4-other*/

	ret = media_filter_create(&filter);

/*Internal Memory*/
	media_content_debug("[Internal Memory]\n");
	/*1. Photo ============================================================*/
	ret = media_filter_set_condition(filter, "MEDIA_STORAGE_TYPE=0 AND MEDIA_TYPE=0", MEDIA_CONTENT_COLLATE_DEFAULT);

	/*Get Photo Count*/
	ret = media_info_get_media_count_from_db(filter, &media_cnt);
	media_content_debug("Photo count = [%d]\n", media_cnt);

	/*Get Photo Size*/
	ret = media_info_foreach_media_from_db(filter, dft_cb, NULL);
	media_content_debug("Photo size = [%d]\n", g_total_photo_size);

	/*2. Video ============================================================*/
	ret = media_filter_set_condition(filter, "MEDIA_STORAGE_TYPE=0 AND MEDIA_TYPE=1", MEDIA_CONTENT_COLLATE_DEFAULT);

	/*Get Video Count*/
	ret = media_info_get_media_count_from_db(filter, &media_cnt);
	media_content_debug("Video count = [%d]\n", media_cnt);

	/*Get Video Size*/
	ret = media_info_foreach_media_from_db(filter, dft_cb, NULL);
	media_content_debug("Video size = [%d]\n", g_total_video_size);

	/*3. MP3 ============================================================*/
	ret = media_filter_set_condition(filter, "MEDIA_STORAGE_TYPE=0 AND MEDIA_TYPE=3 AND MEDIA_MIME_TYPE=\"audio/mpeg\"", MEDIA_CONTENT_COLLATE_DEFAULT);

	/*Get MP3 Count*/
	ret = media_info_get_media_count_from_db(filter, &media_cnt);
	media_content_debug("MP3 count = [%d]\n", media_cnt);

	/*Get MP3 Size*/
	ret = media_info_foreach_media_from_db(filter, dft_cb, NULL);
	media_content_debug("MP3 size = [%d]\n", g_total_mp3_size);

	/*4. Voice Memo ============================================================*/
	ret = media_filter_set_condition(filter, "MEDIA_STORAGE_TYPE=0 AND (MEDIA_MIME_TYPE=\"audio/AMR\" OR MEDIA_MIME_TYPE=\"audio/mp4\")", MEDIA_CONTENT_COLLATE_DEFAULT);

	/*Get Voice Memo Count*/
	ret = media_info_get_media_count_from_db(filter, &media_cnt);
	media_content_debug("Voice Memo count = [%d]\n", media_cnt);

	/*Get Voice Memo Size*/
	ret = media_info_foreach_media_from_db(filter, dft_cb, NULL);
	media_content_debug("Voice Memo size = [%d]\n", g_total_voice_memo_size);

	g_total_photo_size = 0;
	g_total_video_size = 0;
	g_total_mp3_size = 0;
	g_total_voice_memo_size = 0;

/*External Memory*/
	media_content_debug("\n[External Memory]\n");
	/*1. Photo ============================================================*/
	ret = media_filter_set_condition(filter, "MEDIA_STORAGE_TYPE=1 AND MEDIA_TYPE=0", MEDIA_CONTENT_COLLATE_DEFAULT);

	/*Get Photo Count*/
	ret = media_info_get_media_count_from_db(filter, &media_cnt);
	media_content_debug("Photo count = [%d]\n", media_cnt);

	/*Get Photo Size*/
	ret = media_info_foreach_media_from_db(filter, dft_cb, NULL);
	media_content_debug("Photo size = [%d]\n", g_total_photo_size);

	/*2. Video ============================================================*/
	ret = media_filter_set_condition(filter, "MEDIA_STORAGE_TYPE=1 AND MEDIA_TYPE=1", MEDIA_CONTENT_COLLATE_DEFAULT);

	/*Get Video Count*/
	ret = media_info_get_media_count_from_db(filter, &media_cnt);
	media_content_debug("Video count = [%d]\n", media_cnt);

	/*Get Video Size*/
	ret = media_info_foreach_media_from_db(filter, dft_cb, NULL);
	media_content_debug("Video size = [%d]\n", g_total_video_size);

	/*3. MP3 ============================================================*/
	ret = media_filter_set_condition(filter, "MEDIA_STORAGE_TYPE=1 AND MEDIA_TYPE=3 AND MEDIA_MIME_TYPE=\"audio/mpeg\"", MEDIA_CONTENT_COLLATE_DEFAULT);

	/*Get MP3 Count*/
	ret = media_info_get_media_count_from_db(filter, &media_cnt);
	media_content_debug("MP3 count = [%d]\n", media_cnt);

	/*Get MP3 Size*/
	ret = media_info_foreach_media_from_db(filter, dft_cb, NULL);
	media_content_debug("MP3 size = [%d]\n", g_total_mp3_size);

	/*4. Voice Memo ============================================================*/
	ret = media_filter_set_condition(filter, "MEDIA_STORAGE_TYPE=1 AND (MEDIA_MIME_TYPE=\"audio/AMR\" OR MEDIA_MIME_TYPE=\"audio/mp4\")", MEDIA_CONTENT_COLLATE_DEFAULT);

	/*Get Voice Memo Count*/
	ret = media_info_get_media_count_from_db(filter, &media_cnt);
	media_content_debug("Voice Memo count = [%d]\n", media_cnt);

	/*Get Voice Memo Size*/
	ret = media_info_foreach_media_from_db(filter, dft_cb, NULL);
	media_content_debug("Voice Memo size = [%d]\n", g_total_voice_memo_size);
	ret = media_filter_destroy(filter);

	return ret;
}

void insert_batch_cb(media_content_error_e error, void * user_data)
{
	media_content_debug("media_info_insert_batch_to_db completed!\n");
}

int test_batch_operations()
{
	int ret = -1;
	int i;
	char *file_list[10];

	for (i = 0; i < 10; i++) {
		char filepath[255] = {0,};
		snprintf(filepath, sizeof(filepath), "%s%d.jpg", "/opt/usr/media/test/image", i+1);
		media_content_debug("File : %s\n", filepath);
		file_list[i] = strdup(filepath);
	}

	ret = media_info_insert_batch_to_db((const char **)file_list, 10, insert_batch_cb, NULL);
	if(ret != MEDIA_CONTENT_ERROR_NONE) {
		media_content_error("media_info_insert_batch_to_db failed : %d\n", ret);
	}

	filter_h filter;
	char *condition = "MEDIA_PATH LIKE \'/opt/usr/media/test/image%%jpg\'";

	ret = media_filter_create(&filter);
	if(ret != MEDIA_CONTENT_ERROR_NONE) {
		media_content_error("Fail to create filter");
		return ret;
	}

	ret = media_filter_set_condition(filter, condition, MEDIA_CONTENT_COLLATE_DEFAULT);
	if(ret != MEDIA_CONTENT_ERROR_NONE) {
		media_filter_destroy(filter);
		media_content_error("Fail to set condition");
		return ret;
	}

	ret = media_info_delete_batch_from_db(filter);
	if(ret != MEDIA_CONTENT_ERROR_NONE) {
		media_filter_destroy(filter);
		media_content_error("media_info_delete_batch_from_db failed : %d\n", ret);
		return ret;
	}

	ret = media_filter_destroy(filter);

	return ret;
}

void insert_burst_shot_cb(media_content_error_e error, void * user_data)
{
	media_content_debug("media_info_insert_burst_shot_to_db completed![%d]\n", error);
	g_main_loop_quit(g_loop);
}

gboolean test_insert_burst_shot_to_db_start(gpointer data)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	int i = 0;
	char *file_list[10];

	for (i = 0; i < 10; i++) {
		char filepath[255] = {0,};
		snprintf(filepath, sizeof(filepath), "%s%d.jpg", "/opt/usr/media/test/image", i+1);
		media_content_debug("File : %s\n", filepath);
		file_list[i] = strdup(filepath);
	}

	ret = media_info_insert_burst_shot_to_db((const char **)file_list, 10, insert_burst_shot_cb, NULL);
	if(ret != MEDIA_CONTENT_ERROR_NONE) {
		media_content_error("media_info_insert_burst_shot_to_db failed : %d\n", ret);
	}

	return ret;
}

int test_insert_burst_shot_to_db(void)
{
	GSource *source = NULL;
	GMainContext *context = NULL;

	g_loop = g_main_loop_new(NULL, FALSE);
	context = g_main_loop_get_context(g_loop);
	source = g_idle_source_new();
	g_source_set_callback (source, test_insert_burst_shot_to_db_start, NULL, NULL);
	g_source_attach (source, context);

	g_main_loop_run(g_loop);
	g_main_loop_unref(g_loop);

	return 0;
}

void _scan_cb(media_content_error_e err, void *user_data)
{
	media_content_debug("scan callback is called : %d\n", err);
	g_main_loop_quit(g_loop);

	return;
}

int test_scan_file()
{
	int ret = -1;

	const char *file_path = "/opt/usr/media/test/image1.jpg";

	ret = media_content_scan_file(file_path);
	if(ret != MEDIA_CONTENT_ERROR_NONE) {
		media_content_error("Fail to media_content_scan_file : %d", ret);
		return ret;
	}

	return 0;
}

gboolean test_scan_dir_start(gpointer data)
{
	int ret = -1;

	const char *dir_path = "/opt/usr/media";

	ret = media_content_scan_folder(dir_path, TRUE, _scan_cb, NULL);

	if(ret != MEDIA_CONTENT_ERROR_NONE) {
		media_content_error("Fail to test_scan_dir_start : %d", ret);
		return ret;
	}

	return 0;
}

int test_scan_dir()
{
	GSource *source = NULL;
	GMainContext *context = NULL;

	g_loop = g_main_loop_new(NULL, FALSE);
	context = g_main_loop_get_context(g_loop);
	source = g_idle_source_new();
	g_source_set_callback (source, test_scan_dir_start, NULL, NULL);
	g_source_attach (source, context);

	g_main_loop_run(g_loop);
	g_main_loop_unref(g_loop);

	return 0;
}

void _noti_cb(media_content_error_e error,
				int pid,
				media_content_db_update_item_type_e update_item,
				media_content_db_update_type_e update_type,
				media_content_type_e media_type,
				char *uuid,
				char *path,
				char *mime_type,
				void *user_data)
{
	if (error == 0) {
		media_content_debug("noti success! : %d\n", error);
	} else {
		media_content_debug("error occured! : %d\n", error);
	}

	media_content_debug("Noti from PID(%d)\n", pid);

	if (update_item == MEDIA_ITEM_FILE) {
		media_content_debug("Noti item : MEDIA_ITEM_FILE\n");
	} else if (update_item == MEDIA_ITEM_DIRECTORY) {
		media_content_debug("Noti item : MEDIA_ITEM_DIRECTORY\n");
	}

	if (update_type == MEDIA_CONTENT_INSERT) {
		media_content_debug("Noti type : MEDIA_CONTENT_INSERT\n");
	} else if (update_type == MEDIA_CONTENT_DELETE) {
		media_content_debug("Noti type : MEDIA_CONTENT_DELETE\n");
	} else if (update_type == MEDIA_CONTENT_UPDATE) {
		media_content_debug("Noti type : MEDIA_CONTENT_UPDATE\n");
	}

	media_content_debug("content type : %d\n", media_type);

	if (path)
		media_content_debug("path : %s\n", path);
	else
		media_content_debug("path not\n");

	if (uuid)
		media_content_debug("uuid : %s\n", uuid);
	else
		media_content_debug("uuid not\n");

	if (mime_type)
		media_content_debug("mime_type : %s\n", mime_type);
	else
		media_content_debug("mime not\n");

	if (user_data) media_content_debug("String : %s\n", (char *)user_data);

	//g_main_loop_quit(g_loop);
	return;
}

gboolean _send_noti_operations(gpointer data)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;

	/* First of all, noti subscription */
	char *user_str = strdup("hi");
	media_content_set_db_updated_cb(_noti_cb, (void*)user_str);

	/* media_info_insert_to_db */
	media_info_h media_item = NULL;
	char *path = "/opt/usr/media/test/image1.jpg";

	ret = media_info_insert_to_db(path, &media_item);
	if (ret != MEDIA_CONTENT_ERROR_NONE) {
		media_content_error("media_info_insert_to_db failed : %d", ret);
		media_info_destroy(media_item);
		return FALSE;
	}

	media_content_debug("media_info_insert_to_db success");

	/* media_info_delete_batch_from_db */
	filter_h filter;
	char *condition = "MEDIA_PATH LIKE \'/opt/usr/media/test/image%%jpg\'";

	ret = media_filter_create(&filter);
	if(ret != MEDIA_CONTENT_ERROR_NONE) {
		media_content_error("Fail to create filter");
		media_info_destroy(media_item);
		return ret;
	}

	ret = media_filter_set_condition(filter, condition, MEDIA_CONTENT_COLLATE_DEFAULT);
	if(ret != MEDIA_CONTENT_ERROR_NONE) {
		media_filter_destroy(filter);
		media_info_destroy(media_item);
		media_content_error("Fail to set condition");
		return ret;
	}
	ret = media_info_delete_batch_from_db(filter);
	if(ret != MEDIA_CONTENT_ERROR_NONE) {
		media_filter_destroy(filter);
		media_info_destroy(media_item);
		media_content_error("media_info_delete_batch_from_db failed : %d\n", ret);
		return ret;
	}

	media_filter_destroy(filter);

	/* media_info_update_to_db */
	ret = media_info_update_to_db(media_item);
	if(ret != MEDIA_CONTENT_ERROR_NONE) {
		media_content_error("media_info_update_to_db failed : %d\n", ret);
		media_info_destroy(media_item);
		return ret;
	}

	media_info_destroy(media_item);

	return FALSE;
}

int test_noti()
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	GSource *source = NULL;
	GMainContext *context = NULL;

	g_loop = g_main_loop_new(NULL, FALSE);
	context = g_main_loop_get_context(g_loop);
	source = g_idle_source_new();
	g_source_set_callback (source, _send_noti_operations, NULL, NULL);
	g_source_attach (source, context);

	g_main_loop_run(g_loop);
	g_main_loop_unref(g_loop);

	test_filter_destroy();
	media_content_unset_db_updated_cb();

	return ret;
}

int main(int argc, char *argv[])
{
	int ret = MEDIA_CONTENT_ERROR_NONE;

	media_content_debug("--- content manager test start ---\n\n");

	ret = test_connect_database();
	if(ret != MEDIA_CONTENT_ERROR_NONE)
		return MEDIA_CONTENT_ERROR_NONE;

#if 0
	ret = test_move();
	if(ret != MEDIA_CONTENT_ERROR_NONE)
		return ret;

	ret = test_gallery_scenario();
	if(ret != MEDIA_CONTENT_ERROR_NONE)
		return ret;

	ret = test_get_all_music_files();
	if(ret != MEDIA_CONTENT_ERROR_NONE)
		return ret;

	ret = test_media_info_operation();
	if(ret != MEDIA_CONTENT_ERROR_NONE)
		return ret;

	ret = test_folder_operation();
	if(ret != MEDIA_CONTENT_ERROR_NONE)
		return ret;

	ret = test_folder_update();
	if(ret != MEDIA_CONTENT_ERROR_NONE)
		return ret;

	ret = test_playlist_operation();
	if(ret != MEDIA_CONTENT_ERROR_NONE)
		return ret;

	ret = test_tag_operation();
	if(ret != MEDIA_CONTENT_ERROR_NONE)
		return ret;

	ret = test_bookmark_operation();
	if(ret != MEDIA_CONTENT_ERROR_NONE)
		return ret;

	ret = test_album_list();
	if(ret != MEDIA_CONTENT_ERROR_NONE)
		return ret;

	ret = test_group_operation();
	if(ret != MEDIA_CONTENT_ERROR_NONE)
		return ret;

	ret = test_update_operation();
	if(ret != MEDIA_CONTENT_ERROR_NONE)
		return ret;

	ret = test_insert();
	if(ret != MEDIA_CONTENT_ERROR_NONE)
		return ret;

	ret = test_move();
	if(ret != MEDIA_CONTENT_ERROR_NONE)
		return ret;

	ret = test_create_thumbnail(TRUE);
	if(ret != MEDIA_CONTENT_ERROR_NONE)
		return ret;

	ret = test_request_update_db();
	if(ret != MEDIA_CONTENT_ERROR_NONE)
		return ret;

	ret = DFT_test();
	if(ret != MEDIA_CONTENT_ERROR_NONE)
		return ret;

	ret = test_batch_operations();
	if(ret != MEDIA_CONTENT_ERROR_NONE)
		return MEDIA_CONTENT_ERROR_NONE;

	ret = test_insert_burst_shot_to_db();
	if(ret != MEDIA_CONTENT_ERROR_NONE)
		return MEDIA_CONTENT_ERROR_NONE;

	ret = test_scan_file();
	if(ret != MEDIA_CONTENT_ERROR_NONE)
		return MEDIA_CONTENT_ERROR_NONE;

	ret = test_scan_dir();
	if(ret != MEDIA_CONTENT_ERROR_NONE)
		return MEDIA_CONTENT_ERROR_NONE;

	ret = test_noti();
	if(ret != MEDIA_CONTENT_ERROR_NONE)
		return MEDIA_CONTENT_ERROR_NONE;
#endif

	ret = test_disconnect_database();
	if(ret != MEDIA_CONTENT_ERROR_NONE)
		return ret;

	media_content_debug("--- content manager test end ---\n");

	return ret;
}
