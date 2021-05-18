

#define NORMAL_MODE					  0
#define WAKE_UP_MODE 					2
#define POWER_SAVING_MODE 		1
#define SLEEP_MODE 						3


void Lora_Mode(unsigned short mode);
void Lora_ISR(unsigned short uart,unsigned short uart_mgr[], char str[]);
void Lora_send_request(unsigned short uart,unsigned int id_send, unsigned int id_dest,unsigned int status,unsigned int cb1, unsigned int cb2);
void Lora_process(unsigned short uart,unsigned char id, char msg[] ,unsigned short mgr[],unsigned short cb1, unsigned short cb2 );



