//---------------------------------------------------------------------------

#include "TMainThreadObject.h"
#include "TBoardCapture.h"
#include "square.h"
#include <windows.h>
#include <string>
#include <sstream>

void ConvertTimeToString(int time, std::string& s)
{
  time /= 1000;
  int h = time/3600;
  time -= (time/3600)*3600;
  int m = time/60;
  time -= (time/60)*60;
  int sec = time;
  char ss[100];
  sprintf(ss,"%i:%02i.%02i",h,m,sec);
  s = ss;
}

TMainThread::TMainThread(TEngine* engine)
   : Engine(engine), terminated(false)
{
   last_key_w_state = false;
   last_key_b_state = false;
}

TMainThread::~TMainThread()
{
   Stop();
}

void TMainThread::Start()
{
   terminated = false;
   thread = std::thread(&TMainThread::Execute, this);
}

void TMainThread::Stop()
{
   terminated = true;
   if (thread.joinable()) {
      thread.join();
   }
}

//---------------------------------------------------------------------------
void TMainThread::Execute()
{
  int cnt2 = 0;
  int cnt3 = 0;
  while (!terminated) {
    int cnt = 30;
    if (!Engine->BoardRecognize.BoardCapture.Captured)
      cnt = 200;
    if (Engine->AutoPlayWhite && Engine->State.IsWhite() || Engine->AutoPlayBlack && !Engine->State.IsWhite())
      cnt = 1000;
    while (cnt) {
      cnt -= 10;
      Sleep(10);
      BestMove = Engine->TreatThinkResult();
      if (BestMove && Engine->PositionRecognized) {
        MakeBestMove();
        Engine->Tick();
        BestMove = 0;
        break;
      }
      if (++cnt2 >= 10) {
        cnt2 = 0;
        ShowResult();
      }
    }
    if (Engine->move_locked) {
      if (++cnt3 > 50) {
        BestMove = Engine->move_locked;
        MakeBestMove();
        cnt3 = 0;
      }
    }
    Engine->Tick();
  }
}
//---------------------------------------------------------------------------
void TMainThread::MakeBestMove()
{
  Engine->BoardRecognize.BoardCapture.MakeMove(BestMove,Engine->Reversed);
  Engine->State.CommitLastMove();
  if (BestMove != Engine->State.GetLastMove()) {
    undo_t undo;
    move_do(&Engine->State.Board,BestMove,&undo);
    Engine->State.MoveHistory[Engine->State.MoveHistoryLen] = BestMove;
    Engine->State.MoveHistoryLen++;
    Engine->State.LastMove = 0;
    Engine->move_locked = BestMove;
  }
}

void TMainThread::ShowResult()
{
  // No GUI - simplified version
  Engine->UCIInterface->ShowPV();
  
  // Handle keyboard input for autoplay toggles
  bool key_w_state = GetKeyState('W') >> 15;
  if (key_w_state && !last_key_w_state) {
    Engine->AutoPlayWhite = !Engine->AutoPlayWhite;
    if (Engine->State.IsWhite() && Engine->BoardRecognize.BoardCapture.Captured)
      Engine->StartNewThink();
  }
  last_key_w_state = key_w_state;

  bool key_b_state = GetKeyState('B') >> 15;
  if (key_b_state && !last_key_b_state) {
    Engine->AutoPlayBlack = !Engine->AutoPlayBlack;
    if (!Engine->State.IsWhite() && Engine->BoardRecognize.BoardCapture.Captured)
      Engine->StartNewThink();
  }
  last_key_b_state = key_b_state;

  // Time adjustment keys
  SHORT key_a = GetKeyState('A');
  if (key_a >> 15)
    Engine->WhitePlayer.AllTime += 1000;

  SHORT key_z = GetKeyState('Z');
  if (key_z >> 15)
    Engine->WhitePlayer.AllTime -= 1000;

  SHORT key_k = GetKeyState('S');
  if (key_k >> 15)
    Engine->BlackPlayer.AllTime += 1000;

  SHORT key_m = GetKeyState('X');
  if (key_m >> 15)
    Engine->BlackPlayer.AllTime -= 1000;
}


