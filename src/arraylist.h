#ifndef HIGANCC_ARRAYLIST_H
#define HIGANCC_ARRAYLIST_H

#include "common.h"
#include "memory.h"

#define ARRAYLIST_DEFAULT_CAPACITY 8

#define ArrayList(Type) ArrayList_##Type
#define ArrayList_init(Type, Allocator) ArrayList_##Type##_init(Allocator);
#define ArrayList_size(Type, Array) ArrayList_##Type##_size(Array)
#define ArrayList_isEmpty(Type, Array) ArrayList_##Type##_isEmpty(Array)
#define ArrayList_get(Type, Array, Index) ArrayList_##Type##_get(Array, Index)
#define ArrayList_set(Type, Array, Index, Elem) ArrayList_##Type##_set(Array, Index, Elem)
#define ArrayList_add(Type, Array, Elem) ArrayList_##Type##_add(Array, Elem)
#define ArrayList_addAt(Type, Array, Index, Elem) ArrayList_##Type##_addAt(Array, Index, Elem)
#define ArrayList_remove(Type, Array, Index) ArrayList_##Type##_remove(Array, Index)

#define ARRAYLIST_PROTOTYPE(Type) \
typedef struct {\
	Type *data; \
	size_t len; \
	size_t capacity;\
    ArenaAllocator *allocator;\
} ArrayList_##Type;\
ArrayList_##Type* ArrayList_##Type##_init(ArenaAllocator *a);\
int ArrayList_##Type##_size(ArrayList_##Type *arr);\
bool ArrayList_##Type##_isEmpty(ArrayList_##Type *arr);\
Type ArrayList_##Type##_get(ArrayList_##Type *arr, size_t index);\
void ArrayList_##Type##_set(ArrayList_##Type *arr, size_t index, Type elem);\
void ArrayList_##Type##_add(ArrayList_##Type *arr, Type elem);\
void ArrayList_##Type##_addAt(ArrayList_##Type *arr, int index, Type elem);\
Type ArrayList_##Type##_remove(ArrayList_##Type *arr, size_t index);\

#define ARRAYLIST_IMPL(Type) \
ArrayList_##Type* ArrayList_##Type##_init(ArenaAllocator *a) {\
	ArrayList_##Type *array_list = (ArrayList_##Type *) ArenaAllocator_alloc(a, sizeof(ArrayList_##Type));\
	array_list->data = (Type *) ArenaAllocator_alloc(a, ARRAYLIST_DEFAULT_CAPACITY * sizeof(Type));\
	array_list->len = 0;\
	array_list->capacity = ARRAYLIST_DEFAULT_CAPACITY;\
    array_list->allocator = a;\
	return array_list;\
}\
int ArrayList_##Type##_size(ArrayList_##Type *arr) {\
	return arr->len;\
}\
bool ArrayList_##Type##_isEmpty(ArrayList_##Type *arr) {\
	return arr->len == 0;\
}\
Type ArrayList_##Type##_get(ArrayList_##Type *arr, size_t index) {\
	return arr->data[index];\
}\
void ArrayList_##Type##_set(ArrayList_##Type *arr, size_t index, Type elem) {\
	arr->data[index] = elem;\
}\
void ArrayList_##Type##_add(ArrayList_##Type *arr, Type item) {\
	if (arr->len + 1 >= arr->capacity) {\
        int prev_capacity = arr->capacity;\
		if (arr->capacity == 0) arr->capacity = 1;\
		else arr->capacity *= 2;\
		arr->data = (Type*)ArenaAllocator_resize(arr->allocator, arr->data, prev_capacity, sizeof(Type) * arr->capacity);\
	}\
	arr->data[arr->len++]=item;\
}\
void ArrayList_##Type##_addAt(ArrayList_##Type *arr, int index, Type elem) {\
	if (arr->len + 1 >= arr->capacity) {\
		void *prev = arr->data;\
		arr->data = ArenaAllocator_alloc(arr->allocator, arr->capacity * 2 * sizeof(Type));\
		memmove(arr->data, prev, arr->len * sizeof(Type));\
		arr->capacity *= 2;\
	}\
	memmove(arr->data + index + 1, arr->data + index, sizeof(Type) * (arr->len - index));\
	arr->data[index] = elem;\
	arr->len++;\
}\
Type ArrayList_##Type##_remove(ArrayList_##Type *arr, size_t index) {\
	if (index >= arr->len) return (Type){0};\
	Type value = arr->data[index];\
	if (index == arr->len - 1) {\
		arr->len--;\
		return value;\
	}\
	Type *from = arr->data + index + 1;\
	Type *to = arr->data + index;\
	memmove(to, from, sizeof(Type) * (arr->len - index - 1));\
	arr->len--;\
	return value;\
}\

#endif
