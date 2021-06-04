#pragma once

#include "Core.h"

class KartBase {

public:

	static bool		gCarroColidiu;

	static int		globalColunaCarro,
					globalLinhaCarro;


	static KartBase* getInstance();
	~KartBase();
	void moveParaDireita(char** tabuleiroBackEnd);
	void moveParaEsquerda(char** tabuleiroBackEnd);
	void moveParaCima(char** tabuleiroBackEnd);
	void moveParaBaixo(char** tabuleiroBackEnd);

private:
	KartBase();
};
