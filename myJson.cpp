#include"myJson.h"
#include<stdlib.h>
#include<string.h>
#include<iostream>


static const char* skip(const char* data) {
	while (data && *data && (unsigned char)*data <= 32) data++;
	return data;
}

static jchar* myJSON_newStr(const jchar* str) {
	jchar* tmpStr = (jchar*)malloc((strlen(str) + 1) * sizeof(jchar));
	if (tmpStr == nullptr) return nullptr;
	memcpy(tmpStr, str, strlen(str) + 1);
	return tmpStr;
}


myJSON* myJSON_NewItem()
{
	myJSON* item = (myJSON*)malloc(sizeof(myJSON));
	memset(item, 0, sizeof(myJSON));
	return item;
}

//create
myJSON* myJSON_CreateObject()
{
	myJSON* obj = myJSON_NewItem();
	obj->_jsonType = JSONTYPE::_OBJECT;
	return obj;
}

myJSON* myJSON_CreateArray() {
	myJSON* tmp = (myJSON*)malloc(sizeof(myJSON));
	return tmp;
}

myJSON* myJSON_CreateString(const jchar* str) {
	myJSON* obj = myJSON_NewItem();
	obj->_stringValue = myJSON_newStr(str);
	obj->_jsonType = JSONTYPE::_STRING;
	return obj;
}

myJSON* myJSON_CreateNumber(const double num) {
	myJSON* obj = myJSON_NewItem();
	obj->_doubleValue = num;
	obj->_intValue = (int)num;
	obj->_jsonType = JSONTYPE::_NUMBER;
	return obj;
}


//add
status myJSON_AddItemToObject(myJSON* obj, const jchar* name, myJSON* item){
	item->_name = myJSON_newStr(name);
	//printf("%s\n", item->_name);
	if (obj->_child == nullptr) {
		obj->_child = item;
		obj->_child_back = item;
		item->_father = obj;
		return SUCCESS;
	}
	obj->_child_back->_next = item;
	item->_prev = obj->_child_back;
	obj->_child_back = item;
	item->_father = obj;
	return SUCCESS;

}

status myJSON_AddItemToArray(myJSON* array, myJSON* item){
	return SUCCESS;
}

static void printTAB(jchar* data, int count)
{
	const jchar* space = "\t\0";
	while (count != 0) {
		strncat(data, space, strlen(space) + 1);
		count--;
	}
}
//print
static void myError(const char* data) {
	printf("%s	ERROR!", data);
	exit(1);
}


static void printRecursion(jchar *data, myJSON* obj, bool flag = true, unsigned short count = 0) {
	printTAB(data, count);
	if (obj->_jsonType == JSONTYPE::_OBJECT) {
		//如果是object型，就向data里添加name信息和一定的格式 
		//if obj's type is OBJECT then stuff name and form information which use to attribute obj. 
		jchar* name = obj->_name;
		if (name) {
			strncat(data, "\"", strlen("\""));
			strncat(data, name, strlen(name) + 1);
			strncat(data, "\"", strlen("\""));
		}
		jchar* tmpStr = (jchar*) malloc (strlen("{\n") * sizeof(jchar));
		if(flag)
			strncpy(tmpStr, "{\n", strlen("{\n"));
		else
			strncpy(tmpStr, "{", strlen("{"));
		strncat(data, tmpStr, strlen(tmpStr) + 1);
		myJSON* iter = obj->_child;
		while (iter != nullptr) {
			if(flag)
				printRecursion(data, iter, flag, count + 1);
			else
				printRecursion(data, iter, flag, count);
			iter = iter->_next;
		}
		if(flag)
			printTAB(data, count);
		jchar* tmpStr2 = (jchar*)malloc(strlen("}\n") * sizeof(jchar));
		if (flag)
			strncpy(tmpStr2, "}\n", strlen("}\n"));
		else
			strncpy(tmpStr2, "}", strlen("}"));
		strncat(data, tmpStr2, strlen(tmpStr2) + 1);
		return;
	}
	jchar buffer[_MYJSON_DATA_SIZE * 2];
	switch (obj->_jsonType) {
	case JSONTYPE::_STRING:
		if (obj->_next) {
			if(flag)
				sprintf(buffer, "\"%s\":\"%s\",\n", obj->_name, obj->_stringValue);
			else
				sprintf(buffer, "\"%s\":\"%s\",", obj->_name, obj->_stringValue);
		}
		else {
			if(flag)
				sprintf(buffer, "\"%s\":\"%s\",\n", obj->_name, obj->_stringValue);
			else
				sprintf(buffer, "\"%s\":\"%s\",", obj->_name, obj->_stringValue);
		}
		strncat(data, buffer, strlen(buffer) + 1);
		break;
	case JSONTYPE::_NUMBER:
		jchar tmpBuffer[10];
		if (obj->_next) {
			if(flag)
				sprintf(buffer, "\"%s\":%s,\n", obj->_name, itoa(obj->_intValue, tmpBuffer, strlen(tmpBuffer) + 1));
			else
				sprintf(buffer, "\"%s\":%s,", obj->_name, itoa(obj->_intValue, tmpBuffer, strlen(tmpBuffer) + 1));
		}
		else {
			if(flag)
				sprintf(buffer, "\"%s\":%s,\n", obj->_name, itoa(obj->_intValue, tmpBuffer, strlen(tmpBuffer) + 1));
			else
				sprintf(buffer, "\"%s\":%s,", obj->_name, itoa(obj->_intValue, tmpBuffer, strlen(tmpBuffer) + 1));
		}
		strncat(data, buffer, strlen(buffer) + 1);
		break;

	default:
		break;

	}
}



const jchar* myJSON_print(myJSON* obj) {
	//初始化data，调用递归函数
	//initial data and call recursion function for stuff the data string
	if (obj == nullptr) {
		return "0	";
	}
	jchar* data = (jchar*)malloc(_MYJSON_DATA_SIZE * sizeof(jchar));
	if (data == NULL)
		throw std::bad_alloc();
	memset(data, '\0', _MYJSON_DATA_SIZE * sizeof(jchar));
	printRecursion(data, obj);
	return data;
}

const jchar* myJSON_printUnformted(myJSON* obj){
	//初始化data，调用递归函数
	//initial data and call recursion function for stuff the data string
	if (obj == nullptr) {
		return "0	";
	}
	jchar* data = (jchar*)malloc(_MYJSON_DATA_SIZE * sizeof(jchar));
	if (data == NULL)
		throw std::bad_alloc();
	memset(data, '\0', _MYJSON_DATA_SIZE * sizeof(jchar));
	printRecursion(data, obj, false);
	return data;
}

//delete
status myJSON_delete(myJSON* obj)
{
	//递归删除
	//delete which use recursion for delete the obj self and its children
	
	if (obj->_child_back) {
		myJSON_delete(obj->_child_back);
	}
	if (obj->_prev) {
		obj->_prev->_next = obj->_next;
	}
	else if(obj->_prev == nullptr && obj->_father){
		obj->_father->_child = obj->_next;
	}
	obj->_prev = nullptr;
	free(obj);
	obj = nullptr;
	return status();
}



//parse

static const char* parse_string(myJSON* obj, const jchar* data) {
	if (!data) return 0;
	char* stringBuffer, *ptr2;
	const char* ptr = data;
	int len = 0;
	for (; *ptr != '\"'; ptr++, len++);
	stringBuffer = (char*)malloc((len + 1) * sizeof(char));
	if (!stringBuffer) return 0;
	ptr2 = stringBuffer;
	while (*data != '\"') {
		*ptr2 = *data;
		ptr++, data++;
	}
	data++;
	if(stringBuffer)
	obj->_stringValue = stringBuffer;
	return data;
}
/*
static const char* parse_subObject(myJSON* obj, const char* data) {
	printf("%d\n", father);
	myJSON* obj = myJSON_NewItem();
	if (!obj) return 0;
	//handle key
	if(obj)
	data = skip(data);
	printf("%s\n", data);
	if (!data) myError(data);
	if (*data == '\"') data = parse_string(obj, data + 1);
	printf("%s\n", data);
	obj->_name = obj->_stringValue, obj->_stringValue = nullptr;
	//link father(father) and cild(obj)
	if (!father->_child) father->_child = obj, father->_child_back = obj;
	else father->_child_back->_next = obj, obj->_prev = father->_child_back, father->_child_back = obj;
	obj->_father = father;
	//entry child
	father = obj;
}

static const char* parse_broObject(myJSON* obj, const char* data) {
	myJSON* obj = myJSON_NewItem();
	if (!obj) return 0;
	//handle key
	data = skip(data);
	if (!data) myError(data);
	if (*data == '\"') data = parse_string(obj, data + 1);
	obj->_name = obj->_stringValue, obj->_stringValue = nullptr;
	//handle value
	data = skip(data);
	if (*data != ':') myError(data);
	data = skip(data);
	if (*data != '\"') myError(data);
	data = parse_string(obj, data);
	//link father(father) and cild(obj)
	if (!father->_child) father->_child = obj, father->_child_back = obj;
	else father->_child_back->_next = obj, obj->_prev = father->_child_back, father->_child_back = obj;
	obj->_father = father;
	return data;
}

static const char* myJSON_parseRecursion(myJSON* obj, const jchar* data) {
//与cJSON不同，我缩短了递归链。Dave的cJSON递归调用为parse_object->parse_value->parse_object,myJSON中递归调用只有parRecursion递归调用
	data = skip(data);
	while (*data != '\0' && data) {
		if (*data == '\"') data = parse_string(obj, data + 1); 
		if (*data == '{') data = parse_subObject(obj, data + 1);//obj --> child
		if (*data == ':') data = parse_broObject(obj, data + 1);
		if (*data == ',') data++;//data = parse_broObject(obj, data + 1);
		if (*data == '}') obj = obj->_father;//obj --> father;
		//如果是{则obj改为obj的子节点，如果是}则obj回到上一层，改回为父节点。
		data = skip(data);
	}
	return data;
}


myJSON* myJSON_parse(const jchar* data)
{
	myJSON* obj = myJSON_NewItem();
	myJSON* initialPtr = obj;
	if (!obj) return 0;
	data = myJSON_parseRecursion(obj, data);
	return initialPtr;
}
*/

myJSON* myJSON_search(myJSON* obj, const jchar* key, myJSON* ptr)
{
	if (!obj) return ptr;
	if (ptr) return ptr;
	if (strcmp(obj->_name, key)) ptr = obj;
	myJSON_search(obj->_next, key, ptr);
	myJSON_search(obj->_child, key, ptr);
	return ptr;
}


