#include "server.h"

static Vector games = NULL;
static Map users = NULL;
static Vector clients = NULL;
static int cons = 0;

void connection_listener(){
    /*Create a connection listener at 127.0.0.1 (loopback) on port 5959*/
    int addrlen , new_socket , activity, i , sd;
    int max_sd;
    fd_set readfds;
    int master_socket;
    struct sockaddr_in address;
    //pthread_t threads[MAX_CONNECTIONS];
    
    master_socket = create_socket();
    address = init_connection(master_socket);
    
    addrlen = sizeof(address);
    puts("Waiting for connections ...");
     
    while(TRUE){
        printf("Start of the while loop\n");
        FD_ZERO(&readfds);
        FD_SET(master_socket, &readfds);
        max_sd = master_socket;
        printf("The number of connections is %i\n",cons);
        
        for (i = 0 ; i < cons ; i++) {
            sd = *((int *)vector_get(clients,i));
            if(sd > 0)
                FD_SET(sd,&readfds);
             
            if(sd > max_sd)
                max_sd = sd;
        }
        printf("The max process id is %i\n",max_sd);
        activity = select(max_sd + 1 , &readfds , NULL , NULL , NULL);
    
        if ((activity < 0) && (errno!=EINTR)) {
            printf("select error");
        }
          
        if (FD_ISSET(master_socket, &readfds)){
            if ((new_socket = accept(master_socket, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0){
                perror("accept");
                exit(EXIT_FAILURE);
            }           
            printf("New connection , socket fd is %d , ip is : %s , port : %d \n" , new_socket , inet_ntoa(address.sin_addr) , ntohs(address.sin_port));
            handle_connection(inet_ntoa(address.sin_addr),itoa(ntohs(address.sin_port)));
            vector_push(&clients,intmemcopy(&new_socket));
            cons++;
        }
          
        for (i = 0; i < cons; i++){
            printf("Checking on user %i\n",i);
            sd = *((int *)vector_get(clients,i));
            printf("Retrieved the socket of that user\n");
            if (FD_ISSET( sd , &readfds)){
                printf("Checking to see what action occured...\n");
                //thread_count++;
                handle_request((void *)&i);
                /*if(pthread_create(&threads[i],NULL,&handleRequest,(void *)&i)){
                    printf("Failed to create thread\n");
                    exit(EXIT_FAILURE);
                }*/
            }
        }
    }
    puts("Exiting the program");

    return;
}

handleRequest(void *)

int conn_response(libwebsock_client_state *state, libwebsock_message *msg){
    printf("Received a message from the connection!\n");
    return 0;
}

int on_new_connection(libwebsock_client_state *state) {
    printf("New connection with socket descriptor: %d\n", state->sockfd);
    puts("Sending a message to the client");
    libwebsock_send_text(state, "HELLO!");
    puts("done");
    return 0;
}

int on_close(libwebsock_client_state *state){
    fprintf(stderr, "Client closed connection: %d\n", state->sockfd);
    if(state->close_info) {
        fprintf(stderr, "We received a code and/or reason for close from client.\n");
        fprintf(stderr, "Code: %hi\n", state->close_info->code);
        fprintf(stderr, "Reason: %s\n", state->close_info->reason);
    }
    return 0;
}