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


#include <media_info_private.h>


int image_meta_destroy(image_meta_h image)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	image_meta_s *_image = (image_meta_s*)image;

	if(_image)
	{
		SAFE_FREE(_image->media_id);
		SAFE_FREE(_image->date_taken);
		SAFE_FREE(_image->title);
		SAFE_FREE(_image->weather);
		SAFE_FREE(_image->burst_id);
		SAFE_FREE(_image);

		ret = MEDIA_CONTENT_ERROR_NONE;
	}
	else
	{
		media_content_error("INVALID_PARAMETER(0x%08x)", MEDIA_CONTENT_ERROR_INVALID_PARAMETER);
		ret = MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
	}

	return ret;
}

int image_meta_clone(image_meta_h *dst, image_meta_h src)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	image_meta_s *_src = (image_meta_s*)src;

	if(_src != NULL)
	{
		image_meta_s *_dst = (image_meta_s*)calloc(1, sizeof(image_meta_s));
		if(NULL == _dst)
		{
			media_content_error("OUT_OF_MEMORY(0x%08x)", MEDIA_CONTENT_ERROR_OUT_OF_MEMORY);
			return MEDIA_CONTENT_ERROR_OUT_OF_MEMORY;
		}

		if(STRING_VALID(_src->media_id))
		{
			_dst->media_id = strdup(_src->media_id);
			if(_dst->media_id == NULL)
			{
				media_content_error("OUT_OF_MEMORY(0x%08x)", MEDIA_CONTENT_ERROR_OUT_OF_MEMORY);
				image_meta_destroy((image_meta_h)_dst);
				return MEDIA_CONTENT_ERROR_OUT_OF_MEMORY;
			}
		}

		if(STRING_VALID(_src->date_taken))
		{
			_dst->date_taken = strdup(_src->date_taken);
			if(_dst->date_taken == NULL)
			{
				media_content_error("OUT_OF_MEMORY(0x%08x)", MEDIA_CONTENT_ERROR_OUT_OF_MEMORY);
				image_meta_destroy((image_meta_h)_dst);
				return MEDIA_CONTENT_ERROR_OUT_OF_MEMORY;
			}
		}

		if(STRING_VALID(_src->title))
		{
			_dst->title = strdup(_src->title);
			if(_dst->title == NULL)
			{
				media_content_error("OUT_OF_MEMORY(0x%08x)", MEDIA_CONTENT_ERROR_OUT_OF_MEMORY);
				image_meta_destroy((image_meta_h)_dst);
				return MEDIA_CONTENT_ERROR_OUT_OF_MEMORY;
			}
		}

		if(STRING_VALID(_src->weather))
		{
			_dst->weather = strdup(_src->weather);
			if(_dst->weather == NULL)
			{
				media_content_error("OUT_OF_MEMORY(0x%08x)", MEDIA_CONTENT_ERROR_OUT_OF_MEMORY);
				image_meta_destroy((image_meta_h)_dst);
				return MEDIA_CONTENT_ERROR_OUT_OF_MEMORY;
			}
		}

		if(STRING_VALID(_src->burst_id))
		{
			_dst->burst_id = strdup(_src->burst_id);
			if(_dst->burst_id == NULL)
			{
				media_content_error("OUT_OF_MEMORY(0x%08x)", MEDIA_CONTENT_ERROR_OUT_OF_MEMORY);
				image_meta_destroy((image_meta_h)_dst);
				return MEDIA_CONTENT_ERROR_OUT_OF_MEMORY;
			}
		}

		_dst->width = _src->width;
		_dst->height = _src->height;
		_dst->orientation = _src->orientation;

		*dst = (image_meta_h)_dst;

		ret = MEDIA_CONTENT_ERROR_NONE;
	}
	else
	{
		media_content_error("INVALID_PARAMETER(0x%08x)", MEDIA_CONTENT_ERROR_INVALID_PARAMETER);
		ret = MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
	}

	return ret;
}

int image_meta_get_media_id(image_meta_h image, char **media_id)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	image_meta_s *_image = (image_meta_s*)image;

	if(_image && media_id)
	{
		if(STRING_VALID(_image->media_id))
		{
			char *new_string = strdup(_image->media_id);
			if(NULL == new_string)
			{
				media_content_error("OUT_OF_MEMORY(0x%08x)", MEDIA_CONTENT_ERROR_OUT_OF_MEMORY);
				return MEDIA_CONTENT_ERROR_OUT_OF_MEMORY;
			}
			*media_id = new_string;
		}
		else
		{
			*media_id = NULL;
		}
		ret = MEDIA_CONTENT_ERROR_NONE;

	}
	else
	{
		media_content_error("INVALID_PARAMETER(0x%08x)", MEDIA_CONTENT_ERROR_INVALID_PARAMETER);
		ret = MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
	}

	return ret;

}

int image_meta_get_width(image_meta_h image, int *width)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	image_meta_s *_image = (image_meta_s*)image;

	if(_image && width)
	{
		*width = _image->width;
		ret = MEDIA_CONTENT_ERROR_NONE;
	}
	else
	{
		media_content_error("INVALID_PARAMETER(0x%08x)", MEDIA_CONTENT_ERROR_INVALID_PARAMETER);
		ret = MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
	}

	return ret;
}
int image_meta_get_height(image_meta_h image, int *height)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	image_meta_s *_image = (image_meta_s*)image;

	if(_image && height)
	{
		*height = _image->height;
		ret = MEDIA_CONTENT_ERROR_NONE;
	}
	else
	{
		media_content_error("INVALID_PARAMETER(0x%08x)", MEDIA_CONTENT_ERROR_INVALID_PARAMETER);
		ret = MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
	}

	return ret;
}

int image_meta_get_orientation(image_meta_h image, media_content_orientation_e* orientation)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	image_meta_s *_image = (image_meta_s*)image;

	if(_image && orientation)
	{
		*orientation = _image->orientation;
		ret = MEDIA_CONTENT_ERROR_NONE;
	}
	else
	{
		media_content_error("INVALID_PARAMETER(0x%08x)", MEDIA_CONTENT_ERROR_INVALID_PARAMETER);
		ret = MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
	}

	return ret;
}

int image_meta_get_date_taken(image_meta_h image, char **date_taken)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	image_meta_s *_image = (image_meta_s*)image;

	if(_image && date_taken)
	{
		if(STRING_VALID(_image->date_taken))
		{
			char *new_string = strdup(_image->date_taken);
			if(NULL == new_string)
			{
				media_content_error("OUT_OF_MEMORY(0x%08x)", MEDIA_CONTENT_ERROR_OUT_OF_MEMORY);
				return MEDIA_CONTENT_ERROR_OUT_OF_MEMORY;
			}
			*date_taken = new_string;
		}
		else
		{
			*date_taken = NULL;
		}

		ret = MEDIA_CONTENT_ERROR_NONE;
	}
	else
	{
		media_content_error("INVALID_PARAMETER(0x%08x)", MEDIA_CONTENT_ERROR_INVALID_PARAMETER);
		ret = MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
	}

	return ret;
}

int image_meta_get_burst_id(image_meta_h image, char **burst_id)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	image_meta_s *_image = (image_meta_s*)image;

	if(_image && burst_id)
	{
		if(STRING_VALID(_image->burst_id))
		{
			*burst_id = strdup(_image->burst_id);
			if(*burst_id == NULL)
			{
				media_content_error("OUT_OF_MEMORY(0x%08x)", MEDIA_CONTENT_ERROR_OUT_OF_MEMORY);
				return MEDIA_CONTENT_ERROR_OUT_OF_MEMORY;
			}
		}
		else
		{
			*burst_id = NULL;
		}
		ret = MEDIA_CONTENT_ERROR_NONE;
	}
	else
	{
		media_content_error("INVALID_PARAMETER(0x%08x)", MEDIA_CONTENT_ERROR_INVALID_PARAMETER);
		ret = MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
	}

	return ret;
}

int image_meta_is_burst_shot(image_meta_h image, bool *is_burst_shot)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	image_meta_s *_image = (image_meta_s*)image;

	if(_image && is_burst_shot)
	{
		if(STRING_VALID(_image->burst_id))
			*is_burst_shot = true;
		else
			*is_burst_shot = false;

		ret = MEDIA_CONTENT_ERROR_NONE;
	}
	else
	{
		media_content_error("INVALID_PARAMETER(0x%08x)", MEDIA_CONTENT_ERROR_INVALID_PARAMETER);
		ret = MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
	}

	return ret;
}

int image_meta_set_orientation(image_meta_h image, media_content_orientation_e orientation)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	image_meta_s *_image = (image_meta_s*)image;

	if(_image == NULL)
	{
		media_content_error("INVALID_PARAMETER(0x%08x)", MEDIA_CONTENT_ERROR_INVALID_PARAMETER);
		return MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
	}

	if((orientation < MEDIA_CONTENT_ORIENTATION_NOT_AVAILABLE) || (orientation > MEDIA_CONTENT_ORIENTATION_ROT_270))
	{
		media_content_error("INVALID_PARAMETER(0x%08x)", MEDIA_CONTENT_ERROR_INVALID_PARAMETER);
		return MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
	}

	_image->orientation = orientation;

	return ret;
}

int image_meta_update_to_db(image_meta_h image)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	image_meta_s *_image = (image_meta_s*)image;
	char *sql = NULL;

	if(_image != NULL && STRING_VALID(_image->media_id))
	{
		sql = sqlite3_mprintf(UPDATE_IMAGE_META_FROM_MEDIA, _image->orientation, _image->weather, _image->media_id);
		ret = _content_query_sql(sql);
		sqlite3_free(sql);
	}
	else
	{
		media_content_error("INVALID_PARAMETER(0x%08x)", MEDIA_CONTENT_ERROR_INVALID_PARAMETER);
		ret = MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
	}

	return ret;
}