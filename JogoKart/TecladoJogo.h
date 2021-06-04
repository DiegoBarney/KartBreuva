#pragma once
#include "Core.h"

class TecladoJogo {

public:
	static TecladoJogo* getInstance();
	int capturaTeclado();

private:
	TecladoJogo();
	void registraTeclasDoJogo();
};
