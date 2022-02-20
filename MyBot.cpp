//MAX is black is 1

#include "Othello.h"
#include "OthelloBoard.h"
#include "OthelloPlayer.h"
#include <cstdlib>
#include <bits/stdc++.h>
using namespace std;
using namespace Desdemona;

#define INTMAX 1000000
#define INTMIN -1000000
class Node {
public:
	bool type;
	int height = 0;
	OthelloBoard Board;
};

clock_t start_t,end_t;
bool flag = false;
Turn MAXNODE;
Turn MINNODE;
class MyBot: public OthelloPlayer
{
    public:
        MyBot( Turn turn );
        virtual Move play( const OthelloBoard& board );
    private:
};

MyBot::MyBot( Turn turn )
	: OthelloPlayer( turn )
{
}
float calxsquare(OthelloBoard board) {
	int bl=0,rd=0;
	if(board.get(0,0) == EMPTY) {
		for(int i=0;i<2;i++) {
			for(int j=0;i<2;i++) {
				if(i!=0 || j!=0) {
					if(board.get(i,j) == BLACK)
						bl++;
					else if(board.get(i,j) == RED)
						rd++;
				}
			}	
		}
	}
	if(board.get(0,7) == EMPTY) {
		for(int i=0;i<2;i++) {
			for(int j=6;i<8;i++) {
				if(i!=0 || j!= 7) {
					if(board.get(i,j) == BLACK)
						bl++;
					else if(board.get(i,j) == RED)
						rd++;
				}
			}
		}
	}
	if(board.get(7,0) == EMPTY) {
		for(int i=6;i<8;i++) {
			for(int j=0;i<2;i++) {
				if(i!=7 || j!=0) {
					if(board.get(i,j) == BLACK)
						bl++;
					else if(board.get(i,j) == RED)
						rd++;
				}
			}
		}
	}
	if(board.get(7,7) == EMPTY) {
		for(int i=6;i<8;i++) {
			for(int j=6;i<8;i++) {
				if(i!=7 || j!=7) {
					if(board.get(i,j) == BLACK)
						bl++;
					else if(board.get(i,j) == RED)
						rd++;
				}
			}
		}
	}
	return (float)(-12.5)*(rd-bl);
}
float HEURISTIC(OthelloBoard board,bool turnn) {
		float a,b,c,d;
		if(board.getRedCount() > board.getBlackCount())
			a = 100*(float)(board.getRedCount())/(float)(board.getRedCount()+board.getBlackCount());
		else if(board.getRedCount() < board.getBlackCount())
			a = -100*(float)(board.getBlackCount())/(float)(board.getRedCount()+board.getBlackCount());
		else
			a = 0;
		
		int bl = 0,rd = 0;
		if(board.get(0,0) == BLACK)
			bl++;
		if(board.get(0,7) == BLACK)
			bl++;
		if(board.get(7,7) == BLACK)
			bl++;
		if(board.get(7,0) == BLACK)
			bl++;
		if(board.get(0,0) == RED)
			rd++;
		if(board.get(0,7) == RED)
			rd++;
		if(board.get(7,7) == RED)
			rd++;
		if(board.get(7,0) == RED)
			rd++;
		
		b = 25*(float)(rd-bl);
		list <Move> moves1 = board.getValidMoves(RED);
		list <Move> moves2 = board.getValidMoves(BLACK);
		if(moves1.size() > moves2.size())
			c = 100*(float)(moves1.size())/(float)(moves1.size()+moves2.size());
		else if(moves1.size() < moves2.size())
			c = -100*(float)(moves2.size())/(float)(moves1.size()+moves2.size());
		else
			c = 0;
		d = 0;
		d = calxsquare(board);
	if(!turnn) {
		return (10*a+801.724*b+78.922*c+382.026*d);
	}
	else {
		return -(10*a+801.724*b+78.922*c+382.026*d);	
	}
}
float alphabeta(Node N,float alpha,float beta,int h) {
	end_t = clock();
	float tot = (float)(end_t-start_t)/CLOCKS_PER_SEC;
	if(tot > 1.95) {
		flag = true;
		if(MAXNODE == BLACK)
			return alpha;
		else
			return beta;
	}
		
	if(N.height == h) {
		TERMINAL:
		if(MAXNODE == BLACK)
			return HEURISTIC(N.Board,1);
		else
			return HEURISTIC(N.Board,0); 
	}
	else {
		if(N.type == 1) {
			OthelloBoard B = N.Board;
			list<Move> moves = B.getValidMoves(MAXNODE);
			list<Move>::iterator it;
			if (moves.size() == 0)
				goto TERMINAL;
			for(it = moves.begin();it!=moves.end();++it) {
				B.makeMove(MAXNODE,*it);
				Node n1;
				n1.Board = B;
				n1.type = 0;
				n1.height = N.height+1;
				alpha = max(alpha,alphabeta(n1,alpha,beta,h));
				if(alpha >= beta)
					return beta;
				B = N.Board;
			}
			return alpha;
		}
		else {
			OthelloBoard B = N.Board;
			list<Move> moves = B.getValidMoves(MINNODE);
			list<Move>::iterator it;
			if (moves.size() == 0)
				goto TERMINAL;
			for(it = moves.begin();it!=moves.end();++it) {
				B.makeMove(MINNODE,*it);
				Node n1;
				n1.Board = B;
				n1.type = 1;
				n1.height = N.height+1;
				beta = min(beta,alphabeta(n1,alpha,beta,h));
				if(alpha >= beta)
					return alpha;
				B = N.Board;
			}
			return beta;
		}
	}
	
}
bool checkcornermove(Move m) {
	
	if(m.x == 0 && m.y == 0)
		return true;
	else if(m.x == 0 && m.y == 7)
		return true;
	else if(m.x == 7 && m.y == 0)
		return true;
	else if(m.x == 7 && m.y == 7)
		return true;
	return false;
}
Move MyBot::play(const OthelloBoard& board)
{
	flag = false;
	srand(time(NULL));
	start_t = clock();
    list<Move> moves = board.getValidMoves(turn);
    list<Move>::iterator it;
    list<Move>::iterator ITmax = moves.begin();
    OthelloBoard btemp = board;
    float maxval = INTMIN;
    if(turn == BLACK) {
    	MAXNODE = BLACK;
    	MINNODE = RED;
    }
    else {
    	MAXNODE = RED;
    	MINNODE = BLACK;
    }
    int ht;
    float alpha = INTMIN;
    float beta = INTMAX;
    for(ht = 4;ht<8;ht++) {
    	alpha = INTMIN;
    	ITmax = moves.begin();
    	maxval = INTMIN;

    	for(it = moves.begin();it!=moves.end();++it) {
        	btemp.makeMove(turn,*it);
        	if(checkcornermove(*it))
        		return *it;
        	float ans;
        	Node n1;
        	n1.height = 0;
        	n1.type = 0;
        	n1.Board = btemp;	
        	ans = alphabeta(n1,alpha,beta,ht);
        	alpha = max(ans,alpha);
        	if(ans > maxval) {
        		maxval = ans;
        		ITmax = it;
        	}
        	if(flag == true)
        		return *ITmax;
     
        	btemp = board;
		}
	}
    return *ITmax;
}

extern "C" {
    OthelloPlayer* createBot( Turn turn )
    {
        return new MyBot( turn );
    }

    void destroyBot( OthelloPlayer* bot )
    {
        delete bot;
    }
}


