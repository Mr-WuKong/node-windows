#include <nan.h>
#include <Windows.h>

typedef struct EnumHWndsArg
{
	std::vector<HWND> *vecHWnds;
	DWORD dwProcessId;
}EnumHWndsArg, *LPEnumHWndsArg;

void moveWindowImpl(HWND hwnd, int x, int y, int width, int height) {
	::MoveWindow(hwnd, x, y, width, height, true);

}


NAN_METHOD(moveWindow) {
	if (info.Length() != 5) {
		return;
	}

	HWND hwnd = (HWND)info[0]->Uint32Value();
	int x =  info[1]->Uint32Value();
	int y = info[2]->Uint32Value();
	int w = info[3]->Uint32Value();
	int h = info[4]->Uint32Value();
 
	moveWindowImpl(hwnd, x, y, w, h);
}

NAN_METHOD(sendMessageToWindow) {
	if (info.Length() != 5) {
		return;
	}

	HWND hwnd = (HWND)info[0]->Uint32Value();
	int msg_code = info[1]->Uint32Value();
	int msg_value = info[2]->Uint32Value();

	SendMessage(hwnd, WM_USER + msg_code, (WPARAM)msg_value, NULL);
}

BOOL CALLBACK lpEnumFunc(HWND hwnd, LPARAM lParam)
{
	EnumHWndsArg *pArg = (LPEnumHWndsArg)lParam;
	DWORD  processId;
	GetWindowThreadProcessId(hwnd, &processId);
	if (processId == pArg->dwProcessId)
	{
		pArg->vecHWnds->push_back(hwnd);
		//printf("%p\n", hwnd);
	}
	return TRUE;
}

void GetHWndsByProcessID(DWORD processID, std::vector<HWND> &vecHWnds)
{
	EnumHWndsArg wi;
	wi.dwProcessId = processID;
	wi.vecHWnds = &vecHWnds;
	EnumWindows(lpEnumFunc, (LPARAM)&wi);
}

NAN_METHOD(getMainHWndByPID) {
	int PID = info[0]->Uint32Value();
	std::vector<HWND> vecHWnds;
	GetHWndsByProcessID(PID, vecHWnds);
	HWND hnd = NULL;
	for (const HWND &h : vecHWnds)
	{
		HWND parent = GetParent(h);
		if (parent == NULL)
		{
			printf("%p ---> Main Wnd\n", h);
			hnd = h;
		}
		else
		{
			printf("%p ---> %p\n", h, parent);
		}
	}
	v8::Local<v8::Int32> hnd_int = Nan::New((int)hnd);
	info.GetReturnValue().Set(hnd_int);
}



NAN_MODULE_INIT(Initialize) {
	Nan::Set(target, Nan::New<v8::String>("moveWindow").ToLocalChecked(),
		Nan::GetFunction(Nan::New<v8::FunctionTemplate>(moveWindow)).ToLocalChecked());

	Nan::Set(target, Nan::New<v8::String>("getMainHWndByPID").ToLocalChecked(),
		Nan::GetFunction(Nan::New<v8::FunctionTemplate>(getMainHWndByPID)).ToLocalChecked());

	Nan::Set(target, Nan::New<v8::String>("sendMessageToWindow").ToLocalChecked(),
		Nan::GetFunction(Nan::New<v8::FunctionTemplate>(sendMessageToWindow)).ToLocalChecked());
}

NODE_MODULE(window, Initialize)

