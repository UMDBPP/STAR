
/* ifstream objects cannot be copied and therefore
 * cannot be a member of the class, so its defined
 * globally in this file
 */
std::ifstream real_fileobj_;

/*
 * This is a stubbed version of the arduino File
 * class which actually reads from a file on the 
 * local system. This was tested on windows.
 */
class File{
public:
	File();
	int32_t available();
  char read();
  int16_t read(void *buf, uint16_t nbyte);
  operator bool(){
    return opened_;
  }
  void close();
  bool is_open(){
    return opened_;
  }
  bool open(const char *filename);
  
 private:
  // private member 
  char name_[30]; // filename
  bool opened_;
  int filesize_;
  uint8_t *filedata_;
  int file_pos_;
  // private member functions
  int filesize(const char* filename);
};

/*
 * Constructor for File class
 *
 * Inputs:
 * none
 *
 * Output:
 * none
 *
 * Return:
 * none
 */
File::File(){
  opened_ = false;
  filesize_ = 0;
  file_pos_ = 0;
}

/*
 * Returns remaining number of bytes in file
 *
 * Inputs:
 * none
 *
 * Output:
 * none
 *
 * Return:
 * number of bytes remaining in file, or -1 if 
 *  file isn't open
 */
int32_t File::available(){
  if(!opened_){
    return -1;
  }
  return (int32_t)filesize_ - file_pos_;
}  

/*
 * Return the next byte from the file
 *
 * Inputs:
 * none
 *
 * Output:
 * none
 *
 * Return:
 * next byte in the file, or -1 if 
 *  file isn't open
 */
char File::read(){
  if(!opened_){
    return -1;
  }
  return filedata_[file_pos_++];
}

/*
 * Return bytes from the file 
 * Note: Could not getting direct reading from the file to
 * work so instead we open() reads the entire file into a 
 * buffer and so this function just returns the desired 
 * bytes from that buffer
 *
 * Inputs:
 * nbyte - number of bytes to read
 *
 * Output:
 * buf - buffer containing bytes read
 *
 * Return:
 * -1 if file isn't open, or 0 otherwise
 */
int16_t File::read(void *buf, uint16_t nbyte){
  if(!opened_){
    return -1;
  }
  // copy the data from the buffer
  memcpy(buf,&filedata_[file_pos_],nbyte);
  
  // increment the location in the file
  file_pos_ += nbyte;
  return 0;
}

/*
 * Opens a file, allocates memory to store it, and reads the file in
 *
 * Inputs:
 * filename - name of file to open and read
 *
 * Output:
 * none
 *
 * Return:
 * 1 if successful, 0 otherwise
 */
bool File::open(const char *filename){
  
  // copy the filename into the member
  strcpy(name_, filename);
  //printf("StubSD: <debug> attempting to open '%s'\n",name_);
  
  // figure out the filesize
  filesize_ = filesize(name_);
  
  if(filesize_ > 512){
    printf("StubSD: <debug> warning, reading in a file with size: %d\n",filesize_);
  }
  
  // allocate memory to hold the file and assign it to the member variable
  filedata_ = (uint8_t*)malloc(filesize_);
  memset(filedata_,filesize_,0);
  
  // open the file
  real_fileobj_.open(filename,std::ifstream::in|std::ios::binary);
  
  if(real_fileobj_.is_open()){
    //printf("StubSD: <debug> '%s' was opened, filesize %d \n",name_,filesize_);
    opened_ = true;
  }
  else{
    //printf("StubSD: <debug> '%s' was not opened\n",filename);
    opened_ = false;
    return false;
  }
  
  // read the file
  for (int i = 0; i < filesize_; i++){
    filedata_[i] = real_fileobj_.get();
  }
  
  /*
  printf("StubSD: <debug> read bytes from file:\n");
  for(int i = 0; i < filesize_; i++){
    printf("0x%x ",filedata_[i]);
  }
  printf("\n");
  */
  
  // close the file
  real_fileobj_.close();
  
  // reset to beginning of file
  file_pos_ = 0;
  
  return true;
}
  
/*
 * Closes the file and release the memory used to buffer it
 *
 * Inputs:
 * none
 *
 * Output:
 * none
 *
 * Return:
 * none
 */
void File::close(){
  free(filedata_);
  opened_ = false;
  filesize_ = 0;
}

/*
 * Gets the size of a file on the local filesystem
 *
 * Inputs:
 * filename - name of file to return size of
 *
 * Output:
 * none
 *
 * Return:
 * size of file (in bytes)
 */
int File::filesize(const char* filename){
  std::ifstream in(filename, std::ifstream::ate | std::ifstream::binary);
  return in.tellg(); 
}
  
class SDClass{
public:
	//SD(){
  //  fake_fileobj = File();
  //}
	File open(const char *filename, uint8_t mode = 0){
    opened_ = fake_fileobj.open(filename);
    return fake_fileobj;
  }
  File close(){
    fake_fileobj.close();
  }
private:
  File fake_fileobj;
  bool opened_;
  //friend class File;
};

SDClass SD;