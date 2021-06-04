#include "TecladoJogo.h"

static TecladoJogo* instance_;

TecladoJogo* TecladoJogo::getInstance()
{
	if (instance_ == nullptr) {

		instance_ = new TecladoJogo();
	}

	return instance_;
}

TecladoJogo::TecladoJogo() {

	registraTeclasDoJogo();
}

void TecladoJogo::registraTeclasDoJogo()
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

int TecladoJogo::capturaTeclado() {
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