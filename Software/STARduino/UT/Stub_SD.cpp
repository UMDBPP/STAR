std::ifstream real_fileobj_;

class File{
public:
	File(){
    opened_ = false;
  }
	int32_t available(){
    if(!opened_){
      return -1;
    }
    // get current pos
    std::streampos curr_pos = real_fileobj_.tellg();
    printf("StubSD: <debug> curr_pos: %d\n",curr_pos);
    printf("StubSD: <debug> filesize: %d\n",filesize_);
    printf("StubSD: <debug> available bytes: %d\n",(uint32_t) filesize_-curr_pos);
    return (uint32_t)filesize_-curr_pos;
  }  
  char read(){
    return real_fileobj_.get();
  }
  int16_t read(void *buf, uint16_t nbyte){
    if(!opened_){
      return -1;
    }
    real_fileobj_.get((char*)buf, nbyte);
    return 0;
  }
  operator bool(){
    return opened_;
  }
  void close(){
    real_fileobj_.close();
    opened_ = false;
    filesize_ = 0;
  }
  bool is_open(){
    return opened_;
  }
  bool open(const char *filename){
    if(strlen(filename) > 30){
      printf("StubSD: <debug> Filename '%s' is longer than 13 char, not opening\n",filename);
      return false;
    }
    memcpy(name_, filename, strlen(filename));
    filesize_ = filesize(name_);
    printf("StubSD: <debug> filesize: %d\n",filesize_);
    printf("StubSD: <debug> Opening '%s'\n",filename);
    real_fileobj_.open(filename,std::ifstream::in|std::ios::binary);
    if(real_fileobj_.is_open()){
      printf("StubSD: <debug> '%s' was opened\n",name_);
      opened_ = true;
    }
    else{
      printf("StubSD: <debug> '%s' was not opened\n",filename);
    }
    return real_fileobj_.is_open();
  }
  int filesize(const char* filename)
  {
    std::ifstream in(filename, std::ifstream::ate | std::ifstream::binary);
    return in.tellg(); 
  }
 private:
  char name_[30]; // our name
  bool opened_;
  int filesize_;
  

};

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