#ifndef CONTROLLERINTERFACE_H
#define CONTROLLERINTERFACE_H

/* Generic interface for Controller */

#include <string>

class ControllerNull;

class ControllerInterface {
	public:

	// types definitions for game config

	typedef enum { TypeHuman, TypeIA } PlayerType_t;

	struct ConfigPlayer {
		std::string name;
		int force;
		PlayerType_t type;
	};

	struct Config {
		ConfigPlayer player[2];
		int rows, columns, align;
	};

	virtual void Start() = 0;

	// methods for UI calls

	virtual void ConfigChange(const Config &config) = 0;
	virtual void ExitGame() = 0;
	virtual void NewGame() = 0;
	virtual bool PlayAtCol(int col) = 0;
	virtual void ResetScores() = 0;
	virtual void Loop() = 0;

	// null controller

	static ControllerNull controllerNull;
};

// null implementation of controller

class ControllerNull : public ControllerInterface {
	public:

	virtual void ConfigChange(const Config &config __attribute__((unused))) {}
	virtual void ExitGame() {}
	virtual void NewGame() {}
	virtual bool PlayAtCol(int col __attribute__((unused))) { return false; }
	virtual void ResetScores() {}
	virtual void Start() {}
	virtual void Loop() {}
};
#endif
