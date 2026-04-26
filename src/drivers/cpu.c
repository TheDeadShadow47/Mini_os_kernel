#include "cpu.h"
void get_cpu_vendor(char* vendor_str){
    uint32_t ebx ,ecx, edx ,eax;
    asm volatile("cpuid"
                 : "=b"(ebx), "=c"(ecx), "=d"(edx), "=a"(eax)
                 : "a"(0));
    ((uint32_t*)vendor_str)[0]=ebx;
    ((uint32_t*)vendor_str)[1]=edx;
    ((uint32_t*)vendor_str)[2]=ecx;
    vendor_str[12]='\0';
}