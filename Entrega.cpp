#include <iostream>
#include <Windows.h>
#include "Resource.h" 
#include <CommDlg.h>
#include <CommCtrl.h>
#include <string>
#include <stdio.h>
#include <fstream>
#include <math.h>
#include <vector>
#include <algorithm>
#include <set>

using namespace std;

struct Citas {

	Citas* siguiente = nullptr;
	WCHAR fecha2[50];
	SYSTEMTIME fecha;
	WCHAR hora2[50];
	SYSTEMTIME hora;
	WCHAR cliente[50];
	WCHAR tel[50];
	WCHAR especie[50];
	WCHAR mascota[50];
	WCHAR motivo[100];
	WCHAR costo[50];
	WCHAR status[50];
	Citas* anterior = nullptr;

};
Citas* InicioC = nullptr, * FinC = nullptr, * indiceC;

struct Veterinario {

	Veterinario* siguiente;
	WCHAR usuario[50];
	WCHAR nombre[50];
	WCHAR contra[50];
	WCHAR cedula[100];
	WCHAR imagen[100];
	Citas* origen = nullptr;
	Citas* fin = nullptr;
	Veterinario* anterior;
};
Veterinario* InicioV = nullptr, * FinV = nullptr;

Veterinario* veterinarioActual;
Citas* citaActual;

#define MAX_LOADSTRING 100
#define IDC_TIMER1 1000

// Variables globales:
HINSTANCE hInst;                                // instancia actual
WCHAR szTitle[MAX_LOADSTRING];                  // Texto de la barra de t tulo
WCHAR szWindowClass[MAX_LOADSTRING];            // nombre de clase de la ventana principal
WCHAR ruta[300] = { 0 };
int n = 0;
WCHAR** esp = nullptr;
SYSTEMTIME fechact;


// Declaraciones de funciones adelantadas incluidas en este m dulo de c digo:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);

Veterinario* crearDoc(WCHAR* nombre, WCHAR* usuario, WCHAR* contra, WCHAR* cedula, WCHAR* imagen);
Citas* crearCita(WCHAR* cliente, WCHAR* mascota, WCHAR* especie, WCHAR* telefono, WCHAR* motivo, SYSTEMTIME fecha, SYSTEMTIME hora, WCHAR* fecha2, WCHAR* hora2, WCHAR* estado, WCHAR* precio);
void agregarDoctorInicio(Veterinario* dato);
void incluirCita(Citas* dato);
Citas* eliminarCita(WCHAR* nombre);
Citas* eliminarCitaInicio();
Citas* eliminarCitaFinal();
Veterinario* buscarNombre(WCHAR* buscar);
WCHAR* buscarContra(WCHAR* buscar);
void Guardarinfo();
void LeerInfo();
//void GuardarCitas();
//void LeerCitas();
LRESULT CALLBACK IniciarSesion(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK Registrarse(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK Agenda(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK Modificar(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK RegistroCita(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK InfoDoctor(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK ConsultarCita(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK ModificarDoctor(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
//MENU

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // Almacenar identificador de instancia en una variable global

	HWND hWnd = CreateDialog(hInst, MAKEINTRESOURCE(IDC_INICIARSESION), NULL, IniciarSesion);

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

bool menu(int opcion, HWND hwnd) {

	switch (opcion) {
	case ID_info: {
		HWND ventana = CreateDialog(hInst,
			MAKEINTRESOURCE(IDC_InfoDoc),
			NULL,
			InfoDoctor);
		ShowWindow(ventana, SW_SHOW);
		EndDialog(hwnd, 0);

	}break;
	case ID_manejo: {

		HWND ventana = CreateDialog(hInst,
			MAKEINTRESOURCE(IDC_RegistroCita),
			NULL,
			RegistroCita);
		ShowWindow(ventana, SW_SHOW);
		EndDialog(hwnd, 0);

	}break;

	case ID_agenda: {
		HWND ventana = CreateDialog(hInst,
			MAKEINTRESOURCE(IDC_Agenda),
			NULL,
			Agenda);
		ShowWindow(ventana, SW_SHOW);
		EndDialog(hwnd, 0);

	}break;

	case ID_salir: {

		int res = MessageBox(hwnd, L" Realmente deseas salir?", L"ALERTA", MB_OKCANCEL | MB_ICONWARNING);

		if (res == IDOK)
		{
			Guardarinfo();
			EndDialog(hwnd, 0);

		}break;

	}break;

	default: {
		return false;
	}break;
	}
	return true;
}

//FUNCION PRINCIPAL

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
	//Declaracion
	HWND hwnd;
	MSG mensaje;


	hwnd = CreateDialog(hInstance, MAKEINTRESOURCE(IDC_INICIARSESION), NULL, IniciarSesion);

	ShowWindow(hwnd, SW_SHOWDEFAULT);

	LeerInfo();
	GetLocalTime(&fechact);

	/* Bucle de mensajes: */
	while (TRUE == GetMessage(&mensaje, 0, 0, 0))
	{
		TranslateMessage(&mensaje);
		DispatchMessage(&mensaje);
	}

	return mensaje.wParam;
}

//CALLBACKS

LRESULT CALLBACK InfoDoctor(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg) {

	case WM_INITDIALOG: {

		SendMessage(GetDlgItem(hwnd, IDC_LIST1), LB_ADDSTRING, 0, (LPARAM)veterinarioActual->nombre);
		SendMessage(GetDlgItem(hwnd, IDC_LIST2), LB_ADDSTRING, 0, (LPARAM)veterinarioActual->cedula);
		SendMessage(GetDlgItem(hwnd, IDC_LIST3), LB_ADDSTRING, 0, (LPARAM)veterinarioActual->usuario);
		SendMessage(GetDlgItem(hwnd, IDC_LIST4), LB_ADDSTRING, 0, (LPARAM)veterinarioActual->contra);
		SendMessage(GetDlgItem(hwnd, IDC_EDIT1), LB_ADDSTRING, 0, (LPARAM)veterinarioActual->imagen);
		SetDlgItemText(hwnd, IDC_EDIT1, ruta);
		HBITMAP imagen = (HBITMAP)LoadImage(hInst, ruta, IMAGE_BITMAP, 150, 150, LR_LOADFROMFILE | LR_DEFAULTCOLOR | LR_DEFAULTSIZE);
		if (imagen != NULL)
			SendMessage(GetDlgItem(hwnd, IDC_IMAGEN2), STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)imagen);


	}break;

	case WM_COMMAND: {
		int id = LOWORD(wParam);
		if (menu(id, hwnd))
			return FALSE;

		switch (id) {

		case IDC_BUTTON1: {

			HWND ventana = CreateDialog(hInst, MAKEINTRESOURCE(IDC_Agenda), NULL, Agenda);
			ShowWindow(ventana, SW_SHOW);
			EndDialog(hwnd, 0);

		}break;

		case IDC_BUTTON2: {

			HWND ventana = (CreateDialog(hInst, MAKEINTRESOURCE(IDC_ModificarDoc), NULL, ModificarDoctor));
			ShowWindow(ventana, SW_SHOW);
			EndDialog(hwnd, 0);

		}break;

		}break;

	}break;


	case WM_CLOSE: {

		int res = MessageBox(hwnd, L" Realmente deseas salir?", L"ALERTA", MB_OKCANCEL | MB_ICONWARNING);

		if (res == IDOK)
		{
			Guardarinfo();
			EndDialog(hwnd, 0);
		}

	}break;

	case WM_DESTROY: {
		/* env a un mensaje WM_QUIT a la cola de mensajes */
		PostQuitMessage(0);
	}break;


	}

	return 0;

}

LRESULT CALLBACK IniciarSesion(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg) {

	case WM_COMMAND: {
		int wmId = LOWORD(wParam);

		switch (wmId) {
		case IDC_REGISTRARSE: {

			HWND ventana =
				CreateDialog(hInst,
					MAKEINTRESOURCE(IDC_Registro),
					NULL,
					Registrarse);
			ShowWindow(ventana, SW_SHOW);
			EndDialog(hwnd, 0);

		}break;

		case ID_IniciarSesion: {
			WCHAR con[50];
			WCHAR nom[50];
			GetDlgItemText(hwnd, IDC_EDIT2, nom, 50);
			GetDlgItemText(hwnd, IDC_EDIT1, con, 50);
			Veterinario* busquedanom = buscarNombre((WCHAR*)nom);
			WCHAR* busquedacon = NULL;

			if (busquedanom != NULL) {
				veterinarioActual = busquedanom;
				busquedacon = buscarContra((WCHAR*)con);
			}

			if (busquedanom == NULL || busquedacon == NULL) {

				MessageBox(hwnd, L"El usuario o contrase a son incorrectos intente nuevamente", L"Informacion incorrecta", MB_OK | MB_ICONINFORMATION);

			}
			else {

				MessageBox(hwnd, L"Iniciando sesion", L"Inicio de Sesion Exitoso", MB_OK | MB_ICONINFORMATION);

				HWND ventana = CreateDialog(hInst,
					MAKEINTRESOURCE(IDC_Agenda),
					NULL, Agenda);
				ShowWindow(ventana, SW_SHOW);
				EndDialog(hwnd, 0);
			}

		}break;

		case 1017: {

			int res = MessageBox(hwnd, L" Realmente deseas salir?", L"ALERTA", MB_OKCANCEL | MB_ICONWARNING);

			if (res == IDOK)
			{
				Guardarinfo();
				EndDialog(hwnd, 0);
			}

		}break;

		}
	}break;



	case WM_CLOSE: {

		int res = MessageBox(hwnd, L" Realmente deseas salir?", L"ALERTA", MB_OKCANCEL | MB_ICONWARNING);

		if (res == IDOK)
		{
			Guardarinfo();
			EndDialog(hwnd, 0);
		}

	}break;

	case WM_DESTROY: {
		/* env a un mensaje WM_QUIT a la cola de mensajes */
		PostQuitMessage(0);
	}break;

	}

	return FALSE;

}

LRESULT CALLBACK Agenda(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {

	switch (msg) {

	case WM_INITDIALOG: {


		SendMessage(GetDlgItem(hwnd, IDC_LIST5), LB_ADDSTRING, 0, (LPARAM)veterinarioActual->nombre);

		SetDlgItemText(hwnd, IDC_EDIT13, ruta);
		HBITMAP imagen = (HBITMAP)LoadImage(hInst, ruta, IMAGE_BITMAP, 150, 150, LR_LOADFROMFILE | LR_DEFAULTCOLOR | LR_DEFAULTSIZE);

		if (imagen != NULL)
			SendMessage(GetDlgItem(hwnd, IDC_IMAGEN2), STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)imagen);

		indiceC = veterinarioActual->origen;

		std::vector<std::wstring> fechas;

		while (indiceC != NULL) {

			if (indiceC->fecha.wYear == fechact.wYear) {

				if (indiceC->fecha.wMonth >= fechact.wMonth && indiceC->fecha.wDay >= fechact.wDay) {

					fechas.push_back(indiceC->fecha2);

				}

				else {

					if (indiceC->status != L"Cancelado") {

						wcscpy_s(indiceC->status, L"Cancelado");
					}

				}
			}
			else {

				if (indiceC->fecha.wYear > fechact.wYear) {

					fechas.push_back(indiceC->fecha2);

				}
				else {

					wcscpy_s(indiceC->status, L"Cancelado");
				}

			}

			indiceC = indiceC->siguiente;
		}

		std::sort(fechas.begin(), fechas.end(), [](const auto& a, const auto& b) {
	//comparar fechas
			return std::wstring(b.begin() + 6, b.begin() + 10) + std::wstring(b.begin() + 3, b.begin() + 5) + std::wstring(b.begin(), b.begin() + 2) >
				std::wstring(a.begin() + 6, a.begin() + 10) + std::wstring(a.begin() + 3, a.begin() + 5) + std::wstring(a.begin(), a.begin() + 2);
			});

		for (const auto& fecha : fechas) {

			SendMessage(GetDlgItem(hwnd, IDC_LIST1), LB_ADDSTRING, 0, (LPARAM)fecha.c_str());

		}

	}break;

	case WM_COMMAND: {

		int id = LOWORD(wParam);

		if (menu(id, hwnd))
			return FALSE;

		if (LOWORD(wParam) == IDC_BUTTON1) {

			HWND ventana = (CreateDialog(hInst, MAKEINTRESOURCE(IDC_Modificar), NULL, Modificar));
			ShowWindow(ventana, SW_SHOW);
			EndDialog(hwnd, 0);

		}

		if (LOWORD(wParam) == IDC_LIST1) {

			switch (HIWORD(wParam)) {

			case LBN_DBLCLK: {

				char texto[50] = { 0 };
				int seleccion = 0;

				seleccion = SendDlgItemMessage(hwnd, IDC_LIST1, LB_GETCURSEL, 0, 0);
				SendDlgItemMessage(hwnd, IDC_LIST1, LB_GETTEXT, seleccion, (LPARAM)texto);

				citaActual = veterinarioActual->origen;

				while (strcmp(texto, (char*)citaActual->fecha2) != 0 && citaActual->siguiente != nullptr) {

					citaActual = citaActual->siguiente;

				}
				
					SetDlgItemText(hwnd, IDC_EDIT5, citaActual->cliente);
					SetDlgItemText(hwnd, IDC_EDIT4, citaActual->tel);
					SetDlgItemText(hwnd, IDC_EDIT3, citaActual->mascota);
					SetDlgItemText(hwnd, IDC_EDIT7, citaActual->motivo);
					SetDlgItemText(hwnd, IDC_EDIT6, citaActual->costo);
					SetDlgItemText(hwnd, IDC_EDIT8, citaActual->status);
					SetDlgItemText(hwnd, IDC_EDIT10, citaActual->fecha2);
					SetDlgItemText(hwnd, IDC_EDIT12, citaActual->hora2);
					SetDlgItemText(hwnd, IDC_EDIT9, citaActual->especie);

			}break;
			}

		}

		if (LOWORD(wParam) == IDC_BUTTON2) {

			WCHAR* cliente = citaActual->cliente;
			eliminarCita(cliente);

			SendMessage(GetDlgItem(hwnd, IDC_LIST1), LB_RESETCONTENT, 0, 0);

			indiceC = veterinarioActual->origen;

			std::vector<std::wstring> fechas;

			while (indiceC != NULL) {

				if (indiceC->fecha.wYear == fechact.wYear) {

					if (indiceC->fecha.wMonth >= fechact.wMonth && indiceC->fecha.wDay >= fechact.wDay) {

						fechas.push_back(indiceC->fecha2);

					}

					else {

						if (indiceC->status != L"Cancelado") {

							wcscpy_s(indiceC->status, L"Cancelado");
						}

					}
				}
				else {

					if (indiceC->fecha.wYear > fechact.wYear) {

						fechas.push_back(indiceC->fecha2);

					}
					else {

						wcscpy_s(indiceC->status, L"Cancelado");
					}

				}

				indiceC = indiceC->siguiente;
			}

			std::sort(fechas.begin(), fechas.end(), [](const auto& a, const auto& b) {
				//comparar fechas
				return std::wstring(b.begin() + 6, b.begin() + 10) + std::wstring(b.begin() + 3, b.begin() + 5) + std::wstring(b.begin(), b.begin() + 2) >
					std::wstring(a.begin() + 6, a.begin() + 10) + std::wstring(a.begin() + 3, a.begin() + 5) + std::wstring(a.begin(), a.begin() + 2);
				});

			for (const auto& fecha : fechas) {

				SendMessage(GetDlgItem(hwnd, IDC_LIST1), LB_ADDSTRING, 0, (LPARAM)fecha.c_str());

			}

			SetDlgItemText(hwnd, IDC_EDIT10, (WCHAR*)" ");
			SetDlgItemText(hwnd, IDC_EDIT12, (WCHAR*)" ");
			SetDlgItemText(hwnd, IDC_EDIT3, (WCHAR*)" ");
			SetDlgItemText(hwnd, IDC_EDIT4, (WCHAR*)" ");
			SetDlgItemText(hwnd, IDC_EDIT5, (WCHAR*)" ");
			SetDlgItemText(hwnd, IDC_EDIT9, (WCHAR*)" ");
			SetDlgItemText(hwnd, IDC_EDIT7, (WCHAR*)" ");
			SetDlgItemText(hwnd, IDC_EDIT6, (WCHAR*)" ");
			SetDlgItemText(hwnd, IDC_EDIT8, (WCHAR*)" ");



		}

		if (LOWORD(wParam) == IDC_BUTTON4) {

			HWND ventana = (CreateDialog(hInst, MAKEINTRESOURCE(IDC_ConsultarCita), NULL, ConsultarCita));
			ShowWindow(ventana, SW_SHOW);
			EndDialog(hwnd, 0);

		}

	}break;

	case WM_CLOSE: {

		int res = MessageBox(hwnd, L" Realmente deseas salir?", L"ALERTA", MB_OKCANCEL | MB_ICONWARNING);

		if (res == IDOK)
		{
			Guardarinfo();
			EndDialog(hwnd, 0);

		}break;

	}break;

	case WM_DESTROY: {
		/* env a un mensaje WM_QUIT a la cola de mensajes */
		PostQuitMessage(0);
	}break;

	}

	return FALSE;

}

LRESULT CALLBACK Modificar(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {

	switch (msg) {

	case WM_INITDIALOG: {

		SendMessage(GetDlgItem(hwnd, IDC_LIST5), LB_ADDSTRING, 0, (LPARAM)veterinarioActual->nombre);

		SetDlgItemText(hwnd, IDC_EDIT16, ruta);
		HBITMAP imagen = (HBITMAP)LoadImage(hInst, ruta, IMAGE_BITMAP, 150, 150, LR_LOADFROMFILE | LR_DEFAULTCOLOR | LR_DEFAULTSIZE);

		if (imagen != NULL)
			SendMessage(GetDlgItem(hwnd, IDC_IMAGEN2), STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)imagen);

		SetDlgItemText(hwnd, IDC_EDIT5, citaActual->cliente);
		SetDlgItemText(hwnd, IDC_EDIT4, citaActual->tel);
		SetDlgItemText(hwnd, IDC_EDIT3, citaActual->mascota);
		SetDlgItemText(hwnd, IDC_EDIT7, citaActual->motivo);
		SetDlgItemText(hwnd, IDC_EDIT6, citaActual->costo);
		SetDlgItemText(hwnd, IDC_EDIT8, citaActual->status);
		DateTime_SetSystemtime(GetDlgItem(hwnd, IDC_DATETIMEPICKER1), 0, &citaActual->fecha);
		DateTime_SetSystemtime(GetDlgItem(hwnd, IDC_DATETIMEPICKER2), 0, &citaActual->hora);
		SetDlgItemText(hwnd, IDC_EDIT9, citaActual->especie);

		if (esp != nullptr) {

			for (int i = 0; i < n; i++) {

				SendMessage(GetDlgItem(hwnd, IDC_COMBO1), CB_INSERTSTRING, 0, (LPARAM)esp[i]);

			}break;

		}break;

	}

	case WM_COMMAND: {
		int id = LOWORD(wParam);
		if (menu(id, hwnd))
			return FALSE;

		switch (id) {

		case IDC_BUTTON1: {

			WCHAR cliente[50];
			WCHAR mascota[50];
			WCHAR especie[50];
			WCHAR telefono[50];
			WCHAR fecha2[50];
			WCHAR hora2[50];
			WCHAR motivo[100];
			SYSTEMTIME fecha = { 0 };
			SYSTEMTIME hora = { 0 };
			WCHAR estado[50];
			WCHAR precio[50];

			DateTime_GetSystemtime(GetDlgItem(hwnd, IDC_DATETIMEPICKER1), &fecha);
			DateTime_GetSystemtime(GetDlgItem(hwnd, IDC_DATETIMEPICKER2), &hora);

			int v1 = GetWindowTextLength(GetDlgItem(hwnd, IDC_COMBO1));
			int v2 = GetWindowTextLength(GetDlgItem(hwnd, IDC_EDIT5));
			int v3 = GetWindowTextLength(GetDlgItem(hwnd, IDC_EDIT7));
			int v4 = GetWindowTextLength(GetDlgItem(hwnd, IDC_EDIT4));
			int v5 = GetWindowTextLength(GetDlgItem(hwnd, IDC_EDIT3));
			int v6 = GetWindowTextLength(GetDlgItem(hwnd, IDC_EDIT8));
			int v7 = GetWindowTextLength(GetDlgItem(hwnd, IDC_EDIT6));

			if (fecha.wYear == fechact.wYear) {

				if (fecha.wMonth >= fechact.wMonth && fecha.wDay >= fechact.wDay) {

					GetDlgItemText(hwnd, IDC_DATETIMEPICKER1, fecha2, 50);
					GetDlgItemText(hwnd, IDC_DATETIMEPICKER2, hora2, 50);
					GetDlgItemText(hwnd, IDC_EDIT5, cliente, 50);
					GetDlgItemText(hwnd, IDC_EDIT7, motivo, 100);
					GetDlgItemText(hwnd, IDC_COMBO1, especie, 50);
					GetDlgItemText(hwnd, IDC_EDIT4, telefono, 50);
					GetDlgItemText(hwnd, IDC_EDIT3, mascota, 50);
					GetDlgItemText(hwnd, IDC_EDIT8, estado, 50);
					GetDlgItemText(hwnd, IDC_EDIT6, precio, 50);


					if (v1 == 0 || v2 == 0 || v3 == 0 || v4 == 0 || v5 == 0 || v6 == 0 || v7 == 0) {

						MessageBox(hwnd, L"Faltan datos por llenar", L"Advertencia", MB_OK | MB_ICONWARNING);
						break;
					}
					else {

						if (v4 == 8 || v4 == 10 || v4 == 12) {

							if (esp == nullptr) {

								esp = new WCHAR * [100];
								esp[0] = new WCHAR[50];
								wcscpy_s(esp[0], 50, especie);
								n = n + 1;

							}
							else {

								esp[n] = new WCHAR[50];
								wcscpy_s(esp[n], 50, especie);
								n = n + 1;

							}

							citaActual->fecha = fecha;
							citaActual->hora = hora;
							wcscpy_s(citaActual->fecha2, fecha2);
							wcscpy_s(citaActual->hora2, hora2);
							wcscpy_s(citaActual->cliente, cliente);
							wcscpy_s(citaActual->motivo, motivo);
							wcscpy_s(citaActual->tel, telefono);
							wcscpy_s(citaActual->especie, especie);
							wcscpy_s(citaActual->mascota, mascota);
							wcscpy_s(citaActual->status, estado);
							wcscpy_s(citaActual->costo, precio);

							MessageBox(hwnd, L"Modificacion guardada con exito", L"Modificacion Exitosa", MB_OK | MB_ICONINFORMATION);

							HWND ventana = (CreateDialog(hInst, MAKEINTRESOURCE(IDC_Agenda), NULL, Agenda));
							ShowWindow(ventana, SW_SHOW);
							EndDialog(hwnd, 0);

						}
						else {

							MessageBox(hwnd, L"El numero telefonico debe tener exactamente 8, 10 o 12 caracteres", L"Error", MB_OK | MB_ICONINFORMATION);

						}

					}

				}
				else {

					MessageBox(hwnd, L"No se puede elegir una fecha posterior al actual", L"Fecha invalida", MB_OK | MB_ICONINFORMATION);

				}

			}
			else {

				if (fecha.wYear > fechact.wYear) {

					GetDlgItemText(hwnd, IDC_DATETIMEPICKER1, fecha2, 50);
					GetDlgItemText(hwnd, IDC_DATETIMEPICKER2, hora2, 50);
					GetDlgItemText(hwnd, IDC_EDIT5, cliente, 50);
					GetDlgItemText(hwnd, IDC_EDIT7, motivo, 100);
					GetDlgItemText(hwnd, IDC_COMBO1, especie, 50);
					GetDlgItemText(hwnd, IDC_EDIT4, telefono, 50);
					GetDlgItemText(hwnd, IDC_EDIT3, mascota, 50);
					GetDlgItemText(hwnd, IDC_EDIT8, estado, 50);
					GetDlgItemText(hwnd, IDC_EDIT6, precio, 50);

					if (v1 == 0 || v2 == 0 || v3 == 0 || v4 == 0 || v5 == 0 || v6 == 0 || v7 == 0) {

						MessageBox(hwnd, L"Faltan datos por llenar", L"Advertencia", MB_OK | MB_ICONWARNING);
						break;
					}
					else {

						if (v4 == 8 || v4 == 10 || v4 == 12) {

							if (esp == nullptr) {

								esp = new WCHAR * [100];
								esp[0] = new WCHAR[50];
								wcscpy_s(esp[0], 50, especie);
								n = n + 1;

							}
							else {

								esp[n] = new WCHAR[50];
								wcscpy_s(esp[n], 50, especie);
								n = n + 1;

							}

							citaActual->fecha = fecha;
							citaActual->hora = hora;
							wcscpy_s(citaActual->fecha2, fecha2);
							wcscpy_s(citaActual->hora2, hora2);
							wcscpy_s(citaActual->cliente, cliente);
							wcscpy_s(citaActual->motivo, motivo);
							wcscpy_s(citaActual->tel, telefono);
							wcscpy_s(citaActual->especie, especie);
							wcscpy_s(citaActual->mascota, mascota);
							wcscpy_s(citaActual->status, estado);
							wcscpy_s(citaActual->costo, precio);

							MessageBox(hwnd, L"Modificacion guardada con exito", L"Modificacion Exitosa", MB_OK | MB_ICONINFORMATION);

							HWND ventana = (CreateDialog(hInst, MAKEINTRESOURCE(IDC_Agenda), NULL, Agenda));
							ShowWindow(ventana, SW_SHOW);
							EndDialog(hwnd, 0);

						}
						else {

							MessageBox(hwnd, L"El numero telefonico debe tener exactamente 8, 10 o 12 caracteres", L"Error", MB_OK | MB_ICONINFORMATION);

						}

					}

				}
				else {

					MessageBox(hwnd, L"No se puede elegir una fecha posterior al actual", L"Fecha invalida", MB_OK | MB_ICONINFORMATION);

				}

			}

		}break;

		case IDC_EDIT5: {

			if (HIWORD(wParam) == EN_UPDATE) {

				WCHAR buffer[50];
				GetWindowText(GetDlgItem(hwnd, IDC_NOMBRE), buffer, 50);

				for (int i = 0; buffer[i] != L'\0'; ++i) {
					if (!isalpha(buffer[i]) && buffer[i] != L' ') {

						buffer[i] = L'\0';
						SetWindowText(GetDlgItem(hwnd, IDC_NOMBRE), buffer);
						break;

					}
				}
			}

		} break;

		case IDC_EDIT6: {

			if (HIWORD(wParam) == EN_UPDATE) {
				//Eliminar letras
				WCHAR buffer[50];
				GetWindowText(GetDlgItem(hwnd, IDC_EDIT6), buffer, 50);

				bool puntoEncontrado = false;

				for (int i = 0; buffer[i] != '\0'; ++i) {
					if (!isdigit(buffer[i])) {

						if (buffer[i] == L'.') {

							if (!puntoEncontrado && i > 0) {
								puntoEncontrado = true;
							}
							else {
								// Eliminar el punto adicional
								buffer[i] = L'\0';
								SetWindowText(GetDlgItem(hwnd, IDC_EDIT6), buffer);
								break;
							}
						}
						else {
							// Eliminar otros caracteres no numéricos
							buffer[i] = L'\0';
							SetWindowText(GetDlgItem(hwnd, IDC_EDIT6), buffer);
							break;
						}
					}
				}
			}
		} break;

		case IDC_COMBO1: {
			
			if (HIWORD(wParam) == EN_UPDATE) {

				WCHAR buffer[50];
				GetWindowText(GetDlgItem(hwnd, IDC_NOMBRE), buffer, 50);

				for (int i = 0; buffer[i] != L'\0'; ++i) {
					if (!isalpha(buffer[i]) && buffer[i] != L' ') {

						buffer[i] = L'\0';
						SetWindowText(GetDlgItem(hwnd, IDC_NOMBRE), buffer);
						break;

					}
				}
			}

		} break;

		case IDC_EDIT3: {
			
			if (HIWORD(wParam) == EN_UPDATE) {

				WCHAR buffer[50];
				GetWindowText(GetDlgItem(hwnd, IDC_NOMBRE), buffer, 50);

				for (int i = 0; buffer[i] != L'\0'; ++i) {
					if (!isalpha(buffer[i]) && buffer[i] != L' ') {

						buffer[i] = L'\0';
						SetWindowText(GetDlgItem(hwnd, IDC_NOMBRE), buffer);
						break;

					}
				}
			}

		} break;

		case IDC_BUTTON2: {

			HWND ventana = CreateDialog(hInst, MAKEINTRESOURCE(IDC_Agenda), NULL, Agenda);
			ShowWindow(ventana, SW_SHOW);
			EndDialog(hwnd, 0);

		}break;

		}

	}break;
	case WM_CLOSE: {
		int res = MessageBox(hwnd, L" Realmente deseas salir?", L"ALERTA", MB_OKCANCEL | MB_ICONWARNING);

		if (res == IDOK)
		{
			Guardarinfo();
			EndDialog(hwnd, 0);
		}break;

	}break;

	case WM_DESTROY: {
		/* env a un mensaje WM_QUIT a la cola de mensajes */
		PostQuitMessage(0);
	}break;

	}

	return FALSE;

}

LRESULT CALLBACK RegistroCita(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {

	switch (msg) {

	case WM_INITDIALOG: {

		SendMessage(GetDlgItem(hwnd, IDC_LIST5), LB_ADDSTRING, 0, (LPARAM)veterinarioActual->nombre);

		SetDlgItemText(hwnd, IDC_EDIT16, ruta);
		HBITMAP imagen = (HBITMAP)LoadImage(hInst, ruta, IMAGE_BITMAP, 150, 150, LR_LOADFROMFILE | LR_DEFAULTCOLOR | LR_DEFAULTSIZE);

		if (imagen != NULL)
			SendMessage(GetDlgItem(hwnd, IDC_IMAGEN2), STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)imagen);
		

		if (esp != nullptr) {

			for (int i = 0; i < n; i++) {

				SendMessage(GetDlgItem(hwnd, IDC_COMBO1), CB_INSERTSTRING, 0, (LPARAM)esp[i]);

			}

		}

	}break;

	case WM_COMMAND: {

		int id = LOWORD(wParam);
		if (menu(id, hwnd))
			return FALSE;

		switch (id) {

		case IDC_EDIT6: {

			if (HIWORD(wParam) == EN_UPDATE) {
				//Eliminar letras
				WCHAR buffer[50];
				GetWindowText(GetDlgItem(hwnd, IDC_EDIT6), buffer, 50);

				bool puntoEncontrado = false;

				for (int i = 0; buffer[i] != '\0'; ++i) {
					if (!isdigit(buffer[i])) {

						if (buffer[i] == L'.') {

							if (!puntoEncontrado && i > 0) {
								puntoEncontrado = true;
							}
							else {
								// Eliminar el punto adicional
								buffer[i] = L'\0';
								SetWindowText(GetDlgItem(hwnd, IDC_EDIT6), buffer);
								break;
							}
						}
						else {
							// Eliminar otros caracteres no numéricos
							buffer[i] = L'\0';
							SetWindowText(GetDlgItem(hwnd, IDC_EDIT6), buffer);
							break;
						}
					}
				}
			}

		} break;

		case IDC_EDIT5: {

			if (HIWORD(wParam) == EN_UPDATE) {

				WCHAR buffer[50];
				GetWindowText(GetDlgItem(hwnd, IDC_NOMBRE), buffer, 50);

				for (int i = 0; buffer[i] != L'\0'; ++i) {
					if (!isalpha(buffer[i]) && buffer[i] != L' ') {

						buffer[i] = L'\0';
						SetWindowText(GetDlgItem(hwnd, IDC_NOMBRE), buffer);
						break;

					}
				}
			}

		} break;

		case IDC_COMBO1: {

			if (HIWORD(wParam) == EN_UPDATE) {

				WCHAR buffer[50];
				GetWindowText(GetDlgItem(hwnd, IDC_NOMBRE), buffer, 50);

				for (int i = 0; buffer[i] != L'\0'; ++i) {
					if (!isalpha(buffer[i]) && buffer[i] != L' ') {

						buffer[i] = L'\0';
						SetWindowText(GetDlgItem(hwnd, IDC_NOMBRE), buffer);
						break;

					}
				}
			}

		} break;

		case IDC_EDIT3: {

			if (HIWORD(wParam) == EN_UPDATE) {

				WCHAR buffer[50];
				GetWindowText(GetDlgItem(hwnd, IDC_NOMBRE), buffer, 50);

				for (int i = 0; buffer[i] != L'\0'; ++i) {
					if (!isalpha(buffer[i]) && buffer[i] != L' ') {

						buffer[i] = L'\0';
						SetWindowText(GetDlgItem(hwnd, IDC_NOMBRE), buffer);
						break;

					}
				}
			}

		} break;

		case ID_RegistrarCita: {

			WCHAR cliente[50];
			WCHAR mascota[50];
			WCHAR especie[50];
			WCHAR telefono[50];
			WCHAR motivo[100];
			SYSTEMTIME fecha = { 0 };
			WCHAR fecha2[50];
			WCHAR hora2[50];
			SYSTEMTIME hora = { 0 };
			WCHAR estado[50];
			WCHAR precio[50];


			DateTime_GetSystemtime(GetDlgItem(hwnd, IDC_DATETIMEPICKER1), &fecha);
			DateTime_GetSystemtime(GetDlgItem(hwnd, IDC_DATETIMEPICKER2), &hora);

			if (fecha.wYear == fechact.wYear) {

				if (fecha.wMonth >= fechact.wMonth && fecha.wDay >= fechact.wDay) {

					GetDlgItemText(hwnd, IDC_DATETIMEPICKER1, fecha2, 50);
					GetDlgItemText(hwnd, IDC_DATETIMEPICKER2, hora2, 50);
					GetDlgItemText(hwnd, IDC_EDIT5, cliente, 50);
					GetDlgItemText(hwnd, IDC_EDIT7, motivo, 100);
					GetDlgItemText(hwnd, IDC_COMBO1, especie, 50);
					GetDlgItemText(hwnd, IDC_EDIT4, telefono, 50);
					GetDlgItemText(hwnd, IDC_EDIT3, mascota, 50);
					GetDlgItemText(hwnd, IDC_EDIT8, estado, 50);
					GetDlgItemText(hwnd, IDC_EDIT6, precio, 50);

					int v1 = GetWindowTextLength(GetDlgItem(hwnd, IDC_COMBO1));
					int v2 = GetWindowTextLength(GetDlgItem(hwnd, IDC_EDIT5));
					int v3 = GetWindowTextLength(GetDlgItem(hwnd, IDC_EDIT7));
					int v4 = GetWindowTextLength(GetDlgItem(hwnd, IDC_EDIT4));
					int v5 = GetWindowTextLength(GetDlgItem(hwnd, IDC_EDIT3));
					int v6 = GetWindowTextLength(GetDlgItem(hwnd, IDC_EDIT8));
					int v7 = GetWindowTextLength(GetDlgItem(hwnd, IDC_EDIT6));

					if (v1 == 0 || v2 == 0 || v3 == 0 || v4 == 0 || v5 == 0 || v6 == 0 || v7 == 0) {

						MessageBox(hwnd, L"Faltan datos por llenar", L"Advertencia", MB_OK | MB_ICONWARNING);
						break;
					}
					else {

						if (v4 == 8 || v4 == 10 || v4 == 12) {

							if (esp == nullptr) {

								esp = new WCHAR * [100];
								esp[0] = new WCHAR[50];
								wcscpy_s(esp[0], 50, especie);
								n = n + 1;

							}
							else {

								esp[n] = new WCHAR[50];
								wcscpy_s(esp[n], 50, especie);
								n = n + 1;

							}

							Citas* dato = crearCita(cliente, mascota, especie, telefono, motivo, fecha, hora, fecha2, hora2, estado, precio);

							incluirCita(dato);

							MessageBox(hwnd, L"Informacion guardada con exito", L"Guardado Exitoso", MB_OK | MB_ICONINFORMATION);

							HWND ventana = (CreateDialog(hInst, MAKEINTRESOURCE(IDC_Agenda), NULL, Agenda));
							ShowWindow(ventana, SW_SHOW);
							EndDialog(hwnd, 0);

						}
						else {

							MessageBox(hwnd, L"El numero telefonico debe tener exactamente 8, 10 o 12 caracteres", L"Error", MB_OK | MB_ICONINFORMATION);

						}

					}

				}
				else {

					MessageBox(hwnd, L"No se puede elegir una fecha posterior al actual", L"Fecha invalida", MB_OK | MB_ICONINFORMATION);

				}
				
			}
			else {

				if (fecha.wYear > fechact.wYear) {

					GetDlgItemText(hwnd, IDC_DATETIMEPICKER1, fecha2, 50);
					GetDlgItemText(hwnd, IDC_DATETIMEPICKER2, hora2, 50);
					GetDlgItemText(hwnd, IDC_EDIT5, cliente, 50);
					GetDlgItemText(hwnd, IDC_EDIT7, motivo, 100);
					GetDlgItemText(hwnd, IDC_COMBO1, especie, 50);
					GetDlgItemText(hwnd, IDC_EDIT4, telefono, 50);
					GetDlgItemText(hwnd, IDC_EDIT3, mascota, 50);
					GetDlgItemText(hwnd, IDC_EDIT8, estado, 50);
					GetDlgItemText(hwnd, IDC_EDIT6, precio, 50);

					int v1 = GetWindowTextLength(GetDlgItem(hwnd, IDC_COMBO1));
					int v2 = GetWindowTextLength(GetDlgItem(hwnd, IDC_EDIT5));
					int v3 = GetWindowTextLength(GetDlgItem(hwnd, IDC_EDIT7));
					int v4 = GetWindowTextLength(GetDlgItem(hwnd, IDC_EDIT4));
					int v5 = GetWindowTextLength(GetDlgItem(hwnd, IDC_EDIT3));
					int v6 = GetWindowTextLength(GetDlgItem(hwnd, IDC_EDIT8));
					int v7 = GetWindowTextLength(GetDlgItem(hwnd, IDC_EDIT6));

					if (v1 == 0 || v2 == 0 || v3 == 0 || v4 == 0 || v5 == 0 || v6 == 0 || v7 == 0) {

						MessageBox(hwnd, L"Faltan datos por llenar", L"Advertencia", MB_OK | MB_ICONWARNING);
						break;
					}
					else {

						if (v4 == 8 || v4 == 10 || v4 == 12) {

							if (esp == nullptr) {

								esp = new WCHAR * [100];
								esp[0] = new WCHAR[50];
								wcscpy_s(esp[0], 50, especie);
								n = n + 1;

							}
							else {

								esp[n] = new WCHAR[50];
								wcscpy_s(esp[n], 50, especie);
								n = n + 1;

							}

							Citas* dato = crearCita(cliente, mascota, especie, telefono, motivo, fecha, hora, fecha2, hora2, estado, precio);

							incluirCita(dato);

							MessageBox(hwnd, L"Informacion guardada con exito", L"Guardado Exitoso", MB_OK | MB_ICONINFORMATION);


							HWND ventana = (CreateDialog(hInst, MAKEINTRESOURCE(IDC_Agenda), NULL, Agenda));
							ShowWindow(ventana, SW_SHOW);
							EndDialog(hwnd, 0);

						}
						else {

							MessageBox(hwnd, L"El numero telefonico debe tener exactamente 8, 10 o 12 caracteres", L"Error", MB_OK | MB_ICONINFORMATION);

						}

					}

				}
				else {

					MessageBox(hwnd, L"No se puede elegir una fecha posterior al actual", L"Fecha invalida", MB_OK | MB_ICONINFORMATION);

				}

			}
			
		}break;

		}break;

	}break;

	case WM_CLOSE: {

		int res = MessageBox(hwnd, L" Realmente deseas salir del programa?", L"ALERTA", MB_OKCANCEL | MB_ICONWARNING);

		if (res == IDOK)
		{
			Guardarinfo();
			EndDialog(hwnd, 0);

		}break;

	}break;

	case WM_DESTROY: {
		/* env a un mensaje WM_QUIT a la cola de mensajes */
		PostQuitMessage(0);
	}break;

	}

	return FALSE;


}

LRESULT CALLBACK Registrarse(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {

	switch (msg) {

	case WM_COMMAND: {

		int id = LOWORD(wParam);

		switch (id) {

		case IDC_NOMBRE: {

			if (HIWORD(wParam) == EN_UPDATE) {

				WCHAR buffer[50];
				GetWindowText(GetDlgItem(hwnd, IDC_NOMBRE), buffer, 50);

				for (int i = 0; buffer[i] != L'\0'; ++i) {
					if (!isalpha(buffer[i]) && buffer[i] != L' ') {

						buffer[i] = L'\0';
						SetWindowText(GetDlgItem(hwnd, IDC_NOMBRE), buffer);
						break;

					}
				}
			}

		}break;

		case IDC_GUARDAR: {

			WCHAR nom[50];
			WCHAR ced[100];
			WCHAR usu[50];
			WCHAR con[50];
			WCHAR ima[100];


			GetWindowText(GetDlgItem(hwnd, IDC_NOMBRE), nom, 50);
			GetWindowText(GetDlgItem(hwnd, IDC_CEDULA), ced, 100);
			GetWindowText(GetDlgItem(hwnd, IDC_EDIT1), ima, 100);
			GetWindowText(GetDlgItem(hwnd, IDC_CONTRASENA), con, 50);
			GetWindowText(GetDlgItem(hwnd, IDC_USUARIO), usu, 50);

			int v1 = GetWindowTextLength(GetDlgItem(hwnd, IDC_NOMBRE));
			int v2 = GetWindowTextLength(GetDlgItem(hwnd, IDC_CEDULA));
			int v3 = GetWindowTextLength(GetDlgItem(hwnd, IDC_EDIT1));
			int v4 = GetWindowTextLength(GetDlgItem(hwnd, IDC_CONTRASENA));
			int v5 = GetWindowTextLength(GetDlgItem(hwnd, IDC_USUARIO));

			if (v1 == 0 || v2 == 0 || v3 == 0 || v4 == 0 || v5 == 0) {

				MessageBox(hwnd, L"Faltan datos por llenar", L"Datos incompletos", MB_OK | MB_ICONINFORMATION);

			}
			else {

				agregarDoctorInicio(crearDoc(nom, usu, con, ced, ima));

				MessageBox(hwnd, L"Informacion guardada con exito", L"Guardado Exitoso", MB_OK | MB_ICONINFORMATION);

				HWND ventana =
					CreateDialog(hInst,
						MAKEINTRESOURCE(IDC_INICIARSESION),
						NULL,
						IniciarSesion);
				ShowWindow(ventana, SW_SHOW);
				EndDialog(hwnd, 0);

			}

		}break;

		case IDC_IMAGEN1: {


			OPENFILENAME ofn;
			ZeroMemory(&ofn, sizeof(ofn));
			ofn.lStructSize = sizeof(OPENFILENAME);
			ofn.hwndOwner = hwnd;
			ofn.lpstrFile = ruta;
			ofn.nMaxFile = 1000;
			ofn.lpstrFilter = L"Hola Bitmap\0*.bmp\0";
			ofn.nFilterIndex = 1;
			ofn.lpstrTitle = NULL;
			ofn.nMaxFileTitle = 0;
			ofn.lpstrInitialDir = NULL;
			ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

			if (GetOpenFileName(&ofn)) {

				SetDlgItemText(hwnd, IDC_EDIT1, ruta);
				HBITMAP imagen = (HBITMAP)LoadImage(hInst, ruta, IMAGE_BITMAP, 150, 150, LR_LOADFROMFILE | LR_DEFAULTCOLOR | LR_DEFAULTSIZE);


				if (imagen != NULL)

					SendMessage(GetDlgItem(hwnd, IDC_IMAGEN2), STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)imagen);

			}

		}break;

		case IDC_CANCELAR: {

			HWND ventana = (CreateDialog(hInst, MAKEINTRESOURCE(IDC_INICIARSESION), NULL, IniciarSesion));
			ShowWindow(ventana, SW_SHOW);
			EndDialog(hwnd, 0);

		}break;

		}break;

	}break;

	case WM_CLOSE: {

		HWND ventana = (CreateDialog(hInst, MAKEINTRESOURCE(IDC_INICIARSESION), NULL, IniciarSesion));
		ShowWindow(ventana, SW_SHOW);
		EndDialog(hwnd, 0);

	}break;

	case WM_DESTROY: {
		/* env a un mensaje WM_QUIT a la cola de mensajes */
		PostQuitMessage(0);
	}break;

	}

	return FALSE;

	}

LRESULT CALLBACK ConsultarCita(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {

	switch (msg) {

	case WM_INITDIALOG: {

		SendMessage(GetDlgItem(hwnd, IDC_LIST5), LB_ADDSTRING, 0, (LPARAM)veterinarioActual->nombre);

		SetDlgItemText(hwnd, IDC_EDIT13, ruta);
		HBITMAP imagen = (HBITMAP)LoadImage(hInst, ruta, IMAGE_BITMAP, 50, 50, LR_LOADFROMFILE | LR_DEFAULTCOLOR | LR_DEFAULTSIZE);

		if (imagen != NULL)
			SendMessage(GetDlgItem(hwnd, IDC_IMAGEN2), STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)imagen);

	}break;

	case WM_COMMAND: {

		int id = LOWORD(wParam);
		if (menu(id, hwnd))
			return FALSE;

		switch (id) {

		case ID_BUSCAR: {

			SYSTEMTIME fecha;
			SYSTEMTIME fecha2;
			bool a = { 0 };
			bool b = { 0 };
			bool c = { 0 };
			bool d = { 0 };

			SendMessage(GetDlgItem(hwnd, IDC_LIST1), LB_RESETCONTENT, 0, 0);


			DateTime_GetSystemtime(GetDlgItem(hwnd, IDC_DATETIMEPICKER2), &fecha);
			DateTime_GetSystemtime(GetDlgItem(hwnd, IDC_DATETIMEPICKER3), &fecha2);

			indiceC = veterinarioActual->origen;

			std::vector<std::wstring> fechas;


			while (indiceC != NULL) {

				c = indiceC->fecha.wYear == fecha.wYear && indiceC->fecha.wMonth == fecha.wMonth;
				d = indiceC->fecha.wYear == fecha2.wYear && indiceC->fecha.wMonth == fecha2.wMonth;

				if (c == TRUE || d == TRUE) {

					if (c == TRUE) {

						if (indiceC->fecha.wDay >= fecha.wDay) {

							fechas.push_back(indiceC->fecha2);

						}
					}
					else {

						if (indiceC->fecha.wDay <= fecha2.wDay) {

							fechas.push_back(indiceC->fecha2);

						}

					}



				}
				else {

					a = indiceC->fecha.wYear == fecha.wYear && indiceC->fecha.wMonth > fecha.wMonth;
					b = indiceC->fecha.wYear == fecha2.wYear && indiceC->fecha.wMonth < fecha.wMonth;

					if (a == TRUE || b == TRUE) {

						fechas.push_back(indiceC->fecha2);

					}
					else {

						if (indiceC->fecha.wYear > fecha.wYear && indiceC->fecha.wYear < fecha2.wYear) {

							fechas.push_back(indiceC->fecha2);

						}

					}

				}

				indiceC = indiceC->siguiente;

			}

			std::sort(fechas.begin(), fechas.end(), [](const auto& a, const auto& b) {
				//comparar fechas
				return std::wstring(b.begin() + 6, b.begin() + 10) + std::wstring(b.begin() + 3, b.begin() + 5) + std::wstring(b.begin(), b.begin() + 2) >
					std::wstring(a.begin() + 6, a.begin() + 10) + std::wstring(a.begin() + 3, a.begin() + 5) + std::wstring(a.begin(), a.begin() + 2);
				});

			for (const auto& fecha : fechas) {

				SendMessage(GetDlgItem(hwnd, IDC_LIST1), LB_ADDSTRING, 0, (LPARAM)fecha.c_str());

			}


		}break;

		case IDC_LIST1: {

			switch (HIWORD(wParam)) {

			case LBN_DBLCLK: {

				char texto[50]{};
				int seleccion = 0;

				seleccion = SendDlgItemMessage(hwnd, IDC_LIST1, LB_GETCURSEL, 0, 0);
				SendDlgItemMessage(hwnd, IDC_LIST1, LB_GETTEXT, seleccion, (LPARAM)texto);

				citaActual = veterinarioActual->origen;

				while (strcmp(texto, (char*)citaActual->fecha2) != 0 && citaActual->siguiente != nullptr) {

					citaActual = citaActual->siguiente;

				}

				SetDlgItemText(hwnd, IDC_EDIT5, citaActual->cliente);
				SetDlgItemText(hwnd, IDC_EDIT4, citaActual->tel);
				SetDlgItemText(hwnd, IDC_EDIT3, citaActual->mascota);
				SetDlgItemText(hwnd, IDC_EDIT7, citaActual->motivo);
				SetDlgItemText(hwnd, IDC_EDIT6, citaActual->costo);
				SetDlgItemText(hwnd, IDC_EDIT8, citaActual->status);
				SetDlgItemText(hwnd, IDC_EDIT10, citaActual->fecha2);
				SetDlgItemText(hwnd, IDC_EDIT12, citaActual->hora2);
				SetDlgItemText(hwnd, IDC_EDIT9, citaActual->especie);




			}break;

			}break;


		}break;

		case IDCANCEL2: {

			HWND ventana = (CreateDialog(hInst, MAKEINTRESOURCE(IDC_Agenda), NULL, Agenda));
			ShowWindow(ventana, SW_SHOW);
			EndDialog(hwnd, 0);

		}break;

		}break;

	}break;

	case WM_CLOSE: {

		HWND ventana = (CreateDialog(hInst, MAKEINTRESOURCE(IDC_INICIARSESION), NULL, IniciarSesion));
		ShowWindow(ventana, SW_SHOW);
		EndDialog(hwnd, 0);

	}break;

	case WM_DESTROY: {
		/* env a un mensaje WM_QUIT a la cola de mensajes */
		PostQuitMessage(0);
	}break;

	}

	return FALSE;

}

LRESULT CALLBACK ModificarDoctor(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {

	case WM_INITDIALOG: {

		SetDlgItemText(hwnd, IDC_NOMBRE, veterinarioActual->nombre);
		SetDlgItemText(hwnd, IDC_USUARIO, veterinarioActual->usuario);
		SetDlgItemText(hwnd, IDC_CEDULA, veterinarioActual->cedula);
		SetDlgItemText(hwnd, IDC_CONTRASENA, veterinarioActual->contra);
		SetDlgItemText(hwnd, IDC_EDIT1, ruta);
		HBITMAP imagen = (HBITMAP)LoadImage(hInst, ruta, IMAGE_BITMAP, 150, 150, LR_LOADFROMFILE | LR_DEFAULTCOLOR | LR_DEFAULTSIZE);
		if (imagen != NULL)
			SendMessage(GetDlgItem(hwnd, IDC_IMAGEN2), STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)imagen);


	}break;

	case WM_COMMAND: {

		int id = LOWORD(wParam);
		if (menu(id, hwnd))
			return FALSE;

		switch (id) {
		case IDC_NOMBRE: {

			if (HIWORD(wParam) == EN_UPDATE) {

				WCHAR buffer[50];
				GetWindowText(GetDlgItem(hwnd, IDC_NOMBRE), buffer, 50);

				for (int i = 0; buffer[i] != L'\0'; ++i) {
					if (!isalpha(buffer[i]) && buffer[i] != L' ') {

						buffer[i] = L'\0';
						SetWindowText(GetDlgItem(hwnd, IDC_NOMBRE), buffer);
						break;

					}
				}
			}

		}break;

		case IDC_IMAGEN1: {


			OPENFILENAME ofn;
			ZeroMemory(&ofn, sizeof(ofn));
			ofn.lStructSize = sizeof(OPENFILENAME);
			ofn.hwndOwner = hwnd;
			ofn.lpstrFile = ruta;
			ofn.nMaxFile = 1000;
			ofn.lpstrFilter = L"Hola Bitmap\0*.bmp\0";
			ofn.nFilterIndex = 1;
			ofn.lpstrTitle = NULL;
			ofn.nMaxFileTitle = 0;
			ofn.lpstrInitialDir = NULL;
			ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

			if (GetOpenFileName(&ofn)) {

				SetDlgItemText(hwnd, IDC_EDIT1, ruta);
				HBITMAP imagen = (HBITMAP)LoadImage(hInst, ruta, IMAGE_BITMAP, 150, 150, LR_LOADFROMFILE | LR_DEFAULTCOLOR | LR_DEFAULTSIZE);


				if (imagen != NULL)

					SendMessage(GetDlgItem(hwnd, IDC_IMAGEN2), STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)imagen);

			}

		}break;

		case IDC_BUTTON2: {
			WCHAR nom[50];
			WCHAR ced[100];
			WCHAR usu[50];
			WCHAR con[50];
			WCHAR ima[100];

			int v1 = GetWindowTextLength(GetDlgItem(hwnd, IDC_NOMBRE));
			int v2 = GetWindowTextLength(GetDlgItem(hwnd, IDC_CEDULA));
			int v3 = GetWindowTextLength(GetDlgItem(hwnd, IDC_EDIT1));
			int v4 = GetWindowTextLength(GetDlgItem(hwnd, IDC_CONTRASENA));
			int v5 = GetWindowTextLength(GetDlgItem(hwnd, IDC_USUARIO));

			if (v1 == 0 || v2 == 0 || v3 == 0 || v4 == 0 || v5 == 0) {

				MessageBox(hwnd, L"Faltan datos por llenar", L"Datos incompletos", MB_OK | MB_ICONINFORMATION);

			}
			else {

				GetWindowText(GetDlgItem(hwnd, IDC_NOMBRE), nom, 50);
				wcscpy_s(veterinarioActual->nombre, nom);

				GetWindowText(GetDlgItem(hwnd, IDC_CEDULA), ced, 100);
				wcscpy_s(veterinarioActual->cedula, ced);

				GetWindowText(GetDlgItem(hwnd, IDC_EDIT1), ima, 100);
				wcscpy_s(veterinarioActual->imagen, ima);

				GetWindowText(GetDlgItem(hwnd, IDC_CONTRASENA), con, 50);
				wcscpy_s(veterinarioActual->contra, con);

				GetWindowText(GetDlgItem(hwnd, IDC_USUARIO), usu, 50);
				wcscpy_s(veterinarioActual->usuario, usu);

				HWND ventana = CreateDialog(hInst, MAKEINTRESOURCE(IDC_InfoDoc), NULL, InfoDoctor);
				ShowWindow(ventana, SW_SHOW);
				EndDialog(hwnd, 0);

			}

		}break;

		case IDC_BUTTON1: {

			HWND ventana = CreateDialog(hInst, MAKEINTRESOURCE(IDC_InfoDoc), NULL, InfoDoctor);
			ShowWindow(ventana, SW_SHOW);
			EndDialog(hwnd, 0);

		}break;


		}break;

	}break;

	case WM_CLOSE: {

		int res = MessageBox(hwnd, L" Realmente deseas salir de la ventana de modificacion?", L"ALERTA", MB_OKCANCEL | MB_ICONWARNING);

		if (res == IDOK) {

			HWND ventana = (CreateDialog(hInst, MAKEINTRESOURCE(IDC_InfoDoc), NULL, InfoDoctor));
			ShowWindow(ventana, SW_SHOW);
			EndDialog(hwnd, 0);
		}

	}break;

	case WM_DESTROY: {
		/* env a un mensaje WM_QUIT a la cola de mensajes */
		PostQuitMessage(0);
	}break;

	}
	return FALSE;
}

//FUNCIONES

Citas* crearCita(WCHAR* cliente, WCHAR* mascota, WCHAR* especie, WCHAR* telefono, WCHAR* motivo, SYSTEMTIME fecha, SYSTEMTIME hora, WCHAR* fecha2, WCHAR* hora2, WCHAR* estado, WCHAR* precio) {

	Citas* nuevaCita = new Citas;
	wcscpy_s(nuevaCita->cliente, 50, cliente);
	wcscpy_s(nuevaCita->mascota, 50, mascota);
	wcscpy_s(nuevaCita->especie, 50, especie);
	wcscpy_s(nuevaCita->motivo, 100, motivo);
	wcscpy_s(nuevaCita->tel, 50, telefono);
	wcscpy_s(nuevaCita->fecha2, 50, fecha2);
	wcscpy_s(nuevaCita->hora2, 50, hora2);
	nuevaCita->fecha = fecha;
	nuevaCita->hora = hora;
	wcscpy_s(nuevaCita->status, 50, estado);
	wcscpy_s(nuevaCita->costo, 50, precio);
	nuevaCita->anterior = NULL;
	nuevaCita->siguiente = NULL;

	return nuevaCita;

}

Veterinario* crearDoc(WCHAR* nombre, WCHAR* usuario, WCHAR* contra, WCHAR* cedula, WCHAR* imagen) {
	Veterinario* nuevo = new Veterinario;
	wcscpy_s(nuevo->nombre, 50, nombre);
	wcscpy_s(nuevo->usuario, 50, usuario);
	wcscpy_s(nuevo->contra, 50, contra);
	wcscpy_s(nuevo->cedula, 100, cedula);
	wcscpy_s(nuevo->imagen, 100, imagen);

	return nuevo;
}

WCHAR* buscarContra(WCHAR* buscar) {
	if (InicioV == NULL)
		return NULL;
	WCHAR* con = veterinarioActual->contra;

	if (wcscmp(con, buscar) == 0)
		return con;

	return NULL;
}

Veterinario* buscarNombre(WCHAR* buscar) {

	if (InicioV == NULL)
		return NULL;
	Veterinario* indice = InicioV;
	while (indice != NULL) {
		if (wcscmp(indice->usuario, buscar) == 0)
			return indice;
		indice = indice->siguiente;
	}
	return NULL;
}

Citas* buscarCita(WCHAR* cliente) {

	if (veterinarioActual->origen == NULL)
		return NULL;

	Citas* indice = veterinarioActual->origen;

	while (indice != NULL) {
		if (wcscmp(indice->cliente, cliente) == 0)
			return indice;
		indice = indice->siguiente;
	}

	return NULL;

}

void agregarDoctorInicio(Veterinario* dato) {

	if (InicioV == nullptr) {

		InicioV = dato;
		FinV = InicioV;
		InicioV->siguiente = NULL;
		InicioV->anterior = NULL;
		dato->origen = NULL;
		dato->fin = NULL;
	}
	else {
		InicioV->anterior = dato;
		dato->siguiente = InicioV;
		InicioV = dato;
		InicioV->anterior = NULL;
		dato->origen = NULL;
		dato->fin = NULL;
	}
	Guardarinfo();
}

void incluirCita(Citas* dato) {

	if (veterinarioActual->origen == NULL) {

		veterinarioActual->origen = dato;
		veterinarioActual->fin = veterinarioActual->origen;
		veterinarioActual->origen->siguiente = NULL;
		veterinarioActual->origen->anterior = NULL;
	}
	else {

		veterinarioActual->origen->anterior = dato;
		dato->siguiente = veterinarioActual->origen;
		veterinarioActual->origen = dato;
		veterinarioActual->origen->anterior = NULL;

	}

}

void incluirCitaGen(Citas* dato) {


	if (InicioC == NULL) {

		InicioC = dato;
		veterinarioActual->fin = InicioC;
		InicioC->siguiente = NULL;
		InicioC->anterior = NULL;
	}
	else {

		InicioC->anterior = dato;
		dato->siguiente = InicioC;
		InicioC = dato;
		InicioC->anterior = NULL;

	}


}

Citas* eliminarCitaInicio() {

	if (veterinarioActual->origen == NULL)
		return NULL;

	if (veterinarioActual->origen == veterinarioActual->fin)
		veterinarioActual->fin = NULL;

	Citas* anterior = veterinarioActual->origen;
	veterinarioActual->origen = veterinarioActual->origen->siguiente;

	if (veterinarioActual->origen != NULL)
		veterinarioActual->origen->anterior = NULL;

	delete anterior;

	return anterior;

}

Citas* eliminarCitaFinal() {
	if (veterinarioActual->origen == NULL)
		return NULL;

	if (veterinarioActual->origen == veterinarioActual->fin)
		return eliminarCitaInicio();

	Citas* indice = veterinarioActual->fin->anterior;
	Citas* dato = veterinarioActual->fin;
	delete veterinarioActual->fin;

	veterinarioActual->fin = indice;

	if (indice != NULL)
		indice->siguiente = NULL;

	return dato;

}

Citas* eliminarCita(WCHAR* nombre) {

	Citas* busqueda = buscarCita(nombre);
	if (busqueda == NULL)
		return NULL;

	if (busqueda == veterinarioActual->origen)
		return eliminarCitaInicio();

	else if (busqueda == veterinarioActual->fin)
		return eliminarCitaFinal();

	Citas* indice = busqueda->anterior;
	Citas* dato = busqueda;

	if (indice != NULL)
		indice->siguiente = busqueda->siguiente;

	Citas* C = busqueda->siguiente;
	C->anterior = indice;
	delete busqueda;
	return dato;

}

void Guardarinfo() {

	Veterinario* indice = InicioV;
	fstream datos;

	datos.open("DatosVeterinaria.bin", ios::out | ios::binary | ios::trunc);

	if (indice != NULL)
	{
		Veterinario* indice = InicioV;

		if (datos.is_open())
		{
			while (indice != NULL) {

				datos.write((char*)indice, sizeof(Veterinario));
				indice = indice->siguiente;

			}

		}
		datos.close();

	}

}

void LeerInfo() {

	ifstream InfoVet;
	InfoVet.open("DatosVeterinaria.bin", ios::in | ios::binary);

	Veterinario* auxA = InicioV;

	if (InfoVet.is_open()) {

		Veterinario* leerVet = new Veterinario;

		while (!InfoVet.read((char*)leerVet, sizeof(Veterinario)).eof()) {

			while (auxA != nullptr && auxA->siguiente != nullptr)
			{
				auxA = auxA->siguiente;
			}
			if (auxA == nullptr) {
				InicioV = leerVet;
				InicioV->siguiente = nullptr;
				InicioV->anterior = nullptr;
				auxA = InicioV;
			}
			else {
				auxA->siguiente = leerVet;
				auxA->siguiente->anterior = auxA;
				auxA = auxA->siguiente;
				auxA->siguiente = nullptr;
			}

			leerVet = new Veterinario;
		}

		InfoVet.close();
	}
}

/*

void GuardarCitas() {

	Citas* indice = veterinarioActual->origen;
	fstream datos;
	datos.open("DatosCitas.bin", ios::out | ios::binary | ios::trunc);

	if (indice != NULL)
	{
		Citas* indice = veterinarioActual->origen;

		if (datos.is_open())
		{
			while (indice != NULL) {
				datos.write((char*)indice, sizeof(Citas));
				indice = indice->siguiente;
			}
		}
		datos.close();

	}

}

void LeerCitas() {

	ifstream InfoCit;
	InfoCit.open("DatosCitas.bin", ios::in | ios::binary);

	Citas* auxA = veterinarioActual->origen;

	if (InfoCit.is_open()) {

		Citas* leerCit = new Citas;

		while (!InfoCit.read((char*)leerCit, sizeof(Citas)).eof()) {

			while (auxA != nullptr && auxA->siguiente != nullptr)
			{
				auxA = auxA->siguiente;
			}

			if (auxA == nullptr) {
				veterinarioActual->origen = leerCit;
				veterinarioActual->origen->siguiente = nullptr;
				veterinarioActual->origen->anterior = nullptr;
				auxA = veterinarioActual->origen;
			}
			else {
				auxA->siguiente = leerCit;
				auxA->siguiente->anterior = auxA;
				auxA = auxA->siguiente;
				auxA->siguiente = nullptr;
			}

			leerCit = new Citas;
		}

		InfoCit.close();
	}
}
*/