#pragma once

#define STATIC_ALLOCATION_ONLY \
static void * operator new(size_t) = delete; \
static void * operator new[](size_t) = delete; \
static void  operator delete(void*)  = delete; \
static void  operator delete[](void*)  = delete;
