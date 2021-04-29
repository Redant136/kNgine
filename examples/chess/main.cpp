#include "chess.h"

kNgine::SpriteMap *spriteMap;
bool paused = false;
board *chessBoard;
std::vector<EngineChessPiece *> enginePieces;

int main(int argc, const char **argv)
{
  std::vector<chessPiece *> pieces = std::vector<chessPiece *>();
  kNgine::window_name = "chess";
  kNgine::window_size = {600, 600};
  chessCam *cam = new chessCam(8, 600, 600);
  cam->fovType = kNgine::Camera::MIN_WH;
  cam->position = v3(3.5, 3.5, 0);
  kNgine::addObject(cam);

  { //setup spr map
    spriteMap = new kNgine::SpriteMap();
    for (int i = pieceType::P; i <= pieceType::K; i++)
    {
      spriteMap->list.push_back(kNgine::importSprite((std::string("chess_pieces/Chess_") + (char)tolower(getPieceCode((pieceType)i)) + "lt60.png").c_str()));
    }
    for (int i = pieceType::P; i <= pieceType::K; i++)
    {
      spriteMap->list.push_back(kNgine::importSprite((std::string("chess_pieces/Chess_") + (char)tolower(getPieceCode((pieceType)i)) + "dt60.png").c_str()));
    }
    kNgine::addObject(spriteMap);
  }
  chessBoard = new board("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");

  { // pieces
    pieces = chessBoard->pieces;
    for (int i = 0; i < pieces.size(); i++)
    {
      enginePieces.push_back(new EngineChessPiece(pieces[i]));
      kNgine::addObject(enginePieces[i]);
    }
  }

  kNgine::addObject(new bot_player(true, chessBoard));
  kNgine::addObject(new phys_player(false, chessBoard));

  kNgine::AudioEngine *audio = new kNgine::AudioEngine();
  kNgine::addObject(audio);
  kNgine::addEvent({"chess_tock", [audio](void *arg) -> void * {
                      audio->play("chess_tock.wav", kNgine::audiofiletype::wav);
                      return NULL;
                    }});

  kNgine::start();
  kNgine::cleanup();
  delete chessBoard;
  return 0;
}
