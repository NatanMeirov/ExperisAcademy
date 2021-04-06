#ifndef __PERSONLINKEDLIST_H__
#define __PERSONLINKEDLIST_H__

#include <stddef.h> /* size_t */
#include <stdio.h>

typedef struct Person
{
	size_t m_id;
	char m_name[256];
	size_t m_age;
	struct Person* m_next;
} Person;


/*
- Description: Inserts a new Person as the head of the linked list

- Input: [_head:] a Person linked list head, [_p:] a pointer to a Person to insert as the new head of the linked list

- Output: A pointer to the head of the Person linked list
*/
Person* ListInsertHead(Person* _head, Person* _p);


/*
- Description: Removes the Person in the head of the linked list, sets the next Person as the new head of the list, returning the removed Person using the pointer _item

- Input: [_head:] a Person linked list head, [_item:] a pointer to a pointer of Person to return the removed value

- Output: A pointer to the head of the Person linked list
*/
Person* ListRemoveHead(Person* _head, Person** _item);


/*
- Description: Inserts (using iterative way) a new Person to the linked list by its key (ascending order), or ignores the new Person if the key already exists

- Input: [_head:] a Person linked list head, [_key:] the key (id) of the new Person to insert to the linked list, [_p:] a pointer to a Person to insert

- Output: A pointer to the head of the Person linked list
*/
Person* ListInsertByKey(Person* _head, int _key, Person* _p);


/*
- Description: Inserts (using recursive way) a new Person to the linked list by its key (ascending order), or ignores the new Person if the key already exists

- Input: [_head:] a Person linked list head, [_key:] the key (id) of the new Person to insert to the linked list, [_p:] a pointer to a Person to insert

- Output: A pointer to the head of the Person linked list
*/
Person* ListInsertByKeyRec(Person* _head, int _key, Person* _p);


/*
- Description: Removes (using iterative way) a Person from the linked list by its key, and returning the removed value using a pointer to a pointer to a Person

- Input: [_head:] a Person linked list head, [_key:] the key (id) of the Person to remove from the linked list, [_p:] a pointer to a pointer to Person to store and return the removed item

- Output: A pointer to the head of the Person linked list
*/
Person* ListRemoveByKey(Person* _head, int _key, Person** _p);


/*
- Description: Removes (using recursive way) a Person from the linked list by its key, and returning the removed value using a pointer to a pointer to a Person

- Input: [_head:] a Person linked list head, [_key:] the key (id) of the Person to remove from the linked list, [_p:] a pointer to a pointer to Person to store and return the removed item

- Output: A pointer to the head of the Person linked list
*/
Person* ListRemoveByKeyRec(Person* _head, int _key, Person** _p);


/*
- Description: Prints a Person linked list

- Input: [_head:] a Person linked list head

- Output: None
*/
void PrintList(Person* _head);


#endif /*#ifndef __PERSONLINKEDLIST_H__*/
