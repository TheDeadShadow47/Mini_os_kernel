#include "print.h"
#include "keyboard.h"
#include "rtc.h"
#include "cpu.h"
#include "pmm.h"

char input_buffer[128];
int input_ptr = 0;


int str_compare(char* s1, char* s2) {
    while (*s1 && (*s1 == *s2)) {
        s1++; s2++;
    }
    return *(unsigned char*)s1 - *(unsigned char*)s2;
}


int str_starts_with(char* str, char* prefix) {
    while (*prefix) {
        if (*prefix++ != *str++) return 0;
    }
    return 1;
}

void execute_command(char* input) {
    
    if (str_compare(input, "HELP") == 0) {
        print_str("Commands: HELP, CLEAR, TIME,  VERSION, ECHO <text>, CPU, MEM , FREE \n");
    }

    else if (str_compare(input, "CLEAR") == 0) {
        print_clear();
    }

    else if (str_compare(input, "VERSION") == 0) {
        print_str("MiniOsKernal By Aymen v1.1.0 - \n");
    }

    else if(str_compare(input,"TIME")==0){
        uint8_t sec= rtc_seconds();
        print_str("current rtc seconds: ");
        print_char((sec / 10)+ '0');
        print_char((sec % 10)+ '0');
        print_char('\n');

    }

    else if (str_starts_with(input, "ECHO ")) {
        print_str(input + 5); 
        print_char('\n');
    }


     else if (str_compare(input, "CPU") == 0) {
        print_str("attempting to read \n" );
        char vendor[13];
        get_cpu_vendor(vendor);
        print_str("CPU Vendor: ");
        print_str(vendor);
        print_char('\n');
    }

    else if(str_compare(input, "MEM")==0){
        void* addr1=pmm_alloc_page();
        void* addr2=pmm_alloc_page();
        if(addr1){
            print_str("allocated page 1:0x");
            print_uint64_hex((uint64_t)addr1);
            print_char('\n');
        }
        if(addr2){
            print_str("allocated page 2:0x");
            print_uint64_hex((uint64_t)addr2);
            print_char('\n');
        }
        pmm_free_page(addr1);
        print_str("Page 1 released.\n");
    }
    else if (str_compare(input, "FREE") == 0) {
        uint64_t bytes = pmm_get_free_count();
        print_str("Free RAM: ");
        print_uint64_dec(bytes / 1024);
        print_str(" KB\n");
    }

    else if (input[0] != '\0') {
        print_str("Unknown command. Type HELP for list.\n");
    }
   
    
    print_str("> "); 
}

void handle_input(struct KeyboardEvent event) {
    if (event.type == KEYBOARD_EVENT_TYPE_MAKE) {
        char c = to_ascii(event.code);
        
        if (c == '\n') {
            print_char('\n');
            input_buffer[input_ptr] = '\0';
            execute_command(input_buffer);
            input_ptr = 0; 
        } 
        else if (c == '\b') {
            if (input_ptr > 0) {
                input_ptr--;
                print_char('\b'); 
            }
        } 
        else if (c != '?' && input_ptr < 127) {
            input_buffer[input_ptr++] = c;
            print_char(c); 
        }
    }
}

void kernel_main(uint32_t magic, void* mboot_ptr) {
    print_clear();
    
    
    print_set_color(PRINT_COLOR_YELLOW, PRINT_COLOR_BLACK);
    
    print_str("Welcome to My mini os kernel !!!\n");
    print_str("Use command HElP to see all available commands!!!");
    print_str("> ");

    pmm_init(128*1024*1024,(void*)0x1000000);


    for(int i=0; i<256 ; i++){
        pmm_mark_used((void*)(uint64_t)(i * PAGE_SIZE));
    }
    
    keyboard_init();
    keyboard_set_handler(handle_input);
    
   
    while (1) {
        asm volatile("hlt");
    }
   
}