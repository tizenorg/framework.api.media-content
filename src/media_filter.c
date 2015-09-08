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
#include <vconf.h>

static char *media_token[] =
{
	" ",
	"\"",
	"'",
	"(",
	")",
	"=",
	"<=",
	"<",
	">=",
	">",
};


typedef struct _token_t
{
	int type;
	char *str;
}token_t;


#define MAX_LEFT_VALUE 512
#define SPACE_LEN 1
#define SPACE " "
#define UNKNOWN_TYPE 1000
#define STRING_TYPE 100

static char *__get_order_str(media_content_order_e order_enum);
static char *__get_collate_str(media_content_collation_e collate_type);
static void __filter_attribute_free_value(gpointer key, gpointer value, gpointer user_data);
static char *__media_filter_replace_attr(attribute_h attr, char *name);
static int __tokenize_operator(token_t *token, const char *str, int op_type);
static int __tokenize(GList **token_list, const char *str);

static bool __is_pinyin_needed(void)
{
	char *lang = NULL;
	char *china = "zh_CN";
	char *hongkong = "zh_HK";
	int ret = FALSE;

	/*Check CSC first*/
	bool pinyin_support = FALSE;
	media_svc_check_pinyin_support(&pinyin_support);
	if(pinyin_support)
	{
		/*Check Language Setting*/
		lang = vconf_get_str(VCONFKEY_LANGSET);
		if (lang == NULL)
		{
			media_content_error("Fail to get string of language set");
			return ret;
		}
		if((strncmp(china, lang, strlen(china)) == 0) ||
			(strncmp(hongkong, lang, strlen(hongkong)) == 0))
		{
			ret = TRUE;
		}

		SAFE_FREE(lang);
	}

	return ret;
}

static char *__get_order_str(media_content_order_e order_enum)
{
	switch(order_enum) {
		case MEDIA_CONTENT_ORDER_ASC:
			return "ASC";
		case MEDIA_CONTENT_ORDER_DESC:
			return "DESC";
		default:
			return " ";
	}
}

static char *__get_collate_str(media_content_collation_e collate_type)
{
	switch(collate_type) {
		case MEDIA_CONTENT_COLLATE_NOCASE:
			return "NOCASE";
		case MEDIA_CONTENT_COLLATE_RTRIM:
			return "RTRIM";
		case MEDIA_CONTENT_COLLATE_LOCALIZED:
			if(__is_pinyin_needed())
				return "NOCASE";
			else
				return "localized";
		default: return " ";
	}
}

static void __filter_attribute_free_value(gpointer key, gpointer value, gpointer user_data)
{
	SAFE_FREE(key);
	SAFE_FREE(value);
}

static char *__media_filter_replace_attr(attribute_h attr, char *name)
{
	char *key_temp = NULL;
	char *generated_value = NULL;
	attribute_s *_attr = (attribute_s *)attr;

	if(!g_hash_table_lookup_extended(_attr->attr_map,
										name,
										(gpointer)&key_temp, (gpointer)&generated_value))
	{
		//can't find the value
		//media_content_error("NOT_FOUND_VALUE(%s)", name);
		return NULL;
	}

	if(STRING_VALID(generated_value))
	{
		return strdup(generated_value);
	}

	media_content_error("__media_filter_replace_attr fail");

	return NULL;
}

static int __tokenize_operator(token_t *token, const char *str, int op_type)
{
	int ret = 0;
	const char *tmp = str;

	if(token != NULL && STRING_VALID(tmp))
	{
		token->type = op_type;
		int token_size = strlen(media_token[op_type]);
		if(token_size == 0)
		{
			media_content_error("Invalid token_size. op_type[%d]", op_type);
			return -1;
		}

		token->str = (char*)calloc(token_size+1, sizeof(char));
		if(token->str == NULL)
		{
			media_content_error("OUT_OF_MEMORY(0x%08x)", MEDIA_CONTENT_ERROR_OUT_OF_MEMORY);
			return -1;
		}

		strncpy(token->str, tmp, token_size);
		//media_content_debug("type : [%d] str : [%s]", token->type, token->str);
		ret = token_size;
	}
	else
	{
		ret = -1;
	}

	return ret;
}

static int __tokenize_string(token_t *token, const char *str, int size)
{
	int ret = size;
	const char *tmp = str;

	if(token != NULL && STRING_VALID(tmp) && size > 0)
	{
		token->str = (char*)calloc(size+1, sizeof(char));
		if(token->str == NULL)
		{
			media_content_error("OUT_OF_MEMORY(0x%08x)", MEDIA_CONTENT_ERROR_OUT_OF_MEMORY);
			return -1;
		}
		token->type = UNKNOWN_TYPE;
		strncpy(token->str, tmp, size);
		//media_content_debug("type : [%d] str : [%s]", token->type, token->str);
	}
	else
	{
		ret = -1;
	}

	return ret;
}

static int __tokenize_attribute(GList **token_list, const char *str)
{
	int ret = 0;
	int idx = 0;

	if(!STRING_VALID(str)) {
		media_content_error("Parameter string in invalid");
		return MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
	}

	const char *tmp = str;
	const char *dst_ptr = str + strlen(str);

	for (idx = 0; (*(tmp+idx)) && (tmp < dst_ptr); idx++)
	{
		//media_content_debug("[%d] '%c'", idx, tmp[idx]);
		if(tmp[idx] == ' ')		//" "
		{
			if(idx == 0)		// ignore the space.
			{
				tmp++;
				idx = -1;
				continue;
			}
			token_t *token = (token_t*)calloc(1, sizeof(token_t));
			if(token == NULL)
			{
				media_content_error("OUT_OF_MEMORY(0x%08x)", MEDIA_CONTENT_ERROR_OUT_OF_MEMORY);
				return -1;
			}
			token->type = UNKNOWN_TYPE;
			token->str = (char*)calloc(idx+1, sizeof(char));
			if(token->str == NULL)
			{
				SAFE_FREE(token);
				media_content_error("OUT_OF_MEMORY(0x%08x)", MEDIA_CONTENT_ERROR_OUT_OF_MEMORY);
				return -1;
			}
			strncpy(token->str, tmp, idx);
			//media_content_debug("type : [%d] str : [%s]", token->type, token->str);
			*token_list = g_list_append(*token_list, token);
			tmp = tmp +idx + strlen(media_token[0]);
			idx = -1;
		}
		else if(tmp[idx] == ',')	// " , "
		{
			if(idx != 0)
			{
				token_t *token = (token_t*)calloc(1, sizeof(token_t));
				if(token == NULL)
				{
					media_content_error("OUT_OF_MEMORY(0x%08x)", MEDIA_CONTENT_ERROR_OUT_OF_MEMORY);
					return -1;
				}
				ret = __tokenize_string(token, tmp, idx);
				if (ret < 0)
				{
					SAFE_FREE(token);
					media_content_error("tokenize error occured");
					return -1;
				}
				else
				{
					*token_list = g_list_append(*token_list, token);
					tmp = tmp + idx;
				}
			}

			token_t *token = (token_t*)calloc(1, sizeof(token_t));
			int size = __tokenize_operator(token, tmp,3);

			if(token != NULL && STRING_VALID(token->str))
			{
				*token_list = g_list_append(*token_list, token);
				tmp += size;
				idx = -1;
			}
			else
			{
				SAFE_FREE(token);
				media_content_error("tokenize error occured");
				return -1;
			}
		}
	}

	if(*tmp)			//remained string
	{
		token_t *token = (token_t*)calloc(1, sizeof(token_t));
		if(token == NULL)
		{
			media_content_error("OUT_OF_MEMORY(0x%08x)", MEDIA_CONTENT_ERROR_OUT_OF_MEMORY);
			return -1;
		}

		ret = __tokenize_string(token, tmp, idx);
		if (ret < 0)
		{
			SAFE_FREE(token);
			media_content_error("tokenize error occured");
			return -1;
		}

		if(token != NULL && STRING_VALID(token->str))
		{
			*token_list = g_list_append(*token_list, token);
		}
		else
		{
			SAFE_FREE(token);
			media_content_error("tokenize error occured");
			return -1;
		}
	}

	return MEDIA_CONTENT_ERROR_NONE;
}

static int __tokenize(GList **token_list, const char *str)
{
	int ret = 0;
	int idx = 0;

	if(!STRING_VALID(str)) {
		media_content_error("Parameter string in invalid");
		return MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
	}

	const char *tmp = str;
	const char *dst_ptr = str + strlen(str);

	for (idx = 0; (*(tmp+idx)) && (tmp < dst_ptr); idx++)
	{
		//media_content_debug("[%d] '%c'", idx, tmp[idx]);
		if(tmp[idx] == media_token[0][0])		//" "
		{
			if(idx == 0)		// ignore the space.
			{
				tmp++;
				idx = -1;
				continue;
			}

			token_t *token = (token_t*)calloc(1, sizeof(token_t));
			if(token == NULL)
			{
				media_content_error("OUT_OF_MEMORY(0x%08x)", MEDIA_CONTENT_ERROR_OUT_OF_MEMORY);
				return -1;
			}
			token->type = UNKNOWN_TYPE;
			token->str = (char*)calloc(idx+1, sizeof(char));
			if(token->str == NULL)
			{
				SAFE_FREE(token);
				media_content_error("OUT_OF_MEMORY(0x%08x)", MEDIA_CONTENT_ERROR_OUT_OF_MEMORY);
				return -1;
			}
			strncpy(token->str, tmp, idx);
			//media_content_debug("type : [%d] str : [%s]", token->type, token->str);
			*token_list = g_list_append(*token_list, token);
			tmp = tmp +idx + strlen(media_token[0]);
			idx = -1;
		}
		else if(tmp[idx] == media_token[1][0])	// " \" "
		{
			int j;
			bool flag = false;
			for(j = idx+1; tmp[j]; j++)	//find next quotation
			{
				if(tmp[j] == media_token[1][0] && tmp[j+1] == media_token[1][0])
				{
					j += 1;
					continue;
				}
				if(tmp[j] == media_token[1][0])
				{
					token_t *token = (token_t*)calloc(1, sizeof(token_t));
					if(token == NULL)
					{
						media_content_error("OUT_OF_MEMORY(0x%08x)", MEDIA_CONTENT_ERROR_OUT_OF_MEMORY);
						return -1;
					}
					token->str = (char*) calloc(j+1+1, sizeof(char));
					if(token->str == NULL)
					{
						SAFE_FREE(token);
						media_content_error("OUT_OF_MEMORY(0x%08x)", MEDIA_CONTENT_ERROR_OUT_OF_MEMORY);
						return -1;
					}
					token->type = STRING_TYPE;
					strncpy(token->str, tmp, j+1);
					//media_content_debug("type : [%d] str : [%s], j : %d", token->type, token->str, j);
					*token_list = g_list_append(*token_list, token);
					tmp = tmp + strlen(token->str);
					idx = -1;
					flag = true;
					break;
				}
			}

			if(!flag && *tmp != '\0' && tmp[j]=='\0')
			{
				token_t *token = (token_t*)calloc(1, sizeof(token_t));
				if(token == NULL)
				{
					media_content_error("OUT_OF_MEMORY(0x%08x)", MEDIA_CONTENT_ERROR_OUT_OF_MEMORY);
					return -1;
				}
				token->str = (char*) calloc(j+1,sizeof(char));
				if(token->str == NULL)
				{
					SAFE_FREE(token);
					media_content_error("OUT_OF_MEMORY(0x%08x)", MEDIA_CONTENT_ERROR_OUT_OF_MEMORY);
					return -1;
				}
				token->type = UNKNOWN_TYPE;
				strncpy(token->str, tmp,j);
				//media_content_debug("type : [%d] str : [%s]", token->type, token->str);
				*token_list = g_list_append(*token_list, token);
				tmp = tmp +strlen(token->str);
				idx = -1;
			}
		}
		else if(tmp[idx] == media_token[2][0])	// " \' "
		{
			int j;
			bool flag = false;
			for(j = idx+1; tmp[j]; j++)
			{
				if(tmp[j] == media_token[2][0] && tmp[j+1] == media_token[2][0])
				{
					j += 1;
					continue;
				}
				if(tmp[j] == media_token[2][0])
				{
					token_t *token = (token_t*)calloc(1, sizeof(token_t));
					if(token == NULL)
					{
						media_content_error("OUT_OF_MEMORY(0x%08x)", MEDIA_CONTENT_ERROR_OUT_OF_MEMORY);
						return -1;
					}
					token->str = (char*) calloc(j+1+1, sizeof(char));
					if(token->str == NULL)
					{
						SAFE_FREE(token);
						media_content_error("OUT_OF_MEMORY(0x%08x)", MEDIA_CONTENT_ERROR_OUT_OF_MEMORY);
						return -1;
					}
					token->type = STRING_TYPE;
					strncpy(token->str, tmp, j+1);
					//media_content_debug("type : [%d] str : [%s]", token->type, token->str);
					*token_list = g_list_append(*token_list, token);
					tmp = tmp + strlen(token->str);
					idx = -1;
					flag = true;
					break;
				}
			}

			if(!flag && *tmp != '\0' && tmp[j]=='\0')
			{
				token_t *token = (token_t*)calloc(1, sizeof(token_t));
				if(token == NULL)
				{
					media_content_error("OUT_OF_MEMORY(0x%08x)", MEDIA_CONTENT_ERROR_OUT_OF_MEMORY);
					return -1;
				}
				token->str = (char*) calloc(j+1,sizeof(char));
				if(token->str == NULL)
				{
					SAFE_FREE(token);
					media_content_error("OUT_OF_MEMORY(0x%08x)", MEDIA_CONTENT_ERROR_OUT_OF_MEMORY);
					return -1;
				}
				token->type = UNKNOWN_TYPE;
				strncpy(token->str, tmp,j);
				//media_content_debug("type : [%d] str : [%s]", token->type, token->str);
				*token_list = g_list_append(*token_list, token);
				tmp = tmp + strlen(token->str);
				idx = -1;
			}
		}
		else if(tmp[idx] == media_token[3][0])	//"("
		{
			if(idx != 0)
			{
				token_t *token = (token_t*)calloc(1, sizeof(token_t));
				if(token == NULL)
				{
					media_content_error("OUT_OF_MEMORY(0x%08x)", MEDIA_CONTENT_ERROR_OUT_OF_MEMORY);
					return -1;
				}
				ret = __tokenize_string(token, tmp, idx);
				if (ret < 0)
				{
					SAFE_FREE(token);
					media_content_error("tokenize error occured");
					return -1;
				}
				else
				{
					*token_list = g_list_append(*token_list, token);
					tmp = tmp + idx;
				}
			}
			token_t *token = (token_t*)calloc(1, sizeof(token_t));
			int size = __tokenize_operator(token, tmp,3);

			if(token != NULL && STRING_VALID(token->str))
			{
				*token_list = g_list_append(*token_list, token);
				tmp += size;
				idx = -1;
			}
			else
			{
				SAFE_FREE(token);
				media_content_error("tokenize error occured");
				return -1;
			}

		}
		else if(tmp[idx] == media_token[4][0])	//")"
		{
			if(idx != 0)
			{
				token_t *token = (token_t*)calloc(1, sizeof(token_t));
				if(token == NULL)
				{
					media_content_error("OUT_OF_MEMORY(0x%08x)", MEDIA_CONTENT_ERROR_OUT_OF_MEMORY);
					return -1;
				}

				ret = __tokenize_string(token, tmp, idx);
				if (ret < 0)
				{
					SAFE_FREE(token);
					media_content_error("tokenize error occured");
					return -1;
				}
				else
				{
					*token_list = g_list_append(*token_list, token);
					tmp = tmp + idx;
				}
			}
			token_t *token = (token_t*)calloc(1, sizeof(token_t));
			int size = __tokenize_operator(token, tmp,4);

			if(token != NULL && STRING_VALID(token->str))
			{
				*token_list = g_list_append(*token_list, token);
				tmp += size;
				idx = -1;
			}
			else
			{
				SAFE_FREE(token);
				media_content_error("tokenize error occured");
				return -1;
			}

		}
		else if(tmp[idx] == media_token[5][0])	//"="
		{
			if(idx != 0)
			{
				token_t *token = (token_t*)calloc(1, sizeof(token_t));
				if(token == NULL)
				{
					media_content_error("OUT_OF_MEMORY(0x%08x)", MEDIA_CONTENT_ERROR_OUT_OF_MEMORY);
					return -1;
				}

				ret = __tokenize_string(token, tmp, idx);
				if (ret < 0)
				{
					SAFE_FREE(token);
					media_content_error("tokenize error occured");
					return -1;
				}
				else
				{
					*token_list = g_list_append(*token_list, token);
					tmp = tmp + idx;
				}
			}
			token_t *token = (token_t*)calloc(1, sizeof(token_t));
			int size = __tokenize_operator(token, tmp,5);

			if(token != NULL && STRING_VALID(token->str))
			{
				*token_list = g_list_append(*token_list, token);
				tmp += size;
				idx = -1;
			}
			else
			{
				SAFE_FREE(token);
				media_content_error("tokenize error occured");
				return -1;
			}

		}
		else if(tmp[idx] == media_token[6][0] && tmp[idx+1] == media_token[6][1])	//"<=",
		{
			if(idx != 0)
			{
				token_t *token = (token_t*)calloc(1, sizeof(token_t));
				if(token == NULL)
				{
					media_content_error("OUT_OF_MEMORY(0x%08x)", MEDIA_CONTENT_ERROR_OUT_OF_MEMORY);
					return -1;
				}

				ret = __tokenize_string(token, tmp, idx);
				if (ret < 0)
				{
					SAFE_FREE(token);
					media_content_error("tokenize error occured");
					return -1;
				}
				else
				{
					*token_list = g_list_append(*token_list, token);
					tmp = tmp + idx;
				}
			}
			token_t *token = (token_t*)calloc(1, sizeof(token_t));
			int size = __tokenize_operator(token, tmp,6);

			if(token != NULL && STRING_VALID(token->str))
			{
				*token_list = g_list_append(*token_list, token);
				tmp += size;
				idx = -1;
			}
			else
			{
				SAFE_FREE(token);
				media_content_error("tokenize error occured");
				return -1;
			}

		}
		else if(tmp[idx] == media_token[7][0])	//"<",
		{
			if(idx != 0)
			{
				token_t *token = (token_t*)calloc(1, sizeof(token_t));
				if(token == NULL)
				{
					media_content_error("OUT_OF_MEMORY(0x%08x)", MEDIA_CONTENT_ERROR_OUT_OF_MEMORY);
					return -1;
				}

				ret = __tokenize_string(token, tmp, idx);
				if (ret < 0)
				{
					SAFE_FREE(token);
					media_content_error("tokenize error occured");
					return -1;
				}
				else
				{
					*token_list = g_list_append(*token_list, token);
					tmp = tmp + idx;
				}
			}
			token_t *token = (token_t*)calloc(1, sizeof(token_t));
			int size = __tokenize_operator(token, tmp,7);

			if(token != NULL && STRING_VALID(token->str))
			{
				*token_list = g_list_append(*token_list, token);
				tmp += size;
				idx = -1;
			}
			else
			{
				SAFE_FREE(token);
				media_content_error("tokenize error occured");
				return -1;
			}

		}
		else if(tmp[idx] == media_token[8][0] && tmp[idx+1] == media_token[8][1])	//">=",
		{
			if(idx != 0)
			{
				token_t *token = (token_t*)calloc(1, sizeof(token_t));
				if(token == NULL)
				{
					media_content_error("OUT_OF_MEMORY(0x%08x)", MEDIA_CONTENT_ERROR_OUT_OF_MEMORY);
					return -1;
				}

				ret = __tokenize_string(token, tmp, idx);
				if (ret < 0)
				{
					SAFE_FREE(token);
					media_content_error("tokenize error occured");
					return -1;
				}
				else
				{
					*token_list = g_list_append(*token_list, token);
					tmp = tmp + idx;
				}
			}
			token_t *token = (token_t*)calloc(1, sizeof(token_t));
			int size = __tokenize_operator(token, tmp,8);

			if(token != NULL && STRING_VALID(token->str))
			{
				*token_list = g_list_append(*token_list, token);
				tmp += size;
				idx = -1;
			}
			else
			{
				SAFE_FREE(token);
				media_content_error("tokenize error occured");
				return -1;
			}

		}
		else if(tmp[idx] == media_token[9][0])	//">",
		{
			if(idx != 0)
			{
				token_t *token = (token_t*)calloc(1, sizeof(token_t));
				if(token == NULL)
				{
					media_content_error("OUT_OF_MEMORY(0x%08x)", MEDIA_CONTENT_ERROR_OUT_OF_MEMORY);
					return -1;
				}

				ret = __tokenize_string(token, tmp, idx);
				if (ret < 0)
				{
					SAFE_FREE(token);
					media_content_error("tokenize error occured");
					return -1;
				}
				else
				{
					*token_list = g_list_append(*token_list, token);
					tmp = tmp + idx;
				}
			}
			token_t *token = (token_t*)calloc(1, sizeof(token_t));
			int size = __tokenize_operator(token, tmp,9);

			if(token != NULL && STRING_VALID(token->str))
			{
				*token_list = g_list_append(*token_list, token);
				tmp += size;
				idx = -1;
			}
			else
			{
				SAFE_FREE(token);
				media_content_error("tokenize error occured");
				return -1;
			}

		}
	}

	if(*tmp)			//remained string
	{
		token_t *token = (token_t*)calloc(1, sizeof(token_t));
		if(token == NULL)
		{
			media_content_error("OUT_OF_MEMORY(0x%08x)", MEDIA_CONTENT_ERROR_OUT_OF_MEMORY);
			return -1;
		}

		ret = __tokenize_string(token, tmp, idx);
		if (ret < 0)
		{
			SAFE_FREE(token);
			media_content_error("tokenize error occured");
			return -1;
		}

		if(token != NULL && STRING_VALID(token->str))
		{
			*token_list = g_list_append(*token_list, token);
		}
		else
		{
			SAFE_FREE(token);
			media_content_error("tokenize error occured");
			return -1;
		}
	}

	return MEDIA_CONTENT_ERROR_NONE;
}

int _media_filter_attribute_create(attribute_h *attr)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;

	if(attr == NULL)
	{
		media_content_error("INVALID_PARAMETER(0x%08x)", MEDIA_CONTENT_ERROR_INVALID_PARAMETER);
		return MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
	}

	attribute_s *_attr = (attribute_s*)calloc(1, sizeof(attribute_s));

	if(_attr == NULL)
	{
		media_content_error("OUT_OF_MEMORY(0x%08x)", MEDIA_CONTENT_ERROR_OUT_OF_MEMORY);
		ret = MEDIA_CONTENT_ERROR_OUT_OF_MEMORY;
	}
	else
	{
		_attr->attr_map = g_hash_table_new (g_str_hash, g_str_equal);
		*attr = (attribute_h)_attr;
	}

	return ret;
}

int _media_filter_attribute_add(attribute_h attr, char *user_attr, char *platform_attr)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	char *_user = NULL;
	char *_platform = NULL;
	attribute_s *_attr = (attribute_s*)attr;

	if(_attr != NULL)
	{
		if(STRING_VALID(user_attr) && STRING_VALID(platform_attr))
		{
			_user = strdup(user_attr);
			if(_user == NULL)
			{
				media_content_error("OUT_OF_MEMORY(0x%08x)", MEDIA_CONTENT_ERROR_OUT_OF_MEMORY);
				return MEDIA_CONTENT_ERROR_OUT_OF_MEMORY;
			}

			_platform = strdup(platform_attr);
			if(_platform == NULL)
			{
				SAFE_FREE(_user);
				media_content_error("OUT_OF_MEMORY(0x%08x)", MEDIA_CONTENT_ERROR_OUT_OF_MEMORY);
				return MEDIA_CONTENT_ERROR_OUT_OF_MEMORY;
			}

			g_hash_table_insert (_attr->attr_map, _user, _platform);
		}
		else
		{
			media_content_error("INVALID_PARAMETER(0x%08x)", MEDIA_CONTENT_ERROR_INVALID_PARAMETER);
			return MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
		}
	}
	else
	{
		media_content_error("INVALID_PARAMETER(0x%08x)", MEDIA_CONTENT_ERROR_INVALID_PARAMETER);
		return MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
	}

	return ret;
}

int _media_filter_attribute_destory(attribute_h attr)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	attribute_s *_attr = (attribute_s*)attr;

	if(_attr != NULL)
	{
		if(_attr->attr_map != NULL)
		{
			g_hash_table_foreach(_attr->attr_map, __filter_attribute_free_value, NULL);
			g_hash_table_destroy(_attr->attr_map);
		}

		SAFE_FREE(_attr);
	}
	else
	{
		media_content_error("INVALID_PARAMETER(0x%08x)", MEDIA_CONTENT_ERROR_INVALID_PARAMETER);
		return MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
	}

	return ret;
}

int _media_filter_attribute_generate(attribute_h attr, char *condition, media_content_collation_e collate_type, char **generated_condition)
{
	int idx = 0;
	GList *token_list = NULL;
	int size = 0;
	int ret = MEDIA_CONTENT_ERROR_NONE;
	int total_str_size = 0;
	token_t *token;

	if((condition == NULL) || (generated_condition == NULL))
	{
		media_content_error("INVALID_PARAMETER(0x%08x):Invalid the condition", MEDIA_CONTENT_ERROR_INVALID_PARAMETER);
		return MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
	}

	if(attr != NULL)
	{
		if(__tokenize(&token_list, condition) < 0)
		{
			media_content_error("INVALID_PARAMETER(0x%08x):Invalid the condition", MEDIA_CONTENT_ERROR_INVALID_PARAMETER);
			return MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
		}

		for(idx = 0; idx < g_list_length(token_list); idx++)
		{
			token = (token_t*)g_list_nth_data(token_list, idx);

			if(token->type == UNKNOWN_TYPE)
			{
				char *replace_str = __media_filter_replace_attr(attr, token->str);
				if(STRING_VALID(replace_str))
				{
					SAFE_FREE(token->str);
					token->str = replace_str;
				}
			}

			total_str_size += strlen(token->str)+1;
			//media_content_debug("[%d][type:%d]:%s", idx, token->type, token->str);
		}

		//make the statment
		size = total_str_size + COLLATE_STR_SIZE + 1;
		* generated_condition = (char*)calloc(size, sizeof(char));

		for(idx = 0; idx < g_list_length(token_list); idx++)
		{
			token = (token_t*)g_list_nth_data(token_list, idx);

			if((token != NULL) && STRING_VALID(token->str))
			{
				SAFE_STRLCAT(*generated_condition, token->str, size);
				SAFE_STRLCAT(*generated_condition, SPACE, size);

				SAFE_FREE(token->str);
				SAFE_FREE(token);
			}
		}

		if(collate_type == MEDIA_CONTENT_COLLATE_NOCASE || collate_type == MEDIA_CONTENT_COLLATE_RTRIM ||collate_type == MEDIA_CONTENT_COLLATE_LOCALIZED) {
			SAFE_STRLCAT(*generated_condition, "COLLATE ", size);
			SAFE_STRLCAT(*generated_condition, __get_collate_str(collate_type), size);
			SAFE_STRLCAT(*generated_condition, SPACE, size);
		}

		//media_content_debug("statement : %s(%d) (total:%d)", *generated_condition, strlen(*generated_condition), total_str_size);
		media_content_sec_debug("Condition : %s", *generated_condition);

		//if(*generated_condition != NULL)
		//	res = 1;

		if(token_list != NULL)
			g_list_free(token_list);
	}
	else
	{
		media_content_error("DB field mapping table doesn't exist. Check db connection", MEDIA_CONTENT_ERROR_DB_FAILED);
		return MEDIA_CONTENT_ERROR_DB_FAILED;
	}

	return ret;
}

int _media_filter_attribute_option_generate(attribute_h attr, filter_h filter, char **generated_option)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	filter_s *_filter = NULL;
	char option_query[DEFAULT_QUERY_SIZE] = {0, };
	char condition[DEFAULT_QUERY_SIZE] = {0, };
	int size = 0;
	//bool order_by = true;

	if(filter == NULL)
	{
		media_content_error("INVALID_PARAMETER(0x%08x)", MEDIA_CONTENT_ERROR_INVALID_PARAMETER);
		return MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
	}

	if(attr == NULL)
	{
		media_content_error("DB field mapping table doesn't exist. Check db connection", MEDIA_CONTENT_ERROR_DB_FAILED);
		return MEDIA_CONTENT_ERROR_DB_FAILED;
	}

	_filter = (filter_s*)filter;

	memset(option_query, 0x00, sizeof(option_query));

	/* Order by*/
	if(STRING_VALID(_filter->order_keyword) && ((_filter->order_type == MEDIA_CONTENT_ORDER_ASC) ||(_filter->order_type == MEDIA_CONTENT_ORDER_DESC)))
	{
		int idx = 0;
		int total_str_size = 0;
		GList *token_list = NULL;
		token_t *token;
		char *attr_str;

		if(__tokenize_attribute(&token_list, _filter->order_keyword) < 0)
		{
			media_content_error("INVALID_PARAMETER(0x%08x):Invalid the condition", MEDIA_CONTENT_ERROR_INVALID_PARAMETER);
			return MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
		}

		for(idx = 0; idx < g_list_length(token_list); idx++)
		{
			token = (token_t*)g_list_nth_data(token_list, idx);

			if(token->type == UNKNOWN_TYPE)
			{
				char *replace_str = __media_filter_replace_attr(attr, token->str);
				if(STRING_VALID(replace_str))
				{
					attr_str = (char*)calloc(strlen(replace_str) + COLLATE_STR_SIZE + 1, sizeof(char));
					if(attr_str == NULL)
					{
						media_content_error("OUT_OF_MEMORY(0x%08x)", MEDIA_CONTENT_ERROR_OUT_OF_MEMORY);
						SAFE_FREE(replace_str);
						continue;
					}

					if(_filter->order_collate_type == MEDIA_CONTENT_COLLATE_NOCASE || _filter->order_collate_type == MEDIA_CONTENT_COLLATE_RTRIM ||_filter->order_collate_type == MEDIA_CONTENT_COLLATE_LOCALIZED) {
						snprintf(attr_str, strlen(replace_str) + COLLATE_STR_SIZE + 1, "%s COLLATE %s %s", replace_str, __get_collate_str(_filter->order_collate_type), __get_order_str(_filter->order_type));
					} else {
						snprintf(attr_str, strlen(replace_str) + COLLATE_STR_SIZE + 1, "%s %s", replace_str, __get_order_str(_filter->order_type));
					}

					SAFE_FREE(token->str);
					token->str = attr_str;
					SAFE_FREE(replace_str);
				}
				else
				{
					media_content_error("There is no matched db field for %s", token->str);
				}
			}

			total_str_size += strlen(token->str) + 1;
			//media_content_debug("[%d][type:%d]:%s", idx, token->type, token->str);
		}

		//make the statment
		char *generated_condition = NULL;
		size = total_str_size + COLLATE_STR_SIZE + 1;
		generated_condition = (char*)calloc(size, sizeof(char));

		for(idx = 0; idx < g_list_length(token_list); idx++)
		{
			token = (token_t*)g_list_nth_data(token_list, idx);

			if((token != NULL) && STRING_VALID(token->str))
			{
				//media_content_debug("[%d] %s", idx, token->str);
				SAFE_STRLCAT(generated_condition, token->str, size);
				SAFE_STRLCAT(generated_condition, SPACE, size);

				SAFE_FREE(token->str);
				SAFE_FREE(token);
			}
		}

		snprintf(condition, sizeof(condition), "ORDER BY %s", generated_condition);
		SAFE_STRLCAT(option_query, condition, sizeof(option_query));

		if(token_list != NULL)
			g_list_free(token_list);

		SAFE_FREE(generated_condition);
	}

	/* offset */
	SAFE_STRLCAT(option_query, SPACE, sizeof(option_query));
	snprintf(condition, sizeof(condition), "LIMIT %d, %d", _filter->offset, _filter->count);
	SAFE_STRLCAT(option_query, condition, sizeof(option_query));

	if(STRING_VALID(option_query))
	{
		*generated_option = strdup(option_query);
	}
	else
	{
		*generated_option = NULL;
	}

	return ret;
}

int media_filter_create(filter_h *filter)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;

	if(filter == NULL)
	{
		media_content_error("INVALID_PARAMETER(0x%08x)", MEDIA_CONTENT_ERROR_INVALID_PARAMETER);
		return MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
	}

	filter_s *_filter = (filter_s*)calloc(1, sizeof(filter_s));

	if(_filter == NULL)
	{
		media_content_error("OUT_OF_MEMORY(0x%08x)", MEDIA_CONTENT_ERROR_OUT_OF_MEMORY);
		ret = MEDIA_CONTENT_ERROR_OUT_OF_MEMORY;
	}
	else
	{
		_filter->condition = NULL;
		_filter->order_keyword = NULL;
		_filter->order_type = -1;
		_filter->condition_collate_type = MEDIA_CONTENT_COLLATE_DEFAULT;
		_filter->order_collate_type = MEDIA_CONTENT_COLLATE_DEFAULT;
		_filter->offset = -1;
		_filter->count = -1;

		*filter = (filter_h)_filter;
	}

	return ret;
}

int media_filter_destroy(filter_h filter)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	filter_s *_filter = (filter_s*)filter;

	if(_filter)
	{
		SAFE_FREE(_filter->condition);
		SAFE_FREE(_filter->order_keyword);
		SAFE_FREE(_filter);

		ret = MEDIA_CONTENT_ERROR_NONE;
	}
	else
	{
		media_content_error("INVALID_PARAMETER(0x%08x)", MEDIA_CONTENT_ERROR_INVALID_PARAMETER);
		ret = MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
	}

	return ret;
}

int media_filter_set_offset(filter_h filter, int offset, int count)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	filter_s *_filter = (filter_s*)filter;

	if(_filter != NULL)
	{
		_filter->offset = offset;
		_filter->count = count;
	}
	else
	{
		media_content_error("INVALID_PARAMETER(0x%08x)", MEDIA_CONTENT_ERROR_INVALID_PARAMETER);
		ret = MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
	}

	return ret;
}

int media_filter_set_condition(filter_h filter, const char *condition, media_content_collation_e collate_type)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	filter_s *_filter = (filter_s*)filter;

	if((_filter != NULL) && STRING_VALID(condition)
		&& ((collate_type >= MEDIA_CONTENT_COLLATE_DEFAULT) && (collate_type <= MEDIA_CONTENT_COLLATE_LOCALIZED)))
	{
		if(STRING_VALID(_filter->condition))
		{
			SAFE_FREE(_filter->condition);
		}

		_filter->condition = strdup(condition);
		if(_filter->condition == NULL)
		{
			media_content_error("OUT_OF_MEMORY(0x%08x)", MEDIA_CONTENT_ERROR_OUT_OF_MEMORY);
			return MEDIA_CONTENT_ERROR_OUT_OF_MEMORY;
		}

		media_content_sec_debug("Condition string : %s", _filter->condition);

		_filter->condition_collate_type = collate_type;
	}
	else
	{
		media_content_error("INVALID_PARAMETER(0x%08x)", MEDIA_CONTENT_ERROR_INVALID_PARAMETER);
		ret = MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
	}

	return ret;
}

int media_filter_set_order(filter_h filter, media_content_order_e order_type, const char *order_keyword, media_content_collation_e collate_type)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	filter_s *_filter = (filter_s*)filter;

	if((_filter != NULL) && STRING_VALID(order_keyword)
		&& ((order_type == MEDIA_CONTENT_ORDER_ASC) ||(order_type == MEDIA_CONTENT_ORDER_DESC))
		&& ((collate_type >= MEDIA_CONTENT_COLLATE_DEFAULT) && (collate_type <= MEDIA_CONTENT_COLLATE_LOCALIZED)))
	{
		SAFE_FREE(_filter->order_keyword);

		if(STRING_VALID(order_keyword))
		{
			_filter->order_keyword = strdup(order_keyword);

			if(_filter->order_keyword == NULL)
			{
				media_content_error("OUT_OF_MEMORY(0x%08x)", MEDIA_CONTENT_ERROR_OUT_OF_MEMORY);
				return MEDIA_CONTENT_ERROR_OUT_OF_MEMORY;
			}
			_filter->order_type = order_type;
			_filter->order_collate_type = collate_type;
		}
		else
		{
			media_content_error("INVALID_PARAMETER(0x%08x)", MEDIA_CONTENT_ERROR_INVALID_PARAMETER);
			ret = MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
		}
	}
	else
	{
		media_content_error("INVALID_PARAMETER(0x%08x)", MEDIA_CONTENT_ERROR_INVALID_PARAMETER);
		ret = MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
	}

	return ret;
}

int media_filter_get_offset(filter_h filter, int *offset, int *count)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	filter_s *_filter = (filter_s*)filter;

	if(_filter)
	{
		*offset = _filter->offset;
		*count = _filter->count;
	}
	else
	{
		media_content_error("INVALID_PARAMETER(0x%08x)", MEDIA_CONTENT_ERROR_INVALID_PARAMETER);
		ret = MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
	}

	return ret;
}

int media_filter_get_condition(filter_h filter, char **condition, media_content_collation_e *collate_type)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	filter_s *_filter = (filter_s*)filter;

	if(_filter)
	{
		if(STRING_VALID(_filter->condition))
		{
			*condition = strdup(_filter->condition);
			if(*condition == NULL)
			{
				media_content_error("OUT_OF_MEMORY(0x%08x)", MEDIA_CONTENT_ERROR_OUT_OF_MEMORY);
				return MEDIA_CONTENT_ERROR_OUT_OF_MEMORY;
			}
		}
		else
		{
			*condition = NULL;
		}

		*collate_type = _filter->condition_collate_type;
	}
	else
	{
		media_content_error("INVALID_PARAMETER(0x%08x)", MEDIA_CONTENT_ERROR_INVALID_PARAMETER);
		ret = MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
	}

	return ret;
}

int media_filter_get_order(filter_h filter, media_content_order_e* order_type, char **order_keyword, media_content_collation_e *collate_type)
{
	int ret = MEDIA_CONTENT_ERROR_NONE;
	filter_s *_filter = (filter_s*)filter;

	if(_filter)
	{
		if(STRING_VALID(_filter->order_keyword))
		{
			*order_keyword = strdup(_filter->order_keyword);
			if(*order_keyword == NULL)
			{
				media_content_error("OUT_OF_MEMORY(0x%08x)", MEDIA_CONTENT_ERROR_OUT_OF_MEMORY);
				return MEDIA_CONTENT_ERROR_OUT_OF_MEMORY;
			}
		}
		else
		{
			*order_keyword = NULL;
		}

		*order_type = _filter->order_type;
		*collate_type = _filter->order_collate_type;
	}
	else
	{
		media_content_error("INVALID_PARAMETER(0x%08x)", MEDIA_CONTENT_ERROR_INVALID_PARAMETER);
		ret = MEDIA_CONTENT_ERROR_INVALID_PARAMETER;
	}

	return ret;
}
