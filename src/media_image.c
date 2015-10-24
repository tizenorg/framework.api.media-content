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
		SAFE_FREE(_image->exposure_time);
		SAFE_FREE(_image->model);
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
		media_content_retvm_if(_dst == NULL, MEDIA_CONTENT_ERROR_OUT_OF_MEMORY, "OUT_OF_MEMORY");

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

		if(STRING_VALID(_src->exposure_time))
		{
			_dst->exposure_time = strdup(_src->exposure_time);
			if(_dst->exposure_time == NULL)
			{
				media_content_error("OUT_OF_MEMORY(0x%08x)", MEDIA_CONTENT_ERROR_OUT_OF_MEMORY);
				image_meta_destroy((image_meta_h)_dst);
				return MEDIA_CONTENT_ERROR_OUT_OF_MEMORY;
			}
		}

		if(STRING_VALID(_src->model))
		{
			_dst->model = strdup(_src->model);
			if(_dst->model == NULL)
			{
				media_content_error("OUT_OF_MEMORY(0x%08x)", MEDIA_CONTENT_ERROR_OUT_OF_MEMORY);
				image_meta_destroy((image_meta_h)_dst);
				return MEDIA_CONTENT_ERROR_OUT_OF_MEMORY;
			}
		}

		_dst->fnumber = _src->fnumber;
		_dst->iso = _src->iso;
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
			media_content_retvm_if(new_string == NULL, MEDIA_CONTENT_ERROR_OUT_OF_MEMORY, "OUT_OF_MEMORY");

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
			media_content_retvm_if(new_string == NULL, MEDIA_CONTENT_ERROR_OUT_OF_MEMORY, "OUT_OF_MEMORY");

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
			media_content_retvm_if(*burst_id == NULL, MEDIA_CONTENT_ERROR_OUT_OF_MEMORY, "OUT_OF_MEMORY");
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

int image_meta_get_exposure_time(image_meta_h image, char **exposure_time)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	image_meta_s *_image = (image_meta_s*)image;

	if(_image && exposure_time)
	{
		if(STRING_VALID(_image->exposure_time))
		{
			*exposure_time = strdup(_image->exposure_time);
			media_content_retvm_if(*exposure_time == NULL, MEDIA_CONTENT_ERROR_OUT_OF_MEMORY, "OUT_OF_MEMORY");
		}
		else
		{
			*exposure_time = NULL;
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

int image_meta_get_fnumber(image_meta_h image, double *fnumber)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	image_meta_s *_image = (image_meta_s*)image;

	if(_image && fnumber)
	{
		*fnumber = _image->fnumber;
		ret = MEDIA_CONTENT_ERROR_NONE;
	}
	else
	{
		media_content_error("INVALID_PARAMETER(0x%08x)", MEDIA_CONTENT_ERROR_INVALID_PARAMETER);
		ret = MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
	}

	return ret;
}

int image_meta_get_iso(image_meta_h image, int *iso)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	image_meta_s *_image = (image_meta_s*)image;

	if(_image && iso)
	{
		*iso = _image->iso;
		ret = MEDIA_CONTENT_ERROR_NONE;
	}
	else
	{
		media_content_error("INVALID_PARAMETER(0x%08x)", MEDIA_CONTENT_ERROR_INVALID_PARAMETER);
		ret = MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
	}

	return ret;
}

int image_meta_get_model(image_meta_h image, char **model)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	image_meta_s *_image = (image_meta_s*)image;

	if(_image && model)
	{
		if(STRING_VALID(_image->model))
		{
			*model = strdup(_image->model);
			media_content_retvm_if(*model == NULL, MEDIA_CONTENT_ERROR_OUT_OF_MEMORY, "OUT_OF_MEMORY");
		}
		else
		{
			*model = NULL;
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
		char storage_id[MEDIA_CONTENT_UUID_SIZE+1] = {0,};
		memset(storage_id, 0x00, sizeof(storage_id));

		ret = _media_db_get_storage_id_by_media_id(_image->media_id, storage_id);
		media_content_retv_if(ret != MEDIA_CONTENT_ERROR_NONE, ret);

		sql = sqlite3_mprintf(UPDATE_IMAGE_META_FROM_MEDIA, storage_id, _image->orientation, _image->weather, _image->media_id);
		ret = _content_query_sql(sql);
		SQLITE3_SAFE_FREE(sql);
	}
	else
	{
		media_content_error("INVALID_PARAMETER(0x%08x)", MEDIA_CONTENT_ERROR_INVALID_PARAMETER);
		ret = MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
	}

	return ret;
}