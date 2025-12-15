//---------------------------------------------------------------------------

#ifndef TMainThreadObjectH
#define TMainThreadObjectH
//---------------------------------------------------------------------------
#include <thread>
#include <atomic>
//---------------------------------------------------------------------------
#include "move.h"
#include "TEngine.h"
#include "TBoardCapture.h"
#include "TBoardRecognize.h"

class TMainThread
{
private:
   std::thread thread;
   std::atomic<bool> terminated;
   void Execute();
protected:
public:
   TMainThread(TEngine* engine);
   ~TMainThread();
   void Start();
   void Stop();
   void ShowResult();
   void MakeBestMove();
   unsigned __int64 iComputerId;
   TEngine *Engine;
   bool last_key_w_state;
   bool last_key_b_state;
   mv_t BestMove;
};
//---------------------------------------------------------------------------
#endif
