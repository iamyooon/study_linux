#include <stdio.h>
#include <string.h>

#include <json-c/json.h>

#define FILE_NAME_LEN (64)

#define PRINT_KEY_VALUE(type,obj) \
	switch(type){ \
		case json_type_null:\
			printf("Type is null\n");\
			break;\
		case json_type_boolean:\
			printf("%d\n",json_object_get_boolean(obj));\
			break;\
		case json_type_double:\
			printf("%f\n",json_object_get_double(obj));\
			break;\
		case json_type_int:\
			printf("%d\n",json_object_get_int(obj));\
			break;\
		case json_type_object:\
			printf("%ld\n",json_object_get_int64(obj));\
			break;\
		case json_type_array:\
			printf("%s\n",json_object_get_string(obj));\
			break;\
		case json_type_string:\
			printf("%s\n",json_object_get_string(obj));\
			break;\
	}

void parseJsonObject1(json_object * pInput)
{
	json_object * pJsonObject = NULL;
	int type = 0;

	pJsonObject = json_object_object_get(pInput,"name");
	type = json_object_get_type(pJsonObject);
	printf("[ex1]");
	PRINT_KEY_VALUE(type,pJsonObject);
}

void parseJsonObject2(json_object * pInput)
{
	json_object * pJsonObject = NULL;
	int type = 0;

	json_object_object_foreach(pInput, key_name, value_obj)
	{
		printf("[MCC] %s\n",key_name);
		json_object_object_foreach(value_obj, key_name2, value_obj2)
		{
			printf("  [MNC] %s\n",key_name2);
			type = json_object_get_type(value_obj2);
			PRINT_KEY_VALUE(type,value_obj2);
		}
	}
}

void get_data(json_object * pInput, char* mcc, char* mnc)
{
	json_object *carrier_obj = NULL;
	json_object *apn_obj = NULL;
	json_object *servicegroup_obj = NULL;
	json_object *auto_obj = NULL;
	json_object *protocol_obj = NULL;
	json_object *temp = NULL;

	json_object *mcc_obj= NULL;
	json_object *mnc_obj = NULL;
	int type = 0;
	int i = 0;

	json_object_object_foreach(pInput, mcc_val, value_obj)
	{
		if (strcmp(mcc_val,mcc) != 0)
			continue;

		json_object_object_foreach(value_obj, mnc_val, value_obj2)
		{
			if (strcmp(mnc_val, mnc) != 0)
				continue;

			type = json_object_get_type(value_obj2);
			printf("MCC=%s,MNC=%s,data type=%d\n",mcc_val,mnc_val,type);
			PRINT_KEY_VALUE(type,value_obj2);

			for( i = 0 ; i < json_object_array_length(value_obj2) ; i++)
			{
				temp = json_object_array_get_idx(value_obj2,i);
				carrier_obj = json_object_object_get(temp,"carrier");
				type = json_object_get_type(carrier_obj);
				PRINT_KEY_VALUE(type,carrier_obj);
				apn_obj = json_object_object_get(temp,"apn");
				type = json_object_get_type(apn_obj);
				PRINT_KEY_VALUE(type,apn_obj);
				servicegroup_obj = json_object_object_get(temp,"servicegroup");
				type = json_object_get_type(servicegroup_obj);
				PRINT_KEY_VALUE(type,servicegroup_obj);
				auto_obj = json_object_object_get(temp,"auto");
				type = json_object_get_type(auto_obj);
				PRINT_KEY_VALUE(type,auto_obj);
				protocol_obj = json_object_object_get(temp,"protocol");
				type = json_object_get_type(protocol_obj);
				PRINT_KEY_VALUE(type,protocol_obj);
			}
		}
	}

	mcc_obj = json_object_object_get(pInput,mcc);
	mnc_obj = json_object_object_get(mcc_obj,mnc);
	//type = json_object_get_type(mnc_obj);
	//printf("type = %d\n", type);
	for( i = 0 ; i < json_object_array_length(mnc_obj) ; i++)
	{
		temp = json_object_array_get_idx(mnc_obj,i);
		carrier_obj = json_object_object_get(temp,"carrier");
		apn_obj = json_object_object_get(temp,"apn");
		servicegroup_obj = json_object_object_get(temp,"servicegroup");
		auto_obj = json_object_object_get(temp,"auto");
		protocol_obj = json_object_object_get(temp,"protocol");
		printf("carrier=%s, apn=%s, servicegroup=%s, auto=%s, protocol=%s\n"
			,json_object_get_string(carrier_obj)
			,json_object_get_string(apn_obj)
			,json_object_get_string(servicegroup_obj)
			,json_object_get_string(auto_obj)
			,json_object_get_string(protocol_obj));
	}
}

void get_apninfo_from_json(const char* jsonfile, char* mcc, char* mnc)
{
	json_object *root_obj = NULL;
	json_object *carrier_obj = NULL;
	json_object *apn_obj = NULL;
	json_object *servicegroup_obj = NULL;
	json_object *auto_obj = NULL;
	json_object *protocol_obj = NULL;
	json_object *mcc_obj= NULL;
	json_object *mnc_obj = NULL;

	json_object *temp = NULL;
	int type = 0;
	int i = 0;

	root_obj = json_object_from_file(jsonfile);
	if ( root_obj == NULL )
	{
		printf("Reading file object is failed.\n");
		return;
	}

	mcc_obj = json_object_object_get(root_obj,mcc);
	mnc_obj = json_object_object_get(mcc_obj,mnc);
	//type = json_object_get_type(mnc_obj);
	//printf("type = %d\n", type);
	for( i = 0 ; i < json_object_array_length(mnc_obj) ; i++)
	{
		temp = json_object_array_get_idx(mnc_obj,i);
		carrier_obj = json_object_object_get(temp,"carrier");
		apn_obj = json_object_object_get(temp,"apn");
		servicegroup_obj = json_object_object_get(temp,"servicegroup");
		auto_obj = json_object_object_get(temp,"auto");
		protocol_obj = json_object_object_get(temp,"protocol");
		printf("carrier=%s, apn=%s, servicegroup=%s, auto=%s, protocol=%s\n"
			,json_object_get_string(carrier_obj)
			,json_object_get_string(apn_obj)
			,json_object_get_string(servicegroup_obj)
			,json_object_get_string(auto_obj)
			,json_object_get_string(protocol_obj));
	}
}



void parseJsonArray1(json_object * pInput)
{
	json_object * pJsonObject = NULL;
	int type = 0;
	int i = 0;

	pJsonObject = json_object_object_get(pInput,"hobby");
	if( json_object_get_type(pJsonObject) != json_type_array) return;

	for( i = 0 ; i < json_object_array_length(pJsonObject) ; i++)
	{
		json_object * temp = json_object_array_get_idx(pJsonObject,i);
		type = json_object_get_type(temp);
		printf("[ex4]");
		PRINT_KEY_VALUE(type,temp);
	}
}

int main(int argc, char * argv[])
{
	json_object * pJsonObject = NULL;
	char szFileName[FILE_NAME_LEN] = "./default.json";
	int ret = 0;

	if( argc == 2 )
	{
		printf("%s\n",argv[1]);
		strncpy(szFileName,argv[1],FILE_NAME_LEN);
	}

	pJsonObject = json_object_from_file(szFileName);
	if ( pJsonObject == NULL )
	{
		printf("Reading file object is failed.\n");
		return -1;
	}

	//parseJsonObject2(pJsonObject);
	//get_data(pJsonObject, "450","08");
	get_apninfo_from_json(szFileName, "450", "08");
	json_object_put(pJsonObject);
	pJsonObject = NULL;

	return 0;
}
