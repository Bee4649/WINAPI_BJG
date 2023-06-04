#pragma once

#include <GameEngineBase/GameEngineMath.h>
#include <GameEngineBase/GameEngineDebug.h>
#include "GameEngineObject.h"
#include <list>
#include <map>
#include <string>
#include <vector>


// 설명 : 화면혹은 장면을 표현합니다.
// 타이틀 장면
// 플레이 장면
// 엔딩 장면
class GameEngineCore;
class GameEngineActor;
class GameEngineRenderer;
class GameEngineCollision;
class GameEngineLevel : public GameEngineObject
{
	friend class GameEngineCollision;
	friend class GameEngineRenderer;
	friend class GameEngineCore;
	
public:
	// constrcuter destructer
	GameEngineLevel();
	virtual ~GameEngineLevel() = 0;

	// delete Function
	GameEngineLevel(const GameEngineLevel& _Other) = delete;
	GameEngineLevel(GameEngineLevel&& _Other) noexcept = delete;
	GameEngineLevel& operator=(const GameEngineLevel& _Other) = delete;
	GameEngineLevel& operator=(GameEngineLevel&& _Other) noexcept = delete;


	static void DebugRenderSwitch()
	{
		IsDebugRender = !IsDebugRender;
	}

	float4 GetMousePos();
	float4 GetMousePosToCamera();

	
	/// <summary>
	/// 액터를 만드는 함수
	/// </summary>
	/// <typeparam name="ActorType"> GameEngineActor를 상속받은 클래스 타입 </typeparam>
	/// <param name="_Order"> Actor의 업데이트 순서 숫자가 작을수록 먼저 업데이트 </param>
	template<typename ActorType, typename EnumType>
	ActorType* CreateActor(EnumType _Order)
	{
		return CreateActor<ActorType>(static_cast<int>(_Order));
	}

	template<typename ActorType>
	ActorType* CreateActor(int _Order = 0)
	{

		GameEngineActor* NewActor = new ActorType();
		ActorStart(NewActor, _Order);
		Actors.push_back(NewActor);

		return dynamic_cast<ActorType*>(NewActor);
	}

	void SetCamerMove(const float4& _MoveValue)
	{
		CameraPos += _MoveValue;
	}

	void SetCamerPos(const float4& _CameraPos)
	{
		CameraPos += _CameraPos;
	}

	float4 GetCameraPos()
	{
		return CameraPos;
	}

	template<typename ConvType>
	std::vector<ConvType*> GeteCovertActors(int _GroupIndex)
	{
		std::vector<ConvType*> Result;
		std::list<GameEngineActor*>& Group = Actors[_GroupIndex];
		Result.reserve(Group.size());

		for (GameEngineActor* ActorPtr : Group)
		{
			ConvType* ConvPtr = dynamic_cast<ConvType*>(ActorPtr);

			if (nullptr == ConvType)
			{
				MsgAssert("변환할 수 없는 변환입니다.");
			}

			Result.push_back(ConvPtr);
		}

		return Result;
	}

	template<typename EnumType>
	std::vector<GameEngineActor*> GetActors(EnumType _GroupIndex)
	{
		return GetActors(static_cast<int>(_GroupIndex));
	}

	std::vector<GameEngineActor*> GetActors(int _GroupIndex)
	{
		std::vector<GameEngineActor*> Result;

		std::list<GameEngineActor*>& Group = Actors[_GroupIndex];
		Result.reserve(Group.size());

		for (GameEngineActor* ActorPtr : Group)
		{
			Result.push_back(ActorPtr);
		}

		return Result;
	}

	static void DebugTextPush(const std::string& _DebugText)
	{
		DebugTexts.push_back(_DebugText);
	}

	template<typename EnumType>
	void SetTimeScale(EnumType _GroupIndex, float _Time)
	{
		SetTimeScale(static_cast<int>(_GroupIndex), _Time);
	}

	void SetTimeScale(int _GroupIndex, float _Time)
	{
		TimeScales[_GroupIndex] = _Time;
	}

protected:
	virtual void Loading() = 0;
	virtual void Update(float _DeltaTime) = 0;
	
	// 레벨이 시잘될 때 실행
	virtual void LevelChangeStart(GameEngineLevel* _PrevLevel) {}
	// 다른 레벨로 교최될때 실행
	virtual void LevelChangeEnd(GameEngineLevel* _NextLevel) {}

private:
	static bool IsDebugRender;

	float4 CameraPos = float4::ZERO;

	static float4 TextOutStart;
	static std::vector<std::string> DebugTexts;

	// 하나의 자료형으로 모든 화면내에 등장하는 것들을 표현
	std::map<int, std::list<GameEngineActor*>> Actors;

	void ActorStart(GameEngineActor* _Actor, int _Order);

	void ActorsUpdate(float _DeltaTime);
	void ActorsRender(float _DeltaTime);
	void ActorLevelChangeEnd(GameEngineLevel* _NextLevel);
	void ActorLevelChangeStart(GameEngineLevel* _PrevLevel);
	void Release();

	std::map<int, std::list<GameEngineRenderer*>> Renders;
	void PushRender(GameEngineRenderer* _Render, int _ChangeOrder);

	std::map<int, std::list<GameEngineCollision*>> Collisions;
	void PushCollision(GameEngineCollision* _Collision, int _ChangeOrder);

	std::map<int, float> TimeScales;
};

