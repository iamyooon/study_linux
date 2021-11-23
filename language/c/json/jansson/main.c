#include <stdio.h>
#include <jansson.h>

int LoadAPNInfo(char* mcc, char* mnc)
{
	json_t *root, *child, *grandchild, *tmp, *tmp2;
	json_error_t error;
	const char *key, *carrier, *apn, *auto_act, *protocol;
	int type, size, i;

	root = json_load_file("./CCU2_APNLIST_211109.json",0, &error);

	if(!root){
		printf("error occured\n");
		return -1;
	}

	child = json_object_get(root, mcc);
	grandchild = json_object_get(child, mnc);
	printf("type=%d\n", json_typeof(grandchild));

	size = json_array_size(grandchild);
	printf("======================================\n");
	printf("carrier,apn,servicegroup,auto,protocol\n");
	printf("======================================\n");
	for(i=0; i < size; i++) {
		tmp = json_array_get(grandchild, i);
		carrier = json_string_value(json_object_get(tmp, "carrier"));
		apn = json_string_value(json_object_get(tmp, "apn"));
		auto_act = json_string_value(json_object_get(tmp, "auto"));
		protocol = json_string_value(json_object_get(tmp, "protocol"));

		printf("%s,%s,%s,%s\n", carrier, apn, auto_act, protocol);
	}
}

int main(int argc, char** argv)
{
	json_t *root, *child, *grandchild, *tmp, *tmp2;
	json_error_t error;
	const char *mcc, *mnc, *key, *carrier, *apn, *auto_act, *protocol;
	json_t *element;
	int type, size;
	int i;
	root = json_load_file("./CCU2_APNLIST_211109.json",0, &error);

	if(!root){
		printf("error occured\n");
		return -1;
	}

	type = json_typeof(root);
	printf("type=%d, size=%d\n", json_typeof(root), json_object_size(root));

	json_object_foreach(root, mcc, child) {
		printf("->type=%d, size=%d, mcc=%s\n", json_typeof(child), json_object_size(child), mcc);
		json_object_foreach(child, mnc, grandchild) {
			printf("->->type=%d, size=%d, mnc=%s\n", json_typeof(grandchild), json_object_size(grandchild), mnc);
			if (json_typeof(grandchild) == 1)
				size = json_array_size(grandchild);
				printf("->->-> size=%d\n", size);
				for (i=0; i < size; i++) {
					printf("->->->-> type=%d\n",json_typeof( json_array_get(grandchild, i)));
				}
		}
	}

	child = json_object_get(root, "450");
	grandchild = json_object_get(child, "08");

	size = json_array_size(grandchild);
	printf("======================================\n");
	printf("carrier,apn,servicegroup,auto,protocol\n");
	printf("======================================\n");
	for(i=0; i < size; i++) {
		tmp = json_array_get(grandchild, i);
		carrier = json_string_value(json_object_get(tmp, "carrier"));
		apn = json_string_value(json_object_get(tmp, "apn"));
		auto_act = json_string_value(json_object_get(tmp, "auto"));
		protocol = json_string_value(json_object_get(tmp, "protocol"));

		printf("%s,%s,%s,%s\n", carrier, apn, auto_act, protocol);
#if 0
		//printf("type=%d, size=%d\n", json_typeof(tmp), json_object_size(tmp));
		json_object_foreach(tmp, key, tmp2) {
			//printf("%s=%s\n", key, json_string_value(tmp2));
			printf("%s(%d),", json_string_value(tmp2), json_typeof(tmp2));
		}
		printf("\n");
#endif
	}

	LoadAPNInfo("450","08");


	return 0;
}
