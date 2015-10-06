
/* Game implementation
*/

#include <iostream>
#include <assert.h>

#include "Game.h"

#ifndef min
#define min(a,b) (a<b?a:b)
#endif

#ifndef max
#define max(a,b) (a>b?a:b)
#endif

using namespace std;


/* Game state */

Game::BoardDescription::BoardDescription(int rows, int columns, int aligned)
{
	this->rows = rows;
	this->columns = columns;
	this->aligned = aligned;

	nbCase = rows * columns;

	int c = max(0, columns - aligned + 1);
	int r = max(0, rows - aligned + 1);

	nbAlignement = c * rows + r * columns + c * r * 2;

	nbCaseAlignement = nbAlignement * aligned;

	cerr << "game: nbAlignement = " << nbAlignement << endl;

	tabCaseFromAlignement = new int[nbCaseAlignement];
	alignementFromCase = new vector<int>[nbCase];

	int nCaseAlignement = 0;
	int nAlignement = 0;
	int caseIndex, idx, i;

	// tab construction
	for (int y = 0; y < rows; y++) {
		for (int x = 0; x < columns; x++) {
			caseIndex = x + y * columns;
			// horizontal
			if (x < c) {
				for (i = 0; i < aligned; i++) {
					idx = caseIndex + i;
					tabCaseFromAlignement[nCaseAlignement++] = idx;
					alignementFromCase[idx].push_back(nAlignement);
				}
				nAlignement++;
			}
			// vertical
			if (y < r) {
				for (i = 0; i < aligned; i++) {
					idx = caseIndex + i * columns;
					tabCaseFromAlignement[nCaseAlignement++] = idx;
					alignementFromCase[idx].push_back(nAlignement);
				}
				nAlignement++;
			}
			// diag down right
			if (x < c && y < r) {
				for (i = 0; i < aligned; i++) {
					idx = caseIndex + i * (columns + 1);
					tabCaseFromAlignement[nCaseAlignement++] = idx;
					alignementFromCase[idx].push_back(nAlignement);
				}
				nAlignement++;
			}
			// diag down left
			if (x >= aligned - 1 && y < r) {
				for (i = 0; i < aligned; i++) {
					idx = caseIndex + i * (columns - 1);
					tabCaseFromAlignement[nCaseAlignement++] = idx;
					alignementFromCase[idx].push_back(nAlignement);
				}
				nAlignement++;
			}
		}
	}
	assert(nAlignement == nbAlignement);
	assert(nCaseAlignement == nbCaseAlignement);
}

Game::BoardDescription::~BoardDescription()
{
	delete[] alignementFromCase;
	delete[] tabCaseFromAlignement;
}

int Game::BoardDescription::CaseFromAlignement(int algtIndex, int i)
{
	assert(algtIndex >= 0 && algtIndex < nbAlignement);
	const int p = algtIndex * aligned + i;
	assert(p >= 0 && p < nbCaseAlignement);
	return tabCaseFromAlignement[p];
}

int* Game::BoardDescription::CaseArrayFromAlignement(int algtIndex)
{
	assert(algtIndex >= 0 && algtIndex < nbAlignement);
	return tabCaseFromAlignement + algtIndex;
}


/* Player state */

Game::PlayerState::PlayerState(BoardDescription *desc)
{
	this->boardDesc = desc;
	alignementState = new int[boardDesc->nbAlignement];
	nbAlignementDone = new int[boardDesc->aligned + 1];
}

Game::PlayerState::~PlayerState()
{
	delete[] alignementState;
	delete[] nbAlignementDone;
}

void Game::PlayerState::Reset()
{
	fill(alignementState, alignementState + boardDesc->nbAlignement, 0);
	nbAlignementDone[0] = boardDesc->nbAlignement;
	fill(nbAlignementDone + 1, nbAlignementDone + boardDesc->aligned, 0);
}

void Game::PlayerState::PlayAlignement(int algnt)
{
	assert(algnt >= 0 && algnt < boardDesc->nbAlignement);

	int *a = alignementState + algnt;

	assert(*a >= 0 && *a < boardDesc->aligned);
	assert(nbAlignementDone[*a] > 0);

	nbAlignementDone[*a]--;
	(*a)++;
	if (nbAlignementDone[*a]++ == boardDesc->aligned) alignementCompleted = algnt;
}

void Game::PlayerState::LooseAlignement(int algnt)
{
	assert(algnt >= 0 && algnt < boardDesc->nbAlignement);
	int *a = alignementState + algnt;
	nbAlignementDone[*a]--;
	*a = -1; // disable alignement with -1
}

int Game::PlayerState::NbAlignementDone(int nbDone)
{
	assert(nbDone >= 0 && nbDone <= boardDesc->aligned);
	return nbAlignementDone[nbDone];
}

int Game::PlayerState::AlignementState(int algnt)
{
	assert(algnt >= 0 && algnt < boardDesc->nbAlignement);
	return alignementState[algnt];
}

bool Game::PlayerState::HasWon()
{
	// won if at least one alignement completed
	return NbAlignementDone(boardDesc->aligned);
}

int* Game::PlayerState::CaseArrayAligned()
{
	return boardDesc->CaseArrayFromAlignement(alignementCompleted);
}


/* Game state */

Game::GameState::GameState(BoardDescription *desc)
{
	boardDesc = desc;
	playerState = new PlayerState*[boardDesc->nbPlayer];
	for (int i = 0; i < boardDesc->nbPlayer; i++) playerState[i] = new PlayerState(boardDesc);
}

Game::GameState::~GameState()
{
	for (int i = 0; i < boardDesc->nbPlayer; i++) delete playerState[i];
	delete[] playerState;
}
