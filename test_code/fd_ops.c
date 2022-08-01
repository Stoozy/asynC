#include <stdio.h>

#include "../src/asynC.h"

void after_open(int fd, void* cb_arg);

void after_chmod(int ret, void* arg){
    printf("my chmod return value is %d\n", ret);
}

void call_async_open(){
    char filename[] = "../test_files/lorem_ipsum.txt";
    async_open(filename, O_RDONLY, 0644, after_open, NULL);
}

void callchmod(){
    char filename[] = "../test_files/lorem_ipsum.txt";
    async_chmod(filename, 0644, after_chmod, NULL);
}

void after_dns(char** ip_list, int num_ips, void* arg){

    for(int i = 0; i < num_ips; i++){
        printf("curr address is %s\n", ip_list[i]);
    }
    free(ip_list);
}

int main(){
    asynC_init();

    //async_dns_lookup("www.tire.com", after_dns, NULL);
    http_request_options options;
    async_http_request_options_init(&options);
    async_http_request_options_set_header(&options, "foo", "bar");
    async_http_request_options_set_header(&options, "spaghetti", "meatball");
    /*async_outgoing_http_request* new_request = */async_http_request("example.com", "GET", &options, NULL);

    //call_async_open();
    //callchmod();
    /*
    async_container_vector* int_vector = async_container_vector_create(1, 10, sizeof(int));
    int num = 3;
    async_container_vector_add_first(&int_vector, &num);
    num = 4;
    async_container_vector_add_first(&int_vector, &num);
    num = 5;
    async_container_vector_add_last(&int_vector, &num);
    num = 6;
    async_container_vector_add(&int_vector, &num, 1);
    for(int i = 0; i < async_container_vector_size(int_vector); i++){
        int curr_num;
        async_container_vector_get(int_vector, i, &curr_num);
        printf("curr num is %d\n", curr_num);
    }
    */

    asynC_cleanup();

    return 0;
}

void after_read(int, buffer*, int, void*);

void after_open(int fd, void* cb_arg){
    printf("my fd is %d\n", fd);
    int num_buff_bytes = 100;
    async_read(fd, create_buffer(num_buff_bytes, 1), num_buff_bytes, after_read, NULL);
}

void after_read(int fd, buffer* read_buffer, int num_bytes_read, void* arg){
    char* read_buff = (char*)get_internal_buffer(read_buffer);
    printf("%s\n", read_buff);
}