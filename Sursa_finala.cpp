#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>
#include <fstream>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <cstdio>
#include <ctime>
#include <string>
#include <chrono>
int locuri_ocupate[4];
int verificari[10];

char* _strrev_(char* nume)
{
	int i = 0;
	char aux;
	for (i = 0;i < strlen(nume) / 2;i++)
	{
		aux = nume[i];
		nume[i] = nume[strlen(nume) - 1 - i];
		nume[strlen(nume) - 1 - i] = aux;
	}
	return nume;
}

char* _strlwr_(char* nume)
{
	int i = 0;
	for (i = 0;i < strlen(nume);i++)
		if (nume[i] >= 'A' && nume[i] <= 'Z')
			nume[i] = nume[i] + 32;
	return nume;
}

//verifica_adresa, verifica_nume_prenume - pentru a verifica daca prenumele, adresa si numele tuturor sunt caractere alfanumerice specifice

int cauta_duplicat(int n, long long id)
{
	char g[10], s[100];
	int i, j;
	long long id_prov = 0;
	switch (n)
	{
	case 1:
		strcpy(g, "grupa1");
		break;

	case 2:
		strcpy(g, "grupa2");
		break;

	case 3:
		strcpy(g, "grupa3");
		break;
	}
	std::ifstream fin(g);
	while (fin.getline(s, 100))
	{
		id_prov = 0;
		for (j = 0;j < strlen(s);j++)
			if (s[j] == '|')
				break;
			else
			{
				id_prov = id_prov * 10 + s[j] - '0';
			}
		if (id_prov == id)
			return 1;
	}
	return 0;
}

int verifica_adresa(char a[45])
{
	int i = 0;
	if (strlen(a) > 45)
		return 0;
	for (i = 0;i < strlen(a);i++)
		if (!((a[i] >= 'a' && a[i] <= 'z') || (a[i] >= 'A' && a[i] <= 'Z') || (a[i] >= '0'&&a[i] <= '9') || a[i] == '.' || a[i] == ',' || a[i] == ' ' || a[i] == '-'))
			return 0;
	return 1;
}

int verifica_nume_prenume(char a[45])
{
	int i = 0;
	if (strlen(a) > 25)
		return 0;
	for (i = 0;i < strlen(a);i++)
		if (!((a[i] >= 'a' && a[i] <= 'z') || (a[i] >= 'A' && a[i] <= 'Z') || a[i] == ' ' || a[i] == '-'))
			return 0;
	return 1;
}

int verificare_varsta(int varsta)
{
	if (varsta < 0 || varsta >= 7)
		return 0;
	return 1;
}

int stabilire_grupa(int varsta)
{
	if (varsta < 2)
		return 1;
	if (varsta < 4)
		return 2;
	return 3;
}

int verificare_disponibilitate_grupa(int n)
{
	if (locuri_ocupate[n]<25)
		return 1;
	return 0;
}

void initializeza_locuri_gradinita()
{
	int nr;
	std::ifstream fin("nrcopiigrupe");
	if (fin.good())
	{
		nr = 0;
		fin >> nr;
		locuri_ocupate[1] = nr;
		nr = 0;
		fin >> nr;
		locuri_ocupate[2] = nr;
		nr = 0;
		fin >> nr;
		locuri_ocupate[3] = nr;
	}
	else locuri_ocupate[1] = locuri_ocupate[2] = locuri_ocupate[3] = 0;

}

void refacere_fisier_nr_locuri()
{
	std::ofstream fout("nrcopiigrupe");
	fout << locuri_ocupate[1] << '\n' << locuri_ocupate[2] << '\n' << locuri_ocupate[3] << '\n';
}

void calculeaza_id(long long &id, char nume[30], char prenume[30], int varsta, char adresa[45], char nume_mama[30], char nume_tata[30], int valid)
{
	//initializez locurile ocupate
	initializeza_locuri_gradinita();
	//initial -1, copilul nu poate fi inscris
	id = -1;
	//initializez verifica
	int j = 0;
	for (j = 0;j < 8;j++)
		verificari[j] = 0;
	verificari[8] = 1;
	//fac verificarile de baza
	if (verifica_nume_prenume(nume) == 1)
		verificari[1] = 1;
	if (verifica_nume_prenume(prenume) == 1)
		verificari[2] = 1;
	if (verifica_nume_prenume(nume_mama) == 1)
		verificari[5] = 1;
	if (verifica_nume_prenume(nume_tata) == 1)
		verificari[6] = 1;
	if (verificare_varsta(varsta) == 1)
		verificari[3] = 1;
	if (verifica_adresa(adresa) == 1)
		verificari[4] = 1;
	if (valid == 1)
	{
		if (verificare_disponibilitate_grupa(stabilire_grupa(varsta)) == 1)
			verificari[7] = 1;
	}
	else verificari[7] = 1;
	int i = 1, ok = 1;
	for (i = 1;i < 8;i++)
		if (verificari[i] == 0)
		{
			ok = 0;
			break;
		}
	//daca totul e ok, pot inscrie copilul, deci creez id
	if (ok == 1)
	{
		verificari[0] = 1;
		id = 0;
		id = id * 100 + nume[0] - '0';
		id = id * 100 + nume[1] - '0';
		id = id * 100 + prenume[0] - '0';
		id = id * 100 + prenume[1] - '0';
		id = id * 10 + varsta;
		id = id * 100 + nume_mama[0] - '0';
		id = id * 100 + nume_mama[1] - '0';
		id = id * 100 + nume_tata[0] - '0';
		id = id * 100 + nume_tata[1] - '0';
		id = id * 10 + stabilire_grupa(varsta);
	}

	//verific duplicat
	if (valid == 1)
	{
		if (cauta_duplicat(stabilire_grupa(varsta), id) == 1)
		{
			verificari[8] = 0;
			id = -1;
		}
		else verificari[8] = 1;
	}
}

int verifica_varsta_char(char varsta[4])
{
	int i = 0;
	if (strlen(varsta) > 2)
		return 0;
	for (i = 0;i < strlen(varsta);i++)
		if (!(varsta[i] >= '0'&&varsta[i] <= '9'))
			return 0;
	return 1;
}

HWND hnume, hprenume, hvarsta, hadresa, hnume_mama, hnume_tata;

HWND login_window, main_window, logs_window, register_window, view_window, search_window, edit_window, delete_window, editbox_window;

HWND logs_listbox, view_combobox, view_listbox;

HWND search_textbox, search_listbox, search_combobox;

HWND delete_listbox;

HWND edit_listbox;

HWND editboxnume, editboxprenume, editboxvarsta, editboxadresa, editboxnume_mama, editboxnume_tata;

WNDCLASSEX mainw, logsw, registerw, vieww, searchw, editw, deletew, editboxw, loginw;

HINSTANCE hInst;

int showcmd;

int calculare_xpos(int a)
{
	return  (GetSystemMetrics(SM_CXSCREEN) - a) / 2;
}

int calculare_ypos(int a)
{
	return  (GetSystemMetrics(SM_CYSCREEN) - a) / 2;
}

void afisare_logs()
{
	std::ifstream fin("logs");
	long long maxlong = 0;
	SIZE lungime;
	char s[1000];
	if (fin.good())
	{
		while (fin.getline(s, 999))
		{
			GetTextExtentPoint32A(GetDC(logs_listbox), s, strlen(s), &lungime);
			if (lungime.cx > maxlong)
				maxlong = lungime.cx;
			SendMessageA(logs_listbox, LB_ADDSTRING, 0, (LPARAM)s);
		}
	}
	else SendMessageA(logs_listbox, LB_ADDSTRING, 0, (LPARAM) "Nu exista evenimente de afisat! (Fisierul logs inexistent!)");
	fin.close();
	SendMessageA(logs_listbox, LB_SETHORIZONTALEXTENT, maxlong + 10, 0);
	int lbn = SendMessage(logs_listbox, LB_GETCOUNT, 0, 0);
	SendMessage(logs_listbox, LB_SETTOPINDEX, (WPARAM)lbn - 1, 0);
}

void delete_logs()
{
	int result;
	result = MessageBoxA(logs_window, "Sunteti sigur ca doriti stergerea fisierului logs?", "Delete Logs!", MB_YESNO | MB_TOPMOST | MB_ICONWARNING);
	switch (result)
	{
	case IDYES:
		remove("logs");
		SendMessage(logs_listbox, LB_RESETCONTENT, 0, 0);
		MessageBoxA(logs_window, "Fisierul a fost sters cu succes!", "Done!", MB_OK | MB_ICONINFORMATION | MB_TOPMOST);
		SendMessage(logs_listbox, LB_ADDSTRING, 0, (LPARAM)L"Nimic de afisat. Fisierul logs sters!");
		//ShowWindow(main_window, 5);
		//DestroyWindow(logs_window);
		break;
	case IDNO:
		break;
	}
}

void printare_in_logs(char s[500])
{
	std::ofstream fout;
	fout.open("logs", std::ofstream::app);
	time_t rawtime;
	struct tm * timeinfo;
	time(&rawtime);
	timeinfo = localtime(&rawtime);
	fout << /*"<"<<*/ asctime(timeinfo) << "< " << s << ";> \n";
	fout.close();
}

int tabnr;

HWND b1, b2, b3, b4, b5, b6, b7, b8;

LRESULT CALLBACK WND_Main_Window(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CHAR:
	{
		/*if (wParam == VK_ESCAPE)
		{
		remove("temp");
		DestroyWindow(main_window);
		}*/
	}
	case WM_CREATE:
		CreateWindow("Button", "Inregistreaza copil", WS_VISIBLE | WS_BORDER | WS_CHILD, 40, 100, 200, 30, hwnd, (HMENU)0, 0, 0);
		CreateWindow("Button", "Vizualizare grupe", WS_VISIBLE | WS_BORDER | WS_CHILD, 40, 150, 200, 30, hwnd, (HMENU)1, 0, 0);
		CreateWindow("Button", "Cauta o inregistrare", WS_VISIBLE | WS_BORDER | WS_CHILD, 40, 200, 200, 30, hwnd, (HMENU)2, 0, 0);
		CreateWindow("Button", "Editeaza o inregistrare", WS_VISIBLE | WS_BORDER | WS_CHILD, 40, 250, 200, 30, hwnd, (HMENU)3, 0, 0);
		CreateWindow("Button", "Sterge o inregistrare", WS_VISIBLE | WS_BORDER | WS_CHILD, 40, 300, 200, 30, hwnd, (HMENU)4, 0, 0);
		CreateWindow("Button", "Logs", WS_VISIBLE | WS_BORDER | WS_CHILD, 40, 350, 200, 30, hwnd, (HMENU)5, 0, 0);
		CreateWindow("Button", "Exit!", WS_VISIBLE | WS_BORDER | WS_CHILD, 40, 400, 200, 30, hwnd, (HMENU)6, 0, 0);
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case 0:
		{
			int x = 400;
			int y = 300;
			register_window = CreateWindow("Register_Window", "Inregistrare Copil", WS_SYSMENU | WS_MINIMIZEBOX, calculare_xpos(x), calculare_ypos(y), x, y, HWND_DESKTOP, 0, 0, 0);
			ShowWindow(main_window, 0);
			ShowWindow(register_window, 5);
			UpdateWindow(register_window);
		}
		break;
		case 1:
		{
			int x = 800;
			int y = 600;
			view_window = CreateWindow("View_Window", "Vizualizare grupe", WS_SYSMENU | WS_MINIMIZEBOX, calculare_xpos(x), calculare_ypos(y), x, y, HWND_DESKTOP, 0, 0, 0);
			ShowWindow(main_window, 0);
			ShowWindow(view_window, 5);
			UpdateWindow(view_window);
		}
		break;
		case 2:
		{
			int x = 800;
			int y = 600;
			search_window = CreateWindow("Search_Window", "Cauta o inregistrare", WS_SYSMENU | WS_MINIMIZEBOX, calculare_xpos(x), calculare_ypos(y), x, y, HWND_DESKTOP, 0, 0, 0);
			ShowWindow(main_window, 0);
			ShowWindow(search_window, 5);
			UpdateWindow(search_window);
		}
		break;
		case 3:
		{
			int x = 800;
			int y = 600;
			edit_window = CreateWindow("Edit_Window", "Editeaza o inregistrare", WS_SYSMENU | WS_MINIMIZEBOX, calculare_xpos(x), calculare_ypos(y), x, y, HWND_DESKTOP, 0, 0, 0);
			ShowWindow(main_window, 0);
			ShowWindow(edit_window, 5);
			UpdateWindow(edit_window);
		}
		break;
		case 4:
		{
			int x = 800;
			int y = 600;
			delete_window = CreateWindow("Delete_Window", "Sterge o inregistrare", WS_SYSMENU | WS_MINIMIZEBOX, calculare_xpos(x), calculare_ypos(y), x, y, HWND_DESKTOP, 0, 0, 0);
			ShowWindow(main_window, 0);
			ShowWindow(delete_window, 5);
			UpdateWindow(delete_window);
		}
		break;
		case 5:
		{
			int x = 800;
			int y = 600;
			logs_window = CreateWindow("Logs_Window", "Logs", WS_SYSMENU | WS_MINIMIZEBOX, calculare_xpos(x), calculare_ypos(y), x, y, HWND_DESKTOP, 0, 0, 0);
			ShowWindow(main_window, 0);
			ShowWindow(logs_window, showcmd);
			UpdateWindow(logs_window);
		}
		break;
		case 6:
			remove("temp");
			exit(0);
			break;
		}
		break;
	case WM_CLOSE:
		remove("temp");
		DestroyWindow(main_window);
		break;
	case WM_DESTROY:
		remove("temp");
		PostQuitMessage(0);
		break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hDC;
		hDC = BeginPaint(hwnd, &ps);
		TextOut(hDC, 100, 10, " Welcome! ", strlen(" Welcome! "));
		TextOut(hDC, 0, 30, " Acesta este meniul aplicatiei Management! ", strlen(" Acesta este meniul aplicatiei Management! "));
		TextOut(hDC, 40, 50, " Va rugam selectati o optiune! ", strlen(" Va rugam selectati o optiune! "));
		EndPaint(hwnd, &ps);
		return 0;
	}
	break;
	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
}

LRESULT CALLBACK WND_Logs_Window(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CHAR:
	{
		if (wParam == VK_ESCAPE)
		{
			ShowWindow(main_window, 5);
			DestroyWindow(logs_window);
		}
		break;
	}
	case WM_CREATE:
		logs_listbox = CreateWindow("ListBox", "", WS_BORDER | WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL | LBS_DISABLENOSCROLL, 20, 30, 740, 460, hwnd, 0, 0, 0);
		CreateWindow("Button", "Delete Logs", WS_VISIBLE | WS_BORDER | WS_CHILD, 100, 500, 200, 30, hwnd, (HMENU)1, 0, 0);
		CreateWindow("Button", "Back", WS_VISIBLE | WS_BORDER | WS_CHILD, 500, 500, 200, 30, hwnd, (HMENU)2, 0, 0);
		afisare_logs();
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case 1:
		{
			delete_logs();
		}
		break;
		case 2:
		{
			ShowWindow(main_window, 5);
			DestroyWindow(logs_window);
		}
		break;
		default:
			break;
		}
		break;
	case WM_CLOSE:
	{
		ShowWindow(main_window, 5);
		DestroyWindow(logs_window);
	}
	break;
	case WM_DESTROY:
		//PostQuitMessage(0);
		break;
		break;
	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
}

LRESULT CALLBACK WND_Register_Window(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CHAR:
	{
		if (wParam == VK_ESCAPE)
		{
			ShowWindow(main_window, 5);
			DestroyWindow(register_window);
		}
		if (wParam == VK_TAB)
		{

		}
		break;
	}
	case WM_CREATE:
		hnume = CreateWindow("Edit", "", WS_BORDER | WS_CHILD | WS_VISIBLE, 150, 10, 200, 20, hwnd, 0, 0, 0);
		hprenume = CreateWindow("Edit", "", WS_BORDER | WS_CHILD | WS_VISIBLE, 150, 40, 200, 20, hwnd, 0, 0, 0);
		hvarsta = CreateWindow("Edit", "", WS_BORDER | WS_CHILD | WS_VISIBLE, 150, 70, 200, 20, hwnd, 0, 0, 0);
		hadresa = CreateWindow("Edit", "", WS_BORDER | WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL, 150, 100, 200, 20, hwnd, 0, 0, 0);
		hnume_mama = CreateWindow("Edit", "", WS_BORDER | WS_CHILD | WS_VISIBLE, 150, 130, 200, 20, hwnd, 0, 0, 0);
		hnume_tata = CreateWindow("Edit", "", WS_BORDER | WS_CHILD | WS_VISIBLE, 150, 160, 200, 20, hwnd, 0, 0, 0);
		CreateWindow("Button", "Cancel", WS_BORDER | WS_CHILD | WS_VISIBLE, 232, 200, 100, 30, hwnd, (HMENU)1, 0, 0);
		CreateWindow("Button", "Inregistreaza", WS_BORDER | WS_CHILD | WS_VISIBLE, 66, 200, 100, 30, hwnd, (HMENU)2, 0, 0);
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case 1:
			ShowWindow(main_window, 5);
			DestroyWindow(register_window);
			break;
		case 2:
		{
			char nume[30], prenume[30], adresa[45], nume_mama[30], nume_tata[30], varsta[30];
			int varsta_int;
			int gwtstat1 = 0, gwtstat2 = 0, gwtstat3 = 0, gwtstat4 = 0, gwtstat5 = 0, gwtstat6 = 0;
			gwtstat1 = GetWindowTextA(hnume, nume, 24);
			gwtstat2 = GetWindowTextA(hprenume, prenume, 24);
			gwtstat3 = GetWindowTextA(hvarsta, varsta, 24);
			gwtstat4 = GetWindowTextA(hadresa, adresa, 44);
			gwtstat5 = GetWindowTextA(hnume_mama, nume_mama, 24);
			gwtstat6 = GetWindowTextA(hnume_tata, nume_tata, 24);
			if (gwtstat1 == 0 || gwtstat2 == 0 || gwtstat3 == 0 || gwtstat4 == 0 || gwtstat5 == 0 || gwtstat6 == 0)
				MessageBoxA(0, "Toate campurile trebuiesc completate!", "Eroare!", MB_OK | MB_ICONWARNING);
			else if (verifica_varsta_char(varsta) == 1)
			{
				long long id = 0;
				varsta_int = atoi(varsta);
				calculeaza_id(id, nume, prenume, varsta_int, adresa, nume_mama, nume_tata, 1);
				if (id == -1)
				{
					int i;
					for (i = 1;i < 9;i++)
						if (verificari[i] == 0)
							break;
					switch (i)
					{
					case 1:
					{
						MessageBoxA(0, "Nume invalid! Trebuie sã contina doar litere!", "Error!", MB_OK | MB_ICONWARNING);
					}
					break;
					case 2:
					{
						MessageBoxA(0, "Prenume invalid! Trebuie sã contina doar litere!", "Error!", MB_OK | MB_ICONWARNING);
					}
					break;
					case 3:
					{
						MessageBoxA(0, "Varsta invalida! Varsta trebuie sa fie intre 0 si 7 ani!", "Error!", MB_OK | MB_ICONWARNING);
					}
					break;
					case 4:
					{
						MessageBoxA(0, "Adresa invalida! Poate sa contina doar litere, cifre, punct, virgula, cratima!", "Error!", MB_OK | MB_ICONWARNING);
					}
					break;
					case 5:
					{
						MessageBoxA(0, "Numele mamei invalid! Trebuie sã contina doar litere!", "Error!", MB_OK | MB_ICONWARNING);
					}
					break;
					case 6:
					{
						MessageBoxA(0, "Numele tatalui invalid! Trebuie sã contina doar litere!", "Error!", MB_OK | MB_ICONWARNING);
					}
					break;
					case 7:
					{
						MessageBoxA(0, "Nu este loc disponibil in grupa pentru a inscrie copilul!", "Error!", MB_OK | MB_ICONWARNING);
						ShowWindow(main_window, 5);
						DestroyWindow(register_window);
					}
					break;
					case 8:
					{
						MessageBoxA(0, "Copilul este deja inscris!", "Error!", MB_OK | MB_ICONWARNING);
						ShowWindow(main_window, 5);
						DestroyWindow(register_window);
					}
					break;
					}
				}
				else
				{
					int sigur = MessageBoxA(0, "Sunteti sigur ca doriti sa inscrieti copilul?", "Confirmare inregistrare!", MB_YESNO | MB_ICONQUESTION);
					switch (sigur)
					{
					case IDYES:
					{
						int nr;
						switch (stabilire_grupa(varsta_int))
						{
						case 1:
						{
							locuri_ocupate[1]++;
							std::ofstream fout;
							fout.open("grupa1", std::ofstream::app);
							fout << id << "|" << nume << "|" << prenume << "|" << varsta << "|" << adresa << "|" << nume_mama << "|" << nume_tata << '\n';
							MessageBoxA(0, "Copil inregistrat cu succes in grupa 1!", "Inregistrare cu succes!", MB_OK | MB_ICONINFORMATION);
						}
						break;
						case 2:
						{
							locuri_ocupate[2] ++;
							std::ofstream fout;
							fout.open("grupa2", std::ofstream::app);
							fout << id << "|" << nume << "|" << prenume << "|" << varsta << "|" << adresa << "|" << nume_mama << "|" << nume_tata << '\n';
							MessageBoxA(0, "Copil inregistrat cu succes in grupa 2!", "Inregistrare cu succes!", MB_OK | MB_ICONINFORMATION);
						}
						break;
						case 3:
						{

							locuri_ocupate[3]++;
							std::ofstream fout;
							fout.open("grupa3", std::ofstream::app);
							fout << id << "|" << nume << "|" << prenume << "|" << varsta << "|" << adresa << "|" << nume_mama << "|" << nume_tata << '\n';
							MessageBoxA(0, "Copil inregistrat cu succes in grupa 3!", "Inregistrare cu succes!", MB_OK | MB_ICONINFORMATION);
						}
						break;
						}
						std::ofstream fout;
						fout.open("copii", std::ofstream::app);
						fout << id << "|" << nume << "|" << prenume << "|" << varsta << "|" << adresa << "|" << nume_mama << "|" << nume_tata << "|" << stabilire_grupa(varsta_int) << '\n';
						fout.close();
						refacere_fisier_nr_locuri();
						fout.open("logs", std::ofstream::app);
						time_t rawtime;
						struct tm * timeinfo;
						time(&rawtime);
						timeinfo = localtime(&rawtime);
						fout << /*"<"<<*/ asctime(timeinfo) << "< " << ": [RegisterEvent]: " << nume << " " << prenume << " cu id: " << id << " a fost inregistrat in grupa: " << stabilire_grupa(varsta_int) << ";> \n";
						fout.close();
						ShowWindow(main_window, 5);
						DestroyWindow(register_window);
					}
					break;
					case IDNO:
						break;
					}
				}
			}
			else MessageBoxA(0, "Varsta trebuie sa contina maxim 2 cifre!", "Eroare!", MB_OK | MB_ICONWARNING);


		}
		break;
		}
		break;
	case WM_CLOSE:
		ShowWindow(main_window, 5);
		DestroyWindow(register_window);
		break;
	case WM_DESTROY:
		//PostQuitMessage(0);
		break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hDC;
		hDC = BeginPaint(hwnd, &ps);
		TextOut(hDC, 10, 12, "Nume: ", strlen("Nume: "));
		TextOut(hDC, 10, 42, "Prenume: ", strlen("Prenume: "));
		TextOut(hDC, 10, 72, "Varsta: ", strlen("Varsta: "));
		TextOut(hDC, 10, 102, "Adresa: ", strlen("Adresa: "));
		TextOut(hDC, 10, 132, "Numele mamei: ", strlen("Numele mamei: "));
		TextOut(hDC, 10, 162, "Numele tatalui: ", strlen("Numele tatalui: "));
		EndPaint(hwnd, &ps);
		return 0;
	}
	break;

	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
}

int nr_copii_grupe[4];

void add_combobox()
{
	SendMessage(view_combobox, CB_ADDSTRING, 0, (LPARAM)"Grupa 1");
	SendMessage(view_combobox, CB_ADDSTRING, 0, (LPARAM)"Grupa 2");
	SendMessage(view_combobox, CB_ADDSTRING, 0, (LPARAM)"Grupa 3");
	SendMessage(view_combobox, CB_ADDSTRING, 0, (LPARAM)"Total");
}

void initializare_nr_copii(int &ok)
{
	std::ifstream fin("nrcopiigrupe");
	int nr;
	int r = 0;
	ok = 1;
	nr_copii_grupe[1] = nr_copii_grupe[2] = nr_copii_grupe[3] = 0;
	if (fin.good())
	{
		while (fin >> nr)
		{
			r++;
			nr_copii_grupe[r] = nr;
		}
	}
	else ok = 0;
	if (nr_copii_grupe[1] == 0 && nr_copii_grupe[2] == 0 && nr_copii_grupe[3] == 0)
		ok = 0;
}

void view_initial()
{
	int ok;
	initializare_nr_copii(ok);
	if (ok == 1)
		SendMessageA(view_listbox, LB_ADDSTRING, 0, (LPARAM) "Va rugam sa selectati modul de afisare dorit!");
	else SendMessageA(view_listbox, LB_ADDSTRING, 0, (LPARAM) "Nu exista copii inscrisi la gradinita!");

}

void construire_sir_afisari(char s[300], char d[300])
{
	char *p;
	char sep[] = "|";
	char sir[100][200];
	p = strtok(s, sep);
	int i = 0;
	while (p != NULL)
	{
		strcpy(sir[i], p);
		i++;
		p = strtok(NULL, sep);
	}
	strcpy(d, "- ");
	strcat(d, sir[1]);
	strcat(d, " ");
	strcat(d, sir[2]);
	strcat(d, ", ");
	strcat(d, sir[3]);
	strcat(d, " ani"/*, adresa: "*/); //modificari 2 ianuarie 04:31
									  //strcat(d, sir[4]);
	strcat(d, /*" */", id: ");
	strcat(d, sir[0]);
	d[strlen(d)] = NULL;
}

void afisare_grupe()
{
	char a[500], d[500];
	char s[300];
	int id;
	int ok1, ok2;
	ok2 = 1;
	SendMessage(view_listbox, LB_RESETCONTENT, 0, 0);
	id = SendMessage(view_combobox, CB_GETCURSEL, 0, 0);
	SendMessageA(view_combobox, CB_GETLBTEXT, id, (LPARAM)a);
	initializare_nr_copii(ok1);
	if (strcmp(a, "Grupa 1") == 0)
	{
		if (nr_copii_grupe[1] != 0)
			strcpy(d, "grupa1");
		else ok2 = 0;
	}
	else if (strcmp(a, "Grupa 2") == 0)
	{
		if (nr_copii_grupe[2] != 0)
			strcpy(d, "grupa2");
		else ok2 = 0;
	}
	else if (strcmp(a, "Grupa 3") == 0)
	{
		if (nr_copii_grupe[3] != 0)
			strcpy(d, "grupa3");
		else ok2 = 0;
	}
	else if (strcmp(a, "Total") == 0)
	{
		if (nr_copii_grupe[1] != 0 || nr_copii_grupe[2] != 0 || nr_copii_grupe[3] != 0)
			strcpy(d, "copii");
		else ok2 = 0;
	}
	else strcpy(d, "a");
	if (ok2 == 0)
		strcpy(d, "abcde");
	std::ifstream fin(d);
	if (strlen(d) > 3)
	{
		if (ok1 == 1)
		{

			if (fin.good())
			{
				if (strcmp(a, "Total") != 0)
					strcat(a, " are urmatorii copii inscrisi:");
				else strcat(a, " copii inscrisi: ");
				SendMessageA(view_listbox, LB_ADDSTRING, 0, (LPARAM)a);
				while (fin.getline(s, 300))
				{
					char d[300];
					construire_sir_afisari(s, d);
					SendMessageA(view_listbox, LB_ADDSTRING, 0, (LPARAM)d);
				}
			}
			else SendMessageA(view_listbox, LB_ADDSTRING, 0, (LPARAM) "Nu exista copii inscrisi in grupa curent selectata!");
		}
		else SendMessageA(view_listbox, LB_ADDSTRING, 0, (LPARAM) "Nu exista copii inscrisi la gradinita!");
	}
	else SendMessageA(view_listbox, LB_ADDSTRING, 0, (LPARAM) "Nu ati selectat corect modul de afisare dorit!");

}

LRESULT CALLBACK WND_View_Window(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CHAR:
	{
		if (wParam == VK_ESCAPE)
		{
			ShowWindow(main_window, 5);
			DestroyWindow(view_window);
		}
		break;
	}
	case WM_CREATE:
		view_combobox = CreateWindow("Combobox", "", WS_BORDER | WS_CHILD | WS_VISIBLE | CBS_SORT | CBS_DROPDOWNLIST | CBS_HASSTRINGS, 10, 10, 200, 100, hwnd, (HMENU)4, 0, 0);
		view_listbox = CreateWindow("Listbox", "", WS_BORDER | WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL | LBS_DISABLENOSCROLL, 10, 40, 760, 480, hwnd, 0, 0, 0);
		CreateWindow("Button", "View", WS_BORDER | WS_VISIBLE | WS_CHILD, 225, 10, 70, 20, hwnd, (HMENU)2, 0, 0);
		CreateWindow("Button", "Back", WS_BORDER | WS_VISIBLE | WS_CHILD, 350, 515, 70, 20, hwnd, (HMENU)3, 0, 0);
		add_combobox();
		view_initial();
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case 2:
			afisare_grupe();
			break;
		case 3:
			ShowWindow(main_window, 5);
			DestroyWindow(view_window);
			break;
		default:
			break;
		}
		switch (HIWORD(wParam))
		{
		case CBN_SELCHANGE:
		{
			if (LOWORD(wParam) == 4)
				afisare_grupe();
		}
		break;
		}
		break;
	case WM_CLOSE:
		ShowWindow(main_window, 5);
		DestroyWindow(view_window);
		break;
	case WM_DESTROY:
		//PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
}

int nrcopiitotal;

void initializare_search_combo_list()
{
	SendMessageA(search_combobox, CB_ADDSTRING, 0, (LPARAM) "Nume");
	SendMessageA(search_combobox, CB_ADDSTRING, 0, (LPARAM) "Prenume");
	SendMessageA(search_combobox, CB_ADDSTRING, 0, (LPARAM) "ID");
	SendMessageA(search_combobox, CB_ADDSTRING, 0, (LPARAM) "Nume mama");
	SendMessageA(search_combobox, CB_ADDSTRING, 0, (LPARAM) "Nume tata");
	SendMessageA(search_combobox, CB_ADDSTRING, 0, (LPARAM) "Varsta");
	SendMessageA(search_combobox, CB_ADDSTRING, 0, (LPARAM) "Grupa");
	SendMessageA(search_listbox, LB_ADDSTRING, 0, (LPARAM) "Selectati un parametru de cautare si introduceti termenul de cautat!");
}

struct copil
{
	long long id;
	char nume[25];
	char prenume[25];
	char adresa[45];
	int varsta;
	char nume_mama[25];
	char nume_tata[25];
	int grupa;

}copil[76];

void construire_struct()
{
	std::ifstream fin("copii");
	char s[300], *p, sep[] = "|";
	int i = 0;
	while (fin.getline(s, 299))
	{
		p = strtok(s, sep);
		copil[i].id = atoll(p);
		p = strtok(NULL, sep);
		strcpy(copil[i].nume, p);
		p = strtok(NULL, sep);
		strcpy(copil[i].prenume, p);
		p = strtok(NULL, sep);
		copil[i].varsta = atoi(p);
		p = strtok(NULL, sep);
		strcpy(copil[i].adresa, p);
		p = strtok(NULL, sep);
		strcpy(copil[i].nume_mama, p);
		p = strtok(NULL, sep);
		strcpy(copil[i].nume_tata, p);
		p = strtok(NULL, sep);
		copil[i].grupa = atoi(p);
		i++;
	}
	nrcopiitotal = i;
}

void debug_struct()
{
	std::ofstream fout("temp");
	int j = 0;
	for (j = 0;j < nrcopiitotal;j++)
		fout << copil[j].id << '\n';
}

struct printchar
{
	char print[300];
};

printchar construire_print(int i)
{
	printchar a;
	char varsta[2], grupa[2];
	char idchar[100];
	long long id_long;
	int j;
	j = 0;
	id_long = copil[i].id;
	while (id_long > 0)
	{
		idchar[j] = (char)(id_long % 10 + '0');
		id_long = id_long / 10;
		j++;
	}
	grupa[0] = (char)(copil[i].grupa + '0');
	grupa[1] = NULL;
	idchar[j] = NULL;
	_strrev_(idchar);
	varsta[0] = (char)(copil[i].varsta + '0');
	varsta[1] = NULL;
	strcpy(a.print, "- ");
	strcat(a.print, "ID: ");
	strcat(a.print, idchar);
	strcat(a.print, ", Nume: ");
	strcat(a.print, copil[i].nume);
	strcat(a.print, ", Prenume: ");
	strcat(a.print, copil[i].prenume);
	strcat(a.print, ", Grupa: ");
	strcat(a.print, grupa);
	strcat(a.print, ", Varsta: ");
	strcat(a.print, varsta);
	strcat(a.print, ", Adresa: ");
	strcat(a.print, copil[i].adresa);
	strcat(a.print, ", Numele mamei: ");
	strcat(a.print, copil[i].nume_mama);
	strcat(a.print, ", Numele tatalui: ");
	strcat(a.print, copil[i].nume_tata);
	return a;
}

void itemsearch(char* parametru, char* searchtext, int &ok, SIZE &lungimepixeli, long &maxlong, int i, int permite)
{
	if (permite == 1)
		_strlwr_(parametru);
	if (strstr(parametru, _strlwr_(searchtext)) != 0)
	{
		ok = 1;
		printchar a;
		a = construire_print(i);
		GetTextExtentPoint32A(GetDC(search_listbox), a.print, strlen(a.print), &lungimepixeli);
		if (lungimepixeli.cx > maxlong)
			maxlong = lungimepixeli.cx;
		SendMessageA(search_listbox, LB_ADDSTRING, 0, (LPARAM)a.print);
	}
}

void search_procedure()
{
	construire_struct();
	int gwtstats = 0;
	char searchtext[45];
	gwtstats = GetWindowTextA(search_textbox, searchtext, 45);
	if (gwtstats == 0)
		MessageBoxA(0, "Nu ati completat cuvantul de cautat!", "Error!", MB_OK | MB_ICONWARNING);
	else
	{
		SIZE lungimepixeli;
		int id; char a[50];
		id = SendMessage(search_combobox, CB_GETCURSEL, 0, 0);
		SendMessageA(search_combobox, CB_GETLBTEXT, id, (LPARAM)a);
		if (strcmp(a, "Nume") != 0 && strcmp(a, "Prenume") != 0 && strcmp(a, "Nume mama") != 0 && strcmp(a, "Nume tata") != 0 && strcmp(a, "Varsta") != 0 && strcmp(a, "ID") != 0 && strcmp(a, "Grupa") != 0)
			MessageBoxA(0, "Nu ati selectat cheia de cautare (cautare dupa)!", "Error!", MB_OK | MB_ICONWARNING);
		else
		{
			long maxlong = 0;
			_strlwr_(searchtext);
			int ok = 0, i;
			char parametru[45];
			SendMessage(search_listbox, LB_RESETCONTENT, 0, 0);
			if (strcmp(a, "Nume") == 0)
			{
				for (i = 0;i < nrcopiitotal;i++)
				{
					strcpy(parametru, copil[i].nume);
					itemsearch(parametru, searchtext, ok, lungimepixeli, maxlong, i, 1);
				}
			}
			else if (strcmp(a, "Prenume") == 0)
			{
				for (i = 0;i < nrcopiitotal;i++)
				{
					strcpy(parametru, copil[i].prenume);
					itemsearch(parametru, searchtext, ok, lungimepixeli, maxlong, i, 1);
				}
			}
			else if (strcmp(a, "ID") == 0)
			{
				//debug_struct();
				for (i = 0;i < nrcopiitotal;i++)
				{
					char idchar[100];
					long long id_long;
					id_long = copil[i].id;
					int j = 0;
					while (id_long > 0)
					{
						idchar[j] = (char)(id_long % 10 + '0');
						id_long = id_long / 10;
						j++;
					}
					idchar[j] = NULL;
					_strrev_(idchar);
					strcpy(parametru, idchar);
					itemsearch(parametru, searchtext, ok, lungimepixeli, maxlong, i, 0);
				}
			}
			else if (strcmp(a, "Varsta") == 0)
			{
				for (i = 0;i < nrcopiitotal;i++)
				{
					char varsta[2];
					varsta[0] = (char)(copil[i].varsta + '0');
					varsta[1] = NULL;
					strcpy(parametru, varsta);
					itemsearch(parametru, searchtext, ok, lungimepixeli, maxlong, i, 0);
				}
			}
			else if (strcmp(a, "Grupa") == 0)
			{
				for (i = 0;i < nrcopiitotal;i++)
				{
					char grupa[2];
					grupa[0] = (char)(copil[i].grupa + '0');
					grupa[1] = NULL;
					strcpy(parametru, grupa);
					itemsearch(parametru, searchtext, ok, lungimepixeli, maxlong, i, 0);
				}
			}
			else if (strcmp(a, "Nume mama") == 0)
			{

				for (i = 0;i < nrcopiitotal;i++)
				{
					strcpy(parametru, copil[i].nume_mama);
					itemsearch(parametru, searchtext, ok, lungimepixeli, maxlong, i, 1);
				}
			}
			else if (strcmp(a, "Nume tata") == 0)
			{
				for (i = 0;i < nrcopiitotal;i++)
				{
					strcpy(parametru, copil[i].nume_tata);
					itemsearch(parametru, searchtext, ok, lungimepixeli, maxlong, i, 1);
				}
			}
			if (ok == 0)
			{
				SendMessageA(search_listbox, LB_ADDSTRING, 0, (LPARAM) "Nu au fost gasite inregistrari conforme cu parametrul cautat!");
			}
			maxlong += 10;
			SendMessageA(search_listbox, LB_SETHORIZONTALEXTENT, maxlong, 0);
		}

	}


}

void search_procedure_mod()
{
	construire_struct();
	int gwtstats = 0;
	char searchtext[45];
	gwtstats = GetWindowTextA(search_textbox, searchtext, 45);
	if (gwtstats == 0)
	{
		//MessageBoxA(0, "Nu ati completat cuvantul de cautat!", "Error!", MB_OK | MB_ICONWARNING);
		SendMessage(search_listbox, LB_RESETCONTENT, 0, 0);
		SendMessageA(search_listbox, LB_ADDSTRING, 0, (LPARAM) "Nu ati completat cuvantul de cautat!");
	}
	else
	{
		SIZE lungimepixeli;
		int id; char a[50];
		id = SendMessage(search_combobox, CB_GETCURSEL, 0, 0);
		SendMessageA(search_combobox, CB_GETLBTEXT, id, (LPARAM)a);
		if (strcmp(a, "Nume") != 0 && strcmp(a, "Prenume") != 0 && strcmp(a, "Nume mama") != 0 && strcmp(a, "Nume tata") != 0 && strcmp(a, "Varsta") != 0 && strcmp(a, "ID") != 0 && strcmp(a, "Grupa") != 0)
			MessageBoxA(0, "Nu ati selectat cheia de cautare (cautare dupa)!", "Error!", MB_OK | MB_ICONWARNING);
		else
		{
			long maxlong = 0;
			_strlwr_(searchtext);
			int ok = 0, i;
			char parametru[45];
			SendMessage(search_listbox, LB_RESETCONTENT, 0, 0);
			if (strcmp(a, "Nume") == 0)
			{
				for (i = 0;i < nrcopiitotal;i++)
				{
					strcpy(parametru, copil[i].nume);
					itemsearch(parametru, searchtext, ok, lungimepixeli, maxlong, i, 1);
				}
			}
			else if (strcmp(a, "Prenume") == 0)
			{
				for (i = 0;i < nrcopiitotal;i++)
				{
					strcpy(parametru, copil[i].prenume);
					itemsearch(parametru, searchtext, ok, lungimepixeli, maxlong, i, 1);
				}
			}
			else if (strcmp(a, "ID") == 0)
			{
				//debug_struct();
				for (i = 0;i < nrcopiitotal;i++)
				{
					char idchar[100];
					long long id_long;
					id_long = copil[i].id;
					int j = 0;
					while (id_long > 0)
					{
						idchar[j] = (char)(id_long % 10 + '0');
						id_long = id_long / 10;
						j++;
					}
					idchar[j] = NULL;
					_strrev_(idchar);
					strcpy(parametru, idchar);
					itemsearch(parametru, searchtext, ok, lungimepixeli, maxlong, i, 0);
				}
			}
			else if (strcmp(a, "Varsta") == 0)
			{
				for (i = 0;i < nrcopiitotal;i++)
				{
					char varsta[2];
					varsta[0] = (char)(copil[i].varsta + '0');
					varsta[1] = NULL;
					strcpy(parametru, varsta);
					itemsearch(parametru, searchtext, ok, lungimepixeli, maxlong, i, 0);
				}
			}
			else if (strcmp(a, "Grupa") == 0)
			{
				for (i = 0;i < nrcopiitotal;i++)
				{
					char grupa[2];
					grupa[0] = (char)(copil[i].grupa + '0');
					grupa[1] = NULL;
					strcpy(parametru, grupa);
					itemsearch(parametru, searchtext, ok, lungimepixeli, maxlong, i, 0);
				}
			}
			else if (strcmp(a, "Nume mama") == 0)
			{

				for (i = 0;i < nrcopiitotal;i++)
				{
					strcpy(parametru, copil[i].nume_mama);
					itemsearch(parametru, searchtext, ok, lungimepixeli, maxlong, i, 1);
				}
			}
			else if (strcmp(a, "Nume tata") == 0)
			{
				for (i = 0;i < nrcopiitotal;i++)
				{
					strcpy(parametru, copil[i].nume_tata);
					itemsearch(parametru, searchtext, ok, lungimepixeli, maxlong, i, 1);
				}
			}
			if (ok == 0)
			{
				SendMessageA(search_listbox, LB_ADDSTRING, 0, (LPARAM) "Nu au fost gasite inregistrari conforme cu parametrul cautat!");
			}
			maxlong += 10;
			SendMessageA(search_listbox, LB_SETHORIZONTALEXTENT, maxlong, 0);
		}

	}

}

LRESULT CALLBACK WND_Search_Window(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CHAR:
	{
		if (wParam == VK_ESCAPE)
		{
			ShowWindow(main_window, 5);
			DestroyWindow(search_window);
		}
		break;
	}
	case WM_CREATE:
		CreateWindow("Button", "Search", WS_BORDER | WS_VISIBLE | WS_CHILD, 680, 5, 70, 20, hwnd, (HMENU)1, 0, 0);
		CreateWindow("Button", "Back", WS_VISIBLE | WS_CHILD | WS_BORDER, 365, 515, 70, 20, hwnd, (HMENU)2, 0, 0);
		search_combobox = CreateWindow("Combobox", "", WS_BORDER | WS_CHILD | WS_VISIBLE | CBS_HASSTRINGS | CBS_SORT | CBS_DROPDOWNLIST, 110, 5, 190, 200, hwnd, (HMENU)4, 0, 0);
		search_textbox = CreateWindow("Edit", "", WS_BORDER | WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL, 490, 5, 180, 20, hwnd, (HMENU)3, 0, 0);
		search_listbox = CreateWindow("Listbox", "", WS_BORDER | WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL | LBS_DISABLENOSCROLL, 10, 50, 760, 450, hwnd, 0, 0, 0);
		//SendMessageA(search_listbox, LB_SETHORIZONTALEXTENT, 1000, 0);
		initializare_search_combo_list();
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case 2:
			ShowWindow(main_window, 5);
			DestroyWindow(search_window);
			break;
		case 1:
		{
			search_procedure();
		}
		break;
		}
		switch (HIWORD(wParam))
		{
		case EN_CHANGE:
		{
			if (LOWORD(wParam) == 3)
				search_procedure_mod();

		}
		break;
		case CBN_SELCHANGE:
		{
			if (LOWORD(wParam) == 4)
				search_procedure_mod();
		}
		break;
		}
		break;
	case WM_CLOSE:
		ShowWindow(main_window, 5);
		DestroyWindow(search_window);
		break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hDC;
		hDC = BeginPaint(hwnd, &ps);
		TextOut(hDC, 10, 10, " Cautare dupa: ", strlen(" Cautare dupa: "));
		TextOut(hDC, 300, 10, " Inserati termenul de cautat: ", strlen(" Inserati termenul de cautat: "));
		TextOut(hDC, 10, 30, " Rezultate: ", strlen(" Rezultate: "));
		EndPaint(hwnd, &ps);
		return 0;
	}
	case WM_DESTROY:
		//PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
}

int nr_edit = 0;

void construire_listbox_edit()
{
	construire_struct();
	long long maxlong = 0;
	SIZE lungime;
	SendMessage(edit_listbox, LB_RESETCONTENT, 0, 0);
	printchar b;
	int i = 0;
	for (i = 0;i < nrcopiitotal;i++)
	{
		b = construire_print(i);
		GetTextExtentPoint32A(GetDC(edit_listbox), b.print, strlen(b.print), &lungime);
		if (lungime.cx > maxlong)
			maxlong = lungime.cx;
		SendMessageA(edit_listbox, LB_ADDSTRING, 0, (LPARAM)b.print);
	}
	SendMessageA(edit_listbox, LB_SETHORIZONTALEXTENT, maxlong + 10, 0);
}

void edit_procedure()
{
	int i = 0;
	i = SendMessage(edit_listbox, LB_GETCURSEL, 0, 0);
	if (i < 0)
	{
		MessageBoxA(0, "Nu ati selectat o inregistrare!", "Error!", MB_OK | MB_ICONWARNING);
	}
	else
	{
		nr_edit = i;
		int x = 400;
		int y = 300;
		editbox_window = CreateWindow("EditBox_Window", "Editare", WS_SYSMENU | WS_MINIMIZEBOX, calculare_xpos(x), calculare_ypos(y), x, y, HWND_DESKTOP, 0, 0, 0);
		ShowWindow(edit_window, 0);
		ShowWindow(editbox_window, 5);
		UpdateWindow(editbox_window);
	}
}

LRESULT CALLBACK WND_Edit_Window(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CHAR:
	{
		if (wParam == VK_ESCAPE)
		{
			ShowWindow(main_window, 5);
			DestroyWindow(edit_window);
		}
		break;
	}
	case WM_CREATE:
		CreateWindow("Button", "Back", WS_BORDER | WS_CHILD | WS_VISIBLE, 515, 515, 70, 30, hwnd, (HMENU)2, 0, 0);
		CreateWindow("Button", "Edit", WS_BORDER | WS_CHILD | WS_VISIBLE, 175, 515, 70, 30, hwnd, (HMENU)1, 0, 0);
		edit_listbox = CreateWindow("Listbox", "", WS_BORDER | WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL | LBS_DISABLENOSCROLL, 10, 50, 760, 450, hwnd, 0, 0, 0);
		construire_listbox_edit();
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case 2:
		{
			ShowWindow(main_window, 5);
			DestroyWindow(edit_window);
			break;
		}
		case 1:
		{
			edit_procedure();
		}
		break;
		}
		break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hDC;
		hDC = BeginPaint(hwnd, &ps);
		TextOut(hDC, 10, 10, " Selectati o inregistrare si apasati butonul edit pentru a o edita! ", strlen(" Selectati o inregistrare si apasati butonul edit pentru a o edita! "));
		TextOut(hDC, 10, 30, " Inregistrari: ", strlen(" Inregistrari: "));
		EndPaint(hwnd, &ps);
		return 0;
	}
	case WM_CLOSE:
		ShowWindow(main_window, 5);
		DestroyWindow(edit_window);
		break;
	case WM_DESTROY:
		//PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
}

void initializare_editbox()
{
	char c[2];
	c[0] = copil[nr_edit].varsta + '0';
	c[1] = NULL;
	SendMessageA(editboxnume, EM_REPLACESEL, 0, (LPARAM)copil[nr_edit].nume);
	SendMessageA(editboxprenume, EM_REPLACESEL, 0, (LPARAM)copil[nr_edit].prenume);
	SendMessageA(editboxvarsta, EM_REPLACESEL, 0, (LPARAM)c);
	SendMessageA(editboxadresa, EM_REPLACESEL, 0, (LPARAM)copil[nr_edit].adresa);
	SendMessageA(editboxnume_mama, EM_REPLACESEL, 0, (LPARAM)copil[nr_edit].nume_mama);
	SendMessageA(editboxnume_tata, EM_REPLACESEL, 0, (LPARAM)copil[nr_edit].nume_tata);
}

void refacere_listbox_edit()
{
	long long maxlong = 0;
	SIZE lungime;
	SendMessage(edit_listbox, LB_RESETCONTENT, 0, 0);
	printchar b;
	int i = 0;
	for (i = 0;i < nrcopiitotal;i++)
	{
		b = construire_print(i);
		GetTextExtentPoint32A(GetDC(edit_listbox), b.print, strlen(b.print), &lungime);
		if (lungime.cx > maxlong)
			maxlong = lungime.cx;
		SendMessageA(edit_listbox, LB_ADDSTRING, 0, (LPARAM)b.print);
	}
	SendMessageA(edit_listbox, LB_SETHORIZONTALEXTENT, maxlong + 10, 0);
}

void reconstruire_fisiere_edit_procedure(int r)
{
	refacere_fisier_nr_locuri();
	refacere_listbox_edit();
	std::ofstream f("copii");
	std::ofstream g1("grupa1");
	std::ofstream g2("grupa2");
	std::ofstream g3("grupa3");
	int i;
	for (i = 0;i < nrcopiitotal;i++)
	{
		f << copil[i].id << "|" << copil[i].nume << "|" << copil[i].prenume << "|" << copil[i].varsta << "|" << copil[i].adresa << "|" << copil[i].nume_mama << "|" << copil[i].nume_tata << "|" << copil[i].grupa << '\n';
		if (copil[i].grupa == 1)
		{
			g1 << copil[i].id << "|" << copil[i].nume << "|" << copil[i].prenume << "|" << copil[i].varsta << "|" << copil[i].adresa << "|" << copil[i].nume_mama << "|" << copil[i].nume_tata << '\n';
		}
		else if (copil[i].grupa == 2)
		{
			g2 << copil[i].id << "|" << copil[i].nume << "|" << copil[i].prenume << "|" << copil[i].varsta << "|" << copil[i].adresa << "|" << copil[i].nume_mama << "|" << copil[i].nume_tata << '\n';
		}
		else g3 << copil[i].id << "|" << copil[i].nume << "|" << copil[i].prenume << "|" << copil[i].varsta << "|" << copil[i].adresa << "|" << copil[i].nume_mama << "|" << copil[i].nume_tata << '\n';
	}
	std::ofstream l;
	l.open("logs", std::ofstream::app);
	time_t rawtime;
	struct tm * timeinfo;
	time(&rawtime);
	timeinfo = localtime(&rawtime);
	l << /*"<"<<*/ asctime(timeinfo) << "< " << ": [EditEvent]: Datele de inregistrare ale copilului " << copil[nr_edit].nume << " " << copil[nr_edit].prenume << " au fost modificate " << ";> \n";
	l.close();
}

void save_edit_procedure()
{
	char nume[30], prenume[30], adresa[45], nume_mama[30], nume_tata[30], varsta[30];
	int varsta_int;
	int gwtstat1 = 0, gwtstat2 = 0, gwtstat3 = 0, gwtstat4 = 0, gwtstat5 = 0, gwtstat6 = 0;
	gwtstat1 = GetWindowTextA(editboxnume, nume, 24);
	gwtstat2 = GetWindowTextA(editboxprenume, prenume, 24);
	gwtstat3 = GetWindowTextA(editboxvarsta, varsta, 24);
	gwtstat4 = GetWindowTextA(editboxadresa, adresa, 44);
	gwtstat5 = GetWindowTextA(editboxnume_mama, nume_mama, 24);
	gwtstat6 = GetWindowTextA(editboxnume_tata, nume_tata, 24);
	if (gwtstat1 == 0 || gwtstat2 == 0 || gwtstat3 == 0 || gwtstat4 == 0 || gwtstat5 == 0 || gwtstat6 == 0)
		MessageBoxA(0, "Toate campurile trebuiesc completate!", "Eroare!", MB_OK | MB_ICONWARNING);
	else if (verifica_varsta_char(varsta) == 1)
	{
		long long id = 0;
		varsta_int = atoi(varsta);
		calculeaza_id(id, nume, prenume, varsta_int, adresa, nume_mama, nume_tata, 0);
		if (id == -1)
		{
			int i;
			for (i = 1;i < 9;i++)
				if (verificari[i] == 0)
					break;
			switch (i)
			{
			case 1:
			{
				MessageBoxA(0, "Nume invalid! Trebuie sã contina doar litere!", "Error!", MB_OK | MB_ICONWARNING);
			}
			break;
			case 2:
			{
				MessageBoxA(0, "Prenume invalid! Trebuie sã contina doar litere!", "Error!", MB_OK | MB_ICONWARNING);
			}
			break;
			case 3:
			{
				MessageBoxA(0, "Varsta invalida! Varsta trebuie sa fie intre 0 si 7 ani!", "Error!", MB_OK | MB_ICONWARNING);
			}
			break;
			case 4:
			{
				MessageBoxA(0, "Adresa invalida! Poate sa contina doar litere, cifre, punct, virgula, cratima!", "Error!", MB_OK | MB_ICONWARNING);
			}
			break;
			case 5:
			{
				MessageBoxA(0, "Numele mamei invalid! Trebuie sã contina doar litere!", "Error!", MB_OK | MB_ICONWARNING);
			}
			break;
			case 6:
			{
				MessageBoxA(0, "Numele tatalui invalid! Trebuie sã contina doar litere!", "Error!", MB_OK | MB_ICONWARNING);
			}
			break;
			case 7:
			{
				MessageBoxA(0, "Nu este loc disponibil in grupa pentru a inscrie copilul!", "Error!", MB_OK | MB_ICONWARNING);
			}
			break;
			case 8:
			{
				MessageBoxA(0, "Copilul este deja inscris!", "Error!", MB_OK | MB_ICONWARNING);
			}
			break;
			}
		}
		else
		{
			int sigur = MessageBoxA(0, "Sunteti sigur ca doriti sa salvati editarea?", "Confirmare inregistrare!", MB_YESNO | MB_ICONQUESTION);
			switch (sigur)
			{
			case IDYES:
			{
				int varsta_int;
				varsta_int = atoi(varsta);
				int grupa = stabilire_grupa(varsta_int);
				int ok = 0;
				if (grupa != copil[nr_edit].grupa)
				{
					switch (grupa)
					{
					case 1:
					{
						if (locuri_ocupate[1] + 1 > 25)
							MessageBox(0, "Modificarea nu a putut fi salvata! Numar locuri indisponibil in noua grupa!", "Error", MB_OK | MB_ICONWARNING);
						else
						{
							ok = 1;
							locuri_ocupate[1]++;
							locuri_ocupate[copil[nr_edit].grupa]--;
						}
						break;
					}
					case 2:
					{if (locuri_ocupate[2] + 1 > 25)
						MessageBox(0, "Modificarea nu a putut fi salvata! Numar locuri indisponibil in noua grupa!", "Error", MB_OK | MB_ICONWARNING);
					else
					{
						ok = 1;
						locuri_ocupate[2]++;
						locuri_ocupate[copil[nr_edit].grupa]--;
					}
					break;
					}
					case 3:
					{if (locuri_ocupate[3] + 1 > 25)
						MessageBox(0, "Modificarea nu a putut fi salvata! Numar locuri indisponibil in noua grupa!", "Error", MB_OK | MB_ICONWARNING);
					else
					{
						ok = 1;
						locuri_ocupate[3]++;
						locuri_ocupate[copil[nr_edit].grupa]--;
					}
					break;
					}
					}
				}
				else ok = 1;
				if (ok == 1)
				{
					copil[nr_edit].id = id;
					strcpy(copil[nr_edit].nume, nume);
					strcpy(copil[nr_edit].prenume, prenume);
					strcpy(copil[nr_edit].nume_mama, nume_mama);
					strcpy(copil[nr_edit].nume_tata, nume_tata);
					strcpy(copil[nr_edit].adresa, adresa);
					copil[nr_edit].varsta = atoi(varsta);
					copil[nr_edit].grupa = stabilire_grupa(copil[nr_edit].varsta);
					reconstruire_fisiere_edit_procedure(copil[nr_edit].grupa);
					MessageBox(0, "Modificari salvate cu succes!", "Done", MB_OK | MB_ICONINFORMATION);
					ShowWindow(edit_window, 5);
					DestroyWindow(editbox_window);
				}
			}
			break;
			case IDNO:
				break;
			}
		}
	}
	else MessageBoxA(0, "Varsta trebuie sa contina maxim 2 cifre!", "Eroare!", MB_OK | MB_ICONWARNING);


}

LRESULT CALLBACK WND_EditBox_Window(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CHAR:
	{
		if (wParam == VK_ESCAPE)
		{
			ShowWindow(main_window, 5);
			DestroyWindow(editbox_window);
		}
		break;
	}
	case WM_CREATE:
		editboxnume = CreateWindow("Edit", "", WS_BORDER | WS_CHILD | WS_VISIBLE, 150, 10, 200, 20, hwnd, 0, 0, 0);
		editboxprenume = CreateWindow("Edit", "", WS_BORDER | WS_CHILD | WS_VISIBLE, 150, 40, 200, 20, hwnd, 0, 0, 0);
		editboxvarsta = CreateWindow("Edit", "", WS_BORDER | WS_CHILD | WS_VISIBLE, 150, 70, 200, 20, hwnd, 0, 0, 0);
		editboxadresa = CreateWindow("Edit", "", WS_BORDER | WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL, 150, 100, 200, 20, hwnd, 0, 0, 0);
		editboxnume_mama = CreateWindow("Edit", "", WS_BORDER | WS_CHILD | WS_VISIBLE, 150, 130, 200, 20, hwnd, 0, 0, 0);
		editboxnume_tata = CreateWindow("Edit", "", WS_BORDER | WS_CHILD | WS_VISIBLE, 150, 160, 200, 20, hwnd, 0, 0, 0);
		CreateWindow("Button", "Cancel", WS_BORDER | WS_CHILD | WS_VISIBLE, 232, 200, 100, 30, hwnd, (HMENU)1, 0, 0);
		CreateWindow("Button", "Salveaza", WS_BORDER | WS_CHILD | WS_VISIBLE, 66, 200, 100, 30, hwnd, (HMENU)2, 0, 0);
		initializare_editbox();
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case 2:
		{
			save_edit_procedure();
		}
		break;
		case 1:
		{
			ShowWindow(edit_window, 5);
			DestroyWindow(editbox_window);
		}
		break;
		}
		break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hDC;
		hDC = BeginPaint(hwnd, &ps);
		TextOut(hDC, 10, 12, "Nume: ", strlen("Nume: "));
		TextOut(hDC, 10, 42, "Prenume: ", strlen("Prenume: "));
		TextOut(hDC, 10, 72, "Varsta: ", strlen("Varsta: "));
		TextOut(hDC, 10, 102, "Adresa: ", strlen("Adresa: "));
		TextOut(hDC, 10, 132, "Numele mamei: ", strlen("Numele mamei: "));
		TextOut(hDC, 10, 162, "Numele tatalui: ", strlen("Numele tatalui: "));
		EndPaint(hwnd, &ps);
		return 0;
	}
	case WM_CLOSE:
	{
		ShowWindow(edit_window, 5);
		DestroyWindow(editbox_window);
		break;
	}
	case WM_DESTROY:
		//PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
}

void construire_listbox_delete()
{
	construire_struct();
	long long maxlong = 0;
	SIZE lungime;
	SendMessage(delete_listbox, LB_RESETCONTENT, 0, 0);
	printchar b;
	int i = 0;
	for (i = 0;i < nrcopiitotal;i++)
	{
		b = construire_print(i);
		GetTextExtentPoint32A(GetDC(delete_listbox), b.print, strlen(b.print), &lungime);
		if (lungime.cx > maxlong)
			maxlong = lungime.cx;
		SendMessageA(delete_listbox, LB_ADDSTRING, 0, (LPARAM)b.print);
	}
	SendMessageA(delete_listbox, LB_SETHORIZONTALEXTENT, maxlong + 10, 0);
}

void reconstruire_listbox_delete()
{
	long long maxlong = 0;
	SIZE lungime;
	SendMessage(delete_listbox, LB_RESETCONTENT, 0, 0);
	printchar b;
	int i = 0;
	for (i = 0;i < nrcopiitotal;i++)
	{
		b = construire_print(i);
		GetTextExtentPoint32A(GetDC(delete_listbox), b.print, strlen(b.print), &lungime);
		if (lungime.cx > maxlong)
			maxlong = lungime.cx;
		SendMessageA(delete_listbox, LB_ADDSTRING, 0, (LPARAM)b.print);
	}
	SendMessageA(delete_listbox, LB_SETHORIZONTALEXTENT, maxlong + 10, 0);
}

void stergere_din_struct(int i)
{
	int j;
	std::ofstream fout;
	fout.open("logs", std::ofstream::app);
	time_t rawtime;
	struct tm * timeinfo;
	time(&rawtime);
	timeinfo = localtime(&rawtime);
	fout << /*"<"<<*/ asctime(timeinfo) << "< " << ": [DeleteEvent]: " << copil[i].nume << " " << copil[i].prenume << " cu id: " << copil[i].id << " a fost sters din grupa: " << copil[i].grupa << ";> \n";
	fout.close();
	for (j = i;j < nrcopiitotal - 1;j++)
		copil[j] = copil[j + 1];
	nrcopiitotal--;
}

void reconstruire_fisiere_delete_procedure(int r)
{
	std::ofstream f("copii");
	std::ofstream g;
	initializeza_locuri_gradinita();
	switch (r)
	{
	case 1:
	{
		g.open("grupa1");
		locuri_ocupate[1]--;
		break;
	}
	case 2:
	{
		g.open("grupa2");
		locuri_ocupate[2]--;
		break;
	}
	case 3:
	{
		g.open("grupa3");
		locuri_ocupate[3]--;
		break;
	}
	}
	refacere_fisier_nr_locuri();
	int i;
	for (i = 0;i < nrcopiitotal;i++)
	{
		f << copil[i].id << "|" << copil[i].nume << "|" << copil[i].prenume << "|" << copil[i].varsta << "|" << copil[i].adresa << "|" << copil[i].nume_mama << "|" << copil[i].nume_tata << "|" << copil[i].grupa << '\n';
		if (copil[i].grupa == r)
		{
			g << copil[i].id << "|" << copil[i].nume << "|" << copil[i].prenume << "|" << copil[i].varsta << "|" << copil[i].adresa << "|" << copil[i].nume_mama << "|" << copil[i].nume_tata << '\n';
		}
	}
}

void delete_procedure()
{
	int i = 0;
	i = SendMessage(delete_listbox, LB_GETCURSEL, 0, 0);
	if (i < 0)
	{
		MessageBoxA(0, "Nu ati selectat o inregistrare!", "Error!", MB_OK | MB_ICONWARNING);
	}
	else
	{
		int raspuns;
		raspuns = MessageBoxA(0, "Sunteti sigur ca doriti sa stergeti inregistrarea curent selectata?", "Confirmare", MB_YESNO | MB_ICONQUESTION);
		switch (raspuns)
		{
		case IDNO:
			break;
		case IDYES:
		{
			int r = copil[i].grupa;
			stergere_din_struct(i);
			reconstruire_fisiere_delete_procedure(r);
			reconstruire_listbox_delete();
			MessageBoxA(0, "Inregistrare stearsa!", "Stergere Confirmata", MB_OK | MB_ICONINFORMATION);
		}
		break;
		}
	}
}

void delete_all_procedure()
{
	int res = MessageBoxA(0, "Sunteti sigur ca doriti sa stergeti toate inregistrarile?", "Confirmare stergere totala!", MB_YESNO | MB_ICONSTOP);
	switch (res)
	{
	case IDYES:
	{
		remove("copii");
		remove("grupa1");
		remove("grupa2");
		remove("grupa3");
		remove("nrcopiigrupe");
		printare_in_logs(": [DeleteEvent]: Toate inregistrarile au fost sterse! ");
		SendMessage(delete_listbox, LB_RESETCONTENT, 0, 0);
		SendMessageA(delete_listbox, LB_ADDSTRING, 0, (LPARAM) "Toate înregistrarile au fost sterse!");
	}
	break;
	case IDNO:
		break;
	}
}

LRESULT CALLBACK WND_Delete_Window(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CHAR:
	{
		if (wParam == VK_ESCAPE)
		{
			ShowWindow(main_window, 5);
			DestroyWindow(delete_window);
		}
		break;
	}
	case WM_CREATE:
		CreateWindow("Button", "Back", WS_BORDER | WS_CHILD | WS_VISIBLE, 515, 515, 70, 30, hwnd, (HMENU)2, 0, 0);
		CreateWindow("Button", "Delete", WS_BORDER | WS_CHILD | WS_VISIBLE, 175, 515, 70, 30, hwnd, (HMENU)1, 0, 0);
		CreateWindow("Button", "Delete All", WS_BORDER | WS_CHILD | WS_VISIBLE, 345, 515, 70, 30, hwnd, (HMENU)3, 0, 0);
		delete_listbox = CreateWindow("Listbox", "", WS_BORDER | WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL | LBS_DISABLENOSCROLL, 10, 50, 760, 450, hwnd, 0, 0, 0);
		construire_listbox_delete();
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case 2:
			ShowWindow(main_window, 5);
			DestroyWindow(delete_window);
			break;
		case 1:
		{
			delete_procedure();
		}
		break;
		case 3:
		{
			delete_all_procedure();
		}
		break;
		}
		break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hDC;
		hDC = BeginPaint(hwnd, &ps);
		TextOut(hDC, 10, 10, " Selectati o inregistrare si apasati butonul delete pentru a o sterge! ", strlen(" Selectati o inregistrare si apasati butonul delete pentru a o sterge! "));
		TextOut(hDC, 10, 30, " Inregistrari: ", strlen(" Inregistrari: "));
		EndPaint(hwnd, &ps);
		return 0;
	}
	case WM_CLOSE:
		ShowWindow(main_window, 5);
		DestroyWindow(delete_window);
		break;
	case WM_DESTROY:
		//PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
}

HANDLE login_textbox;

LRESULT CALLBACK WND_Login_Window(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CHAR:
	{
		if (wParam == VK_ESCAPE)
		{
			//ShowWindow(main_window, 5);
			DestroyWindow(login_window);
		}
		break;
	}
	case WM_CREATE:
		CreateWindow("Button", "Exit", WS_BORDER | WS_CHILD | WS_VISIBLE, 10, 100, 70, 30, hwnd, (HMENU)2, 0, 0);
		CreateWindow("Button", "Login", WS_BORDER | WS_CHILD | WS_VISIBLE, 100, 100, 70, 30, hwnd, (HMENU)1, 0, 0);
		login_textbox = CreateWindow("Edit", "", WS_BORDER | WS_CHILD | WS_VISIBLE, 10, 50, 100, 30, hwnd, (HMENU)3, 0, 0);
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case 2:
			//ShowWindow(main_window, 5);
			DestroyWindow(login_window);
			break;
		case 1:
		{
			//delete_procedure();

			int x = 300;
			int y = 500;
			main_window = CreateWindowEx(WS_EX_ACCEPTFILES, "Main_Window", "Meniu", WS_SYSMENU | WS_MINIMIZEBOX, calculare_xpos(x), calculare_ypos(y), x, y, HWND_DESKTOP, 0, 0, 0);
			ShowWindow(main_window, 5);
			ShowWindow(login_window, 0);

		}
		break;
		}
		break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hDC;
		hDC = BeginPaint(hwnd, &ps);
		//TextOut(hDC, 10, 10, L" Introduceti codul de manager! ", strlen(" Selectati o inregistrare si apasati butonul delete pentru a o sterge! "));
		//TextOut(hDC, 10, 30, L" Inregistrari: ", strlen(" Inregistrari: "));
		EndPaint(hwnd, &ps);
		return 0;
	}
	case WM_CLOSE:
		//ShowWindow(main_window, 5);
		DestroyWindow(login_window);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
}

void declaratii_WINDCLASSEX_fara_lpsz_class_name_si_wndproc(WNDCLASSEX &a, HINSTANCE hInstance)
{
	a.cbSize = sizeof(WNDCLASSEX);
	a.cbClsExtra = 0;
	a.cbWndExtra = 0;
	a.hIconSm = 0;

	a.hInstance = hInstance;
	a.style = CS_HREDRAW | CS_VREDRAW;
	a.lpszMenuName = 0;
	a.hCursor = LoadCursor(0, IDC_ARROW);
	a.hIcon = LoadIcon(0, IDI_SHIELD);
	a.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
}

void declaratii_WINDCLASSEX(HINSTANCE hInstance)
{
	mainw.lpfnWndProc = WND_Main_Window;
	mainw.lpszClassName = "Main_Window";
	logsw.lpfnWndProc = WND_Logs_Window;
	logsw.lpszClassName = "Logs_Window";
	registerw.lpfnWndProc = WND_Register_Window;
	registerw.lpszClassName = "Register_Window";
	vieww.lpfnWndProc = WND_View_Window;
	vieww.lpszClassName = "View_Window";
	searchw.lpfnWndProc = WND_Search_Window;
	searchw.lpszClassName = "Search_Window";
	editw.lpfnWndProc = WND_Edit_Window;
	editw.lpszClassName = "Edit_Window";
	deletew.lpfnWndProc = WND_Delete_Window;
	deletew.lpszClassName = "Delete_Window";
	editboxw.lpfnWndProc = WND_EditBox_Window;
	editboxw.lpszClassName = "EditBox_Window";
	loginw.lpfnWndProc = WND_Login_Window;
	loginw.lpszClassName = "Login_Window";
	declaratii_WINDCLASSEX_fara_lpsz_class_name_si_wndproc(loginw, hInstance);
	declaratii_WINDCLASSEX_fara_lpsz_class_name_si_wndproc(mainw, hInstance);
	declaratii_WINDCLASSEX_fara_lpsz_class_name_si_wndproc(logsw, hInstance);
	declaratii_WINDCLASSEX_fara_lpsz_class_name_si_wndproc(registerw, hInstance);
	declaratii_WINDCLASSEX_fara_lpsz_class_name_si_wndproc(vieww, hInstance);
	declaratii_WINDCLASSEX_fara_lpsz_class_name_si_wndproc(searchw, hInstance);
	declaratii_WINDCLASSEX_fara_lpsz_class_name_si_wndproc(editw, hInstance);
	declaratii_WINDCLASSEX_fara_lpsz_class_name_si_wndproc(deletew, hInstance);
	declaratii_WINDCLASSEX_fara_lpsz_class_name_si_wndproc(editboxw, hInstance);
}

void register_class_ex()
{
	RegisterClassEx(&mainw);
	RegisterClassEx(&logsw);
	RegisterClassEx(&registerw);
	RegisterClassEx(&vieww);
	RegisterClassEx(&searchw);
	RegisterClassEx(&editw);
	RegisterClassEx(&deletew);
	RegisterClassEx(&editboxw);
	RegisterClassEx(&loginw);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, PSTR cmdLine, int showCmd)
{
	hInst = hInstance;
	showcmd = showCmd;
	std::fstream fout;
	fout.open("temp", std::ofstream::app);
	fout.close();
	//STEP 1 - declar toate clasele pe care o sa le foloses

	declaratii_WINDCLASSEX(hInstance);

	//STEP2 - inregistrez toate clasele declarate

	register_class_ex();

	//STEP3

	int x = 300;
	int y = 500;
	main_window = CreateWindowEx(WS_EX_ACCEPTFILES, "Main_Window", "Meniu", WS_SYSMENU | WS_MINIMIZEBOX, calculare_xpos(x), calculare_ypos(y), x, y, HWND_DESKTOP, 0, 0, 0);

	//int x = 300;
	//int y = 300;
	//login_window = CreateWindowEx(WS_EX_ACCEPTFILES, L"Login_Window", L"Login", WS_SYSMENU | WS_MINIMIZEBOX, calculare_xpos(x), calculare_ypos(y), x, y, HWND_DESKTOP, 0, hInstance, 0);

	//STEP4

	if (main_window == 0)
	{
		MessageBoxA(0, "No window!", "Error!", MB_ICONERROR);
		exit(0);
	}
	//STEP5

	ShowWindow(main_window, showCmd);

	//STEP6

	UpdateWindow(main_window);

	MSG msg;

	SecureZeroMemory(&msg, sizeof(MSG));
	int returnValue = 0;

	while ((returnValue = GetMessage(&msg, 0, 0, 0)) != 0)
	{
		if (returnValue == -1)
		{
			MessageBoxA(main_window, "Get Message Failed!", "Error!", MB_ICONERROR);
			break;
		}
		//if (IsDialogMessage(main_window, &msg)==0)
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	return (int)msg.wParam;
}
