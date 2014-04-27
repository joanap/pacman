#include "Wizard.h"

Wizard::~Wizard() {
    
}

void Wizard::loadMap(std::string fileName) {
    std::ifstream infile;
    std::string line;
    infile.open(fileName.c_str(), std::ifstream::in);
    _map = "";
    
    if (infile.is_open()) {
        while (getline(infile, line)) {
            _map += line.c_str();
            _mapHeight++;
            _mapWidth = (int)line.length();        
        }

    } else {
        throw new std::string("Could not load the map file.");
    }
}

void Wizard::setGhosts(std::vector<Ghost*> ghosts) {
    _ghosts = ghosts;
}

void Wizard::setPacmen(std::vector<Pacman*> pacmen) {
    _pacmen = pacmen;
}

std::vector<Ghost*> Wizard::getGhosts() {
    return _ghosts;
}

std::vector<Pacman*> Wizard::getPacmen() {
    return _pacmen;
}


std::string Wizard::getMap() {
    return _map;
}

float Wizard::getMapWidth() {
    return _mapWidth;
}

float Wizard::getMapHeight() {
    return _mapHeight;
}

int Wizard::bigBallsCatched() {
    return _bigBallsCatched;
}

void Wizard::setBigBallsCatched(int value) {
    _bigBallsCatched = value;
}

int Wizard::positionToIndex(float x, float y) {
    int position = (((_mapHeight - 1) / 2) - round(y)) * _mapWidth;
    position += ((_mapWidth - 1) / 2) + round(x);
    return position;
}

std::vector<float> Wizard::indexToPosition(int index) {
	std::vector<float> coords(2);
	
	coords[0] = (index % _mapWidth) - ((_mapWidth - 1) / 2);
	coords[1] = ((_mapWidth * ((_mapHeight - 1) / 2)) + ((_mapWidth - 1) / 2) + coords[0] - index) / _mapWidth;
	
	return coords;
}

int Wizard::rightPosition(int index, int nPositions) {
	return index + nPositions;
}

int Wizard::leftPosition(int index, int nPositions) {
	return index - nPositions;
}

int Wizard::upPosition(int index, int nPositions) {
	return index - nPositions * _mapWidth;
}
int Wizard::downPosition(int index, int nPositions) {
	return index + nPositions * _mapWidth;

}

std::vector<int> Wizard::availablePositions(int index) {
	std::vector<int> neighbours;
	int newIndex = 0;
	
	newIndex = leftPosition(index, 2);
	if(_map[newIndex] != WALL && _map[newIndex] != ' ') {
		neighbours.push_back(LEFT); 
	}
	newIndex = rightPosition(index, 2);
	if(_map[newIndex] != WALL && _map[newIndex] != ' ') {
		neighbours.push_back(RIGHT); 
	}
	
	newIndex = upPosition(index, 2);
	if(_map[newIndex] != WALL && _map[newIndex] != ' ') {
		neighbours.push_back(UP); 
	}
	
	newIndex = downPosition(index, 2);
	if(_map[newIndex] != WALL && _map[newIndex] != ' ') {
		neighbours.push_back(DOWN); 
	}
	
	return neighbours;
}

bool Wizard::canTurn(float x, float y) {
	int index = positionToIndex(x, y);
	return availablePositions(index).size() > 2;
}

int Wizard::availablePosition(float x, float y) {
	int index = positionToIndex(x, y);
	std::vector<int> positions = availablePositions(index);
    int size = (int)positions.size();
    if(!size) {
        std::cout << "nao encontrei posicoes" << std::endl;
        return -1;
    }
    return positions[rand() % size];
}   

int Wizard::availablePosition(int index) {
	std::vector<int> positions = availablePositions(index);
    int size = (int)positions.size();
    if(!size) {
        std::cout << "nao encontrei posicoes" << std::endl;
        return -1;
    }
    return positions[rand() % size];
}

int Wizard::positionAhead(float x, float y, int direction, int nPositions) {
	int index = 0;
	switch (direction) {    
		case UP:
			index = upPosition(positionToIndex(x, y), nPositions);
			break;
		case LEFT:
			index = leftPosition(positionToIndex(x, y), nPositions);
			break;
		case DOWN:
			index = downPosition(positionToIndex(x, y), nPositions);
			break;
		case RIGHT:
			index = rightPosition(positionToIndex(x, y), nPositions);
			break;
		default:
			break;
	}
	
	return index;
}

char Wizard::getMapSymbol(float x, float y) {
    int index = positionToIndex(x, y);
    return _map[index];
}

void Wizard::changeMap(float x, float y, char symbol) {
    int index = positionToIndex(x, y);
    _map[index] = symbol;
}

void Wizard::printMap() {
    int limit = 0;
    for (int i = 0; i < _map.length(); i++) {
        std::cout << _map[i];
        limit++;
        if (limit == 55) {
            std::cout << std::endl;
            limit = 0;
        }
    }
    
    std::cout << std::endl;
}

bool Wizard::isWall(float x, float y, int direction) {
	int indexAhead = positionAhead(x, y, direction, 2);
	return _map[indexAhead] == WALL;
}

bool Wizard::isBall(float x, float y) {
    int index = positionToIndex(x, y);
    return (_map[index] == SMALL_BALL || _map[index] == BIG_BALL);
}

bool Wizard::isGhost(float x, float y, int direction) {
    int maxPositions = 4;

    for (int i = 1; i <= maxPositions; i++) {
        if (_map[positionAhead(x, y, direction, i)] == GHOST) {
            return true;
        }
    }

    return false;
}

bool Wizard::isGhostScared(float x, float y, int direction) {
    int ghostIndex = 0;
    int indexAhead = positionAhead(x, y, direction, 1);
    int indexFarAway = positionAhead(x, y, direction, 2);
    for (int i=0; i < _ghosts.size(); i++) {
        ghostIndex = positionToIndex(_ghosts[i]->getX(), _ghosts[i]->getY());
        if (_ghosts[i]->getTrouble() &&
            (ghostIndex == positionToIndex(x, y)
            || ghostIndex == indexAhead
            || ghostIndex == indexFarAway)) {
            return true;
        }
    }
    return false;
}

bool Wizard::isPacman(char name, float x, float y, int direction) {

    int maxPositions = 3;
    for (int i = 0; i <= maxPositions; i++) {
        int index = positionAhead(x, y, direction, i);
        if (_map[index] != name && (_map[index] == PACMAN || _map[index] == POCMAN)) {
            return true;
        }
    }

    return false;
}

void Wizard::treatIfSick(char name, float x, float y, int direction) {
    int maxPositions = 3;
    char symbol = HALL;
    for (int i = 0; i <= maxPositions; i++) {
        int index = positionAhead(x, y, direction, i);
        symbol = _map[index];
        if (symbol != name && (symbol == PACMAN || symbol == POCMAN)) {
            break;
        }
    }
    
    for (int i = 0; i < _pacmen.size(); i++) {
        if (_pacmen[i]->getName() == symbol && _pacmen[i]->getSick()) {
            _pacmen[i]->treat();
        }
    }
}

bool Wizard::isSameIndex(float x1, float y1, float x2, float y2) {
    
    int i = positionToIndex(x1, y1);
    int j = positionToIndex(x2, y2);
    
    return i == j;
}

void Wizard::addAmmunitionToQueue(float x, float y) {
    std::vector<float> coords;
    coords.push_back(x);
    coords.push_back(y);
    
    _ammunitionsQueue.push(coords);
}

void Wizard::removeAmmunitionFromQueue() {
    std::vector<float> coords = _ammunitionsQueue.front();
    _ammunitionsQueue.pop();
    changeMap(coords[0], coords[1], AMMUNITION);
}


void Wizard::ghostsTrouble() {
    _bigBallsCatched++;
    for (int i = 0; i < _ghosts.size(); i++) {
        _ghosts[i]->setTrouble(true);
        _ghosts[i]->setSpeed(GHOST_ESCAPE_SPEED);
    }
    glutTimerFunc(10000, backToNormal, 1);
}

void Wizard::ghostHidden(float x, float y) {
    int ghostIndex = 0;
    for (int i = 0; i < _ghosts.size(); i++) {
        ghostIndex = positionToIndex(_ghosts[i]->getX(), _ghosts[i]->getY());
        if (ghostIndex == positionToIndex(x, y)) {
            _ghosts[i]->setHidden(true);
            _ghosts[i]->setTrouble(false);
            _ghosts[i]->backAgain();
            glutTimerFunc(5000, theComeBack, i);
            return;
        }
    }
    
}

int Wizard::shotGhost(float x, float y) {
    int deadGhost = 0;
    int index = positionToIndex(x, y);
    for(int i = 0; i < _ghosts.size(); i++) {
        int ghostIndex = positionToIndex(_ghosts[i]->getX(), _ghosts[i]->getY());
        if (index == ghostIndex) {
            deadGhost = _ghosts[i]->shoot(i);
            _map[ghostIndex] = _ghosts[i]->getLastSymbol();
        }
    }
    return deadGhost;
}


void Wizard::killPacman(char pacman) {
    for(int i = 0; i < _pacmen.size(); i++) {
        if (pacman == _pacmen[i]->getName()) {
            _pacmen[i]->detonate();
        }
    }
}


void backToNormal(int value) {
    int ballsCatched = Wizard::getInstance().bigBallsCatched();
    if (ballsCatched == 1) {
        std::vector<Ghost*> ghosts = Wizard::getInstance().getGhosts();
        for (int i = 0; i < ghosts.size(); i++) {
            ghosts[i]->setTrouble(false);
            ghosts[i]->setSpeed(GHOST_NORMAL_SPEED);
        }
    }
    
    Wizard::getInstance().setBigBallsCatched(ballsCatched - 1);
}

void theComeBack(int ghostIndex) {
    std::vector<Ghost*> ghosts = Wizard::getInstance().getGhosts();
    Ghost* ghost = ghosts[ghostIndex];
    ghost->setHidden(false);
    ghost->setLife(100);
}