#include "KartBase.h"

bool	KartBase::gCarroColidiu = false;

int		KartBase::globalColunaCarro = 0,
		KartBase::globalLinhaCarro = 0;

static KartBase* instance_;

KartBase* KartBase::getInstance()
{
	if (instance_ == nullptr) {

		instance_ = new KartBase();
	}

	return instance_;
}

KartBase::KartBase() {

}

KartBase::~KartBase() {
	instance_ = nullptr;
}

void KartBase::moveParaDireita(char** pistaBackEnd) {

	if (globalColunaCarro < (COLUNAS - 4))
	{
		pistaBackEnd[globalLinhaCarro][globalColunaCarro] = ' ';

		globalColunaCarro++;

		if (pistaBackEnd[globalLinhaCarro][globalColunaCarro] == GAME_CARRO_INIMIGO)
		{
			pistaBackEnd[globalLinhaCarro][globalColunaCarro] = GAME_CARRO_COLISAO;
			gCarroColidiu = true;
			return;
		}

		pistaBackEnd[globalLinhaCarro][globalColunaCarro] = GAME_CARRO_PLAYER;
	}
}

void KartBase::moveParaEsquerda(char** pistaBackEnd) {

	if (globalColunaCarro > 0)
	{
		pistaBackEnd[globalLinhaCarro][globalColunaCarro] = ' ';
		globalColunaCarro--;

		if (pistaBackEnd[globalLinhaCarro][globalColunaCarro] == GAME_CARRO_INIMIGO)
		{
			pistaBackEnd[globalLinhaCarro][globalColunaCarro] = GAME_CARRO_COLISAO;
			gCarroColidiu = true;
			return;
		}

		pistaBackEnd[globalLinhaCarro][globalColunaCarro] = GAME_CARRO_PLAYER;
	}
}

void KartBase::moveParaCima(char** pistaBackEnd) {

	if (globalLinhaCarro > 0)
	{
		pistaBackEnd[globalLinhaCarro][globalColunaCarro] = ' ';
		globalLinhaCarro--;

		if (pistaBackEnd[globalLinhaCarro][globalColunaCarro] == GAME_CARRO_INIMIGO)
		{
			pistaBackEnd[globalLinhaCarro][globalColunaCarro] = GAME_CARRO_COLISAO;
			gCarroColidiu = true;
			return;
		}
		pistaBackEnd[globalLinhaCarro][globalColunaCarro] = GAME_CARRO_PLAYER;
	}
}

void KartBase::moveParaBaixo(char** pistaBackEnd) {

	if (globalLinhaCarro < 4)
	{
		pistaBackEnd[globalLinhaCarro][globalColunaCarro] = ' ';

		globalLinhaCarro++;

		if (pistaBackEnd[globalLinhaCarro][globalColunaCarro] == GAME_CARRO_INIMIGO)
		{
			pistaBackEnd[globalLinhaCarro][globalColunaCarro] = GAME_CARRO_COLISAO;
			gCarroColidiu = true;
			return;
		}

		pistaBackEnd[globalLinhaCarro][globalColunaCarro] = GAME_CARRO_PLAYER;
	}
}