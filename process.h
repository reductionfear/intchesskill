//---------------------------------------------------------------------------

#ifndef processH
#define processH
//---------------------------------------------------------------------------
#include <windows.h>
#include <string>

class TProcess {
public:
  void Send(const std::string& Command);
  bool Get(char *buf);
  bool IsActive();
  TProcess(const std::string& FileName);
  ~TProcess();
private:
  HANDLE write_stdin, read_stdout;
};

#endif
