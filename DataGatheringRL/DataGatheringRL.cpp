#include "pch.h"
#include "DataGatheringRL.h"
#include <iostream>

BAKKESMOD_PLUGIN(DataGatheringRL, "A data gatherer that can connect to a COM port and send data to be used by a micro controller", plugin_version, PERMISSION_ALL)

std::shared_ptr<CVarManagerWrapper> _globalCvarManager;

/// <summary>
/// OnLoad and Hook Events
/// </summary>

void DataGatheringRL::onLoad()
{
	_globalCvarManager = cvarManager;
	this->setPlayerName(gameWrapper->GetPlayerName().ToString());
	cvarManager->log(std::to_string(gameWrapper->IsInReplay()));
	this->HookEvent();
}

void DataGatheringRL::onUnload()
{
	disconnectFromSerialPort();
}

//sets the players name
void DataGatheringRL::setPlayerName(std::string name)
{
	ownerName = name;
}

// events caller

void DataGatheringRL::HookEvent() {

	/****
	* 
	* This is an example of how to get a hook event to get called to the arduino. 
	* That just gets the boost amount for the local car that I used for a real time boost meter with addressable LED's.
	* You can use a caller hook event if you want but you need to uncomment the template in the .h file. Use that at your own risk o.O.
	* Make sure to null check if you don't want the crashing.
	* 
	*****/
	
	//only gets the Local cars change in boost. For all cars boost, check CommentedForLater.txt in resource files.
	/*gameWrapper->HookEvent("Function TAGame.GFxData_LocalCar_TA.BoostPercentageChanged", [this](std::string eventName)
	{
		boostAmount = static_cast<int>(gameWrapper->GetLocalCar().GetBoostComponent().GetCurrentBoostAmount() * 100);
		if (boostAmount == lastBoostAmount) { return; };
		lastBoostAmount = boostAmount;
		char temp[20] = BOOST_STRING;

		cvarManager->log(arduinoString);
		if (gameWrapper->GetLocalCar().GetOwnerName() == ownerName)
		{
			sendToArduino();
		}
	});*/
	

}

//sends information to com port
void DataGatheringRL::sendToArduino()
{
	if (!this->connected) { return; }
	this->writeSerialPort(arduinoString, MAX_DATA_LENGTH);
}

///

/// <summary>
/// Serial port connection, disconnection, reading and writing
/// </summary>

void DataGatheringRL::connectToSerialPort()
{
	if (!this->connected) {
		SerialPort(comDevice);
	}
}

void DataGatheringRL::disconnectFromSerialPort()
{
	if (this->connected)
	{
		this->connected = false;
		CloseHandle(this->handler);
	}
}

void DataGatheringRL::SerialPort(const char* portName)
{

	this->connected = false;

	this->handler = CreateFileA(static_cast<LPCSTR>(portName),
		GENERIC_READ | GENERIC_WRITE,
		0,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);
	if (this->handler == INVALID_HANDLE_VALUE)
	{
		if (GetLastError() == ERROR_FILE_NOT_FOUND)
		{
			cvarManager->log("ERROR: Handle was not attached. Reason: %s not available\n");
		}
		else
		{
			cvarManager->log("ERROR!!!");
		}
	}
	else
	{
		DCB dcbSerialParameters = { 0 };

		if (!GetCommState(this->handler, &dcbSerialParameters))
		{
			cvarManager->log("failed to get current serial parameters");
		}
		else {
			dcbSerialParameters.BaudRate = CBR_9600;
			dcbSerialParameters.ByteSize = 8;
			dcbSerialParameters.StopBits = ONESTOPBIT;
			dcbSerialParameters.Parity = NOPARITY;
			dcbSerialParameters.fDtrControl = DTR_CONTROL_ENABLE;

			if (!SetCommState(handler, &dcbSerialParameters))
			{
				cvarManager->log("ALERT: could not set Serial port parameters\n");
			}
			else
			{
				this->connected = true;
				PurgeComm(this->handler, PURGE_RXCLEAR | PURGE_TXCLEAR);
				Sleep(ARDUINO_WAIT_TIME);
			}
		}
	}
}

int DataGatheringRL::readSerialPort(char* buffer, unsigned int buf_size)
{
    DWORD bytesRead;
    unsigned int toRead = 0;

    ClearCommError(this->handler, &this->errors, &this->status);

    if (this->status.cbInQue > 0) {
        if (this->status.cbInQue > buf_size) {
            toRead = buf_size;
        }
        else toRead = this->status.cbInQue;
    }

    if (ReadFile(this->handler, buffer, toRead, &bytesRead, NULL)) return bytesRead;

    return 0;
}

bool DataGatheringRL::writeSerialPort(char* buffer, unsigned int buf_size)
{
	DWORD bytesSend;

	if (!WriteFile(this->handler, (void*)buffer, buf_size, &bytesSend, 0))
	{
		ClearCommError(this->handler, &this->errors, &this->status);
		return false;
	}
	else return true;
}

bool DataGatheringRL::isConnected()
{
	return this->connected;
}

///

/// <summary>
/// ImGui API calls
/// </summary>

void DataGatheringRL::SetImGuiContext(uintptr_t ctx) {
	ImGui::SetCurrentContext(reinterpret_cast<ImGuiContext*>(ctx));
}

std::string DataGatheringRL::GetPluginName() {
	return "Data Gathering RL";
}

//place all settings for GUI in here
void DataGatheringRL::RenderSettings() {
	char temp[255];
	strcpy(temp, ownerName.c_str());
	ImGui::Text("Current name of player : %s", temp);

	ImGui::TextUnformatted("To connect, enter the port that your device is connected to and then click on the connect button");

	ImGui::InputText(" Input COM Device", comDevice, IM_ARRAYSIZE(comDevice));

	if (ImGui::Button("Connect to Serial Port")) {
		if (!this->connected) {
			connectToSerialPort();

			if (this->connected) cvarManager->log("Connection successfully established to PORT");
		}
		else {
			cvarManager->log("Connection already established -- disconnect if you wish to connect to another device");
		}
	}

	if (ImGui::Button("Disconnect from Serial Port")) {
		if (this->connected) {
			disconnectFromSerialPort();
			if (!this->connected) {
				cvarManager->log("Connection disconnected from PORT");
			}
		}
		else cvarManager->log("No device currently connected");
	}


}