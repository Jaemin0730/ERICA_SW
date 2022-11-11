#include <stdio.h>
#include <string.h>
#include "dll_address.h"

// main.c 소스는 수정하지 않고 결과 확인용으로만 사용합니다.
// 평가시에는 입력 값이나 횟수가 변경됩니다.

// 자유롭게 함수 테스트 하는 공간입니다.
void test_free(void)
{

}

void test_list(void)
{
	DoublyLinkedList test_list;
	bool bool_result = true; 
	int int_result = 0;

	printf("1\n");
	list_init(&test_list);
	printf("2\n");
	bool_result = is_empty_list(&test_list);
	printf("3\n");
	
	if (bool_result == true)
		printf("TRUE\n");
	else
		printf("FALSE\n");

	bool_result = is_full_list(&test_list);
	if (bool_result == true)
		printf("TRUE\n");
	else
		printf("FALSE\n");

	char name[NAME_ARRAY_SIZE] = "";
	int id = 0;

	// insert_head
	strcpy_s(name, NAME_ARRAY_SIZE, "오주현");
	id = 20220001;
	insert_head(&test_list, name, id);

	strcpy_s(name, NAME_ARRAY_SIZE, "서민욱");
	id = 20220002;
	insert_head(&test_list, name, id);
	
	strcpy_s(name, NAME_ARRAY_SIZE, "남상식");
	id = 20220003;
	insert_head(&test_list, name, id);
	
	strcpy_s(name, NAME_ARRAY_SIZE, "박춘수");
	id = 20220004;
	insert_head(&test_list, name, id);
	
	strcpy_s(name, NAME_ARRAY_SIZE, "김미진");
	id = 20220005;
	insert_head(&test_list, name, id);
	
	strcpy_s(name, NAME_ARRAY_SIZE, "제갈지성");
	id = 20220006;
	insert_head(&test_list, name, id);

	strcpy_s(name, NAME_ARRAY_SIZE, "서정일");
	id = 20220007;
	insert_head(&test_list, name, id);

	strcpy_s(name, NAME_ARRAY_SIZE, "하성남");
	id = 20220008;
	insert_head(&test_list, name, id);

	strcpy_s(name, NAME_ARRAY_SIZE, "하만세");
	id = 20220009;
	insert_head(&test_list, name, id);

	strcpy_s(name, NAME_ARRAY_SIZE, "송윤선");
	id = 20220010;
	insert_head(&test_list, name, id);

	strcpy_s(name, NAME_ARRAY_SIZE, "사공예성");
	id = 20220011;
	insert_head(&test_list, name, id);

	strcpy_s(name, NAME_ARRAY_SIZE, "유예준");
	id = 20220012;
	insert_head(&test_list, name, id);
	// insert_tail
	strcpy_s(name, NAME_ARRAY_SIZE, "전정민");
	id = 20220013;
	insert_tail(&test_list, name, id);
	
	strcpy_s(name, NAME_ARRAY_SIZE, "임정주");
	id = 20220014;
	insert_tail(&test_list, name, id);

	strcpy_s(name, NAME_ARRAY_SIZE, "오주현");
	id = 20220015;
	insert_tail(&test_list, name, id);

	strcpy_s(name, NAME_ARRAY_SIZE, "노은용");
	id = 20220016;
	insert_tail(&test_list, name, id);

	strcpy_s(name, NAME_ARRAY_SIZE, "류다운");
	id = 20220017;
	insert_tail(&test_list, name, id);

	strcpy_s(name, NAME_ARRAY_SIZE, "이미르");
	id = 20220018;
	insert_tail(&test_list, name, id);
	print_list(&test_list);
	
	search_name(&test_list, 20220014);
	search_id(&test_list, "오주현");
	search_id(&test_list, "김재현");
}

int main(void)
{
	test_free();
	test_list();

	return 0;
}