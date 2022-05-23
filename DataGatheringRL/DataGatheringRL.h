#pragma once

#include <string>
#include <iostream>
#include "bakkesmod/plugin/bakkesmodplugin.h"
#include "bakkesmod/plugin/pluginwindow.h"
#include "bakkesmod/plugin/PluginSettingsWindow.h"
#include "Definitions.cpp"

#include "version.h"
#include <windows.h>
#include <chrono>
#include <thread>

#define ARDUINO_WAIT_TIME 2000
#define MAX_DATA_LENGTH 12



constexpr auto plugin_version = stringify(VERSION_MAJOR) "." stringify(VERSION_MINOR) "." stringify(VERSION_PATCH) "." stringify(VERSION_BUILD);

//template <typename T, typename std::enable_if<std::is_base_of<ObjectWrapper, T>::value>::type*>
//
//void GameWrapper::HookEventWithCallerPost(std::string eventName,
//
////THIS CALLS ALL FUNCTIONS! BE CAREFUL WITH FUNCTION NAMES! CRASHES IF YOU FUCK IT UP!
//std::function<void(T caller, void* params, std::string eventName)> callback)
//{
//	auto wrapped_callback = [callback](ActorWrapper caller, void* params, std::string eventName)
//	{
//		callback(T(caller.memory_address), params, eventName);
//	};
//	HookEventWithCaller<ActorWrapper>(eventName, wrapped_callback);
//}

class DataGatheringRL: public BakkesMod::Plugin::BakkesModPlugin, public BakkesMod::Plugin::PluginSettingsWindow/*, public BakkesMod::Plugin::PluginWindow*/
{
public:

	//Constructor and Loading
	virtual void	onLoad();
	virtual void	onUnload();

	//data gathering
	void			setPlayerName(std::string name);
	void			HookEvent();
	void			sendToArduino();

	//Arduino connection
	void			connectToSerialPort();
	void			disconnectFromSerialPort();
	void			SerialPort(const char* portName);
	int				readSerialPort(char* buffer, unsigned int buf_size);
	bool			writeSerialPort(char* buffer, unsigned int buf_size);
	bool			isConnected();
	
	// Inherited via PluginSettingsWindow
	
	void			RenderSettings() override;
	std::string		GetPluginName() override;
	void			SetImGuiContext(uintptr_t ctx) override;

private:

	//variables for data
	//std::vector<std::string> functionList;
	int				boostAmount;
	int				lastBoostAmount;
	char			arduinoString[MAX_DATA_LENGTH];
	char			tempString[MAX_DATA_LENGTH];
	std::string		ownerName;

	

	//variables for imgui
	char			comDevice[10];
	char			connectionEstablished[255];
	

	//to connect to COM port
	
	HANDLE			handler;
	bool			connected;
	COMSTAT			status;
	DWORD			errors;
	int				dataLength = 8;
	char			output[MAX_DATA_LENGTH];
	char			input[MAX_DATA_LENGTH];


	 

	// Inherited via PluginWindow
	/*

	bool isWindowOpen_ = false;
	bool isMinimized_ = false;
	std::string menuTitle_ = "DataGatheringRL";

	virtual void Render() override;
	virtual std::string GetMenuName() override;
	virtual std::string GetMenuTitle() override;
	virtual void SetImGuiContext(uintptr_t ctx) override;
	virtual bool ShouldBlockInput() override;
	virtual bool IsActiveOverlay() override;
	virtual void OnOpen() override;
	virtual void OnClose() override;
	
	*/
};
