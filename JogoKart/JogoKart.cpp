#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <thread>
#include <random>
#include <iostream>

#define LINHAS 5
#define COLUNAS 22

#define TAM_LINHAS_INTERFACE 9
#define TAM_COLUNAS_INTERFACE 22

#define TECLA_MENU_OPCAO_JOGAR 1
#define TECLA_MENU_OPCAO_TUTORIAL 2
#define TECLA_MENU_OPCAO_SAIR 3

#define TECLA_GAMEPLAY_DIRECIONAL_UP 88
#define TECLA_GAMEPLAY_DIRECIONAL_DOWN 22
#define TECLA_GAMEPLAY_DIRECIONAL_RIGHT 66
#define TECLA_GAMEPLAY_DIRECIONAL_LEFT 44
#define TECLA_GAMEPLAY_ACAO_CAPTURAR 55
#define TECLA_GAMEPLAY_ACAO_SOLTAR 33
#define TECLA_GAMEPLAY_ACAO_CANCELAR_JOGADA 11
#define TECLA_GAMEPLAY_ACAO_SAIR_DO_JOGO 118

#define TECLA_INVALIDA 999


int globalColunaCarro, 
	globalLinhaCarro;

char globalCarroPlayer = '#',
	 globalCarroInimigo = '$';

HANDLE gInterfaceEvent;

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
				if (pistaBackEnd[linhasInterface - descontoLinhasInterfaceMatriz][colunasInterface] == globalCarroPlayer) {
					printf("\x1b[37m|==|-");
				}
				else if (pistaBackEnd[linhasInterface - descontoLinhasInterfaceMatriz][colunasInterface] == globalCarroInimigo)
				{
					printf("\x1b[32m-|==|");
				}
				else {
					printf("\x1b[37m  %c  ", pistaBackEnd[linhasInterface - descontoLinhasInterfaceMatriz][colunasInterface]);
				}
			}
		}
	}
	printf("\n\x1b[31m_____________________________________________________________________________________________________________\n\n\n\n");
}

void registraTeclasDoJogo()
{
	//DOC teclas virtuais https://docs.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes
	//DOC registro de teclas https://docs.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-registerhotkey

	//TECLAS DE GAMEPLAY
	RegisterHotKey(NULL, 1, MOD_NOREPEAT, 0x26);//DIRECIONAL PARA CIMA
	RegisterHotKey(NULL, 1, MOD_NOREPEAT, 0x28);//DIRECIONAL PARA BAIXO
	RegisterHotKey(NULL, 1, MOD_NOREPEAT, 0x27);//DIRECIONAL PARA DIREITA
	RegisterHotKey(NULL, 1, MOD_NOREPEAT, 0x25);//DIRECIONAL PARA ESQUERDA
	RegisterHotKey(NULL, 1, MOD_NOREPEAT, 0x70);//F1 = CAPTURA A PECA
	RegisterHotKey(NULL, 1, MOD_NOREPEAT, 0x71);//F2 = SOLTA A PECA
	RegisterHotKey(NULL, 1, MOD_NOREPEAT, 0x72);//F3 = CANCELAR JOGADA
	RegisterHotKey(NULL, 1, MOD_NOREPEAT, 0x78);//F9 = SAIR DO JOGO

	//TECLAS DE MENU
	RegisterHotKey(NULL, 1, MOD_NOREPEAT, 0x61);//FUNCAO NUMERO 1
	RegisterHotKey(NULL, 1, MOD_NOREPEAT, 0x62);//FUNCAO NUMERO 2
	RegisterHotKey(NULL, 1, MOD_NOREPEAT, 0x63);//FUNCAO NUMERO 3
}

int capturaTeclado() {
	MSG msg = { 0 };

	while (GetMessage(&msg, NULL, 0, 0) != 0)
	{
		if (msg.message == WM_HOTKEY)
		{
			//DIRECIONAIS
			if (msg.lParam == 2490368)
				return TECLA_GAMEPLAY_DIRECIONAL_UP;
			if (msg.lParam == 2621440)
				return TECLA_GAMEPLAY_DIRECIONAL_DOWN;
			if (msg.lParam == 2424832)
				return TECLA_GAMEPLAY_DIRECIONAL_LEFT;
			if (msg.lParam == 2555904)
				return TECLA_GAMEPLAY_DIRECIONAL_RIGHT;

			//ACOES
			if (msg.lParam == 7340032)
				return TECLA_GAMEPLAY_ACAO_CAPTURAR;
			if (msg.lParam == 7405568)
				return TECLA_GAMEPLAY_ACAO_SOLTAR;
			if (msg.lParam == 7471104)
				return TECLA_GAMEPLAY_ACAO_CANCELAR_JOGADA;
			if (msg.lParam == 7864320)
				return TECLA_GAMEPLAY_ACAO_SAIR_DO_JOGO;

			//TECLA MENU
			if (msg.lParam == 6356992)
				return TECLA_MENU_OPCAO_JOGAR;
			if (msg.lParam == 6422528)
				return TECLA_MENU_OPCAO_TUTORIAL;
			if (msg.lParam == 6488064)
				return TECLA_MENU_OPCAO_SAIR;
		}
	}

	return TECLA_INVALIDA;
}

void moveParaDireita(char** pistaBackEnd) {

	if (globalColunaCarro < (COLUNAS - 4))
	{
		pistaBackEnd[globalLinhaCarro][globalColunaCarro] = ' ';
		globalColunaCarro++;
		pistaBackEnd[globalLinhaCarro][globalColunaCarro] = globalCarroPlayer;
	}
}

void moveParaEsquerda(char** pistaBackEnd) {

	if (globalColunaCarro > 0 )
	{
		pistaBackEnd[globalLinhaCarro][globalColunaCarro] = ' ';
		globalColunaCarro--;
		pistaBackEnd[globalLinhaCarro][globalColunaCarro] = globalCarroPlayer;
	}
}

void moveParaCima(char** pistaBackEnd) {

	if (globalLinhaCarro > 0 )
	{
		pistaBackEnd[globalLinhaCarro][globalColunaCarro] = ' ';
		globalLinhaCarro--;
		pistaBackEnd[globalLinhaCarro][globalColunaCarro] = globalCarroPlayer;
	}
}

void moveParaBaixo(char** pistaBackEnd) {

	if (globalLinhaCarro < 4)
	{
		pistaBackEnd[globalLinhaCarro][globalColunaCarro] = ' ';
		globalLinhaCarro++;
		pistaBackEnd[globalLinhaCarro][globalColunaCarro] = globalCarroPlayer;
	}
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
				globalLinhaCarro = l;
				globalColunaCarro = c;
				matriz[l][c] = globalCarroPlayer;
			}
			else {
				matriz[l][c] = ' ';
			}
		}
	}
	return matriz;
}

void movimentacaoNaPistaBackEnd(char** pistaBackEnd, int movimento) {

	if (movimento == TECLA_GAMEPLAY_DIRECIONAL_RIGHT)
		moveParaDireita(pistaBackEnd);

	if (movimento == TECLA_GAMEPLAY_DIRECIONAL_LEFT)
		moveParaEsquerda(pistaBackEnd);

	if (movimento == TECLA_GAMEPLAY_DIRECIONAL_UP)
		moveParaCima(pistaBackEnd);

	if (movimento == TECLA_GAMEPLAY_DIRECIONAL_DOWN)
		moveParaBaixo(pistaBackEnd);
}

void gerarCarrosAleatoriosNaPista(char** pistaBackEnd) {
	while (true) {

		for (int linha = 0, coluna = (COLUNAS -1); linha <= 4; linha++) {
			if (rand() % 2 == 0)
				pistaBackEnd[linha][coluna] = globalCarroInimigo;
			else
				pistaBackEnd[linha][coluna] = ' ';
		}

		for (int linha = 0, coluna = (COLUNAS - 1); linha <= 4; linha++) {
			if (rand() % 2 == 0)
				pistaBackEnd[linha][coluna] = ' ';
		}

		Sleep(2000);
		SetEvent(gInterfaceEvent);
	}
}

void moveCarrosGeradosAleatoriamente(char** pistaBackEnd) {
	while (true) {

		Sleep(200);

		for (int linha = 0; linha <= 4; linha++) {
			for (int coluna = 0; coluna <= (COLUNAS - 1); coluna++) {

				if (pistaBackEnd[linha][coluna] == globalCarroInimigo) {
					pistaBackEnd[linha][coluna] = ' ';
					pistaBackEnd[linha][coluna - 1] = globalCarroInimigo;
				}
			}
		}
		SetEvent(gInterfaceEvent);
	}
}

int atualizacaoInterfaceGrafica(char** pistaBackEnd) {
	DWORD dwWaitResult;
	
	while (true) {

		dwWaitResult = WaitForSingleObject(gInterfaceEvent, INFINITE);

		switch (dwWaitResult)
		{
			case WAIT_OBJECT_0:
				pistaUserIterface(pistaBackEnd);
				ResetEvent(gInterfaceEvent);
			break;

			default:
				printf("Wait error (%d)\n", GetLastError());
				return 0;
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

	registraTeclasDoJogo();
	pistaBackEnd = organizaPistaBackEndIncial();
	criacaoDeEventosThreads(pistaBackEnd);

	std::thread interfaceGrafica(atualizacaoInterfaceGrafica, pistaBackEnd);
	std::thread gerarCarros(gerarCarrosAleatoriosNaPista, pistaBackEnd);
	std::thread movimentaCarros(moveCarrosGeradosAleatoriamente, pistaBackEnd);

	while (true) {
		teclaDirecional = capturaTeclado();
		movimentacaoNaPistaBackEnd(pistaBackEnd, teclaDirecional);
		SetEvent(gInterfaceEvent);
	}

	return 0;
}
