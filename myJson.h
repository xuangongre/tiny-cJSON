#pragma once
#include <wchar.h>
#include<string.h>

#ifdef __cplusplus
extern "C" {
#endif
#define SUCCESS 0
#define FAULT -1
#define _MYJSON_NAME_SIZE 128
#define _MYJSON_DATA_SIZE 1024

	struct myHooks {
		//jchar* (*jstrcpy)(jchar*, const jchar*);
		//jchar* (*jstrcat)(jchar*, const jchar*);
	};
	


#ifdef JUNICODE
	typedef wchar_t jchar;
#else
	typedef char jchar;
#endif


	typedef int status;

	enum JSONTYPE { _STRING, _NUMBER, _OBJECT, _ARRAY };

	typedef struct myJson {
		struct myJson* _next, * _prev, * _child_back;
		struct myJson* _child;
		struct myJson* _father;

		enum JSONTYPE _jsonType;
		typedef jchar* jsonString;
		typedef int jsonInt;
		typedef double jsonDouble;
		typedef jchar* jsonName;

		jsonString _stringValue;
		jsonInt _intValue;
		jsonDouble _doubleValue;
		jsonName _name;
	}myJSON;

	//create
	myJSON* myJSON_NewItem();
	myJSON* myJSON_CreateObject();
	myJSON* myJSON_CreateArray();
	myJSON* myJSON_CreateString(const jchar* str);
	myJSON* myJSON_CreateNumber(const double num);
	//add
	status myJSON_AddItemToObject(myJSON* obj, const jchar* name, myJSON* item);
	status myJSON_AddItemToArray(myJSON* array, myJSON* item);
	//print
	const jchar* myJSON_print(myJSON* obj);
	const jchar* myJSON_printUnformted(myJSON* obj);
	//delete
	status myJSON_delete(myJSON* obj);
	//parse
	myJSON* myJSON_parse(const jchar* data);
	//search
	myJSON* myJSON_search(myJSON* obj, const jchar* key, myJSON* ptr = nullptr);
#ifdef __cplusplus
}
#endif


