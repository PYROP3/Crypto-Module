#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<fcntl.h>
#include<string.h>
#include<unistd.h>

#define BUFFER_LENGTH 258              // The buffer length including option
unsigned char receive[BUFFER_LENGTH];  // The receive buffer from the LKM

void c2h(char *, char *, int );
char c2h_conv(char);

int main(){
	
    int ret, fd, opcao, op;
    char stringToSend[BUFFER_LENGTH - 2];
	char send[BUFFER_LENGTH];

	fd = open("/dev/MyCryptoRomance", O_RDWR);         // Open the device with read/write access
	
	do{
		
		do{
			system("clear");
			printf("-------------------------------------------------\n");
			printf(" Digite a opcao desejada: \n\n");
			printf(" 0. Sair;\n");
			printf(" 1. Cifrar string;\n");
			printf(" 2. Decifrar string;\n");
			printf(" 3. Resumo criptografico;\n\n");
			printf(" Opcao: ");
			scanf("%i", &opcao);
			printf("-------------------------------------------------\n");
		}while(opcao < 0 || opcao > 3);      
	   
		if(opcao != 0){
			
            if (fd < 0){
				perror("FOMOS FALHOS AO ABRIR O DISPOSITIVO...\n");
				printf("Erro cod. %d, %d\n", fd, (int)errno);
				return errno;
			}
			
	
			system("clear");
			printf("-------------------------------------------------\n");
			printf(" Digite a forma que deseja digitar a string: \n\n");
			printf(" 1. Hexadecimal;\n");
			printf(" 2. ASCII;\n\n");
			printf(" Opcao: ");
			scanf("%i", &op);
			printf("-------------------------------------------------\n");
	
	        char fu[] = {'c', 'd', 'h'};
			
			printf("Digite a string a ser ");
			
			switch(opcao){
				case 1:
						printf("cifr");
					break;
				case 2:
						printf("decifr");
					break;
				case 3:
						printf("hashe");
					break;
			}
			
			printf("ada: ");
			getchar();
			scanf("%[^\n]%*c", stringToSend);  // Read in a string (with spaces)
			
			send[0] = ' ';
			send[1] = ' ';
			
			if(op == 2)
				c2h(&(stringToSend[2]), &(send[2]), strlen(stringToSend) + 1);
	
	        stringToSend[0] = fu[opcao - 1];
			stringToSend[1] = ' ';	
	
			printf("Enviarei: [%s]\n", stringToSend);
			
			ret = write(fd, stringToSend, strlen(stringToSend)); // Send the string to the LKM
			if (ret < 0){
				perror("Failed to write the message to the device.");
				return errno;
			}

			printf("Press ENTER to read back from the device...\n");
			getchar();

			printf("Reading from the device...\n");
			
			ret = read(fd, receive, BUFFER_LENGTH);        // Read the response from the LKM
			if (ret < 0){
				perror("Failed to read the message from the device.");
				return errno;
			}
            
            int temp = strlen(stringToSend);
            if (temp % 16) {
                temp /= 16;
                temp += 1;
                temp *= 16;
            }
            
			unsigned char c;
			/*
            for(int i=0;i<32;i++) {
				c = receive[i];
				printf("The received message is: [%u]\n", c);
			}
            */
			printf("Hex: ");
				for(int i=0;i<temp;i++) {
				c = receive[i];
				printf("%02X", c);
			}
			printf("\n\n");
			
			printf("ASCII: ");
			for(int i=0;i<temp;i++) {
				c = receive[i];
				printf("%c", c);
			}
			printf("\n");
			
			printf("Press ENTER to return to menu...\n");
			getchar();
			
			stringToSend[0] = 0;
		}
		
	}while(opcao != 0);
	
	close(fd);
	
	printf("End of the program\n");
	return 0;
}

void c2h(char *charstrn, char *hexstrn, int charlen) {
    charlen--;
    while (charlen-- >= 0) {
        hexstrn[2*charlen+1] = c2h_conv(charstrn[charlen] % (char)16); //1s
        hexstrn[2*charlen] = c2h_conv(charstrn[charlen] / (char)16);   //16s
    }
}

char c2h_conv(char c) {
    if (c < (char)10) return c + '0';
    return c + 'A' - (char)10;
}
