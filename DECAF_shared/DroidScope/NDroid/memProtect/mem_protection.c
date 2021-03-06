/**
 * author: Chenxiong (R0r5ch4ch) Qian
 * date: 2014-11-10
 */

#include "mem_protection.h"
#include "DECAF_shared/DroidScope/linuxAPI/ProcessInfo.h"
#include "DECAF_shared/utils/OutputWrapper.h"

#define DVM_HEAP_MODULE_NAME "/dev/ashmem/dalvik-heap"
#define DVM_STACK_MODULE_NAME "/dev/ashmem/dalvik-LinearAlloc"

gva_t DVM_HEAP_START = -1, DVM_HEAP_END = -1;
gva_t DVM_STACK_START = -1, DVM_STACK_END = -1;
gva_t DVM_LINEAR_ALLOC_START = -1, DVM_LINEAR_ALLOC_END = -1;

void initDVMHeapRanges(gpid_t tracingPID){
	ModuleNode* node = getModulesByName(tracingPID, DVM_HEAP_MODULE_NAME);
	if(node != NULL){
		ModuleNode* i = node;
		DVM_HEAP_START = i->startAddr;
		DVM_HEAP_END = i->endAddr;
		DECAF_printf("%s's address starts@%x\n", DVM_HEAP_MODULE_NAME, DVM_HEAP_START);
		for(; i != NULL; i = i->next){
			if(strcmp(getModuleNodeName(i), DVM_HEAP_MODULE_NAME) != 0){
				break;
			}
			DVM_HEAP_END = i->endAddr;
		}
		DECAF_printf("%s's address ends@%x\n", DVM_HEAP_MODULE_NAME, DVM_HEAP_END);
	}else{
		DECAF_printf("Cannot find module named %s!\n", DVM_HEAP_MODULE_NAME);
	}
}

//unused!
void initDVMLinearAllocRanges(gpid_t tracingPID){
	ModuleNode* node = getModulesByName(tracingPID, DVM_STACK_MODULE_NAME);
	if(node != NULL){
		ModuleNode* i = node;
		DVM_LINEAR_ALLOC_START = i->startAddr;
		DVM_LINEAR_ALLOC_END = i->endAddr;
		DECAF_printf("%s's address starts@%x\n", DVM_STACK_MODULE_NAME, DVM_LINEAR_ALLOC_START);
		for(; i != NULL; i = i->next){
			if(strcmp(getModuleNodeName(i), DVM_STACK_MODULE_NAME) != 0){
				break;
			}
			DVM_LINEAR_ALLOC_END = i->endAddr;
		}
		DECAF_printf("%s's address ends@%x\n", DVM_STACK_MODULE_NAME, DVM_LINEAR_ALLOC_END);
	}else{
		DECAF_printf("Cannot find module named %s!\n", DVM_STACK_MODULE_NAME);
	}
}

void refreshDVMHeapRanges(gpid_t tracingPID){
	initDVMHeapRanges(tracingPID);
}

void refreshDVMLinearAllocRanges(gpid_t tracingPID){
	initDVMLinearAllocRanges(tracingPID);
}

int isWithinDVMLinearAlloc(gva_t addr){
	if ((addr >= DVM_LINEAR_ALLOC_START)
			&& (addr <= DVM_LINEAR_ALLOC_END)){
		return (1);
	}
	return (0);
}

int isWithinDVMStack(gva_t addr){
	if((addr <= DVM_STACK_START) 
			&& (addr >= DVM_STACK_END)){
		return (1);
	}
	return (0);
}

int isWithinDVMHeap(gva_t addr){
	if((addr >= DVM_HEAP_START)
			&& (addr <= DVM_HEAP_END)){
		return (1);
	}
	return (0);
}
