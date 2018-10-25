#ifndef APPLICATION_LAYER_H_
#define APPLICATION_LAYER_H_

typedef struct {
int fileDescriptor; /*Descritor correspondente à porta série*/
int status; /*TRANSMITTER | RECEIVER*/
int size;
}applicationLayer;

applicationLayer app_layer;

#define PACKET_H_C      1       //Value of packet header Control
#define PACKET_SIZE     128     //Size you send each iteration

//  WRITER FUNCTIONS

unsigned char * makeControlPackage_I(off_t fileSize, unsigned char * fileName, long int fileName_size, long int * finalSize, unsigned char start_or_end);
unsigned char * readFile(unsigned char * fileName, long int * fileSize);
void data_writer(int argc, char * argv[]);

unsigned char * makePacketHeader(unsigned char * fileFragment, long int fileSize, int * sizeOfFragment);
unsigned char * splitFile(unsigned char * file, long int * curr_index, int * packetSize, long int fileSize);
void createFile(unsigned char *mensagem, off_t *sizeFile, char * filename);
//  READER FUNCTIONS

void data_reader(int argc, char * argv[]);
unsigned char * removeHeaders(unsigned char * packetWithHeader,unsigned long * sizeWithHeaders);

#endif
