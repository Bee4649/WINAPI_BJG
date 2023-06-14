#pragma once

#include <GameEngineBase/GameEngineMath.h>
#include <GameEngineCore/GameEngineActor.h>
#include <string.h>
#include <vector>
#include "ContentsEnum.h"

const int Black = RGB(0, 0, 0);			// �ٴ�, �� ǥ�� ����
const int White = RGB(255, 255, 255);	// �� ���� ǥ�� ����
const int Green = RGB(0, 255, 0);		// �Ʒ����� ����Ǵ� �ٴ� ǥ�� ����
const int Red = RGB(255, 0, 0);			// ��Ż�� ǥ�� ����


class Map	:	public GameEngineActor
{

public:

	Map();
	~Map();

	Map(const Map& _Other) = delete;
	Map(Map&& _Other) noexcept = delete;
	Map& operator=(const Map& _Other) = delete;
	Map& operator=(Map&& _Other) noexcept = delete;

	static Map* MainMap;

	void SetImage(const std::string_view& _BackGroundName, const std::string_view& _StageName, const std::string_view& _StageColName);
	void SetDebugMap(const std::string_view& _DebugMapName);
	void SetStartPos(const std::vector <float4>& _StartPos);
	void MoveMap(int _StartPosIndex);

	inline GameEngineRenderer* GetBackGroundRender()
	{
		return BackGroundRender;
	}

	inline GameEngineRenderer* GetStageRender()
	{
		return StageRender;
	}

	inline std::string_view GetStageColName()
	{
		return StageColName;
	}
	inline float4 GetBackGroundSize()
	{
		return BackGroundSize;
	}
	inline float4 GetStageSize()
	{
		return StageSize;
	}
	inline void BackGroundAnimOn()
	{
		IsBackAnim = true;
	}
	inline void AddStartPos(float4 _Pos)
	{
		_Pos.Y -= 1;
		StartPos.push_back(_Pos);
	}

protected:
	void Update(float _DeltaTime);

private:
	GameEngineRenderer* BackGroundRender = nullptr;
	GameEngineRenderer* StageRender = nullptr;
	GameEngineRenderer* DebugRender = nullptr;

	float4 BackGroundSize = float4::ZERO;
	float4 StageSize = float4::ZERO;

	std::string_view BackGroundName = "";
	std::string_view StageName = "";
	std::string_view StageColName = "";

	bool IsBackAnim = false;

	std::vector <float4> StartPos = std::vector <float4>();


};
