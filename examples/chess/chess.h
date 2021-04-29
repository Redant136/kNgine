#pragma once

// #define noMoveCheck
#define utils_VectorObjects
// my own engine I buid, is used here to render stuff.
// it's not made for board games so it redraws frame from scratch.
#include <kNgine/kNgine.hpp>

#include <string>
#include <vector>
#include <functional>
#include <cmath>
#include <iostream>
#include <stdlib.h>

// im lazy so i have macros to print stuff
#define print(x) \
  std::cout << x << std::endl;
#define printv2(a) \
  std::cout << a.x << " " << a.y << std::endl;
#define printv3(a) \
  std::cout << a.x << " " << a.y << " " << a.z << std::endl;
#define printv4(a) \
  std::cout << a.x << " " << a.y << " " << a.z << " " << a.w << std::endl;

// foward declaration of objects
class board;
class EngineChessPiece;
class chessPiece;

static std::vector<kiv2> pawnMoveFunc(board *bd, chessPiece *piece);
static std::vector<kiv2> bishopMoveFunc(board *bd, chessPiece *piece);
static std::vector<kiv2> knightMoveFunc(board *bd, chessPiece *piece);
static std::vector<kiv2> rookMoveFunc(board *bd, chessPiece *piece);
static std::vector<kiv2> queenMoveFunc(board *bd, chessPiece *piece);
static std::vector<kiv2> kingMoveFunc(board *bd, chessPiece *piece);

// left to main to initialize
extern kNgine::SpriteMap *spriteMap;
extern bool paused;
extern board *chessBoard;
extern std::vector<EngineChessPiece *> enginePieces;

// piece type(pawn,bishop,etc)
enum pieceType
{
  P, //pawn
  B, //bishop
  N, //knight
  R, //rook
  Q, //queen
  K  //king
};
char getPieceCode(pieceType p)
{
  switch (p)
  {
  case pieceType::P:
    return 'p';
  case pieceType::B:
    return 'b';
  case pieceType::N:
    return 'n';
  case pieceType::R:
    return 'r';
  case pieceType::Q:
    return 'q';
  case pieceType::K:
    return 'k';
  default:
    return 0;
  }
}
unsigned int getPieceScore(pieceType p)
{
  switch (p)
  {
  case pieceType::P:
    return 1;
  case pieceType::B:
    return 3;
  case pieceType::N:
    return 3;
  case pieceType::R:
    return 5;
  case pieceType::Q:
    return 9;
  case pieceType::K:
    return 99999;
  default:
    return 0;
  }
}

// a chess piece, difference between types is move func
class chessPiece
{
public:
  std::vector<kiv2> (*possMoves)(board *, chessPiece *);
  //std::function<std::vector<kiv2>(board *, chessPiece *)> possMoves;
  pieceType type;
  bool isWhite;
  kiv2 pos;
  bool hasMoved = false;
  bool eaten = false;
  chessPiece(pieceType type, bool isWhite, kiv2 pos, std::vector<kiv2> (*possMoves)(board *, chessPiece *))
  {
    this->possMoves = possMoves;
    this->type = type;
    this->isWhite = isWhite;
    this->pos = pos;
  }
  chessPiece(pieceType type, bool isWhite, kiv2 pos, std::vector<kiv2> (*possMoves)(board *, chessPiece *), bool hasMoved, bool eaten)
  {
    this->possMoves = possMoves;
    this->type = type;
    this->isWhite = isWhite;
    this->pos = pos;
    this->hasMoved = hasMoved;
    this->eaten = eaten;
  }
  std::vector<kiv2> possibleMoves(board *b); // only declared and not defined because board has yet to be defined
};

// object used by engine
class EngineChessPiece : public kNgine::ComponentGameObject
{
public:
  chessPiece *piece;
  bool grabed = false;
  EngineChessPiece(chessPiece *piece) : ComponentGameObject()
  {
    this->piece = piece;
    addComponent(new kNgine::SpriteReferenceComponent(this, spriteMap, piece->type + (piece->isWhite ? 0 : pieceType::K + 1)));
  }
  EngineChessPiece(const EngineChessPiece &base) : ComponentGameObject(base)
  {
    this->piece = base.piece;
  }
  void update(std::vector<kNgine::msg> msgs);
};

// board object that stores references to all pieces
class board
{
public:
  unsigned int turn = 0;
  std::vector<chessPiece *> pieces;
  u8 boardPieceLocationIndex[64];
  bool turnWhite = true;
  kiv2 lastMovePos, movePos;
  board()
  {
  }
  board(std::vector<chessPiece *> pieces)
  {
    this->pieces = pieces;
    for (u32 i = 0; i < 64; i++)
    {
      boardPieceLocationIndex[i] = 255;
    }
    for (u32 i = 0; i < pieces.size(); i++)
    {
      boardPieceLocationIndex[pieces[i]->pos.y * 8 + pieces[i]->pos.x] = i;
    }
  }
  board(std::string fenNotation)
  {
    for (int i = 0; i < 2; i++)
    {
      bool isWhite = i == 0;
      for (int i = 0; i < 8; i++) // pawn
      {
        chessPiece *piece = new chessPiece(pieceType::P, isWhite, kiv2(-1, -1), pawnMoveFunc);
        pieces.push_back(piece);
      }
      { // bishop
        chessPiece *piece1 = new chessPiece(pieceType::B, isWhite, kiv2(-1, -1), bishopMoveFunc);
        chessPiece *piece2 = new chessPiece(pieceType::B, isWhite, kiv2(-1, -1), bishopMoveFunc);
        pieces.push_back(piece1);
        pieces.push_back(piece2);
      }
      { // knight
        chessPiece *piece1 = new chessPiece(pieceType::N, isWhite, kiv2(-1, -1), knightMoveFunc);
        chessPiece *piece2 = new chessPiece(pieceType::N, isWhite, kiv2(-1, -1), knightMoveFunc);
        pieces.push_back(piece1);
        pieces.push_back(piece2);
      }
      { //rook
        chessPiece *piece1 = new chessPiece(pieceType::R, isWhite, kiv2(-1, -1), rookMoveFunc);
        chessPiece *piece2 = new chessPiece(pieceType::R, isWhite, kiv2(-1, -1), rookMoveFunc);
        pieces.push_back(piece1);
        pieces.push_back(piece2);
      }
      { // queen
        chessPiece *piece = new chessPiece(pieceType::Q, isWhite, kiv2(-1, -1), queenMoveFunc);
        pieces.push_back(piece);
      }
      { // king
        chessPiece *piece = new chessPiece(pieceType::K, isWhite, kiv2(-1, -1), kingMoveFunc);
        pieces.push_back(piece);
      }
    }
    for (u32 i = 0; i < 64; i++)
    {
      boardPieceLocationIndex[i] = 255;
    }

    std::string fenCommands[6];
    u8 currentCommandIndex = 0;
    for (char c : fenNotation)
    {
      if (c == ' ')
      {
        currentCommandIndex++;
      }
      else
      {
        fenCommands[currentCommandIndex] += c;
      }
    }
    // fen arg 0
    {
      u32 numUsedPieces[2][7] = {{0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0}};
      u32 currentX = 0;
      u32 currentY = 7;
      for (char c : fenCommands[0])
      {
        for (u32 i = 0; i <= pieceType::K; i++)
        {
          u32 pIndex = 0;
          bool isPiece = false;
          if (c == getPieceCode((pieceType)i))
          { // black
            isPiece = true;
            pIndex += 16;
            pIndex += numUsedPieces[1][i];
            numUsedPieces[1][i]++;
          }
          else if (c == getPieceCode((pieceType)i) - 32)
          { // white
            isPiece = true;
            pIndex += numUsedPieces[0][i];
            numUsedPieces[0][i]++;
          }
          if (isPiece)
          {
            if (i > pieceType::P)
            {
              pIndex += 8;
            }
            if (i > pieceType::B)
            {
              pIndex += 2;
            }
            if (i > pieceType::N)
            {
              pIndex += 2;
            }
            if (i > pieceType::R)
            {
              pIndex += 2;
            }
            if (i > pieceType::Q)
            {
              pIndex += 1;
            }
            boardPieceLocationIndex[currentY * 8 + currentX] = pIndex;
            pieces[pIndex]->pos = kiv2(currentX, currentY);
            currentX++;
          }
        }
        if (c == '/')
        {
          currentY--;
          currentX = 0;
        }
        else if (c > 48 && c < 57)
        {
          currentX += c - 48;
        }
      }
    }
    for (u32 i = 0; i < pieces.size(); i++)
    {
      if (pieces[i]->pos == kiv2(-1, -1))
      {
        pieces[i]->eaten = true;
      }
    }
    // arg 1
    turnWhite = fenCommands[1].c_str()[0] == 'w';
    // arg 2
    pieces[12]->hasMoved = true;      // rook q
    pieces[13]->hasMoved = true;      // rook k
    pieces[15]->hasMoved = true;      // king
    pieces[12 + 16]->hasMoved = true; // rook q
    pieces[13 + 16]->hasMoved = true; // rook k
    pieces[15 + 16]->hasMoved = true; // king
    for (char c : fenCommands[2])
    {
      if (c == 'K')
      {
        pieces[boardPieceLocationIndex[7 * 8 + 7]]->hasMoved = false;
        pieces[boardPieceLocationIndex[7 * 8 + 4]]->hasMoved = false;
      }
      else if (c == 'Q')
      {
        pieces[boardPieceLocationIndex[7 * 8 + 0]]->hasMoved = false;
        pieces[boardPieceLocationIndex[7 * 8 + 4]]->hasMoved = false;
      }
      else if (c == 'k')
      {
        pieces[boardPieceLocationIndex[0 * 8 + 7]]->hasMoved = false;
        pieces[boardPieceLocationIndex[0 * 8 + 4]]->hasMoved = false;
      }
      else if (c == 'q')
      {
        pieces[boardPieceLocationIndex[0 * 8 + 0]]->hasMoved = false;
        pieces[boardPieceLocationIndex[0 * 8 + 4]]->hasMoved = false;
      }
    }

    // arg 3
    // screw en passant

    // arg 4
    // halfmove advance clock not implemented

    // arg 5
    turn = (((u32)fenCommands[5].c_str()[0] - 48) - 1) * 2;
  }
  board(const board &base)
  {
    this->pieces = base.pieces;
    memcpy(this->boardPieceLocationIndex, base.boardPieceLocationIndex, sizeof(u8) * 64);
  }
  ~board()
  {
    for (int i = 0; i < pieces.size(); i++)
    {
      delete pieces[i];
    }
  }
  chessPiece *pieceAtPos(kiv2 pos)
  {
    // for (int i = 0; i < pieces.size(); i++)
    // {
    //   if (!pieces[i]->eaten && kiv2(pieces[i]->pos) == pos)
    //   {
    //     return pieces[i];
    //   }
    // }
    // return NULL;
    if (boardPieceLocationIndex[pos.y * 8 + pos.x] == 255)
    {
      return NULL;
    }
    if (pos.x < 0 || pos.x > 7)
    {
      return NULL;
    }
    if (pos.y < 0 || pos.y > 7)
    {
      return NULL;
    }

    return pieces[boardPieceLocationIndex[pos.y * 8 + pos.x]];
  }
  int boardScore(bool isWhite)
  {
    int score = 0;
    for (int i = 0; i < pieces.size(); i++)
    {
      if (pieces[i]->eaten)
      {
        if (pieces[i]->isWhite == isWhite)
        {
          if (pieces[i]->type == pieceType::K)
          {
            score += getPieceScore(pieces[i]->type) * -10;
          }
          score += getPieceScore(pieces[i]->type) * -1;
        }
        else
        {
          score += getPieceScore(pieces[i]->type) * 1;
        }
      }
    }
    return score;
  }
  void move(unsigned int index, kiv2 movement)
  {
    move(pieces[index], movement);
  }
  void move(chessPiece *piece, kiv2 movement)
  {
    if (kiv2(piece->pos) != kiv2(movement))
    {
      std::vector<kiv2> moves = piece->possibleMoves(this);
      bool correct = false;
#ifndef noMoveCheck
      for (int i = 0; i < moves.size(); i++)
      {
        if (kiv2(moves[i]) == kiv2(movement))
        {
          correct = true;
        }
      }
#else
      correct = true;
#endif
      if (correct)
      {
        turn++;
        piece->hasMoved = true;
        chessPiece *eat = pieceAtPos(movement);
        if (piece->type == pieceType::K && kiv2(piece->pos).x == 4)
        {
          if (kiv2(movement).x == 2)
          {
            chessPiece *rook = pieceAtPos(kv2(0, piece->pos.y));
            if (rook && rook->type == pieceType::R)
            {
              boardPieceLocationIndex[rook->pos.y * 8 + 3] = boardPieceLocationIndex[rook->pos.y * 8 + rook->pos.x];
              boardPieceLocationIndex[rook->pos.y * 8 + rook->pos.x] = 255;
              rook->pos.x = 3;
            }
            else
            {
              print("error");
              printBoard();
              throw "";
            }
          }
          else if (kiv2(movement).x == 6)
          {
            chessPiece *rook = pieceAtPos(kv2(7, piece->pos.y));
            if (rook && rook->type == pieceType::R)
            {
              boardPieceLocationIndex[rook->pos.y * 8 + 5] = boardPieceLocationIndex[rook->pos.y * 8 + rook->pos.x];
              boardPieceLocationIndex[rook->pos.y * 8 + rook->pos.x] = 255;
              rook->pos.x = 5;
            }
            else
            {
              print("error");
              printBoard();
              throw "";
            }
          }
        }
        else if (piece->type == pieceType::P)
        {
          int lastLine = piece->isWhite ? 7 : 0;
          if (movement.y == lastLine)
          {
            piece->type = pieceType::Q;
            piece->possMoves = queenMoveFunc;
          }
          // check if en passant
          else if ((movement - piece->pos).x != 0)
          { // check if pawn eat
            chessPiece *atPos = pieceAtPos(movePos);
            if (atPos && atPos->type == pieceType::P &&
                (std::abs((movePos - lastMovePos).y) == 2) &&
                (std::abs((movePos - piece->pos).x) == 1))
            {
              eat = atPos;
            }
          }
        }
        lastMovePos = piece->pos;
        movePos = movement;
        if (eat)
        {
          eat->eaten = true;
          boardPieceLocationIndex[eat->pos.y * 8 + eat->pos.x] = 255;
        }
        boardPieceLocationIndex[movement.y * 8 + movement.x] = boardPieceLocationIndex[piece->pos.y * 8 + piece->pos.x];
        boardPieceLocationIndex[piece->pos.y * 8 + piece->pos.x] = 255;
        piece->pos = movement;
        turnWhite = !turnWhite;
      }
    }
  }
  void clone(const board &base)
  {
    pieces = std::vector<chessPiece *>(base.pieces.size());
    for (int i = 0; i < base.pieces.size(); i++)
    {
      pieces[i] = new chessPiece(base.pieces[i]->type, base.pieces[i]->isWhite, base.pieces[i]->pos, base.pieces[i]->possMoves, base.pieces[i]->hasMoved, base.pieces[i]->eaten);
    }
    memcpy(this->boardPieceLocationIndex, base.boardPieceLocationIndex, sizeof(u8) * 64);
    this->turn = base.turn;
    this->turnWhite = base.turnWhite;
  }
  void printBoard()
  {
    for (int y = 7; y >= 0; y--)
    {
      for (int x = 0; x < 8; x++)
      {
        chessPiece *atPos = pieceAtPos(kv2(x, y));
        if (atPos)
        {
          if (atPos->isWhite)
          {
            std::cout << (char)(getPieceCode(atPos->type) - 32);
          }
          else
          {
            std::cout << getPieceCode(atPos->type);
          }
        }
        else
        {
          std::cout << " ";
        }
      }
      std::cout << std::endl;
    }
  }
  bool operator==(const board &b)
  {
    if (b.pieces.size() != pieces.size())
      return false;
    for (int i = 0; i < pieces.size(); i++)
    {
      if (pieces[i]->type != b.pieces[i]->type ||
          pieces[i]->isWhite != b.pieces[i]->isWhite ||
          pieces[i]->pos != b.pieces[i]->pos ||
          pieces[i]->hasMoved != b.pieces[i]->hasMoved ||
          pieces[i]->eaten != b.pieces[i]->eaten)
      {
        return false;
      }
    }
    return true;
  }
};

// object used by engine to render pieces and board
class chessCam : public kNgine::Camera
{
public:
  chessCam(float fov, int width, int height) : Camera(fov, width, height)
  {
  }
  void update(std::vector<kNgine::msg> msgs)
  {
    kNgine::Camera::update(msgs);
    float board_size = std::min(kRenderer_getWindowWidth(), kRenderer_getWindowHeight());
    kRenderer_clear(v4(232.0 / 255, 204.0 / 255, 163.0 / 255, 1));
    for (int x = 0; x < 4; x++)
    {
      for (int y = 0; y < 8; y++)
      {
        kRenderer_setDrawColor(v4(180.0 / 255, 136.0 / 255, 98.0 / 255, 1));
        // kRenderer_drawRectV4(v4(x * board_size / 4.0 + board_size / 8.0 * ((y + 1) % 2), y * board_size / 8.0, board_size / 8.0, board_size / 8.0));
        kRenderer_drawRectV4(v4(x * 0.5 - 1 + 0.25 * ((y) % 2), y * 0.25 - 1, 0.25, 0.25));
      }
    }
    kRenderer_setDrawColor(v4(0, 150 / 255.0, 0, 75 / 255.0));
    kv4 lastMove = kv4(chessBoard->lastMovePos.x, chessBoard->lastMovePos.y, 1, 1) * 0.25 - kv4(1, 1, 0, 0);
    kRenderer_drawRectV4(toV4(lastMove));
    kv4 movePos = kv4(chessBoard->movePos.x, chessBoard->movePos.y, 1, 1) * 0.25 - kv4(1, 1, 0, 0);
    kRenderer_drawRectV4(toV4(movePos));

    kv2 p = {posMapper.min.x + fov / 2, posMapper.max.y - fov / 2};
    p *= -1;
    p += kv2(3.5, 3.5);
    position.x = p.x;
    position.y = p.y;
  }
};

// definition of possible moves
std::vector<kiv2> chessPiece::possibleMoves(board *b)
{
  std::vector<kiv2> res = possMoves(b, this);
  return res;
}

// definition of update. takes care of grabbing piece and displaying possible moves
void EngineChessPiece::update(std::vector<kNgine::msg> msgs)
{
  findComponent<kNgine::SpriteReferenceComponent>("[sprite]")->mapIndex = piece->type + (piece->isWhite ? 0 : pieceType::K + 1);
  if (piece->eaten)
  {
    kNgine::disableObject(this);
  }
  if (!this->enabled)
    this->position = {-1, -1};
  if (grabed)
  {
    // float cellSize = std::min(kRenderer_getWindowWidth(), kRenderer_getWindowHeight()) / 8.0;
    std::vector<kiv2> moves = piece->possibleMoves(chessBoard);
    kRenderer_setDrawColor(v4(255 / 255.0, 0, 0, 75 / 255.0));
    for (int i = 0; i < moves.size(); i++)
    {
      kRenderer_drawRectV4(v4(moves[i].x * 0.25 - 1, (moves[i].y) * 0.25 - 1, 0.25, 0.25));
    }
  }
  else
  {
    this->position.x = piece->pos.x;
    this->position.y = piece->pos.y;
  }
}

// user input and player
class phys_player : public kNgine::EngineObject
{
private:
  bool grabed = false;

public:
  board *b;
  bool color;
  phys_player(bool color, board *b)
  {
    this->color = color;
    this->b = b;
  }
  void update(std::vector<kNgine::msg> msgs)
  {
    if (b->turnWhite == color)
    {
      float windowSize = std::min(kRenderer_getWindowSize().x, kRenderer_getWindowSize().y);
      v2 cursor;
      bool hasClick = false;
      for (int i = 0; i < msgs.size(); i++)
      {
        if (msgs[i].msgType == kNgine::msg::CURSOR)
        {
          cursor = msgs[i].cursorPos;
        }
        else if (msgs[i].msgType == kNgine::msg::NONASCII_KEY)
        {
          if (msgs[i].key & KeyBitmap(Key::MOUSE_LEFT))
          {
            hasClick = true;
            kiv2 newPos = kiv2(std::floor(cursor.x * 4 + 4), std::floor(cursor.y * 4 + 4));
            for (int j = 0; j < b->pieces.size(); j++)
            {
              if (b->pieces[j]->isWhite == color && !b->pieces[j]->eaten && !grabed)
              {
                if (newPos == b->pieces[j]->pos)
                {
                  grabed = true;
                  enginePieces[j]->grabed = true;
                }
              }
              if (enginePieces[j]->grabed)
              {
                enginePieces[j]->position.x = newPos.x;
                enginePieces[j]->position.y = newPos.y;
              }
            }
          }
        }
      }
      if (!hasClick)
      {
        for (int i = 0; i < enginePieces.size(); i++)
        {
          if (enginePieces[i]->grabed)
          {
            drop(i);
          }
          grabed = false;
        }
      }
    }
  }

  void drop(unsigned int index)
  {
    enginePieces[index]->grabed = false;
    b->move(index, kiv2(kv2(enginePieces[index]->position)));
    kNgine::callEvent("chess_tock");
    // b->printBoard();
    // print(b->boardScore(true));
  }
};

// bot player
class bot_player : public kNgine::EngineObject
{
protected:
  u64 totalMoves;
  bool done = true;
  // recusivly called func to decide best move. uses minimax algorithm with alpha beta prunning
  virtual f32 recusiveMove(bool color, int iterationsLeft, board *b1,
                           int alpha = INT_MIN, int beta = INT_MAX)
  {
    float numMovesW = 0.f;
    if (iterationsLeft <= 0)
    {
      totalMoves++;
      return boardScore(b1);
    }
    std::vector<iv3> moves;
    for (int i = 0; i < b1->pieces.size(); i++)
    {
      if (b1->pieces[i]->isWhite == color && !b1->pieces[i]->eaten)
      {
        std::vector<kiv2> pMoves = b1->pieces[i]->possibleMoves(b1);
        for (kiv2 m : pMoves)
        {
          moves.push_back(iv3(m.x, m.y, i));
        }
      }
    }

    int res = (color == this->color) ? INT_MIN : INT_MAX;
    int index = -1;
    for (int i = 0; i < moves.size(); i++)
    {
      f32 score;
      board nBoard = board();
      nBoard.clone(*b1);
      nBoard.move((u32)moves[i].z, kiv2(moves[i].x,moves[i].y));
      bool kingDead = false;
      score = recusiveMove(!color, iterationsLeft - 1, &nBoard, alpha, beta);
      if (color == this->color)
      {
        if (res < score)
        {
          res = score;
          index = i;
        }
        alpha = std::max(alpha, res);
      }
      else
      {
        if (res > score)
        {
          res = score;
          index = i;
        }
        beta = std::min(beta, res);
      }

      if (beta <= alpha)
      {
        break;
      }
    }
    if (iterationsLeft == precision)
    {
      if (index == -1)
      {
        print("index -1");
        b1->printBoard();
        // b1->pieces[15+((this->color)?0:16)]->eaten = true;
        // b1->turnWhite=!b1->turnWhite;
        assert(index != -1);
        assert(0);
      }
      b1->move(moves[index].z, kiv2(moves[index].x,moves[index].y));
      this->positionScores[b1->pieces[moves[index].z]->type][(u32)(moves[index].y * 8 + moves[index].x)] *= 0.8;
    }
    return res + moves.size() * numMovesW;
  }

public:
  f32 positionScores[6][64];
  bool asyncMove = true;
  board *b;
  bool color;
  unsigned int precision = 4;
  bot_player(bool color, board *b)
  {
    this->color = color;
    this->b = b;

    f32 pawn_pS[] = {
        7.0, 7.0, 7.0, 7.0, 7.0, 7.0, 7.0, 7.0,
        5.0, 5.0, 5.0, 5.0, 5.0, 5.0, 5.0, 5.0,
        1.0, 1.0, 2.0, 3.0, 3.0, 2.0, 1.0, 1.0,
        0.5, 0.5, 1.0, 2.5, 2.5, 1.0, 0.5, 0.5,
        0.0, 0.0, 0.0, 1.5, 2.0, 0.0, 0.0, 0.0,
        0.5, -0.5, -0.5, 0.0, 0.0, -0.5, -0.5, 0.5,
        0.5, 1.0, 1.0, -1.0, -1.5, 1.0, 1.0, 0.5,
        0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
    f32 bishop_pS[] = {
        -2.0, -1.0, -1.0, -1.0, -1.0, -1.0, -1.0, -2.0,
        -1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, -1.0,
        -1.0, 0.0, 0.5, 1.0, 1.0, 0.5, 0.0, -1.0,
        -1.0, 0.5, 0.5, 1.0, 1.0, 0.5, 0.5, -1.0,
        -1.0, 0.0, 1.0, 1.0, 1.0, 1.0, 0.0, -1.0,
        -1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, -1.0,
        -1.0, 0.5, 0.0, 0.0, 0.0, 0.0, 0.5, -1.0,
        -2.0, -1.0, -1.0, -1.0, -1.0, -1.0, -1.0, -2.0};
    f32 knight_pS[] = {
        -5.0, -4.0, -3.0, -3.0, -3.0, -3.0, -4.0, -5.0,
        -4.0, -2.0, 0.0, 0.0, 0.0, 0.0, -2.0, -4.0,
        -3.0, 0.0, 1.0, 1.5, 1.5, 1.0, 0.0, -3.0,
        -3.0, 0.5, 1.5, 2.0, 2.0, 1.5, 0.5, -3.0,
        -3.0, 0.0, 1.5, 2.0, 2.0, 1.5, 0.0, -3.0,
        -3.0, 0.5, 1.5, 1.5, 1.5, 1.0, 0.5, -3.0,
        -4.0, -2.0, 0.0, 0.5, 0.5, 0.0, -2.0, -4.0,
        -5.0, -4.0, -3.0, -3.0, -3.0, -3.0, -3.0, -5.0};
    f32 rook_pS[] = {
        0.5, 0.5, 0.5, 0.0, 0.0, 0.5, 0.5, 0.5,
        0.5, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 0.5,
        -0.5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, -0.5,
        -0.5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, -0.5,
        -0.5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, -0.5,
        -0.5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, -0.5,
        -0.5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, -0.5,
        1.5, -1.5, 0.0, 0.5, 0.5, 0.0, -1.5, 1.5};
    f32 queen_pS[] = {
        -2.0, -1.0, -1.0, -0.5, -0.5, -1.0, -1.0, -2.0,
        -1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, -1.0,
        -1.0, 0.0, 0.5, 0.5, 0.5, 0.5, 0.0, -1.0,
        -0.5, 0.0, 0.5, 0.5, 0.5, 0.5, 0.0, -0.5,
        0.0, 0.0, 0.5, 0.5, 0.5, 0.5, 0.0, -0.5,
        -1.0, 0.5, 0.5, 0.5, 0.5, 0.0, 0.0, -1.0,
        -1.0, 0.0, 0.5, 0.0, 0.0, 0.0, 0.0, -1.0,
        -2.0, -1.0, -1.0, -0.5, -0.5, -1.0, -1.0, -2.0};
    f32 king_pS[] = {
        -3.0, -4.0, -4.0, -5.0, -5.0, -4.0, -4.0, -3.0,
        -3.0, -4.0, -4.0, -5.0, -5.0, -4.0, -4.0, -3.0,
        -3.0, -4.0, -4.0, -5.0, -5.0, -4.0, -4.0, -3.0,
        -3.0, -4.0, -4.0, -5.0, -5.0, -4.0, -4.0, -3.0,
        -2.0, -3.0, -3.0, -4.0, -4.0, -3.0, -3.0, -2.0,
        -1.0, -2.0, -2.0, -2.0, -2.0, -2.0, -2.0, -1.0,
        2.0, 2.0, 0.0, 0.0, 0.0, 0.0, 2.0, 2.0,
        2.0, 2.0, 2.0, -1.0, 0.0, -3.0, 3.0, 2.0};

    if (!color)
    {
      memcpy(positionScores[0], pawn_pS, sizeof(f32) * 64);
      memcpy(positionScores[1], bishop_pS, sizeof(f32) * 64);
      memcpy(positionScores[2], knight_pS, sizeof(f32) * 64);
      memcpy(positionScores[3], rook_pS, sizeof(f32) * 64);
      memcpy(positionScores[4], queen_pS, sizeof(f32) * 64);
      memcpy(positionScores[5], king_pS, sizeof(f32) * 64);
    }
    else
    {
      for (u32 y = 0; y < 8; y++)
      {
        for (u32 x = 0; x < 8; x++)
        {
          positionScores[0][y * 8 + x] = pawn_pS[(7 - y) * 8 + x];
          positionScores[1][y * 8 + x] = bishop_pS[(7 - y) * 8 + x];
          positionScores[2][y * 8 + x] = knight_pS[(7 - y) * 8 + x];
          positionScores[3][y * 8 + x] = rook_pS[(7 - y) * 8 + x];
          positionScores[4][y * 8 + x] = queen_pS[(7 - y) * 8 + x];
          positionScores[5][y * 8 + x] = king_pS[(7 - y) * 8 + x];
        }
      }
    }

    for (u32 i = 0; i < 6; i++)
    {
      for (u32 j = 0; j < 64; j++)
      {
        if (positionScores[i][j] < -(1 << 8) || positionScores[i][j] > 1 << 8)
        {
          print(color);
          print(i << " " << j);
          print(positionScores[i][j]);
          assert(0);
        }
      }
    }
  }
  void update(std::vector<kNgine::msg> msgs)
  {
    if (!paused && b->turnWhite == color && done)
    {
      done = false;
      if (asyncMove)
      {
        kThreadDetach([this](void *arg) {
          totalMoves = 0;
          recusiveMove(color, precision, b);
          done = true;
          kNgine::callEvent("chess_tock");
        },
                      NULL);
      }
      else
      {
        recusiveMove(color, precision, b);
        done = true;
      }
    }
  }
  virtual float boardScore(board *b)
  {
    return b->boardScore(this->color) + positionScore(b);
  }
  virtual float positionScore(board *b)
  {
    f64 total = 0;
    for (u32 i = 0; i < b->pieces.size(); i++)
    {
      if (!b->pieces[i]->eaten && isnormal(this->positionScores[b->pieces[i]->type][b->pieces[i]->pos.y * 8 + b->pieces[i]->pos.x]))
      {
        if (b->pieces[i]->isWhite == color)
        {
          total += this->positionScores[b->pieces[i]->type][b->pieces[i]->pos.y * 8 + b->pieces[i]->pos.x];
          // this->positionScores[b->pieces[i]->type][b->pieces[i]->pos.y * 8 + b->pieces[i]->pos.x]*=0.99;
        }
      }
    }
    return total;
  }
};

// declaration of all pieces move functions
static std::vector<kiv2> pawnMoveFunc(board *bd, chessPiece *piece)
{
  int dir = piece->isWhite ? 1 : -1;
  std::vector<kiv2> moves;
  chessPiece *atPos = bd->pieceAtPos(piece->pos + kv2(0, dir));
  if (!atPos)
  {
    moves.push_back(piece->pos + kiv2(0, dir));
    if (!piece->hasMoved)
    {
      atPos = bd->pieceAtPos(piece->pos + kiv2(0, dir * 2));
      if (!atPos)
      {
        moves.push_back(piece->pos + kv2(0, dir * 2));
      }
    }
  }
  // en passant
  if (piece->pos.y + dir * 3 == 0 || piece->pos.y + dir * 3 == 7)
  {
    // check if last move was pawn passing beside this with double move foward
    // yes, this is very ugly
    if (bd->movePos.y == piece->pos.y && bd->pieceAtPos(bd->movePos) &&
        (bd->pieceAtPos(bd->movePos)->type == pieceType::P) &&
        (bd->lastMovePos.y == piece->pos.y + dir * 2))
    {
      if (bd->lastMovePos.x == piece->pos.x + 1)
      {
        moves.push_back(piece->pos + kiv2(1, dir));
      }
      if (bd->lastMovePos.x == piece->pos.x - 1)
      {
        moves.push_back(piece->pos + kiv2(-1, dir));
      }
    }
  }
  atPos = bd->pieceAtPos(piece->pos + kv2(1, dir));
  if (atPos && atPos->isWhite != piece->isWhite)
  {
    moves.push_back(piece->pos + kv2(1, dir));
  }
  atPos = bd->pieceAtPos(piece->pos + kv2(-1, dir));
  if (atPos && atPos->isWhite != piece->isWhite)
  {
    moves.push_back(piece->pos + kv2(-1, dir));
  }
  return moves;
}
static std::vector<kiv2> bishopMoveFunc(board *bd, chessPiece *piece)
{

  std::vector<kiv2> move;
  for (int dir = 0; dir < 4; dir++)
  {
    for (int distance = 1; distance < 8; distance++)
    {
      kv2 vel;
      switch (dir)
      {
      case 0:
        vel.x = 1;
        vel.y = 1;
        break;
      case 1:
        vel.x = 1;
        vel.y = -1;
        break;
      case 2:
        vel.x = -1;
        vel.y = -1;
        break;
      case 3:
        vel.x = -1;
        vel.y = 1;
        break;
      default:
        break;
      }
      vel *= distance;
      if ((piece->pos + vel).x < 0 || (piece->pos + vel).y < 0 || (piece->pos + vel).x > 7 || (piece->pos + vel).y > 7)
      {
        break;
      }
      chessPiece *atPos = bd->pieceAtPos(piece->pos + vel);
      if (atPos)
      {
        if (atPos->isWhite != piece->isWhite)
        {
          move.push_back(piece->pos + vel);
          break;
        }
        else
        {
          break;
        }
      }
      else
      {
        move.push_back(piece->pos + vel);
      }
    }
  }
  return move;
}
static std::vector<kiv2> knightMoveFunc(board *bd, chessPiece *piece)
{
  std::vector<kiv2> move;
  for (int dir = 0; dir < 8; dir++)
  {
    kv2 vel;
    switch (dir)
    {
    case 0:
      vel = {2, 1};
      break;
    case 1:
      vel = {2, -1};
      break;
    case 2:
      vel = {1, 2};
      break;
    case 3:
      vel = {1, -2};
      break;
    case 4:
      vel = {-2, 1};
      break;
    case 5:
      vel = {-2, -1};
      break;
    case 6:
      vel = {-1, 2};
      break;
    case 7:
      vel = {-1, -2};
      break;
    default:
      break;
    }
    if (!((piece->pos + vel).x < 0 || (piece->pos + vel).y < 0 || (piece->pos + vel).x > 7 || (piece->pos + vel).y > 7))
    {
      chessPiece *atPos = bd->pieceAtPos(piece->pos + vel);
      if (atPos)
      {
        if (atPos->isWhite != piece->isWhite)
        {
          move.push_back(piece->pos + vel);
        }
      }
      else
      {
        move.push_back(piece->pos + vel);
      }
    }
  }
  return move;
}
static std::vector<kiv2> rookMoveFunc(board *bd, chessPiece *piece)
{
  std::vector<kiv2> move;
  for (int dir = 0; dir < 4; dir++)
  {
    for (int distance = 1; distance < 8; distance++)
    {
      kv2 vel;
      switch (dir)
      {
      case 0:
        vel.x = 1;
        break;
      case 1:
        vel.x = -1;
        break;
      case 2:
        vel.y = 1;
        break;
      case 3:
        vel.y = -1;
        break;
      default:
        break;
      }
      vel *= distance;
      if ((piece->pos + vel).x < 0 || (piece->pos + vel).y < 0 || (piece->pos + vel).x > 7 || (piece->pos + vel).y > 7)
      {
        break;
      }
      chessPiece *atPos = bd->pieceAtPos(piece->pos + vel);
      if (atPos)
      {
        if (atPos->isWhite != piece->isWhite)
        {
          move.push_back(piece->pos + vel);
          break;
        }
        else
        {
          break;
        }
      }
      else
      {
        move.push_back(piece->pos + vel);
      }
    }
  }
  return move;
}
static std::vector<kiv2> queenMoveFunc(board *bd, chessPiece *piece)
{
  std::vector<kiv2> move;
  for (int dir = 0; dir < 8; dir++)
  {
    for (int distance = 1; distance < 8; distance++)
    {
      kv2 vel;
      switch (dir)
      {
      case 0:
        vel = {1, 0};
        break;
      case 1:
        vel = {1, 1};
        break;
      case 2:
        vel = {0, 1};
        break;
      case 3:
        vel = {-1, 1};
        break;
      case 4:
        vel = {-1, 0};
        break;
      case 5:
        vel = {-1, -1};
        break;
      case 6:
        vel = {0, -1};
        break;
      case 7:
        vel = {1, -1};
        break;
      default:
        break;
      }
      vel *= distance;
      if ((piece->pos + vel).x < 0 || (piece->pos + vel).y < 0 || (piece->pos + vel).x > 7 || (piece->pos + vel).y > 7)
      {
        break;
      }
      chessPiece *atPos = bd->pieceAtPos(piece->pos + vel);
      if (atPos)
      {
        if (atPos->isWhite != piece->isWhite)
        {
          move.push_back(piece->pos + vel);
          break;
        }
        else
        {
          break;
        }
      }
      else
      {
        move.push_back(piece->pos + vel);
      }
    }
  }
  return move;
}
static std::vector<kiv2> kingMoveFunc(board *bd, chessPiece *piece)
{
  std::vector<kiv2> move;
  for (int dir = 0; dir < 8; dir++)
  {
    for (int distance = 1; distance < 2; distance++)
    {
      kv2 vel;
      switch (dir)
      {
      case 0:
        vel = {1, 0};
        break;
      case 1:
        vel = {1, 1};
        break;
      case 2:
        vel = {0, 1};
        break;
      case 3:
        vel = {-1, 1};
        break;
      case 4:
        vel = {-1, 0};
        break;
      case 5:
        vel = {-1, -1};
        break;
      case 6:
        vel = {0, -1};
        break;
      case 7:
        vel = {1, -1};
        break;
      default:
        break;
      }
      vel *= distance;
      if ((piece->pos + vel).x < 0 || (piece->pos + vel).y < 0 || (piece->pos + vel).x > 7 || (piece->pos + vel).y > 7)
      {
        break;
      }
      chessPiece *atPos = bd->pieceAtPos(piece->pos + vel);
      if (atPos)
      {
        if (atPos->isWhite != piece->isWhite)
        {
          move.push_back(piece->pos + vel);
          break;
        }
        else
        {
          break;
        }
      }
      else
      {
        move.push_back(piece->pos + vel);
      }
    }
  }
  if (!piece->hasMoved)
  {
    chessPiece *lRook = bd->pieceAtPos(kiv2(0, piece->pos.y));
    if (lRook && !lRook->hasMoved && !lRook->eaten)
    {
      bool correct = true;
      int targetX = 2;
      for (int x = targetX; x <= piece->pos.x; x++)
      {
        for (int i = 0; i < bd->pieces.size(); i++)
        {
          if (x != lRook->pos.x && x != piece->pos.x && kiv2(x, piece->pos.y) == kiv2(bd->pieces[i]->pos))
          {
            correct = false;
            break;
          }
          else
          {
            if (bd->pieces[i]->isWhite != piece->isWhite && bd->pieces[i]->type != pieceType::K)
            {
              for (kiv2 move : bd->pieces[i]->possibleMoves(bd))
              {
                if (kiv2(x, piece->pos.y) == move)
                {
                  correct = false;
                  break;
                }
              }
            }
          }
        }
      }
      if (correct)
      {
        move.push_back(kiv2(targetX, piece->pos.y));
      }
    }
    chessPiece *rRook = bd->pieceAtPos(kiv2(7, piece->pos.y));
    if (rRook && !rRook->hasMoved && !rRook->eaten)
    {
      bool correct = true;
      int targetX = 6;
      for (int x = piece->pos.x; x <= targetX; x++)
      {
        for (int i = 0; i < bd->pieces.size(); i++)
        {
          if (x != rRook->pos.x && x != piece->pos.x && kiv2(x, piece->pos.y) == kiv2(bd->pieces[i]->pos))
          {
            correct = false;
            break;
          }
          else
          {
            if (bd->pieces[i]->isWhite != piece->isWhite && bd->pieces[i]->type != pieceType::K)
            {
              for (kiv2 move : bd->pieces[i]->possibleMoves(bd))
              {
                if (kiv2(x, piece->pos.y) == move || kiv2(x, piece->pos.y) == kiv2(bd->pieces[i]->pos))
                {
                  correct = false;
                  break;
                }
              }
            }
          }
        }
      }
      if (correct)
      {
        move.push_back(kiv2(targetX, piece->pos.y));
      }
    }
  }
  return move;
}
