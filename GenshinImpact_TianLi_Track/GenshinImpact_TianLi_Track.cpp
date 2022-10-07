#include "pch.h"
#include "GenshinImpact_TianLi_Track.h"
#include "GenshinImpact_TianLi_Track_TaskResult.h"


#include "..\GenshinImpact_TianLi_Resource\GenshinImpact_TianLi_Resource.h"
#pragma comment(lib,"GenshinImpact_TianLi_Resource.lib")

GenshinImpact_TianLi_Track::GenshinImpact_TianLi_Track()
{
	track_manage = new TrackManage();
}

GenshinImpact_TianLi_Track::~GenshinImpact_TianLi_Track()
{
	StopServer();
	
	if (track_manage != nullptr)
		delete track_manage;
}

void GenshinImpact_TianLi_Track::StartServer()
{
	track_manage->Start();
}

void GenshinImpact_TianLi_Track::StopServer()
{
	track_manage->Stop();
}

bool GenshinImpact_TianLi_Track::ServerState()
{
	return track_manage->GetIsRun();
}

TianLi::Track::TrackResult& GenshinImpact_TianLi_Track::GetResult()
{
	return track_manage->GetResult();
}

void GenshinImpact_TianLi_Track::SetConfig(TianLi::Track::TrackConfig& config)
{
	return track_manage->SetConfig(config);
}