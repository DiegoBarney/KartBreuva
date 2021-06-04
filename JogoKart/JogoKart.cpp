#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <thread>
#include <random>
#include <iostream>
#include "Core.h"
#include "TecladoJogo.h"
#include "KartBase.h"

HANDLE gInterfaceEvent;
char gAvisosJogo[100] = {' '};

void pistaUserIterface(char** pistaBackEnd) {

	int descontoLinhasInterfaceMatriz = 0;

	system("cls");

	printf("\x1b[37mDirecionais - \x1b[32mMovimenta o carro \n");

	printf("\x1b[31m_____________________________________________________________________________________________________________\n");
	for (int linhasInterface = 1; linhasInterface <= TAM_LINHAS_INTERFACE; linhasInterface++) {
		printf("\n");

		if (linhasInterface % 2 == 0)
		{
			printf("\x1b[31m__ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ __ _\n");
		}
		else {

			descontoLinhasInterfaceMatriz++;

			for (int colunasInterface = 0; colunasInterface < TAM_COLUNAS_INTERFACE; colunasInterface++)
			{
				if (pistaBackEnd[linhasInterface - descontoLinhasInterfaceMatriz][colunasInterface] == GAME_CARRO_PLAYER) {
					printf("\x1b[37m|==|-");
				}
				else if (pistaBackEnd[linhasInterface - descontoLinhasInterfaceMatriz][colunasInterface] == GAME_CARRO_INIMIGO)
				{
					printf("\x1b[32m-|==|");
				}
				else if (pistaBackEnd[linhasInterface - descontoLinhasInterfaceMatriz][colunasInterface] == GAME_CARRO_COLISAO) {
					printf("\x1b[32m|==||==|");
				}
				else {
					printf("\x1b[37m  %c  ", pistaBackEnd[linhasInterface - descontoLinhasInterfaceMatriz][colunasInterface]);
				}
			}
		}
	}
	printf("\n\x1b[31m_____________________________________________________________________________________________________________\n");
	printf("\n\x1b[31mAVISOS: %s \n", gAvisosJogo);
}

char** organizaPistaBackEndIncial() {
	char** matriz = NULL;

	matriz = (char**)malloc(LINHAS * sizeof(char*));

	for (int i = 0; i < LINHAS; ++i)
		matriz[i] = (char*)malloc(COLUNAS * sizeof(char));


	for (int l = 0; l < LINHAS; l++)
	{
		for (int c = 0; c < COLUNAS; c++)
		{
			if (l == 2 && c == 0) {
				KartBase::globalLinhaCarro = l;
				KartBase::globalColunaCarro = c;
				matriz[l][c] = GAME_CARRO_PLAYER;
			}
			else {
				matriz[l][c] = ' ';
			}
		}
	}
	return matriz;
}

void movimentacaoNaPistaBackEnd(char** pistaBackEnd, int movimento) {
	KartBase* kartbase = KartBase::getInstance();

	if (movimento == TECLA_GAMEPLAY_DIRECIONAL_RIGHT)
		kartbase->moveParaDireita(pistaBackEnd);

	if (movimento == TECLA_GAMEPLAY_DIRECIONAL_LEFT)
		kartbase->moveParaEsquerda(pistaBackEnd);

	if (movimento == TECLA_GAMEPLAY_DIRECIONAL_UP)
		kartbase->moveParaCima(pistaBackEnd);

	if (movimento == TECLA_GAMEPLAY_DIRECIONAL_DOWN)
		kartbase->moveParaBaixo(pistaBackEnd);
}

void gerarCarrosAleatoriosNaPista(char** pistaBackEnd) {
	while (KartBase::gCarroColidiu == false) {

		for (int linha = 0, coluna = (COLUNAS -1); linha <= (LINHAS -1); linha++) {
			if (rand() % 2 == 0)
				pistaBackEnd[linha][coluna] = GAME_CARRO_INIMIGO;
			else
				pistaBackEnd[linha][coluna] = VAZIO;
		}

		for (int linha = 0, coluna = (COLUNAS - 1); linha <= (LINHAS - 1); linha++) {
			if (rand() % 2 == 0)
				pistaBackEnd[linha][coluna] = VAZIO;
		}

		Sleep(2000);
		SetEvent(gInterfaceEvent);
	}
}

void moveCarrosGeradosAleatoriamente(char** pistaBackEnd) {

	while (KartBase::gCarroColidiu == false) {

		Sleep(200);

		for (int linha = 0; linha <= 4; linha++) {
			for (int coluna = 0; coluna <= (COLUNAS - 1); coluna++) {

				if (pistaBackEnd[linha][coluna] == GAME_CARRO_INIMIGO) {
					pistaBackEnd[linha][coluna] = VAZIO;

					if (pistaBackEnd[linha][coluna - 1] == GAME_CARRO_PLAYER)
					{
						pistaBackEnd[linha][coluna - 1] = GAME_CARRO_COLISAO;
						KartBase::gCarroColidiu = true;
						memcpy(gAvisosJogo, GAME_AVISO_COLISAO, strlen(GAME_AVISO_COLISAO) + 1);
					}
					else {
						pistaBackEnd[linha][coluna - 1] = GAME_CARRO_INIMIGO;
					}
				}
			}
		}

		SetEvent(gInterfaceEvent);
	}
}

void atualizacaoInterfaceGrafica(char** pistaBackEnd) {
	DWORD dwWaitResult;
	
	while (KartBase::gCarroColidiu == false) {

		dwWaitResult = WaitForSingleObject(gInterfaceEvent, INFINITE);

		switch (dwWaitResult)
		{
			case WAIT_OBJECT_0:
				pistaUserIterface(pistaBackEnd);
				ResetEvent(gInterfaceEvent);
			break;

			default:
				printf("Wait error (%d)\n", GetLastError());
				return;
		}
	}
}

void criacaoDeEventosThreads(char** pistaBackEnd) {

	gInterfaceEvent = CreateEvent(NULL, TRUE, FALSE, TEXT("InterfacePista"));

	if (gInterfaceEvent == NULL)
	{
		printf("CreateEvent failed (%d)\n", GetLastError());
		return;
	}
}

int main()
{
	char** pistaBackEnd;
	int teclaDirecional = 0;
	TecladoJogo* TecladoJogo = TecladoJogo::getInstance();

	pistaBackEnd = organizaPistaBackEndIncial();
	criacaoDeEventosThreads(pistaBackEnd);

	std::thread interfaceGrafica(atualizacaoInterfaceGrafica, pistaBackEnd);
	std::thread gerarCarros(gerarCarrosAleatoriosNaPista, pistaBackEnd);
	std::thread movimentaCarros(moveCarrosGeradosAleatoriamente, pistaBackEnd);

	while (KartBase::gCarroColidiu == false) {

		teclaDirecional = TecladoJogo->capturaTeclado();
		movimentacaoNaPistaBackEnd(pistaBackEnd, teclaDirecional);
		SetEvent(gInterfaceEvent);
	}

	movimentaCarros.join();
	gerarCarros.join();
	interfaceGrafica.join();

	system("pause");


	return 0;
}
