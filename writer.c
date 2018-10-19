#include "writer.h"
#include "data_link.h"
#include "stateMachine.h"

int numAttempts=0;
int isConnected=0;

//handles the alarm signal
void timeout(){

  if((!isConnected) && (numAttempts < 4))
  {
    numAttempts++;
    printf("Attempt number=%d\n", numAttempts);
    connect();
  }
  else
  {
    printf("Operation failed!\n");
    exit(-1);
  }

}

//main function called after choosing sender/receiver
void data_writer(int argc, char * argv[]){

    int fd;

    fd=llopenW(1,2);


    int file_name_size = strlen(pinguim);
    unsigned char * file_name = (unsigned char *)malloc(file_name_size);
    file_name = (unsigned char *)pinguim;
    off_t final_size;
    unsigned char * read_file=readFile(file_name,&final_size);
    unsigned char * pointerToCtrlPacket=makeControlPackage_I(final_size,file_name, file_name_size,(int *) &final_size,CTRL_C_START);
    printf("size of File %ld \n",sizeof(final_size));
    llwriteW(fd,read_file,final_size);
}
int llwriteW(int fd, unsigned char * startOfFile,int finalSize){
  //unsigned char * BCC2s = (unsigned char *)malloc(sizeof(unsigned char));
  return 0;
}

int llopenW(int porta, int status){
    int res;
    SET[0] = FLAG;
    SET[1] = Aemiss;
    SET[2] = setC;
    SET[3]= SET[1]^SET[2];
    SET[4] = FLAG;

    UA[0] = FLAG;
    UA[1] = Arec;
    UA[2] = uaC;
    UA[3]= UA[1]^UA[2];
    UA[4] = FLAG;
    char buf[255];
    int curr_level=0;

    /*
      Open serial port device for reading and writing and not as controlling tty
      because we don't want to get killed if linenoise sends CTRL-C.
    */
    fd = open(link_layer.port, O_RDWR | O_NOCTTY );
    if (fd <0) {perror(link_layer.port); exit(-1); }
    if(setTermios(fd)<0){
      perror("Setting termios settings");
      return -1;
    }
    if(!link_layer.status){//EMISSOR
     (void) signal(SIGALRM,timeout);
     res=write(fd,SET,5);
     alarm(3);
     printf("Sent SET,waiting for receiver\n");
    //RECEIVE UA
     while (curr_level<5) {       /* loop for input */
        res = read(fd,buf,1);
        if(res>0){
          curr_level=stateMachine(buf[0],curr_level,UA);
        }
      }
          printf("Received UA\n");
    }

    isConnected = 1;

    return fd;
  }

void connect(){
    write(fd,SET,5);
    alarm(3);
}





unsigned char * readFile(unsigned char * fileName, off_t * fileSize){

    FILE * fd;
    struct stat data;

    if((fd= fopen(((const char *)fileName), "rb")) == NULL){
        perror("Error while opening the file");
        return NULL;
    }
    stat((const char*)fileName, &data); //get the file metadata

    *fileSize = data.st_size; //gets file size in bytes


    unsigned char * fileData = (unsigned char*)malloc(*fileSize);

    fread(fileData,sizeof(unsigned char),*fileSize,fd);
    if(ferror(fd)){
          perror("error writting to file\n");
    }
    fclose(fd);
    return fileData;

}

unsigned char * makeControlPackage_I(off_t fileSize, unsigned char * fileName, int fileName_size, int * finalSize, unsigned char start_or_end){
  /*

TLV (Type, Length, Value)
– T (um octeto) – indica qual o parâmetro (0 – tamanho do ficheiro, 1 – nome do
ficheiro, outros valores – a definir, se necessário)
– L (um octeto) – indica o tamanho em octetos do campo V (valor do parâmetro)
– V (número de octetos indicado em L) – valor do parâmetro
  */
  
  
  	int start_packet_len = 5 + sizeof(fileSize) + fileName_size;
  	char *finalPackage = ( char *)malloc(sizeof(char) * start_packet_len);

    if(start_or_end == CTRL_C_START){
      finalPackage[0]=start_or_end;
    }
    else if(start_or_end == CTRL_C_END){
      finalPackage[0] = start_or_end;
    }
    else {printf("Invalid value in start_or_end!");return NULL;}
    finalPackage[1]=T1; //Tamanho do ficheiro
    finalPackage[2]=sizeof(fileSize);  //8
	*((off_t *)(finalPackage + 3)) = fileSize;
  	finalPackage[3 + sizeof(fileSize)] = T2;  //Nome do ficheiro
  	finalPackage[4 + sizeof(fileSize)] = fileName_size;
	strcat(finalPackage + 5 + sizeof(fileSize),(char *)fileName);
	printf("size of fileSize:%ld    size of finalPackage:%ld",fileSize,sizeof(finalPackage[4]));

   // finalPackage[3]=
    return NULL;

}
