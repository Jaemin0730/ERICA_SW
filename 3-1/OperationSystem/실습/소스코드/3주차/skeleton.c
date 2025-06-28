#include <linux/module.h>
#include <linux/init.h>
#include <linux/list.h> 
#include <linux/printk.h> 
#include <linux/slab.h>	

typedef struct birthday{
	int day;
	int month;
	int year;
	struct list_head list;
}bd;

struct list_head bdList;
LIST_HEAD(bdList);

static bd *createBirthday(int _day, int _month, int _year) {

	/* 1. TODO: 생일을 위한 메모리를 할당하고, 인자들을 채워서 
	 *          생일노드를 완성하세요. 
	 */

	bd* new = (bd*)kmalloc(sizeof(bd), GFP_KERNEL);
	new->day = _day;
	new->month = _month;
	new->year = _year;
	INIT_LIST_HEAD(&new->list);
	return new;
}

static __init int moduleInit(void) {

	pr_info("INSTALL MODULE: skeleton\n");

	/* 2. TODO: 생일 목록을 하나씩 생성하는대로 
	 *          연결리스트에 연결시키세요. 
	 *    < list_add_tail 사용 >
	 */
	bd* bd1 = createBirthday(30, 7, 2001);
	bd* bd2 = createBirthday(26, 2, 2002);
	bd* bd3 = createBirthday(13, 10, 2006);
	
	list_add_tail(&bd1->list, &bdList);
	list_add_tail(&bd2->list, &bdList);
	list_add_tail(&bd3->list, &bdList);
	/* 3. TODO: 완성된 연결리스트를 탐색하는
	 *          커널 함수를 사용하여 출력하세요. 
	 */
	bd *search; // 현재 순회 중인 노드를 담을 구조체 포인터
	list_for_each_entry(search, &bdList, list) {
		pr_info("Day: %d Month: %d Year: %d\n",search->day, search->month, search->year);
	}
	return 0;
}

static __exit void moduleExit(void) {

	/* Warining: 모듈 삭제할 때, 할당된 연결리스트도 
	 *           하나씩 제거하여 주세요 !
	 *           제거하기 전, 연결리스트가 비어 있는지, 
	 *           예외처리가 필요함
	 */
	if( list_empty(&bdList) ) {
		pr_info("List is empty !!");		
		return;
	}

	/* 4. TODO: 연결 리스트를 탐색하면서 하나씩 제거함
	 *    
	 * Warning: 제거를 하면서 연결리스트 탐색하면 다소 
         *          문제가 생길 수 있는데, 어떤 방법으로 
	 *          취할지, 생각하고 코드를 작성하세요.
	 */
	bd *search, *k; // 현재 순회 중인 노드를 담을 구조체 포인터, 다음 노드를 미리 저장해두는 포인터(임시 변수)
	list_for_each_entry_safe(search, k, &bdList, list) {
		pr_info("Delete %d Day, %d Month, %d Year\n", search->day, search->month, search->year);
		list_del(&search->list);
		kfree(search);
	}
	pr_info("REMOVE MODULE: skeleton\n");
}  

module_init(moduleInit);
module_exit(moduleExit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("김재민_2021050300"); // 이름_학번 형식으로 작성하여 제출함


